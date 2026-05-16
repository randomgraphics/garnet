// Shared between C++17 and GLSL 4.5+.
//
// C++ usage: #include "vk-shaders/scene-ubo.h"
// GLSL usage: #include "scene-ubo.h" (requires GL_GOOGLE_include_directive)
//
// std140 layout (all offsets in bytes):
//   frameCounter             uint  offset   0
//   frameDurationMs          float offset   4
//   environmentRadianceScale float offset   8
//   numLights                uint  offset  12
//   lights[]                 DirectLightData[16]  offset 16  (16 × 48 = 768 bytes)
//   Total: 784 bytes
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
namespace GN::rdg2::shader {
// Use DirectLight::POINT/SPOT/DIRECTIONAL (from pipeline.h) for the type tags on the C++ side.
static constexpr uint32_t MAX_SCENE_LIGHTS = 16;
#endif

// One direct light entry (48 bytes, std140-compatible).
struct DirectLightData {
    vec4 positionOrDir; // xyz = world-space pos (point/spot) or direction (directional), w = type tag
    vec4 colorAndRange; // xyz = linear RGB × luminous intensity, w = range (0 for directional)
    vec4 coneAngles;    // x = cos(innerConeAngle), y = cos(outerConeAngle), zw = unused
};

struct SceneUBO {
    uint            frameCounter;
    float           frameDurationMs;
    float           environmentRadianceScale;
    uint            numLights;
    DirectLightData lights[MAX_SCENE_LIGHTS];
};

#ifdef __cplusplus
    #undef vec4
    #undef uint
static_assert(sizeof(DirectLightData) == 48, "DirectLightData must be 48 bytes");
static_assert(sizeof(SceneUBO) == 784, "SceneUBO must be 784 bytes");
static_assert(offsetof(SceneUBO, frameCounter) == 0);
static_assert(offsetof(SceneUBO, frameDurationMs) == 4);
static_assert(offsetof(SceneUBO, environmentRadianceScale) == 8);
static_assert(offsetof(SceneUBO, numLights) == 12);
static_assert(offsetof(SceneUBO, lights) == 16);
} // namespace GN::rdg2::shader
#endif

#ifndef __cplusplus
// GLSL-only: SceneUBO binding (set 0, binding 0) and environment texture bindings (bindings 2-5).
layout(std140, set = 0, binding = 0) uniform SceneBlock {
    uint            frameCounter;
    float           frameDurationMs;
    float           environmentRadianceScale;
    uint            numLights;
    DirectLightData lights[MAX_SCENE_LIGHTS];
}
u_scene;

layout(set = 0, binding = 2) uniform samplerCube sscSkyboxCubemap;
layout(set = 0, binding = 3) uniform samplerCube sscIrradianceMap;
layout(set = 0, binding = 4) uniform samplerCube sscPrefilteredEnvMap;
layout(set = 0, binding = 5) uniform sampler2D sscBrdfLut;
#endif
