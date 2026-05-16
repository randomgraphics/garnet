#version 450

// Cubemap is now set=1, binding=0 (set 0 is reserved for SSC resources).
layout(set = 1, binding = 0) uniform samplerCube uCubemap;

layout(location = 0) in vec3 inDirection;
layout(location = 0) out vec4 outColor;

void main() { outColor = texture(uCubemap, inDirection); }
