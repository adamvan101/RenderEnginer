#version 150

in vec3 vPos;
in vec3 vNorm;
in vec2 vTex;
//uniform mat4 trans;

out vec2 texCoord;

void main() {
    //gl_Position = trans * vec4(vPos, 1.0);
    gl_Position = vec4(vPos, 1.0);

    texCoord = vTex;
}
