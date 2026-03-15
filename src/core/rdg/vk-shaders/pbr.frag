#version 450
#extension GL_GOOGLE_include_directive : require

/// PBR fragment shader (metallic-roughness).
/// Set 0, binding 1: DirectLightingUBO (direct lights).

#include "direct-lighting-ubo.h"
layout(std140, set = 0, binding = 1) uniform DirectLightingBlock { DirectLightingUBO data; }
u_lighting;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 N         = normalize(inNormal);
    vec3 baseColor = vec3(0.8, 0.8, 0.8);
    vec3 color     = baseColor * 0.15; // ambient

    for (uint i = 0u; i < u_lighting.data.numLights && i < uint(MAX_DIRECT_LIGHTS); ++i) {
        DirectLightData light = u_lighting.data.lights[i];
        vec3            L     = vec3(0.0);
        uint            ltype = uint(light.positionOrDir.w);
        if (ltype == uint(LIGHT_TYPE_DIRECTIONAL)) {
            L = normalize(-light.positionOrDir.xyz);
        } else {
            vec3 toLight = light.positionOrDir.xyz - inWorldPos;
            L            = normalize(toLight);
        }
        float NdotL = max(dot(N, L), 0.0);
        color += baseColor * light.colorAndRange.rgb * NdotL;
    }

    // Fallback: if no lights, use a default directional light from above.
    if (u_lighting.data.numLights == 0u) {
        vec3  L     = normalize(vec3(0.2, -1.0, 0.3));
        float NdotL = max(dot(N, L), 0.0);
        color       = baseColor * (0.15 + vec3(1.0) * NdotL);
    }

    outColor = vec4(color, 1.0);
}
