#include "core/scene.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "core/camera.h"
#include "core/shader.h"

#include "core/assets.h"

#include "core/gfx.h"
#include "core/file_watcher.h"

#include "core/shader_loader.h"

#include <complex.h>
#include <numbers>
#include <random>
#include <iostream>
#include <limits>

Assets<gfx::shader> shaders;

template<typename T>
using limits = std::numeric_limits<T>;

namespace universe {
  constexpr float gravity = 9.81f;
}

struct SurfaceMesh {
  struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
  };
  std::vector<Vertex> vertices;
  uint32_t vao{};
  uint32_t vbo{};
  uint32_t ebo{};
};

// tweakable
struct SceneSettings {
  uint32_t  dispmap_size       = 512u;
  uint32_t  mesh_size          = 512u;
  float     patch_size         = 20.f;
  uint32_t  furthest_cover     = 8u;
  float     max_coverage       = 64.f;
  glm::vec2 wind_dir           = { -0.4f, -0.9f };
  float     wind_speed         = 6.5f;
  float     amplitude_constant = 0.45f * 1e-3f;
};

struct SceneState {
  GLFWwindow* window;
  uint32_t displacement_map;
  uint32_t initial;
  uint32_t freq;
  uint32_t gradients;
  uint32_t spectrum[2];
  mutable free_camera camera;
  Handle<gfx::shader> surface;
  SurfaceMesh mesh;
  bool fly_mode = false;
};

struct ComputeTask {
  uint32_t id;
};

void on_mouse_move(GLFWwindow* window, double xpos, double ypos)
{
  static float sensitivity = 0.2f;

  SceneState& scene = *(SceneState*)glfwGetWindowUserPointer(window);

  float x = xpos;
  float y = ypos;

  static float last_x = static_cast<float>(x);
  static float last_y = static_cast<float>(y);

  if (!scene.fly_mode)
  {
    return;
  }

  //else 
  //{
  //  x = last_x;
  //  y = last_y;
  //}

  float xoffset = static_cast<float>(x - last_x);
  float yoffset = static_cast<float>(last_y - y);

  last_x = static_cast<float>(x);
  last_y = static_cast<float>(y);

  glm::vec3 rotation = scene.camera.rotation();

  rotation.x += xoffset * sensitivity;
  rotation.y += yoffset * sensitivity;

  if (rotation.y > 89.0f)
  {
    rotation.y = 89.0f;
  }
  if (rotation.y < -89.0f)
  {
    rotation.y = -89.0f;
  }

  scene.camera.rotation(rotation);
}

static float phillips(const glm::vec2& k, const glm::vec2& w, float wind_speed, float amplitude)
{
  float largest_wave  = (wind_speed * wind_speed) / universe::gravity;
  float smallest_wave = largest_wave / 1000.f;

  float kdotw = glm::dot(k, w);
  float k2    = glm::dot(k, k);

  float ph = (std::exp(-1.f / (k2 * largest_wave * largest_wave)) / (k2 * k2 * k2) * (kdotw * kdotw));

  if (kdotw < 0.f) 
  {
    ph *= 0.07f;
  }

  return ph * std::exp(-k2 * smallest_wave * smallest_wave);
}

void on_mouse_button(GLFWwindow* window, int key, int action, int mods)
{
  SceneState& scene = *(SceneState*)glfwGetWindowUserPointer(window);

  switch (key) {
  case GLFW_MOUSE_BUTTON_RIGHT: {
      if (action == GLFW_PRESS)
      {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        scene.fly_mode = true;
      }
      else if (action == GLFW_RELEASE)
      {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        scene.fly_mode = false;
      }
    } break;
  }
}

static SceneState setup(const SceneSettings& settings)
{
  // glDepthFunc(GL_LESS);
  
  std::mt19937 gen;
  std::normal_distribution gaussian(0.f, 1.f);

  GLFWwindow* window = glfwCreateWindow(1280, 720, "fft-water", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, on_mouse_move);
  glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  glfwSetMouseButtonCallback(window, on_mouse_button);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "could not load opengl\n";
  }

  //glEnable(GL_CULL_FACE);
  //glEnable(GL_BACK);

  glEnable(GL_DEPTH_TEST);

  SurfaceMesh mesh;
  
  // generate the actual terrain mesh
  glGenVertexArrays(1, &mesh.vao);
  glBindVertexArray(mesh.vao);

  glCreateBuffers(1, &mesh.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

  glCreateBuffers(1, &mesh.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, SurfaceMesh::Vertex().position.length(), GL_FLOAT, GL_FALSE, sizeof(SurfaceMesh::Vertex), (const void*)(0));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, SurfaceMesh::Vertex().uv.length(), GL_FLOAT, GL_FALSE, sizeof(SurfaceMesh::Vertex), (const void*)offsetof(SurfaceMesh::Vertex, uv));

  // init vertex data
  mesh.vertices.reserve(settings.mesh_size * settings.mesh_size);
  for (int z = 0; z < settings.mesh_size; z++)
  {
    for (int x = 0; x < settings.mesh_size; x++)
    {
      mesh.vertices.push_back(SurfaceMesh::Vertex{
        .position = { x, 0.f, z },
        .uv = {(float)x / (settings.mesh_size - 1), (float)z / (settings.mesh_size - 1)}
      });
    }
  }

  // init index data
  std::vector<uint32_t> indices;
  indices.reserve((((size_t)settings.mesh_size - 1) * ((size_t)settings.mesh_size - 1)) * 6ull);

  for (int z = 0; z < settings.mesh_size - 1; z++)
  {
    for (int x = 0; x < settings.mesh_size - 1; x++)
    {
      uint32_t bottom_left  = z * settings.mesh_size + x;
      uint32_t top_left     = (z + 1) * settings.mesh_size + x;
      uint32_t top_right    = (z + 1) * settings.mesh_size + x + 1;
      uint32_t bottom_right = z * settings.mesh_size + x + 1;

      indices.push_back(bottom_left);
      indices.push_back(top_left);
      indices.push_back(top_right);
      
      indices.push_back(bottom_left);
      indices.push_back(top_right);
      indices.push_back(bottom_right);
    }
  }
  
  glm::vec2 k = {};
  float l = settings.patch_size;
  
  glm::vec2 wn = glm::normalize(settings.wind_dir);

  int32_t startpos = settings.dispmap_size / 2;

  size_t size = ((size_t)settings.dispmap_size + 1) * ((size_t)settings.dispmap_size + 1);
  
  std::vector<std::complex<float>> hdata(size);
  hdata.resize(size);
  
  std::vector<float> wdata;
  wdata.resize(size);
  
  for (uint32_t j = 0; j <= settings.dispmap_size; j++)
  {
    k.y = (std::numbers::pi * 2) * float(startpos - j) / l;
    for (uint32_t i = 0; i <= settings.dispmap_size; i++)
    {
      k.x = (std::numbers::pi * 2) * float(startpos - i) / l;

      auto index = j * (settings.dispmap_size + 1) + i;
      
      float sqrt_p_h = (k.x || k.y) ? std::sqrt(phillips(k, wn, settings.wind_speed, settings.amplitude_constant)) : 0.f;
      
      float one_over_sqrt2 = float(1.0 / glm::sqrt(2.0));

      hdata[index].real((float)(sqrt_p_h * gaussian(gen) * one_over_sqrt2));
      hdata[index].imag((float)(sqrt_p_h * gaussian(gen) * one_over_sqrt2));

      wdata[index] = std::sqrt(universe::gravity * sqrtf(k.x * k.x + k.y * k.y));
    }
  }

  uint32_t initial = {};
  uint32_t freq = {};

  glGenTextures(1, &initial);
  glBindTexture(GL_TEXTURE_2D, initial);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, settings.dispmap_size + 1, settings.dispmap_size + 1);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, settings.dispmap_size + 1, settings.dispmap_size + 1, GL_RG, GL_FLOAT, hdata.data());

  glGenTextures(1, &freq);
  glBindTexture(GL_TEXTURE_2D, freq);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, settings.dispmap_size + 1, settings.dispmap_size + 1);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, settings.dispmap_size + 1, settings.dispmap_size + 1, GL_RED, GL_FLOAT, wdata.data());

  uint32_t spectrum[2]{};

  glGenTextures(1, &spectrum[0]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, settings.dispmap_size, settings.dispmap_size);
  glGenTextures(1, &spectrum[1]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, settings.dispmap_size, settings.dispmap_size);

  uint32_t tempdata{};
  glGenTextures(1, &tempdata);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, settings.dispmap_size, settings.dispmap_size);

  uint32_t displacement;
  glGenTextures(1, &displacement);
  glBindTexture(GL_TEXTURE_2D, displacement);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, settings.dispmap_size, settings.dispmap_size);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  uint32_t gradients{};
  // create gradient & folding map
  glGenTextures(1, &gradients);
  glBindTexture(GL_TEXTURE_2D, gradients);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, settings.dispmap_size, settings.dispmap_size);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1 / 2);

  // gfx::shader surface;
  // surface.load("../../examples/fft-water-simulation/res/shaders/surface.vert", "../../examples/fft-water-simulation/res/shaders/surface.frag");
  // Handle<gfx::shader> handle = shaders.add(Guid("Surface_Shader"), std::move(surface));
  
  // file_watcher::watch("../../examples/fft-water-simulation/res/shaders/surface.vert", [handle](auto file) {
  //   auto frag = file;
  //   gfx::shader s;
  //   s.load(file, frag.replace_extension(".frag"));

  //   gfx::shader& shader = shaders.get(handle);
  //   shader = std::move(s);
  // });
  
  // file_watcher::watch("../../examples/fft-water-simulation/res/shaders/surface.frag", [](auto frag) {
  //   auto vert = frag;
  //   gfx::shader s;
  //   s.load(vert.replace_extension(".vert"), frag);

  //   auto handle = shaders.find(Guid("Surface_Shader"));
  //   gfx::shader& shader = shaders.get(handle);
  //   shader = std::move(s);
  // });
  Handle<gfx::shader> handle = shaders.load(Guid("Surface_Shader"), "../../examples/fft-water-simulation/res/shaders/surface.vert", "../../examples/fft-water-simulation/res/shaders/surface.frag");

  glBufferData(GL_ARRAY_BUFFER, sizeof(SurfaceMesh::Vertex) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
  free_camera camera;

  camera.perspective(90.f, 16 / 9.f, 0.01f, 100'000.f);
  camera.position({settings.mesh_size * 0.5f, 100.f, -10.f });

  return SceneState {
    .window = window,
    .initial = initial,
    .freq = freq,
    .camera = camera,
    .surface = handle,
    .mesh = std::move(mesh),
  };
}

void update(SceneState& scene, SceneSettings& settings, float dt)
{
  static float hot_reload = 0.f;

  hot_reload += dt;
  if (hot_reload >= 0.2f)
  {
    file_watcher::scan_for_changes();
    hot_reload = 0.f;
  }

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  static float velocity = 100.f;
  if (glfwGetKey(scene.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(scene.window, true);

  if (glfwGetKey(scene.window, GLFW_KEY_W) == GLFW_PRESS)
    scene.camera.position(scene.camera.position() + (scene.camera.front() * velocity * dt));
  if (glfwGetKey(scene.window, GLFW_KEY_S) == GLFW_PRESS)
    scene.camera.position(scene.camera.position() - (scene.camera.front() * velocity * dt));
  if (glfwGetKey(scene.window, GLFW_KEY_A) == GLFW_PRESS)
    scene.camera.position(scene.camera.position() - (scene.camera.right() * velocity * dt));
  if (glfwGetKey(scene.window, GLFW_KEY_D) == GLFW_PRESS)
    scene.camera.position(scene.camera.position() + (scene.camera.right() * velocity * dt));
}

void render(const SceneState& scene, SceneSettings& settings)
{
  glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  auto& shader = shaders.get(scene.surface);

  shader.bind();
  shader.mat4("VP", scene.camera.projection() * scene.camera.view());
  
  glBindTexture(GL_TEXTURE_2D, scene.initial);

  glBindVertexArray(scene.mesh.vao);

  glDrawElements(GL_TRIANGLES, (settings.mesh_size * settings.mesh_size) * 6, GL_UNSIGNED_INT, nullptr);
}

int main()
{
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  SceneSettings settings;
  SceneState scene = setup(settings);
  
  glfwSetWindowUserPointer(scene.window, &scene);
  
  while(!glfwWindowShouldClose(scene.window))
  {
    update(scene, settings, .016f);
    render(scene, settings);

    glfwSwapBuffers(scene.window);
    glfwPollEvents();
  }

  return 0;
}