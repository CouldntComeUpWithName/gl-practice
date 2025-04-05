#version 450 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_uv;

layout(std140, binding = 0) uniform view_info
{
  mat4 projection;
  mat4 view;
};

out vec4 o_color;
out vec2 o_uv;

void main()
{
  vec4 vertex_position = vec4(a_position, 1.0);
  gl_Position = projection * view * mat4(1.0) * vertex_position;
  
  o_color = a_color;
  o_uv = a_uv;
}