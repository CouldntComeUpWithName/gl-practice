#include <cstdint>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <core/model.h>
#include <core/shader.h>
#include <core/camera.h>

constexpr auto SCREEN_WIDTH  = 1280;
constexpr auto SCREEN_HEIGHT = 720;

void update_mouse(GLFWwindow* window, free_camera& camera, float dt)
{
  double x, y;
  
  glfwGetCursorPos(window, &x, &y);
  static float sensitivity = 0.2f;

  static float last_x = static_cast<float>(x);
  static float last_y = static_cast<float>(y);

  float xoffset = static_cast<float>(x - last_x);
  float yoffset = static_cast<float>(last_y - y);

  last_x = static_cast<float>(x);
  last_y = static_cast<float>(y);

  glm::vec3 rotation = camera.rotation();
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

  camera.rotation(rotation);
}

void update_keys(GLFWwindow* window, free_camera& camera, float dt)
{
  float static velocity = 40.f;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.position(camera.position() + (camera.front() * velocity * dt));
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.position(camera.position() - (camera.front() * velocity * dt));
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.position(camera.position() - (camera.right() * velocity * dt));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.position(camera.position() + (camera.right() * velocity * dt));
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "hello model", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    return -1;
  }
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);
  // create uniform buffer
  uint32_t ubuffer{};
  glCreateBuffers(1, &ubuffer);
  glNamedBufferData(ubuffer, static_cast<uint32_t>(sizeof(view_info)), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0u, ubuffer);
  
  free_camera camera;
  camera.perspective(45.f, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.f);
  
  glfwSwapInterval(0);
  gfx::shader model_shader;
  model_shader.load("../../res/shaders/surface.vert", "../../res/shaders/surface.frag");
  
  auto start = std::chrono::steady_clock::now();
  model model("E:\\3D\\prehistoric_planet_trex\\scene.gltf");
  
  auto end = std::chrono::steady_clock::now();
  
  std::cout << "Asset loading took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << std::endl;
  
  float angle_accum{ 0.f };
  
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(7/255.f, 11/255.f, 52/255.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    static float dt = 0;
    static float lastFrame = 0;

    float currentFrame = static_cast<float>(glfwGetTime());
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;

    update_keys(window, camera, dt);
    update_mouse(window, camera, dt);
    
    int w{}, h{};
    glfwGetWindowSize(window, &w, &h);

    view_info vi{ camera.projection(), camera.view() };
    glNamedBufferSubData(ubuffer, 0, sizeof(view_info), &vi);
    
    model_shader.bind();
    glm::mat4 model_mat(1.f);

    angle_accum += 1.f * dt;
    
    model_mat = glm::translate(model_mat, {0.f, 0.f, -5.f});
    model_mat = glm::rotate(model_mat, angle_accum, {0.f, 1.f, 0.f});

    model_shader.mat4("model", model_mat);
    model_shader.mat3("normal_mat", glm::transpose(glm::inverse(model_mat)));
    model_shader.vec3("light_dir", glm::vec3(0.f, 5.f, -7.f));
    
    model.draw(model_shader);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
}