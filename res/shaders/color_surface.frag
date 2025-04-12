#version 450 core

out vec4 FragColor;

in vec4 o_color;
in vec2 o_uv;

uniform vec3 light_pos;

void main()
{
  FragColor = o_color;
}