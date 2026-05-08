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

#include <algorithm>
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

    // Populated during pass 1 (collectPassResources); read during pass 2 (recordDraw).
    std::vector<uint64_t> invalidResourceIds;     ///< resource IDs rejected by tracker; skip their bindings
    bool                  geometryHazard = false; ///< true if any vertex/index buffer was rejected; skip the draw
};

// Color formats for every attachment slot + depth format; built in buildAndBeginRendering,
// then forwarded to recordDraw so each pipeline matches the active dynamic rendering state.
struct PassFormats {
    std::vector<vk::Format> colors;
    vk::Format              depth = vk::Format::eUndefined;
};

static inline bool resolveColorAttachment(const GpuResourceView & v, vk::Image * outImage, vk::ImageView * outView, vk::Extent2D * outExt,
                                          vk::Format * outVkFormat) {
    if (v.empty() || !v.isTexture()) return false;
    auto * base = RuntimeType::cast<TextureVulkanBase>(v.texture().get());
    if (!base || !base->nativeImage()) return false;
    gfx::img::PixelFormat pf = v.imageView.format;
    if (pf == gfx::img::PixelFormat::UNKNOWN()) pf = base->descriptor().format;
    vk::Format fmt = pixelFormatToVkFormat(pf);
    if (fmt == vk::Format::eUndefined) return false;
    vk::ImageView view = base->nativeView(v.imageView);
    if (!view) return false;
    *outImage    = base->nativeImage();
    *outView     = view;
    *outExt      = vk::Extent2D(base->descriptor().width, base->descriptor().height);
    *outVkFormat = fmt;
    return true;
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
        : GpuPayloadVulkan(name), mRenderTarget(std::move(rt)), mDraws(std::move(draws)) {}

    void recordForVulkanSubmit(const RecordContext & ctx) override;
    void onSubmitComplete() override {
        if (mRenderPassRecorded) mStateTracker.flushStatesToResources();
    }

private:
    RasterTarget               mRenderTarget;
    ArrayContainer<StoredDraw> mDraws;
    RasterStateTrackerVulkan   mStateTracker;
    bool                       mRenderPassRecorded = false;

    // Pass 1: register the render target attachments and, for each draw, upgrade any
    // read-only attachment that the draw's merged state requires as read-write, then
    // register the draw's shader resources and geometry buffers. Must be called before
    // emitPrePassBarriers() so the single barrier uses the most permissive layout.
    // Returns false if any render target texture has a hazard; the caller should skip
    // the entire render pass in that case.
    bool collectPassResources();

    // Resolve all color and depth attachments, build vk::RenderingInfo, and call
    // beginRendering. Writes the render extent and PassFormats used in pass 2.
    // Returns false on failure.
    bool buildAndBeginRendering(vk::CommandBuffer vkcb, vk::Extent2D & outExt, PassFormats & outFormats);

    // Record one draw call into the already-active dynamic render pass.
    void recordDraw(size_t di, const StoredDraw & d, const RecordContext & ctx, rv::Ref<rv::Sampler> & defaultSampler, vk::Extent2D ext,
                    const PassFormats & formats);
};

bool GpuRasterPayloadVulkan::collectPassResources() {
    if (!mStateTracker.addRasterTarget(mRenderTarget)) return false;
    for (size_t di = 0; di < mDraws.size(); ++di) {
        StoredDraw & d = mDraws[di];
        // Promote any read-only depth-stencil attachment to read-write if this draw's
        // merged state requires writes. Must happen before registering other resources
        // so the barrier covers the promoted layout from the start.
        mStateTracker.upgradeForDrawRasterState(d.states);
        d.invalidResourceIds = mStateTracker.addGpuResourceTable(d.resources);
        if (!d.invalidResourceIds.empty()) {
            GN_ERROR(sLogger)("RasterPassPayload: draw {} has {} shader resource(s) with layout hazards; "
                              "those bindings will be skipped during recording",
                              di, d.invalidResourceIds.size());
        }
        d.geometryHazard = !mStateTracker.addRasterGeometry(d.geometry);
        if (d.geometryHazard) { GN_ERROR(sLogger)("RasterPassPayload: draw {} has vertex/index buffer layout hazard; draw will be skipped", di); }
    }
    return true;
}

bool GpuRasterPayloadVulkan::buildAndBeginRendering(vk::CommandBuffer vkcb, vk::Extent2D & outExt, PassFormats & outFormats) {
    outExt = vk::Extent2D(~0u, ~0u);

    // --- Color attachments ---
    const auto &        cc = mRenderTarget.clearColor;
    vk::ClearColorValue clearCv(std::array<float, 4> {cc.f4[0], cc.f4[1], cc.f4[2], cc.f4[3]});

    std::vector<vk::RenderingAttachmentInfo> colorAtts;
    colorAtts.reserve(mRenderTarget.colorTargets.size());
    outFormats.colors.reserve(mRenderTarget.colorTargets.size());

    for (size_t i = 0; i < mRenderTarget.colorTargets.size(); ++i) {
        vk::Image     img {};
        vk::ImageView view {};
        vk::Extent2D  ext {};
        vk::Format    fmt = vk::Format::eUndefined;
        if (!resolveColorAttachment(mRenderTarget.colorTargets[i].target, &img, &view, &ext, &fmt)) {
            GN_ERROR(sLogger)("RasterPassPayload: could not resolve color attachment {}", i);
            return false;
        }
        outExt.width  = std::min(outExt.width, ext.width);
        outExt.height = std::min(outExt.height, ext.height);
        outFormats.colors.push_back(fmt);

        vk::RenderingAttachmentInfo att;
        att.setImageView(view)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setClearValue(vk::ClearValue(clearCv));
        colorAtts.push_back(att);
    }

    if (colorAtts.empty()) {
        GN_ERROR(sLogger)("RasterPassPayload: render target has no color attachments");
        return false;
    }

    // --- Depth/stencil attachment ---
    vk::RenderingAttachmentInfo depthAtt;
    bool                        hasDepth = false;
    const auto &                dst      = mRenderTarget.depthStencilTarget;
    if (dst.isTexture() && dst.texture()) {
        auto *        depthTex  = RuntimeType::cast<TextureVulkanBase>(dst.texture().get());
        vk::ImageView depthView = depthTex ? depthTex->nativeView(dst.imageView) : vk::ImageView {};
        if (depthTex && depthView) {
            gfx::img::PixelFormat dpf = dst.imageView.format;
            if (dpf == gfx::img::PixelFormat::UNKNOWN()) dpf = depthTex->descriptor().format;
            outFormats.depth            = pixelFormatToVkFormat(dpf);
            vk::ImageLayout depthLayout = mStateTracker.attachmentPassLayout(depthTex);
            depthAtt.setImageView(depthView)
                .setImageLayout(depthLayout)
                .setLoadOp(vk::AttachmentLoadOp::eClear)
                .setStoreOp(vk::AttachmentStoreOp::eStore)
                .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(mRenderTarget.clearDepth, mRenderTarget.clearStencil)));
            outExt.width  = std::min(outExt.width, depthTex->descriptor().width);
            outExt.height = std::min(outExt.height, depthTex->descriptor().height);
            hasDepth      = true;
        }
    }

    // Guard against no valid attachment contributing an extent.
    if (outExt.width == ~0u || outExt.height == ~0u) outExt = vk::Extent2D(0, 0);

    vk::RenderingInfo ri;
    ri.setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), outExt)).setLayerCount(1).setColorAttachments(colorAtts);
    if (hasDepth) ri.setPDepthAttachment(&depthAtt);

    vkcb.beginRendering(ri);
    return true;
}

void GpuRasterPayloadVulkan::recordDraw(size_t di, const StoredDraw & d, const RecordContext & ctx, rv::Ref<rv::Sampler> & defaultSampler, vk::Extent2D ext,
                                        const PassFormats & formats) {
    // Early exit: nothing to draw.
    const RasterGeometry & geom = d.geometry;
    if (geom.vertexCount == 0 && geom.indexCount == 0) GN_UNLIKELY return;
    if (geom.instanceCount == 0 && !geom.instances.empty()) GN_UNLIKELY return;

    auto * vsVk = RuntimeType::cast<GpuShaderVulkan>(d.vs.get());
    auto * psVk = RuntimeType::cast<GpuShaderVulkan>(d.ps.get());
    if (!vsVk || !vsVk->rvShader()) GN_UNLIKELY {
            GN_ERROR(sLogger)("RasterPassPayload: draw {} missing Vulkan vertex shader", di);
            return;
        }

    // Viewport/scissor — self-contained per draw (baseline merged with overrides at record time).
    vk::CommandBuffer vkcb = ctx.cmd.handle();
    {
        vk::Viewport vp = rsViewportToVk(*d.states.viewport, ext);
        vkcb.setViewport(0, 1, &vp);
        vk::Rect2D sc = rsScissorToVk(*d.states.scissorRect, ext);
        vkcb.setScissor(0, 1, &sc);
    }

    // --- Pipeline ---
    rv::GraphicsPipeline::ConstructParameters gcp;
    gcp.setName(std::string(StrA::format("rdg2_gfx_{}", di).c_str()));
    gcp.setDynamicRendering(vk::ArrayProxy<const vk::Format>(formats.colors.size(), formats.colors.data()), formats.depth);
    gcp.setVS(vsVk->rvShader());
    if (psVk && psVk->rvShader()) gcp.setFS(psVk->rvShader());
    gcp.dynamicViewport(1).dynamicScissor(1);

    // Vertex bindings first, then instance bindings (determines Vulkan binding indices).
    for (const auto & vb : geom.vertices) gcp.addVertexBuffer(vb.stride);
    for (const auto & ib : geom.instances) gcp.addInstanceBuffer(ib.stride);
    for (const auto & a : geom.format.attributes) gcp.addVertexAttribute(a.location, (uint32_t) a.offset, vertexAttributeFormatToVk(a.format));

    applyRasterPipelineState(gcp, d.states);

    rv::Ref<const rv::GraphicsPipeline> pipeline;
    try {
        pipeline = rv::Ref<const rv::GraphicsPipeline>::make(gcp);
    } catch (const std::exception & ex) {
        GN_ERROR(sLogger)("RasterPassPayload: GraphicsPipeline creation failed: {}", ex.what());
        return;
    }
    if (!pipeline || !pipeline->handle()) GN_UNLIKELY return;

    rv::Drawable::ConstructParameters dcp;
    dcp.setPipeline(pipeline);
    rv::Drawable drawable(dcp);

    // Push constants.
    if (!d.immediates.empty()) {
        if (d.immediates.size() > 128) GN_UNLIKELY {
                GN_ERROR(sLogger)("RasterPassPayload: immediates size {} exceeds 128", d.immediates.size());
            }
        else { drawable.c(0, d.immediates.size(), d.immediates.data(), vk::ShaderStageFlagBits::eVertex); }
    }

    // --- Descriptor binding from resource table ---
    // Resources rejected by the tracker during pass 1 are in d.invalidResourceIds and skipped.
    rv::Ref<const rv::Sampler> linSampler(ensureLinearSampler(ctx.dev, defaultSampler)); // TODO: implement sampler class.
    for (size_t setIdx = 0; setIdx < d.resources.size(); ++setIdx) {
        const auto & set = d.resources[setIdx];
        for (size_t bindingIdx = 0; bindingIdx < set.size(); ++bindingIdx) {
            const auto & slot = set[bindingIdx];
            if (slot.empty()) continue;
            rv::DescriptorIdentifier descId((uint32_t) setIdx, (uint32_t) bindingIdx);
            if (slot[0].isTexture()) {
                std::vector<rv::ImageSampler> imgs;
                imgs.reserve(slot.size());
                for (const auto & view : slot) {
                    if (view.empty() || !view.isTexture()) continue;
                    auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                    if (!tex) continue;
                    if (std::find(d.invalidResourceIds.begin(), d.invalidResourceIds.end(), tex->id) != d.invalidResourceIds.end()) continue;
                    vk::ImageLayout layout =
                        (view.imageView.type == GpuResourceView::ImageView::STORAGE) ? vk::ImageLayout::eGeneral : vk::ImageLayout::eShaderReadOnlyOptimal;
                    rv::ImageSampler is;
                    is.view    = tex->nativeView(view.imageView);
                    is.layout  = layout;
                    is.sampler = linSampler;
                    imgs.push_back(is);
                }
                if (!imgs.empty()) drawable.t(descId, vk::ArrayProxy<const rv::ImageSampler>((uint32_t) imgs.size(), imgs.data()));
            } else if (slot[0].isBuffer()) {
                std::vector<rv::BufferView> bufs;
                bufs.reserve(slot.size());
                for (const auto & view : slot) {
                    if (view.empty() || !view.isBuffer()) continue;
                    auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                    if (!buf) continue;
                    if (std::find(d.invalidResourceIds.begin(), d.invalidResourceIds.end(), buf->id) != d.invalidResourceIds.end()) continue;
                    rv::BufferView bv;
                    bv.buffer = buf->rvBuffer();
                    bv.offset = (vk::DeviceSize) view.bufferView.offset;
                    bv.size   = view.bufferView.size ? (vk::DeviceSize) view.bufferView.size : vk::DeviceSize(-1);
                    bufs.push_back(bv);
                }
                if (!bufs.empty()) drawable.b(descId, vk::ArrayProxy<const rv::BufferView>((uint32_t) bufs.size(), bufs.data()));
            }
        }
    }

    // --- Vertex and instance buffer binding ---
    // Binding order mirrors gcp.addVertexBuffer / addInstanceBuffer above.
    {
        std::vector<rv::BufferView> vbViews;
        vbViews.reserve(geom.vertices.size() + geom.instances.size());
        auto pushGeomBuf = [&](const RasterGeometry::GeometryBuffer & gb) {
            rv::BufferView bv;
            if (gb.buffer) {
                if (auto * buf = RuntimeType::cast<BufferVulkan>(gb.buffer.get())) {
                    bv.buffer = buf->rvBuffer();
                    bv.offset = (vk::DeviceSize) gb.offset;
                }
            }
            vbViews.push_back(bv);
        };
        for (const auto & vb : geom.vertices) pushGeomBuf(vb);
        for (const auto & ib : geom.instances) pushGeomBuf(ib);
        if (!vbViews.empty()) drawable.v(vk::ArrayProxy<const rv::BufferView>((uint32_t) vbViews.size(), vbViews.data()));
    }

    // --- Index buffer binding ---
    if (geom.indexCount > 0 && geom.indices.buffer) {
        if (auto * buf = RuntimeType::cast<BufferVulkan>(geom.indices.buffer.get())) {
            rv::BufferView bv;
            bv.buffer             = buf->rvBuffer();
            bv.offset             = (vk::DeviceSize) geom.indices.offset;
            vk::IndexType idxType = (geom.indices.stride == 2) ? vk::IndexType::eUint16 : vk::IndexType::eUint32;
            drawable.i(bv, idxType);
        }
    }

    // --- Draw call ---
    rv::GraphicsPipeline::DrawParameters drawParams;
    if (geom.indexCount > 0) {
        drawParams.setIndexed(geom.indexCount, 0, 0);
    } else {
        drawParams.setNonIndexed(geom.vertexCount, 0);
    }
    drawParams.setInstance(geom.instanceCount);

    drawable.draw(drawParams);

    rv::Ref<const rv::DrawPack> pack = drawable.compile();
    if (!pack || pack->empty()) GN_UNLIKELY {
            GN_ERROR(sLogger)("RasterPassPayload: Drawable::compile produced empty DrawPack");
            return;
        }
    ctx.cmd.render(pack);
}

void GpuRasterPayloadVulkan::recordForVulkanSubmit(const RecordContext & ctx) {
    if (!ctx.dev || ctx.cmd.empty()) return;

    vk::CommandBuffer vkcb = ctx.cmd.handle();

    // Pass 1: collect all resource usages from every draw into the state tracker.
    // Abort if any render target texture has a hazard (same texture bound to multiple slots).
    if (!collectPassResources()) return;

    // Single pre-pass barrier: transitions all attachments, shader resources, and
    // geometry buffers into their required layouts / access masks in one call.
    mStateTracker.emitPrePassBarriers(vkcb);

    // Begin the dynamic render pass.
    vk::Extent2D ext {};
    PassFormats  formats;
    if (!buildAndBeginRendering(vkcb, ext, formats)) return;

    // Pass 2: record each draw into the active render pass. Attachment layouts were
    // already resolved to the most permissive state needed by any draw in pass 1, so
    // no per-draw barriers are required.
    rv::Ref<rv::Sampler> defaultSampler;
    for (size_t di = 0; di < mDraws.size(); ++di) {
        const StoredDraw & d = mDraws[di];
        if (d.geometryHazard) continue;
        recordDraw(di, d, ctx, defaultSampler, ext, formats);
    }

    vkcb.endRendering();
    mRenderPassRecorded = true;
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
