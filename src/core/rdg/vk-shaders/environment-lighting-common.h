// GLSL-only: SharedShaderConstants Set 0 environment IBL + skybox (bindings 3–6) and sampling helpers.
// Do not include from C++.
//
// Expects: #extension GL_GOOGLE_include_directive : require before this file.
//
// Declares (with environment-lighting-ubo.h):
//   binding 3 = u_irradianceMap (diffuse IBL cubemap)
//   binding 4 = u_prefilteredEnvMap (specular IBL cubemap, mips)
//   binding 5 = u_brdfLut (split-sum BRDF integration, 2D)
//   binding 6 = u_environmentLighting (EnvironmentLightingUBO)
//
// Skybox uses binding 2 (u_skyboxCubemap) in the including shader — declare it before this include.

#ifndef GN_ENVIRONMENT_LIGHTING_COMMON_H_
#define GN_ENVIRONMENT_LIGHTING_COMMON_H_

#include "environment-lighting-ubo.h"

// This cubemap is similar to u_prefilteredEnvMap.mip[0], but usually in much higher resolution
// for high fidelity skybox rendering.
layout(set = 0, binding = 2) uniform samplerCube u_skyboxCubemap;
layout(set = 0, binding = 3) uniform samplerCube u_irradianceMap;
layout(set = 0, binding = 4) uniform samplerCube u_prefilteredEnvMap;
layout(set = 0, binding = 5) uniform sampler2D u_brdfLut;

// Linear radiance multiplier from UBO (dimensionless).
float gn_environmentRadianceScale() { return u_environmentLighting.data.environmentRadianceScale; }

// Apply environment strength to linear RGB radiance (or irradiance map sample proportional to radiance).
vec3 gn_scaleEnvironmentRadiance(vec3 sRGBRadiance) {
    vec3 linearRadiance = pow(sRGBRadiance, vec3(2.2));
    return linearRadiance * gn_environmentRadianceScale();
}

// Skybox: sample cubemap and scale RGB (alpha unchanged). Binding 2 sampler is declared by the including shader.
vec3 gn_sampleSkyboxRadiance(vec3 worldDirection) {
    vec3 c = texture(u_skyboxCubemap, worldDirection).rgb;
    return gn_scaleEnvironmentRadiance(c);
}

// Diffuse IBL: irradiance map (binding 3), scaled by environment radiance multiplier.
vec3 gn_sampleIrradianceMapRadiance(vec3 worldNormal) { return gn_scaleEnvironmentRadiance(texture(u_irradianceMap, worldNormal).rgb); }

// Specular IBL: prefiltered env map (binding 4); scale RGB only.
vec3 gn_samplePrefilteredEnvRadiance(vec3 reflectionDir, float lod) {
    return gn_scaleEnvironmentRadiance(textureLod(u_prefilteredEnvMap, reflectionDir, lod).rgb);
}

// Split-sum BRDF LUT (binding 5): not radiance — no environment scale. xy = (NdotV, roughness).
vec2 gn_sampleBrdfLut(vec2 ndotVRoughness) { return texture(u_brdfLut, ndotVRoughness).rg; }

vec3 gn_linearToSRGB(vec3 lrgb) {
    // // Standard accurate conversion
    // return mix(
    //     12.92 * lrgb,
    //     1.055 * pow(lrgb, vec3(1.0 / 2.4)) - 0.055,
    //     step(0.0031308, lrgb)
    // );
    return pow(lrgb, vec3(1.0 / 2.2));
}

// A simple tonemapping function that transforms radiance in nit to color in sRGB.
vec3 gn_tonemap(vec3 radiance) {
    // Exposure: scale cd/m² into a range where Reinhard's knee is useful.
    // Without this, Lo ≈ 3000 cd/m² would give Lo/(Lo+1) ≈ 1 (blown-out white).
    // exposure ≈ 1/477 so that a white diffuse surface at NdotL=0.5 maps to mid-gray.
    const float exposure    = 0.002;
    vec3        linearColor = radiance * exposure;

    // Reinhard tonemapping: maps [0, ∞) → [0, 1).
    linearColor = linearColor / (linearColor + vec3(1.0));

    // TODO: do sRGB conversion depending on if the render target is sRGB or not

    // done
    return linearColor;
}

#endif // GN_ENVIRONMENT_LIGHTING_COMMON_H_
