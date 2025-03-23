#pragma once
#include<string>
#include<filesystem>

class binary_loader
{
public:
  void load(const std::filesystem::path&);
  
  char* data() { return data_.data(); }
  const char* data() const { return data_.data(); }
private:
  std::string data_ = "NoData";
};