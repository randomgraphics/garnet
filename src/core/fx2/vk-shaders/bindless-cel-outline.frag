#version 450
#extension GL_GOOGLE_include_directive : require
#include "bindless-cel-material.h"

layout(push_constant, std430) uniform CelDraw {
    mat4  worldFromObject;
    uvec4 materialIndex;
}
draw;

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

void main() {
    CelMaterialData material = celMaterialHeap.materials[draw.materialIndex.x];
    outColor                 = material.outlineColor;
}
