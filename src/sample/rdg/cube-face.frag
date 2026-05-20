#version 450

// Source image texture supplied by the CPU per draw call (impl 1, one pass per face).
layout(set = 0, binding = 0) uniform sampler2D uFaceTexture;

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

void main() { outColor = texture(uFaceTexture, inUV); }
