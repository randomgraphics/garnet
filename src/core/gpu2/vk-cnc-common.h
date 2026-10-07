#pragma once

#include "vk-gpu-context.h"
#include "vk-gpu-resource-state-tracker.h"
#include "vk-buffer.h"
#include "vk-texture.h"
#include "vk-format-utils.h"
#include <garnet/GNgpu2.h>
#include <cstring>
#include <future>
#include <memory>
#include <vector>

namespace GN::gpu2 {

// A movable promise wrapper that always signals its future exactly once.
template<typename T>
class DownloadResult {
public:
    DownloadResult(): mPromise(std::make_unique<std::promise<T>>()) {}
    DownloadResult(DownloadResult &&) noexcept             = default;
    DownloadResult & operator=(DownloadResult &&) noexcept = default;
    ~DownloadResult() { resolve(T {}); }

    std::future<T> future() { return mPromise->get_future(); }

    void resolve(T value) {
        if (!mPromise) return;
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

struct StoredImageToImage {
    AutoRef<Texture>                   src;
    AutoRef<Texture>                   dst;
    DynaArray<GpuCnC::ImageCopyRegion> regions;
};

inline bool validImageRegion(const Texture::Descriptor & desc, uint32_t mip, uint32_t face, const Vector3<uint32_t> & offset,
                             const Vector3<uint32_t> & extent) {
    if (mip >= desc.levels || face >= desc.faces || mip >= 32) return false;
    const uint32_t w = std::max(1u, desc.width >> mip), h = std::max(1u, desc.height >> mip), d = std::max(1u, desc.depth >> mip);
    if (!extent.x || !extent.y || !extent.z || offset.x > w || offset.y > h || offset.z > d || extent.x > w - offset.x || extent.y > h - offset.y ||
        extent.z > d - offset.z)
        return false;
    const auto     layout = desc.format.layoutDesc();
    const uint32_t bw = std::max(1u, (uint32_t) layout.blockWidth), bh = std::max(1u, (uint32_t) layout.blockHeight);
    return offset.x % bw == 0 && offset.y % bh == 0 && (extent.x % bw == 0 || offset.x + extent.x == w) && (extent.y % bh == 0 || offset.y + extent.y == h);
}

struct CncImageLayout {
    uint64_t rowBytes = 0, rows = 0, rowPitch = 0, slicePitch = 0, size = 0;
};

inline bool imageCpuLayout(const Texture::Descriptor & desc, const GpuCnC::Region & r, CncImageLayout & layout) {
    if (!validImageRegion(desc, r.mip, r.face, r.imageOffset, r.imageExtent)) return false;
    const auto     format = desc.format.layoutDesc();
    const uint64_t bw = std::max(1u, (uint32_t) format.blockWidth), bh = std::max(1u, (uint32_t) format.blockHeight);
    const uint64_t bb = desc.format.bytesPerBlock();
    if (!bb) return false;
    layout.rowBytes = ((r.imageExtent.x + bw - 1) / bw) * bb;
    layout.rows     = (r.imageExtent.y + bh - 1) / bh;
    layout.rowPitch = r.rowPitchBytes ? r.rowPitchBytes : layout.rowBytes;
    if (layout.rowPitch < layout.rowBytes || layout.rowPitch > UINT64_MAX / layout.rows) return false;
    const uint64_t planeBytes         = layout.rowPitch * layout.rows;
    layout.slicePitch                 = r.slicePitchBytes ? r.slicePitchBytes : planeBytes;
    const uint64_t requiredPlaneBytes = (layout.rows - 1) * layout.rowPitch + layout.rowBytes;
    if (layout.slicePitch < requiredPlaneBytes || layout.slicePitch > UINT64_MAX / r.imageExtent.z) return false;
    layout.size = layout.rowBytes * layout.rows * r.imageExtent.z;
    return true;
}

inline bool singleCopyAspect(const Texture::Descriptor & desc) {
    const auto mask = (VkImageAspectFlags) aspectFromViewFormat(desc.format, desc.format);
    return mask && !(mask & (mask - 1));
}

inline bool validImageUpload(Texture * dst, ArrayView<const uint8_t> content, ArrayView<const GpuCnC::Region> regions) {
    if (!RuntimeType::cast<TextureVulkanBase>(dst) || !content.data() || content.empty() || regions.empty()) return false;
    const auto & desc = dst->descriptor();
    if (desc.samples != 1 || !singleCopyAspect(desc)) return false;
    for (const auto & r : regions) {
        CncImageLayout layout;
        if (!imageCpuLayout(desc, r, layout) || r.dataOffset > content.size()) return false;
        // Only bytes actually read must exist; the last row/slice need not include trailing padding.
        const uint64_t preceding = (r.imageExtent.z - 1) * layout.slicePitch + (layout.rows - 1) * layout.rowPitch;
        const uint64_t available = content.size() - r.dataOffset;
        if (preceding > available || layout.rowBytes > available - preceding) return false;
    }
    return true;
}

inline bool validImageCopy(const GpuCnC::ImageToImage & p) {
    auto * src = RuntimeType::cast<TextureVulkanBase>(p.src.get());
    auto * dst = RuntimeType::cast<TextureVulkanBase>(p.dst.get());
    if (!src || !dst || src == dst || p.regions.empty()) return false;
    const auto & a = src->descriptor();
    const auto & b = dst->descriptor();
    if (a.format != b.format || a.samples != b.samples) return false;
    for (const auto & r : p.regions) {
        if (!validImageRegion(a, r.srcMip, r.srcFace, r.srcOffset, r.extent) || !validImageRegion(b, r.dstMip, r.dstFace, r.dstOffset, r.extent)) return false;
    }
    return true;
}

// CNC-only upload storage. Mappings follow the recording into its payload and close at submission.
// Operations retain backing buffers separately, so neither arena growth nor unmapping invalidates GPU bytes.
class CncUploadStorage {
public:
    struct Slice {
        AutoRef<Buffer> buffer;
        uint64_t        offset = 0;
        uint8_t *       data   = nullptr;
    };
    Slice allocate(AutoRef<GpuContext> gpu, const StrA & name, uint64_t size, uint64_t alignment = 256) {
        if (!size) return {};
        uint64_t offset = mBlocks.empty() ? 0 : ((mBlocks.back().used + alignment - 1) / alignment) * alignment;
        if (mBlocks.empty() || offset > mBlocks.back().mapped.size() || size > mBlocks.back().mapped.size() - offset) {
            const uint64_t capacity = std::max<uint64_t>(1024 * 1024, size);
            auto           buffer   = Buffer::create(name + "/upload-arena", {.context = gpu, .size = capacity, .mappable = true});
            if (!buffer) return {};
            auto mapped = buffer->map();
            if (!mapped.data()) return {};
            mBlocks.push_back({std::move(buffer), std::move(mapped), 0});
            offset = 0;
        }
        auto & block = mBlocks.back();
        block.used   = offset + size;
        return {block.buffer, offset, (uint8_t *) block.mapped.data() + offset};
    }
    Slice copy(AutoRef<GpuContext> gpu, const StrA & name, ArrayView<const uint8_t> content) {
        auto slice = allocate(gpu, name, content.size());
        if (slice.buffer) memcpy(slice.data, content.data(), content.size());
        return slice;
    }
    Slice copyImage(AutoRef<GpuContext> gpu, const StrA & name, ArrayView<const uint8_t> content, const Texture::Descriptor & desc,
                    const GpuCnC::Region & region) {
        CncImageLayout layout;
        if (!imageCpuLayout(desc, region, layout)) return {};
        // GPU copy offsets obey block-byte alignment; arbitrary CPU pitches/offsets never escape into Vulkan.
        auto slice = allocate(gpu, name, layout.size, 4 * desc.format.bytesPerBlock());
        if (!slice.buffer) return {};
        const auto * source = content.data() + region.dataOffset;
        for (uint32_t z = 0; z < region.imageExtent.z; ++z)
            for (uint64_t row = 0; row < layout.rows; ++row)
                memcpy(slice.data + (z * layout.rows + row) * layout.rowBytes, source + z * layout.slicePitch + row * layout.rowPitch,
                       (size_t) layout.rowBytes);
        return slice;
    }
    void unmapForSubmit() {
        // Mappable gpu2 buffers use HOST_COHERENT memory: no explicit flush is needed.
        // Clear only CPU mappings here; stored operations own buffers until GPU completion.
        mBlocks.clear();
    }

private:
    struct Block {
        AutoRef<Buffer> buffer;
        Buffer::Mapped  mapped;
        uint64_t        used;
    };
    std::vector<Block> mBlocks;
};

struct StoredUploadBuffer {
    AutoRef<Buffer> staging;
    uint64_t        srcOffset = 0;
    AutoRef<Buffer> dst;
    uint64_t        dstOffset = 0;
    uint64_t        size      = 0;
};

struct StoredDownloadBuffer {
    AutoRef<Buffer>                     src;
    AutoRef<Buffer>                     staging;
    uint64_t                            srcOffset = 0;
    uint64_t                            size      = 0;
    DownloadResult<AutoRef<const Blob>> result;
};

struct StoredDownloadImage {
    AutoRef<Texture>                       src;
    AutoRef<Buffer>                        staging;
    DynaArray<GpuCnC::Region>              regions;
    DownloadResult<GpuCnC::TextureContent> result;
};

inline void emitBufferCopy(BufferVulkan * srcVk, BufferVulkan * dstVk, uint64_t srcOffset, uint64_t dstOffset, uint64_t size, vk::CommandBuffer cb,
                           GpuResourceStateTrackerVulkan & tracker) {
    if (size == 0) return;
    if (!srcVk || !dstVk) GN_UNLIKELY {
            return;
        }
    vk::Buffer srcBuf = srcVk->nativeBuffer();
    vk::Buffer dstBuf = dstVk->nativeBuffer();
    if (!srcBuf || !dstBuf) GN_UNLIKELY {
            return;
        }

    tracker.addTransferSrcBuffer(srcVk);
    tracker.addTransferDstBuffer(dstVk);
    tracker.emitPrePassBarriers(cb);

    cb.copyBuffer(srcBuf, dstBuf, vk::BufferCopy(srcOffset, dstOffset, size));
}

inline void recordBufToImg(const StoredBufferToImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    if (op.regions.empty()) return;

    auto * srcVk = RuntimeType::cast<BufferVulkan>(op.src.get());
    auto * dstVk = RuntimeType::cast<TextureVulkanBase>(op.dst.get());
    if (!srcVk || !dstVk) GN_UNLIKELY {
            return;
        }

    vk::Buffer srcBuf = srcVk->nativeBuffer();
    vk::Image  dstImg = dstVk->nativeImage();
    if (!srcBuf || !dstImg) GN_UNLIKELY {
            return;
        }

    tracker.addTransferSrcBuffer(srcVk);
    GpuResourceView::ImageView fullRange;
    tracker.addTransferDstImage(dstVk, fullRange);
    tracker.emitPrePassBarriers(cb);

    const auto & desc    = dstVk->descriptor();
    auto         aspects = aspectFromViewFormat(desc.format, desc.format);
    if (!aspects) aspects = vk::ImageAspectFlagBits::eColor;

    std::vector<vk::BufferImageCopy> copies;
    copies.reserve(op.regions.size());
    for (const auto & r : op.regions) {
        vk::BufferImageCopy c;
        c.setBufferOffset(r.dataOffset)
            .setBufferRowLength(0)
            .setBufferImageHeight(0)
            .setImageSubresource({aspects, r.mip, r.face, 1})
            .setImageOffset({(int32_t) r.imageOffset.x, (int32_t) r.imageOffset.y, (int32_t) r.imageOffset.z})
            .setImageExtent({r.imageExtent.x, r.imageExtent.y, r.imageExtent.z});
        copies.push_back(c);
    }

    cb.copyBufferToImage(srcBuf, dstImg, vk::ImageLayout::eTransferDstOptimal, (uint32_t) copies.size(), copies.data());
}

inline void recordImageCopy(const StoredImageToImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    auto * src = RuntimeType::cast<TextureVulkanBase>(op.src.get());
    auto * dst = RuntimeType::cast<TextureVulkanBase>(op.dst.get());
    if (!src || !dst || src == dst || op.regions.empty()) GN_UNLIKELY return;
    auto aspects = aspectFromViewFormat(src->descriptor().format, src->descriptor().format);
    if (!aspects) aspects = vk::ImageAspectFlagBits::eColor;
    GpuResourceView::ImageView fullRange;
    tracker.addTransferSrcImage(src, fullRange);
    tracker.addTransferDstImage(dst, fullRange);
    tracker.emitPrePassBarriers(cb);
    std::vector<vk::ImageCopy> copies;
    for (const auto & r : op.regions) {
        // Depth and stencil are separate copy regions in Vulkan, even for a combined format.
        for (auto aspect : {vk::ImageAspectFlagBits::eColor, vk::ImageAspectFlagBits::eDepth, vk::ImageAspectFlagBits::eStencil}) {
            if (!(aspects & aspect)) continue;
            vk::ImageCopy c;
            c.setSrcSubresource({aspect, r.srcMip, r.srcFace, 1})
                .setDstSubresource({aspect, r.dstMip, r.dstFace, 1})
                .setSrcOffset({(int32_t) r.srcOffset.x, (int32_t) r.srcOffset.y, (int32_t) r.srcOffset.z})
                .setDstOffset({(int32_t) r.dstOffset.x, (int32_t) r.dstOffset.y, (int32_t) r.dstOffset.z})
                .setExtent({r.extent.x, r.extent.y, r.extent.z});
            copies.push_back(c);
        }
    }
    cb.copyImage(src->nativeImage(), vk::ImageLayout::eTransferSrcOptimal, dst->nativeImage(), vk::ImageLayout::eTransferDstOptimal, (uint32_t) copies.size(),
                 copies.data());
}

inline void recordDownloadImage(const StoredDownloadImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
    if (op.regions.empty()) return;

    auto * srcVk = RuntimeType::cast<TextureVulkanBase>(op.src.get());
    auto * dstVk = RuntimeType::cast<BufferVulkan>(op.staging.get());
    if (!srcVk || !dstVk) GN_UNLIKELY {
            return;
        }

    vk::Image  srcImg = srcVk->nativeImage();
    vk::Buffer dstBuf = dstVk->nativeBuffer();
    if (!srcImg || !dstBuf) GN_UNLIKELY {
            return;
        }

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
        c.setBufferOffset(r.dataOffset)
            .setBufferRowLength(0)
            .setBufferImageHeight(0)
            .setImageSubresource({aspects, r.mip, r.face, 1})
            .setImageOffset({(int32_t) r.imageOffset.x, (int32_t) r.imageOffset.y, (int32_t) r.imageOffset.z})
            .setImageExtent({r.imageExtent.x, r.imageExtent.y, r.imageExtent.z});
        copies.push_back(c);
    }

    cb.copyImageToBuffer(srcImg, vk::ImageLayout::eTransferSrcOptimal, dstBuf, (uint32_t) copies.size(), copies.data());
}

inline void resolveDownloadBuffer(StoredDownloadBuffer & op) {
    AutoRef<const Blob> blob;
    if (op.staging && op.size > 0) {
        auto m = op.staging->map();
        if (m.data()) { blob = AutoRef<const Blob>(new SimpleBlob<uint8_t>((size_t) op.size, (const uint8_t *) m.data())); }
    }
    op.staging.clear();
    op.result.resolve(std::move(blob));
}

inline void resolveDownloadImage(StoredDownloadImage & op) {
    GpuCnC::TextureContent content;
    if (op.staging && !op.regions.empty()) {
        auto m = op.staging->map();
        if (m.data()) {
            content.blob = AutoRef<const Blob>(new SimpleBlob<uint8_t>(m.size(), (const uint8_t *) m.data()));
            for (const auto & r : op.regions) content.regions.append(r);
        }
    }
    op.staging.clear();
    op.result.resolve(std::move(content));
}

} // namespace GN::gpu2
