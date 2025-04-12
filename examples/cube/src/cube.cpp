#include <cstdint>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <core/vertex.h>
#include <core/camera.h>
#include <core/shader.h>
#include <core/gfx.h>
#include <core/mesh.h>

constexpr auto SCREEN_WIDTH  = 1280;
constexpr auto SCREEN_HEIGHT = 720;

constexpr simple_vertex cube[]{
  {.position = { -0.5f, -0.5f, -0.5f }, .color = {1.f, 0.f, 0.f, 1.f}, .uv = {0.f, 0.f} },
  {.position = {  0.5f, -0.5f, -0.5f }, .color = {1.f, 0.f, 0.f, 1.f}, .uv = {1.f, 0.f} },
  {.position = {  0.5f,  0.5f, -0.5f }, .color = {1.f, 0.f, 0.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = {  0.5f,  0.5f, -0.5f }, .color = {1.f, 0.f, 0.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { -0.5f,  0.5f, -0.5f }, .color = {1.f, 0.f, 0.f, 1.f}, .uv = {0.f, 1.f} },
  {.position = { -0.5f, -0.5f, -0.5f }, .color = {1.f, 0.f, 0.f, 1.f}, .uv = {0.f, 0.f} },

  {.position = { -0.5f, -0.5f, 0.5f }, .color = {0.f, 0.f, 1.f, 1.f}, .uv = {0.f, 0.f} },
  {.position = {  0.5f, -0.5f, 0.5f }, .color = {0.f, 0.f, 1.f, 1.f}, .uv = {1.f, 0.f} },
  {.position = {  0.5f,  0.5f, 0.5f }, .color = {0.f, 0.f, 1.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = {  0.5f,  0.5f, 0.5f }, .color = {0.f, 0.f, 1.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { -0.5f,  0.5f, 0.5f }, .color = {0.f, 0.f, 1.f, 1.f}, .uv = {0.f, 1.f} },
  {.position = { -0.5f, -0.5f, 0.5f }, .color = {0.f, 0.f, 1.f, 1.f}, .uv = {0.f, 0.f} },

  {.position = { -0.5f,  0.5f,  0.5f }, .color = {0.f, 1.f, 0.f, 1.f}, .uv = {0.f, 0.f} },
  {.position = { -0.5f,  0.5f, -0.5f }, .color = {0.f, 1.f, 0.f, 1.f}, .uv = {1.f, 0.f} },
  {.position = { -0.5f, -0.5f, -0.5f }, .color = {0.f, 1.f, 0.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { -0.5f, -0.5f, -0.5f }, .color = {0.f, 1.f, 0.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { -0.5f, -0.5f,  0.5f }, .color = {0.f, 1.f, 0.f, 1.f}, .uv = {0.f, 1.f} },
  {.position = { -0.5f,  0.5f,  0.5f }, .color = {0.f, 1.f, 0.f, 1.f}, .uv = {0.f, 0.f} },

  {.position = { 0.5f,  0.5f,  0.5f }, .color = {1.f, 1.f, 0.f, 1.f}, .uv = {0.f, 0.f} },
  {.position = { 0.5f,  0.5f, -0.5f }, .color = {1.f, 1.f, 0.f, 1.f}, .uv = {1.f, 0.f} },
  {.position = { 0.5f, -0.5f, -0.5f }, .color = {1.f, 1.f, 0.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { 0.5f, -0.5f, -0.5f }, .color = {1.f, 1.f, 0.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { 0.5f, -0.5f,  0.5f }, .color = {1.f, 1.f, 0.f, 1.f}, .uv = {0.f, 1.f} },
  {.position = { 0.5f,  0.5f,  0.5f }, .color = {1.f, 1.f, 0.f, 1.f}, .uv = {0.f, 0.f} },

  {.position = { -0.5f, -0.5f, -0.5f }, .color = {1.f, 1.f, 1.f, 1.f}, .uv = {0.f, 0.f} },
  {.position = { 0.5f,  -0.5f, -0.5f }, .color = {1.f, 1.f, 1.f, 1.f}, .uv = {1.f, 0.f} },
  {.position = { 0.5f,  -0.5f,  0.5f }, .color = {1.f, 1.f, 1.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { 0.5f,  -0.5f,  0.5f }, .color = {1.f, 1.f, 1.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { -0.5f, -0.5f,  0.5f }, .color = {1.f, 1.f, 1.f, 1.f}, .uv = {0.f, 1.f} },
  {.position = { -0.5f, -0.5f, -0.5f }, .color = {1.f, 1.f, 1.f, 1.f}, .uv = {0.f, 0.f} },

  {.position = { -0.5f, 0.5f, -0.5f }, .color = {1.f, 0.4f, 0.f, 1.f}, .uv = {0.f, 0.f} },
  {.position = {  0.5f, 0.5f, -0.5f }, .color = {1.f, 0.4f, 0.f, 1.f}, .uv = {1.f, 0.f} },
  {.position = {  0.5f, 0.5f,  0.5f }, .color = {1.f, 0.4f, 0.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = {  0.5f, 0.5f,  0.5f }, .color = {1.f, 0.4f, 0.f, 1.f}, .uv = {1.f, 1.f} },
  {.position = { -0.5f, 0.5f,  0.5f }, .color = {1.f, 0.4f, 0.f, 1.f}, .uv = {0.f, 1.f} },
  {.position = { -0.5f, 0.5f, -0.5f }, .color = {1.f, 0.4f, 0.f, 1.f}, .uv = {0.f, 0.f} },
};

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

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cube", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    return -1;
  }
  
  glEnable(GL_DEPTH_TEST);

  uint32_t vbo{}, vao{}, ebo{};

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

  glBindVertexArray(vao);

  glVertexAttribPointer(0, simple_vertex{}.position.length(), GL_FLOAT, false, sizeof(simple_vertex), (void*)offsetof(simple_vertex, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, simple_vertex{}.color.length(), GL_FLOAT, false, sizeof(simple_vertex), (void*)offsetof(simple_vertex, color));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, simple_vertex{}.uv.length(), GL_FLOAT, false, sizeof(simple_vertex), (void*)offsetof(simple_vertex, uv));
  glEnableVertexAttribArray(2);
  
  // create uniform buffer
  uint32_t ubuffer{};
  glCreateBuffers(1, &ubuffer);
  glNamedBufferData(ubuffer, static_cast<uint32_t>(sizeof(view_info)), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0u, ubuffer);

  free_camera camera;
  camera.perspective(45.f, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.001f, 100.f);

  gfx::shader surface;
  surface.load("../../res/shaders/color_surface.vert", "../../res/shaders/color_surface.frag");
  
  glfwSwapInterval(0);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  texture texture = load_texture("E:/Programming/LearningOpenGL/resources/textures/pbr/wall/albedo.png", texture_type::albedo);

  glm::vec3 light_position = { 1.f, 2.f, 0.f };

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
    
    view_info vi{ camera.projection(), camera.view() };
    glNamedBufferSubData(ubuffer, 0, sizeof(view_info), &vi);
    
    surface.bind();
    surface.mat4("model", glm::mat4(1.f));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
}