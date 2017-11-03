#version 330 core

layout (location = 8) in vec3 vertices;
layout (location = 9) in vec3 normals;

out vec3 reflectN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main()
{
  vec4 worldPos = model * vec4(vertices, 1.0);
  vec4 worldNorm = inverse(transpose(model)) * vec4(normals, 1.0);
  reflectN = normalize(reflect(vec3(worldPos) - cameraPos, vec3(worldNorm)));
  reflectN = vec3(worldPos);
  gl_Position = projection * view * worldPos;
}
