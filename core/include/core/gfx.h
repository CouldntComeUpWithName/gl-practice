#pragma once
#include <cstdint>
#include <string>

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

texture load_texture(const std::filesystem::path& filepath, const texture_type type = texture_type::albedo);
