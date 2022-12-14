#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in vec3 o_lightPos;

out vec4 FragColor;

uniform vec4 color;

//Ex1
//uniform sampler2D colorTexture;

//Ex2
uniform vec3 lightColor;
uniform vec3 lightPos;

/*struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}

uniform Material material;*/

/*uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;*/


void main() {
    //Ex1
    //FragColor = texture(colorTexture, o_uv0) * color;

    //Ex2
    //FragColor = color;
    //FragColor = vec4(lightColor, 1.0) * color;

    //ambient 
    /*vec3 ambient = lightColor * material.ambient;

    //diffuse
    vec3 norm = normalize(o_normalWorld);
    vec3 lightDir = normalize(o_lightPos - o_positionWorld);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    //specular
    vec3 viewDir = normalize(-o_positionWorld);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);*/

    
    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * lightColor;

    /*vec4 result = vec4(ambient, 1.0) * color;
    FragColor = result;*/

    vec3 norm = normalize(o_normalWorld);
    vec3 lightDir = normalize(lightPos - o_positionWorld);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    /*vec4 result = vec4((ambient + diffuse), 1.0) * color;
    FragColor = result;*/

    float specularStrength = 0.5;

    vec3 viewDir = normalize(-o_positionWorld);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * color;
    FragColor = result;

}
