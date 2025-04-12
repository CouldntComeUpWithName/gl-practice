#pragma once
#include <glm/glm.hpp>

struct simple_vertex
{
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 uv;
};

struct vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};
