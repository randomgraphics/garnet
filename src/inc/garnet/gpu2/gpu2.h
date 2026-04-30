// GPU abstraction layer v2: context, resources, shader.
// Include via <garnet/GNgpu2.h>.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/gpu2.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

namespace GN::gpu2 {

// -----------------------------
// Array container and proxy
// -----------------------------
//
// Keep these local to gpu2 to avoid forcing rdg2 public headers into all gpu2 users.
template<typename T>
using ArrayContainer = DynaArray<T, size_t>;

template<typename T>
using ArrayProxy = SafeArrayAccessor<T>;

// -----------------------------
// Base entity
// -----------------------------

/// Base class for all GPU objects. Provides reference counting and runtime type info.
struct RootEntity : public RefCounter, public RuntimeType {
    GN_API GN_REGISTER_RUNTIME_TYPE();

    /// Process-unique ID assigned at construction.
    const uint64_t id;

    /// Human-readable name. Optional; no uniqueness requirement.
    const StrA name;

    virtual ~RootEntity() {
#if GN_BUILD_DEBUG_ENABLED
        static auto * logger = GN::getLogger("GN.gpu2");
        GN_VVTRACE(logger)("Destroying entity, name='{}', type = {}, id={}", name, typeInfo().name, id);
#endif
    }

protected:
    GN_API RootEntity(const RuntimeType::TypeInfo & type, const StrA & name);
};

// -----------------------------
// GpuPayload
// -----------------------------

/// API-neutral base class for a GPU payload. Represents a self-contained unit of GPU work load that is ready to record and submit to the GPU.
struct GpuPayload : public RootEntity {
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

protected:
    using RootEntity::RootEntity;
};

// -----------------------------
// GPU context
// -----------------------------

struct GpuContext : public RootEntity {
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    enum class DebugMode {
        DISABLED,
        ENABLED, ///< Enable API's built-in debug facilities, if available.
    };

    enum class Verbosity {
        SILENCE = 0,
        BRIEF,
        VERBOSE,
    };

    /// Target queue tier for a GPU submission. The backend maps each tier to an actual
    /// hardware queue. Falls back to Graphics if the requested tier is unavailable.
    /// Cross-queue dependencies in waitForGpu automatically use semaphores;
    /// same-queue dependencies use pipeline barriers.
    enum class Queue {
        GRAPHICS,      ///< Default. Graphics pipeline + inline compute. Supports all work types.
        ASYNC_COMPUTE, ///< Dedicated async-compute queue. Runs concurrently with Graphics.
        TRANSFER,      ///< Dedicated transfer/DMA queue. Copy operations only.
    };

    struct CreateParameters {
        /// The graphics API ("auto" = platform default; "vulkan", "d3d12", "metal").
        StrA      api                  = "auto";
        DebugMode debug                = GN_BUILD_DEBUG_ENABLED ? DebugMode::ENABLED : DebugMode::DISABLED;
        Verbosity howToPrintDeviceCaps = Verbosity::BRIEF;
    };

    static GN_API AutoRef<GpuContext> create(const StrA & name, const CreateParameters &);

    struct Caps {
        gfx::img::PixelFormat defaultDepthFormat = gfx::img::PixelFormat::UNKNOWN();
    };

    virtual Caps caps() const = 0;

    /// Vulkan instance handle (VkInstance as intptr_t) for use with Window::getVulkanSurfaceHandle. Returns 0 if not Vulkan.
    virtual intptr_t getVulkanInstanceHandle() const { return 0; }

    // --------------------------------------------------------
    // GPU submission
    // --------------------------------------------------------

    struct SubmitParameters {
        StrA                                name;
        Queue                               queue = Queue::GRAPHICS; ///< Target queue tier for this submission.
        ArrayContainer<AutoRef<GpuPayload>> work;                    ///< Sealed GPU payloads to execute.
        ArrayContainer<AutoRef<GpuPayload>> waitForGpu; ///< GPU-side dependencies. Backend inserts semaphore (cross-queue) or barrier (same-queue) as needed.
        std::function<void()>               onComplete; ///< CPU callback fired (from any thread) when the GPU fence signals.

        SubmitParameters(const StrA & name_): name(name_) {}

        SubmitParameters & appendWork(AutoRef<GpuPayload> w) {
            if (w) work.append(w);
            return *this;
        }
        SubmitParameters & waitFor(AutoRef<GpuPayload> w) {
            if (w) waitForGpu.append(w);
            return *this;
        }
        SubmitParameters & setOnComplete(std::function<void()> f) {
            onComplete = std::move(f);
            return *this;
        }
    };

    /// Submit GPU work. Command buffer allocation, queue selection, fence and semaphore
    /// management are internal. Each GpuPayload in \p work is associated with the resulting
    /// fence/semaphore so it can be waited on by subsequent submit() or present() calls.
    virtual void submit(const SubmitParameters &) = 0;

    /// Check pending GPU submissions and fire onComplete callbacks for any whose fence has signaled.
    /// Called by the L1 graph's wait loop while sleeping on manual-complete nodes.
    virtual void pump() = 0;

protected:
    using RootEntity::RootEntity;
};

// -----------------------------
// Texture
// -----------------------------

struct Texture : RootEntity {
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    /// Descriptor used when creating or declaring the texture (format, dimensions).
    struct Descriptor {
        gfx::img::PixelFormat format  = gfx::img::PixelFormat::UNKNOWN();
        uint32_t              width   = 1;
        uint32_t              height  = 1;
        uint32_t              depth   = 1; ///< 1 = 2D, >1 = 3D
        uint32_t              faces   = 1; ///< 1 = 2D, 6 = cubemap, >1 = array
        uint32_t              levels  = 0; ///< 0 = full mipmap chain, 1 = single level, >1 = mipmap chain with specified number of levels
        uint32_t              samples = 1; ///< 1 = no multisampling

        Descriptor & setFormat(gfx::img::PixelFormat format_) {
            format = format_;
            return *this;
        }

        Descriptor & setDimensions(uint32_t width_, uint32_t height_, uint32_t depth_ = 1) {
            width  = width_;
            height = height_;
            depth  = depth_;
            return *this;
        }

        Descriptor & setFaces(uint32_t faces_) {
            faces = faces_;
            return *this;
        }

        Descriptor & setLevels(uint32_t levels_) {
            levels = levels_;
            return *this;
        }

        Descriptor & setSamples(uint32_t samples_) {
            samples = samples_;
            return *this;
        }
    };

    struct CreateParameters {
        AutoRef<GpuContext> context;
        Descriptor          descriptor;
    };

    struct LoadParameters {
        AutoRef<GpuContext> context;
        StrA                filename;
        bool                forceNewInstance =
            false; ///< If false, return existing texture if the file has been loaded before. If true, always load a new texture instance from file.
    };

    virtual const Descriptor & descriptor() const = 0;

    /// Convenience method to read the texture content into an image. Stalls CPU and GPU. Slow. Do NOT use in performance-sensitive code or render loop.
    virtual gfx::img::Image readback() const = 0;

    /// Create a new instance of empty Texture. Must call reset() at least once for the texture to be valid to use.
    static GN_API AutoRef<Texture> create(const StrA & name, const CreateParameters & params);

    /// Load texture from file. Returns a texture named after the file name.
    static GN_API AutoRef<Texture> load(const LoadParameters & params);

protected:
    using RootEntity::RootEntity;
};

// -----------------------------
// Sampler, buffer
// -----------------------------

struct Sampler : public RootEntity {
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

protected:
    using RootEntity::RootEntity;
};

struct Buffer : public RootEntity {
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    struct CreateParameters {
        AutoRef<GpuContext> context;
        uint64_t            size = 0; ///< Size in bytes. Must be greater than 0.

        /// Set to true, if you need an CPU mappable buffer.
        /// CPU mappable buffer is slower for GPU to access. It is mostly used to store short-lived data that is read or write only once by GPU.
        bool mappable = false;
    };

    struct Mapped : NoCopy {
        Mapped(TransientBuffer & buffer_, void * data_, size_t size_): mBuffer(&buffer_), mData(data_), mSize(size_) {}

        ~Mapped() { unmap(); }

        Mapped(Mapped && other) {
            mBuffer     = std::move(other.mBuffer);
            mData       = other.mData;
            mSize       = other.mSize;
            other.mData = nullptr;
            other.mSize = 0;
            GN_ASSERT(!other.mBuffer);
        }

        Mapped & operator=(Mapped && other) {
            if (this == &other) GN_UNLIKELY return *this;
            mBuffer     = std::move(other.mBuffer);
            mData       = other.mData;
            mSize       = other.mSize;
            other.mData = nullptr;
            other.mSize = 0;
            GN_ASSERT(!other.mBuffer);
            return *this;
        }

        void unmap() {
            mData = nullptr;
            mSize = 0;
            if (mBuffer) {
                mBuffer->unmap(*this);
                mBuffer = {};
            }
        }

        bool empty() const { return mBuffer.empty(); }

        void * data() const { return mData; }

        size_t size() const { return mSize; }

    private:
        AutoRef<Buffer> mBuffer = {};
        void *          mData   = nullptr;
        size_t          mSize   = 0;
    };

    /// Returns the data accessor of a transient buffer that can be directly written to.
    /// Trying to map a un-mappable, or already mapped, buffer returns an empty Mapped object.
    virtual Mapped map() = 0;

    static GN_API AutoRef<Buffer> create(const StrA & name, const CreateParameters & params);

protected:
    virtual void unmap(const Mapped &) = 0;

    using RootEntity::RootEntity;
};

// -----------------------------
// Resource view
// -----------------------------

struct GpuResourceView {
    struct SubresourceIndex {
        uint32_t mip  = 0;
        uint32_t face = 0;
        bool     operator==(const SubresourceIndex & o) const { return mip == o.mip && face == o.face; }
        bool     operator!=(const SubresourceIndex & o) const { return !(*this == o); }
    };

    struct SubresourceExtent {
        uint32_t numMipLevels   = (uint32_t) -1;
        uint32_t numArrayLayers = (uint32_t) -1;
        bool     operator==(const SubresourceExtent & o) const { return numMipLevels == o.numMipLevels && numArrayLayers == o.numArrayLayers; }
        bool     operator!=(const SubresourceExtent & o) const { return !(*this == o); }
    };

    struct SubresourceRange {
        SubresourceIndex  i = {};
        SubresourceExtent e = {};
        bool              operator==(const SubresourceRange & o) const { return i == o.i && e == o.e; }
        bool              operator!=(const SubresourceRange & o) const { return !(*this == o); }
    };

    struct ImageView {
        enum Type { SAMPLED, STORAGE };
        Type                  type   = SAMPLED;
        SubresourceRange      range  = {};
        gfx::img::PixelFormat format = gfx::img::PixelFormat::UNKNOWN();
        bool                  operator==(const ImageView & o) const { return type == o.type && range == o.range && format == o.format; }
        bool                  operator!=(const ImageView & o) const { return !(*this == o); }
    };

    struct BufferView {
        enum Type { UNIFORM, STORAGE };
        Type     type   = UNIFORM;
        uint64_t offset = 0;
        uint64_t size   = 0;
        bool     operator==(const BufferView & o) const { return type == o.type && offset == o.offset && size == o.size; }
        bool     operator!=(const BufferView & o) const { return !(*this == o); }
    };

    AutoRef<RootEntity> resource               = {};
    AutoRef<Sampler>    combinedTextureSampler = {};
    ImageView           imageView              = {};
    BufferView          bufferView             = {};

    bool empty() const { return resource.empty(); }
    bool isTexture() const { return resource && resource->isKindOf<Texture>(); }
    bool isSampler() const { return resource && resource->isKindOf<Sampler>(); }
    bool isBuffer() const { return resource && resource->isKindOf<Buffer>(); }

    auto texture() const -> AutoRef<Texture> {
        auto p = RuntimeType::cast<Texture>(resource.get());
        return p ? GN::referenceTo(p) : AutoRef<Texture>();
    }
    auto sampler() const -> AutoRef<Sampler> {
        auto p = RuntimeType::cast<Sampler>(resource.get());
        return p ? GN::referenceTo(p) : AutoRef<Sampler>();
    }
    auto buffer() const -> AutoRef<Buffer> {
        auto p = RuntimeType::cast<Buffer>(resource.get());
        return p ? GN::referenceTo(p) : AutoRef<Buffer>();
    }
    GpuResourceView & setCombinedTextureSampler(AutoRef<Sampler> sampler_) {
        combinedTextureSampler = std::move(sampler_);
        return *this;
    }
    GpuResourceView & setImageView(ImageView imageView_) {
        imageView = imageView_;
        return *this;
    }
    GpuResourceView & setImageViewType(ImageView::Type t) {
        imageView.type = t;
        return *this;
    }
    GpuResourceView & setSubresourceIndex(SubresourceIndex index) {
        imageView.range.i = index;
        return *this;
    }
    GpuResourceView & setSubresourceExtent(SubresourceExtent e) {
        imageView.range.e = e;
        return *this;
    }
    GpuResourceView & setImageViewFormat(gfx::img::PixelFormat format) {
        imageView.format = format;
        return *this;
    }
    GpuResourceView & setBufferView(BufferView bv) {
        bufferView = bv;
        return *this;
    }
    GpuResourceView & setBufferViewType(BufferView::Type t) {
        bufferView.type = t;
        return *this;
    }
    GpuResourceView & setBufferViewOffset(uint64_t offset) {
        bufferView.offset = offset;
        return *this;
    }
    GpuResourceView & setBufferViewSize(uint64_t s) {
        bufferView.size = s;
        return *this;
    }
    bool operator==(const GpuResourceView & o) const {
        return resource == o.resource && combinedTextureSampler == o.combinedTextureSampler && imageView == o.imageView && bufferView == o.bufferView;
    }
    bool operator!=(const GpuResourceView & o) const { return !(*this == o); }
};

/// List of resource views bound to one shader binding slot.
using GpuResourceSlot = ArrayContainer<GpuResourceView>;

/// One descriptor set: array of binding slots.
using GpuResourceSet = ArrayContainer<GpuResourceSlot>;

/// Full binding table: \c resources[setIndex][bindingIndex][arrayIndex].
using GpuResourceTable = ArrayContainer<GpuResourceSet>;

// -----------------------------
// GpuShader
// -----------------------------

struct GpuShader : public RootEntity {
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    struct CreateParameters {
        AutoRef<GpuContext> context;
        StrA                name;
        const void *        binary;
        size_t              size  = 0;
        const char *        entry = nullptr;
    };

    struct LoadParameters {
        AutoRef<GpuContext> context;
        AutoRef<File>       file;
    };

    static GN_API AutoRef<GpuShader> create(const CreateParameters &);

    static GN_API AutoRef<GpuShader> load(const LoadParameters &);

protected:
    using RootEntity::RootEntity;
};

} // namespace GN::gpu2
