#pragma once
#include <vector>
#include <core/mesh.h>
#include <core/fwd.h>
#include <glm/glm.hpp>

struct TerrainMesh {
  struct Vertex {
    glm::vec3 position;
  };
  
  uint32_t vao = {0u};
  uint32_t vbo = {0u};
  uint32_t ebo = {0u};
  std::vector<Vertex> vertices;
};

class TerrainGenerator {
public:
  TerrainGenerator() = default;
  TerrainGenerator(uint32_t width, uint32_t depth)
  : width_{ width }, depth_{ depth }
  { }
  
  void heightmap_from_file(const std::fs::path&);
  void heightmap_from_source(uint8_t* src);
  
  void build_terrain();
  
  const float& height(int x, int z) const { return heightmap_[0]; }
  float& height(int x, int z) { return heightmap_[0]; }
  
  void scale(float scale) { scale_ = scale; }
  float& scale() { return scale_; }
  const float& scale() const { return scale_; }

  void draw();
  
private:
  void populate_buffers();
  void delete_buffers();

private:
  TerrainMesh mesh_;
  uint32_t width_;
  uint32_t depth_;
  std::vector<float> heightmap_;
  float scale_{1.f};
};