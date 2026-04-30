#pragma once

#include <garnet/gfx/image.h>
#include <unordered_map>
#include <variant>

namespace GN::rdg {

// =============================================================================
// GPU artifact types
// =============================================================================

/// GpuContext represents a GPU context (wrapper of D3D/Vulkan context).
struct GpuContext : public Artifact {
    GN_API GN_REGISTER_RUNTIME_TYPE(Artifact);

    struct CreateParameters {
        enum DebugMode {
            DISABLED,
            ENABLED, ///< Enable API's built-in debug facilities, if available.
        };

        enum Verbosity {
            SILENCE = 0,
            BRIEF,
            VERBOSE,
        };

        /// The graphics API ("auto" = platform default; "vulkan", "d3d12", "metal").
        StrA api = "auto";

        /// Set the debug mode.
        DebugMode debug = GN_BUILD_DEBUG_ENABLED ? DebugMode::ENABLED : DebugMode::DISABLED;

        /// Set the device capabilities print verbosity.
        Verbosity howToPrintDeviceCaps = Verbosity::BRIEF;
    };

    struct Caps {
        gfx::img::PixelFormat defaultDepthFormat = gfx::img::PixelFormat::UNKNOWN();
    };

    /// Create a new instance of GpuContext.
    static GN_API AutoRef<GpuContext> create(const StrA & name, const CreateParameters & params);

    /// Query commonly used GPU device capabilities.
    virtual Caps caps() const = 0;

    /// Vulkan instance handle (VkInstance cast to intptr_t) for use with Window::getVulkanSurfaceHandle.
    /// Returns 0 if this context is not Vulkan.
    virtual intptr_t getVulkanInstanceHandle() const { return 0; }

protected:
    using Artifact::Artifact;
};

/// Base class for all GPU resources.
struct GpuResource : public Artifact {
    GN_API GN_REGISTER_RUNTIME_TYPE(Artifact);

    virtual GpuContext & gpu() const = 0;

protected:
    using Artifact::Artifact;
};

/// Backbuffer represents the swapchain that can be drawn to by GPU and presented to screen.
/// - It is in renderable state if and only if it is prepared but not presented. See
///   PrepareBackbuffer and PresentBackbuffer actions for more details.
/// - A newly created backbuffer is always in non-renderable state.
struct Backbuffer : public GpuResource {
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuResource);

    /// Surface/window handle and size for backbuffer. Decoupled from window system; caller passes native handles.
    /// If handle is 0, create a headless backbuffer. width/height must be positive.
    /// If format is UNKNOWN(), automatically select the best back buffer format.
    struct Descriptor {
        /// Native window/surface/layer handle. API- and platform-dependent:
        /// - D3D12: HWND.
        /// - Vulkan: VkSurfaceKHR
        /// - Metal: CAMetalLayer*.
        intptr_t              window = 0;
        gfx::img::PixelFormat format = gfx::img::PixelFormat::UNKNOWN();
        uint32_t              width  = 1280;
        uint32_t              height = 720;

        Descriptor & setWindow(intptr_t handle_) {
            window = handle_;
            return *this;
        }

        Descriptor & setFormat(gfx::img::PixelFormat format_) {
            format = format_;
            return *this;
        }

        Descriptor & setDimensions(uint32_t width_, uint32_t height_) {
            width  = width_;
            height = height_;
            return *this;
        }
    };

    struct CreateParameters {
        AutoRef<GpuContext> context;
        Descriptor          descriptor;
    };

    /// Return the current backbuffer descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Read the content into an image.
    virtual gfx::img::Image readback() const = 0;

    /// Create a new instance of Backbuffer.
    static GN_API AutoRef<Backbuffer> create(const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

/// Texture represents a 2D/3D/cube texture with optional mipmap and array layers.
struct Texture : public GpuResource {
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuResource);

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
    };

    /// Return the current texture descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Read the texture content into an image.
    virtual gfx::img::Image readback() const = 0;

    /// Create a new instance of empty Texture The texture is not bound to any GPU resource yet. Must call reset() at least once for the texture to be valid to
    /// use.
    static GN_API AutoRef<Texture> create(const StrA & name, const CreateParameters & params);

    /// Load texture from file. Returns a texture artifact named after the file name.
    /// If the file has been loaded before, return the existing artifact.
    static GN_API AutoRef<Texture> load(const LoadParameters & params);

protected:
    using GpuResource::GpuResource;
};

/// Sampler represents GPU sampler state (filtering, addressing, LOD, anisotropy).
struct Sampler : public GpuResource {
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuResource);

    // enum class Filter { POINT, LINEAR, ANISOTROPIC };
    // enum class AddressMode { REPEAT, MIRROR_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER, MIRROR_CLAMP_TO_EDGE };

    // /// Descriptor for sampler creation.
    // struct Descriptor {
    //     Filter      filterMin     = Filter::LINEAR;
    //     Filter      filterMag     = Filter::LINEAR;
    //     Filter      filterMip     = Filter::LINEAR;
    //     AddressMode addressU      = AddressMode::REPEAT;
    //     AddressMode addressV      = AddressMode::REPEAT;
    //     AddressMode addressW      = AddressMode::REPEAT;
    //     float       mipLodBias    = 0.f;
    //     uint32_t    maxAnisotropy = 1;
    //     float       minLod        = 0.f;
    //     float       maxLod        = 0.f; ///< 0 often means "all mips"
    // };

    // struct CreateParameters {
    //     AutoRef<GpuContext> context;
    //     Descriptor          descriptor;
    // };

    // /// Return the current sampler descriptor.
    // virtual const Descriptor & descriptor() const = 0;

    // /// Create a new instance of Sampler. Must call reset() at least once for the sampler to be valid to use.
    // static GN_API AutoRef<Sampler> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

/// API-agnostic buffer usage flags. Combine multiple values with bitwise OR.
enum class BufferUsageBits : uint32_t {
    VERTEX  = 1 << 0, ///< Vertex buffer (geometry data).
    INDEX   = 1 << 1, ///< Index buffer.
    UNIFORM = 1 << 2, ///< Uniform / constant buffer (read-only by shaders).
    STORAGE = 1 << 3, ///< Storage buffer (read/write by shaders).
};

typedef BitFlags<BufferUsageBits> BufferUsageFlags;

inline BufferUsageFlags operator|(BufferUsageBits a, BufferUsageBits b) { return BufferUsageFlags(static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); }

/// Base class that represents a GPU buffer
struct Buffer : public GpuResource {
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuResource);

    /// Upload CPU data into the buffer. Returns false if data is null, size exceeds capacity, or backend fails.
    /// @note this is just a convenience method for quick prototyping only. It stalls CPU & GPU.
    /// Use TransientBuffer with GpuCopy for better performance.
    virtual bool setContent(const void * data, uint64_t size) = 0;

    /// Read the buffer content back into a blob.
    /// @note this is just a convenience method for quick prototyping only. It stalls CPU & GPU.
    /// Use TransientBuffer with GpuCopy for better performance.
    virtual AutoRef<Blob> readback() const = 0;

protected:
    using GpuResource::GpuResource;
};

/// Buffer that stays alive across submission boundaries. Usually used for persistent/static data.
struct PersistentBuffer : public Buffer {
    GN_API GN_REGISTER_RUNTIME_TYPE(Buffer);

    struct CreateParameters {
        AutoRef<GpuContext> context;
        uint64_t            size = 0; ///< Size of the buffer in bytes.
        /// Default: vertex + index + uniform.
        BufferUsageFlags usage = BufferUsageBits::VERTEX | BufferUsageBits::INDEX | BufferUsageBits::UNIFORM;
    };

    static GN_API AutoRef<PersistentBuffer> create(const StrA & name, const CreateParameters & params);

protected:
    using Buffer::Buffer;
};

/// Transient host-visible buffer is the most efficient way for one-time GPU read data, either
/// to upload/copy data to other more permanent GPU buffer or to be used directly as a shader
/// resource for one-time rendering.
///
/// Allocate via TransientArena::allocate(). Use it's map() method to get a CPU accessible buffer
/// to read from or write to the buffer. Once you are done, release the accessor by calling unmap(),
/// or simplly let it go out of scope. Then the buffer is ready to be given to any GPU action for
/// copying or drawing.
///
/// Just as what its name implies, it is one-shot and per-submission only buffer object.
/// It is utterly important to NOT keep the reference to the buffer object or the Mapped accessor
/// for longer than necessary, especially NOT after the buffer is submitted to GPU.
///
/// Release your own reference to the buffer object, once it is passed to a GPU action. That allows it to be
/// properly recycled by the arena after GPU has finished consuming it.
struct TransientBuffer : public Buffer {
    GN_API GN_REGISTER_RUNTIME_TYPE(Buffer);

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

        void * data() const { return mData; }

        size_t size() const { return mSize; }

    private:
        AutoRef<TransientBuffer> mBuffer = {};
        void *                   mData   = nullptr;
        size_t                   mSize   = 0;
    };

    /// Returns the data accessor of the buffer that can be directly written to.
    virtual Mapped map() = 0;

protected:
    virtual void unmap(const Mapped &) = 0;

    using Buffer::Buffer;
};

/// This is the pool/allocator of transient buffers.
/// It also manages the lifetime of the backing buffers to ensure used space
/// are properly recycled when GPU is done with them.
struct TransientArena : public GpuResource {
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuResource);

    struct CreateParameters {
        AutoRef<GpuContext> context;

        /// Suggested size of the arena in bytes. The actual arena size is completely at the discretion of the backend,
        /// it can be larger or smaller than the suggested size. Leave it at 0 if you don't have a preference.
        uint64_t suggestedArenaSize = 0;

        /// Default usage: nothing.
        BufferUsageFlags usage = 0;
    };

    /// Allocate a transient buffer from the arena.
    virtual AutoRef<TransientBuffer> allocate(uint64_t size, const char * name) = 0;

    static GN_API AutoRef<TransientArena> create(const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

/// Represents a view to single GPU resource: texture, backbuffer, buffer, sampler.
struct GpuResourceView {
    struct SubresourceIndex {
        uint32_t mip  = 0; ///< index into mipmap chain
        uint32_t face = 0; ///< index into array of faces

        bool operator==(const SubresourceIndex & other) const { return mip == other.mip && face == other.face; }
        bool operator!=(const SubresourceIndex & other) const { return !operator==(other); }
    };

    struct SubresourceExtent {
        uint32_t numMipLevels   = (uint32_t) -1; ///< -1 means all mip levels
        uint32_t numArrayLayers = (uint32_t) -1; ///< -1 means all array layers

        bool operator==(const SubresourceExtent & other) const { return numMipLevels == other.numMipLevels && numArrayLayers == other.numArrayLayers; }
        bool operator!=(const SubresourceExtent & other) const { return !operator==(other); }
    };

    /// Combined subresource index and extent for image views (classes/functions that need both use this).
    struct SubresourceRange {
        SubresourceIndex  i = {};
        SubresourceExtent e = {};

        bool operator==(const SubresourceRange & other) const { return i == other.i && e == other.e; }
        bool operator!=(const SubresourceRange & other) const { return !operator==(other); }
    };

    // TODO: compress type and range into 32 bits. So the whole view is 64 bits
    struct ImageView {
        enum Type {
            SAMPLED,
            STORAGE,
        };

        Type             type  = Type::SAMPLED; ///< sampled or storage image
        SubresourceRange range = {};            ///< subresource index (i) and extent (e)

        /// pixel format of the image view. Unknown means use the intrinsic format of the texture or backbuffer.
        gfx::img::PixelFormat format = gfx::img::PixelFormat::UNKNOWN();

        bool operator==(const ImageView & other) const { return type == other.type && range == other.range && format == other.format; }
        bool operator!=(const ImageView & other) const { return !operator==(other); }
    };

    // TODO: compress whole buffer view into 128 bits.
    struct BufferView {
        enum Type {
            UNIFORM,
            STORAGE,
        };

        /// uniform or storage buffer
        Type type = Type::UNIFORM; ///< uniform or storage buffer

        /// offset in bytes from beginning of the buffer to the first element.
        uint64_t offset = 0;

        /// size of the data in the buffer in bytes, starting from offset.
        uint64_t size = 0;

        bool operator==(const BufferView & other) const { return type == other.type && offset == other.offset && size == other.size; }
        bool operator!=(const BufferView & other) const { return !operator==(other); }
    };

    /// The main artifact that is being viewed.
    AutoRef<Artifact> artifact = {};

    /// Optional sampler. Only used when the main artifact is an sampled texture
    AutoRef<Sampler> combinedTextureSampler = {};

    /// Image view parameters. Only used when the main artifact is an image (texture or backbuffer).
    ImageView imageView = {};

    /// Buffer view parameters. Only used when the main artifact is a buffer.
    BufferView bufferView = {};

    bool empty() const { return artifact.empty(); }
    bool isBackbuffer() const { return artifact && artifact->isKindOf<Backbuffer>(); }
    bool isTexture() const { return artifact && artifact->isKindOf<Texture>(); }
    bool isImage() const { return isTexture() || isBackbuffer(); }
    bool isSampler() const { return artifact && artifact->isKindOf<Sampler>(); }
    bool isTransientBuffer() const { return artifact && artifact->isKindOf<TransientBuffer>(); }
    bool isPersistentBuffer() const { return artifact && artifact->isKindOf<PersistentBuffer>(); }
    bool isBuffer() const { return artifact && artifact->isKindOf<Buffer>(); }

    auto backbuffer() const -> AutoRef<Backbuffer> { return RuntimeType::cast<Backbuffer>(artifact); }
    auto texture() const -> AutoRef<Texture> { return RuntimeType::cast<Texture>(artifact); }
    auto sampler() const -> AutoRef<Sampler> { return RuntimeType::cast<Sampler>(artifact); }
    auto transientBuffer() const -> AutoRef<TransientBuffer> { return RuntimeType::cast<TransientBuffer>(artifact); }
    auto persistentBuffer() const -> AutoRef<PersistentBuffer> { return RuntimeType::cast<PersistentBuffer>(artifact); }
    auto buffer() const -> AutoRef<Buffer> { return RuntimeType::cast<Buffer>(artifact); }

    GpuResourceView & setArtifact(AutoRef<Artifact> artifact_) {
        artifact = std::move(artifact_);
        return *this;
    }

    GpuResourceView & setCombinedTextureSampler(AutoRef<Sampler> sampler_) {
        combinedTextureSampler = std::move(sampler_);
        return *this;
    }

    GpuResourceView & setImageView(ImageView imageView_) {
        imageView = imageView_;
        return *this;
    }

    GpuResourceView & setImageViewType(ImageView::Type type) {
        imageView.type = type;
        return *this;
    }

    GpuResourceView & setSubresourceIndex(SubresourceIndex index) {
        imageView.range.i = index;
        return *this;
    }

    GpuResourceView & setSubresourceExtent(SubresourceExtent extent) {
        imageView.range.e = extent;
        return *this;
    }

    GpuResourceView & setImageViewFormat(gfx::img::PixelFormat format) {
        imageView.format = format;
        return *this;
    }

    GpuResourceView & setBufferView(BufferView bufferView_) {
        bufferView = bufferView_;
        return *this;
    }

    GpuResourceView & setBufferViewType(BufferView::Type type) {
        bufferView.type = type;
        return *this;
    }

    GpuResourceView & setBufferViewOffset(uint64_t offset) {
        bufferView.offset = offset;
        return *this;
    }

    GpuResourceView & setBufferViewSize(uint64_t size) {
        bufferView.size = size;
        return *this;
    }

    bool operator==(const GpuResourceView & other) const {
        if (artifact != other.artifact) return false;
        if (isImage()) {
            if (imageView != other.imageView) return false;
            if (imageView.type == ImageView::Type::SAMPLED && combinedTextureSampler != other.combinedTextureSampler) return false;
        } else if (isBuffer()) {
            if (bufferView != other.bufferView) return false;
        }
        return true;
    }
    bool operator!=(const GpuResourceView & other) const { return !operator==(other); }
};

/// Render target represents a set of color and depth/stencil targets that GPU draws to.
struct RenderTarget : public Artifact {
    GN_API GN_REGISTER_RUNTIME_TYPE(Artifact);

    struct BlendState {
        enum Arg {
            ZERO = 0,
            ONE,
            SRC_COLOR,
            INV_SRC_COLOR,
            SRC_ALPHA,
            INV_SRC_ALPHA,
            DEST_ALPHA,
            INV_DEST_ALPHA,
            DEST_COLOR,
            INV_DEST_COLOR,
            BLEND_FACTOR,
            INV_BLEND_FACTOR,
        };

        enum Op {
            ADD,
            SUB,
            REV_SUB,
            MIN,
            MAX,
        };

        // Default blend mode is disabled.
        Op       colorOp  = Op::ADD;
        Arg      colorSrc = Arg::ONE;
        Arg      colorDst = Arg::ZERO;
        Op       alphaOp  = Op::ADD;
        Arg      alphaSrc = Arg::ONE;
        Arg      alphaDst = Arg::ZERO;
        Vector4f factors  = {0.0f, 0.0f, 0.0f, 0.0f};

        constexpr bool enabled() const {
            return colorOp != Op::ADD || colorSrc != Arg::ONE || colorDst != Arg::ZERO || alphaOp != Op::ADD || alphaSrc != Arg::ONE || alphaDst != Arg::ZERO;
        }
        constexpr bool operator==(const BlendState & other) const {
            return colorOp == other.colorOp && colorSrc == other.colorSrc && colorDst == other.colorDst && alphaOp == other.alphaOp &&
                   alphaSrc == other.alphaSrc && alphaDst == other.alphaDst;
        }
        constexpr bool operator!=(const BlendState & other) const { return !operator==(other); }
    };

    enum class Compare {
        NEVER = 0,     // no read, no write
        LESS,          // read and write
        LESS_EQUAL,    // read and write
        EQUAL,         // read and write
        GREATER_EQUAL, // read and write
        GREATER,       // read and write
        NOT_EQUAL,     // read and write
        ALWAYS,        // write only
    };

    struct DepthState {
        // default state equals to depth disabled.
        Compare func  = Compare::ALWAYS;
        bool    write = false;

        constexpr bool testEnabled() const { return Compare::NEVER != func && Compare::ALWAYS != func; }
        constexpr bool writeEnabled() const { return Compare::NEVER != func && write; }
        constexpr bool operator==(const DepthState & other) const { return func == other.func && write == other.write; }
        constexpr bool operator!=(const DepthState & other) const { return !operator==(other); }
    };

    struct StencilState {
        enum Op {
            KEEP = 0, // no read, no write
            ZERO,     // write only
            REPLACE,  // write only
            INC_SAT,  // read and write
            DEC_SAT,  // read and write
            INVERT,   // read and write
            INC,      // read and write
            DEC,      // read and write
        };

        // default to an state that stencil is effectively disabled.
        Compare compare   = Compare::ALWAYS; ///< stencil comparison function
        Op      pass      = KEEP;            ///< stencil operation on pass
        Op      fail      = KEEP;            ///< stencil operation on fail
        Op      zFail     = KEEP;            ///< stencil operation on depth fail
        uint8_t ref       = 0;               ///< stencil reference value
        uint8_t readMask  = 0xFF;            ///< stencil read mask
        uint8_t writeMask = 0xFF;            ///< stencil write mask

        constexpr bool enabled() const {
            bool read  = (0 != readMask) && (Compare::NEVER != compare) && (Compare::ALWAYS != compare);
            bool write = (0 != writeMask) && ((Op::KEEP != pass) || (Op::KEEP != fail) || (Op::KEEP != zFail));
            return read || write;
        }
        constexpr bool operator==(const StencilState & other) const {
            return pass == other.pass && fail == other.fail && zFail == other.zFail && compare == other.compare && ref == other.ref &&
                   readMask == other.readMask && writeMask == other.writeMask;
        }
        constexpr bool operator!=(const StencilState & other) const { return !operator==(other); }
    };

    /// Viewport settings. Defines transform of normalized device coordinates (NDC) to Window coordinates.
    ///   - Left top is (-1, 1) in NDC space, map to Window space coordinate (0, 0).
    ///   - Right bottom is (1, -1) in NDC space, map to Window space coordinate (width, height).
    ///   - Set width and/or heigh to FLT_MAX indicating the current size of the render target.
    struct Viewport {
        float x        = 0.0f;
        float y        = 0.0f;
        float width    = FLT_MAX; ///< default to current size of the render target.
        float height   = FLT_MAX; ///< default to current size of the render target.
        float minDepth = 0.0f;
        float maxDepth = 1.0f;

        constexpr bool fullScreen() const { return 0.0f == x && 0.0f == y && FLT_MAX == width && FLT_MAX == height; }
        constexpr bool operator==(const Viewport & other) const {
            return x == other.x && y == other.y && width == other.width && height == other.height && minDepth == other.minDepth && maxDepth == other.maxDepth;
        }
        constexpr bool operator!=(const Viewport & other) const { return !operator==(other); }
    };

    /// Scissor rectangle in Window coordinates. (0, 0) is the left top corner of the window.
    struct ScissorRect {
        int32_t  x      = 0;
        int32_t  y      = 0;
        uint32_t width  = (~0u); ///< Set to (~0u) indicating with of the current window.
        uint32_t height = (~0u); ///< Set to (~0u) indicating height of the current window.

        constexpr bool disabled() const { return (0 == x) && (0 == y) && (~0u == width) && (~0u == height); }
        constexpr bool operator==(const ScissorRect & other) const { return x == other.x && y == other.y && width == other.width && height == other.height; }
        constexpr bool operator!=(const ScissorRect & other) const { return !operator==(other); }
    };

    union ClearColorValue {
        float    f4[4];
        uint32_t u4[4];
        int32_t  i4[4];
    };

    struct ColorTarget {
        GpuResourceView target     = {};   // could be texture or backbuffer
        BlendState      blendState = {};   // blend state for the color target
        uint8_t         writeMask  = 0xFF; // 4 lower bits are write mask for R, G, B, A. Other bits are ignored.

        bool operator==(const ColorTarget & other) const { return target == other.target && blendState == other.blendState && writeMask == other.writeMask; }
        bool operator!=(const ColorTarget & other) const { return !operator==(other); }
    };

    StackArray<ColorTarget, 8> colors;
    ClearColorValue            clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}}; // clear to solid black.
    GpuResourceView            depthStencilTarget;                      // must be a texture
    DepthState                 depthState   = {};
    StencilState               stencilState = {};
    float                      clearDepth   = 1.0;
    uint32_t                   clearStencil = 0;
    Viewport                   viewport     = {};
    ScissorRect                scissorRect  = {};

    RenderTarget & addColorTarget(AutoRef<Texture> texture, uint32_t mipLevel = 0, uint32_t arrayLayer = 0) {
        colors.append(ColorTarget {.target = GpuResourceView {}.setArtifact(texture).setSubresourceExtent({1, 1}).setSubresourceIndex({mipLevel, arrayLayer})});
        return *this;
    }

    RenderTarget & addColorTarget(AutoRef<Backbuffer> backbuffer) {
        colors.append(ColorTarget {.target = GpuResourceView {}.setArtifact(backbuffer).setSubresourceExtent({1, 1}).setSubresourceIndex({0, 0})});
        return *this;
    }

    RenderTarget & setColorTarget(size_t index, AutoRef<Texture> texture, uint32_t mipLevel = 0, uint32_t arrayLayer = 0) {
        GN_ASSERT(index < colors.size());
        colors[index].target.setArtifact(texture).setSubresourceExtent({1, 1}).setSubresourceIndex({mipLevel, arrayLayer});
        return *this;
    }

    RenderTarget & setColorTarget(size_t index, AutoRef<Backbuffer> backbuffer) {
        colors[index].target.setArtifact(backbuffer).setSubresourceExtent({1, 1}).setSubresourceIndex({0, 0});
        return *this;
    }

    RenderTarget & setColorFormat(size_t index, gfx::img::PixelFormat format) {
        colors[index].target.setImageViewFormat(format);
        return *this;
    }

    RenderTarget & setClearColor(float r, float g, float b, float a = 1.0f) {
        clearColor.f4[0] = r;
        clearColor.f4[1] = g;
        clearColor.f4[2] = b;
        clearColor.f4[3] = a;
        return *this;
    }

    RenderTarget & setDepthStencilTarget(AutoRef<Texture> target) {
        depthStencilTarget.setArtifact(target);
        return *this;
    }

    RenderTarget & setDepthStencilFormat(gfx::img::PixelFormat format) {
        depthStencilTarget.imageView.format = format;
        return *this;
    }

    RenderTarget & setDepthStencilSubresourceIndex(GpuResourceView::SubresourceIndex index) {
        depthStencilTarget.imageView.range.i = index;
        return *this;
    }

    RenderTarget & setDepthState(DepthState depthState_) {
        depthState = depthState_;
        return *this;
    }

    RenderTarget & setStencilState(StencilState stencilState_) {
        stencilState = stencilState_;
        return *this;
    }

    RenderTarget & setClearDepth(float clearDepth_) {
        clearDepth = clearDepth_;
        return *this;
    }

    RenderTarget & setClearStencil(uint32_t clearStencil_) {
        clearStencil = clearStencil_;
        return *this;
    }

    RenderTarget & setViewport(Viewport viewport_) {
        viewport = viewport_;
        return *this;
    }

    RenderTarget & setScissorRect(ScissorRect scissorRect_) {
        scissorRect = scissorRect_;
        return *this;
    }

    struct CreateParameters {
        // tbd
    };

    static GN_API AutoRef<RenderTarget> create(const StrA & name, const CreateParameters & params);

protected:
    using Artifact::Artifact;
};

static_assert(GN::rdg::RenderTarget::BlendState {}.enabled() == false);
static_assert(GN::rdg::RenderTarget::DepthState {}.testEnabled() == false);
static_assert(GN::rdg::RenderTarget::DepthState {}.writeEnabled() == false);
static_assert(GN::rdg::RenderTarget::StencilState {}.enabled() == false);
static_assert(GN::rdg::RenderTarget::Viewport {}.fullScreen() == true);
static_assert(GN::rdg::RenderTarget::ScissorRect {}.disabled() == true);

enum class GpuShaderStageBits : uint32_t {
    VERTEX   = 1 << 0,
    HULL     = 1 << 1,
    DOMAIN_  = 1 << 2,
    GEOMETRY = 1 << 3,
    PIXEL    = 1 << 4,
    COMPUTE  = 1 << 5,
}; // namespace GN::rdg

typedef BitFlags<GpuShaderStageBits> GpuShaderStageFlags;

// GpuResourceGroup commented out: we use GpuResourceTable (set/binding/view) + rapid_vulkan Drawable/DrawPack instead.
#if 0
/// A group of bindable GPU shader resources.
/// Conceptually, it matches Vulkan's descriptor set.
struct GpuResourceGroup : public GpuResource {
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuResource);

    struct SlotDescription {
        enum Type {
            UNIFORM_BUFFER,
            STORAGE_BUFFER,
            SAMPLED_TEXTURE,
            STORAGE_TEXTURE,
            SAMPLER,
        };

        Type                type  = UNIFORM_BUFFER;
        size_t              count = 1; ///< 1: single resource, >1: fixed sized resource array.
        GpuShaderStageFlags stages;    ///< shader stages that can access this resource or resource array.
    };

    struct CreateParameters {
        AutoRef<GpuContext>        context;
        DynaArray<SlotDescription> slots;
    };

    virtual void setResourceViews(size_t slot, size_t offset, SafeArrayAccessor<const GpuResourceView> views) = 0;

    virtual void addToReadWriteList(Arguments::ArtifactReadWriteList & list) const = 0;

    static GN_API AutoRef<GpuResourceGroup> create(const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};
#endif

} // namespace GN::rdg
