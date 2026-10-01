#version 450
#extension GL_GOOGLE_include_directive : require

#include "scene-ubo.h"
#include "camera-ubo.h"
#include "model-material-ubo.h"
#include "cel-shading-ubo.h"

layout(set = 1, binding = 0) uniform sampler2D u_baseColor;
layout(set = 1, binding = 1) uniform sampler2D u_normal;
layout(set = 1, binding = 2) uniform sampler2D u_emissive;
layout(set = 1, binding = 3) uniform sampler2D u_occlusion;
layout(set = 1, binding = 4) uniform sampler2D u_metalRough;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec4 inColor;
layout(location = 0) out vec4 outColor;

void main() {
    uint flags      = uint(u_material.roughnessAlphaWorkflow.z + 0.5);
    vec4 baseSample = texture(u_baseColor, inTexCoord) * u_material.baseColor;
    if ((flags & 2u) != 0u) baseSample *= inColor;
    float alphaCutoff = u_material.roughnessAlphaWorkflow.y;
    if (alphaCutoff > 0.0 && baseSample.a < alphaCutoff) { discard; }

    // Keep pipeline layout 100% compatible with cel-model across all sets and stages
    // (Set 0: u_scene [F], u_camera [V|F], sscIrradianceMap [F]; Set 1: samplers 0..4 [F], u_material [F], u_cel [V|F]).
    vec4 dummy = texture(u_normal, inTexCoord) * 0.0001 + texture(u_emissive, inTexCoord) * 0.0001 + texture(u_occlusion, inTexCoord) * 0.0001 +
                 texture(u_metalRough, inTexCoord) * 0.0001 + texture(sscIrradianceMap, vec3(0.0, 1.0, 0.0)) * 0.0001 +
                 vec4(u_scene.environmentAmbientFloor) * 0.0001 + vec4(u_camera.cameraPosition.x) * 0.0001;

    vec3 lineColor = mix(u_cel.outlineColor.rgb, baseSample.rgb * 0.25, 0.15) + dummy.rgb;
    outColor       = vec4(lineColor, u_cel.outlineColor.a);
}
