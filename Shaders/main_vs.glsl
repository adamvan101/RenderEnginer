#version 150

in vec3 vPos;
//uniform vec3 inColor;
//uniform mat4 trans;
out vec4 color;

void main() {
    //gl_Position = trans * vec4(vPos, 1.0);
    gl_Position = vec4(vPos, 1.0);
    //color = vec4(inColor, 1.0);
    color = vec4(1.0, 0.0, 0.0, 1.0);
}
