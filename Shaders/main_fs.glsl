#version 150

in vec4 color;
in vec2 texCoord;

uniform sampler2D tex;

out vec4 fColor;

void main() {
    fColor = vec4(texture(tex,texCoord).rgb, 1.0);
}
