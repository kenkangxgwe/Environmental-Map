#version 330 core

out vec4 fragColor;

uniform samplerCube cubeTex;

in vec3 reflectN;

void main()
{
  fragColor = texture(cubeTex, reflectN);
}
