#version 330 core
//A faire
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle
// i.e. a appliquer sur tous les sommets
//---------

uniform float scale;
uniform vec3 translation;

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shader
out vec3 o_color;
out vec2 o_text;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
// A faire : ajouter un attribut de sommet color, contenant les couleurs pour chaque sommet à ( location = 1 )
//------
layout(location = 1) in vec3 normals_color;
layout (location = 2) in vec2 texture;

void main(){

    //Mettre à jour ce code pour appliquer la translation et la mise à l'échelle
    //gl_Position =  vec4(vertexPosition_modelspace,1);

    gl_Position = vec4(vertexPosition_modelspace * scale + translation, 1);

    //Assigner la normale à la variable interpolée color
    //ajouter ici
    o_color = normals_color;
    o_text = texture;
}

