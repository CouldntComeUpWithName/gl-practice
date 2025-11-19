#pragma once
#include<vector>

namespace std::filesystem {
  class path;
}

struct load_context {
  std::vector<std::filesystem::path> paths;
};

// this struct has to be speciallized for each asset
template<typename T>
struct Loader {
  T load(const load_context&) = delete;
};