#pragma once
#include<string>
#include<filesystem>
#include<glm/fwd.hpp>

namespace gfx
{
  class shader
  {
  public:
    enum class type
    {
      undefined,
      vertex,
      geometry,
      fragment,
    };

    ~shader()
    {
      _delete_program();
    }

    void bind() const;

    shader() = default;

    shader(shader&& shader) noexcept
        : program_(shader.program_)
    {
      shader.program_ = 0;
    }

    shader& operator=(shader&& shader) noexcept
    {
      if (this != &shader)
      {
        _delete_program();
        this->program_ = shader.program_;
        shader.program_ = 0;
      }
      return *this;
    }

    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    template<typename Filepath1, typename Filepath2, typename... Files>
    void load(Filepath1&& f0, Filepath2&& f1, Files&&... files)
    {
      _create_program();

      compile_status results[2 + sizeof...(Files)] = {_filepath(std::forward<Filepath1>(f0)), _filepath(std::forward<Filepath2>(f1)),
          _filepath(std::forward<Files>(files))... };

      for(auto& stage : results)
        _attach(stage.shader);

      auto linkage_status = _link_program();
      

      for(auto& stage : results)
        _cleanup(stage.shader);
    }

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void vec2(const std::string& name, const glm::vec2& value) const;
    void vec2(const std::string& name, float x, float y) const;
    void vec3(const std::string& name, const glm::vec3& value) const;
    void vec3(const std::string& name, float x, float y, float z) const;
    void vec4(const std::string& name, const glm::vec4& value) const;
    void vec4(const std::string& name, float x, float y, float z, float w) const;
    void mat2(const std::string& name, const glm::mat2& mat) const;
    void mat3(const std::string& name, const glm::mat3& mat) const;
    void mat4(const std::string& name, const glm::mat4& mat) const;
    auto id()const { return program_; }

  private:
    using shader_program = uint32_t;

    struct compile_status
    {
      uint32_t shader;
      int32_t status;
      type type;
    };

    compile_status _filepath(const std::filesystem::path& path);
    //compile_status _filepath(const std::string_view path);

    void _attach(uint32_t shader);

    void _cleanup(uint32_t shader);

    int _link_program();

    void _create_program();
    void _delete_program();
  private:
    shader_program program_ = 0;
  };
}