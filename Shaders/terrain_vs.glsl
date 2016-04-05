#version 150

in vec3 vPos;
in vec3 vNorm;
in vec2 vTex;
//uniform mat4 trans;

out vec2 texCoord;
out vec4 color;

uniform sampler2D heights;

uniform mat4 ModelView;
//uniform vec4 LightPosition;
uniform mat4 Projection;

void main() {
    gl_Position = vec4(vPos, 1.0); 
    //gl_Position.y = texture(heights, vPos.xy).r * 100;
    gl_Position = Projection * ModelView * gl_Position;

    //color = vec4(texture(heights, vPos.xy).rgb, 1);
    color = vec4(vPos.xy, 0, 1);

    texCoord = vTex;
}
