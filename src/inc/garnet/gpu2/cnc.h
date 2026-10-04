// Non-graphics GPU operations: compute dispatches and copy commands.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/cnc.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

#include <future>

namespace GN::gpu2 {

/// Accumulates non-graphics GPU work (compute dispatches and copies), then produces
/// a sealed GpuPayload for submission via GpuContext::submit().
/// record*() methods prepare and retain commands; GPU execution starts only after submission.
/// Upload methods snapshot CPU content before returning.
/// One-shot recorder: seal() finalizes the payload; no further recording or sealing
/// is allowed afterward. This class is not thread-safe. All calls on one instance
/// must be single-threaded or externally serialized; use separate instances for parallel recording.
/// Transfer payloads restore touched textures to shader-readable state and buffers to read-ready state before GPU completion.
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

    /// Record a buffer upload. Content is copied during recording;
    /// the GPU transfer executes after the sealed payload is submitted.
    virtual void recordUploadBuffer(AutoRef<Buffer> dst, uint64_t offset, ArrayView<const uint8_t> content) = 0;

    /// @brief Enqueue a buffer download operation. The buffer content is copied from the source buffer into an internal staging buffer, then
    /// a CPU-side copy is performed to transfer the data into a new Blob. The returned future is always signaled exactly once: with the
    /// downloaded Blob on success, or with an empty Blob if the download failed or was canceled (e.g. the sealed payload was dropped
    /// without submission, or the GpuContext was destroyed before the work completed). The future never throws.
    virtual std::future<AutoRef<const Blob>> recordDownloadBuffer(AutoRef<Buffer> src, uint64_t offset = 0, uint64_t size = uint64_t(~0)) = 0;

    /// Describes CPU pixel layout and the destination image subresource for uploads/downloads.
    struct Region {
        uint32_t          mip         = 0;         ///< Image mip level.
        uint32_t          face        = 0;         ///< Image array layer or cube face.
        Vector3<uint32_t> imageOffset = {0, 0, 0}; ///< Offset in texels, including compressed formats; must align to compression blocks.
        Vector3<uint32_t> imageExtent = {0, 0, 0}; ///< Dimensions in texels; compressed extents align to blocks unless reaching the mip edge.
        uint64_t          dataOffset  = 0;         ///< Byte offset in the CPU content.
        /// Byte stride between pixel-block rows, matching rapid-image PlaneDesc::pitch. An uncompressed block is one pixel.
        /// May include arbitrary trailing byte padding, even for compressed rows. 0 = tightly packed.
        uint64_t rowPitchBytes = 0;
        /// Byte stride between depth slices, matching rapid-image PlaneDesc::slice. 0 = effective row pitch times block-row count.
        uint64_t slicePitchBytes = 0;
    };

    /// Snapshot CPU pixel content immediately; image transfer executes after submission.
    /// Region data offsets are relative to content. The caller may release or modify content on return.
    virtual void recordUploadImage(AutoRef<Texture> dst, ArrayView<const uint8_t> content, ArrayView<const Region> regions) = 0;

    /// Upload all faces, mip levels, and depth slices from a CPU image. Content is copied before returning.
    GN_API void recordUploadImage(AutoRef<Texture> dst, const gfx::img::Image & content);

    /// Region for an image-to-image copy. Source and destination must have matching formats and sample counts.
    struct ImageCopyRegion {
        uint32_t          srcMip = 0, srcFace = 0;
        uint32_t          dstMip = 0, dstFace = 0;
        Vector3<uint32_t> srcOffset = {0, 0, 0};
        Vector3<uint32_t> dstOffset = {0, 0, 0};
        Vector3<uint32_t> extent    = {0, 0, 0};
    };

    /// Image copy resources and regions. Copies retain both textures and snapshot region descriptions.
    struct ImageToImage {
        AutoRef<Texture>                 src;
        AutoRef<Texture>                 dst;
        ArrayView<const ImageCopyRegion> regions;
    };

    /// Record image-to-image copies. Source and destination must be distinct textures.
    virtual void recordCopyImage(const ImageToImage &) = 0;

    struct TextureContent {
        AutoRef<const Blob> blob;
        DynaArray<Region>   regions;
    };

    /// @brief Enqueue a texture download operation. The data is copied from the source texture into an internal staging buffer, then
    /// a CPU-side copy is performed to transfer the data into a new Blob. The returned future is always signaled exactly once: with the
    /// downloaded TextureContent on success, or with an empty TextureContent (empty blob, empty regions) if the download failed or was
    /// canceled (e.g. the sealed payload was dropped without submission, or the GpuContext was destroyed before the work completed).
    /// The future never throws.
    /// Returned regions describe tightly packed block rows in the CPU blob. Input CPU offsets/pitches are ignored.
    virtual std::future<TextureContent> recordDownloadImage(AutoRef<Texture> src, ArrayView<const Region> regions) = 0;

    /// Seal the object. Generate payload for all enqueued operations.
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::gpu2
