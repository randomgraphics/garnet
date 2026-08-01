#version 450
#extension GL_GOOGLE_include_directive : require

#include "camera-ubo.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexCoord;

// 128-byte push constant: worldTransform + normalTransform (pre-computed on CPU).
layout(push_constant, std430) uniform PC {
    mat4 worldTransform;
    mat4 normalTransform; // transpose(inverse(worldTransform)); pre-computed to avoid per-vertex mat inverse
}
pc;

void main() {
    vec4 worldPos = pc.worldTransform * vec4(inPosition, 1.0);
    outWorldPos   = worldPos.xyz;
    outNormal     = mat3(pc.normalTransform) * inNormal;
    outTexCoord   = inTexCoord;
    gl_Position   = u_camera.projViewMatrix * worldPos;
}
