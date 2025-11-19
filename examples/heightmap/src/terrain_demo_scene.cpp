#include "terrain_demo_scene.hpp"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

struct KeyEvent {
  int key;
  int scancode;
  int action;
  int mods;
};

struct MouseMoveEvent {
  float xpos;
  float ypos;
};

struct MouseButtonEvent {
  int key;
  int action;
  int mods;
};

void TerrainDemo::on_init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(1280, 720, "terrain", nullptr, nullptr);
  glfwMakeContextCurrent(window_);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Could not load OpenGL\n";
  }

  glfwSetWindowUserPointer(window_, this);
   
  set_callbacks();

  terrain_ = TerrainGenerator(0, 0);
  terrain_.heightmap_from_file("../../examples/heightmap/res/textures/heightmaps/heightmap0.png");
  terrain_.scale(1.f);
  terrain_.build_terrain();

  shader_.load("../../examples/heightmap/res/shaders/terrain.vert", "../../examples/heightmap/res/shaders/terrain.frag");
  
  camera_.position({2048.f * 0.5f * 0.5f, 255.f, 0.f});

  camera_.perspective(90, 16/9.f, 0.01f, 100'000.f);

  glEnable(GL_DEPTH_TEST);

  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void TerrainDemo::on_update(float dt)
{
  float static velocity = 40.f;

  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window_, true);

  if(camera_fly_)
  {
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
      camera_.position(camera_.position() + (camera_.front() * velocity * dt));
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
      camera_.position(camera_.position() - (camera_.front() * velocity * dt));
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
      camera_.position(camera_.position() - (camera_.right() * velocity * dt));
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
      camera_.position(camera_.position() + (camera_.right() * velocity * dt));
  }

  imgui_update(dt);
}

void TerrainDemo::on_render()
{
  glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_.bind();
  
  shader_.mat4("VP", camera_.projection() * camera_.view());
  
  terrain_.draw();
 
  imgui_render();
}

void TerrainDemo::on_shutdown()
{
  
}

void TerrainDemo::run()
{
  while (!glfwWindowShouldClose(window_))
  {
    on_update(0.016f);
    on_render();

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void TerrainDemo::on_key_event(const KeyEvent& event)
{
  float static velocity = 40.f;

  switch (event.key) {
  case GLFW_KEY_G:
  {
    if(event.action == GLFW_PRESS)
    {
      if(wireframe_ = !wireframe_)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else 
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    } 
    break;
  }
  };
}

void TerrainDemo::on_mouse_move(const MouseMoveEvent& event)
{
  static float sensitivity = 0.2f;
  
  float x = event.xpos;
  float y = event.ypos;

  static float last_x = static_cast<float>(x);
  static float last_y = static_cast<float>(y);

  
  float xoffset = static_cast<float>(x - last_x);
  float yoffset = static_cast<float>(last_y - y);
  
  last_x = static_cast<float>(x);
  last_y = static_cast<float>(y);

  if(!camera_fly_)
    return;

  glm::vec3 rotation = camera_.rotation();

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

  camera_.rotation(rotation);
}

void TerrainDemo::on_mouse_button(const MouseButtonEvent& event)
{
  switch(event.key)
  {
  case GLFW_MOUSE_BUTTON_RIGHT:
    
    if(event.action == GLFW_PRESS)
    {
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      camera_fly_ = true;
    }
    else if(event.action == GLFW_RELEASE)
    {
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      camera_fly_ = false;
    }
    break;
  }
}

void TerrainDemo::imgui_update(float)
{
  if(camera_fly_)
    return;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  ImGui::Begin("Scene");
    if(ImGui::Checkbox("Wireframe", &wireframe_))
    {
      if(wireframe_)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else 
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    }
    
    if(ImGui::SliderFloat("Map Scale", &terrain_.scale(), 0.1f, 10.f))
    {
      terrain_.build_terrain();
    }

    if(ImGui::Button("Reset Default", {}))
    {
      wireframe_ = false;
      terrain_.scale(1.f);

      terrain_.build_terrain();
    }

  ImGui::End();
}

void TerrainDemo::imgui_render()
{
  if(camera_fly_)
    return;

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TerrainDemo::set_callbacks()
{
  glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    TerrainDemo* self = (TerrainDemo*)glfwGetWindowUserPointer(window);
    self->on_key_event(KeyEvent{key, scancode, action, mods});
  });

  glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
    TerrainDemo* self = (TerrainDemo*)glfwGetWindowUserPointer(window);
    self->on_mouse_move(MouseMoveEvent{(float)xpos, (float)ypos});
  });

  glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int key, int action, int mods) {
    TerrainDemo* self = (TerrainDemo*)glfwGetWindowUserPointer(window);
    self->on_mouse_button(MouseButtonEvent{key, action, mods});
  });

  glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });
}
