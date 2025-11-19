#version 450
layout(location = 0) out vec4 FragColor;

in float height;

void main()
{
  FragColor = vec4(height / 255., height / 255., height / 255., 1.0);
}