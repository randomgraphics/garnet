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

    /// GPU pipeline management
    //@{
    struct CompiledShaderBlob {
        ArrayProxy<char> binary;
        const char *     entry;
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
        CompiledShaderBlob      vs, hs, ds, gs, ps, cs;
        DynaArray<InputElement> inputElements;
    };
    struct Pipeline : RefCounter {
        //
    };
    virtual auto createPipelines(ArrayProxy<const PipelineCreateParameters>) -> DynaArray<AutoRef<Pipeline>> = 0;
    //@}

    struct Surface;

    /// GPU command list
    //@{
    enum class CommandListType {
        GRAPHICS,
        COMPUTE,
        DMA, // for copy operations
    };
    enum class PrimitiveType {
        POINT_LIST,
        LINE_LIST,
        LINE_STRIP,
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
    };
    struct SubPass {
        DynaArray<size_t> inputs;
        DynaArray<size_t> colors;
        size_t            depth = size_t(~0);
    };
    struct RenderPass {
        DynaArray<AutoRef<Surface>> targets;
        DynaArray<SubPass>          subs;
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
    struct VertexBuffer {
        const Surface * surface;
        uint32_t        offset; // offset in bytes
        uint32_t        stride; // stride in bytes
    };
    struct DrawParameters {
        AutoRef<Pipeline> pipeline;
        ArrayProxy<const VertexBuffer> vertexBuffers;
        Surface *                      indexBuffer = nullptr; // set to null for non-indexed draw.

        PrimitiveType primitive = PrimitiveType::TRIANGLE_LIST;
        uint32_t      instanceBase = 0; ///< the instance ID of the first instance to draw
        uint32_t      instanceCount = 1; ///< the number of instances to draw
        uint32_t      vertexBase = 0; ///< the value added to the vertex index before indexing into the vertex buffer
        uint32_t      vertexOrIndexCount = 0;
        uint32_t      indexBase = 0; ///< the base index within the index buffer. Ignored if indexBuffer is null.
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

    /// A one-time use command list to record GPU rendering commands. Once the command list is submitted to GPU (via kickOff() method),
    /// it becomes inaccessible and should not be used anymore.
    struct CommandList : public RefCounter {
        // graphics commands (not valid on compute and copy command list)
        //@{
        virtual void begin(const RenderPass &)            = 0; ///< begin a new render pass
        virtual void next()                               = 0; ///< move to next subpass.
        virtual void end()                                = 0; ///< end current render pass
        virtual void clear(const ClearScreenParameters &) = 0; ///< clear screen. must be called between begin() and end() calls.
        virtual void draw(const DrawParameters &)         = 0; ///< issue GPU draw command. must be called between begin() and end() calls.
        //@}
        virtual void comp(const ComputeParameters &)      = 0; ///< issue GPU compute command.
        virtual void copy(const CopySurfaceParameters &)  = 0; ///< issue GPU copy command
        void         copy(Surface * from, Surface * to) { copy({from, 0, uint64_t(~0), to, 0}); } ///< helper to copy entire surface.
    };

    struct Kicked {
        uint64_t fence     = 0;
        uint64_t semaphore = 0;
        bool     empty() const { return 0 == fence && 0 == semaphore; }
    };

    /// Create a new command list. Once the command list is created, it is empty and ready to record commands.
    /// It is imperative that a command list must be either discarded or kicked off, otherwise it will be leaked,
    /// along with all resources it references.
    virtual auto createCommandList(const CommandListCreateParameters &) -> AutoRef<CommandList> = 0;

    /// Kick off an array of command list. Must be called in between beginFrame() and present() calls.
    virtual auto kickOff(ArrayProxy<CommandList *>) -> Kicked = 0;

    /// Discard an array of command list. This is used to discard the command list that were created but not useful any more.
    virtual void discard(ArrayProxy<CommandList *>) = 0;

    /// Block the calling CPU thread until the fence, if specified, is passed. If fence is 0, wait all pending
    /// works from all engine to be done.
    virtual void finish(uint64_t fence = 0) = 0;
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

    /// GPU surface management
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
    /// This could be a texture or a buffer.
    struct Surface : public RefCounter {
        virtual MappedSurfaceData map(uint32_t subSurfaceId)   = 0;
        virtual void              unmap(uint32_t subSurfaceId) = 0;
    };
    virtual AutoRef<Surface> createSurface(const SurfaceCreateParameters &) = 0;
    //@}

    // //@{
    // struct QueryCreateParameters {};
    // struct Query : public RefCounter {};
    // virtual AutoRef<Query> createQuery(const QueryCreateParameters &) = 0;
    // //@}

    /// frame management
    //@{
    struct Frame {
        //
    };
    struct PresentParameters {
        //
    };
    virtual auto beginFrame() -> Frame = 0;
    virtual void present(const PresentParameters &) = 0;
    //@}
};

struct ShaderCompileParameters {
    struct Options {
        bool debuggable : 1; ///< set to true to generate shader binary with debug information.
        bool optimized  : 1; ///< set to true to generate optimized shader.
        Options(): debuggable(GN_BUILD_DEBUG_ENABLED), optimized(true) {}
    };

    const char * source;
    size_t       length {}; // could be set to 0, if the source string is null-terminated.
    const char * entry {};
    const char * profile {};
    Options      options {};
};
GN_API DynaArray<uint8_t> compileShader(const ShaderCompileParameters &);
} // end of namespace gfx
} // end of namespace GN
