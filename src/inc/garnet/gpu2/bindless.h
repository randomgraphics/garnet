// GPU bindless rendering: descriptor heap and bindless raster pass types.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/bindless.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

namespace GN::gpu2::bindless {

/// Sentinel value returned when descriptor allocation fails.
static constexpr uint32_t INVALID_DESCRIPTOR_INDEX = ~0u;

/// Persistent global descriptor heap managing unbounded resource arrays for bindless shaders.
///
/// Under the hood, this allocates a long-lived descriptor set with VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
/// and VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT. It maintains a thread-safe slot allocator with free-list recycling,
/// allowing background resource streaming threads to allocate, update, and free descriptor indices concurrently
/// with the render loop.
///
/// Descriptors are bound once per pass, completely bypassing per-draw descriptor pool allocation and updating.
class DescriptorHeap : public RCRT64 {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
        uint32_t            capacity     = 65536; ///< Maximum number of descriptors in the heap (default 65,536).
        uint32_t            bindingIndex = 0;     ///< Target binding slot within the descriptor set (default 0).
    };

    /// Create a persistent descriptor heap on the supplied GPU context. Returns an empty ref on failure.
    GN_API static AutoRef<DescriptorHeap> create(const StrA & name, const CreateParameters & cp);

    /// Allocate a persistent descriptor index for the given resource view (texture, storage image, or buffer).
    /// Performs driver descriptor write once internally. Returns INVALID_DESCRIPTOR_INDEX on failure.
    /// Thread-safe.
    virtual uint32_t allocate(const GpuResourceView & view) = 0;

    /// Allocate a contiguous batch of descriptor indices for the given views.
    /// All-or-nothing: if any view allocation fails or capacity is insufficient,
    /// no slots are allocated, outIndices is unchanged, and false is returned.
    /// Returns true on success, filling outIndices with the allocated indices.
    /// Thread-safe.
    virtual bool allocate(ArrayView<const GpuResourceView> views, ArrayView<uint32_t> outIndices) = 0;

    /// Update an existing descriptor slot in-place with a new resource view.
    /// Useful for background streaming (e.g. replacing a 1x1 fallback texture with a loaded 4K texture).
    /// Thread-safe and valid to invoke while GPU commands are in flight.
    virtual bool update(uint32_t slot, const GpuResourceView & view) = 0;

    /// Update a batch of existing descriptor slots in-place with new resource views.
    /// Permissive policy: each valid slot/view pair is updated in-place; invalid or inactive
    /// slots are safely skipped without aborting other updates.
    /// Returns the number of slots successfully updated.
    /// Thread-safe and valid to invoke while GPU commands are in flight.
    virtual uint32_t update(ArrayView<const uint32_t> slots, ArrayView<const GpuResourceView> views) = 0;

    /// Free a previously allocated descriptor slot for recycling.
    /// Thread-safe.
    virtual void free(uint32_t slot) = 0;

    /// Free a batch of previously allocated descriptor slots for recycling.
    /// Any invalid or unallocated slots in the array are safely ignored.
    /// Thread-safe.
    virtual void free(ArrayView<const uint32_t> slots) = 0;

    /// Maximum capacity of this descriptor heap.
    virtual uint32_t capacity() const = 0;

    /// Number of currently active allocated descriptor slots.
    virtual uint32_t size() const = 0;

    /// Target binding slot within the descriptor set.
    virtual uint32_t bindingIndex() const = 0;

    /// Owning GPU context.
    virtual AutoRef<GpuContext> gpu() const = 0;

protected:
    using RCRT64::RCRT64;
};

/// High-performance bindless raster pass recorder.
///
/// Bypasses per-draw GpuResourceTable descriptor set compilation and per-draw table scanning.
/// All draws execute against a unified, cached pipeline layout with the bindless descriptor set
/// bound once at the start of the pass.
///
/// This class is not thread-safe. All calls on one instance must be single-threaded or externally
/// serialized; use separate instances for parallel recording.
class Raster : public RCRT64 {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Plain POD configuration. The caller describes the intended pass layout;
    /// gpu2 automatically manages, hashes, and caches the native pipeline layout.
    struct CreateParameters {
        AutoRef<GpuContext>     gpu;
        const RasterTarget *    target = nullptr;       ///< Borrowed for creation only; Raster stores its own copy.
        AutoRef<DescriptorHeap> heap;                   ///< Persistent global descriptor heap.
        uint32_t                heapSetIndex = 0;       ///< Descriptor set index for the bindless heap (default 0).
        GpuResourceTable        passResources;          ///< Optional pass-wide resources (e.g. Set 0 Camera UBO).
        uint32_t                maxImmediateSize = 128; ///< Maximum immediate data size in bytes (default 128).
    };

    /// Create a new bindless raster recorder. Performs a fast conflict check between
    /// passResources and heapSetIndex, returning an empty ref if a collision is detected.
    GN_API static AutoRef<Raster> create(const StrA & name, const CreateParameters & cp);

    struct DrawParameters {
        AutoRef<GpuShader>       vs = {}, hs = {}, ds = {}, gs = {}, ps = {};
        RasterState              states = {};     ///< Transient state overrides for this draw.
        const RasterGeometry &   geometry;        ///< Vertex and index buffer bindings.
        ArrayView<const uint8_t> immediates = {}; ///< Inline uniform data (root constants / push constants).
    };

    /// Optional hint to pre-allocate storage for expected draws and immediate data to eliminate vector reallocations.
    virtual void reserve(size_t drawCount, size_t immediateBytes = 0) {
        (void) drawCount;
        (void) immediateBytes;
    }

    /// Record a draw call. Thread-safe when called on thread-local recorder instances.
    virtual void recordDraw(const DrawParameters & params) = 0;

    /// Retain an arbitrary cleanup callable that will be invoked when the payload finishes execution.
    virtual void retainCleanup(std::function<void()> cleanup) = 0;

    /// Generic helper to retain any resource or object until GPU completes execution.
    /// Accepts AutoRef<T>, std::shared_ptr<T>, or any move-constructible object.
    template<typename T>
    void retainResource(T && resource) {
        retainCleanup([res = std::forward<T>(resource)]() mutable { (void) res; });
    }

    /// Seal recorded work into an opaque, self-contained GpuPayload ready for GpuContext::submit().
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RCRT64::RCRT64;
};

/// High-performance bindless copy and compute (CnC) recorder.
///
/// Bypasses per-dispatch GpuResourceTable descriptor set compilation and per-dispatch table scanning.
/// All compute dispatches execute against a unified, cached pipeline layout with the bindless descriptor set
/// bound once at the start of the pass.
///
/// Also provides asynchronous memory transfer and copy operations (buffer-to-buffer, buffer-to-image,
/// staging uploads, and asynchronous downloads with std::future).
///
/// This class is not thread-safe. All calls on one instance must be single-threaded or externally
/// serialized; use separate instances for parallel recording.
class CnC : public RCRT64 {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Plain POD configuration. The caller describes the intended pass layout;
    /// gpu2 automatically manages, hashes, and caches the native pipeline layout.
    struct CreateParameters {
        AutoRef<GpuContext>     gpu;
        AutoRef<DescriptorHeap> heap;                   ///< Persistent global descriptor heap.
        uint32_t                heapSetIndex = 0;       ///< Descriptor set index for the bindless heap (default 0).
        GpuResourceTable        passResources;          ///< Optional pass-wide resources (e.g. Set 1 storage buffers/UBOs).
        uint32_t                maxImmediateSize = 128; ///< Maximum immediate data size in bytes (default 128).
    };

    /// Create a new bindless CnC recorder. Performs a fast conflict check between
    /// passResources and heapSetIndex, returning an empty ref if a collision is detected.
    GN_API static AutoRef<CnC> create(const StrA & name, const CreateParameters & cp);

    struct ComputeParameters {
        AutoRef<GpuShader>       cs;              ///< Compute shader module.
        uint32_t                 x          = 1;  ///< Number of local workgroups to dispatch in X dimension.
        uint32_t                 y          = 1;  ///< Number of local workgroups to dispatch in Y dimension.
        uint32_t                 z          = 1;  ///< Number of local workgroups to dispatch in Z dimension.
        ArrayView<const uint8_t> immediates = {}; ///< Inline uniform data (push constants).
    };

    /// Optional hint to pre-allocate storage for expected operations and immediate data to eliminate vector reallocations.
    virtual void reserve(size_t opCount, size_t immediateBytes = 0) {
        (void) opCount;
        (void) immediateBytes;
    }

    /// Record a compute dispatch using the bindless descriptor set and pass resources.
    virtual void recordCompute(const ComputeParameters & params) = 0;

    using BufferToBuffer = GpuCnC::BufferToBuffer;

    /// Record a buffer copy for execution when the sealed payload is submitted.
    virtual void recordCopyBufferToBuffer(const BufferToBuffer &) = 0;

    /// Record a buffer upload. Content is copied into internal staging storage during recording;
    /// the GPU transfer executes after the sealed payload is submitted.
    virtual void recordUploadBuffer(AutoRef<Buffer> dst, uint64_t offset, ArrayView<const uint8_t> content) = 0;

    /// Enqueue a buffer download operation. The buffer content is copied from the source buffer into an internal
    /// staging buffer, then a CPU-side copy is performed to transfer the data into a new Blob. The returned future
    /// is signaled with the downloaded Blob on success, or empty Blob if failed or canceled.
    virtual std::future<AutoRef<const Blob>> recordDownloadBuffer(AutoRef<Buffer> src, uint64_t offset = 0, uint64_t size = uint64_t(~0)) = 0;

    using Region        = GpuCnC::Region;
    using BufferToImage = GpuCnC::BufferToImage;

    /// Record a buffer-to-image copy, retaining resources and copying the region descriptions.
    virtual void recordCopyBufferToImage(const BufferToImage &) = 0;

    /// Convenience: upload all regions from a StagedTexture into dst without any conversion.
    void recordCopyBufferToImage(const Buffer::StagedTexture & staged, AutoRef<Texture> dst) {
        recordCopyBufferToImage({.src = staged.staging, .dst = std::move(dst), .regions = staged.regions});
    }

    using TextureContent = GpuCnC::TextureContent;

    /// Enqueue a texture download operation. The data is copied from the source texture into an internal staging buffer,
    /// then transferred into a TextureContent.
    virtual std::future<TextureContent> recordDownloadImage(AutoRef<Texture> src, ArrayView<const Region> regions) = 0;

    /// Retain an arbitrary cleanup callable that will be invoked when the payload finishes execution.
    virtual void retainCleanup(std::function<void()> cleanup) = 0;

    /// Generic helper to retain any resource or object until GPU completes execution.
    /// Accepts AutoRef<T>, std::shared_ptr<T>, or any move-constructible object.
    template<typename T>
    void retainResource(T && resource) {
        retainCleanup([res = std::forward<T>(resource)]() mutable { (void) res; });
    }

    /// Seal recorded work into an opaque, self-contained GpuPayload ready for GpuContext::submit().
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::gpu2::bindless
