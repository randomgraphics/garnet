// Shared between C++17 and GLSL 4.5+.
//
// C++ usage: #include "vk-shaders/camera-ubo.h"
// GLSL usage: #include "camera-ubo.h" (requires GL_GOOGLE_include_directive)
//
// std140 layout (all offsets in bytes):
//   viewMatrix       mat4  offset   0  (64 bytes)
//   projMatrix       mat4  offset  64  (64 bytes)
//   projViewMatrix   mat4  offset 128  (64 bytes)
//   cameraPosition   vec4  offset 192  (16 bytes)  xyz = world pos, w = 1
//   renderTargetSize vec2  offset 208  ( 8 bytes)  width, height in pixels
//   nearPlane        float offset 216  ( 4 bytes)
//   farPlane         float offset 220  ( 4 bytes)
//   Total: 224 bytes
#pragma once

#ifdef __cplusplus
    #include <glm/mat4x4.hpp>
    #include <glm/vec2.hpp>
    #include <glm/vec4.hpp>
    #define mat4 glm::mat4
    #define vec4 glm::vec4
    #define vec2 glm::vec2
namespace GN::fx2::shader {
#endif

struct CameraUBO {
    mat4  viewMatrix;       // world → view
    mat4  projMatrix;       // view → clip; caller must apply Vulkan clip-space Y-inversion ([1][1] *= -1) before uploading
    mat4  projViewMatrix;   // projMatrix * viewMatrix; pre-multiplied to avoid per-vertex recomputation
    vec4  cameraPosition;   // xyz = camera world-space position, w = 1
    vec2  renderTargetSize; // width, height in pixels
    float nearPlane;
    float farPlane;
};

#ifdef __cplusplus
    #undef mat4
    #undef vec4
    #undef vec2

    #include <cstddef>
static_assert(sizeof(CameraUBO) == 224, "CameraUBO must be 224 bytes");
static_assert(offsetof(CameraUBO, viewMatrix) == 0);
static_assert(offsetof(CameraUBO, projMatrix) == 64);
static_assert(offsetof(CameraUBO, projViewMatrix) == 128);
static_assert(offsetof(CameraUBO, cameraPosition) == 192);
static_assert(offsetof(CameraUBO, renderTargetSize) == 208);
static_assert(offsetof(CameraUBO, nearPlane) == 216);
static_assert(offsetof(CameraUBO, farPlane) == 220);
} // namespace GN::fx2::shader
#endif

#ifndef __cplusplus
// GLSL-only: CameraUBO binding at set 0, binding 1.
layout(std140, set = 0, binding = 1) uniform CameraBlock {
    mat4  viewMatrix;
    mat4  projMatrix;
    mat4  projViewMatrix;
    vec4  cameraPosition;
    vec2  renderTargetSize;
    float nearPlane;
    float farPlane;
}
u_camera;
#endif
