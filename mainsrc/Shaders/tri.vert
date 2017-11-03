#version 330 core
layout (location = 0) in vec3 triCoord;
layout (location = 1) in vec2 triTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  texCoord = triTexCoord;
  gl_Position = projection * view * model * vec4(triCoord, 1.0);
}
