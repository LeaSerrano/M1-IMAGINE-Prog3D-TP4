#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 o_normalWorld;
out vec3 o_positionWorld;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    o_normalWorld = mat3(transpose(inverse(model))) * normal;
    o_positionWorld = vec3(model * vec4(position, 1.0));
    gl_Position = projection * view * model *  vec4(position, 1.0);
}