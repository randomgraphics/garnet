#include "../GNwin.h"
#include <vector>

namespace GN {
///
/// namespace for GFX module
///
namespace gfx {
struct Gpu2 : public RefCounter {
    //@{
    enum class GraphicsAPI {
        AUTO,
        D3D12,
        VULKAN,
    };
    struct CreateParameters {
        win::Window * window;
        GraphicsAPI   api   = GraphicsAPI::AUTO;
        uint32_t      width = 0, height = 0; // back buffer size. set to 0 to use window size.
        bool          fullscreen = false;
        bool          debug      = GN_BUILD_DEBUG_ENABLED;
    };
    static GN_API AutoRef<Gpu2> createGpu2(const CreateParameters &);
    //@}

    /// GPU pipeline
    //@{
    struct CompiledShaderBlob {
        const void * ptr         = nullptr; ///< pointer to shader byte code. Null means the shader stage is disbaled.
        uint64_t     sizeInBytes = 0;       ///< size of the byte code
        const char * entry       = nullptr; ///< entry point of the shader (only used in spir-v shader)
    };

    struct InputElement {
        const char * semantic;
        uint32_t     index;
        PixelFormat  format;
        uint32_t     slot;                         // input assembly slot: 0-15
        uint32_t     offset       = (uint32_t) -1; // byte offset of the element. Set to -1 for auto alignment.
        bool         instanceData = false;
        uint32_t     instanceRate = 0;
    };
    struct PipelineCreateParameters {
        CompiledShaderBlob   vs, hs, ds, gs, ps, cs;
        const InputElement * inputElements;
        uint32_t             numInputElements;
        const char *         states; // pipeline state defined in JSON format.
    };
    virtual DynaArray<uint64_t> createPipelineStates(const PipelineCreateParameters *, size_t) = 0;
    virtual void                deletePipelineStates(const uint64_t *, size_t)                 = 0;
    //@}

    struct Surface;

    /// GPU command list
    //@{
    enum class CommandListType {
        GRAPHICS,
        COMPUTE,
        DMA, // for copy operations
    };
    struct SubPass {
        ConstRange<size_t> inputs;
        ConstRange<size_t> colors;
        size_t             depth = size_t(~0);
    };
    struct RenderPass {
        ConstRange<Surface *> targets;
        ConstRange<SubPass *> subs;
    };
    struct ClearScreenParameters {
        float    color[4] = {.0f, .0f, .0f, .0f};
        float    depth    = 1.0f;
        uint32_t stencil  = 0;
        union {
            uint8_t u8 = 0xFF;
            struct {
                uint8_t c : 1;
                uint8_t d : 1;
                uint8_t s : 1;
            };
        } flags {};
    };
    struct VertexBufferView {
        const Surface * surface;
        uint32_t        offset; // offset in bytes
        uint32_t        stride; // stride in bytes
    };
    struct DrawParameters {
        uint64_t pso;

        uint32_t                 vertexBufferCount;
        const VertexBufferView * vertexBuffers;
        Surface *                indexBuffer; // set to null for non-indexed draw.

        PrimitiveType prim;
        uint32_t      vertexOrIndexCount;
        uint32_t      baseVertex = 0;
        uint32_t      baseIndex  = 0; // ignored when indexBuffer is null.
    };
    struct ComputeParameters {
        uint64_t pso;
        uint32_t groupX;
        uint32_t groupY = 1;
        uint32_t groupZ = 1;
    };
    struct CopySurfaceParameters {
        Surface * source;
        uint64_t  sourceOffset = 0;
        uint64_t  sourceBytes  = uint64_t(~0);
        Surface * dest;
        uint64_t  destOffset = 0;
    };
    struct CommandListCreateParameters {
        CommandListType type                 = CommandListType::GRAPHICS;
        uint64_t        initialPipelineState = 0; // optional initial pipeline state. If empty, the default state is used.
    };
    struct CommandList : public RefCounter {
        virtual void begin(const RenderPass &)                  = 0; ///< begin a new render pass
        virtual void next()                                     = 0; ///< move to next subpass.
        virtual void end()                                      = 0; ///< end current render pass
        virtual void clearScreen(const ClearScreenParameters &) = 0;
        virtual void draw(const DrawParameters &)               = 0;
        virtual void compute(const ComputeParameters &)         = 0;
        virtual void copySurface(const CopySurfaceParameters &) = 0;
        void         copySurface(Surface * from, Surface * to) { copySurface({from, 0, uint64_t(~0), to, 0}); }
    };
    struct Kicked {
        uint64_t fence     = 0;
        uint64_t semaphore = 0;
        bool     empty() const { return 0 == fence && 0 == semaphore; }
    };
    virtual auto createCommandList(const CommandListCreateParameters &) -> AutoRef<CommandList> = 0;

    /// Kick off an array of command list.
    virtual auto kickOff(ConstRange<CommandList *>) -> Kicked = 0;

    /// Kick off one command list.
    auto kickOff(CommandList & cl) -> Kicked { return kickOff(ConstRange<CommandList *> {&cl}); }

    /// Block the calling CPU thread until the fence, if specified, is passed. If fence is 0, wait all pending
    /// works from all engine to be done.
    virtual void finish(uint64_t fence = 0) = 0;
    //@}

    /// GPU memory pool
    //@{
    enum class MemoryType {
        /// The memory type optimized for GPU reading & writing. CPU access is prohibited.
        DEFAULT = 0,

        /// The memory type optimized for uploading dynamic data from CPU to GPU. GPU access bandwidth is limited.
        /// Best for CPU-write-once, GPU-read-once data. CPU read is allowed but could be slow. GPU-write to this
        /// is not recommended and could cause undefined behavior.
        UPLOAD,

        /// The memory type optimized for reading data back from GPU. GPU access bandwidth is limited.
        /// Best for GPU-write-once, CPU-readable data. CPU-write to this buffer is not recommended and could cause
        /// undefined result.
        READ_BACK,
    };
    struct MemoryBlockCreateParameters {
        uint64_t   sizeInMB;
        MemoryType type;
    };
    struct MemoryBlock : public RefCounter {};
    virtual AutoRef<MemoryBlock> createMemoryBlock(const MemoryBlockCreateParameters &) = 0;
    //@}

    // /// descriptor pool
    // //@{
    // struct DescriptorPoolCreateParameters
    // {
    //     uint32_t capacity = 1024;
    // };
    // struct DescriptorPool : public RefCounter
    // {
    //     virtual uint32_t descSize() = 0; // returns size of one descriptor.
    // };
    // virtual AutoRef<DescriptorPool> createDescriptorPool(const DescriptorPoolCreateParameters &) = 0;
    // //@}

    // //@{
    // struct DescriptorCreateParameters
    // {
    // };
    // virtual void createDescriptors(const DescriptorCreateParameters *, size_t) = 0;
    // //@}

    /// GPU surface
    //@{
    enum class SurfaceType {
        BUFFER,
        TEXTURE,
    };
    union SurfaceFlags {
        uint8_t u8 = 0;
        struct {
            uint8_t sr : 1; // the surface could be used as SRV
            uint8_t ua : 1; // the surface could be used as UAV
            uint8_t rt : 1; // the surface could be used as RTV
            uint8_t ds : 1; // the surface could be used as DSV
        };
    };
    struct SurfaceCreateParameters {
        MemoryBlock * memory;
        uint64_t      offset;
        SurfaceType   type;
        struct TextureDesc {
            uint32_t    w, h = 1, d = 1, a = 1, m = 1, s = 1; ///< width, height, depth, array, mipmaps, samples.
            PixelFormat f;                                    ///< format
        } t;
        struct BufferDesc {
            uint32_t bytes;
        } b;
        SurfaceFlags flags = {0};
    };
    struct MappedSurfaceData {
        void *   ptr;
        uint64_t slicePitch;
        uint64_t rawPitch;
        uint32_t subSurfaceId;
    };
    /// this could be a texture or a buffer.
    struct Surface : public RefCounter {
        virtual MappedSurfaceData map(uint32_t subSurfaceId)   = 0;
        virtual void              unmap(uint32_t subSurfaceId) = 0;
    };
    virtual AutoRef<Surface> createSurface(const SurfaceCreateParameters &) = 0;
    //@}

    //@{
    struct QueryCreateParameters {};
    struct Query : public RefCounter {};
    virtual AutoRef<Query> createQuery(const QueryCreateParameters &) = 0;
    //@}

    /// present
    //@{
    struct PresentParameters {};
    virtual void present(const PresentParameters &) = 0;
    //@}
};

struct ShaderCompileParameters {
    struct Options {
        bool debugable : 1; ///< set to true to generate shader binary with debug information.
        bool optimized : 1; ///< set to true to generate optimized shader.
        Options(): debugable(GN_BUILD_DEBUG_ENABLED), optimized(true) {}
    };

    const char * source;
    size_t       length {}; // could be 0 for null-terminated string.
    const char * entry {};
    const char * profile {};
    Options      options {};
};
GN_API DynaArray<uint8_t> compileHLSL(const ShaderCompileParameters &);
} // end of namespace gfx
} // end of namespace GN
