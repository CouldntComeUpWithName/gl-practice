#pragma once
#include <cstdint>

namespace std::filesystem {
  class path;
}

enum class texture_type {
  albedo,
  normal,
  specular,
  ambient
};

struct texture {
  uint32_t id;
  texture_type type;
};

struct image {
  uint8_t* data;
  int width;
  int height;
  int channels;
};

texture load_texture(const std::filesystem::path& filepath, const texture_type type = texture_type::albedo);

texture texture_from_image(const image& image);

image load_image(const std::filesystem::path& filepath);
