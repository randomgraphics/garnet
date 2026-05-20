#version 450
#extension GL_GOOGLE_include_directive : enable

// Camera UBO — set 0, binding 1. Field order must match camera-ubo.h.
#include "camera-ubo.h"

// Model matrix only — view/proj come from the camera UBO above.
layout(push_constant) uniform PC { mat4 model; }
pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in float inFaceIndex; // unused; kept so the VBO layout does not change

layout(location = 0) out vec3 outDirection; // object-space position → cubemap sampling direction

void main() {
    // Use the pre-multiplied projViewMatrix to avoid per-vertex view * proj recomputation.
    gl_Position  = u_camera.projViewMatrix * pc.model * vec4(inPosition, 1.0);
    outDirection = inPosition; // cube centered at origin; vertex position = sample direction
}
