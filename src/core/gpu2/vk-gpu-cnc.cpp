// Must be included first so rapid-vulkan defines/includes happen before any other vk-*.h
#include "vk-gpu-cnc.h"
#include "vk-gpu-context.h"
#include "vk-gpu-payload.h"
#include "vk-gpu-shader.h"
#include "vk-buffer.h"
#include "vk-texture.h"
#include "vk-format-utils.h"
#include "gpu-context.h"

#include <unordered_map>
#include <variant>
#include <vector>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

namespace {

// ── Stored operation records (built during compute/copy calls, consumed by seal/record) ──

struct StoredCompute {
    AutoRef<GpuShader> cs;
    GpuResourceTable   resources;
    DynaArray<uint8_t> immediates;
    uint32_t           x = 1, y = 1, z = 1;
};

struct StoredBufferToBuffer {
    AutoRef<Buffer> src;
    AutoRef<Buffer> dst;
    uint64_t        srcOffset = 0;
    uint64_t        dstOffset = 0;
    uint64_t        size      = 0;
};

struct StoredBufferToImage {
    AutoRef<Buffer>           src;
    AutoRef<Texture>          dst;
    DynaArray<GpuCnC::Region> regions;
};

using StoredOp = std::variant<StoredCompute, StoredBufferToBuffer, StoredBufferToImage>;

// ── Per-resource state trackers ──────────────────────────────────────────────────────

struct TrackedBuf {
    BufferVulkan *    buf;
    BufferStateVulkan state; // current Vulkan access/stage state, updated after each barrier
};

struct TrackedTex {
    TextureVulkanBase *          tex;
    rv::Image::State::PlaneState state; // whole-image state (same layout for all subresources)
};

// ── GpuCncPayloadVulkan ──────────────────────────────────────────────────────────────

class GpuCncPayloadVulkan final : public GpuPayloadVulkan {
public:
    GpuCncPayloadVulkan(const StrA & name, std::vector<StoredOp> ops): GpuPayloadVulkan(name), mOps(std::move(ops)) {}

    void recordForVulkanSubmit(const RecordContext & ctx) override;
    void onSubmitComplete() override;

private:
    std::vector<StoredOp> mOps;

    // State maps populated lazily (on first access) during recordForVulkanSubmit.
    std::unordered_map<uint64_t, TrackedBuf> mBufs;
    std::unordered_map<uint64_t, TrackedTex> mTexs;

    // Snapshot the current GPU state of a buffer on first access.
    TrackedBuf & getBuf(BufferVulkan * buf) {
        auto [it, inserted] = mBufs.try_emplace(buf->id, TrackedBuf {buf, buf->gpuState});
        return it->second;
    }

    // Snapshot the GPU state of a texture on first access.  Uses the first non-UNDEFINED plane
    // found at subresource (mip=0, face=0) as the representative whole-image state.
    TrackedTex & getTex(TextureVulkanBase * tex) {
        auto [it, inserted] = mTexs.try_emplace(tex->id, TrackedTex {});
        if (inserted) {
            it->second.tex   = tex;
            const auto & s   = tex->getState();
            it->second.state = rv::Image::State::PlaneState::UNDEFINED();
            if (!s.subresources.empty()) {
                for (const auto & [aspect, ps] : s.subresources[0].planes) {
                    if (ps.layout != vk::ImageLayout::eUndefined) {
                        it->second.state = ps;
                        break;
                    }
                }
            }
        }
        return it->second;
    }

    void transitionBuf(vk::CommandBuffer cb, BufferVulkan * buf, vk::AccessFlags newAccess, vk::PipelineStageFlags newStage);
    void transitionTex(vk::CommandBuffer cb, TextureVulkanBase * tex, const rv::Image::State::PlaneState & newState);

    void recordCompute(const StoredCompute & op, const RecordContext & ctx);
    void recordBufToBuf(const StoredBufferToBuffer & op, vk::CommandBuffer cb);
    void recordBufToImg(const StoredBufferToImage & op, vk::CommandBuffer cb);
};

// ── Barrier helpers ──────────────────────────────────────────────────────────────────

void GpuCncPayloadVulkan::transitionBuf(vk::CommandBuffer cb, BufferVulkan * buf, vk::AccessFlags newAccess, vk::PipelineStageFlags newStage) {
    TrackedBuf & tb = getBuf(buf);
    if (tb.state.access == newAccess && tb.state.stages == newStage) return;

    vk::Buffer vkBuf = buf->nativeBuffer();
    if (!vkBuf) GN_UNLIKELY {
            GN_WARN(sLogger)("GpuCncPayloadVulkan: buffer '{}' has no Vulkan handle; skipping barrier", buf->name);
            return;
        }

    auto srcStages = tb.state.stages;
    if (!srcStages) srcStages = vk::PipelineStageFlagBits::eTopOfPipe;

    vk::BufferMemoryBarrier b;
    b.setSrcAccessMask(tb.state.access)
        .setDstAccessMask(newAccess)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setBuffer(vkBuf)
        .setOffset(0)
        .setSize(VK_WHOLE_SIZE);
    cb.pipelineBarrier(srcStages, newStage, {}, nullptr, b, nullptr);

    tb.state.access = newAccess;
    tb.state.stages = newStage;
}

void GpuCncPayloadVulkan::transitionTex(vk::CommandBuffer cb, TextureVulkanBase * tex, const rv::Image::State::PlaneState & newState) {
    TrackedTex & tt = getTex(tex);
    if (tt.state == newState) return;

    vk::Image vkImg = tex->nativeImage();
    if (!vkImg) GN_UNLIKELY {
            GN_WARN(sLogger)("GpuCncPayloadVulkan: texture '{}' has no Vulkan handle; skipping barrier", tex->name);
            return;
        }

    // Use the texture's valid aspect flags; fall back to color as a safe default.
    auto aspects = tex->getState().validAspects;
    if (!aspects) aspects = vk::ImageAspectFlagBits::eColor;

    auto srcStages = tt.state.stages;
    if (!srcStages) srcStages = vk::PipelineStageFlagBits::eTopOfPipe;

    vk::ImageMemoryBarrier b;
    b.setOldLayout(tt.state.layout)
        .setNewLayout(newState.layout)
        .setImage(vkImg)
        .setSubresourceRange({aspects, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS})
        .setSrcAccessMask(tt.state.access)
        .setDstAccessMask(newState.access)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
    cb.pipelineBarrier(srcStages, newState.stages, {}, nullptr, nullptr, b);

    tt.state = newState;
}

// ── Compute dispatch ─────────────────────────────────────────────────────────────────

static rv::Sampler * ensureDefaultSampler(const rv::Device * dev, rv::Ref<rv::Sampler> & slot) {
    if (slot.valid()) return slot.get();
    rv::Sampler::ConstructParameters scp;
    scp.gi = dev->gi();
    scp.setLinear();
    slot = rv::Ref<rv::Sampler>::make(scp);
    return slot.get();
}

void GpuCncPayloadVulkan::recordCompute(const StoredCompute & op, const RecordContext & ctx) {
    auto * csVk = RuntimeType::cast<GpuShaderVulkan>(op.cs.get());
    if (!csVk || !csVk->rvShader()) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuCncPayloadVulkan: compute requires a valid compute shader");
            return;
        }

    vk::CommandBuffer vkcb = ctx.cmd.handle();

    // Pre-dispatch barriers: transition all shader resources to their required Vulkan states.
    for (size_t setIdx = 0; setIdx < op.resources.size(); ++setIdx) {
        const auto & set = op.resources[setIdx];
        for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
            const auto & slot = set[bindIdx];
            for (const auto & view : slot) {
                if (view.empty()) continue;
                if (view.isTexture()) {
                    auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                    if (!tex) continue;
                    rv::Image::State::PlaneState required;
                    if (view.imageView.type == GpuResourceView::ImageView::STORAGE) {
                        required.layout = vk::ImageLayout::eGeneral;
                        required.access = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite;
                        required.stages = vk::PipelineStageFlagBits::eComputeShader;
                        required.usage  = "compute storage image";
                    } else {
                        required.layout = vk::ImageLayout::eShaderReadOnlyOptimal;
                        required.access = vk::AccessFlagBits::eShaderRead;
                        required.stages = vk::PipelineStageFlagBits::eComputeShader;
                        required.usage  = "compute sampled image";
                    }
                    transitionTex(vkcb, tex, required);
                } else if (view.isBuffer()) {
                    auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                    if (!buf) continue;
                    if (view.bufferView.type == GpuResourceView::BufferView::STORAGE) {
                        // Storage buffer: may be both read and written by the shader.
                        transitionBuf(vkcb, buf, vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite, vk::PipelineStageFlagBits::eComputeShader);
                    } else {
                        transitionBuf(vkcb, buf, vk::AccessFlagBits::eUniformRead, vk::PipelineStageFlagBits::eComputeShader);
                    }
                }
            }
        }
    }

    // Build compute pipeline. No PSO cache yet; create fresh per-dispatch.
    // TODO: add a compute PSO factory (keyed on shader ID) if per-frame dispatch overhead shows up in profiles.
    rv::ComputePipeline::ConstructParameters ccp;
    ccp.name = std::string(name.c_str()) + "/compute_pso";
    ccp.cs   = csVk->rvShader();
    rv::Ref<rv::ComputePipeline> pipeline(new rv::ComputePipeline(ccp));
    if (!pipeline->handle()) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuCncPayloadVulkan: failed to create compute pipeline");
            return;
        }

    // Build drawable for descriptor binding and push constants.
    rv::Drawable::ConstructParameters dcp;
    dcp.setPipeline(pipeline);
    rv::Drawable drawable(dcp);

    if (!op.immediates.empty()) {
        if (op.immediates.size() > 128) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuCncPayloadVulkan: immediates size {} exceeds 128 bytes", op.immediates.size());
            }
        else { drawable.c(0, op.immediates.size(), op.immediates.data(), vk::ShaderStageFlagBits::eCompute); }
    }

    rv::Ref<rv::Sampler> defaultSampler;
    for (size_t setIdx = 0; setIdx < op.resources.size(); ++setIdx) {
        const auto & set = op.resources[setIdx];
        for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
            const auto & slot = set[bindIdx];
            if (slot.empty()) continue;
            rv::DescriptorIdentifier descId((uint32_t) setIdx, (uint32_t) bindIdx);
            if (slot[0].isTexture()) {
                std::vector<rv::ImageSampler> imgs;
                imgs.reserve(slot.size());
                for (const auto & view : slot) {
                    if (view.empty() || !view.isTexture()) continue;
                    auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                    if (!tex) continue;
                    vk::ImageLayout layout =
                        (view.imageView.type == GpuResourceView::ImageView::STORAGE) ? vk::ImageLayout::eGeneral : vk::ImageLayout::eShaderReadOnlyOptimal;
                    rv::ImageSampler is;
                    is.view    = tex->nativeView(view.imageView);
                    is.layout  = layout;
                    is.sampler = rv::Ref<const rv::Sampler>(ensureDefaultSampler(ctx.dev, defaultSampler));
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

    drawable.dispatch(rv::ComputePipeline::DispatchParameters {.width = op.x, .height = op.y, .depth = op.z});

    rv::Ref<const rv::DrawPack> pack = drawable.compile();
    if (!pack || pack->empty()) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuCncPayloadVulkan: Drawable::compile produced empty DrawPack for compute dispatch");
            return;
        }
    ctx.cmd.render(pack);
}

// ── Buffer-to-buffer copy ────────────────────────────────────────────────────────────

void GpuCncPayloadVulkan::recordBufToBuf(const StoredBufferToBuffer & op, vk::CommandBuffer cb) {
    if (op.size == 0) return;

    auto * srcVk = RuntimeType::cast<BufferVulkan>(op.src.get());
    auto * dstVk = RuntimeType::cast<BufferVulkan>(op.dst.get());
    if (!srcVk || !dstVk) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToBuffer: null Vulkan buffer");
            return;
        }
    if (srcVk == dstVk) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToBuffer: src and dst are the same buffer");
            return;
        }

    vk::Buffer srcBuf = srcVk->nativeBuffer();
    vk::Buffer dstBuf = dstVk->nativeBuffer();
    if (!srcBuf || !dstBuf) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToBuffer: buffer missing Vulkan handle");
            return;
        }

    transitionBuf(cb, srcVk, vk::AccessFlagBits::eTransferRead, vk::PipelineStageFlagBits::eTransfer);
    transitionBuf(cb, dstVk, vk::AccessFlagBits::eTransferWrite, vk::PipelineStageFlagBits::eTransfer);

    cb.copyBuffer(srcBuf, dstBuf, vk::BufferCopy(op.srcOffset, op.dstOffset, op.size));
}

// ── Buffer-to-image copy ─────────────────────────────────────────────────────────────

void GpuCncPayloadVulkan::recordBufToImg(const StoredBufferToImage & op, vk::CommandBuffer cb) {
    if (op.regions.empty()) return;

    auto * srcVk = RuntimeType::cast<BufferVulkan>(op.src.get());
    auto * dstVk = RuntimeType::cast<TextureVulkanBase>(op.dst.get());
    if (!srcVk || !dstVk) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToImage: null Vulkan resource");
            return;
        }

    vk::Buffer srcBuf = srcVk->nativeBuffer();
    vk::Image  dstImg = dstVk->nativeImage();
    if (!srcBuf || !dstImg) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToImage: resource has no Vulkan handle");
            return;
        }

    transitionBuf(cb, srcVk, vk::AccessFlagBits::eTransferRead, vk::PipelineStageFlagBits::eTransfer);

    rv::Image::State::PlaneState transferDst;
    transferDst.layout = vk::ImageLayout::eTransferDstOptimal;
    transferDst.access = vk::AccessFlagBits::eTransferWrite;
    transferDst.stages = vk::PipelineStageFlagBits::eTransfer;
    transferDst.usage  = "buffer-to-image copy destination";
    transitionTex(cb, dstVk, transferDst);

    // Derive the aspect flags from the destination texture's format.
    const auto & desc    = dstVk->descriptor();
    auto         aspects = aspectFromViewFormat(desc.format, desc.format);
    if (!aspects) aspects = vk::ImageAspectFlagBits::eColor;

    std::vector<vk::BufferImageCopy> copies;
    copies.reserve(op.regions.size());
    for (const auto & r : op.regions) {
        vk::BufferImageCopy c;
        c.setBufferOffset(r.bufferOffset)
            .setBufferRowLength(r.bufferRowLength)
            .setBufferImageHeight(r.bufferHeight)
            .setImageSubresource({aspects, r.imageSubresource.mip, r.imageSubresource.face, 1})
            .setImageOffset({(int32_t) r.imageOffset.x, (int32_t) r.imageOffset.y, (int32_t) r.imageOffset.z})
            .setImageExtent({r.imageExtent.x, r.imageExtent.y, r.imageExtent.z});
        copies.push_back(c);
    }

    cb.copyBufferToImage(srcBuf, dstImg, vk::ImageLayout::eTransferDstOptimal, (uint32_t) copies.size(), copies.data());
}

// ── recordForVulkanSubmit ────────────────────────────────────────────────────────────

void GpuCncPayloadVulkan::recordForVulkanSubmit(const RecordContext & ctx) {
    if (!ctx.dev || ctx.cmd.empty()) return;
    vk::CommandBuffer vkcb = ctx.cmd.handle();

    for (const auto & op : mOps) {
        std::visit(
            [&](const auto & o) {
                using T = std::decay_t<decltype(o)>;
                if constexpr (std::is_same_v<T, StoredCompute>)
                    recordCompute(o, ctx);
                else if constexpr (std::is_same_v<T, StoredBufferToBuffer>)
                    recordBufToBuf(o, vkcb);
                else if constexpr (std::is_same_v<T, StoredBufferToImage>)
                    recordBufToImg(o, vkcb);
            },
            op);
    }
}

// ── onSubmitComplete ─────────────────────────────────────────────────────────────────

void GpuCncPayloadVulkan::onSubmitComplete() {
    // Flush final resource states back so subsequent payloads see the correct "incoming" state.
    for (auto & [id, tb] : mBufs) tb.buf->gpuState = tb.state;
    for (auto & [id, tt] : mTexs) {
        auto aspects = tt.tex->getState().validAspects;
        if (!aspects) aspects = vk::ImageAspectFlagBits::eColor;
        vk::ImageSubresourceRange range(aspects, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS);
        tt.tex->setState(tt.state, range);
    }
}

// ── GpuCncVulkan2 ────────────────────────────────────────────────────────────────────

class GpuCncVulkan2 final : public GpuCnC {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuCnC);

    GpuCncVulkan2(const StrA & entityName, const CreateParameters & cp): GpuCnC(TYPE_INFO(), entityName), mCreateParams(cp) {}

    void compute(const ComputeParameters & cp) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuCncVulkan2::compute: already sealed");
                return;
            }
        StoredCompute op;
        op.cs        = cp.cs;
        op.resources = cp.resources;
        op.x         = cp.x;
        op.y         = cp.y;
        op.z         = cp.z;
        if (cp.immediates.data() && cp.immediates.size() > 0) op.immediates.append(cp.immediates.data(), (size_t) cp.immediates.size());
        mOps.emplace_back(std::move(op));
    }

    void copyBufferToBuffer(const BufferToBuffer & p) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuCncVulkan2::copyBufferToBuffer: already sealed");
                return;
            }
        mOps.emplace_back(StoredBufferToBuffer {p.src, p.dst, p.srcOffset, p.dstOffset, p.size});
    }

    void copyBufferToImage(const BufferToImage & p) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuCncVulkan2::copyBufferToImage: already sealed");
                return;
            }
        StoredBufferToImage op;
        op.src = p.src;
        op.dst = p.dst;
        for (const auto & r : p.regions) op.regions.append(r);
        mOps.emplace_back(std::move(op));
    }

    AutoRef<GpuPayload> seal() override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuCncVulkan2::seal: double seal");
                return {};
            }
        mSealed = true;
        return AutoRef<GpuPayload>(new GpuCncPayloadVulkan(name + "/payload", std::move(mOps)));
    }

private:
    CreateParameters      mCreateParams;
    bool                  mSealed = false;
    std::vector<StoredOp> mOps;
};

} // anonymous namespace

// ── Factory ──────────────────────────────────────────────────────────────────────────

AutoRef<GpuCnC> createGpuCncVulkan2(const GpuCnC::CreateParameters & params) {
    if (!params.gpu) return {};
    auto vkGpu = params.gpu.staticCastTo<GpuContextVulkan2>();
    if (!vkGpu || !vkGpu->ready()) return {};
    StrA n = params.gpu->name.empty() ? StrA("cnc") : params.gpu->name + "/cnc";
    return AutoRef<GpuCnC>(new GpuCncVulkan2(n, params));
}

} // namespace GN::gpu2
