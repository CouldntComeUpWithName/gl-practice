#include "core/file_watcher.h"
#include <filesystem>
#include <unordered_map>

struct file_data {
  file_watcher::callback_type callback;
  std::fs::file_time_type last_write;
};

namespace std {
  template<typename Key, typename Value>
  using umap = unordered_map<Key, Value>;
}

std::umap<std::fs::path, file_data> files;

void file_watcher::watch(const std::fs::path &file, callback_type callback)
{
  if(!files.contains(file))
  {
    files[file] = file_data {
      .callback = std::move(callback),
      .last_write = std::fs::last_write_time(file)
    };
  }
  else
  {
    files[file].callback = std::move(callback);
  }
}

void file_watcher::scan_for_changes()
{
  for (auto& [key, value] : files)
  {
    auto last_write = std::fs::last_write_time(key);
    if (value.last_write < last_write)
    {
      value.callback(key);
      value.last_write = last_write;
    }
  }
}