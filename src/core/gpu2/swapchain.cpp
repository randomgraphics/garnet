#include "pch.h"
#include "vk-swapchain.h"
#include "gpu-context.h"

using GN::gpu2::GpuContextCommon2;

static GN::Logger * sLogger = GN::getLogger("GN.gpu2");

namespace GN::gpu2 {

GN_API AutoRef<Swapchain> Swapchain::create(const CreateDesc & desc) {
    if (!desc.gpu) {
        GN_ERROR(sLogger)("Swapchain::create: gpu context is null");
        return {};
    }

    AutoRef<GpuContextCommon2> common = desc.gpu.staticCastTo<GpuContextCommon2>();
    if (!common) {
        GN_ERROR(sLogger)("Swapchain::create: GpuContext is not a GpuContextCommon2 implementation");
        return {};
    }

    switch (common->api()) {
    case GpuContextCommon2::Api::VULKAN:
        return createSwapchainVulkan2(desc);
    case GpuContextCommon2::Api::D3D12:
        GN_ERROR(sLogger)("Swapchain::create: D3D12 swapchain not implemented yet");
        return {};
    case GpuContextCommon2::Api::METAL:
        GN_ERROR(sLogger)("Swapchain::create: Metal swapchain not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("Swapchain::create: unknown GpuContext::Api value");
        return {};
    }
}

} // namespace GN::gpu2
