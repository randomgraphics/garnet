#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 1) uniform textureCube heapCubemaps[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];

layout(push_constant, std430) uniform CubeDraw { mat4 model; uint cubemapIndex; uint samplerIndex; uint padding0; uint padding1; }
draw;

layout(location = 0) in vec3 inDirection;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(samplerCube(heapCubemaps[nonuniformEXT(draw.cubemapIndex)], heapSamplers[nonuniformEXT(draw.samplerIndex)]), inDirection);
}
