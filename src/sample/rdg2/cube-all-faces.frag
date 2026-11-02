#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];

layout(push_constant, std430) uniform AllFacesDraw { uint textureIndices[6]; uint samplerIndex; }
draw;

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outFace0;
layout(location = 1) out vec4 outFace1;
layout(location = 2) out vec4 outFace2;
layout(location = 3) out vec4 outFace3;
layout(location = 4) out vec4 outFace4;
layout(location = 5) out vec4 outFace5;

void main() {
    outFace0 = texture(sampler2D(heapTextures[nonuniformEXT(draw.textureIndices[0])], heapSamplers[nonuniformEXT(draw.samplerIndex)]), inUV);
    outFace1 = texture(sampler2D(heapTextures[nonuniformEXT(draw.textureIndices[1])], heapSamplers[nonuniformEXT(draw.samplerIndex)]), inUV);
    outFace2 = texture(sampler2D(heapTextures[nonuniformEXT(draw.textureIndices[2])], heapSamplers[nonuniformEXT(draw.samplerIndex)]), inUV);
    outFace3 = texture(sampler2D(heapTextures[nonuniformEXT(draw.textureIndices[3])], heapSamplers[nonuniformEXT(draw.samplerIndex)]), inUV);
    outFace4 = texture(sampler2D(heapTextures[nonuniformEXT(draw.textureIndices[4])], heapSamplers[nonuniformEXT(draw.samplerIndex)]), inUV);
    outFace5 = texture(sampler2D(heapTextures[nonuniformEXT(draw.textureIndices[5])], heapSamplers[nonuniformEXT(draw.samplerIndex)]), inUV);
}
