#pragma once

#include <garnet/GNgpu2.h>
#include <memory>

namespace GN::gpu2::rv {
class Shader;
}

namespace GN::gpu2 {

class GpuShaderVulkan;

/// Vulkan \c GpuShader: owns a \c rv::Shader (SPIR-V module + entry).
class GpuShaderVulkan final : public GpuShader {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuShader);

    const rv::Shader * rvShader() const { return mShader.get(); }

private:
    friend AutoRef<GpuShader> createGpuShaderVulkan2(const GpuShader::CreateParameters & params);

    GpuShaderVulkan(const StrA & entityName, std::unique_ptr<rv::Shader> sh);
    ~GpuShaderVulkan() override;

    std::unique_ptr<rv::Shader> mShader;
};

AutoRef<GpuShader> createGpuShaderVulkan2(const GpuShader::CreateParameters & params);

} // namespace GN::gpu2
