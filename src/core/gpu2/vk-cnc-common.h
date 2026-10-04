#pragma once

#include "vk-gpu-context.h"
#include "vk-gpu-resource-state-tracker.h"
#include "vk-buffer.h"
#include "vk-texture.h"
#include "vk-format-utils.h"
#include <garnet/GNgpu2.h>
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
    AutoRef<Buffer>                          src;
    AutoRef<Texture>                         dst;
    DynaArray<Buffer::StagedTexture::Region> regions;
};

struct StoredUploadBuffer {
    AutoRef<Buffer> staging;
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
    AutoRef<Texture>                         src;
    AutoRef<Buffer>                          staging;
    DynaArray<Buffer::StagedTexture::Region> regions;
    DownloadResult<GpuCnC::TextureContent>   result;
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
        c.setBufferOffset(r.bufferOffset)
            .setBufferRowLength(r.bufferRowLength)
            .setBufferImageHeight(r.bufferHeight)
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
