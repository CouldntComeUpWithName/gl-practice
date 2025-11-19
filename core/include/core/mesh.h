#pragma once
#include "vertex.h"
#include "gfx.h"

#include <vector>
#include "shader.h"

struct render_context
{
  uint32_t vao;
  uint32_t vbo;
  uint32_t ebo;
};

class mesh
{
public:
  mesh() = default;
  mesh(const std::vector<vertex>& v, const std::vector<texture>& t, const std::vector<uint32_t>& indices)
    : vertices_{v}, textures_{t}, indices_{indices}
  { 
    _setup();
  }

  mesh(std::vector<vertex>&& v, std::vector<texture>&& t, std::vector<uint32_t>&& indices)
    : vertices_{ std::move(v) }, textures_{ std::move(t) }, indices_{std::move(indices)}
  {
    _setup();
  }

  mesh(const mesh& mesh) = default;
  mesh(mesh&& mesh) = default;

  mesh& operator=(const mesh& mesh) = default;
  mesh& operator=(mesh&& mesh) = default;

  void draw();
  void draw(gfx::shader& shader);
private:
  void _setup();

private:
  render_context rcontext_ = {};
  
  std::vector<vertex> vertices_;
  std::vector<texture> textures_;
  std::vector<uint32_t> indices_;
};

mesh make_sphere(uint32_t sector, uint32_t stack); 
mesh make_surface(uint32_t vwidth, uint32_t vheight);