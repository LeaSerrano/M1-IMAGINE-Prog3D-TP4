#version 330 core
out vec4 FragColor;

in vec3 o_normalWorld;
in vec3 o_positionWorld;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(o_positionWorld - cameraPos);
    vec3 R = reflect(I, normalize(o_normalWorld));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}