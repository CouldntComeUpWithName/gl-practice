#pragma once
#include <functional>
#include "fwd.h"

struct file_watcher {
  using callback_type = std::function<void(const std::fs::path&)>;

  static void watch(const std::fs::path& file, callback_type callback);
  static void watch(callback_type callback);

  static void scan_for_changes();
};