// Non-graphics GPU operations: compute dispatches and copy commands.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/cnc.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

namespace GN::gpu2 {

/// Accumulates non-graphics GPU work (compute dispatches and copies), then produces
/// a sealed GpuPayload for submission via GpuContext::submit().
struct GpuCnC : public RootEntity {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    GN_API static AutoRef<GpuCnC> create(const CreateParameters &);

    struct ComputeParameters {
        AutoRef<GpuShader>      cs;
        GpuResourceTable        resources;  ///< shader resources
        ArrayContainer<uint8_t> immediates; ///< immediate constants. Backend copies to GPU when non-empty.
        uint32_t                x = 1;
        uint32_t                y = 1;
        uint32_t                z = 1;
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

    struct Region {
        GpuResourceView::SubresourceIndex imageSubresource;
        Vector3<uint32_t>                 imageOffset;
        Vector3<uint32_t>                 imageExtent;

        /// Byte offset from the start of the source buffer to the first texel of this region.
        uint64_t bufferOffset = 0;

        /// Row length in pixels of the image data in the buffer. 0 = use imageExtent.x.
        uint32_t bufferRowLength = 0;

        /// Image height in pixels of the image data in the buffer. 0 = use imageExtent.y.
        uint32_t bufferHeight = 0;
    };

    /// Upload pixel data from a CPU-visible (mappable) staging buffer into a texture.
    struct BufferToImage {
        AutoRef<Buffer>    src; ///< CPU-visible staging buffer.
        AutoRef<Texture>   dst;
        ArrayProxy<Region> regions;
    };

    virtual void copyBufferToImage(const BufferToImage &) = 0;

    /// Seal the object. Generate payload for all enqueued operations.
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RootEntity::RootEntity;
};

} // namespace GN::gpu2
