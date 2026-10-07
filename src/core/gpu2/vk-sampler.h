#pragma once

#include "vk-gpu-context.h"

namespace GN::gpu2 {

class SamplerVulkan final : public Sampler {
public:
    GN_REGISTER_RUNTIME_TYPE(Sampler);
    SamplerVulkan(const StrA & name, AutoRef<GpuContextVulkan2> gpu, vk::Sampler sampler);
    ~SamplerVulkan() override;

    vk::Sampler         nativeSampler() const { return mSampler; }
    GpuContextVulkan2 * context() const { return mGpu.get(); }

private:
    AutoRef<GpuContextVulkan2> mGpu;
    vk::Sampler                mSampler;
};

AutoRef<Sampler> createSamplerVulkan(const StrA & name, const Sampler::CreateParameters & cp);

} // namespace GN::gpu2
