#include "core/model.h"
#include "core/gfx.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <iostream>

void model::load(const std::filesystem::path &path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "Couldn\'t load a model at " << path << " reason: " << importer.GetErrorString() << '\n';
  }
  else
    _process_node(scene->mRootNode, scene, path);
}

void model::draw(gfx::shader& shader)
{
  for (auto& mesh : meshes_)
  {
    mesh.draw(shader);
  }
}

void model::draw()
{
  for(auto& mesh : meshes_)
  {
    mesh.draw();
  }
}

void model::_process_node(aiNode *node, const aiScene *scene, const std::filesystem::path& model_path)
{
  for(int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.emplace_back(_process_mesh(mesh, scene, model_path));
  }

  for(int i = 0; i < node->mNumChildren; i++)
  {
    _process_node(node->mChildren[i], scene, model_path);
  }
}

mesh model::_process_mesh(aiMesh *msh, const aiScene *scene, const std::filesystem::path& model_path)
{
  std::vector<vertex>  vertices;
  std::vector<texture> textures;
  std::vector<uint32_t> indices;

  vertices.reserve(msh->mNumVertices);

  for(int i = 0; i < msh->mNumVertices; i++)
  {
    vertex& vertex = vertices.emplace_back();
    
    const auto& pos = msh->mVertices[i];
    vertex.position = { pos.x, pos.y, pos.z };
    
    if(msh->HasNormals())
    {
      const auto& normal = msh->mNormals[i];
      vertex.normal = glm::normalize(glm::vec3{ normal.x, normal.y, normal.z });
    }

    if (msh->mTextureCoords[0])
    {
      const auto& uvs = msh->mTextureCoords[0][i];
      vertex.uv = { uvs.x, uvs.y };
    }
  }
  
  size_t size = 0;
  for (uint32_t i = 0; i < msh->mNumFaces; i++)
  {
    aiFace face = msh->mFaces[i];
    size += face.mNumIndices;
  }

  indices.reserve(indices.size() + size);
  for(uint32_t i = 0; i < msh->mNumFaces; i++)
  {
    aiFace face = msh->mFaces[i];

    for(uint32_t j = 0; j < face.mNumIndices; j++)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  aiMaterial* material = scene->mMaterials[msh->mMaterialIndex];

  auto load_material_textures = [&textures, parent_path = model_path.parent_path()](const aiMaterial* material, const aiTextureType aitype, const texture_type type) mutable {
    
    for(uint32_t i = 0; i < material->GetTextureCount(aitype); i++)
    {
      aiString name;
      material->GetTexture(aitype, i, &name);
      
      auto texture_path = parent_path / name.C_Str();

      textures.emplace_back(load_texture(texture_path, type));
    }
  };

  load_material_textures(material, aiTextureType_DIFFUSE, texture_type::albedo);
  load_material_textures(material, aiTextureType_SPECULAR, texture_type::specular);
  load_material_textures(material, aiTextureType_HEIGHT, texture_type::normal);
  load_material_textures(material, aiTextureType_AMBIENT, texture_type::ambient);

  return mesh(std::move(vertices), std::move(textures), std::move(indices));
}
