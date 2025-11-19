#include "sphere_scene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

void sphere_scene::update_mouse(float dt)
{
  if (focused_)
  {
    double x, y;
    glfwGetCursorPos(window_, &x, &y);

    static float sensitivity = 0.2f;

    static float last_x = static_cast<float>(x);
    static float last_y = static_cast<float>(y);

    float xoffset = static_cast<float>(x - last_x);
    float yoffset = static_cast<float>(last_y - y);

    last_x = static_cast<float>(x);
    last_y = static_cast<float>(y);

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
}

void on_key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  switch (action)
  {
  case GLFW_PRESS:
    static bool toggle = false;
    if (key == GLFW_KEY_F)
    {
      toggle = !toggle;
      if (toggle)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    }
    
    if (key == GLFW_KEY_ESCAPE)
    {
      /*static bool toggle = false;
      if (toggle)
      {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      else
      {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
      toggle = !toggle;*/
    }
  }
}

void sphere_scene::update_keys(float dt)
{
  float static velocity = 4.f;

  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
  {
    focused_ = !focused_;
  }

  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
    camera_.position(camera_.position() + (camera_.front() * velocity * dt));
  if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
    camera_.position(camera_.position() - (camera_.front() * velocity * dt));
  if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
    camera_.position(camera_.position() - (camera_.right() * velocity * dt));
  if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
    camera_.position(camera_.position() + (camera_.right() * velocity * dt));
}

void sphere_scene::on_init()
{
  sphere_mesh_ = make_sphere(32, 32);
  
  surface_shader_.load("../../res/shaders/surface.vert", "../../res/shaders/surface.frag");

  transforms_.reserve(1000);

  glfwSetKeyCallback(window_, on_key_pressed);

  for(int i = 0; i < 10; i++)
    for(int j = 0; j < 10; j++)
      for(int k = 0; k < 10; k++)
      {
        auto& transform = transforms_.emplace_back();
        transform.position = {i * 3, j * 3, k * 3};
        transform.rotation = {0.f, 0.f, 0.f};
        transform.scale = glm::vec3(1.f);
      }
    
  camera_.perspective(45.f, 16.f/9.f, 0.1f, 1000.f);

  glCreateBuffers(1, &ubuffer_);
  glNamedBufferData(ubuffer_, static_cast<uint32_t>(sizeof(view_info)), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0u, ubuffer_);

  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void sphere_scene::on_update(float dt)
{
  update_keys(dt);
  update_mouse(dt);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Scene");
  {
    if (ImGui::Button("hello in console"))
      std::cout << "hello\n";

    if (ImGui::TreeNode("entities"))
    {
      for (int i = 0; i < transforms_.size(); i++)
      {
        ImGui::PushID(i);
        if(ImGui::CollapsingHeader(std::format("Entity {0}:", i).c_str()))
        {
          ImGui::InputFloat3("position", (float*)&transforms_[i].position);
          ImGui::InputFloat3("rotation", (float*)&transforms_[i].rotation);
          ImGui::InputFloat3("scale", (float*)&transforms_[i].scale);
        }
        ImGui::PopID();
      }
      
      ImGui::TreePop();
    }
  }
  ImGui::End();
}

void sphere_scene::on_render()
{
  surface_shader_.bind();
  
  view_info vi{ camera_.projection(), camera_.view() };
  glNamedBufferSubData(ubuffer_, 0, sizeof(view_info), &vi);
  
  for (const auto& transform : transforms_)
  {
    glm::mat4 model = glm::translate(glm::mat4(1.f), transform.position);
    model = glm::scale(model, transform.scale);
    
    surface_shader_.mat4("model", model);
    surface_shader_.mat3("normal_mat", glm::transpose(glm::inverse(model)));
    surface_shader_.vec3("light_pos", { 5.f * 3.f, 0.f, 50.f });
    sphere_mesh_.draw();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void sphere_scene::on_shutdown()
{

}

