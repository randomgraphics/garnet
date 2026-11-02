#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];

layout(push_constant, std430) uniform FaceDraw { uint textureIndex; uint samplerIndex; }
draw;

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

void main() { outColor = texture(sampler2D(heapTextures[nonuniformEXT(draw.textureIndex)], heapSamplers[nonuniformEXT(draw.samplerIndex)]), inUV); }
