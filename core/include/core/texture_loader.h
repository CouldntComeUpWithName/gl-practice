#pragma once

#include"loader.h"
#include"gfx.h"

template<>
struct Loader<texture> {
  texture load(const std::filesystem::path& path, texture_type type) 
  {
    return load_texture(path, type);
  }
};