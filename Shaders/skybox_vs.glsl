#version 150

in vec3 vPos;
uniform mat4 MV;
uniform mat4 Proj;
out vec3 texcoords;

void main () {
    texcoords = vPos;
    gl_Position = Proj * MV * vec4 (vPos, 1.0);
}