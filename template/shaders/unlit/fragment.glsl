#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in vec3 o_lightPos;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;

struct Material {
    vec3 ambient ;
    vec3 diffuse ;
    vec3 specular ;
    float shininess ;
};
uniform Material material ;

void main() {
    //FragColor = color;
    //FragColor = texture(colorTexture, o_uv0) * color;
    // DEBUG: position
    //FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    // FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    // FragColor = vec4(o_uv0, 1.0);

    // ambient
    vec3 ambient = light.color * material.ambient ;
    // diffuse
    vec3 norm = normalize ( o_normalWorld ) ;
    vec3 lightDir = normalize ( o_lightPos - o_positionWorld ) ;
    float diff = max ( dot ( norm , lightDir ) , 0.0) ;
    vec3 diffuse = lightColor * ( diff * material.diffuse ) ;
    // specular
    vec3 viewDir = normalize ( - FragPos ) ;
    vec3 reflectDir = reflect ( - lightDir , norm ) ;
    float spec = pow ( max ( dot ( viewDir , reflectDir ) , 0.0) , material.shininess ) ;
    vec3 specular = lightColor * ( spec * material.specular ) ;
    vec3 result = ambient + diffuse + specular ;
    FragColor = vec4 ( result , 1.0) ;
}
