// Shared between C++17 and GLSL 4.5+.
//
// C++ usage:
//   #include "vk-shaders/direct-lighting-ubo.h"
//   // DirectLightingUBO is the CPU-side struct for buffer upload.
//
// GLSL usage:
//   #extension GL_GOOGLE_include_directive : require
//   #include "direct-lighting-ubo.h"
//   layout(std140, set = 0, binding = 1) uniform DirectLightingBlock { DirectLightingUBO data; } u_lighting;
//
// DirectLightData std140 layout (48 bytes per light):
//   positionOrDir  vec4  offset  0   xyz = world-space position (point/spot) or direction (directional), w = type
//   colorAndRange  vec4  offset 16   xyz = linear-space RGB × intensity,  w = range (0 for directional)
//   coneAngles     vec4  offset 32   x = cos(innerConeAngle),  y = cos(outerConeAngle),  zw = unused
//
// DirectLightingUBO std140 layout:
//   numLights  uint  offset   0  (4 bytes)
//   _pad       3×f32 offset   4  (12 bytes, padding array start to 16-byte boundary)
//   lights[]   DirectLightData[MAX_DIRECT_LIGHTS]  offset 16  (16 × 48 = 768 bytes)
//   Total: 784 bytes

#ifndef GLSL_DIRECT_LIGHTING_UBO_H_
#define GLSL_DIRECT_LIGHTING_UBO_H_

#ifdef __cplusplus
    #include <cstdint>
    #include <glm/vec4.hpp>
    #define vec4 glm::vec4
    #define uint uint32_t
#endif

// Maximum number of direct lights supported per frame.
#define MAX_DIRECT_LIGHTS 16

// Light type tag stored in positionOrDir.w.
#define LIGHT_TYPE_POINT       0
#define LIGHT_TYPE_SPOT        1
#define LIGHT_TYPE_DIRECTIONAL 2

struct DirectLightData {
    vec4 positionOrDir; // xyz = world-space pos (point/spot) or direction (directional),  w = type
    vec4 colorAndRange; // xyz = linear RGB × luminous intensity [cd or lx],  w = range (0 for directional)
    vec4 coneAngles;    // x = cos(innerConeAngle),  y = cos(outerConeAngle),  zw = unused
};

struct DirectLightingUBO {
    uint            numLights;
    float           _pad0;
    float           _pad1;
    float           _pad2;
    DirectLightData lights[MAX_DIRECT_LIGHTS];
};

#ifdef __cplusplus
    #undef vec4
    #undef uint

    #include <cstddef>
static_assert(sizeof(DirectLightData) == 48, "DirectLightData must be 48 bytes");
static_assert(sizeof(DirectLightingUBO) == 784, "DirectLightingUBO must be 784 bytes");
static_assert(offsetof(DirectLightingUBO, numLights) == 0);
static_assert(offsetof(DirectLightingUBO, lights) == 16);
#endif

#endif // GLSL_DIRECT_LIGHTING_UBO_H_