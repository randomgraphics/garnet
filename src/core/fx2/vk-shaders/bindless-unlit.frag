#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_nonuniform_qualifier : require
layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];
vec4 sampleHeap(uint textureIndex, uint samplerIndex, vec2 uv) {
    return texture(sampler2D(heapTextures[nonuniformEXT(textureIndex)], heapSamplers[nonuniformEXT(samplerIndex)]), uv);
}
layout(location = 0) flat in vec4 tint;
layout(location = 1) flat in vec4 emissive;
layout(location = 2) flat in uvec4 textureIndices;
layout(location = 3) in vec2 uv;
layout(location = 0) out vec4 outColor;
void main() {
    vec4 color = tint;
    if ((textureIndices.w & 1u) != 0u) color *= sampleHeap(textureIndices.x, textureIndices.z, uv);
    outColor = vec4(color.rgb + emissive.rgb, color.a);
}
