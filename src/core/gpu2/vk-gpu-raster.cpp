#include "pch.h"
#include "vk-gpu-raster.h"
#include "vk-format-utils.h"
#include "gpu-context.h"
#include "vk-raster-pso-factory.h" // includes vk-gpu-context.h + vk-gpu-shader.h
#include "vk-gpu-payload.h"
#include "vk-gpu-resource-state-tracker.h"
#include "vk-texture.h"

#include <algorithm>
#include <string>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.raster");

namespace GN::gpu2 {

namespace {

struct StoredDraw {
    AutoRef<GpuShader>  vs, hs, ds, gs, ps;
    uint32_t            stateIndex         = 0;
    uint32_t            geometryIndex      = 0;
    uint32_t            resourceTableIndex = ~0u;
    AutoRef<const Blob> immediates;
};

// PassFormats is defined in vk-raster-pso-factory.h (shared with the PSO factory).

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
    // Roughness selects prefiltered environment mips; Vulkan's zero default
    // otherwise clamps every explicit shader LOD to the sharpest level.
    scp.info.maxLod = VK_LOD_CLAMP_NONE;
    slot            = rv::Ref<rv::Sampler>::make(scp);
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

// State → Vulkan conversion helpers are in vk-raster-pso-factory.cpp (single authoritative source).

static AutoRef<GpuContextVulkan2> checkGpu(const AutoRef<GpuContext> & gpu) {
    AutoRef<GpuContextVulkan2> vkGpu = RuntimeType::cast<GpuContextVulkan2>(gpu);
    if (!vkGpu || !vkGpu->ready()) { GN_ERROR(sLogger, "Failed to create raster object: null/invalid GPU pointer."); }
    return vkGpu;
}

static RasterTarget checkRasterTarget(const RasterTarget * target) {
    if (!target) {
        GN_ERROR(sLogger, "GpuRasterVulkan2: no valid raster target provided");
        return {};
    }
    if (target->empty()) {
        GN_ERROR(sLogger, "GpuRasterVulkan2: no color/depth target defined.");
        return {};
    }
    return *target;
}

class GpuRasterPayloadVulkan final : public GpuPayloadVulkan {
public:
    GpuRasterPayloadVulkan(const StrA & name, RasterPsoFactory * factory, RasterTarget rt, DynaArray<StoredDraw> draws, DynaArray<RasterGeometry> geoms,
                           DynaArray<GpuResourceTable> resTables, DynaArray<RasterState> states)
        : GpuPayloadVulkan(name), mPsoFactory(factory), mRenderTarget(std::move(rt)), mDraws(std::move(draws)), mGeometries(std::move(geoms)),
          mResourceTables(std::move(resTables)), mStates(std::move(states)) {}

    void recordForVulkanSubmit(const RecordContext & ctx) override;

private:
    RasterPsoFactory *                mPsoFactory = nullptr; // owned by GpuContextVulkan2; lifetime > this payload
    RasterTarget                      mRenderTarget;
    DynaArray<StoredDraw>             mDraws;
    DynaArray<RasterGeometry>         mGeometries;
    DynaArray<GpuResourceTable>       mResourceTables;
    DynaArray<RasterState>            mStates;
    std::vector<bool>                 mGeomHazards;
    std::vector<std::vector<int64_t>> mTableInvalidIds;

    // Pass 1: register render targets and per-draw resources into the batch tracker.
    // Returns false if any render target has a hazard; the caller should skip the pass.
    bool collectPassResources(GpuResourceStateTrackerVulkan & tracker);

    // Resolve attachments, build vk::RenderingInfo, and call beginRendering.
    // Returns false on failure.
    bool buildAndBeginRendering(vk::CommandBuffer vkcb, GpuResourceStateTrackerVulkan & tracker, vk::Extent2D & outExt, PassFormats & outFormats);

    // Record one draw call into the already-active dynamic render pass.
    rv::Ref<const rv::DrawPack> recordDraw(size_t di, const StoredDraw & d, const RecordContext & ctx, rv::Ref<rv::Sampler> & defaultSampler, vk::Extent2D ext,
                                           const PassFormats & formats);
};

static inline bool sameGeometry(const RasterGeometry & a, const RasterGeometry & b) {
    if (a.vertexCount != b.vertexCount || a.indexCount != b.indexCount || a.instanceCount != b.instanceCount) return false;
    if (a.indices.buffer.get() != b.indices.buffer.get() || a.indices.offset != b.indices.offset || a.indices.stride != b.indices.stride) return false;
    if (a.vertices.size() != b.vertices.size() || a.instances.size() != b.instances.size()) return false;
    for (size_t i = 0; i < a.vertices.size(); ++i) {
        if (a.vertices[i].buffer.get() != b.vertices[i].buffer.get() || a.vertices[i].offset != b.vertices[i].offset ||
            a.vertices[i].stride != b.vertices[i].stride)
            return false;
    }
    for (size_t i = 0; i < a.instances.size(); ++i) {
        if (a.instances[i].buffer.get() != b.instances[i].buffer.get() || a.instances[i].offset != b.instances[i].offset ||
            a.instances[i].stride != b.instances[i].stride)
            return false;
    }
    if (a.format != b.format) return false;
    return true;
}

static inline bool sameResources(const GpuResourceTable & a, const GpuResourceTable & b) {
    if (a.size() != b.size()) return false;
    for (size_t s = 0; s < a.size(); ++s) {
        if (a[s].size() != b[s].size()) return false;
        for (size_t slot = 0; slot < a[s].size(); ++slot) {
            if (a[s][slot].size() != b[s][slot].size()) return false;
            for (size_t i = 0; i < a[s][slot].size(); ++i) {
                if (a[s][slot][i] != b[s][slot][i]) return false;
            }
        }
    }
    return true;
}

static inline bool isReadOnlyResourceTable(const GpuResourceTable & table) {
    for (const auto & set : table) {
        for (const auto & slot : set) {
            for (const auto & view : slot) {
                if (view.empty()) continue;
                if (view.isTexture() && view.imageView.type == GpuResourceView::ImageView::STORAGE) return false;
                if (view.isBuffer() && view.bufferView.type == GpuResourceView::BufferView::STORAGE) return false;
            }
        }
    }
    return true;
}

bool GpuRasterPayloadVulkan::collectPassResources(GpuResourceStateTrackerVulkan & tracker) {
    if (!tracker.addRasterTarget(mRenderTarget)) return false;

    // Promote read-only depth-stencil attachment to read-write if any draw writes depth/stencil.
    for (const auto & s : mStates) {
        if ((s.depthState && s.depthState->writeEnabled()) || (s.stencilState && s.stencilState->enabled())) {
            tracker.upgradeForDrawRasterState(s);
            break;
        }
    }

    // 1. Process unique geometries
    mGeomHazards.resize(mGeometries.size());
    for (size_t g = 0; g < mGeometries.size(); ++g) {
        mGeomHazards[g] = !tracker.addRasterGeometry(mGeometries[g]);
        if (mGeomHazards[g]) { GN_ERROR(sLogger, "RasterPassPayload: geometry {} has vertex/index buffer layout hazard", g); }
    }

    // 2. Process unique resource tables
    mTableInvalidIds.resize(mResourceTables.size());
    for (size_t r = 0; r < mResourceTables.size(); ++r) {
        mTableInvalidIds[r] = tracker.addGpuResourceTable(mResourceTables[r]);
        if (!mTableInvalidIds[r].empty()) {
            GN_ERROR(sLogger, "RasterPassPayload: resource table {} has {} shader resource(s) with layout hazards", r, mTableInvalidIds[r].size());
        }
    }

    return true;
}

bool GpuRasterPayloadVulkan::buildAndBeginRendering(vk::CommandBuffer vkcb, GpuResourceStateTrackerVulkan & tracker, vk::Extent2D & outExt,
                                                    PassFormats & outFormats) {
    outExt = vk::Extent2D(~0u, ~0u);

    // --- Color attachments ---
    const auto &        cc = mRenderTarget.clearColor;
    vk::ClearColorValue clearCv(std::array<float, 4> {cc.f4[0], cc.f4[1], cc.f4[2], cc.f4[3]});

    std::vector<vk::RenderingAttachmentInfo> colorAtts;
    colorAtts.reserve(mRenderTarget.colorTargets.size());
    outFormats.colors.reserve(mRenderTarget.colorTargets.size());

    for (size_t i = 0; i < mRenderTarget.colorTargets.size(); ++i) {
        vk::Image             img {};
        vk::ImageView         view {};
        vk::Extent2D          ext {};
        vk::Format            fmt    = vk::Format::eUndefined;
        const GpuResourceView ctView = mRenderTarget.colorTargets[i].view();
        if (!resolveColorAttachment(ctView, &img, &view, &ext, &fmt)) {
            GN_ERROR(sLogger, "RasterPassPayload: could not resolve color attachment {}", i);
            return false;
        }
        outExt.width  = std::min(outExt.width, ext.width);
        outExt.height = std::min(outExt.height, ext.height);
        outFormats.colors.push_back(fmt);

        // Verify the pre-pass barrier landed the attachment in the expected layout.
        GN_ASSERT(tracker.texturePassLayout(RuntimeType::cast<TextureVulkanBase>(ctView.texture().get()), ctView.imageView.range.i.mip,
                                            ctView.imageView.range.i.face) == vk::ImageLayout::eColorAttachmentOptimal);

        vk::RenderingAttachmentInfo att;
        att.setImageView(view)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setClearValue(vk::ClearValue(clearCv));
        colorAtts.push_back(att);
    }

    if (colorAtts.empty()) {
        GN_ERROR(sLogger, "RasterPassPayload: render target has no color attachments");
        return false;
    }

    // --- Depth/stencil attachment ---
    vk::RenderingAttachmentInfo depthAtt;
    bool                        hasDepth = false;
    const GpuResourceView       dst      = mRenderTarget.depthStencilTarget.view();
    if (dst.isTexture() && dst.texture()) {
        auto *        depthTex  = RuntimeType::cast<TextureVulkanBase>(dst.texture().get());
        vk::ImageView depthView = depthTex ? depthTex->nativeView(dst.imageView) : vk::ImageView {};
        if (depthTex && depthView) {
            gfx::img::PixelFormat dpf = dst.imageView.format;
            if (dpf == gfx::img::PixelFormat::UNKNOWN()) dpf = depthTex->descriptor().format;
            outFormats.depth            = pixelFormatToVkFormat(dpf);
            vk::ImageLayout depthLayout = tracker.texturePassLayout(depthTex, dst.imageView.range.i.mip, dst.imageView.range.i.face);
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

rv::Ref<const rv::DrawPack> GpuRasterPayloadVulkan::recordDraw(size_t di, const StoredDraw & d, const RecordContext & ctx,
                                                               rv::Ref<rv::Sampler> & defaultSampler, vk::Extent2D ext, const PassFormats & formats) {
    // Early exit: nothing to draw.
    const RasterGeometry & geom = mGeometries[d.geometryIndex];
    if (geom.vertexCount == 0 && geom.indexCount == 0) GN_UNLIKELY return {};
    if (geom.instanceCount == 0 && !geom.instances.empty()) GN_UNLIKELY return {};

    auto * vsVk = RuntimeType::cast<GpuShaderVulkan>(d.vs.get());
    auto * psVk = RuntimeType::cast<GpuShaderVulkan>(d.ps.get());
    if (!vsVk || !vsVk->rvShader()) GN_UNLIKELY {
            GN_ERROR(sLogger, "RasterPassPayload: draw {} missing Vulkan vertex shader", di);
            return {};
        }

    // Viewport/scissor — self-contained per draw (baseline merged with overrides at record time).
    const RasterState & drawStates = mStates[d.stateIndex];
    vk::CommandBuffer   vkcb       = ctx.cmd.handle();
    if (drawStates.viewport && drawStates.scissorRect) {
        vk::Viewport vp = rsViewportToVk(*drawStates.viewport, ext);
        vkcb.setViewport(0, 1, &vp);
        vk::Rect2D sc = rsScissorToVk(*drawStates.scissorRect, ext);
        vkcb.setScissor(0, 1, &sc);
    }

    // --- Pipeline (get-or-create from factory) ---
    if (!mPsoFactory) GN_UNLIKELY {
            GN_ERROR(sLogger, "RasterPassPayload: draw {} has no PSO factory", di);
            return {};
        }
    Gpu2RasterPsoCreateParams psoParams {
        .vs           = vsVk,
        .ps           = psVk,
        .state        = drawStates,
        .geometry     = geom,
        .formats      = formats,
        .colorTargets = mRenderTarget.colorTargets,
    };
    rv::Ref<const rv::GraphicsPipeline> pipeline = mPsoFactory->getOrCreate(psoParams);
    if (!pipeline || !pipeline->handle()) GN_UNLIKELY return {};

    rv::Drawable::ConstructParameters dcp;
    dcp.setPipeline(pipeline);
    rv::Drawable drawable(dcp);

    // Push constants.
    if (d.immediates && !d.immediates->empty()) {
        if (d.immediates->size() > 128) GN_UNLIKELY {
                GN_ERROR(sLogger, "RasterPassPayload: immediates size {} exceeds 128", d.immediates->size());
            }
        // eAllGraphics so fragment shaders can also declare layout(push_constant) blocks.
        else { drawable.c(0, d.immediates->size(), d.immediates->data(), vk::ShaderStageFlagBits::eAllGraphics); }
    }

    // --- Descriptor binding from resource table ---
    // Resources rejected by the tracker during pass 1 are in mTableInvalidIds and skipped.
    static const std::vector<int64_t> sEmptyInvalidIds;
    const auto & invalidResourceIds = (d.resourceTableIndex < mTableInvalidIds.size()) ? mTableInvalidIds[d.resourceTableIndex] : sEmptyInvalidIds;
    rv::Ref<const rv::Sampler> linSampler(ensureLinearSampler(ctx.dev, defaultSampler)); // TODO: implement sampler class.
    if (d.resourceTableIndex < mResourceTables.size()) {
        const auto & resTable = mResourceTables[d.resourceTableIndex];
        for (size_t setIdx = 0; setIdx < resTable.size(); ++setIdx) {
            const auto & set = resTable[setIdx];
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
                        if (std::find(invalidResourceIds.begin(), invalidResourceIds.end(), tex->id) != invalidResourceIds.end()) continue;
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
                        if (std::find(invalidResourceIds.begin(), invalidResourceIds.end(), buf->id) != invalidResourceIds.end()) continue;
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
            GN_ERROR(sLogger, "RasterPassPayload: Drawable::compile produced empty DrawPack");
            return {};
        }
    ctx.cmd.render(pack);
    return pack;
}

struct CachedDrawConfig {
    rv::Ref<const rv::Pipeline>    pipeline;
    rv::Ref<const rv::DrawPack>    pack;
    const StoredDraw *             firstDraw = nullptr;
    std::vector<vk::DescriptorSet> descriptorSets;
    std::vector<vk::Buffer>        vertexBuffers;
    std::vector<vk::DeviceSize>    vertexOffsets;
    vk::Buffer                     indexBuffer {};
    vk::DeviceSize                 indexOffset   = 0;
    vk::IndexType                  indexType     = vk::IndexType::eUint16;
    uint32_t                       indexCount    = 0;
    uint32_t                       vertexCount   = 0;
    uint32_t                       instanceCount = 1;
};

static inline bool matchesConfig(const StoredDraw & d, const CachedDrawConfig & cfg) {
    if (!cfg.firstDraw) return false;
    const StoredDraw & f = *cfg.firstDraw;
    return d.vs.get() == f.vs.get() && d.ps.get() == f.ps.get() && d.stateIndex == f.stateIndex && d.geometryIndex == f.geometryIndex &&
           d.resourceTableIndex == f.resourceTableIndex;
}

void GpuRasterPayloadVulkan::recordForVulkanSubmit(const RecordContext & ctx) {
    if (!ctx.dev || ctx.cmd.empty() || !ctx.batchTracker) return;

    GpuResourceStateTrackerVulkan & tracker = *ctx.batchTracker;
    vk::CommandBuffer               vkcb    = ctx.cmd.handle();

    // Pass 1: register this pass's resources into the shared batch tracker.
    // The tracker already holds committed state from prior payloads, so it produces
    if (!collectPassResources(tracker)) return;

    // Single pre-pass barrier covering all attachments and shader resources.
    tracker.emitPrePassBarriers(vkcb);

    // Begin the dynamic render pass.
    vk::Extent2D ext {};
    PassFormats  formats;
    if (!buildAndBeginRendering(vkcb, tracker, ext, formats)) return;

    rv::Ref<rv::Sampler> defaultSampler;

    std::vector<CachedDrawConfig>  configs;
    int                            activeConfigIndex = -1;
    vk::Pipeline                   boundPipeline {};
    std::vector<vk::DescriptorSet> boundSets;
    std::vector<vk::Buffer>        boundVertexBuffers;
    std::vector<vk::DeviceSize>    boundVertexOffsets;
    vk::Buffer                     boundIndexBuffer {};
    vk::DeviceSize                 boundIndexOffset = 0;
    vk::IndexType                  boundIndexType   = vk::IndexType::eUint16;
    RasterState::Viewport          boundVp {};
    RasterState::ScissorRect       boundSc {};
    bool                           hasBoundVpSc = false;

    for (size_t di = 0; di < mDraws.size(); ++di) {
        const StoredDraw & d = mDraws[di];
        if (d.geometryIndex < mGeomHazards.size() && mGeomHazards[d.geometryIndex]) continue;

        // Fastest path: matches current active hardware state.
        if (activeConfigIndex >= 0 && matchesConfig(d, configs[activeConfigIndex])) {
            const auto & cfg = configs[activeConfigIndex];
            if (d.immediates && !d.immediates->empty()) {
                if (d.immediates->size() > 128) GN_UNLIKELY {
                        GN_ERROR(sLogger, "RasterPassPayload: immediates size {} exceeds 128", d.immediates->size());
                    }
                else {
                    vkcb.pushConstants(cfg.pipeline->layout(), vk::ShaderStageFlagBits::eAllGraphics, 0, (uint32_t) d.immediates->size(), d.immediates->data());
                }
            }
            if (cfg.indexCount > 0) {
                vkcb.drawIndexed(cfg.indexCount, cfg.instanceCount, 0, 0, 0);
            } else {
                vkcb.draw(cfg.vertexCount, cfg.instanceCount, 0, 0);
            }
            continue;
        }

        // Fast path: matches a previously cached configuration.
        int foundIndex = -1;
        for (size_t c = 0; c < configs.size(); ++c) {
            if ((int) c == activeConfigIndex) continue;
            if (matchesConfig(d, configs[c])) {
                foundIndex = (int) c;
                break;
            }
        }

        if (foundIndex >= 0) {
            const auto & cfg     = configs[foundIndex];
            const auto & dStates = mStates[d.stateIndex];

            // Viewport & scissor
            if (dStates.viewport && (!hasBoundVpSc || boundVp != *dStates.viewport)) {
                vk::Viewport vp = rsViewportToVk(*dStates.viewport, ext);
                vkcb.setViewport(0, 1, &vp);
                boundVp = *dStates.viewport;
            }
            if (dStates.scissorRect && (!hasBoundVpSc || boundSc != *dStates.scissorRect)) {
                vk::Rect2D sc = rsScissorToVk(*dStates.scissorRect, ext);
                vkcb.setScissor(0, 1, &sc);
                boundSc = *dStates.scissorRect;
            }
            hasBoundVpSc = true;

            // Pipeline
            if (boundPipeline != cfg.pipeline->handle()) {
                vkcb.bindPipeline(vk::PipelineBindPoint::eGraphics, cfg.pipeline->handle());
                boundPipeline = cfg.pipeline->handle();
                boundSets.clear();
            }

            // Descriptors
            if (boundSets.size() < cfg.descriptorSets.size()) boundSets.resize(cfg.descriptorSets.size());
            for (uint32_t s = 0; s < (uint32_t) cfg.descriptorSets.size(); ++s) {
                if (cfg.descriptorSets[s] && cfg.descriptorSets[s] != boundSets[s]) {
                    vkcb.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, cfg.pipeline->layout(), s, 1, &cfg.descriptorSets[s], 0, nullptr);
                    boundSets[s] = cfg.descriptorSets[s];
                }
            }

            // Vertex buffers
            if (!cfg.vertexBuffers.empty() && (cfg.vertexBuffers != boundVertexBuffers || cfg.vertexOffsets != boundVertexOffsets)) {
                vkcb.bindVertexBuffers(0, (uint32_t) cfg.vertexBuffers.size(), cfg.vertexBuffers.data(), cfg.vertexOffsets.data());
                boundVertexBuffers = cfg.vertexBuffers;
                boundVertexOffsets = cfg.vertexOffsets;
            }

            // Index buffer
            if (cfg.indexBuffer && (cfg.indexBuffer != boundIndexBuffer || cfg.indexOffset != boundIndexOffset || cfg.indexType != boundIndexType)) {
                vkcb.bindIndexBuffer(cfg.indexBuffer, cfg.indexOffset, cfg.indexType);
                boundIndexBuffer = cfg.indexBuffer;
                boundIndexOffset = cfg.indexOffset;
                boundIndexType   = cfg.indexType;
            }

            // Push constants
            if (d.immediates && !d.immediates->empty()) {
                if (d.immediates->size() > 128) GN_UNLIKELY {
                        GN_ERROR(sLogger, "RasterPassPayload: immediates size {} exceeds 128", d.immediates->size());
                    }
                else {
                    vkcb.pushConstants(cfg.pipeline->layout(), vk::ShaderStageFlagBits::eAllGraphics, 0, (uint32_t) d.immediates->size(), d.immediates->data());
                }
            }

            // Draw
            if (cfg.indexCount > 0) {
                vkcb.drawIndexed(cfg.indexCount, cfg.instanceCount, 0, 0, 0);
            } else {
                vkcb.draw(cfg.vertexCount, cfg.instanceCount, 0, 0);
            }

            activeConfigIndex = foundIndex;
            continue;
        }

        // Slow path: first time encountering this configuration in this pass.
        auto pack = recordDraw(di, d, ctx, defaultSampler, ext, formats);
        if (!pack || !pack->pipeline) continue;

        const auto & geom    = mGeometries[d.geometryIndex];
        const auto & dStates = mStates[d.stateIndex];

        CachedDrawConfig newCfg;
        newCfg.pipeline  = pack->pipeline;
        newCfg.pack      = pack;
        newCfg.firstDraw = &d;
        newCfg.descriptorSets.resize(pack->descriptors.size());
        for (size_t s = 0; s < pack->descriptors.size(); ++s) {
            if (!pack->descriptors[s].writes.empty()) { newCfg.descriptorSets[s] = pack->descriptors[s].writes[0].dstSet; }
        }
        newCfg.vertexBuffers.resize(pack->vertexBuffers.size());
        for (size_t i = 0; i < pack->vertexBuffers.size(); ++i) {
            newCfg.vertexBuffers[i] = pack->vertexBuffers[i] ? pack->vertexBuffers[i]->handle() : vk::Buffer {};
        }
        newCfg.vertexOffsets = pack->vertexOffsets;
        newCfg.indexBuffer   = pack->indexBuffer ? pack->indexBuffer->handle() : vk::Buffer {};
        newCfg.indexOffset   = pack->indexOffset;
        newCfg.indexType     = pack->indexType;
        newCfg.indexCount    = geom.indexCount;
        newCfg.vertexCount   = geom.vertexCount;
        newCfg.instanceCount = geom.instanceCount;

        // Update hardware tracking state
        boundPipeline      = newCfg.pipeline->handle();
        boundSets          = newCfg.descriptorSets;
        boundVertexBuffers = newCfg.vertexBuffers;
        boundVertexOffsets = newCfg.vertexOffsets;
        boundIndexBuffer   = newCfg.indexBuffer;
        boundIndexOffset   = newCfg.indexOffset;
        boundIndexType     = newCfg.indexType;
        if (dStates.viewport) boundVp = *dStates.viewport;
        if (dStates.scissorRect) boundSc = *dStates.scissorRect;
        hasBoundVpSc = true;

        configs.push_back(std::move(newCfg));
        activeConfigIndex = (int) configs.size() - 1;
    }

    vkcb.endRendering();
}

static inline bool isRasterStateEmpty(const RasterState & s) {
    return !s.fillMode && !s.cullMode && !s.frontFace && !s.depthState && !s.stencilState && !s.viewport && !s.scissorRect;
}

class GpuRasterVulkan2 final : public GpuRaster {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuRaster);

    GpuRasterVulkan2(const StrA & entityName, const CreateParameters & cp)
        : GpuRaster(TYPE_INFO(), entityName), mGpu(checkGpu(cp.gpu)), mRenderTarget(checkRasterTarget(cp.target)) {
        if (!mGpu) return;
        if (mRenderTarget.empty()) return;
        mDraws.reserve(cp.numberOfDrawsHint);
        mStates.append(mRenderTarget.states);
        mValid = true;
    }

    bool valid() const { return mValid; }

    const RasterTarget & target() const override { return mRenderTarget; }

    void draw(const DrawParameters & dp) override {
        if (mSealed) {
            GN_ERROR(sLogger, "GpuRasterVulkan2::draw: already sealed");
            return;
        }

        uint32_t stateIdx = 0;
        if (!isRasterStateEmpty(dp.states)) {
            RasterState merged = mRenderTarget.states;
            mergeRenderState(merged, dp.states);
            if (mLastStateIndex < mStates.size() && mStates[mLastStateIndex] == merged) {
                stateIdx = mLastStateIndex;
            } else {
                bool found = false;
                for (uint32_t i = 0; i < (uint32_t) mStates.size(); ++i) {
                    if (mStates[i] == merged) {
                        stateIdx = i;
                        found    = true;
                        break;
                    }
                }
                if (!found) {
                    stateIdx = (uint32_t) mStates.size();
                    mStates.append(std::move(merged));
                }
                mLastStateIndex = stateIdx;
            }
        }

        uint32_t geomIdx = 0;
        if (mLastGeometryIndex < mGeometries.size() && sameGeometry(mGeometries[mLastGeometryIndex], dp.geometry)) {
            geomIdx = mLastGeometryIndex;
        } else {
            bool found = false;
            for (uint32_t i = 0; i < (uint32_t) mGeometries.size(); ++i) {
                if (sameGeometry(mGeometries[i], dp.geometry)) {
                    geomIdx = i;
                    found   = true;
                    break;
                }
            }
            if (!found) {
                geomIdx = (uint32_t) mGeometries.size();
                mGeometries.append(dp.geometry);
            }
            mLastGeometryIndex = geomIdx;
        }

        uint32_t resIdx = ~0u;
        if (!dp.resources.empty()) {
            if (mLastResourceIndex < mResourceTables.size() && sameResources(mResourceTables[mLastResourceIndex], dp.resources)) {
                resIdx = mLastResourceIndex;
            } else {
                bool found = false;
                for (uint32_t i = 0; i < (uint32_t) mResourceTables.size(); ++i) {
                    if (sameResources(mResourceTables[i], dp.resources)) {
                        resIdx = i;
                        found  = true;
                        break;
                    }
                }
                if (!found) {
                    resIdx = (uint32_t) mResourceTables.size();
                    mResourceTables.append(dp.resources);
                }
                mLastResourceIndex = resIdx;
            }
        }

        mDraws.resize(mDraws.size() + 1);
        StoredDraw & s       = mDraws.back();
        s.vs                 = dp.vs;
        s.hs                 = dp.hs;
        s.ds                 = dp.ds;
        s.gs                 = dp.gs;
        s.ps                 = dp.ps;
        s.stateIndex         = stateIdx;
        s.geometryIndex      = geomIdx;
        s.resourceTableIndex = resIdx;
        s.immediates         = dp.immediates;
    }

    AutoRef<GpuPayload> seal() override {
        if (mSealed) {
            GN_ERROR(sLogger, "GpuRasterVulkan2::seal: double seal");
            return {};
        }
        mSealed        = true;
        auto   vkGpu   = mGpu.staticCastTo<GpuContextVulkan2>();
        auto * factory = (vkGpu && vkGpu->ready()) ? &vkGpu->psoFactory() : nullptr;
        return AutoRef<GpuPayload>(new GpuRasterPayloadVulkan(name + "/payload", factory, std::move(mRenderTarget), std::move(mDraws), std::move(mGeometries),
                                                              std::move(mResourceTables), std::move(mStates)));
    }

private:
    AutoRef<GpuContextVulkan2>  mGpu;
    RasterTarget                mRenderTarget;
    bool                        mValid  = false;
    bool                        mSealed = false;
    DynaArray<StoredDraw>       mDraws;
    DynaArray<RasterGeometry>   mGeometries;
    DynaArray<GpuResourceTable> mResourceTables;
    DynaArray<RasterState>      mStates;
    uint32_t                    mLastGeometryIndex = 0;
    uint32_t                    mLastResourceIndex = ~0u;
    uint32_t                    mLastStateIndex    = 0;
};

} // namespace

AutoRef<GpuRaster> createGpuRasterVulkan2(const StrA & name, const GpuRaster::CreateParameters & params) {
    AutoRef<GpuRasterVulkan2> raster(new GpuRasterVulkan2(name, params));
    if (!raster->valid()) return {};
    return raster;
}

} // namespace GN::gpu2
