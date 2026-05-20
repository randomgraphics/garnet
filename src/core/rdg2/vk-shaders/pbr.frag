#version 450
#extension GL_GOOGLE_include_directive : require

#include "scene-ubo.h"
#include "camera-ubo.h"

// Set 1: PBR material textures
layout(set = 1, binding = 0) uniform sampler2D u_albedo;
layout(set = 1, binding = 1) uniform sampler2D u_normal;
layout(set = 1, binding = 2) uniform sampler2D u_emissive;
layout(set = 1, binding = 3) uniform sampler2D u_occlusion;
layout(set = 1, binding = 4) uniform sampler2D u_metalRough;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 0) out vec4 outColor;

const float PI = 3.14159265359;

mat3 buildTBN(vec3 N, vec3 worldPos, vec2 uv) {
    vec3 dpdx  = dFdx(worldPos);
    vec3 dpdy  = dFdy(worldPos);
    vec2 duvdx = dFdx(uv);
    vec2 duvdy = dFdy(uv);
    vec3 T     = normalize(dpdx * duvdy.t - dpdy * duvdx.t);
    vec3 B     = normalize(dpdy * duvdx.s - dpdx * duvdy.s);
    return mat3(T, B, N);
}

vec3 fresnelSchlick(float cos, vec3 F0) { return F0 + (1.0 - F0) * pow(clamp(1.0 - cos, 0.0, 1.0), 5.0); }
vec3 fresnelSchlickR(float cos, vec3 F0, float r) { return F0 + (max(vec3(1.0 - r), F0) - F0) * pow(clamp(1.0 - cos, 0.0, 1.0), 5.0); }

float distGGX(vec3 N, vec3 H, float r) {
    float a = r * r, a2 = a * a, NdH = max(dot(N, H), 0.0), NdH2 = NdH * NdH;
    float d = NdH2 * (a2 - 1.0) + 1.0;
    d       = PI * d * d;
    return (d > 0.0) ? (a2 / d) : 0.0;
}
float geoSGGX(float NdX, float k) { return NdX / (NdX * (1.0 - k) + k); }
float geoSmith(vec3 N, vec3 V, vec3 L, float r) {
    float k = r * r * 0.5;
    return geoSGGX(max(dot(N, V), 0.0), k) * geoSGGX(max(dot(N, L), 0.0), k);
}
vec3 tonemap(vec3 r) {
    const float e = 0.002;
    r *= e;
    return r / (r + vec3(1.0));
}

void main() {
    vec3  baseColor = texture(u_albedo, inTexCoord).rgb;
    vec3  arm       = texture(u_metalRough, inTexCoord).rgb;
    float metallic  = arm.b;
    float roughness = max(arm.g, 0.04);
    float ao        = texture(u_occlusion, inTexCoord).r;
    vec3  emissive  = texture(u_emissive, inTexCoord).rgb;

    vec3 N  = normalize(inNormal);
    vec3 Ns = texture(u_normal, inTexCoord).rgb * 2.0 - 1.0;
    N       = normalize(buildTBN(N, inWorldPos, inTexCoord) * Ns);

    vec3 V  = normalize(u_camera.cameraPosition.xyz - inWorldPos);
    vec3 F0 = mix(vec3(0.04), baseColor, metallic);

    vec3 Lo = vec3(0.0);
    for (uint i = 0u; i < u_scene.numLights && i < uint(MAX_SCENE_LIGHTS); ++i) {
        DirectLightData light = u_scene.lights[i];
        vec3            L     = (uint(light.positionOrDir.w) == uint(SCENE_LIGHT_TYPE_DIRECTIONAL)) ? normalize(-light.positionOrDir.xyz)
                                                                                                    : normalize(light.positionOrDir.xyz - inWorldPos);
        vec3            H     = normalize(V + L);
        float           NdL = max(dot(N, L), 0.0), NdV = max(dot(N, V), 1e-5);
        float           D = distGGX(N, H, roughness), G = geoSmith(N, V, L, roughness);
        vec3            F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
        vec3            spec = (D * G * F) / (4.0 * NdV * NdL + 1e-5);
        vec3            kD   = (vec3(1.0) - F) * (1.0 - metallic);
        Lo += (kD * baseColor / PI + spec) * light.colorAndRange.rgb * NdL;
    }

    // IBL ambient
    float       NdV      = max(dot(N, V), 0.0);
    vec3        Fr       = fresnelSchlickR(NdV, F0, roughness);
    vec3        kDi      = (vec3(1.0) - Fr) * (1.0 - metallic);
    float       envScale = u_scene.environmentRadianceScale;
    vec3        irr      = texture(sscIrradianceMap, N).rgb * envScale;
    vec3        R        = reflect(-V, N);
    const float MAX_LOD  = 4.0;
    vec3        pref     = textureLod(sscPrefilteredEnvMap, R, roughness * MAX_LOD).rgb * envScale;
    vec2        brdf     = texture(sscBrdfLut, vec2(NdV, roughness)).rg;
    Lo += kDi * baseColor * irr * ao + pref * (Fr * brdf.x + brdf.y) * ao;

    if (u_scene.numLights == 0u) Lo += baseColor * 0.03;

    Lo += emissive;
    outColor = vec4(tonemap(Lo), 1.0);
}
