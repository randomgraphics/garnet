#version 450
#extension GL_GOOGLE_include_directive : require

#include "camera-ubo.h"

// E2 rebases all positions against the observing camera before taking the FX2 snapshot, so
// the FX2 cameraPosition is zero and its view matrix carries orientation only.

// Per-draw constants. Must match GN::e2::DrawConstants.
layout(push_constant) uniform DrawBlock {
    mat4 model;
    vec4 baseColor;
}
pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 vWorldPos;
layout(location = 1) out vec3 vWorldNormal;
layout(location = 2) out vec3 vColor;

void main() {
    vec4 worldPos = pc.model * vec4(inPosition, 1.0);
    vWorldPos     = worldPos.xyz;
    // Approximate normal transform with the model's upper-left 3x3. Adequate for the
    // near-uniform box scales this simple world uses.
    vWorldNormal = mat3(pc.model) * inNormal;
    vColor       = pc.baseColor.rgb;
    gl_Position  = u_camera.projViewMatrix * worldPos;
}
