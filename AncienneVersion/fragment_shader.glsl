#version 330 core

uniform sampler2D tex;

// Output data
out vec4 FragColor;
// Ajouter une variable interpolée o_color venant du vertex shader
in vec3 o_color;
in vec2 o_text;
//

void main()
{

    // Mettre à jour la couleur avec la variable interpolée venant du vertex shader
    //FragColor = vec4(1., 0., 0., 1.);// Output color = red

    //L'interpolation se fait toute seule grace au fragment shader
    //FragColor = vec4(o_color, 1.);
    FragColor = texture(tex, o_text);

}
