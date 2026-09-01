#version 450
#extension GL_GOOGLE_include_directive : require

#include "scene-ubo.h"

// E2 adapts visual-moment lights into FX2's graph-agnostic render-space scene constants.

layout(location = 0) in vec3 vWorldPos;
layout(location = 1) in vec3 vWorldNormal;
layout(location = 2) in vec3 vColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 n   = normalize(vWorldNormal);
    vec3 lit = vec3(0.04, 0.04, 0.05);

    // Simple Lambert diffuse with photometric inverse-square falloff.
    for (uint i = 0; i < u_scene.numLights; ++i) {
        // The current visual moment exposes point lights. Future moment light variants can
        // select the matching FX2 light type without changing the frame graph.
        vec3  toLight = u_scene.lights[i].positionOrDir.xyz - vWorldPos;
        float dist2   = max(dot(toLight, toLight), 1e-4);
        vec3  l       = toLight * inversesqrt(dist2);
        float ndotl   = max(dot(n, l), 0.0);
        lit += u_scene.lights[i].colorAndRange.rgb * (ndotl / dist2);
    }

    outColor = vec4(vColor * lit, 1.0);
}
