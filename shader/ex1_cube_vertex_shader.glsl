#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 o_positionWorld;
out vec3 o_normalWorld;
out vec2 o_uv0;

void main() {
  
  o_positionWorld = vec3(model * vec4(position, 1.0));
  gl_Position = projection * view * vec4(o_positionWorld, 1.0);

  mat3 normalMatrix = mat3(transpose(inverse(model)));
  o_normalWorld = normalMatrix * normal;
  o_uv0 = uv0;
}
