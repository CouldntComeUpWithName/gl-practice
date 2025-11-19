#version 450 

layout(location = 0) in vec3 Position;

uniform mat4 VP;

out float height;

void main() 
{
  gl_Position = VP * vec4(Position, 1.0);

  height = Position.y;
}
