#extension GL_EXT_nonuniform_qualifier : require
// gpu2's default heap layout: material SSBO at 0, sampled images at 1, samplers at 5.
layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];
vec4 sampleHeap(uint textureIndex, uint samplerIndex, vec2 uv) {
    return texture(sampler2D(heapTextures[nonuniformEXT(textureIndex)], heapSamplers[nonuniformEXT(samplerIndex)]), uv);
}
