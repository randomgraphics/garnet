#include "pch.h"
#include "vk-gpu-shader.h"
#include "gpu-context.h"
#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

static StrA shaderEntityName(const GpuShader::CreateParameters & p) { return p.name.empty() ? StrA("gpu_shader") : p.name; }

static const char * shaderEntryCStr(const GpuShader::CreateParameters & p) {
    if (p.entry && p.entry[0]) return p.entry;
    return "main";
}

GpuShaderVulkan::GpuShaderVulkan(const StrA & entityName, std::unique_ptr<rv::Shader> sh): GpuShader(TYPE_INFO(), entityName), mShader(std::move(sh)) {}

GpuShaderVulkan::~GpuShaderVulkan() = default;

AutoRef<GpuShader> createGpuShaderVulkan2(const GpuShader::CreateParameters & p) {
    if (!p.context) {
        GN_ERROR(sLogger, "createGpuShaderVulkan2: null GpuContext");
        return {};
    }
    AutoRef<GpuContextVulkan2> vkctx = p.context.staticCastTo<GpuContextVulkan2>();
    if (!vkctx || !vkctx->ready()) {
        GN_ERROR(sLogger, "createGpuShaderVulkan2: GpuContext is not a ready Vulkan context");
        return {};
    }
    const rv::Device & dev = vkctx->vulkanDevice();
    if (!dev.handle()) {
        GN_ERROR(sLogger, "createGpuShaderVulkan2: no Vulkan device");
        return {};
    }
    if (!p.binary || p.size == 0) {
        GN_ERROR(sLogger, "createGpuShaderVulkan2: empty SPIR-V for '{}'", shaderEntityName(p));
        return {};
    }
    if ((p.size % sizeof(uint32_t)) != 0) {
        GN_ERROR(sLogger, "createGpuShaderVulkan2: SPIR-V size {} not multiple of 4 for '{}'", p.size, shaderEntityName(p));
        return {};
    }

    rv::Shader::ConstructParameters scp;
    scp.setGi(dev.gi());
    scp.setSpirv(p.size / sizeof(uint32_t), static_cast<const uint32_t *>(p.binary));
    scp.entry = shaderEntryCStr(p);

    std::unique_ptr<rv::Shader> sh;
    try {
        sh.reset(new rv::Shader(scp));
    } catch (const std::exception & ex) {
        GN_ERROR(sLogger, "createGpuShaderVulkan2: Shader ctor failed for '{}': {}", shaderEntityName(p), ex.what());
        return {};
    }
    if (!sh->handle()) {
        GN_ERROR(sLogger, "createGpuShaderVulkan2: null shader module for '{}'", shaderEntityName(p));
        return {};
    }

    return AutoRef<GpuShader>(new GpuShaderVulkan(shaderEntityName(p), std::move(sh)));
}

} // namespace GN::gpu2
