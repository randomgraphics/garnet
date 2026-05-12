#version 450

layout(set = 0, binding = 0) uniform samplerCube uCubemap;

layout(location = 0) in vec3 inDirection;
layout(location = 0) out vec4 outColor;

void main() { outColor = texture(uCubemap, inDirection); }
