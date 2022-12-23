#version 330 core


out vec4 FragColor;

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in mat3 o_TNB;

uniform vec3 color;

uniform vec3 cameraPos;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 ambientMaterial;
uniform vec3 diffuseMaterial;
uniform vec3 specularMaterial;
uniform float shininessMaterial;

uniform sampler2D normalTexture;

void main() {

    //Phong avec normal map
    vec3 normalMap = texture(normalTexture, o_uv0).rgb;
    normalMap = normalMap * 2.0 - 1.0;
    normalMap = normalize(o_TNB * normalMap);

    vec3 ambient = ambientMaterial * lightColor;

    vec3 lightDir = normalize(lightPos - o_positionWorld);

    float diff = max(dot(normalMap, lightDir), 0.0);
    vec3 diffuse = (diff * diffuseMaterial) * lightColor;

    vec3 viewDir = normalize(cameraPos-o_positionWorld);
    vec3 reflectDir = reflect(-lightDir, normalMap);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessMaterial);
    vec3 specular = (specularMaterial * spec) * lightColor;

    vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.0);

}
