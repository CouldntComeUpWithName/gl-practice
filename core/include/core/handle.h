#pragma once
#include <cstdint>

template<typename T>
class Assets;

template<typename Res>
class Handle {
  friend Assets<Res>;
  using element_type = Res;
  using index_type   = uint16_t;
  using version_type = uint16_t;

public:
  Handle() = default;
  Handle(const Handle&) = default;
  Handle(Handle&&) noexcept = default;
  
  Handle& operator=(const Handle&) = default;
  Handle& operator=(Handle&&) noexcept = default;

  explicit operator bool() const { return valid(); }
  bool valid() const { return index_ != invalid && version_ != 0; }

  friend bool operator==(const Handle lhs, const Handle rhs) noexcept 
  {
    return lhs.index_ == rhs.index_ && lhs.version_ == rhs.version_;
  }

  friend bool operator<(const Handle lhs, const Handle rhs) noexcept 
  {
    return lhs.index_ < rhs.index_;
  }

  const index_type& index() { return index_; }

private:
  Handle(index_type offset, version_type version)
    : index_{offset}, version_{version} 
  { }

private:
  static constexpr index_type invalid = -1;

private:
  index_type index_ = invalid;
  version_type version_ = {};
};
