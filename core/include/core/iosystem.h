#pragma once
#include<string>
#include<filesystem>

namespace io
{
  using openmode = int32_t;
  std::string read_file(const std::filesystem::path&, openmode mode);
  
  inline std::string read_binary(const std::filesystem::path& path)
  {
    constexpr openmode std_input_binary = 1 | 32; // std::ios::in | std::ios::binary
    return read_file(path, std_input_binary);
  }

}