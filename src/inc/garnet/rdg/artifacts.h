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
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "GpuContext";

    struct CreateParameters {
        /// The graphics API ("auto" = platform default; "vulkan", "d3d12", "metal").
        StrA api = "auto";
    };

    /// Create a new instance of GpuContext.
    static GN_API AutoRef<GpuContext> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

    /// Vulkan instance handle (VkInstance cast to intptr_t) for use with Window::getVulkanSurfaceHandle.
    /// Returns 0 if this context is not Vulkan.
    virtual intptr_t getVulkanInstanceHandle() const { return 0; }

protected:
    using Artifact::Artifact;
};

/// Base class for all GPU resources.
struct GpuResource : public Artifact {
    virtual GpuContext & gpu() const = 0;

protected:
    using Artifact::Artifact;
};

/// Backbuffer represents the swapchain that can be drawn to by GPU and presented to screen.
/// - It is in renderable state if and only if it is prepared but not presented. See
///   PrepareBackbuffer and PresentBackbuffer actions for more details.
/// - A newly created backbuffer is always in non-renderable state.
struct Backbuffer : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "Backbuffer";

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
    static GN_API AutoRef<Backbuffer> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

/// Texture represents a 2D/3D/cube texture with optional mipmap and array layers.
struct Texture : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "Texture";

    /// Descriptor used when creating or declaring the texture (format, dimensions).
    struct Descriptor {
        gfx::img::PixelFormat format  = gfx::img::PixelFormat::UNKNOWN();
        uint32_t              width   = 1;
        uint32_t              height  = 1;
        uint32_t              depth   = 1; ///< 1 = 2D, >1 = 3D
        uint32_t              faces   = 1; ///< 1 = 2D, 6 = cubemap, >1 = array
        uint32_t              levels  = 0; ///< 0 = full mipmap chain, 1 = single level, >1 = mipmap chain with specified number of levels
        uint32_t              samples = 1; ///< 1 = no multisampling
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
    static GN_API AutoRef<Texture> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

    /// Load texture from file. Returns a texture artifact named after the file name.
    /// If the file has been loaded before, return the existing artifact.
    static GN_API AutoRef<Texture> load(ArtifactDatabase & db, const LoadParameters & params);

protected:
    using GpuResource::GpuResource;
};

/// Sampler represents GPU sampler state (filtering, addressing, LOD, anisotropy).
struct Sampler : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "Sampler";

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

/// Buffer represents a GPU buffer
struct Buffer : public GpuResource {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "Buffer";

    struct CreateParameters {
        AutoRef<GpuContext> context;
        uint64_t            size = 0; ///< Size of the buffer in bytes.
    };

    /// Synchronously upload CPU data into this buffer.
    /// \return true on success.
    virtual bool setContent(const void * data, uint64_t size) = 0;

    static GN_API AutoRef<Buffer> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuResource::GpuResource;
};

struct BufferView {
    /// The buffer that contains the geometry data.
    AutoRef<Buffer> buffer;

    /// offset in bytes from beginning of the buffer to the first element.
    uint64_t offset = 0;

    /// size of the data in the buffer in bytes, starting from offset.
    uint64_t size = 0;
};

// /// Allocate a block of memory for uploading dynamic data to GPU. The allocated slice will be released when the transient arena is deleted.
// struct GpuTransientUploader : public TransientArena {

//     struct Slice {
//         void * data = nullptr; ///< pointer to the allocated data.
//         size_t size = 0; ///< size of the allocated data in bytes.
//     protected:
//         Slice(void * data_, size_t size_): data(data_), size(size_) {}
//     };

//     virtual Slice allocate(size_t size) = 0;

// protected:
//     using TransientArena::TransientArena;
// };

// /// Base class of all mesh types.
// /// Meshes can be either indexed (using an index buffer) or non-indexed (drawing vertices directly).
// struct Mesh : public GpuResource {
//     GN_API static const uint64_t         TYPE_ID;
//     inline static constexpr const char * TYPE_NAME = "Mesh";

//     // struct VertexBuffer {
//     //     // AutoRef<Buffer>       buffer;
//     //     gfx::img::PixelFormat format;     ///< pixel format of the vertex
//     //     uint32_t              offset = 0; ///< offset in bytes from beginning of the buffer to the first vertex
//     //     uint32_t              stride = 0; ///< vertex stride in bytes
//     // };

//     // /// Complete mesh descriptor containing all vertex and index data information
//     // struct Descriptor {
//     //     /// vertices, key is semantic name
//     //     std::unordered_map<StrA, VertexBuffer> vertices;

//     //     /// number of vertices in the mesh
//     //     uint32_t vertexCount;

//     //     /// index buffer. Null if mesh is non-indexed.
//     //     // AutoRef<Buffer> indexBuffer;

//     //     /// number of indices. 0, if non-indexed.
//     //     uint32_t indexCount;

//     //     /// offset in bytes from beginning of the index buffer to the first index. Ignored if indexCount is 0.
//     //     uint32_t indexOffset;
//     // };

//     // struct CreateParameters {
//     //     AutoRef<GpuContext> context;
//     //     Descriptor          descriptor;
//     // };

//     struct LoadParameters {
//         AutoRef<GpuContext> context;
//         StrA                filename;
//     };

//     // /// Get the complete mesh descriptor containing all vertex and index data.
//     // virtual const Descriptor & descriptor() const = 0;

//     // /// Create a new instance of Mesh.
//     // static GN_API AutoRef<Mesh> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

//     /// Load mesh from file. Returns a mesh artifact named after the file name.
//     /// If the file has been loaded before, return the existing artifact.
//     static GN_API AutoRef<Mesh> load(ArtifactDatabase & db, const LoadParameters & params);

// protected:
//     using GpuResource::GpuResource;
// };

// Represents a view to a GPU image. Could be a texture or a backbuffer.
struct GpuImageView {
    struct SubresourceIndex {
        uint32_t mip  = 0; ///< index into mipmap chain
        uint32_t face = 0; ///< index into array of faces

        bool operator==(const SubresourceIndex & other) const { return mip == other.mip && face == other.face; }
        bool operator!=(const SubresourceIndex & other) const { return !operator==(other); }
    };

    struct SubresourceRange {
        uint32_t numMipLevels   = (uint32_t) -1; ///< -1 means all mip levels
        uint32_t numArrayLayers = (uint32_t) -1; ///< -1 means all array layers

        bool operator==(const SubresourceRange & other) const { return numMipLevels == other.numMipLevels && numArrayLayers == other.numArrayLayers; }
        bool operator!=(const SubresourceRange & other) const { return !operator==(other); }
    };

    std::variant<AutoRef<Texture>, AutoRef<Backbuffer>> image;
    gfx::img::PixelFormat                               format           = gfx::img::PixelFormat::UNKNOWN();
    SubresourceIndex                                    subresourceIndex = {}; // default to mip level 0, face 0
    SubresourceRange                                    subresourceRange = {}; // default to all mip levels, all array layers

    bool empty() const { return 0 == image.index() ? std::get<0>(image) == nullptr : std::get<1>(image) == nullptr; }
    bool isTexture() const { return image.index() == 0; }
    bool isBackbuffer() const { return image.index() == 1; }

    AutoRef<Artifact> artifact() const {
        if (image.index() == 0)
            return std::get<0>(image);
        else
            return std::get<1>(image);
    }

    AutoRef<Backbuffer> backbuffer() const {
        if (isBackbuffer()) return std::get<1>(image);
        return {};
    }

    AutoRef<Texture> texture() const {
        if (isTexture()) return std::get<0>(image);
        return {};
    }

    bool operator==(const GpuImageView & other) const {
        return image == other.image && format == other.format && subresourceIndex == other.subresourceIndex && subresourceRange == other.subresourceRange;
    }
    bool operator!=(const GpuImageView & other) const { return !operator==(other); }
};

struct TextureView : GpuImageView {
    AutoRef<Sampler> sampler;
};

struct RenderTarget : public Artifact {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "RenderTarget";

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

    /// Viewport settings. Defines tranform of normalized device coordinates (NDC) to Window coordinates.
    ///   - Left top is (-1, 1) in NDC space, map to Window space coordiante (0, 0).
    ///   - Right bottom is (1, -1) in NDC space, map to Window space coordiante (width, height).
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
        GpuImageView    target {};
        BlendState      blendState = {};
        uint8_t         writeMask  = 0xFF;                       // 4 lower bits are write mask for R, G, B, A. Other bits are ignored.
        ClearColorValue clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}}; // clear to solid black.

        ColorTarget & setClearColor(float r, float g, float b, float a = 1.0f) {
            clearColor.f4[0] = r;
            clearColor.f4[1] = g;
            clearColor.f4[2] = b;
            clearColor.f4[3] = a;
            return *this;
        }

        bool operator==(const ColorTarget & other) const { return target == other.target && blendState == other.blendState && writeMask == other.writeMask; }
        bool operator!=(const ColorTarget & other) const { return !operator==(other); }
    };

    struct DepthStencil {
        AutoRef<Texture>               target;
        gfx::img::PixelFormat          format = gfx::img::PixelFormat::UNKNOWN();
        GpuImageView::SubresourceIndex subresourceIndex {};
        DepthState                     depthState   = {};
        StencilState                   stencilState = {};
        float                          clearDepth   = 1.0;
        uint32_t                       clearStencil = 0;

        bool operator==(const DepthStencil & other) const {
            if (target != other.target) return false;
            if (target && subresourceIndex != other.subresourceIndex) return false; // only check subresource index for non-empty texture targets
            if (depthState != other.depthState) return false;
            if (stencilState != other.stencilState) return false;
            return true;
        }

        bool operator!=(const DepthStencil & other) const { return !operator==(other); }
    };

    StackArray<ColorTarget, 8> colors;
    DepthStencil               depthStencil = {};
    Viewport                   viewport     = {};
    ScissorRect                scissorRect  = {};

    /// Returns list of artifacts referenced by this render target.
    virtual SafeArrayAccessor<const Artifact * const> artifacts() const = 0;

    bool operator==(const RenderTarget & other) const {
        return colors == other.colors && depthStencil == other.depthStencil && viewport == other.viewport && scissorRect == other.scissorRect;
    }
    bool operator!=(const RenderTarget & other) const { return !operator==(other); }

    struct CreateParameters {
        // tbd
    };

    static GN_API AutoRef<RenderTarget> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Artifact::Artifact;
};

static_assert(GN::rdg::RenderTarget::BlendState {}.enabled() == false);
static_assert(GN::rdg::RenderTarget::DepthState {}.testEnabled() == false);
static_assert(GN::rdg::RenderTarget::DepthState {}.writeEnabled() == false);
static_assert(GN::rdg::RenderTarget::StencilState {}.enabled() == false);
static_assert(GN::rdg::RenderTarget::Viewport {}.fullScreen() == true);
static_assert(GN::rdg::RenderTarget::ScissorRect {}.disabled() == true);

} // namespace GN::rdg
