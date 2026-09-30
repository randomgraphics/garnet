#version 450
#extension GL_GOOGLE_include_directive : require

#include "model-material-ubo.h"
#include "cel-shading-ubo.h"

layout(set = 1, binding = 0) uniform sampler2D u_baseColor;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec4 inColor;
layout(location = 0) out vec4 outColor;

void main() {
    vec4 baseSample = texture(u_baseColor, inTexCoord) * u_material.baseColor * inColor;
    float alphaCutoff = u_material.roughnessAlphaWorkflow.y;
    if (alphaCutoff > 0.0 && baseSample.a < alphaCutoff) {
        discard;
    }
    outColor = u_cel.outlineColor;
}
