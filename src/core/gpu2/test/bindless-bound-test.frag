#version 450

layout(location = 0) in vec2 inUV;

// Bound texture at set 1, binding 0 (non-heap set)
layout(set = 1, binding = 0) uniform sampler2D u_boundTexture;

layout(location = 0) out vec4 outColor;

void main() { outColor = texture(u_boundTexture, inUV); }
