#version 330 core

uniform mat4 MVP;
uniform mat4 M;

in vec3 inVert;
in vec2 inUV;
in vec3 inNormal;

out vec3 fragmentNormal;
out vec3 fragmentPosition;

void main()
{
  fragmentNormal = normalize(inVert);
  vec4 worldPosition = M * vec4(inVert,1.0);
  fragmentPosition = worldPosition.xyz;
  gl_Position = MVP * vec4(inVert,1.0);
}
