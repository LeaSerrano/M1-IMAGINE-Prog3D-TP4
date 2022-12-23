#version 330 core


out vec4 FragColor;

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;

uniform sampler2D colorTexture;

void main() {
    
    FragColor = texture(colorTexture, o_uv0);

}
