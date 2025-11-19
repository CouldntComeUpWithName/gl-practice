#version 450
out vec4 FragColor;

uniform sampler2D tex;

in vec2 o_uv;


void main()
{
  FragColor = texture(tex, o_uv);
  // FragColor = vec4(.0,0.1, 0.0, 1.0);
}