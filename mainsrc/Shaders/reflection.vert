#version 330 core

layout (location = 8) in vec3 vertices;
layout (location = 9) in vec3 normals;

out vec3 reflectN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vec3 incidentN = normalize(-vertices);
  vec3 normalN = normalize(normals);
  reflectN = reflect(incidentN, normalN);
  gl_Position = projection * view * model * vec4(vertices, 1.0);
}
