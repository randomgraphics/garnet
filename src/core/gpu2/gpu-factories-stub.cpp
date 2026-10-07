#include "pch.h"
#include "gpu-context.h"
#include "vk-buffer.h"
#include "vk-gpu-cnc.h"
#include "vk-gpu-raster.h"
#include "vk-gpu-shader.h"
#include "vk-texture.h"
#include "vk-sampler.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-raster.h"

namespace GN::gpu2 {
AutoRef<bindless::CnC> createVkBindlessCnc(const StrA & name, const bindless::CnC::CreateParameters & cp);
}

static GN::Logger * sLogger = GN::getLogger("GN.gpu2");

// -----------------------------------------------------------------------
// Factories for gpu2 types (GpuShader, Texture)
// -----------------------------------------------------------------------
namespace GN::gpu2 {

AutoRef<GpuShader> GpuShader::create(const CreateParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger, "GpuShader::create: GpuContext is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = params.context.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "GpuShader::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createGpuShaderVulkan2(params);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        return {};
    default:
        GN_ERROR(sLogger, "GpuShader::create: unknown GpuContext::Api");
        return {};
    }
}

AutoRef<GpuShader> GpuShader::load(const LoadParameters & params) {
    if (!params.context) { GN_ERROR(sLogger, "GpuShader::load: GpuContext is null"); }
    return {};
}

AutoRef<Sampler> Sampler::create(const StrA & name, const CreateParameters & cp) {
    if (!cp.context) {
        GN_ERROR(sLogger, "Sampler::create: GpuContext is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = cp.context.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "Sampler::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createSamplerVulkan(name, cp);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "Sampler::create: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "Sampler::create: unknown GpuContext::Api");
        return {};
    }
}

AutoRef<Buffer> Buffer::create(const StrA & name, const CreateParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger, "Buffer::create: GpuContext is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = params.context.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "Buffer::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createBufferVulkan2(name, params);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "Buffer::create: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "Buffer::create: unknown GpuContext::Api");
        return {};
    }
}

AutoRef<Texture> Texture::create(const StrA & entityName, const CreateParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger, "Texture::create: GpuContext is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = params.context.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "Texture::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createTextureVulkan2(entityName, params);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "Texture::create: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "Texture::create: unknown GpuContext::Api");
        return {};
    }
}

AutoRef<Texture> Texture::load(const LoadParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger, "Texture::load: GpuContext is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = params.context.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "Texture::load: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return loadTextureVulkan2(params);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "Texture::load: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "Texture::load: unknown GpuContext::Api");
        return {};
    }
}

} // namespace GN::gpu2

// -----------------------------------------------------------------------
// Factory for GpuRaster (still an rdg2 type defined in raster.h)
// -----------------------------------------------------------------------
namespace GN::gpu2 {

AutoRef<GpuRaster> GpuRaster::create(const StrA & name, const CreateParameters & params) {
    if (!params.gpu) {
        GN_ERROR(sLogger, "GpuRaster::create: GpuContext is null");
        return {};
    }
    AutoRef<GN::gpu2::GpuContextCommon2> common = params.gpu.staticCastTo<GN::gpu2::GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "GpuRaster::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GN::gpu2::GpuContextCommon2::Api::VULKAN:
        return createGpuRasterVulkan2(name, params);
    case GN::gpu2::GpuContextCommon2::Api::D3D12:
    case GN::gpu2::GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "GpuRaster::create: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "GpuRaster::create: unknown GpuContext::Api");
        return {};
    }
}

AutoRef<GpuCnC> GpuCnC::create(const CreateParameters & params) {
    if (!params.gpu) {
        GN_ERROR(sLogger, "GpuCnC::create: GpuContext is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = params.gpu.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "GpuCnC::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createGpuCncVulkan2(params);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "GpuCnC::create: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "GpuCnC::create: unknown GpuContext::Api");
        return {};
    }
}
} // namespace GN::gpu2

namespace GN::gpu2::bindless {

AutoRef<DescriptorHeap> DescriptorHeap::create(const StrA & name, const CreateParameters & cp) {
    if (!cp.gpu) {
        GN_ERROR(sLogger, "DescriptorHeap::create: GpuContext is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = cp.gpu.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "DescriptorHeap::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createVkBindlessDescriptorHeap(name, cp);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "DescriptorHeap::create: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "DescriptorHeap::create: unknown GpuContext::Api");
        return {};
    }
}

AutoRef<Raster> Raster::create(const StrA & name, const CreateParameters & cp) {
    if (!cp.gpu) {
        GN_ERROR(sLogger, "Raster::create: GpuContext is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = cp.gpu.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "Raster::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createVkBindlessRaster(name, cp);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "Raster::create: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "Raster::create: unknown GpuContext::Api");
        return {};
    }
}

AutoRef<CnC> CnC::create(const StrA & name, const CreateParameters & cp) {
    if (!cp.gpu) {
        GN_ERROR(sLogger, "CnC::create: GpuContext is null");
        return {};
    }
    if (!cp.heap) {
        GN_ERROR(sLogger, "CnC::create: DescriptorHeap is null");
        return {};
    }
    AutoRef<GpuContextCommon2> common = cp.gpu.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger, "CnC::create: GpuContext is not GpuContextCommon2");
        return {};
    }
    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createVkBindlessCnc(name, cp);
    case GpuContextCommon2::Api::D3D12:
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger, "CnC::create: backend not implemented");
        return {};
    default:
        GN_ERROR(sLogger, "CnC::create: unknown GpuContext::Api");
        return {};
    }
}

} // namespace GN::gpu2::bindless
