#include "vk-gpu-raster.h"
#include <garnet/GNrdg2.h>
#include "vk-format-utils.h"
#include "gpu-context.h"
#include "vk-gpu-context.h"
#include "vk-gpu-image-state.h"
#include "vk-gpu-payload.h"
#include "vk-gpu-shader.h"
#include "vk-texture.h"

#include <string>

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu2.vk");

namespace GN::gpu2 {

namespace {

struct StoredDraw {
    AutoRef<GpuShader> vs, hs, ds, gs, ps;
    RenderStates       renderStates;
    RenderGeometry     geometry;
    GpuResourceTable   resources;
    DynaArray<uint8_t> immediates;
};

static bool resolveFirstColorAttachment(const RenderTarget & rt, vk::Image * outImage, vk::ImageView * outView, vk::Extent2D * outExt,
                                        vk::Format * outVkFormat) {
    if (rt.colors.empty()) return false;
    const GpuResourceView & v = rt.colors[0].target;
    if (v.empty() || !v.isTexture()) return false;
    AutoRef<Texture> t = v.texture();
    if (!t) return false;
    auto * base = RuntimeType::cast<TextureVulkanBase>(t.get());
    if (!base) return false;
    if (base->resolveVulkanColorAttachment(v, outImage, outView, outExt, outVkFormat)) return true;
    GN_WARN(sLogger)("GpuRaster Vulkan: color target could not be resolved to a Vulkan texture");
    return false;
}

static GpuResourceView::SubresourceRange subresourceRangeForPostSubmitFlush(const GpuResourceView & v, const Texture::Descriptor & d) {
    GpuResourceView::SubresourceRange r  = v.imageView.range;
    uint32_t                          nm = r.e.numMipLevels;
    uint32_t                          na = r.e.numArrayLayers;
    if (nm == (uint32_t) -1) nm = d.levels > r.i.mip ? d.levels - r.i.mip : 0;
    if (na == (uint32_t) -1) na = d.faces > r.i.face ? d.faces - r.i.face : 0;
    r.e.numMipLevels   = nm;
    r.e.numArrayLayers = na;
    return r;
}

static rv::Sampler * ensureLinearSampler(const rv::Device * dev, rv::Ref<rv::Sampler> & slot) {
    if (slot.valid()) return slot.get();
    rv::Sampler::ConstructParameters scp;
    scp.gi = dev->gi();
    scp.setLinear();
    slot = rv::Ref<rv::Sampler>::make(scp);
    return slot.get();
}

struct PostSubmitTextureFlush {
    AutoRef<Texture>                  texture;
    GpuResourceView::SubresourceRange range {};
    TextureGpuImageState::ImageState  latest {};
};

class GpuRasterPayloadVulkan final : public GpuPayloadVulkan {
public:
    GpuRasterPayloadVulkan(const StrA & name, RenderTarget rt, ArrayContainer<StoredDraw> draws)
        : GpuPayloadVulkan(name), mRenderTarget(std::move(rt)), mDraws(std::move(draws)) {
        if (!mRenderTarget.colors.empty()) {
            const GpuResourceView & cv = mRenderTarget.colors[0].target;
            if (cv.isTexture() && cv.texture()) {
                AutoRef<Texture> t = cv.texture();
                if (auto * base = RuntimeType::cast<TextureVulkanBase>(t.get())) {
                    PostSubmitTextureFlush e;
                    e.texture = std::move(t);
                    e.range   = subresourceRangeForPostSubmitFlush(cv, base->descriptor());
                    e.latest  = TextureGpuImageState::ImageState {vk::ImageLayout::ePresentSrcKHR, vk::AccessFlagBits::eMemoryRead,
                                                                  vk::PipelineStageFlagBits::eBottomOfPipe};
                    mPostSubmitTextureFlushes.append(std::move(e));
                }
            }
        }
    }

    void recordForVulkanSubmit(const RecordContext & ctx) override;
    void onSubmitComplete() override;

private:
    RenderTarget                           mRenderTarget;
    ArrayContainer<StoredDraw>             mDraws;
    ArrayContainer<PostSubmitTextureFlush> mPostSubmitTextureFlushes;
};

void GpuRasterPayloadVulkan::onSubmitComplete() {
    for (size_t i = 0; i < mPostSubmitTextureFlushes.size(); ++i) {
        PostSubmitTextureFlush & e = mPostSubmitTextureFlushes[i];
        if (auto * tb = RuntimeType::cast<TextureVulkanBase>(e.texture.get())) tb->gpuStates.set(e.range, e.latest);
    }
}

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

    vk::ImageMemoryBarrier preBarrier;
    preBarrier.setOldLayout(vk::ImageLayout::eUndefined)
        .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setImage(colorImage)
        .setSubresourceRange({vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1})
        .setSrcAccessMask({})
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);
    vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput, {}, nullptr, nullptr, preBarrier);

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

    vk::Viewport vp(0.0f, 0.0f, (float) ext.width, (float) ext.height, 0.0f, 1.0f);
    vkcb.setViewport(0, 1, &vp);
    vk::Rect2D scissor(vk::Offset2D(0, 0), ext);
    vkcb.setScissor(0, 1, &scissor);

    rv::Ref<rv::Sampler> defaultSampler;

    for (size_t di = 0; di < mDraws.size(); ++di) {
        const StoredDraw & d    = mDraws[di];
        auto *             vsVk = RuntimeType::cast<GpuShaderVulkan>(d.vs.get());
        auto *             psVk = RuntimeType::cast<GpuShaderVulkan>(d.ps.get());
        if (!vsVk || !vsVk->rvShader()) {
            GN_ERROR(sLogger)("RasterPassPayload: draw {} missing Vulkan vertex shader", di);
            continue;
        }

        rv::GraphicsPipeline::ConstructParameters gcp;
        gcp.setName(std::string(StrA::format("rdg2_gfx_{}", di).c_str()));
        gcp.setDynamicRendering(colorFmt, vk::Format::eUndefined);
        gcp.setVS(vsVk->rvShader());
        if (psVk && psVk->rvShader()) gcp.setFS(psVk->rvShader());
        gcp.dynamicViewport(1).dynamicScissor(1);

        const RenderGeometry & geom = d.geometry;
        for (size_t bi = 0; bi < geom.vertices.size(); ++bi) { gcp.addVertexBuffer(geom.vertices[bi].stride); }
        for (size_t ai = 0; ai < geom.format.attributes.size(); ++ai) {
            const RenderGeometry::VertexAttribute & a = geom.format.attributes[ai];
            gcp.addVertexAttribute(a.location, (uint32_t) a.offset, vertexAttributeFormatToVk(a.format));
        }

        if (d.renderStates.fillMode && *d.renderStates.fillMode == RenderStates::FILL_WIREFRAME) { gcp.rast.setPolygonMode(vk::PolygonMode::eLine); }
        if (d.renderStates.cullMode) {
            switch (*d.renderStates.cullMode) {
            case RenderStates::CULL_NONE:
                gcp.rast.setCullMode(vk::CullModeFlagBits::eNone);
                break;
            case RenderStates::CULL_FRONT:
                gcp.rast.setCullMode(vk::CullModeFlagBits::eFront);
                break;
            case RenderStates::CULL_BACK:
                gcp.rast.setCullMode(vk::CullModeFlagBits::eBack);
                break;
            }
        }
        if (d.renderStates.frontFace && *d.renderStates.frontFace == RenderStates::FRONT_CW) { gcp.rast.setFrontFace(vk::FrontFace::eClockwise); }

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

    vk::ImageMemoryBarrier postBarrier;
    postBarrier.setOldLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
        .setImage(colorImage)
        .setSubresourceRange({vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1})
        .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
        .setDstAccessMask(vk::AccessFlagBits::eMemoryRead);
    vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe, {}, nullptr, nullptr, postBarrier);
}

class GpuRasterVulkan2 final : public GpuRaster {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuRaster);

    GpuRasterVulkan2(const StrA & entityName, RenderTarget rt): GpuRaster(TYPE_INFO(), entityName), mRenderTarget(std::move(rt)), mSealed(false) {}

    void draw(const DrawParameters & dp) override {
        if (mSealed) {
            GN_ERROR(sLogger)("GpuRasterVulkan2::draw: already sealed");
            return;
        }
        StoredDraw s;
        s.vs           = dp.vs;
        s.hs           = dp.hs;
        s.ds           = dp.ds;
        s.gs           = dp.gs;
        s.ps           = dp.ps;
        s.renderStates = dp.renderStates;
        s.geometry     = dp.geometry;
        s.resources    = dp.resources;
        s.immediates.clear();
        if (dp.immediates.data() && dp.immediates.size() > 0) { s.immediates.append(dp.immediates.data(), (size_t) dp.immediates.size()); }
        mDraws.append(std::move(s));
    }

    AutoRef<GpuPayload> seal() override {
        if (mSealed) {
            GN_ERROR(sLogger)("GpuRasterVulkan2::seal: double seal");
            return {};
        }
        mSealed = true;
        return AutoRef<GpuPayload>(new GpuRasterPayloadVulkan(name + "/payload", std::move(mRenderTarget), std::move(mDraws)));
    }

private:
    RenderTarget               mRenderTarget;
    bool                       mSealed;
    ArrayContainer<StoredDraw> mDraws;
};

} // namespace

AutoRef<GpuRaster> createGpuRasterVulkan2(const GpuRaster::CreateParameters & params) {
    if (!params.gpu) return {};
    AutoRef<GpuContextVulkan2> vkGpu = params.gpu.staticCastTo<GpuContextVulkan2>();
    if (!vkGpu || !vkGpu->ready()) return {};
    StrA n = params.gpu->name.empty() ? StrA("raster_pass") : params.gpu->name + "/raster";
    return AutoRef<GpuRaster>(new GpuRasterVulkan2(n, params.renderTarget));
}

} // namespace GN::gpu2
