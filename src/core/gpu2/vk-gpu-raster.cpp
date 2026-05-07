#include "vk-gpu-raster.h"
#include <garnet/GNrdg2.h>
#include "vk-format-utils.h"
#include "gpu-context.h"
#include "vk-gpu-context.h"
#include "vk-gpu-image-state.h"
#include "vk-gpu-payload.h"
#include "vk-gpu-shader.h"
#include "vk-raster-state-tracker.h"
#include "vk-texture.h"

#include <string>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

namespace {

struct StoredDraw {
    AutoRef<GpuShader> vs, hs, ds, gs, ps;
    RasterState        states;
    RasterGeometry     geometry;
    GpuResourceTable   resources;
    DynaArray<uint8_t> immediates;
};

static inline bool resolveColorAttachment(const GpuResourceView & v, vk::Image * outImage, vk::ImageView * outView, vk::Extent2D * outExt,
                                          vk::Format * outVkFormat) {
    if (v.empty() || !v.isTexture()) return false;
    auto * base = RuntimeType::cast<TextureVulkanBase>(v.texture().get());
    if (!base || !base->nativeImage() || !base->nativeView()) return false;
    gfx::img::PixelFormat pf = v.imageView.format;
    if (pf == gfx::img::PixelFormat::UNKNOWN()) pf = base->descriptor().format;
    vk::Format fmt = pixelFormatToVkFormat(pf);
    if (fmt == vk::Format::eUndefined) return false;
    *outImage    = base->nativeImage();
    *outView     = base->nativeView();
    *outExt      = vk::Extent2D(base->descriptor().width, base->descriptor().height);
    *outVkFormat = fmt;
    return true;
}

static bool resolveFirstColorAttachment(const RasterTarget & rt, vk::Image * outImage, vk::ImageView * outView, vk::Extent2D * outExt,
                                        vk::Format * outVkFormat) {
    if (rt.colorTargets.empty()) return false;
    if (resolveColorAttachment(rt.colorTargets[0].target, outImage, outView, outExt, outVkFormat)) return true;
    GN_WARN(sLogger)("GpuRaster Vulkan: color target could not be resolved to a Vulkan texture");
    return false;
}

static rv::Sampler * ensureLinearSampler(const rv::Device * dev, rv::Ref<rv::Sampler> & slot) {
    if (slot.valid()) return slot.get();
    rv::Sampler::ConstructParameters scp;
    scp.gi = dev->gi();
    scp.setLinear();
    slot = rv::Ref<rv::Sampler>::make(scp);
    return slot.get();
}

// Merge non-empty fields from src into dst. Used once per draw at record time to fold draw-level
// overrides into the target baseline, producing a fully self-contained per-draw state.
static void mergeRenderState(RasterState & dst, const RasterState & src) {
    if (src.fillMode) dst.fillMode = src.fillMode;
    if (src.cullMode) dst.cullMode = src.cullMode;
    if (src.frontFace) dst.frontFace = src.frontFace;
    if (src.depthState) dst.depthState = src.depthState;
    if (src.stencilState) dst.stencilState = src.stencilState;
    if (src.viewport) dst.viewport = src.viewport;
    if (src.scissorRect) dst.scissorRect = src.scissorRect;
}

// Convert RasterState viewport/scissor to Vulkan, using the render extent as the fallback for FLT_MAX/~0u.
static vk::Viewport rsViewportToVk(const RasterState::Viewport & vp, vk::Extent2D ext) {
    return vk::Viewport(vp.x, vp.y, (vp.width == FLT_MAX) ? (float) ext.width : vp.width, (vp.height == FLT_MAX) ? (float) ext.height : vp.height, vp.minDepth,
                        vp.maxDepth);
}

static vk::Rect2D rsScissorToVk(const RasterState::ScissorRect & sr, vk::Extent2D ext) {
    return vk::Rect2D(vk::Offset2D(sr.x, sr.y),
                      vk::Extent2D((sr.width == (~0u)) ? ext.width : (uint32_t) sr.width, (sr.height == (~0u)) ? ext.height : (uint32_t) sr.height));
}

static void applyRasterPipelineState(rv::GraphicsPipeline::ConstructParameters & gcp, const RasterState & rs) {
    if (rs.fillMode && *rs.fillMode == RasterState::FILL_WIREFRAME) gcp.rast.setPolygonMode(vk::PolygonMode::eLine);
    if (rs.cullMode) {
        switch (*rs.cullMode) {
        case RasterState::CULL_NONE:
            gcp.rast.setCullMode(vk::CullModeFlagBits::eNone);
            break;
        case RasterState::CULL_FRONT:
            gcp.rast.setCullMode(vk::CullModeFlagBits::eFront);
            break;
        case RasterState::CULL_BACK:
            gcp.rast.setCullMode(vk::CullModeFlagBits::eBack);
            break;
        }
    }
    if (rs.frontFace && *rs.frontFace == RasterState::FRONT_CW) gcp.rast.setFrontFace(vk::FrontFace::eClockwise);
}

class GpuRasterPayloadVulkan final : public GpuPayloadVulkan {
public:
    GpuRasterPayloadVulkan(const StrA & name, RasterTarget rt, ArrayContainer<StoredDraw> draws)
        : GpuPayloadVulkan(name), mRenderTarget(std::move(rt)), mDraws(std::move(draws)) {
        for (const auto & ct : mRenderTarget.colorTargets) {
            if (!ct.target.isTexture() || !ct.target.texture()) GN_UNLIKELY continue;
            if (auto * base = RuntimeType::cast<TextureVulkanBase>(ct.target.texture().get())) mStateTracker.addColorTarget(base, ct.target);
        }
        if (mRenderTarget.depthStencilTarget.isTexture() && mRenderTarget.depthStencilTarget.texture()) {
            if (auto * base = RuntimeType::cast<TextureVulkanBase>(mRenderTarget.depthStencilTarget.texture().get())) GN_LIKELY {
                    const auto & ds = mRenderTarget.states;
                    bool         ro = ds.depthState && !ds.depthState->writeEnabled() && !(ds.stencilState && ds.stencilState->enabled());
                    mStateTracker.addDepthStencilTarget(base, mRenderTarget.depthStencilTarget, ro);
                }
        }
    }

    void recordForVulkanSubmit(const RecordContext & ctx) override;
    void onSubmitComplete() override { mStateTracker.flushStatesToResources(); }

private:
    RasterTarget               mRenderTarget;
    ArrayContainer<StoredDraw> mDraws;
    RasterStateTrackerVulkan   mStateTracker;
};

void GpuRasterPayloadVulkan::recordForVulkanSubmit(const RecordContext & ctx) {
    if (!ctx.dev || ctx.cmd.empty()) return;

    vk::CommandBuffer vkcb = ctx.cmd.handle();

    vk::Image     colorImage {};
    vk::ImageView colorView {};
    vk::Extent2D  ext {};
    vk::Format    colorFmt = vk::Format::eUndefined;
    if (!resolveFirstColorAttachment(mRenderTarget, &colorImage, &colorView, &ext, &colorFmt)) {
        GN_ERROR(sLogger)("RasterPassPayload: could not resolve color attachment");
        return;
    }

    mStateTracker.emitPrePassBarriers(vkcb);

    const auto &        cc = mRenderTarget.clearColor;
    vk::ClearColorValue clearCv(std::array<float, 4> {cc.f4[0], cc.f4[1], cc.f4[2], cc.f4[3]});

    vk::RenderingAttachmentInfo colorAtt;
    colorAtt.setImageView(colorView)
        .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setClearValue(vk::ClearValue(clearCv));

    vk::RenderingInfo ri;
    ri.setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), ext)).setLayerCount(1).setColorAttachments(colorAtt);

    vkcb.beginRendering(ri);

    rv::Ref<rv::Sampler> defaultSampler;

    for (size_t di = 0; di < mDraws.size(); ++di) {
        const StoredDraw & d    = mDraws[di];
        auto *             vsVk = RuntimeType::cast<GpuShaderVulkan>(d.vs.get());
        auto *             psVk = RuntimeType::cast<GpuShaderVulkan>(d.ps.get());
        if (!vsVk || !vsVk->rvShader()) {
            GN_ERROR(sLogger)("RasterPassPayload: draw {} missing Vulkan vertex shader", di);
            continue;
        }

        // Each draw's state is fully self-contained (target baseline merged with draw overrides at record
        // time). State changes are transient: they apply only to this draw and do not affect later draws.
        {
            vk::Viewport vp = rsViewportToVk(*d.states.viewport, ext);
            vkcb.setViewport(0, 1, &vp);
            vk::Rect2D sc = rsScissorToVk(*d.states.scissorRect, ext);
            vkcb.setScissor(0, 1, &sc);
        }

        rv::GraphicsPipeline::ConstructParameters gcp;
        gcp.setName(std::string(StrA::format("rdg2_gfx_{}", di).c_str()));
        gcp.setDynamicRendering(colorFmt, vk::Format::eUndefined);
        gcp.setVS(vsVk->rvShader());
        if (psVk && psVk->rvShader()) gcp.setFS(psVk->rvShader());
        gcp.dynamicViewport(1).dynamicScissor(1);

        const RasterGeometry & geom = d.geometry;
        for (size_t bi = 0; bi < geom.vertices.size(); ++bi) { gcp.addVertexBuffer(geom.vertices[bi].stride); }
        for (size_t ai = 0; ai < geom.format.attributes.size(); ++ai) {
            const RasterGeometry::VertexAttribute & a = geom.format.attributes[ai];
            gcp.addVertexAttribute(a.location, (uint32_t) a.offset, vertexAttributeFormatToVk(a.format));
        }

        applyRasterPipelineState(gcp, d.states);

        rv::Ref<const rv::GraphicsPipeline> pipeline;
        try {
            pipeline = rv::Ref<const rv::GraphicsPipeline>::make(gcp);
        } catch (const std::exception & ex) {
            GN_ERROR(sLogger)("RasterPassPayload: GraphicsPipeline creation failed: {}", ex.what());
            continue;
        }
        if (!pipeline || !pipeline->handle()) continue;

        rv::Drawable::ConstructParameters dcp;
        dcp.setPipeline(pipeline);
        rv::Drawable drawable(dcp);

        if (!d.immediates.empty()) {
            if (d.immediates.size() > 128) {
                GN_ERROR(sLogger)("RasterPassPayload: immediates size {} exceeds 128", d.immediates.size());
            } else {
                drawable.c(0, d.immediates.size(), d.immediates.data(), vk::ShaderStageFlagBits::eVertex);
            }
        }

        (void) ensureLinearSampler(ctx.dev, defaultSampler);
        if (!d.resources.empty()) {
            GN_WARN(sLogger)("RasterPassPayload: descriptor binding from GpuResourceTable is not implemented yet; draw may fail if shaders require bindings");
        }

        if (geom.indexCount > 0 && geom.indices.buffer) {
            GN_WARN(sLogger)("RasterPassPayload: indexed draws require Vulkan buffer binding (not implemented); skipping draw");
            continue;
        }
        if (!geom.vertices.empty() && geom.vertices[0].buffer) {
            GN_WARN(sLogger)("RasterPassPayload: vertex buffer draws require Vulkan buffer binding (not implemented); skipping draw");
            continue;
        }

        if (geom.vertexCount == 0 && geom.indexCount == 0) {
            GN_VERBOSE(sLogger)("RasterPassPayload: draw {} has zero vertices and indices; skipping", di);
            continue;
        }

        rv::GraphicsPipeline::DrawParameters drawParams;
        if (geom.indexCount > 0) {
            drawParams.setIndexed(geom.indexCount, 0, 0);
        } else {
            drawParams.setNonIndexed(geom.vertexCount, 0);
        }
        drawable.draw(drawParams);

        rv::Ref<const rv::DrawPack> pack = drawable.compile();
        if (!pack || pack->empty()) {
            GN_ERROR(sLogger)("RasterPassPayload: Drawable::compile produced empty DrawPack");
            continue;
        }
        ctx.cmd.render(pack);
    }

    vkcb.endRendering();
}

class GpuRasterVulkan2 final : public GpuRaster {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuRaster);

    GpuRasterVulkan2(const StrA & entityName, CreateParameters cp): GpuRaster(TYPE_INFO(), entityName), mCreateParams(std::move(cp)) {
        mDraws.reserve(mCreateParams.numberOfDrawsHint);
    }

    const RasterTarget & target() const override { return mCreateParams.target; }

    void draw(const DrawParameters & dp) override {
        if (mSealed) {
            GN_ERROR(sLogger)("GpuRasterVulkan2::draw: already sealed");
            return;
        }
        mDraws.resize(mDraws.size() + 1);
        StoredDraw & s = mDraws.back();
        s.vs           = dp.vs;
        s.hs           = dp.hs;
        s.ds           = dp.ds;
        s.gs           = dp.gs;
        s.ps           = dp.ps;
        s.geometry     = dp.geometry;
        s.resources    = dp.resources;
        s.states       = mCreateParams.target.states;
        mergeRenderState(s.states, dp.states);
        s.immediates.clear();
        if (dp.immediates.data() && dp.immediates.size() > 0) { s.immediates.append(dp.immediates.data(), (size_t) dp.immediates.size()); }
    }

    AutoRef<GpuPayload> seal() override {
        if (mSealed) {
            GN_ERROR(sLogger)("GpuRasterVulkan2::seal: double seal");
            return {};
        }
        mSealed = true;
        return AutoRef<GpuPayload>(new GpuRasterPayloadVulkan(name + "/payload", std::move(mCreateParams.target), std::move(mDraws)));
    }

private:
    CreateParameters           mCreateParams;
    bool                       mSealed = false;
    ArrayContainer<StoredDraw> mDraws;
};

} // namespace

AutoRef<GpuRaster> createGpuRasterVulkan2(const GpuRaster::CreateParameters & params) {
    if (!params.gpu) return {};
    AutoRef<GpuContextVulkan2> vkGpu = params.gpu.staticCastTo<GpuContextVulkan2>();
    if (!vkGpu || !vkGpu->ready()) return {};
    StrA n = params.gpu->name.empty() ? StrA("raster_pass") : params.gpu->name + "/raster";
    return AutoRef<GpuRaster>(new GpuRasterVulkan2(n, params));
}

} // namespace GN::gpu2
