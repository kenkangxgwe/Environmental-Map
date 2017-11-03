#version 330 core
layout (location = 0) in vec3 cubeCoord;

out vec3 texCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
  texCoord = cubeCoord;
  gl_Position = projection * view * vec4(cubeCoord, 1.0);
}
