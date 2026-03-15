// Shared between C++17 and GLSL 4.5+.
//
// C++ usage:
//   #include "vk-shaders/global-camera-ubo.h"
//   // GlobalCameraUBO is the CPU-side struct for buffer upload.
//
// GLSL usage:
//   #extension GL_GOOGLE_include_directive : require
//   #include "global-camera-ubo.h"
//   layout(std140, set = 0, binding = 0) uniform GlobalCameraBlock { GlobalCameraUBO data; } u_camera;
//
// std140 layout (all offsets in bytes):
//   viewMatrix       mat4  offset   0  (64 bytes)
//   projMatrix       mat4  offset  64  (64 bytes)
//   viewProjMatrix   mat4  offset 128  (64 bytes)
//   cameraPosition   vec4  offset 192  (16 bytes)  xyz = world pos, w = 1
//   renderTargetSize vec2  offset 208  ( 8 bytes)  width, height in pixels
//   nearPlane        float offset 216  ( 4 bytes)
//   farPlane         float offset 220  ( 4 bytes)
//   frameCounter     uint  offset 224  ( 4 bytes)
//   frameDurationMs  float offset 228  ( 4 bytes)
//   _pad0/_pad1      float offset 232  ( 8 bytes)  padding to 240 bytes
//   Total: 240 bytes
#ifndef GLSL_GLOBAL_CAMERA_UBO_H_
#define GLSL_GLOBAL_CAMERA_UBO_H_

#ifdef __cplusplus
    #include <cstdint>
    #include <glm/mat4x4.hpp>
    #include <glm/vec2.hpp>
    #include <glm/vec4.hpp>
    #define mat4 glm::mat4
    #define vec4 glm::vec4
    #define vec2 glm::vec2
    #define uint uint32_t
#endif

struct GlobalCameraUBO {
    mat4  viewMatrix;       // world → view
    mat4  projMatrix;       // view → clip  (Vulkan: Y-flipped, [0,1] depth)
    mat4  viewProjMatrix;   // projMatrix * viewMatrix
    vec4  cameraPosition;   // xyz = camera world-space position,  w = 1
    vec2  renderTargetSize; // width, height in pixels
    float nearPlane;
    float farPlane;
    uint  frameCounter;
    float frameDurationMs;
    float _pad0;
    float _pad1;
};

#ifdef __cplusplus
    #undef mat4
    #undef vec4
    #undef vec2
    #undef uint

    #include <cstddef>
static_assert(sizeof(GlobalCameraUBO) == 240, "GlobalCameraUBO size must be 240 bytes");
static_assert(offsetof(GlobalCameraUBO, viewMatrix) == 0);
static_assert(offsetof(GlobalCameraUBO, projMatrix) == 64);
static_assert(offsetof(GlobalCameraUBO, viewProjMatrix) == 128);
static_assert(offsetof(GlobalCameraUBO, cameraPosition) == 192);
static_assert(offsetof(GlobalCameraUBO, renderTargetSize) == 208);
static_assert(offsetof(GlobalCameraUBO, nearPlane) == 216);
static_assert(offsetof(GlobalCameraUBO, farPlane) == 220);
static_assert(offsetof(GlobalCameraUBO, frameCounter) == 224);
static_assert(offsetof(GlobalCameraUBO, frameDurationMs) == 228);
#endif

#endif // GLSL_GLOBAL_CAMERA_UBO_H_