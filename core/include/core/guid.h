#pragma once
#include<string_view>

namespace detail {
  constexpr uint64_t hash_code(std::string_view);
}

class Guid {
public:
  constexpr Guid() = default;
  
  constexpr Guid(std::string_view string) 
  {
    *this = Guid::from_string(string);
  }

  static constexpr Guid from_string(std::string_view string)
  {
    Guid guid{};
    guid.repr_ = detail::hash_code(string);
    return guid;
  }

  constexpr bool is_valid() const
  {
    return repr_ != 0;
  }
  
  constexpr uint64_t as_u64() const
  {
    return repr_;
  } 
  friend constexpr bool operator==(const Guid& lhs, const Guid& rhs);
private:
  uint64_t repr_{};
};

constexpr auto sha1(std::string_view string)
{
  using char_type = std::string_view::value_type;

  uint32_t h0 = 0x67452301;
  uint32_t h1 = 0xEFCDAB89;
  uint32_t h2 = 0x98BADCFE;
  uint32_t h3 = 0x10325476;
  uint32_t h4 = 0xC3D2E1F0;

  auto msg_len = string.length() * 8u;
  // Well, I thought about it and decided that I don't need that
}

namespace detail {
    
  template<typename>
  struct fnv1a;

  template<>
  struct fnv1a<std::uint32_t> {
    static constexpr auto offset = 2166136261U;
    static constexpr auto prime  = 16777619U;
  };

  template<>
  struct fnv1a<std::uint64_t> {
    static constexpr auto offset = 1099511628211ULL;
    static constexpr auto prime  = 14695981039346656037ULL;
  };

  constexpr uint64_t hash_code(std::string_view name)
  {
    constexpr auto prime = detail::fnv1a<uint64_t>::prime;
    
    uint64_t hash = detail::fnv1a<uint64_t>::offset;
    for (auto&& c : name)
      hash = (hash ^ static_cast<uint64_t>(c)) * prime;

    return hash;
  }
}

namespace std
{
	// Template specialization for std::swap<Guid>() --
	// See guid.cpp for the function definition

	// Specialization for std::hash<Guid> -- this implementation
	// uses std::hash<std::string> on the stringification of the guid
	// to calculate the hash
	template <>
	struct hash<Guid>
	{
		std::size_t operator()(const Guid& guid) const
		{
			return std::hash<uint64_t>{}(guid.as_u64());
		}
	};
}

constexpr bool operator==(const Guid& lhs, const Guid& rhs) 
{
  return lhs.repr_ == rhs.repr_; 
}
