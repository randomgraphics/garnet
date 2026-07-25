// Non-graphics GPU operations: compute dispatches and copy commands.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/cnc.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

#include <future>

namespace GN::gpu2 {

/// Accumulates non-graphics GPU work (compute dispatches and copies), then produces
/// a sealed GpuPayload for submission via GpuContext::submit().
struct GpuCnC : public RCRT64 {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    GN_API static AutoRef<GpuCnC> create(const CreateParameters &);

    struct ComputeParameters {
        AutoRef<GpuShader>  cs;
        GpuResourceTable    resources;  ///< shader resources
        AutoRef<const Blob> immediates; ///< reference counted immediate constants.
        uint32_t            x = 1;
        uint32_t            y = 1;
        uint32_t            z = 1;
    };
    virtual void compute(const ComputeParameters &) = 0;

    struct BufferToBuffer {
        AutoRef<Buffer> src;
        AutoRef<Buffer> dst;
        uint64_t        srcOffset = 0; ///< Byte offset within the source buffer.
        uint64_t        dstOffset = 0; ///< Byte offset within the destination buffer.
        uint64_t        size      = 0; ///< Number of bytes to copy. 0 = copy nothing.
    };

    virtual void copyBufferToBuffer(const BufferToBuffer &) = 0;

    /// @brief Enqueue a buffer upload operation. The content is copied into an internal staging buffer, then
    /// a GPU copy command is issued to transfer the content into the destination buffer.
    virtual void uploadBuffer(AutoRef<Buffer> dst, uint64_t offset, AutoRef<const Blob> content) = 0;

    void uploadBuffer(AutoRef<Buffer> dst, uint64_t offset, ArrayProxy<const uint8_t> content) {
        uploadBuffer(std::move(dst), offset, referenceTo(new SimpleBlob<uint8_t>(content.size(), content.data())));
    }

    /// @brief Enqueue a buffer download operation. The buffer content is copied from the source buffer into an internal staging buffer, then
    /// a CPU-side copy is performed to transfer the data into a new Blob. The returned future is always signaled exactly once: with the
    /// downloaded Blob on success, or with an empty Blob if the download failed or was canceled (e.g. the sealed payload was dropped
    /// without submission, or the GpuContext was destroyed before the work completed). The future never throws.
    virtual std::future<AutoRef<const Blob>> downloadBuffer(AutoRef<Buffer> src, uint64_t offset = 0, uint64_t size = uint64_t(~0)) = 0;

    /// Describes one buffer→image copy region. Reuses Buffer::StagedTexture::Region
    /// to allow direct pass-through from loadTextureToStagingBuffer() without conversion.
    using Region = Buffer::StagedTexture::Region;

    /// Upload pixel data from a CPU-visible (mappable) staging buffer into a texture.
    struct BufferToImage {
        AutoRef<Buffer>          src; ///< CPU-visible staging buffer.
        AutoRef<Texture>         dst;
        ArrayProxy<const Region> regions;
    };

    virtual void copyBufferToImage(const BufferToImage &) = 0;

    /// Convenience: upload all regions from a StagedTexture into dst without any conversion.
    void copyBufferToImage(const Buffer::StagedTexture & staged, AutoRef<Texture> dst) {
        copyBufferToImage({.src = staged.staging, .dst = std::move(dst), .regions = staged.regions});
    }

    struct TextureContent {
        AutoRef<const Blob>    blob;
        ArrayContainer<Region> regions;
    };

    /// @brief Enqueue a texture download operation. The data is copied from the source texture into an internal staging buffer, then
    /// a CPU-side copy is performed to transfer the data into a new Blob. The returned future is always signaled exactly once: with the
    /// downloaded TextureContent on success, or with an empty TextureContent (empty blob, empty regions) if the download failed or was
    /// canceled (e.g. the sealed payload was dropped without submission, or the GpuContext was destroyed before the work completed).
    /// The future never throws.
    virtual std::future<TextureContent> downloadImage(AutoRef<Texture> src, ArrayProxy<const Region> regions) = 0;

    /// Seal the object. Generate payload for all enqueued operations.
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::gpu2
