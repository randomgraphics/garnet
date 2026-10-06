#version 450
#extension GL_GOOGLE_include_directive : require
#include "bindless-shared-uniforms.h"
#include "bindless-pbr-material.h"
layout(push_constant, std430) uniform PbrDraw {
    mat4  worldFromObject;
    uvec4 materialIndex;
}
draw;
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 0) out vec3 worldNormal;
layout(location = 1) flat out vec4 baseColor;
layout(location = 2) flat out vec4 emissive;
layout(location = 3) flat out vec4 factors;
layout(location = 4) flat out uvec4 textureIndices;
layout(location = 5) flat out uvec4 extraIndices;
layout(location = 6) out vec2 uv;
layout(location = 7) out vec3 worldPosition;
void main() {
    PbrMaterialData material = pbrMaterialHeap.materials[draw.materialIndex.x];
    vec4            p        = draw.worldFromObject * vec4(position, 1.0);
    gl_Position              = sscUniforms.projViewMatrix * p;
    worldPosition            = p.xyz;
    worldNormal              = transpose(inverse(mat3(draw.worldFromObject))) * normal;
    baseColor                = material.baseColor;
    emissive                 = material.emissive;
    factors                  = material.factors;
    textureIndices           = material.textureIndices;
    extraIndices             = material.extraIndices;
    uv                       = texcoord;
}
