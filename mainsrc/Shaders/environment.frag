#version 330 core
out vec4 fragColor;

in vec3 texCoord;

uniform samplerCube cubeTex;

void main()
{
  // fragColor = vec4(1.0f,0.0f,0.0f,1.0f);
  fragColor = texture(cubeTex, texCoord);
}
