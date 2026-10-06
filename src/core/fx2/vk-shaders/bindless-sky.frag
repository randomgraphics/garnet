#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_nonuniform_qualifier : require

#include "bindless-shared-uniforms.h"
#include "bindless-sky-material.h"

layout(push_constant, std430) uniform SkyDraw { uvec4 materialIndex; }
draw;

layout(set = 0, binding = 1) uniform textureCube heapCubemaps[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];

layout(location = 0) in vec3 v_dir;
layout(location = 0) out vec4 outColor;

vec3 gn_tonemap(vec3 radiance, float exposure) {
    vec3 c = radiance * exposure;
    return c / (c + vec3(1.0));
}

void main() {
    uint matIdx = draw.materialIndex.x;
    if (matIdx == 0xFFFFFFFFu) {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    SkyMaterialData material   = skyMaterialHeap.materials[matIdx];
    uint            skyboxTex  = material.textureIndices.x;
    uint            skyboxSamp = material.samplerIndices.x;
    float           lumScale   = material.factors.x;

    vec3 raw = texture(samplerCube(heapCubemaps[nonuniformEXT(skyboxTex)], heapSamplers[nonuniformEXT(skyboxSamp)]), v_dir).rgb * lumScale;
    outColor = vec4(gn_tonemap(raw, sscUniforms.exposure), 1.0);
}
