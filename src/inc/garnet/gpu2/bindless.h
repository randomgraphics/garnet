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

    /// Update an existing descriptor slot in-place with a new resource view.
    /// Useful for background streaming (e.g. replacing a 1x1 fallback texture with a loaded 4K texture).
    /// Thread-safe and valid to invoke while GPU commands are in flight.
    virtual bool update(uint32_t slot, const GpuResourceView & view) = 0;

    /// Free a previously allocated descriptor slot for recycling.
    /// Thread-safe.
    virtual void free(uint32_t slot) = 0;

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

} // namespace GN::gpu2::bindless
