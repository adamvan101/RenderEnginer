#version 150

in vec3 vPos;
in vec3 vNorm;
in vec2 vTex;
//uniform mat4 trans;

out vec2 texCoord;

uniform mat4 ModelView;
//uniform vec4 LightPosition;
uniform mat4 Projection;

void main() {
    gl_Position = Projection * ModelView * vec4(vPos, 1.0); 

    texCoord = vTex;
}
