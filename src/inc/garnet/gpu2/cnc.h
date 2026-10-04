// Non-graphics GPU operations: compute dispatches and copy commands.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/cnc.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

#include <future>

namespace GN::gpu2 {

/// Accumulates non-graphics GPU work (compute dispatches and copies), then produces
/// a sealed GpuPayload for submission via GpuContext::submit().
/// record*() methods prepare and retain commands; GPU execution starts only after submission.
/// Recording uploads may allocate staging storage and copy CPU data immediately.
/// One-shot recorder: seal() finalizes the payload; no further recording or sealing
/// is allowed afterward. This class is not thread-safe. All calls on one instance
/// must be single-threaded or externally serialized; use separate instances for parallel recording.
struct GpuCnC : public RCRT64 {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    GN_API static AutoRef<GpuCnC> create(const CreateParameters &);

    struct ComputeParameters {
        AutoRef<GpuShader>       cs;
        GpuResourceTable         resources;  ///< shader resources
        ArrayView<const uint8_t> immediates; ///< Inline uniform data (root constants / push constants).
        uint32_t                 x = 1;
        uint32_t                 y = 1;
        uint32_t                 z = 1;
    };
    /// Record a compute dispatch in the pending payload without executing it.
    virtual void recordCompute(const ComputeParameters &) = 0;

    struct BufferToBuffer {
        AutoRef<Buffer> src;
        AutoRef<Buffer> dst;
        uint64_t        srcOffset = 0; ///< Byte offset within the source buffer.
        uint64_t        dstOffset = 0; ///< Byte offset within the destination buffer.
        uint64_t        size      = 0; ///< Number of bytes to copy. 0 = copy nothing.
    };

    /// Record a buffer copy for execution when the sealed payload is submitted.
    virtual void recordCopyBuffer(const BufferToBuffer &) = 0;

    /// Record a buffer upload. Content is copied into internal staging storage during recording;
    /// the GPU transfer executes after the sealed payload is submitted.
    virtual void recordUploadBuffer(AutoRef<Buffer> dst, uint64_t offset, ArrayView<const uint8_t> content) = 0;

    /// @brief Enqueue a buffer download operation. The buffer content is copied from the source buffer into an internal staging buffer, then
    /// a CPU-side copy is performed to transfer the data into a new Blob. The returned future is always signaled exactly once: with the
    /// downloaded Blob on success, or with an empty Blob if the download failed or was canceled (e.g. the sealed payload was dropped
    /// without submission, or the GpuContext was destroyed before the work completed). The future never throws.
    virtual std::future<AutoRef<const Blob>> recordDownloadBuffer(AutoRef<Buffer> src, uint64_t offset = 0, uint64_t size = uint64_t(~0)) = 0;

    /// Describes one buffer→image copy region. Reuses Buffer::StagedTexture::Region
    /// to allow direct pass-through from loadTextureToStagingBuffer() without conversion.
    using Region = Buffer::StagedTexture::Region;

    /// Upload pixel data from a CPU-visible (mappable) staging buffer into a texture.
    struct BufferToImage {
        AutoRef<Buffer>         src; ///< CPU-visible staging buffer.
        AutoRef<Texture>        dst;
        ArrayView<const Region> regions;
    };

    /// Record a buffer-to-image copy, retaining resources and copying the region descriptions.
    virtual void recordCopyBufferToImage(const BufferToImage &) = 0;

    /// Convenience: upload all regions from a StagedTexture into dst without any conversion.
    void recordCopyBufferToImage(const Buffer::StagedTexture & staged, AutoRef<Texture> dst) {
        recordCopyBufferToImage({.src = staged.staging, .dst = std::move(dst), .regions = staged.regions});
    }

    struct TextureContent {
        AutoRef<const Blob> blob;
        DynaArray<Region>   regions;
    };

    /// @brief Enqueue a texture download operation. The data is copied from the source texture into an internal staging buffer, then
    /// a CPU-side copy is performed to transfer the data into a new Blob. The returned future is always signaled exactly once: with the
    /// downloaded TextureContent on success, or with an empty TextureContent (empty blob, empty regions) if the download failed or was
    /// canceled (e.g. the sealed payload was dropped without submission, or the GpuContext was destroyed before the work completed).
    /// The future never throws.
    virtual std::future<TextureContent> recordDownloadImage(AutoRef<Texture> src, ArrayView<const Region> regions) = 0;

    /// Seal the object. Generate payload for all enqueued operations.
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::gpu2
