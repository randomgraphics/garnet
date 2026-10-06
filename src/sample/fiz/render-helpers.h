#pragma once
#include <garnet/GNfx2.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <algorithm>

namespace fizsample {

struct HelperVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

inline GN::gpu2::RasterGeometry makeGeometry(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::GpuCnC & uploads, const GN::DynaArray<HelperVertex> & verts,
                                             const GN::DynaArray<uint32_t> & indices) {
    using AF = GN::gpu2::RasterGeometry::AttributeFormat;
    GN::gpu2::RasterGeometry geometry;
    geometry.format.attributes.push_back({.location = 0, .binding = 0, .offset = 0, .format = AF::F32_3});
    geometry.format.attributes.push_back({.location = 1, .binding = 0, .offset = 12, .format = AF::F32_3});
    geometry.format.attributes.push_back({.location = 2, .binding = 0, .offset = 24, .format = AF::F32_2});

    const uint32_t stride      = sizeof(HelperVertex);
    const bool     use16       = verts.size() <= 65536;
    const uint32_t indexStride = use16 ? 2 : 4;

    GN::DynaArray<uint16_t> indices16;
    if (use16) {
        indices16.resize(indices.size());
        for (size_t i = 0; i < indices.size(); ++i) indices16[i] = static_cast<uint16_t>(indices[i]);
    }

    auto vb = GN::gpu2::Buffer::create("fiz.geom.vb", {.context = gpu, .size = verts.size() * stride});
    auto ib = GN::gpu2::Buffer::create("fiz.geom.ib", {.context = gpu, .size = indices.size() * indexStride});
    if (!vb || !ib) return {};

    uploads.recordUploadBuffer(vb, 0, {reinterpret_cast<const uint8_t *>(verts.data()), verts.size() * stride});
    const auto * indexBytes = use16 ? reinterpret_cast<const uint8_t *>(indices16.data()) : reinterpret_cast<const uint8_t *>(indices.data());
    uploads.recordUploadBuffer(ib, 0, {indexBytes, indices.size() * indexStride});

    geometry.vertices.push_back({.buffer = vb, .offset = 0, .stride = stride});
    geometry.indices     = {.buffer = ib, .offset = 0, .stride = indexStride};
    geometry.vertexCount = static_cast<uint32_t>(verts.size());
    geometry.indexCount  = static_cast<uint32_t>(indices.size());
    return geometry;
}

inline GN::gpu2::RasterGeometry boxGeometry(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::GpuCnC & uploads, glm::vec3 halfExtents) {
    const float     hx = halfExtents.x, hy = halfExtents.y, hz = halfExtents.z;
    const glm::vec3 corners[8] = {
        {-hx, -hy, -hz}, {hx, -hy, -hz}, {hx, hy, -hz}, {-hx, hy, -hz}, {-hx, -hy, hz}, {hx, -hy, hz}, {hx, hy, hz}, {-hx, hy, hz},
    };
    static const glm::vec3 normals[6] = {
        {0, 0, -1}, {0, -1, 0}, {1, 0, 0}, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1},
    };
    static const int faceCorners[6][4] = {
        {0, 1, 2, 3}, {0, 4, 5, 1}, {1, 5, 6, 2}, {2, 6, 7, 3}, {3, 7, 4, 0}, {7, 6, 5, 4},
    };
    static const glm::vec2 uvs[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

    GN::DynaArray<HelperVertex> verts;
    GN::DynaArray<uint32_t>     indices;
    for (int f = 0; f < 6; ++f) {
        uint32_t base = static_cast<uint32_t>(verts.size());
        for (int v = 0; v < 4; ++v) { verts.append({corners[faceCorners[f][v]], normals[f], uvs[v]}); }
        indices.append(base + 2);
        indices.append(base + 1);
        indices.append(base + 0);
        indices.append(base + 3);
        indices.append(base + 2);
        indices.append(base + 0);
    }
    return makeGeometry(gpu, uploads, verts, indices);
}

inline GN::gpu2::RasterGeometry sphereGeometry(GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::GpuCnC & uploads, float radius, uint32_t slices,
                                               uint32_t stacks) {
    GN::DynaArray<HelperVertex> verts;
    GN::DynaArray<uint32_t>     indices;
    for (uint32_t i = 0; i <= stacks; ++i) {
        const float phi    = GN_PI * static_cast<float>(i) / static_cast<float>(stacks);
        const float sinPhi = (i == 0 || i == stacks) ? 0.0f : std::sin(phi);
        const float cosPhi = std::cos(phi);
        for (uint32_t j = 0; j <= slices; ++j) {
            const float theta    = (j == slices) ? 0.0f : GN_TWO_PI * static_cast<float>(j) / static_cast<float>(slices);
            const float sinTheta = std::sin(theta);
            const float cosTheta = std::cos(theta);
            glm::vec3   n(sinPhi * cosTheta, cosPhi, sinPhi * sinTheta);
            verts.append({n * radius, n, {static_cast<float>(j) / slices, static_cast<float>(i) / stacks}});
        }
    }
    for (uint32_t i = 0; i < stacks; ++i) {
        for (uint32_t j = 0; j < slices; ++j) {
            uint32_t first  = i * (slices + 1) + j;
            uint32_t second = first + slices + 1;
            if (i != 0) {
                indices.append(first);
                indices.append(first + 1);
                indices.append(second);
            }
            if (i + 1 != stacks) {
                indices.append(second);
                indices.append(first + 1);
                indices.append(second + 1);
            }
        }
    }
    return makeGeometry(gpu, uploads, verts, indices);
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
    if (skyboxTex) skyParams.skyboxMap = GN::gpu2::GpuResourceView {skyboxTex}.setImageViewType(GN::gpu2::GpuResourceView::ImageView::SAMPLED);
    if (irradianceTex) skyParams.irradianceMap = GN::gpu2::GpuResourceView {irradianceTex}.setImageViewType(GN::gpu2::GpuResourceView::ImageView::SAMPLED);
    if (prefilteredTex) skyParams.prefilteredMap = GN::gpu2::GpuResourceView {prefilteredTex}.setImageViewType(GN::gpu2::GpuResourceView::ImageView::SAMPLED);
    if (brdfLutTex) skyParams.brdfLut = GN::gpu2::GpuResourceView {brdfLutTex}.setImageViewType(GN::gpu2::GpuResourceView::ImageView::SAMPLED);
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
