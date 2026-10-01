#pragma once
#include <garnet/GNfx2.h>

namespace fizsample {
inline GN::fx2::PbrKernel::Inputs boxInputs(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::GpuCnC & uploads, glm::vec3 halfExtents, glm::vec4 color,
                                            float metallic, float roughness) {
    GN::fx2::LitKernelInputs::CubeCreateOptions options;
    options.width  = halfExtents.x * 2;
    options.height = halfExtents.y * 2;
    options.depth  = halfExtents.z * 2;
    options.uv = options.tangent = false;
    GN::fx2::PbrKernel::Inputs result;
    result.geometry  = GN::fx2::LitKernelInputs::createBox(gpu, uploads, options);
    result.color     = color;
    result.metallic  = metallic;
    result.roughness = roughness;
    return result;
}

inline GN::fx2::PbrKernel::Inputs sphereInputs(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::GpuCnC & uploads, float radius, uint32_t slices,
                                               uint32_t stacks, glm::vec4 color, float metallic, float roughness) {
    GN::fx2::LitKernelInputs::SphereCreateOptions options;
    options.radius = radius;
    options.slices = slices;
    options.stacks = stacks;
    options.uv = options.tangent = false;
    GN::fx2::PbrKernel::Inputs result;
    result.geometry  = GN::fx2::LitKernelInputs::createSphere(gpu, uploads, options);
    result.color     = color;
    result.metallic  = metallic;
    result.roughness = roughness;
    return result;
}
} // namespace fizsample
