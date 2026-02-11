#pragma once

#include <garnet/GNwin.h>
#include <garnet/gfx/image.h>
#include <unordered_map>
#include <variant>

namespace GN::rdg {

// =============================================================================
// GPU artifact types
// =============================================================================

/// GpuContext represents a GPU context (wrapper of D3D/Vulkan context) that exposes display properties.
struct GpuContext : public Artifact {
    static inline const Guid TYPE = {0x3c4d5e6f, 0x7a8b, 0x9c0d, {0x1e, 0x2f, 0x3a, 0x4b, 0x5c, 0x6d, 0x7e, 0x8f}};

    struct ResetParameters {
        /// The graphics API
        StrA api = "auto";

        /// The target window. If null, then create GPU context in headless mode.
        GN::win::Window * win = {};

        /// display width. If 0, use window width. Must be positive for headless mode.
        uint32_t width = 0;

        // display height. If 0, use window height. Must be positive for headless mode.
        uint32_t height = 0;
    };

    /// Create a new instance of GpuContext.
    static GN_API AutoRef<GpuContext> create(ArtifactDatabase & db);

    virtual bool reset(const ResetParameters &) = 0;

    /// Get display width and height
    virtual std::pair<uint32_t, uint32_t> dimension() const = 0;

protected:
    using Artifact::Artifact;
};

/// Texture represents a 2D/3D/cube texture with optional mipmap and array layers.
struct Texture : public Artifact {
    static inline const Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
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

    struct SubresourceIndex {
        uint32_t mip   = 0; ///< index into mipmap chain
        uint32_t face  = 0; ///< index into array of faces
        uint32_t slice = 0; ///< index into depth slice
    };

    struct SubresourceRange {
        uint32_t numMipLevels   = (uint32_t) -1; ///< -1 means all mip levels
        uint32_t numArrayLayers = (uint32_t) -1; ///< -1 means all array layers
        uint32_t numDepthSlices = (uint32_t) -1; ///< -1 means all depth slices
    };

    /// Return the current texture descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create texture from the given descriptor. Discard old one. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

    /// Create a new instance of empty Texture The texture is not bound to any GPU resource yet. Must call reset() at least once for the texture to be valid to
    /// use.
    static GN_API AutoRef<Texture> create(ArtifactDatabase & db);

    /// Load texture from file. Discard old one. Returns true on success.
    virtual bool load(const StrA & filename) = 0;

protected:
    using Artifact::Artifact;
};

/// Backbuffer represents the a swapchain that can be present to screen.
struct Backbuffer : public Artifact {
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
    /// Descriptor for backbuffer initialization.
    struct Descriptor {
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    /// Create a new instance of Backbuffer.
    static GN_API AutoRef<Backbuffer> create(ArtifactDatabase & db);

    /// Initialize or reinitialize the backbuffer from the given descriptor. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

protected:
    using Artifact::Artifact;
};

/// Sampler represents GPU sampler state (filtering, addressing, LOD, anisotropy).
struct Sampler : public Artifact {
    static inline const Guid TYPE = {0x7be9c60e, 0xf783, 0x5c6f, {0x9f, 0xed, 0x08, 0xbd, 0xe5, 0x02, 0xaa, 0xee}};
    enum class Filter { POINT, LINEAR, ANISOTROPIC };
    enum class AddressMode { REPEAT, MIRROR_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER, MIRROR_CLAMP_TO_EDGE };

    /// Descriptor for sampler creation.
    struct Descriptor {
        Filter      filterMin     = Filter::LINEAR;
        Filter      filterMag     = Filter::LINEAR;
        Filter      filterMip     = Filter::LINEAR;
        AddressMode addressU      = AddressMode::REPEAT;
        AddressMode addressV      = AddressMode::REPEAT;
        AddressMode addressW      = AddressMode::REPEAT;
        float       mipLodBias    = 0.f;
        uint32_t    maxAnisotropy = 1;
        float       minLod        = 0.f;
        float       maxLod        = 0.f; ///< 0 often means "all mips"
    };

    /// Create a new instance of Sampler. Must call reset() at least once for the sampler to be valid to use.
    static GN_API AutoRef<Sampler> create(ArtifactDatabase & db);

    /// Return the current buffer descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create or recreate the underlying buffer from the given descriptor. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

protected:
    using Artifact::Artifact;
};

/// Buffer represents a GPU buffer (vertex, index, constant, storage, etc.).
struct Buffer : public Artifact {
    static inline const Guid TYPE = {0x1f2e3d4c, 0x5b6a, 0x7c8d, {0x9e, 0xaf, 0xb1, 0xc2, 0xd3, 0xe4, 0xf5, 0x06}};
    /// Buffer usage flags.
    enum Usage {
        VERTEX,       ///< Vertex buffer
        INDEX,        ///< Index buffer
        CONSTANT,     ///< Constant/uniform buffer
        STORAGE,      ///< Storage (RW) buffer
        INDIRECT,     ///< Indirect command buffer
        TRANSFER_SRC, ///< Can be used as transfer source
        TRANSFER_DST  ///< Can be used as transfer destination
    };

    /// Buffer descriptor for creation/reset.
    struct Descriptor {
        size_t size        = 0;      ///< Number of bytes for the buffer.
        Usage  usage       = VERTEX; ///< Usage flag.
        bool   cpuWritable = false;  ///< CPU can write (mapped buffer).
        bool   cpuReadable = false;  ///< CPU can read (mapped buffer).
    };

    /// Create a new instance of empty Buffer. The buffer is not bound to any GPU resource yet.
    /// Must call reset() at least once for the buffer to be valid to use.
    static GN_API AutoRef<Buffer> create(ArtifactDatabase & db);

    /// Return the current buffer descriptor.
    virtual const Descriptor & descriptor() const = 0;

    /// Create or recreate the underlying buffer from the given descriptor. Returns true on success.
    virtual bool reset(const Descriptor & d) = 0;

protected:
    using Artifact::Artifact;
};

/// Base class of all mesh types.
/// Meshes can be either indexed (using an index buffer) or non-indexed (drawing vertices directly).
struct Mesh : public Artifact {
    static inline constexpr Guid TYPE = {0x8c9d4a1f, 0xb284, 0x5d7f, {0x9a, 0xfe, 0x19, 0xce, 0xf6, 0x13, 0xbb, 0xff}};

    struct VertexBuffer {
        AutoRef<Buffer>       buffer;
        gfx::img::PixelFormat format;     ///< pixel format of the vertex
        uint32_t              offset = 0; ///< offset in bytes from beginning of the buffer to the first vertex
        uint32_t              stride = 0; ///< vertex stride in bytes
    };

    /// Complete mesh descriptor containing all vertex and index data information
    struct Descriptor {
        std::unordered_map<StrA, VertexBuffer> vertices;    ///< vertices, key is semantic name
        uint32_t                               vertexCount; ///< number of vertices in the mesh
        AutoRef<Buffer>                        indexBuffer; ///< index buffer. Null if mesh is non-indexed.
        uint32_t                               indexCount;  ///< number of indices. Undefined if non-indexed.
        uint32_t indexOffset; ///< offset in bytes from beginning of the index buffer to the first index. Undefined if non-indexed.
    };

    /// Create a new instance of Mesh.
    static GN_API AutoRef<Mesh> create(ArtifactDatabase & db);

    /// Get the complete mesh descriptor containing all vertex and index data.
    virtual const Descriptor & descriptor() const = 0;

    /// Initialie the mesh. Discard old one. Returns true on success.
    virtual bool reset(const Descriptor &) = 0;

    /// Load mesh from file. Discard old one. Returns true on success.
    virtual bool load(const StrA & filename) = 0;

protected:
    using Artifact::Artifact;
};

/// Render target is ether a subsurface of a texture or a back buffer
struct RenderTarget {
    std::variant<AutoRef<Texture>, AutoRef<Backbuffer>> target;
    Texture::SubresourceIndex                           sub; ///< only used for texture targets
};

} // namespace GN::rdg
