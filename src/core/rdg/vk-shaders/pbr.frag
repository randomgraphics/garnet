#version 450
#extension GL_GOOGLE_include_directive : require

/// PBR fragment shader (metallic-roughness) with Cook-Torrance BRDF.
/// Set 0, binding 0: GlobalCameraUBO, binding 1: DirectLightingUBO.
/// Set 1, binding 0: base color texture, binding 1: metallic-roughness (ARM), binding 2: normal map.

#include "global-camera-ubo.h"
#include "direct-lighting-ubo.h"

layout(std140, set = 0, binding = 0) uniform GlobalCameraBlock { GlobalCameraUBO data; }
u_camera;
layout(std140, set = 0, binding = 1) uniform DirectLightingBlock { DirectLightingUBO data; }
u_lighting;

layout(set = 1, binding = 0) uniform sampler2D u_baseColor;
layout(set = 1, binding = 1) uniform sampler2D u_metallicRoughness;
layout(set = 1, binding = 2) uniform sampler2D u_normalMap;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

const float PI = 3.14159265359;

// Build TBN from world position and normal (approximate tangent from derivatives).
mat3 buildTBN(vec3 N, vec3 worldPos, vec2 uv) {
    vec3 dpdx  = dFdx(worldPos);
    vec3 dpdy  = dFdy(worldPos);
    vec2 duvdx = dFdx(uv);
    vec2 duvdy = dFdy(uv);
    vec3 T     = normalize(dpdx * duvdy.t - dpdy * duvdx.t);
    vec3 B     = normalize(dpdy * duvdx.s - dpdx * duvdy.s);
    return mat3(T, B, N);
}

// Fresnel-Schlick: F0 = mix(0.04, baseColor, metallic) for dielectrics/metals.
vec3 fresnelSchlick(float cosTheta, vec3 F0) { return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0); }

// GGX/Trowbridge-Reitz NDF.
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom  = NdotH2 * (a2 - 1.0) + 1.0;
    denom        = PI * denom * denom;
    return (denom > 0.0) ? (a2 / denom) : 0.0;
}

// Schlick-GGX geometry (Smith).
float geometrySchlickGGX(float NdotX, float k) { return NdotX / (NdotX * (1.0 - k) + k); }

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float k     = (roughness * roughness) * 0.5; // k = (roughness+1)^2/8 for direct; use roughness^2/2 for remapped
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return geometrySchlickGGX(NdotV, k) * geometrySchlickGGX(NdotL, k);
}

void main() {
    vec3  baseColor = texture(u_baseColor, inTexCoord).rgb;
    vec3  arm       = texture(u_metallicRoughness, inTexCoord).rgb;
    float metallic  = arm.b;
    float roughness = max(arm.g, 0.04);

    vec3 N               = normalize(inNormal);
    vec3 normalMapSample = texture(u_normalMap, inTexCoord).rgb;
    vec3 tangentNormal   = normalMapSample * 2.0 - 1.0;
    mat3 TBN             = buildTBN(N, inWorldPos, inTexCoord);
    N                    = normalize(TBN * tangentNormal);

    vec3 V  = normalize(u_camera.data.cameraPosition.xyz - inWorldPos);
    vec3 F0 = mix(vec3(0.04), baseColor, metallic);

    vec3 Lo = baseColor * 0.03; // ambient

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
        vec3  H     = normalize(V + L);
        float NdotL = max(dot(N, L), 0.0);

        vec3 radiance = light.colorAndRange.rgb;

        // Cook-Torrance specular: (D * G * F) / (4 * NdotV * NdotL)
        float NdotV = max(dot(N, V), 1e-5);
        float D     = distributionGGX(N, H, roughness);
        float G     = geometrySmith(N, V, L, roughness);
        vec3  F     = fresnelSchlick(max(dot(H, V), 0.0), F0);
        vec3  spec  = (D * G * F) / (4.0 * NdotV * NdotL + 1e-5);

        // Lambert diffuse; metals have no diffuse.
        vec3 kD      = (vec3(1.0) - F) * (1.0 - metallic);
        vec3 diffuse = kD * baseColor / PI;

        Lo += (diffuse + spec) * radiance * NdotL;
    }

    if (u_lighting.data.numLights == 0u) {
        vec3  L     = normalize(vec3(0.2, -1.0, 0.3));
        float NdotL = max(dot(N, L), 0.0);
        Lo          = baseColor * (0.03 + 0.97 * NdotL);
    }

    outColor = vec4(baseColor, 1.0);
}
