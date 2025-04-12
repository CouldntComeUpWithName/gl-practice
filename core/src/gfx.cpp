#include "core/gfx.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <filesystem>

// temp
#include <unordered_map>

std::unordered_map<std::filesystem::path, uint32_t> texture_cache;

texture load_texture(const std::filesystem::path& filepath, const texture_type type)
{
  if (auto it = texture_cache.find(filepath); it != texture_cache.end())
  {
    return texture{ it->second, type };
  }
  
  //stbi_set_flip_vertically_on_load(true);
  
  int width, height, channels;
  uint8_t* data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
  
  texture texture {0, type};
  glGenTextures(1, &texture.id);

  if(data)
  {
    GLenum format = GL_RGBA;

    switch(channels)
    {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    }

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else 
  {
    std::cout << "texture failed to load at path: " << filepath << std::endl;
  }

  stbi_image_free(data);
  
  texture_cache.emplace(std::pair{filepath, texture.id});

  return texture;
}