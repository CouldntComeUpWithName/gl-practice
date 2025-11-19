#include "terrain_generator.hpp"
#include <core/gfx.h>

#include <glad/glad.h>

#include <iostream>

void TerrainGenerator::heightmap_from_file(const std::fs::path& filepath)
{
  image img = load_image(filepath);

  width_ = img.width;
  depth_ = img.height;
  
  heightmap_from_source(img.data);

  free(img.data);
}

void TerrainGenerator::heightmap_from_source(uint8_t* src)
{
  size_t size = (size_t)width_ * (size_t)depth_;
  heightmap_.reserve(size);

  for (uint64_t z = 0; z < depth_; z++)
  {
    for (uint64_t x = 0; x < width_; x++)
    {
      heightmap_.push_back(src[(z * width_ + x) * 3]);
    }
  }
}
//TODO: refactor
void TerrainGenerator::build_terrain()
{
  if(mesh_.vao)
  {
    glDeleteBuffers(1, &mesh_.vao);
  }
  if(mesh_.vbo)
  {
    glDeleteBuffers(1, &mesh_.vbo);
  }
  mesh_.vertices.resize(0);
  
  // generate the actual terrain mesh
  glGenVertexArrays(1, &mesh_.vao);
  glBindVertexArray(mesh_.vao);

  glCreateBuffers(1, &mesh_.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh_.vbo);

  glCreateBuffers(1, &mesh_.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_.ebo);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, TerrainMesh::Vertex().position.length(), GL_FLOAT, GL_FALSE, sizeof(TerrainMesh::Vertex), (const void*)(0));

  // init vertex data
  mesh_.vertices.reserve((size_t)width_ * depth_);
  for (int z = 0; z < depth_; z++)
  {
    for (int x = 0; x < width_; x++)
    {
      float y = heightmap_[(uint64_t)z * width_ + x];
      
      mesh_.vertices.push_back(TerrainMesh::Vertex {
        .position = { x * scale_, y, z * scale_ }
      });
    }
  }

  // init index data
  std::vector<uint32_t> indices;
  indices.reserve((((size_t)depth_ - 1) * ((size_t)width_ - 1)) * 6ull);

  for (int z = 0; z < depth_ - 1; z++)
  {
    for (int x = 0; x < width_ - 1; x++)
    {
      uint32_t bottom_left  = z * width_ + x;
      uint32_t top_left     = (z + 1) * width_ + x;
      uint32_t top_right    = (z + 1) * width_ + x + 1;
      uint32_t bottom_right = z * width_ + x + 1;

      // bl
      // *
      // | \
      // |  \
      // |   \
      // |    \
      // * - - *
      // tl    tr
      
      indices.push_back(bottom_left);
      indices.push_back(top_left);
      indices.push_back(top_right);
      
      indices.push_back(bottom_left);
      indices.push_back(top_right);
      indices.push_back(bottom_right);
    }
  }

  glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainMesh::Vertex) * mesh_.vertices.size(), mesh_.vertices.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void TerrainGenerator::draw()
{
  glBindVertexArray(mesh_.vao);
  glDrawElements(GL_TRIANGLES, (depth_ - 1) * (width_ - 1) * 6, GL_UNSIGNED_INT, nullptr);
}

void TerrainGenerator::populate_buffers()
{

}

void TerrainGenerator::delete_buffers()
{
  if(mesh_.vao)
  {
    glDeleteBuffers(1, &mesh_.vao);
  }
  if(mesh_.vbo)
  {
    glDeleteBuffers(1, &mesh_.vbo);
  }
  if(mesh_.ebo)
  {
    glDeleteBuffers(1, &mesh_.ebo);
  }
}
