#pragma once

class scene
{
public:
  scene() = default;
  
  virtual void on_init() = 0;
  virtual void on_update(float dt) = 0;
  virtual void on_debug_update(float dt)
  { }
  virtual void on_render() = 0;
  virtual void on_debug_render()
  { }
  virtual void on_shutdown() = 0;

  virtual ~scene() = default;
};