#pragma once
#include <garnet/GNfx2.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <algorithm>

namespace fizsample {

inline GN::gpu2::RasterGeometry boxGeometry(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::GpuCnC & uploads, glm::vec3 halfExtents) {
    GN::fx2::LitKernelInputs::CubeCreateOptions options;
    options.width   = halfExtents.x * 2;
    options.height  = halfExtents.y * 2;
    options.depth   = halfExtents.z * 2;
    options.uv      = true;
    options.tangent = false;
    return GN::fx2::LitKernelInputs::createBox(gpu, uploads, options);
}

inline GN::gpu2::RasterGeometry sphereGeometry(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::GpuCnC & uploads, float radius, uint32_t slices,
                                               uint32_t stacks) {
    GN::fx2::LitKernelInputs::SphereCreateOptions options;
    options.radius  = radius;
    options.slices  = slices;
    options.stacks  = stacks;
    options.uv      = true;
    options.tangent = false;
    return GN::fx2::LitKernelInputs::createSphere(gpu, uploads, options);
}

inline GN::AutoRef<GN::fx2::bindless::PbrMaterial> createPbrMaterial(GN::AutoRef<GN::fx2::bindless::PbrKernel> kernel, GN::gpu2::bindless::CnC & uploads,
                                                                     glm::vec4 color, float metallic, float roughness) {
    auto p      = kernel->defaultMaterialParameters();
    p.baseColor = color;
    p.metallic  = metallic;
    p.roughness = roughness;
    return kernel->createMaterial(uploads, p);
}

inline GN::AutoRef<GN::fx2::bindless::SkyMaterial> createSkyMaterial(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::AutoRef<GN::fx2::bindless::SkyKernel> skyKernel,
                                                                     GN::gpu2::bindless::CnC & uploads, float luminanceScale = 3000.f) {
    auto skyboxTex      = GN::gpu2::Texture::load({.context = gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds"});
    auto irradianceTex  = GN::gpu2::Texture::load({.context = gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds"});
    auto prefilteredTex = GN::gpu2::Texture::load({.context = gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds"});
    auto brdfLutTex     = GN::gpu2::Texture::load({.context = gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds"});

    auto skyParams = skyKernel->defaultMaterialParameters();
    if (skyboxTex) skyParams.skyboxMap = GN::gpu2::GpuResourceView(skyboxTex).setImageViewType(GN::gpu2::GpuResourceView::ImageView::SAMPLED);
    if (irradianceTex) skyParams.irradianceMap = GN::gpu2::GpuResourceView(irradianceTex).setImageViewType(GN::gpu2::GpuResourceView::ImageView::SAMPLED);
    if (prefilteredTex) skyParams.prefilteredMap = GN::gpu2::GpuResourceView(prefilteredTex).setImageViewType(GN::gpu2::GpuResourceView::ImageView::SAMPLED);
    if (brdfLutTex) skyParams.brdfLut = GN::gpu2::GpuResourceView(brdfLutTex).setImageViewType(GN::gpu2::GpuResourceView::ImageView::SAMPLED);
    skyParams.luminanceScale = luminanceScale;
    return skyKernel->createMaterial(uploads, skyParams);
}

inline GN::AutoRef<GN::fx2::bindless::SharedShaderConstants::UniformState> updateUniforms(GN::AutoRef<GN::fx2::bindless::SharedShaderConstants> ssc,
                                                                                          GN::gpu2::bindless::CnC &                             uploads,
                                                                                          const GN::gpu2::RasterTarget & target, const glm::vec3 & eye,
                                                                                          const glm::vec3 & targetPos, int frameIdx) {
    const glm::vec3 kUp(0.0f, 1.0f, 0.0f);
    const auto      rasterSize = target.calcRasterSizeInPixel();
    const float     aspect     = static_cast<float>(rasterSize.x) / static_cast<float>(std::max(1u, rasterSize.y));

    GN::fx2::bindless::SharedUniforms uniforms;
    uniforms.frameCounter   = static_cast<uint32_t>(frameIdx);
    uniforms.cameraPosition = {eye, 1.0f};
    uniforms.viewMatrix     = glm::lookAtRH(eye, targetPos, kUp);
    uniforms.projMatrix     = glm::perspectiveRH_ZO(glm::radians(60.0f), aspect, 0.01f, 10000.0f);
    uniforms.projMatrix[1][1] *= -1; // Vulkan clip space
    uniforms.projViewMatrix   = uniforms.projMatrix * uniforms.viewMatrix;
    uniforms.renderTargetSize = {static_cast<float>(rasterSize.x), static_cast<float>(rasterSize.y)};
    uniforms.nearPlane        = 0.01f;
    uniforms.farPlane         = 10000.0f;
    uniforms.exposure         = 0.002f;

    return ssc->recordUniformUpdate(uploads, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
}

} // namespace fizsample
