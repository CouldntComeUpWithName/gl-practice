#version 450 core

out vec4 FragColor;

in vec4 o_color;
in vec2 o_uv;

uniform sampler2D utexture;

void main()
{
  FragColor = o_color;
}