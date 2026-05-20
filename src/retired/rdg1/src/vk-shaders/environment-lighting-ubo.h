// Shared between C++17 and GLSL 4.5+.
//
// C++ usage:
//   #include "vk-shaders/environment-lighting-ubo.h"
//   // EnvironmentLightingUBO is the CPU-side struct for buffer upload.
//
// GLSL usage:
//   #extension GL_GOOGLE_include_directive : require
//   #include "environment-lighting-ubo.h"
//   // Declares u_environmentLighting (set 0, binding 6) in the header.
//
// std140 layout (16 bytes):
//   environmentRadianceScale  float  offset 0   dimensionless linear multiplier on sampled environment radiance
//   _pad0..2                  float  offset 4   padding to 16-byte vec4 alignment
//
// Physical meaning:
//   Environment cubemap / irradiance / prefiltered maps store linear HDR radiance (or irradiance proportional
//   to radiance) in asset units. Multiplying by environmentRadianceScale applies a linear radiance gain in
//   rendering without rebaking textures. Value 1.0 = use authored calibration; values > 1 brighten IBL and
//   skybox consistently. This is not exposure (display); tonemapping/exposure may still be applied later in the shader.

#ifndef GLSL_ENVIRONMENT_LIGHTING_UBO_H_
#define GLSL_ENVIRONMENT_LIGHTING_UBO_H_

struct EnvironmentLightingUBO {
    float environmentRadianceScale; // linear radiance multiplier [dimensionless], default 1.0
    float _pad0;
    float _pad1;
    float _pad2;
};

#ifndef __cplusplus
layout(std140, set = 0, binding = 6) uniform EnvironmentLightingBlock { EnvironmentLightingUBO data; }
u_environmentLighting;
#endif

#ifdef __cplusplus
    #include <cstddef>
static_assert(sizeof(EnvironmentLightingUBO) == 16, "EnvironmentLightingUBO must be 16 bytes");
static_assert(offsetof(EnvironmentLightingUBO, environmentRadianceScale) == 0);
#endif

#endif // GLSL_ENVIRONMENT_LIGHTING_UBO_H_
