#include "core/shader.h"

#include <core/iosystem.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <iostream>

namespace detail
{
  static const std::unordered_map<std::string, gfx::shader::type> shader_extension_dictionary =
  {
    { ".vert", gfx::shader::type::vertex },   { ".vs", gfx::shader::type::vertex },
    { ".frag", gfx::shader::type::fragment }, { ".fs", gfx::shader::type::fragment },
  };

  gfx::shader::type get_type(const std::string& extension)
  {
    auto type = gfx::shader::type::undefined;
    auto res = shader_extension_dictionary.find(extension);
    
    if (res != shader_extension_dictionary.end())
    {
      type = res->second;
    }

    return type;
  }
  
  GLenum native_shader_type(const gfx::shader::type type)
  {
    switch(type)
    {
    case gfx::shader::type::vertex:
      return GL_VERTEX_SHADER;
    case gfx::shader::type::fragment:
      return GL_FRAGMENT_SHADER;
    case gfx::shader::type::geometry:
      return GL_GEOMETRY_SHADER;
    }
    return GL_NONE;
  }

  std::string to_string(const gfx::shader::type type)
  {   
    switch(type)
    {
    case gfx::shader::type::vertex:
      return "VERTEX";
    case gfx::shader::type::fragment:
      return "FRAGMENT";
    case gfx::shader::type::geometry:
      return "GEOMETRY";
    }
    return "UNDEFINED";
  }
}

void gfx::shader::bind() const
{
  glUseProgram(program_);
}

gfx::shader::compile_status gfx::shader::_filepath(const std::filesystem::path& path)
{
  assert(std::filesystem::exists(path) && "file does not exist");
  
  compile_status result{};
  
  result.type = detail::get_type(path.extension().string());

  auto source = io::read_binary(path);

  const char* source_cstring = source.c_str();

  result.shader = glCreateShader(detail::native_shader_type(result.type));
  glShaderSource(result.shader, 1, &source_cstring, nullptr);
  glCompileShader(result.shader);
  
  glGetShaderiv(result.shader, GL_COMPILE_STATUS, &result.status);

  char infoLog[512];
  if(!result.status)
  {
    glGetShaderInfoLog(result.shader, 1024, NULL, infoLog);
    std::cout << path.filename() << "COMPILATION FAILED: " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl; 
  }
  return result;
}

void gfx::shader::_attach(shader_program shader)
{
  glAttachShader(program_, shader);
}

void gfx::shader::_cleanup(shader_program shader)
{
  glDeleteShader(shader);
}

int gfx::shader::_link_program()
{
  int status;
  glLinkProgram(program_);
  glGetProgramiv(program_, GL_LINK_STATUS, &status);
  
  char infoLog[1024]{};
  
  if (!status)
  {
    glGetProgramInfoLog(program_, 1024, NULL, infoLog);
    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << " linking " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
  }

  return status;
}

void gfx::shader::_create_program()
{
  program_ = glCreateProgram();
}

void gfx::shader::_delete_program()
{
  glDeleteProgram(program_);
}

void gfx::shader::set_bool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(program_, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void gfx::shader::set_int(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}
// ------------------------------------------------------------------------
void gfx::shader::set_float(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}
void gfx::shader::vec2(const std::string& name, const glm::vec2& value) const
{
  glUniform2fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void gfx::shader::vec2(const std::string& name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(program_, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void gfx::shader::vec3(const std::string& name, const glm::vec3& value) const
{
  glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void gfx::shader::vec3(const std::string& name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(program_, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void gfx::shader::vec4(const std::string& name, const glm::vec4& value) const
{
  glUniform4fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void gfx::shader::vec4(const std::string& name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(program_, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void gfx::shader::mat2(const std::string& name, const glm::mat2& mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void gfx::shader::mat3(const std::string& name, const glm::mat3& mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void gfx::shader::mat4(const std::string& name, const glm::mat4& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
