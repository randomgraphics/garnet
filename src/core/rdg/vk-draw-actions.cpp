#include "pch.h"
#include "vk-draw-actions.h"
#include "vk-submission-context.h"
#include "vk-backbuffer.h"
#include "vk-texture.h"
#include "vk-buffer.h"
#include "vk-pso-factory.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

// =====================================================================================================================
// ClearRenderTargetVulkan
// =====================================================================================================================

class ClearRenderTargetVulkan : public ClearRenderTarget {
    AutoRef<GpuContextVulkan> mGpu;

public:
    ClearRenderTargetVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu)
        : ClearRenderTarget(db, TYPE_ID, TYPE_NAME, name), mGpu(gpu) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = arguments.castTo<ClearRenderTarget::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not ClearRenderTarget::A", taskInfo);

        // standard preparation.
        auto & submissionContext = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(submissionContext.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        GN_RDG_FAIL_ON_FALSE(submissionContext.renderPassManager.prepareDraw(taskInfo, a->renderTarget.value));

        // done
        return Action::PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        bool hasWarning = false;

        auto & submission = taskInfo.submission;

        auto a = arguments.castTo<ClearRenderTarget::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not ClearRenderTarget::A", taskInfo);

        // stadnard execution
        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FALSE(cb.queue && cb.commandBuffer);

        // acquire render pass. End if if needed.
        auto rp = sc.renderPassManager.execute(taskInfo, cb.commandBuffer.handle());
        GN_RDG_FAIL_ON_FAIL(rp.result);
        if (rp.needToEnd) cb.commandBuffer.handle().endRendering();

        // submit command buffer, if asked to do so.
        if (cb.submit) cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb.commandBuffer}});

        // done
        return hasWarning ? WARNING : PASSED;
    }
};

AutoRef<ClearRenderTarget> createVulkanClearRenderTarget(ArtifactDatabase & db, const StrA & name, const ClearRenderTarget::CreateParameters & params) {
    auto gpu = params.gpu.castTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanClearRenderTarget: gpu is empty, name='{}'", name);
            return {};
        }
    return AutoRef<ClearRenderTarget>(new ClearRenderTargetVulkan(db, name, gpu));
}

// =====================================================================================================================
// GpuDrawVulkan
// =====================================================================================================================

static vk::Format toVkFormat(GpuGeometry::AttributeFormat f) {
    using F = GpuGeometry::AttributeFormat;
    switch (f) {
    case F::F32_1:
        return vk::Format::eR32Sfloat;
    case F::F32_2:
        return vk::Format::eR32G32Sfloat;
    case F::F32_3:
        return vk::Format::eR32G32B32Sfloat;
    case F::F32_4:
        return vk::Format::eR32G32B32A32Sfloat;
    case F::F16_1:
        return vk::Format::eR16Sfloat;
    case F::F16_2:
        return vk::Format::eR16G16Sfloat;
    case F::F16_3:
        return vk::Format::eR16G16B16Sfloat;
    case F::F16_4:
        return vk::Format::eR16G16B16A16Sfloat;
    case F::U32_1:
        return vk::Format::eR32Uint;
    case F::U32_2:
        return vk::Format::eR32G32Uint;
    case F::U32_3:
        return vk::Format::eR32G32B32Uint;
    case F::U32_4:
        return vk::Format::eR32G32B32A32Uint;
    case F::U16_1:
        return vk::Format::eR16Uint;
    case F::U16_2:
        return vk::Format::eR16G16Uint;
    case F::U16_3:
        return vk::Format::eR16G16B16Uint;
    case F::U16_4:
        return vk::Format::eR16G16B16A16Uint;
    case F::U8_1:
        return vk::Format::eR8Uint;
    case F::U8_2:
        return vk::Format::eR8G8Uint;
    case F::U8_3:
        return vk::Format::eR8G8B8Uint;
    case F::U8_4:
        return vk::Format::eR8G8B8A8Uint;
    case F::I32_1:
        return vk::Format::eR32Sint;
    case F::I32_2:
        return vk::Format::eR32G32Sint;
    case F::I32_3:
        return vk::Format::eR32G32B32Sint;
    case F::I32_4:
        return vk::Format::eR32G32B32A32Sint;
    case F::I16_1:
        return vk::Format::eR16Sint;
    case F::I16_2:
        return vk::Format::eR16G16Sint;
    case F::I16_3:
        return vk::Format::eR16G16B16Sint;
    case F::I16_4:
        return vk::Format::eR16G16B16A16Sint;
    case F::I8_1:
        return vk::Format::eR8Sint;
    case F::I8_2:
        return vk::Format::eR8G8Sint;
    case F::I8_3:
        return vk::Format::eR8G8B8Sint;
    case F::I8_4:
        return vk::Format::eR8G8B8A8Sint;
    default:
        return vk::Format::eR32G32B32Sfloat;
    }
}

static uint64_t hashShaderBinary(const void * vs, size_t vsSize, const void * ps, size_t psSize) {
    uint64_t     h = 0xcbf29ce484222325ULL; // FNV-1a offset
    const auto * p = static_cast<const uint8_t *>(vs);
    for (size_t i = 0; i < vsSize; ++i) h = (h ^ p[i]) * 0x100000001b3ULL;
    if (ps && psSize > 0) {
        p = static_cast<const uint8_t *>(ps);
        for (size_t i = 0; i < psSize; ++i) h = (h ^ p[i]) * 0x100000001b3ULL;
    }
    return h;
}

static uint64_t hashVertexFormat(const GpuGeometry::VertexFormat & fmt) {
    uint64_t h = 0;
    for (const auto & a : fmt.attributes) h = h * 31 + (uint64_t) a.location + ((uint64_t) a.offset << 8) + ((uint64_t) static_cast<uint8_t>(a.format) << 16);
    return h & ((1ULL << 27) - 1); // 27 bits
}

class GpuDrawVulkan : public GpuDraw {
    AutoRef<GpuContextVulkan> mGpu;
    GpuDraw::CreateParameters mCreateParams {};

public:
    GpuDrawVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu, const GpuDraw::CreateParameters & params)
        : GpuDraw(db, TYPE_ID, TYPE_NAME, name), mGpu(gpu), mCreateParams(params) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<GpuDraw::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        auto & submissionContext = submissionImpl.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(submissionContext.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        GN_RDG_FAIL_ON_FALSE(submissionContext.renderPassManager.prepareDraw(taskInfo, {}));

        return PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        bool hasWarning = false;

        auto & submission = taskInfo.submission;

        auto a = arguments.castTo<GpuDraw::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FALSE(cb.queue && cb.commandBuffer);

        const GpuGeometry & geom        = a->geometry;
        const bool          hasGeometry = !geom.format.empty() && !geom.vertices.empty() && geom.vertices[0].stride > 0;

        uint32_t vertexCount = 0;
        if (hasGeometry)
            vertexCount = (uint32_t) geom.vertices[0].count();
        else if (geom.vertexCount > 0)
            vertexCount = geom.vertexCount;

        if (vertexCount == 0) {
            auto rp = sc.renderPassManager.execute(taskInfo, cb.commandBuffer.handle());
            GN_RDG_FAIL_ON_FAIL(rp.result);
            if (rp.needToEnd) cb.commandBuffer.handle().endRendering();
            if (cb.submit) cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb.commandBuffer}});
            return hasWarning ? WARNING : PASSED;
        }

        if (!mCreateParams.vs.binary || mCreateParams.vs.size == 0) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - GpuDraw has no vertex shader", taskInfo);
                return FAILED;
            }

        GraphicsPsoKey key {};
        key.shaderHash       = hashShaderBinary(mCreateParams.vs.binary, mCreateParams.vs.size, mCreateParams.ps.binary, mCreateParams.ps.size);
        key.noVertexInput    = hasGeometry ? 0 : 1;
        key.vertexStride     = hasGeometry ? static_cast<uint64_t>(geom.vertices[0].stride) & ((1ULL << 16) - 1) : 0;
        key.vertexFormatHash = hasGeometry ? hashVertexFormat(geom.format) : 0;
        key.colorFormat      = static_cast<uint64_t>(vk::Format::eB8G8R8A8Unorm) & ((1ULL << 12) - 1);
        key.pushConstantSize = 128;

        GraphicsPsoCreateParams createParams {};
        createParams.vsSpirv = mCreateParams.vs.binary;
        createParams.vsSize  = mCreateParams.vs.size;
        createParams.vsEntry = mCreateParams.vs.entry ? mCreateParams.vs.entry : "main";
        createParams.psSpirv = mCreateParams.ps.binary;
        createParams.psSize  = mCreateParams.ps.size;
        createParams.psEntry = mCreateParams.ps.entry ? mCreateParams.ps.entry : "main";

        DynaArray<vk::VertexInputAttributeDescription> vkAttrs;
        DynaArray<vk::VertexInputBindingDescription>   vkBinds;
        if (hasGeometry) {
            const uint32_t stride = static_cast<uint32_t>(geom.vertices[0].stride);
            vkBinds.append({0, stride, vk::VertexInputRate::eVertex});
            for (const auto & attr : geom.format.attributes) vkAttrs.append({attr.location, 0, toVkFormat(attr.format), attr.offset});
            createParams.va      = vkAttrs.data();
            createParams.vaCount = static_cast<uint32_t>(vkAttrs.size());
            createParams.vb      = vkBinds.data();
            createParams.vbCount = static_cast<uint32_t>(vkBinds.size());
        }

        auto pipeline = mGpu->psoFactory().getOrCreateGraphicsPso(key, &createParams);
        if (!pipeline) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - PSO factory returned null pipeline", taskInfo);
                return FAILED;
            }

        if (!a->constants.empty()) {
            const auto size = static_cast<uint32_t>(a->constants.size());
            if (size > 128) GN_UNLIKELY {
                    GN_ERROR(sLogger)("{} - inline constants size is too large, size={}", taskInfo, size);
                    return FAILED;
                }
        }

        rapid_vulkan::Drawable::ConstructParameters dcp;
        dcp.setPipeline(pipeline);
        rapid_vulkan::Drawable drawable(dcp);

        if (!a->constants.empty()) drawable.c(0, a->constants.size(), a->constants.data(), vk::ShaderStageFlagBits::eVertex);

        if (hasGeometry && !geom.vertices.empty() && geom.vertices[0].buffer) {
            auto *                                  bv  = static_cast<BufferVulkan *>(geom.vertices[0].buffer.get());
            rapid_vulkan::Ref<rapid_vulkan::Buffer> ref = bv->refVkBuffer();
            if (ref) {
                const rapid_vulkan::BufferView view {ref, geom.vertices[0].offset, vk::DeviceSize(-1)};
                drawable.v(vk::ArrayProxy<const rapid_vulkan::BufferView>(1, &view));
            }
        }

        rapid_vulkan::GraphicsPipeline::DrawParameters drawParams {};
        drawParams.setNonIndexed(vertexCount, 0).setInstance(1, 0);
        drawable.draw(drawParams);

        rapid_vulkan::Ref<const rapid_vulkan::DrawPack> drawPack = drawable.compile();
        if (!drawPack || drawPack->empty()) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - Drawable compile produced empty DrawPack", taskInfo);
                return FAILED;
            }

        cb.commandBuffer.render(drawPack);

        auto rp = sc.renderPassManager.execute(taskInfo, cb.commandBuffer.handle());
        GN_RDG_FAIL_ON_FAIL(rp.result);
        if (rp.needToEnd) GN_UNLIKELY {
                GN_VERBOSE(sLogger)("{} - ending render pass", taskInfo);
                cb.commandBuffer.handle().endRendering();
            }

        if (cb.submit) cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb.commandBuffer}});

        return hasWarning ? WARNING : PASSED;
    }
};

AutoRef<GpuDraw> createVulkanGpuDraw(ArtifactDatabase & db, const StrA & name, const GpuDraw::CreateParameters & params) {
    auto gpu = params.context.castTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanGpuDraw: gpu is empty, name='{}'", name);
            return {};
        }
    return AutoRef<GpuDraw>(new GpuDrawVulkan(db, name, gpu, params));
}

} // namespace GN::rdg