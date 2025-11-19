#pragma once
#include "loader.h"

namespace gfx {
  class shader;
}

template<>
struct Loader<gfx::shader> {
  static gfx::shader load(const load_context& ctx);
};