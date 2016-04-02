#version 150

in vec3 vPos;
in vec4 inColor;
uniform mat4 trans;
out vec4 color;

void main() {
    gl_Position = trans * vec4(vPos, 1.0);
    color = inColor;
}
