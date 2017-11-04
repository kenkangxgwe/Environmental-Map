#version 330 core

out vec4 fragColor;

uniform samplerCube cubeTex;

in vec3 reflectN;

void main()
{
	if (reflectN.z > 0) {
	fragColor = vec4(0.3, 0.0, 0.0, 1.0);
	} else {
	fragColor = vec4(0.0, 0.3, 0.0, 1.0);
	}
  // fragColor = texture(cubeTex, reflectN);
}
