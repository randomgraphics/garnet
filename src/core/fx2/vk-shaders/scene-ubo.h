// Shared between C++17 and GLSL 4.5+.
//
// C++ usage: #include "vk-shaders/scene-ubo.h"
// GLSL usage: #include "scene-ubo.h" (requires GL_GOOGLE_include_directive)
//
// std140 layout (all offsets in bytes):
//   frameCounter             uint  offset   0
//   frameDurationMs          float offset   4
//   environmentLuminanceScale float offset   8
//   numLights                uint  offset  12
//   lights[]                 DirectLightData[16]  offset 16  (16 × 48 = 768 bytes)
//   environmentAmbientFloor float offset 784 (4 bytes) debugging minimum for model IBL
//   trailing padding               offset 788 (12 bytes) std140 block alignment
//   Total: 800 bytes
#pragma once

#ifndef __cplusplus
    // GLSL-only: symbolic constants for array size and light type tags.
    #define MAX_SCENE_LIGHTS             16
    #define SCENE_LIGHT_TYPE_POINT       0
    #define SCENE_LIGHT_TYPE_SPOT        1
    #define SCENE_LIGHT_TYPE_DIRECTIONAL 2
#endif

#ifdef __cplusplus
    #include <cstdint>
    #include <cstddef>
    #include <glm/vec4.hpp>
    #define vec4 glm::vec4
    #define uint uint32_t
namespace GN::fx2::shader {
// Use DirectLight::POINT/SPOT/DIRECTIONAL (from pipeline.h) for the type tags on the C++ side.
static constexpr uint32_t MAX_SCENE_LIGHTS = 16;
#endif

// One direct light entry (48 bytes, std140-compatible).
struct DirectLightData {
    vec4 positionOrDir; // xyz = world-space pos (point/spot) or direction (directional), w = type tag
    vec4 colorAndRange; // xyz = linear RGB × luminous intensity, w = range (0 for directional)
    vec4 coneAngles;    // x = cos(innerConeAngle), y = cos(outerConeAngle), zw = unused
};

// Environment calibration, not camera exposure: multiply linear source RGB by environmentLuminanceScale
// so a neutral source value of 1 represents that many cd/m^2 (nits). Use target/source luminance for a
// measured reference, or 1 for already calibrated maps; arbitrary HDR values alone do not establish nits.
// Set through SharedShaderConstants::set0.envLighting; use a nonnegative value, with 0 disabling sampled IBL (the debugging floor is independent).
// Skybox and prefiltered maps share the source scale. Diffuse maps must contain illuminance / pi so
// their samples can be multiplied by diffuse reflectance without another division by pi.
struct SceneUBO {
    uint            frameCounter;
    float           frameDurationMs;
    float           environmentLuminanceScale;
    uint            numLights;
    DirectLightData lights[MAX_SCENE_LIGHTS];
    // Debug-only minimum in nits after environment scaling; 0 disables. Set envLighting.environmentAmbientFloor
    // to e.g. 5 and increase temporarily when diagnosing black models. Material/AO/exposure still apply.
    float environmentAmbientFloor;
#ifdef __cplusplus
    // Match std140 block rounding; a GLSL float array would instead require 16-byte element stride.
    float padding[3];
#endif
};

#ifdef __cplusplus
    #undef vec4
    #undef uint
static_assert(sizeof(DirectLightData) == 48, "DirectLightData must be 48 bytes");
static_assert(sizeof(SceneUBO) == 800, "SceneUBO must be 800 bytes");
static_assert(offsetof(SceneUBO, frameCounter) == 0);
static_assert(offsetof(SceneUBO, frameDurationMs) == 4);
static_assert(offsetof(SceneUBO, environmentLuminanceScale) == 8);
static_assert(offsetof(SceneUBO, numLights) == 12);
static_assert(offsetof(SceneUBO, lights) == 16);
static_assert(offsetof(SceneUBO, environmentAmbientFloor) == 784);
} // namespace GN::fx2::shader
#endif

#ifndef __cplusplus
// GLSL-only: SceneUBO binding (set 0, binding 0) and environment texture bindings (bindings 2-5).
layout(std140, set = 0, binding = 0) uniform SceneBlock {
    uint            frameCounter;
    float           frameDurationMs;
    float           environmentLuminanceScale;
    uint            numLights;
    DirectLightData lights[MAX_SCENE_LIGHTS];
    // Debug-only minimum in nits after environment scaling; 0 disables. Set envLighting.environmentAmbientFloor
    // to e.g. 5 and increase temporarily when diagnosing black models. Material/AO/exposure still apply.
    float environmentAmbientFloor;
}
u_scene;

layout(set = 0, binding = 2) uniform samplerCube sscSkyboxCubemap;
layout(set = 0, binding = 3) uniform samplerCube sscIrradianceMap;
layout(set = 0, binding = 4) uniform samplerCube sscPrefilteredEnvMap;
layout(set = 0, binding = 5) uniform sampler2D sscBrdfLut;
#endif
