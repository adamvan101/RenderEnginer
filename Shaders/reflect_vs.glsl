#version 150

in vec3 vPos;
in vec3 vNorm;
uniform mat4 Proj;
uniform mat4 MV;
out vec3 E;
out vec3 N;

void main () {
  E = vec3 (MV * vec4 (vPos, 1.0));
  N = vec3 (MV * vec4 (vNorm, 0.0));
  gl_Position = Proj * MV * vec4 (vPos, 1.0);
}