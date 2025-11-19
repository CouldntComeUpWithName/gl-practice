#version 450

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 UV;

uniform mat4 VP;

out vec2 o_uv;

void main()
{
  gl_Position = VP * vec4(Position, 1.0);
  o_uv = UV;
}