#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];

layout(push_constant) uniform UiPushConstants {
    layout(offset = 16) uint textureIndex;
    layout(offset = 20) uint samplerIndex;
} pc;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec4 inColor;
layout(location = 0) out vec4 outColor;

void main() { outColor = inColor * texture(sampler2D(heapTextures[nonuniformEXT(pc.textureIndex)], heapSamplers[pc.samplerIndex]), inTexCoord); }
