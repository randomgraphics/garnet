#include "pch.h"
// vk-gpu-context.h must precede any other rapid-vulkan include in this TU (ODR guard).
#include "vk-gpu-cnc.h"
#include "vk-gpu-context.h"
#include "vk-gpu-payload.h"
#include "vk-gpu-shader.h"
#include "vk-gpu-resource-state-tracker.h"
#include "vk-buffer.h"
#include "vk-texture.h"
#include "vk-format-utils.h"
#include "gpu-context.h"

#include <cstring>
#include <variant>
#include <vector>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

namespace {

// ── Stored operation records (built during compute/copy calls, consumed by seal/record) ──

struct StoredCompute {
    AutoRef<GpuShader>  cs;
    GpuResourceTable    resources;
    AutoRef<const Blob> immediates;
    uint32_t            x = 1, y = 1, z = 1;
};

struct StoredBufferToBuffer {
    AutoRef<Buffer> src;
    AutoRef<Buffer> dst;
    uint64_t        srcOffset = 0;
    uint64_t        dstOffset = 0;
    uint64_t        size      = 0;
};

struct StoredBufferToImage {
    AutoRef<Buffer>                          src;
    AutoRef<Texture>                         dst;
    DynaArray<Buffer::StagedTexture::Region> regions;
};

// Async upload: a pre-filled, host-visible staging buffer that is copied into dst on the GPU.
// The staging buffer is owned here, so it is kept alive for exactly as long as the GPU needs it
// (released by the payload's onGpuComplete()/destructor — see below).
struct StoredUploadBuffer {
    AutoRef<Buffer> staging; ///< host-visible; pre-filled with the upload content at enqueue time
    AutoRef<Buffer> dst;
    uint64_t        dstOffset = 0;
    uint64_t        size      = 0;
};

// A movable promise wrapper that *always* signals its future exactly once. If it is destroyed
// before being explicitly resolved — the owning payload is dropped without submission, or the whole
// GpuContext is torn down with the work still pending — it fulfills the future with a
// default-constructed (empty) value instead of letting the promise break. This makes a canceled
// download observable as an empty result rather than a std::future_error, matching the public
// "signaled when ... failed or canceled" contract. Held by unique_ptr so the wrapper is cheaply
// movable (as required inside the StoredOp variant) and a moved-from instance becomes inert.
template<typename T>
class DownloadResult {
public:
    DownloadResult(): mPromise(std::make_unique<std::promise<T>>()) {}
    DownloadResult(DownloadResult &&) noexcept             = default;
    DownloadResult & operator=(DownloadResult &&) noexcept = default;
    ~DownloadResult() { resolve(T {}); } // empty result if not already resolved / moved-from

    std::future<T> future() { return mPromise->get_future(); }

    void resolve(T value) {
        if (!mPromise) return; // already resolved, or moved-from
        try {
            mPromise->set_value(std::move(value));
        } catch (const std::future_error &) {
            // Defensive: the underlying promise was somehow already satisfied. Ignore.
        }
        mPromise.reset();
    }

private:
    std::unique_ptr<std::promise<T>> mPromise;
};

// Async download: a GPU copy from src into a host-visible staging buffer, read back on the CPU once
// the GPU work completes. The result Blob is delivered through the DownloadResult future.
struct StoredDownloadBuffer {
    AutoRef<Buffer>                     src;
    AutoRef<Buffer>                     staging; ///< host-visible transfer destination; read back on completion
    uint64_t                            srcOffset = 0;
    uint64_t                            size      = 0;
    DownloadResult<AutoRef<const Blob>> result;
};

struct StoredDownloadImage {
    AutoRef<Texture>                         src;
    AutoRef<Buffer>                          staging; ///< host-visible transfer destination, tightly packed
    DynaArray<Buffer::StagedTexture::Region> regions; ///< bufferOffset of each region filled at enqueue time
    DownloadResult<GpuCnC::TextureContent>   result;
};

using StoredOp = std::variant<StoredCompute, StoredBufferToBuffer, StoredBufferToImage, StoredUploadBuffer, StoredDownloadBuffer, StoredDownloadImage>;

// ── GpuCncPayloadVulkan ──────────────────────────────────────────────────────────────

class GpuCncPayloadVulkan final : public GpuPayloadVulkan {
public:
    GpuCncPayloadVulkan(const StrA & name, std::vector<StoredOp> ops): GpuPayloadVulkan(name), mOps(std::move(ops)) {}

    void recordForVulkanSubmit(const RecordContext & ctx) override;

    /// Fired after the GPU fence signals: read back download staging buffers, resolve their futures,
    /// and free all transient staging buffers. If the payload is dropped without submission this is
    /// never called; the DownloadResult destructor then resolves each pending future with an empty
    /// value (the public API's documented "canceled" outcome).
    void onGpuComplete() override;

private:
    std::vector<StoredOp> mOps;

    void recordCompute(const StoredCompute & op, const RecordContext & ctx);
    void recordBufToBuf(const StoredBufferToBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker);
    void recordBufToImg(const StoredBufferToImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker);
    void recordUploadBuffer(const StoredUploadBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker);
    void recordDownloadBuffer(const StoredDownloadBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker);
    void recordDownloadImage(const StoredDownloadImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker);

    static void resolveDownloadBuffer(StoredDownloadBuffer & op);
    static void resolveDownloadImage(StoredDownloadImage & op);
};

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
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: compute requires a valid compute shader");
            return;
        }

    vk::CommandBuffer               vkcb    = ctx.cmd.handle();
    GpuResourceStateTrackerVulkan & tracker = *ctx.batchTracker;

    // Register all shader resources with compute-stage pipeline flags, then emit a single
    // pre-dispatch barrier. Using one batch barrier (rather than per-resource barriers) keeps
    // the pattern consistent with how raster passes work via the same tracker.
    for (size_t setIdx = 0; setIdx < op.resources.size(); ++setIdx) {
        const auto & set = op.resources[setIdx];
        for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
            const auto & slot = set[bindIdx];
            for (const auto & view : slot) {
                if (view.empty()) continue;
                if (view.isTexture()) {
                    auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                    if (!tex) continue;
                    if (view.imageView.type == GpuResourceView::ImageView::STORAGE)
                        tracker.addStorageTexture(tex, view, vk::PipelineStageFlagBits::eComputeShader);
                    else
                        tracker.addSampledTexture(tex, view, vk::PipelineStageFlagBits::eComputeShader);
                } else if (view.isBuffer()) {
                    auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                    if (!buf) continue;
                    if (view.bufferView.type == GpuResourceView::BufferView::STORAGE)
                        // Treat storage buffers as read-write; the shader may write without
                        // declaring it in the resource table, so be conservative.
                        tracker.addStorageBuffer(buf, /*write=*/true, vk::PipelineStageFlagBits::eComputeShader);
                    else
                        tracker.addUniformBuffer(buf, vk::PipelineStageFlagBits::eComputeShader);
                }
            }
        }
    }
    tracker.emitPrePassBarriers(vkcb);

    // Build compute pipeline. No PSO cache yet; create fresh per-dispatch.
    // TODO: add a compute PSO factory (keyed on shader ID) if per-frame dispatch overhead shows up in profiles.
    rv::ComputePipeline::ConstructParameters ccp;
    ccp.name = std::string(name.c_str()) + "/compute_pso";
    ccp.cs   = csVk->rvShader();
    rv::Ref<rv::ComputePipeline> pipeline(new rv::ComputePipeline(ccp));
    if (!pipeline->handle()) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: failed to create compute pipeline");
            return;
        }

    rv::Drawable::ConstructParameters dcp;
    dcp.setPipeline(pipeline);
    rv::Drawable drawable(dcp);

    if (op.immediates && !op.immediates->empty()) {
        if (op.immediates->size() > 128) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncPayloadVulkan: immediates size {} exceeds 128 bytes", op.immediates->size());
            }
        else { drawable.c(0, op.immediates->size(), op.immediates->data(), vk::ShaderStageFlagBits::eCompute); }
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
                    is.view   = tex->nativeView(view.imageView);
                    is.layout = layout;
                    // Storage images are bound as plain image views (no sampler).
                    // Adding a sampler would change the rapid-vulkan ImageArgs type to COMBINED,
                    // which is incompatible with eStorageImage and fails pipeline validation.
                    if (view.imageView.type != GpuResourceView::ImageView::STORAGE)
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
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: Drawable::compile produced empty DrawPack for compute dispatch");
            return;
        }
    ctx.cmd.render(pack);
}

// ── Buffer-to-buffer copy ────────────────────────────────────────────────────────────

// Shared core for every buffer→buffer transfer (plain copy, upload staging→dst, download src→staging).
// Registers the transfer states, emits the barrier, and records the copy. Caller guarantees src != dst.
static void emitBufferCopy(BufferVulkan * srcVk, BufferVulkan * dstVk, uint64_t srcOffset, uint64_t dstOffset, uint64_t size, vk::CommandBuffer cb,
                           GpuResourceStateTrackerVulkan & tracker) {
    if (size == 0) return;
    if (!srcVk || !dstVk) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: buffer copy: null Vulkan buffer");
            return;
        }
    vk::Buffer srcBuf = srcVk->nativeBuffer();
    vk::Buffer dstBuf = dstVk->nativeBuffer();
    if (!srcBuf || !dstBuf) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: buffer copy: buffer missing Vulkan handle");
            return;
        }

    tracker.addTransferSrcBuffer(srcVk);
    tracker.addTransferDstBuffer(dstVk);
    tracker.emitPrePassBarriers(cb);

    cb.copyBuffer(srcBuf, dstBuf, vk::BufferCopy(srcOffset, dstOffset, size));
}

void GpuCncPayloadVulkan::recordBufToBuf(const StoredBufferToBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    if (op.size == 0) return;

    auto * srcVk = RuntimeType::cast<BufferVulkan>(op.src.get());
    auto * dstVk = RuntimeType::cast<BufferVulkan>(op.dst.get());
    if (srcVk && srcVk == dstVk) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: copyBufferToBuffer: src and dst are the same buffer");
            return;
        }
    emitBufferCopy(srcVk, dstVk, op.srcOffset, op.dstOffset, op.size, cb, tracker);
}

void GpuCncPayloadVulkan::recordUploadBuffer(const StoredUploadBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    emitBufferCopy(RuntimeType::cast<BufferVulkan>(op.staging.get()), RuntimeType::cast<BufferVulkan>(op.dst.get()), 0, op.dstOffset, op.size, cb, tracker);
}

void GpuCncPayloadVulkan::recordDownloadBuffer(const StoredDownloadBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    emitBufferCopy(RuntimeType::cast<BufferVulkan>(op.src.get()), RuntimeType::cast<BufferVulkan>(op.staging.get()), op.srcOffset, 0, op.size, cb, tracker);
}

// ── Buffer-to-image copy ─────────────────────────────────────────────────────────────

void GpuCncPayloadVulkan::recordBufToImg(const StoredBufferToImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    if (op.regions.empty()) return;

    auto * srcVk = RuntimeType::cast<BufferVulkan>(op.src.get());
    auto * dstVk = RuntimeType::cast<TextureVulkanBase>(op.dst.get());
    if (!srcVk || !dstVk) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: copyBufferToImage: null Vulkan resource");
            return;
        }

    vk::Buffer srcBuf = srcVk->nativeBuffer();
    vk::Image  dstImg = dstVk->nativeImage();
    if (!srcBuf || !dstImg) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: copyBufferToImage: resource has no Vulkan handle");
            return;
        }

    tracker.addTransferSrcBuffer(srcVk);
    // Transition the whole image: vkCmdCopyBufferToImage can target any mip/face per region,
    // so the barrier must cover all subresources to be conservative.
    GpuResourceView::ImageView fullRange;
    tracker.addTransferDstImage(dstVk, fullRange);
    tracker.emitPrePassBarriers(cb);

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
            .setImageSubresource({aspects, r.mip, r.face, 1})
            .setImageOffset({(int32_t) r.imageOffset.x, (int32_t) r.imageOffset.y, (int32_t) r.imageOffset.z})
            .setImageExtent({r.imageExtent.x, r.imageExtent.y, r.imageExtent.z});
        copies.push_back(c);
    }

    cb.copyBufferToImage(srcBuf, dstImg, vk::ImageLayout::eTransferDstOptimal, (uint32_t) copies.size(), copies.data());
}

// ── Image-to-buffer copy (download) ──────────────────────────────────────────────────

void GpuCncPayloadVulkan::recordDownloadImage(const StoredDownloadImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    if (op.regions.empty()) return;

    auto * srcVk = RuntimeType::cast<TextureVulkanBase>(op.src.get());
    auto * dstVk = RuntimeType::cast<BufferVulkan>(op.staging.get());
    if (!srcVk || !dstVk) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: downloadImage: null Vulkan resource");
            return;
        }

    vk::Image  srcImg = srcVk->nativeImage();
    vk::Buffer dstBuf = dstVk->nativeBuffer();
    if (!srcImg || !dstBuf) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCncPayloadVulkan: downloadImage: resource has no Vulkan handle");
            return;
        }

    // Conservatively transition the whole image to transfer-src (any mip/face may be read).
    tracker.addTransferDstBuffer(dstVk);
    GpuResourceView::ImageView fullRange;
    tracker.addTransferSrcImage(srcVk, fullRange);
    tracker.emitPrePassBarriers(cb);

    const auto & desc    = srcVk->descriptor();
    auto         aspects = aspectFromViewFormat(desc.format, desc.format);
    if (!aspects) aspects = vk::ImageAspectFlagBits::eColor;

    std::vector<vk::BufferImageCopy> copies;
    copies.reserve(op.regions.size());
    for (const auto & r : op.regions) {
        vk::BufferImageCopy c;
        c.setBufferOffset(r.bufferOffset)
            .setBufferRowLength(r.bufferRowLength) // 0 = tightly packed (rows == imageExtent.x)
            .setBufferImageHeight(r.bufferHeight)
            .setImageSubresource({aspects, r.mip, r.face, 1})
            .setImageOffset({(int32_t) r.imageOffset.x, (int32_t) r.imageOffset.y, (int32_t) r.imageOffset.z})
            .setImageExtent({r.imageExtent.x, r.imageExtent.y, r.imageExtent.z});
        copies.push_back(c);
    }

    cb.copyImageToBuffer(srcImg, vk::ImageLayout::eTransferSrcOptimal, dstBuf, (uint32_t) copies.size(), copies.data());
}

// ── Download read-back (CPU side, after GPU completion) ───────────────────────────────

void GpuCncPayloadVulkan::resolveDownloadBuffer(StoredDownloadBuffer & op) {
    AutoRef<const Blob> blob;
    if (op.staging && op.size > 0) {
        auto m = op.staging->map();
        if (m.data()) {
            blob = AutoRef<const Blob>(new SimpleBlob<uint8_t>((size_t) op.size, (const uint8_t *) m.data()));
        } else
            GN_UNLIKELY { GN_ERROR(sLogger, "GpuCncPayloadVulkan: downloadBuffer: failed to map staging buffer for read-back"); }
        // m unmaps on scope exit (RAII)
    }
    op.staging.clear(); // transient data done; release immediately on GPU completion.
    op.result.resolve(std::move(blob));
}

void GpuCncPayloadVulkan::resolveDownloadImage(StoredDownloadImage & op) {
    GpuCnC::TextureContent content;
    if (op.staging && !op.regions.empty()) {
        auto m = op.staging->map();
        if (m.data()) {
            content.blob = AutoRef<const Blob>(new SimpleBlob<uint8_t>(m.size(), (const uint8_t *) m.data()));
            for (const auto & r : op.regions) content.regions.append(r);
        } else
            GN_UNLIKELY { GN_ERROR(sLogger, "GpuCncPayloadVulkan: downloadImage: failed to map staging buffer for read-back"); }
    }
    op.staging.clear();
    op.result.resolve(std::move(content));
}

void GpuCncPayloadVulkan::onGpuComplete() {
    for (auto & op : mOps) {
        std::visit(
            [&](auto & o) {
                using T = std::decay_t<decltype(o)>;
                if constexpr (std::is_same_v<T, StoredUploadBuffer>)
                    o.staging.clear(); // transient upload staging done; release immediately.
                else if constexpr (std::is_same_v<T, StoredDownloadBuffer>)
                    resolveDownloadBuffer(o);
                else if constexpr (std::is_same_v<T, StoredDownloadImage>)
                    resolveDownloadImage(o);
            },
            op);
    }
}

// ── recordForVulkanSubmit ────────────────────────────────────────────────────────────

void GpuCncPayloadVulkan::recordForVulkanSubmit(const RecordContext & ctx) {
    if (!ctx.dev || ctx.cmd.empty() || !ctx.batchTracker) return;
    GpuResourceStateTrackerVulkan & tracker = *ctx.batchTracker;
    vk::CommandBuffer               vkcb    = ctx.cmd.handle();

    for (const auto & op : mOps) {
        std::visit(
            [&](const auto & o) {
                using T = std::decay_t<decltype(o)>;
                if constexpr (std::is_same_v<T, StoredCompute>)
                    recordCompute(o, ctx);
                else if constexpr (std::is_same_v<T, StoredBufferToBuffer>)
                    recordBufToBuf(o, vkcb, tracker);
                else if constexpr (std::is_same_v<T, StoredBufferToImage>)
                    recordBufToImg(o, vkcb, tracker);
                else if constexpr (std::is_same_v<T, StoredUploadBuffer>)
                    recordUploadBuffer(o, vkcb, tracker);
                else if constexpr (std::is_same_v<T, StoredDownloadBuffer>)
                    recordDownloadBuffer(o, vkcb, tracker);
                else if constexpr (std::is_same_v<T, StoredDownloadImage>)
                    recordDownloadImage(o, vkcb, tracker);
            },
            op);
    }
}

// ── GpuCncVulkan2 ────────────────────────────────────────────────────────────────────

class GpuCncVulkan2 final : public GpuCnC {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuCnC);

    GpuCncVulkan2(const StrA & entityName, const CreateParameters & cp): GpuCnC(TYPE_INFO(), entityName), mGpu(cp.gpu) {}

    void compute(const ComputeParameters & cp) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::compute: already sealed");
                return;
            }
        StoredCompute op;
        op.cs         = cp.cs;
        op.resources  = cp.resources;
        op.x          = cp.x;
        op.y          = cp.y;
        op.z          = cp.z;
        op.immediates = cp.immediates;
        mOps.emplace_back(std::move(op));
    }

    void copyBufferToBuffer(const BufferToBuffer & p) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::copyBufferToBuffer: already sealed");
                return;
            }
        mOps.emplace_back(StoredBufferToBuffer {p.src, p.dst, p.srcOffset, p.dstOffset, p.size});
    }

    void copyBufferToImage(const BufferToImage & p) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::copyBufferToImage: already sealed");
                return;
            }
        StoredBufferToImage op;
        op.src = p.src;
        op.dst = p.dst;
        for (const auto & r : p.regions) op.regions.append(r);
        mOps.emplace_back(std::move(op));
    }

    void uploadBuffer(AutoRef<Buffer> dst, uint64_t offset, AutoRef<const Blob> content) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::uploadBuffer: already sealed");
                return;
            }
        if (!dst || !content || content->empty()) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::uploadBuffer: null destination or empty content");
                return;
            }
        const uint64_t size    = content->size();
        auto           staging = createStaging("upload_stg", size);
        if (!staging) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::uploadBuffer: staging buffer allocation failed");
                return;
            }
        {
            auto m = staging->map();
            if (!m.data()) GN_UNLIKELY {
                    GN_ERROR(sLogger, "GpuCncVulkan2::uploadBuffer: failed to map staging buffer");
                    return;
                }
            memcpy(m.data(), content->data(), (size_t) size);
            // m unmaps on scope exit (RAII)
        }
        StoredUploadBuffer op;
        op.staging   = std::move(staging);
        op.dst       = std::move(dst);
        op.dstOffset = offset;
        op.size      = size;
        mOps.emplace_back(std::move(op));
    }

    std::future<AutoRef<const Blob>> downloadBuffer(AutoRef<Buffer> src, uint64_t offset, uint64_t size) override {
        // On any early return below, `result` destructs and resolves the future with an empty blob.
        DownloadResult<AutoRef<const Blob>> result;
        auto                                future = result.future();

        auto * srcVk = RuntimeType::cast<BufferVulkan>(src.get());
        if (mSealed || !srcVk) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::downloadBuffer: {}", mSealed ? "already sealed" : "null/invalid source buffer");
                return future;
            }

        const uint64_t bufSize = srcVk->bufferSize();
        if (offset > bufSize) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::downloadBuffer: offset {} exceeds buffer size {}", offset, bufSize);
                return future;
            }
        if (size == uint64_t(~0)) size = bufSize - offset;
        if (offset + size > bufSize) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::downloadBuffer: range [{}, {}) exceeds buffer size {}", offset, offset + size, bufSize);
                return future;
            }
        if (size == 0) return future; // nothing to download => empty blob; not an error.

        auto staging = createStaging("download_stg", size);
        if (!staging) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::downloadBuffer: staging buffer allocation failed");
                return future;
            }

        StoredDownloadBuffer op;
        op.src       = std::move(src);
        op.staging   = std::move(staging);
        op.srcOffset = offset;
        op.size      = size;
        op.result    = std::move(result);
        mOps.emplace_back(std::move(op));
        return future;
    }

    std::future<TextureContent> downloadImage(AutoRef<Texture> src, ArrayProxy<const Region> regions) override {
        // On any early return below, `result` destructs and resolves the future with an empty content.
        DownloadResult<TextureContent> result;
        auto                           future = result.future();

        auto * srcVk = RuntimeType::cast<TextureVulkanBase>(src.get());
        if (mSealed || !srcVk || regions.empty()) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::downloadImage: {}",
                         mSealed ? "already sealed" : (!srcVk ? "null/invalid source texture" : "no regions specified"));
                return future;
            }

        // Lay out the requested regions tightly in the staging buffer, honoring Vulkan's bufferOffset
        // alignment (multiple of 4 and of the texel block size). Each region is read back tightly packed.
        const auto     fmt = srcVk->descriptor().format;
        const auto &   ld  = fmt.layoutDesc();
        const uint32_t bw  = ld.blockWidth ? ld.blockWidth : 1;
        const uint32_t bh  = ld.blockHeight ? ld.blockHeight : 1;
        const uint32_t bb  = fmt.bytesPerBlock() ? fmt.bytesPerBlock() : 1;

        auto lcm4 = [](uint64_t n) -> uint64_t {
            uint64_t a = 4, b = n;
            while (b) {
                uint64_t t = a % b;
                a          = b;
                b          = t;
            }
            return (4 / a) * n; // lcm(4, n)
        };
        const uint64_t align = lcm4(bb);

        DynaArray<Buffer::StagedTexture::Region> packed;
        uint64_t                                 cursor = 0;
        for (const auto & r : regions) {
            const uint32_t w        = r.imageExtent.x ? r.imageExtent.x : 1;
            const uint32_t h        = r.imageExtent.y ? r.imageExtent.y : 1;
            const uint32_t d        = r.imageExtent.z ? r.imageExtent.z : 1;
            const uint64_t blocksX  = (w + bw - 1) / bw;
            const uint64_t blocksY  = (h + bh - 1) / bh;
            const uint64_t regBytes = blocksX * blocksY * d * bb;

            cursor = ((cursor + align - 1) / align) * align;

            Buffer::StagedTexture::Region pr = r;
            pr.bufferOffset                  = cursor;
            pr.bufferRowLength               = 0; // tightly packed: rows == imageExtent.x
            pr.bufferHeight                  = 0;
            packed.append(pr);

            cursor += regBytes;
        }

        if (cursor == 0) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::downloadImage: regions describe zero bytes");
                return future;
            }

        auto staging = createStaging("download_img_stg", cursor);
        if (!staging) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::downloadImage: staging buffer allocation failed");
                return future;
            }

        StoredDownloadImage op;
        op.src     = std::move(src);
        op.staging = std::move(staging);
        op.regions = std::move(packed);
        op.result  = std::move(result);
        mOps.emplace_back(std::move(op));
        return future;
    }

    AutoRef<GpuPayload> seal() override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::seal: double seal");
                return {};
            }
        mSealed = true;
        return AutoRef<GpuPayload>(new GpuCncPayloadVulkan(name + "/payload", std::move(mOps)));
    }

private:
    AutoRef<GpuContext>   mGpu;
    bool                  mSealed = false;
    std::vector<StoredOp> mOps;

    /// Allocate a host-visible staging buffer owned by the upcoming payload.
    AutoRef<Buffer> createStaging(const char * suffix, uint64_t size) {
        return Buffer::create(name + "/" + suffix, {.context = mGpu, .size = size, .mappable = true});
    }
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
