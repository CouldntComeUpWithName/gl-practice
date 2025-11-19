#include "sphere_scene.h"

#include <cstdint>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr auto SCREEN_WIDTH = 1280;
constexpr auto SCREEN_HEIGHT = 720;

static void glfw_init();
static GLFWwindow* create_main_window(int width, int height, const char* title);

int main()
{
  glfw_init();
  GLFWwindow* window = create_main_window(SCREEN_WIDTH, SCREEN_HEIGHT, "primitives");
  
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  texture t = load_texture("../../res/textures/wall/albedo.png");
  
  sphere_scene scene(window);
  scene.on_init();

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float dt = 0;
    static float lastFrame = 0;

    float currentFrame = static_cast<float>(glfwGetTime());
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;

    scene.on_update(dt);
    scene.on_render();
    
    glfwSwapBuffers(window);

    glfwPollEvents();
  }
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

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    return nullptr;
  }
  glfwSwapInterval(0);

  return window;
}