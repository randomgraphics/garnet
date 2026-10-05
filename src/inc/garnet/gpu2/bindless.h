// GPU bindless rendering: descriptor heap and bindless raster pass types.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/bindless.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

namespace GN::gpu2::bindless {

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

        /// Maximum total active descriptors across all types (default 65,536).
        /// Each typed binding has this array length; allocation shares one slot pool.
        uint32_t capacity = 65536;

        /// The first target binding slot within the descriptor set (default 0).
        /// Note that each descriptor heap takes up to 6 slots, one material buffer and 5 descriptor buffers (one for each type):
        ///  - material buffer
        ///  - sampled texture
        ///  - storage texture
        ///  - uniform buffer
        ///  - storage buffer
        ///  - sampler
        uint32_t bindingIndex = 0;

        /// Fixed material-buffer capacity in bytes (default 4 MiB). Exhaustion returns INVALID_MATERIAL_TOKEN.
        /// This first implementation never relocates recorded material data.
        uint64_t materialCapacity = 4 * 1024 * 1024;
    };

    /// Descriptor kind. Its array binding is bindingIndex + 1 + type.
    enum DescriptorType {
        SAMPLED_TEXTURE, // bindingIndex + 1
        STORAGE_TEXTURE, // bindingIndex + 2
        UNIFORM_BUFFER,  // bindingIndex + 3
        STORAGE_BUFFER,  // bindingIndex + 4
        SAMPLER,         // bindingIndex + 5
    };

    /// Packed CPU handle. Shaders use slot to index the array at bindingIndex + 1 + type.
    union DescriptorIndex {
        uint32_t u32 = 0;
        bool     operator==(DescriptorIndex other) const { return u32 == other.u32; }
        bool     operator!=(DescriptorIndex other) const { return u32 != other.u32; }
        struct {
            uint32_t slot : 28; ///< index into the descriptor array.
            uint32_t type : 3;  ///< Resource type; shader binding is bindingIndex + 1 + type.
            uint32_t tag  : 1;  ///< must be 1 to be considered as a valid index.
        };
    };

    /// Sentinel value returned when descriptor allocation fails.
    static constexpr DescriptorIndex INVALID_DESCRIPTOR_INDEX = {0};

    /// Create a persistent descriptor heap on the supplied GPU context. Returns an empty ref on failure.
    GN_API static AutoRef<DescriptorHeap> create(const StrA & name, const CreateParameters & cp);

    /// Allocate a persistent descriptor index for the given resource view (texture, storage image, or buffer).
    /// Performs driver descriptor write once internally. Returns INVALID_DESCRIPTOR_INDEX on failure.
    /// Rejects resource-kind/view-type mismatches and combined texture/sampler views.
    /// Thread-safe.
    virtual DescriptorIndex allocate(DescriptorType type, const GpuResourceView & view) = 0;

    /// Update an existing descriptor slot in-place with a new resource view.
    /// Useful for background streaming (e.g. replacing a 1x1 fallback texture with a loaded 4K texture).
    /// CPU-thread-safe; do not update slots referenced by recorded/in-flight work.
    virtual bool update(DescriptorIndex index, const GpuResourceView & view) = 0;

    /// Update a batch of existing descriptor slots in-place with new resource views.
    /// Permissive policy: each valid slot/view pair is updated in-place; invalid or inactive
    /// slots are safely skipped without aborting other updates.
    /// Returns the number of slots successfully updated.
    /// CPU-thread-safe; do not update slots referenced by recorded/in-flight work.
    virtual size_t update(ArrayView<const DescriptorIndex> indices, ArrayView<const GpuResourceView> views) = 0;

    /// Free a previously allocated descriptor slot for recycling.
    /// CPU-thread-safe; free only after recorded/in-flight consumers stop using the slot.
    virtual void free(DescriptorIndex index) = 0;

    /// Free a batch of previously allocated descriptor slots for recycling.
    /// Any invalid or unallocated slots in the array are safely ignored.
    /// CPU-thread-safe; free only after recorded/in-flight consumers stop using the slot.
    virtual void free(ArrayView<const DescriptorIndex> indices) = 0;

    /// Opaque material allocation identifier; zero is invalid.
    using MaterialToken                                   = uint64_t;
    static constexpr MaterialToken INVALID_MATERIAL_TOKEN = 0;

    /// Allocate an opaque, variable-sized chunk; alignment must be a nonzero power of two.
    /// No upload is recorded. Fill materialView(token) through a caller-owned CnC before use.
    /// The fixed-capacity allocator returns INVALID_MATERIAL_TOKEN on invalid input or exhaustion.
    virtual MaterialToken allocateMaterial(uint64_t size, uint64_t alignment = 4) = 0;

    /// Return the allocated buffer range, or an empty view for a stale/foreign token.
    /// Captured views retain the buffer but do not prevent freeMaterial() from reusing its bytes.
    virtual GpuResourceView materialView(MaterialToken token) const = 0;

    /// Immediately recycle a chunk. Invalid/stale tokens are ignored.
    /// Call only after all recorded/in-flight consumers stop using the range.
    virtual void freeMaterial(MaterialToken token) = 0;

    /// Maximum capacity of this descriptor heap.
    virtual uint32_t capacity() const = 0;

    /// Number of currently active allocated descriptor slots.
    virtual uint32_t size() const = 0;

    /// The first binding index within the descriptor set.
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
        /// Preallocates draw storage, geometry backing for eight buffers with eight attributes each per draw,
        /// and numberOfDrawsHint * maxImmediateSize immediate bytes. Recording may exceed this hint.
        size_t numberOfDrawsHint = 100;
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

    /// Record a draw call. Thread-safe when called on thread-local recorder instances.
    virtual void recordDraw(const DrawParameters & params) = 0;

    /// Register a cleanup callback invoked once when the payload completes or is destroyed without submission.
    virtual void addCleanupCallback(std::function<void()> cleanup) = 0;

    /// Generic helper to retain any resource or object until GPU completes execution.
    /// Accepts AutoRef<T>, std::shared_ptr<T>, or any move-constructible object.
    template<typename T>
    void retainResource(T && resource) {
        addCleanupCallback([res = std::forward<T>(resource)]() mutable { (void) res; });
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
        size_t                  opCountHint      = 0;   ///< Expected operation count for preallocation; recording may exceed this hint.
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

    /// Record a compute dispatch using the bindless descriptor set and pass resources.
    virtual void recordCompute(const ComputeParameters & params) = 0;

    using BufferToBuffer = GpuCnC::BufferToBuffer;

    /// Record a buffer copy for execution when the sealed payload is submitted.
    virtual void recordCopyBuffer(const BufferToBuffer &) = 0;

    /// Record a buffer upload. Content is copied during recording;
    /// the GPU transfer executes after the sealed payload is submitted.
    virtual void recordUploadBuffer(AutoRef<Buffer> dst, uint64_t offset, ArrayView<const uint8_t> content) = 0;

    /// Enqueue a buffer download operation. The buffer content is copied from the source buffer into an internal
    /// staging buffer, then a CPU-side copy is performed to transfer the data into a new Blob. The returned future
    /// is signaled with the downloaded Blob on success, or empty Blob if failed or canceled.
    virtual std::future<AutoRef<const Blob>> recordDownloadBuffer(AutoRef<Buffer> src, uint64_t offset = 0, uint64_t size = uint64_t(~0)) = 0;

    using Region          = GpuCnC::Region;
    using ImageCopyRegion = GpuCnC::ImageCopyRegion;
    using ImageToImage    = GpuCnC::ImageToImage;

    /// Snapshot CPU pixel content immediately; image transfer executes after submission.
    /// Region data offsets are relative to content. The caller may release or modify content on return.
    virtual void recordUploadImage(AutoRef<Texture> dst, ArrayView<const uint8_t> content, ArrayView<const Region> regions) = 0;

    /// Upload all faces, mip levels, and depth slices from a CPU image. Content is copied before returning.
    GN_API void recordUploadImage(AutoRef<Texture> dst, const gfx::img::Image & content);

    /// Record copies between distinct textures with matching formats and sample counts; snapshots the regions.
    virtual void recordCopyImage(const ImageToImage &) = 0;

    using TextureContent = GpuCnC::TextureContent;

    /// Enqueue a texture download operation. The data is copied from the source texture into an internal staging buffer,
    /// then transferred into a TextureContent.
    /// Returned regions describe tightly packed block rows in the CPU blob. Input CPU offsets/pitches are ignored.
    virtual std::future<TextureContent> recordDownloadImage(AutoRef<Texture> src, ArrayView<const Region> regions) = 0;

    /// Register a cleanup callback invoked once when the payload completes or is destroyed without submission.
    virtual void addCleanupCallback(std::function<void()> cleanup) = 0;

    /// Generic helper to retain any resource or object until GPU completes execution.
    /// Accepts AutoRef<T>, std::shared_ptr<T>, or any move-constructible object.
    template<typename T>
    void retainResource(T && resource) {
        addCleanupCallback([res = std::forward<T>(resource)]() mutable { (void) res; });
    }

    /// Seal recorded work into an opaque, self-contained GpuPayload ready for GpuContext::submit().
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::gpu2::bindless
