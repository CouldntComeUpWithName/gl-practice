#pragma once
#include "handle.h"
#include "file_watcher.h"
#include "guid.h"
#include "loader.h"

#include <vector>
#include <unordered_map>

template<typename T>
class Assets {
public:
  using asset_type   = T;
  using handle_type  = Handle<asset_type>;
  using id_type      = Guid;
  using index_type   = handle_type::index_type;
  using version_type = handle_type::version_type;

  template<typename Filepath, typename... Filepaths>
  Handle<T> load(const id_type& key, Filepath&& path, Filepaths&&... paths) noexcept
  {
    auto it = lookup_.find(key);
    if (it != lookup_.end())
    {
      return handle_type{ it->second, registry_[it->second].version };
    }
    
    handle_type handle;
    
    load_context load_ctx{.paths = { path, std::forward<Filepaths>(paths)... }};

    if (!freelist_.empty())
    {
      auto index = freelist_.back();
      // should not be invalid ever
      auto& entry = registry_[index];

      entry = AssetData {
        .asset   = Loader<asset_type>{}.load(load_ctx),
        .meta    = AssetMeta{}, 
        .id      = key,
        .version = uint16_t(entry.version + 1)
      };

      lookup_.emplace(std::make_pair(key, index));
      
      freelist_.resize(freelist_.size() - 1);

      handle = handle_type{ index,  entry.version };
    }
    else
    {
      registry_.emplace_back(AssetData{ Loader<asset_type>{}.load(load_ctx), {}, key, 1});
      
      auto index = static_cast<index_type>(registry_.size() - 1);
      lookup_.emplace(std::make_pair(key, index));
      
      handle = handle_type{ index, 1 };
    }
    
    for(auto& file : load_ctx.paths)
    {
      file_watcher::watch(file, [this, handle, ctx = load_ctx](const std::filesystem::path& path) {
        auto& asset = this->get(handle);
        asset = Loader<asset_type>{}.load(ctx);
      });
    }

    return handle;
  }

  handle_type add(const id_type& key, asset_type&& asset) noexcept
  {
    auto it = lookup_.find(key);

    if (it != lookup_.end())
    {
      return handle_type{ it->second, registry_[it->second].version };
    }

    if (!freelist_.empty())
    {
      auto index = freelist_.back();
      // should not be invalid ever
      auto& entry = registry_[index];

      entry = AssetData{ 
        .asset   = std::forward<asset_type>(asset), 
        .meta    = AssetMeta{}, 
        .id      = key,
        .version = entry.version + 1
      };

      lookup_.emplace(std::make_pair(key, index));
      
      freelist_.resize(freelist_.size() - 1);

      return handle_type{ index,  entry.version };
    }
    else
    {
      registry_.emplace_back(std::move(asset), {}, key, 1);
      
      auto index = static_cast<index_type>(registry_.size() - 1);
      lookup_.emplace(std::make_pair(key, index));
      
      return handle_type{ index, 1};
    }
  }
  
  void remove(const handle_type& handle) noexcept
  {
    if (handle.index_ < registry_.size() && handle.version_ == registry_[handle.index_].version)
    {
      lookup_.erase(handle.id_);
      freelist_.push_back(handle.index_);
    }
  }
  
  void remove(const id_type& id) noexcept
  {
    auto handle = find(id);
    if (handle.valid())
    {
      remove(handle);
    }
  }
  
  Handle<T> find(const id_type& key) noexcept
  {
    auto it = lookup_.find(key);

    if(it != lookup_.end())
      return { it->second, registry_[it->second].version };
    
    return handle_type{};
  }
  
  T& get(const handle_type& handle) noexcept
  {
    assert(handle);
    assert(registry_.at(handle.index_).version == handle.version_);

    return registry_[handle.index_].asset;
  }

  T* try_get(const handle_type& handle) noexcept
  {
    if (!handle)
      return nullptr;

    auto version = registry_[handle.index_].version;
    
    if(version != handle.version_)
      return nullptr;
    
    return &registry_[handle.index_].asset;
  }

private:
  struct AssetMeta {
  #ifndef _NDEBUG  
    
  #endif
  };

  struct AssetData {
    asset_type     asset;
    AssetMeta       meta;
    id_type           id;
    version_type version;
  };

  std::vector<AssetData> registry_;
  std::unordered_map<id_type, index_type> lookup_;
  std::vector<index_type> freelist_;
};