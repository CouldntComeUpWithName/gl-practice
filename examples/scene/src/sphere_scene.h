#pragma once
#include <core/scene.h>

#include <core/mesh.h>
#include <core/camera.h>

class sphere_scene final : public scene {
private:
  struct sphere_transform {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
  };

public:
  sphere_scene() = default;
  sphere_scene(struct GLFWwindow* window)
    : window_{window}
  { }

  void on_init() override;
  void on_update(float) override;
  void on_render() override;
  void on_shutdown() override;

private:
  void update_keys(float dt);
  void update_mouse(float dt);

private:
  gfx::shader surface_shader_;
  mesh sphere_mesh_;
  uint32_t ubuffer_ = 0;
  free_camera camera_;
  std::vector<sphere_transform> transforms_;
  struct GLFWwindow* window_;

  bool focused_ = true;
};