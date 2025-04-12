#pragma once
#include "mesh.h"

namespace std::filesystem {
  class path;
}

class model
{
public:
  model() = default;
  model(const std::filesystem::path& path)
  {
    load(path);
  }

  void load(const std::filesystem::path& path);
  void draw();
  void draw(gfx::shader& shader);
private:
  void _process_node(struct aiNode*, struct aiScene const*, const std::filesystem::path&);
  mesh _process_mesh(struct aiMesh*, struct aiScene const*, const std::filesystem::path&);
private:
  std::vector<mesh> meshes_;
};