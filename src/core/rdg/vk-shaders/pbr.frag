#version 450
#extension GL_GOOGLE_include_directive : require

/// PBR fragment shader (metallic-roughness) with Cook-Torrance BRDF + split-sum IBL.
/// Set 0:  binding 0 = GlobalCameraUBO, binding 1 = DirectLightingUBO,
///         binding 2 = skyboxCubemap (unused here; declared for Set0 layout compatibility),
///         binding 3 = irradianceMap (Lambertian diffuse IBL),
///         binding 4 = prefilteredEnvMap (mip-mapped specular IBL, roughness → mip),
///         binding 5 = brdfLut (split-sum BRDF LUT: NdotV × roughness → scale, bias),
///         bindings 3–6 = IBL maps + EnvironmentLightingUBO (see environment-lighting-common.h).
/// Set 1:  binding 0 = base color, binding 1 = metallic-roughness (ARM), binding 2 = normal map.

#include "global-camera-ubo.h"
#include "direct-lighting-ubo.h"
#include "environment-lighting-common.h"

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

// Fresnel-Schlick with roughness bias (for IBL ambient).
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

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

    vec3 Lo = vec3(0.0); // accumulate direct lighting

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

        vec3 irradiance = light.colorAndRange.rgb;

        // Cook-Torrance specular: (D * G * F) / (4 * NdotV * NdotL)
        float NdotV = max(dot(N, V), 1e-5);
        float D     = distributionGGX(N, H, roughness);
        float G     = geometrySmith(N, V, L, roughness);
        vec3  F     = fresnelSchlick(max(dot(H, V), 0.0), F0);
        vec3  spec  = (D * G * F) / (4.0 * NdotV * NdotL + 1e-5);

        // Lambert diffuse; metals have no diffuse.
        vec3 kD      = (vec3(1.0) - F) * (1.0 - metallic);
        vec3 diffuse = kD * baseColor / PI;

        Lo += (diffuse + spec) * irradiance * NdotL;
    }

    // IBL ambient: split-sum diffuse (irradiance map) + specular (prefiltered env map + BRDF LUT).
    {
        float NdotV  = max(dot(N, V), 0.0);
        vec3  F_ibl  = fresnelSchlickRoughness(NdotV, F0, roughness);
        vec3  kD_ibl = (vec3(1.0) - F_ibl) * (1.0 - metallic);

        // Diffuse IBL: pre-convolved irradiance map (scaled via SharedShaderConstants env radiance multiplier).
        vec3 irradiance  = gn_sampleIrradianceMapRadiance(N);
        vec3 diffuse_ibl = kD_ibl * baseColor * irradiance;

        // Specular IBL: prefiltered env map (scaled); BRDF LUT is dimensionless — gn_sampleBrdfLut (no env scale).
        const float MAX_LOD      = 4.0; // valid for a 512px prefilteredEnvMap (mips 0-4)
        vec3        R            = reflect(-V, N);
        vec3        prefiltColor = gn_samplePrefilteredEnvRadiance(R, roughness * MAX_LOD);
        vec2        brdf         = gn_sampleBrdfLut(vec2(NdotV, roughness));
        vec3        specular_ibl = prefiltColor * (F_ibl * brdf.x + brdf.y);

        Lo += diffuse_ibl + specular_ibl;
    }

    if (u_lighting.data.numLights == 0u) {
        // No direct lights: add a small fallback so geometry is visible even with black IBL maps.
        Lo += baseColor * 0.03;
    }

    // Exposure: scale cd/m² into a range where Reinhard's knee is useful.
    // Without this, Lo ≈ 3000 cd/m² would give Lo/(Lo+1) ≈ 1 (blown-out white).
    // exposure ≈ 1/477 so that a white diffuse surface at NdotL=0.5 maps to mid-gray.
    const float exposure = 0.002;
    Lo *= exposure;

    // Reinhard tonemapping: maps [0, ∞) → [0, 1).
    Lo = Lo / (Lo + vec3(1.0));

    // Gamma correction: convert linear light to sRGB (γ ≈ 2.2).
    // Skip if the render target has a _SRGB format (hardware does it automatically).
    Lo = pow(Lo, vec3(1.0 / 2.2));

    outColor = vec4(Lo, 1.0);
}
