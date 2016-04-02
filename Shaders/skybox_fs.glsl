#version 150

in vec3 texcoords;
uniform samplerCube cube_texture;
uniform vec4 Ambience;
out vec4 fColor;

void main () {
    fColor = texture (cube_texture, texcoords) + Ambience;
}