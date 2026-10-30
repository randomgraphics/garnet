#include "pch.h"
#include "vk-sampler.h"
#include <cmath>

namespace GN::gpu2 {

SamplerVulkan::SamplerVulkan(const StrA & name, AutoRef<GpuContextVulkan2> gpu, vk::Sampler sampler)
    : Sampler(TYPE_INFO(), name), mGpu(std::move(gpu)), mSampler(sampler) {}

SamplerVulkan::~SamplerVulkan() { mGpu->vulkanDevice().handle().destroySampler(mSampler); }

AutoRef<Sampler> createSamplerVulkan(const StrA & name, const Sampler::CreateParameters & cp) {
    auto *       gpu = RuntimeType::cast<GpuContextVulkan2>(cp.context.get());
    const auto & d   = cp.descriptor;
    if (!gpu || !gpu->ready() || !std::isfinite(d.minLod) || !std::isfinite(d.maxLod) || d.minLod < 0 || d.maxLod < d.minLod) GN_UNLIKELY return {};
    using Filter      = Sampler::Descriptor::Filter;
    using Address     = Sampler::Descriptor::Address;
    auto validFilter  = [](Filter f) { return f == Filter::NEAREST || f == Filter::LINEAR; };
    auto validAddress = [](Address a) { return a == Address::REPEAT || a == Address::MIRRORED_REPEAT || a == Address::CLAMP_TO_EDGE; };
    if (!validFilter(d.minFilter) || !validFilter(d.magFilter) || !validFilter(d.mipFilter) || !validAddress(d.addressU) || !validAddress(d.addressV) ||
        !validAddress(d.addressW))
        GN_UNLIKELY return {};
    auto filter  = [](Filter f) { return f == Filter::NEAREST ? vk::Filter::eNearest : vk::Filter::eLinear; };
    auto address = [](Address a) {
        switch (a) {
        case Address::REPEAT:
            return vk::SamplerAddressMode::eRepeat;
        case Address::MIRRORED_REPEAT:
            return vk::SamplerAddressMode::eMirroredRepeat;
        default:
            return vk::SamplerAddressMode::eClampToEdge;
        }
    };
    vk::SamplerCreateInfo info;
    info.setMinFilter(filter(d.minFilter))
        .setMagFilter(filter(d.magFilter))
        .setMipmapMode(d.mipFilter == Filter::NEAREST ? vk::SamplerMipmapMode::eNearest : vk::SamplerMipmapMode::eLinear)
        .setAddressModeU(address(d.addressU))
        .setAddressModeV(address(d.addressV))
        .setAddressModeW(address(d.addressW))
        .setMinLod(d.minLod)
        .setMaxLod(d.maxLod);
    try {
        auto native = gpu->vulkanDevice().handle().createSampler(info);
        return referenceTo(new SamplerVulkan(name, referenceTo(gpu), native));
    } catch (const std::exception & e) {
        GN_ERROR(getLogger("GN.gpu2.sampler"), "Sampler creation failed: {}", e.what());
        return {};
    }
}

} // namespace GN::gpu2
