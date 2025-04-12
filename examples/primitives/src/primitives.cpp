#include <cstdint>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <core/vertex.h>
#include <core/camera.h>
#include <core/shader.h>
#include <core/gfx.h>
#include <core/mesh.h>

constexpr auto SCREEN_WIDTH = 1280;
constexpr auto SCREEN_HEIGHT = 720;

static void glfw_init();
static GLFWwindow* create_main_window(int width, int height, const char* title);
void update_keys(GLFWwindow* window, free_camera& camera, float dt);
void update_mouse(GLFWwindow* window, free_camera& camera, float dt);

int main()
{
  glfw_init();
  GLFWwindow* window = create_main_window(SCREEN_WIDTH, SCREEN_HEIGHT, "primitives");
  
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // create uniform buffer
  uint32_t ubuffer{};
  glCreateBuffers(1, &ubuffer);
  glNamedBufferData(ubuffer, static_cast<uint32_t>(sizeof(view_info)), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0u, ubuffer);

  free_camera camera;
  camera.perspective(45.f, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.001f, 100.f);

  gfx::shader surface;
  surface.load("../../res/shaders/surface.vert", "../../res/shaders/surface.frag");
  
  texture texture = load_texture("../../res/textures/wall/albedo.png");

  mesh sphere = make_sphere(36, 18);
  
  glm::vec3 light_pos = { 0.f, 0.f, 0.f };
  
  float angle_accum = 0;

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(7 / 255.f, 11 / 255.f, 52 / 255.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float dt = 0;
    static float lastFrame = 0;

    float currentFrame = static_cast<float>(glfwGetTime());
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;

    update_keys(window, camera, dt);
    update_mouse(window, camera, dt);

    view_info vi{ camera.projection(), camera.view() };
    glNamedBufferSubData(ubuffer, 0, sizeof(view_info), &vi);
    
    surface.bind();

    glm::mat4 model_mat(1.f);

    model_mat = glm::translate(model_mat, { 0.f, 0.f, -5.f });
    model_mat = glm::rotate(model_mat, angle_accum, { 0.f, 1.f, 0.f });
    angle_accum += 3.f * dt;

    surface.mat4("model", model_mat);
    surface.mat3("normal_mat", glm::transpose(glm::inverse(model_mat)));

    surface.vec3("light_pos", light_pos);

    glBindTextureUnit((int)texture.type, texture.id);
    
    surface.set_int("albedo", (int)texture.type);
    
    sphere.draw();

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
}

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

void on_key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  switch(action)
  {
  case GLFW_PRESS:
    static bool toggle = false;
    if(key == GLFW_KEY_F)
    {
      toggle = !toggle;
      if(toggle)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else 
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    }
  }
}

void update_keys(GLFWwindow* window, free_camera& camera, float dt)
{
  float static velocity = 4.f;

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

static void glfw_init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static GLFWwindow* create_main_window(int width, int height, const char* title)
{
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cube", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  glfwSetKeyCallback(window, on_key_pressed);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    return nullptr;
  }
  glfwSwapInterval(0);

  return window;
}