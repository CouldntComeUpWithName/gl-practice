#pragma once

#include <core/scene.h>
#include <core/camera.h>
#include "terrain_generator.hpp"

class TerrainDemo : public scene {
public:
  void on_init() override;
  void on_update(float dt) override;
  void on_render() override;
  void on_shutdown() override;

  void run();

private:
  void set_callbacks();
  void on_key_event(const struct KeyEvent&);
  void on_mouse_move(const struct MouseMoveEvent&);
  void on_mouse_button(const struct MouseButtonEvent&);

  void imgui_update(float);
  void imgui_render();

private:
  struct GLFWwindow* window_;
  TerrainGenerator terrain_;
  free_camera camera_;
  gfx::shader shader_;
  bool camera_fly_ = false;
  bool wireframe_  = false;
};