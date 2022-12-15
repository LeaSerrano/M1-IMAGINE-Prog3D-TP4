#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in vec3 o_lightPos;
in mat3 TNB;

out vec4 FragColor;

uniform vec4 color;

//Ex1
//uniform sampler2D colorTexture;

//Ex2
uniform vec3 lightColor;
uniform vec3 lightPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

uniform sampler2D normalTexture;


void main() {
    //Ex1
    //FragColor = texture(colorTexture, o_uv0) * color;

    //Ex2
    //Couleur de l'objet
    //FragColor = color;

    //Ajout de la lumière
    //FragColor = vec4(lightColor, 1.0) * color;

    //Phong avec normal map
    /*vec3 normalMap = texture(normalTexture, o_uv0).rgb;
    normalMap = normalize(normalMap * 2.0 - 1.0);*/
    vec3 normalMap = texture(normalTexture, o_uv0).rgb;
    normalMap = normalMap * 2.0 - 1.0;
    normalMap = normalize(TNB * normalMap);

    vec3 ambient = material.ambient * lightColor;

    vec3 lightDir = normalize(lightPos - o_positionWorld);

    float diff = max(dot(normalMap, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * lightColor;

    vec3 viewDir = normalize(-o_positionWorld);
    vec3 reflectDir = reflect(-lightDir, normalMap);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * lightColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * color;
    FragColor = result;

}
