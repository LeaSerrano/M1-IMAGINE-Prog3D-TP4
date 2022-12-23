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
  
 mat3 normalMatrix = mat3(transpose(inverse(model)));
  o_uv0 = uv0;
  
  vec4 positionWorld = model * vec4(position, 1.0);
  o_positionWorld = positionWorld.xyz;
  o_normalWorld = normalMatrix * normal;

  gl_Position = projection * view * positionWorld;

}
