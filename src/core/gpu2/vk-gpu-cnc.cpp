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
#include "vk-cnc-common.h"
#include "gpu-context.h"

#include <cstring>
#include <variant>
#include <vector>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

namespace {

// ── Stored operation records (built during compute/copy calls, consumed by seal/record) ──

struct StoredCompute {
    AutoRef<GpuShader>   cs;
    GpuResourceTable     resources;
    std::vector<uint8_t> immediates;
    uint32_t             x = 1, y = 1, z = 1;
};

using StoredOp =
    std::variant<StoredCompute, StoredBufferToBuffer, StoredBufferToImage, StoredImageToImage, StoredUploadBuffer, StoredDownloadBuffer, StoredDownloadImage>;

// ── GpuCncPayloadVulkan ──────────────────────────────────────────────────────────────

class GpuCncPayloadVulkan final : public GpuPayloadVulkan {
public:
    GpuCncPayloadVulkan(const StrA & name, std::vector<StoredOp> ops, std::unique_ptr<CncUploadStorage> uploads)
        : GpuPayloadVulkan(name), mOps(std::move(ops)), mUploadStorage(std::move(uploads)) {}

    void recordForVulkanSubmit(const RecordContext & ctx) override;

    /// Fired after the GPU fence signals: read back download staging buffers, resolve their futures,
    /// and free all transient staging buffers. If the payload is dropped without submission this is
    /// never called; the DownloadResult destructor then resolves each pending future with an empty
    /// value (the public API's documented "canceled" outcome).
    void onGpuComplete() override;

private:
    std::vector<StoredOp>             mOps;
    std::unique_ptr<CncUploadStorage> mUploadStorage = std::make_unique<CncUploadStorage>();

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

    if (!op.immediates.empty()) {
        if (op.immediates.size() > 128) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncPayloadVulkan: immediates size {} exceeds 128 bytes", op.immediates.size());
            }
        else { drawable.c(0, op.immediates.size(), op.immediates.data(), vk::ShaderStageFlagBits::eCompute); }
    }

    rv::Ref<rv::Sampler>          defaultSampler;
    std::vector<rv::ImageSampler> imgs;
    std::vector<rv::BufferView>   bufs;
    for (size_t setIdx = 0; setIdx < op.resources.size(); ++setIdx) {
        const auto & set = op.resources[setIdx];
        for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
            const auto & slot = set[bindIdx];
            if (slot.empty()) continue;
            rv::DescriptorIdentifier descId((uint32_t) setIdx, (uint32_t) bindIdx);
            if (slot[0].isTexture()) {
                imgs.clear();
                imgs.reserve(slot.size());
                for (const auto & view : slot) {
                    if (view.empty() || !view.isTexture()) continue;
                    auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                    if (!tex) continue;
                    vk::ImageLayout  layout = (view.imageView.type == GpuResourceView::ImageView::STORAGE) ? vk::ImageLayout::eGeneral
                                                                                                           : shaderReadOnlyLayout(tex->descriptor().format);
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
                bufs.clear();
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
    emitBufferCopy(RuntimeType::cast<BufferVulkan>(op.staging.get()), RuntimeType::cast<BufferVulkan>(op.dst.get()), op.srcOffset, op.dstOffset, op.size, cb,
                   tracker);
}

void GpuCncPayloadVulkan::recordDownloadBuffer(const StoredDownloadBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    emitBufferCopy(RuntimeType::cast<BufferVulkan>(op.src.get()), RuntimeType::cast<BufferVulkan>(op.staging.get()), op.srcOffset, 0, op.size, cb, tracker);
}

// ── Buffer-to-image copy ─────────────────────────────────────────────────────────────

void GpuCncPayloadVulkan::recordBufToImg(const StoredBufferToImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    GN::gpu2::recordBufToImg(op, cb, tracker);
}

// ── Image-to-buffer copy (download) ──────────────────────────────────────────────────

void GpuCncPayloadVulkan::recordDownloadImage(const StoredDownloadImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    GN::gpu2::recordDownloadImage(op, cb, tracker);
}

// ── Download read-back (CPU side, after GPU completion) ───────────────────────────────

void GpuCncPayloadVulkan::resolveDownloadBuffer(StoredDownloadBuffer & op) { GN::gpu2::resolveDownloadBuffer(op); }

void GpuCncPayloadVulkan::resolveDownloadImage(StoredDownloadImage & op) { GN::gpu2::resolveDownloadImage(op); }

void GpuCncPayloadVulkan::onGpuComplete() {
    for (auto & op : mOps) {
        std::visit(
            [&](auto & o) {
                using T = std::decay_t<decltype(o)>;
                if constexpr (std::is_same_v<T, StoredUploadBuffer>)
                    o.staging.clear(); // transient upload staging done; release immediately.
                else if constexpr (std::is_same_v<T, StoredBufferToImage>)
                    o.src.clear();
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
    mUploadStorage->unmapForSubmit();
    if (!ctx.dev || ctx.cmd.empty() || !ctx.batchTracker) return;
    GpuResourceStateTrackerVulkan & tracker = *ctx.batchTracker;
    vk::CommandBuffer               vkcb    = ctx.cmd.handle();

    std::vector<BufferVulkan *>                                  writtenBuffers;
    std::vector<std::pair<TextureVulkanBase *, GpuResourceView>> writtenTextures;

    auto addWrittenBuffer = [&](BufferVulkan * b) {
        if (b && std::find(writtenBuffers.begin(), writtenBuffers.end(), b) == writtenBuffers.end()) { writtenBuffers.push_back(b); }
    };
    auto addWrittenTexture = [&](TextureVulkanBase * t, const GpuResourceView & v) {
        if (!t) return;
        for (const auto & [existingT, existingV] : writtenTextures) {
            if (existingT == t) return;
        }
        writtenTextures.push_back({t, v});
    };

    for (const auto & op : mOps) {
        std::visit(
            [&](const auto & o) {
                using T = std::decay_t<decltype(o)>;
                if constexpr (std::is_same_v<T, StoredCompute>) {
                    recordCompute(o, ctx);
                    for (size_t setIdx = 0; setIdx < o.resources.size(); ++setIdx) {
                        const auto & set = o.resources[setIdx];
                        for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
                            const auto & slot = set[bindIdx];
                            for (const auto & view : slot) {
                                if (view.empty()) continue;
                                if (view.isBuffer() && view.bufferView.type == GpuResourceView::BufferView::STORAGE) {
                                    auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                                    addWrittenBuffer(buf);
                                } else if (view.isTexture() && view.imageView.type == GpuResourceView::ImageView::STORAGE) {
                                    auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                                    addWrittenTexture(tex, view);
                                }
                            }
                        }
                    }
                } else if constexpr (std::is_same_v<T, StoredBufferToBuffer>) {
                    recordBufToBuf(o, vkcb, tracker);
                    auto * dstVk = RuntimeType::cast<BufferVulkan>(o.dst.get());
                    addWrittenBuffer(dstVk);
                    addWrittenBuffer(RuntimeType::cast<BufferVulkan>(o.src.get()));
                } else if constexpr (std::is_same_v<T, StoredBufferToImage>) {
                    recordBufToImg(o, vkcb, tracker);
                    auto * dstVk = RuntimeType::cast<TextureVulkanBase>(o.dst.get());
                    addWrittenTexture(dstVk, GpuResourceView {});
                } else if constexpr (std::is_same_v<T, StoredImageToImage>) {
                    recordImageCopy(o, vkcb, tracker);
                    addWrittenTexture(RuntimeType::cast<TextureVulkanBase>(o.src.get()), GpuResourceView {});
                    addWrittenTexture(RuntimeType::cast<TextureVulkanBase>(o.dst.get()), GpuResourceView {});
                } else if constexpr (std::is_same_v<T, StoredUploadBuffer>) {
                    recordUploadBuffer(o, vkcb, tracker);
                    auto * dstVk = RuntimeType::cast<BufferVulkan>(o.dst.get());
                    addWrittenBuffer(dstVk);
                } else if constexpr (std::is_same_v<T, StoredDownloadBuffer>) {
                    recordDownloadBuffer(o, vkcb, tracker);
                    addWrittenBuffer(RuntimeType::cast<BufferVulkan>(o.src.get()));
                } else if constexpr (std::is_same_v<T, StoredDownloadImage>) {
                    recordDownloadImage(o, vkcb, tracker);
                    auto * srcVk = RuntimeType::cast<TextureVulkanBase>(o.src.get());
                    addWrittenTexture(srcVk, GpuResourceView {});
                }
            },
            op);
    }

    if (!writtenBuffers.empty()) { tracker.restoreBuffersToReadReady(writtenBuffers, vkcb); }
    for (const auto & [tex, view] : writtenTextures) { tracker.restoreAttachmentToShaderReadOnly(tex, view, vkcb); }
}

// ── GpuCncVulkan2 ────────────────────────────────────────────────────────────────────

class GpuCncVulkan2 final : public GpuCnC {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuCnC);

    bool hasRecordedWork() const { return !mOps.empty(); }

    GpuCncVulkan2(const StrA & entityName, const CreateParameters & cp): GpuCnC(TYPE_INFO(), entityName), mGpu(cp.gpu) {}

    void recordCompute(const ComputeParameters & cp) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordCompute: already sealed");
                return;
            }
        StoredCompute op;
        op.cs        = cp.cs;
        op.resources = cp.resources;
        op.x         = cp.x;
        op.y         = cp.y;
        op.z         = cp.z;
        if (!cp.immediates.empty()) { op.immediates.assign(cp.immediates.begin(), cp.immediates.end()); }
        mOps.emplace_back(std::move(op));
    }

    void recordCopyBuffer(const BufferToBuffer & p) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordCopyBuffer: already sealed");
                return;
            }
        mOps.emplace_back(StoredBufferToBuffer {p.src, p.dst, p.srcOffset, p.dstOffset, p.size});
    }

    void recordUploadImage(AutoRef<Texture> dst, ArrayView<const uint8_t> content, ArrayView<const Region> regions) override {
        if (mSealed || !validImageUpload(dst.get(), content, regions)) GN_UNLIKELY {
                GN_ERROR(sLogger, "CNC::recordUploadImage: sealed recorder or empty upload");
                return;
            }
        StoredBufferToImage op;
        op.dst = dst;
        for (auto region : regions) {
            auto slice = mUploadStorage->copyImage(mGpu, name, content, dst->descriptor(), region);
            if (!slice.buffer) GN_UNLIKELY {
                    GN_ERROR(sLogger, "CNC::recordUploadImage: staging allocation failed");
                    return;
                }
            if (op.src && op.src.get() != slice.buffer.get()) {
                mOps.emplace_back(std::move(op));
                op     = StoredBufferToImage {};
                op.dst = dst;
            }
            op.src                 = std::move(slice.buffer);
            region.dataOffset      = slice.offset;
            region.rowPitchBytes   = 0;
            region.slicePitchBytes = 0;
            op.regions.append(region);
        }
        mOps.emplace_back(std::move(op));
    }

    void recordCopyImage(const ImageToImage & p) override {
        if (mSealed || !validImageCopy(p)) GN_UNLIKELY {
                GN_ERROR(sLogger, "CNC::recordCopyImage: sealed recorder or invalid copy resources");
                return;
            }
        StoredImageToImage op;
        op.src = p.src;
        op.dst = p.dst;
        for (const auto & r : p.regions) op.regions.append(r);
        mOps.emplace_back(std::move(op));
    }

    void recordUploadBuffer(AutoRef<Buffer> dst, uint64_t offset, ArrayView<const uint8_t> content) override {
        if (mSealed) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordUploadBuffer: already sealed");
                return;
            }
        if (!dst || content.empty()) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordUploadBuffer: null destination or empty content");
                return;
            }
        const uint64_t size  = content.size();
        auto           slice = mUploadStorage->copy(mGpu, name, content);
        if (!slice.buffer) GN_UNLIKELY {
                GN_ERROR(sLogger, "CNC::recordUploadBuffer: staging allocation failed");
                return;
            }
        StoredUploadBuffer op;
        op.staging   = std::move(slice.buffer);
        op.dst       = std::move(dst);
        op.dstOffset = offset;
        op.size      = size;
        op.srcOffset = slice.offset;
        mOps.emplace_back(std::move(op));
    }

    std::future<AutoRef<const Blob>> recordDownloadBuffer(AutoRef<Buffer> src, uint64_t offset, uint64_t size) override {
        // On any early return below, `result` destructs and resolves the future with an empty blob.
        DownloadResult<AutoRef<const Blob>> result;
        auto                                future = result.future();

        auto * srcVk = RuntimeType::cast<BufferVulkan>(src.get());
        if (mSealed || !srcVk) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordDownloadBuffer: {}", mSealed ? "already sealed" : "null/invalid source buffer");
                return future;
            }

        const uint64_t bufSize = srcVk->bufferSize();
        if (offset > bufSize) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordDownloadBuffer: offset {} exceeds buffer size {}", offset, bufSize);
                return future;
            }
        if (size == uint64_t(~0)) size = bufSize - offset;
        if (offset + size > bufSize) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordDownloadBuffer: range [{}, {}) exceeds buffer size {}", offset, offset + size, bufSize);
                return future;
            }
        if (size == 0) return future; // nothing to download => empty blob; not an error.

        auto staging = createStaging("download_stg", size);
        if (!staging) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordDownloadBuffer: staging buffer allocation failed");
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

    std::future<TextureContent> recordDownloadImage(AutoRef<Texture> src, ArrayView<const Region> regions) override {
        // On any early return below, `result` destructs and resolves the future with an empty content.
        DownloadResult<TextureContent> result;
        auto                           future = result.future();

        auto * srcVk = RuntimeType::cast<TextureVulkanBase>(src.get());
        if (mSealed || !srcVk || regions.empty()) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordDownloadImage: {}",
                         mSealed ? "already sealed" : (!srcVk ? "null/invalid source texture" : "no regions specified"));
                return future;
            }

        if (srcVk->descriptor().samples != 1 || !singleCopyAspect(srcVk->descriptor())) GN_UNLIKELY {
                GN_ERROR(sLogger, "CNC::recordDownloadImage: image downloads require a single-sample, single-aspect format");
                return future;
            }
        for (const auto & region : regions) {
            if (!validImageRegion(srcVk->descriptor(), region.mip, region.face, region.imageOffset, region.imageExtent)) GN_UNLIKELY {
                    GN_ERROR(sLogger, "CNC::recordDownloadImage: invalid image region");
                    return future;
                }
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

        DynaArray<GpuCnC::Region> packed;
        uint64_t                  cursor = 0;
        for (const auto & r : regions) {
            const uint32_t w        = r.imageExtent.x ? r.imageExtent.x : 1;
            const uint32_t h        = r.imageExtent.y ? r.imageExtent.y : 1;
            const uint32_t d        = r.imageExtent.z ? r.imageExtent.z : 1;
            const uint64_t blocksX  = (w + bw - 1) / bw;
            const uint64_t blocksY  = (h + bh - 1) / bh;
            const uint64_t regBytes = blocksX * blocksY * d * bb;

            cursor = ((cursor + align - 1) / align) * align;

            GpuCnC::Region pr  = r;
            pr.dataOffset      = cursor;
            pr.rowPitchBytes   = blocksX * bb;
            pr.slicePitchBytes = blocksX * blocksY * bb;
            packed.append(pr);

            cursor += regBytes;
        }

        if (cursor == 0) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordDownloadImage: regions describe zero bytes");
                return future;
            }

        auto staging = createStaging("download_img_stg", cursor);
        if (!staging) GN_UNLIKELY {
                GN_ERROR(sLogger, "GpuCncVulkan2::recordDownloadImage: staging buffer allocation failed");
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
        return AutoRef<GpuPayload>(new GpuCncPayloadVulkan(name + "/payload", std::move(mOps), std::move(mUploadStorage)));
    }

private:
    AutoRef<GpuContext>               mGpu;
    bool                              mSealed = false;
    std::vector<StoredOp>             mOps;
    std::unique_ptr<CncUploadStorage> mUploadStorage = std::make_unique<CncUploadStorage>();

    /// Allocate a host-visible staging buffer owned by the upcoming payload.
    AutoRef<Buffer> createStaging(const char * suffix, uint64_t size) {
        return Buffer::create(name + "/" + suffix, {.context = mGpu, .size = size, .mappable = true});
    }
};

} // anonymous namespace

AutoRef<GpuPayload> createCncBufferUploadPayload(AutoRef<GpuContext> gpu, AutoRef<Buffer> dst, ArrayView<const uint8_t> content, uint64_t offset) {
    auto cnc = AutoRef<GpuCncVulkan2>(new GpuCncVulkan2(dst->name + "/upload", {.gpu = std::move(gpu)}));
    cnc->recordUploadBuffer(std::move(dst), offset, content);
    return cnc->hasRecordedWork() ? cnc->seal() : AutoRef<GpuPayload> {};
}

AutoRef<GpuPayload> createCncImageUploadPayload(AutoRef<GpuContext> gpu, AutoRef<Texture> dst, const gfx::img::Image & content) {
    auto cnc = AutoRef<GpuCncVulkan2>(new GpuCncVulkan2(dst->name + "/upload", {.gpu = std::move(gpu)}));
    cnc->GpuCnC::recordUploadImage(std::move(dst), content);
    return cnc->hasRecordedWork() ? cnc->seal() : AutoRef<GpuPayload> {};
}

// ── Factory ──────────────────────────────────────────────────────────────────────────

AutoRef<GpuCnC> createGpuCncVulkan2(const GpuCnC::CreateParameters & params) {
    if (!params.gpu) return {};
    auto vkGpu = params.gpu.staticCastTo<GpuContextVulkan2>();
    if (!vkGpu || !vkGpu->ready()) return {};
    StrA n = params.gpu->name.empty() ? StrA("cnc") : params.gpu->name + "/cnc";
    return AutoRef<GpuCnC>(new GpuCncVulkan2(n, params));
}

} // namespace GN::gpu2
