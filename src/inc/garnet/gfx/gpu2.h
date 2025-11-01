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
        METAL,
    };
    struct CreateParameters {
        win::Window * window; ///< window to use for rendering. Set to null for headless mode.
        GraphicsAPI   api   = GraphicsAPI::AUTO; ///< graphics API to use.
        uint32_t      width = 0; ///< back buffer width. set to 0 to use window size.
        uint32_t      height = 0; ///< back buffer height. set to 0 to use window size.
        bool          fullscreen = false; ///< set to true to use fullscreen mode.
        bool          debug      = GN_BUILD_DEBUG_ENABLED; ///< set to true to enable debug mode.
    };
    static GN_API AutoRef<Gpu2> createGpu2(const CreateParameters &);
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

    // *********************************************************************************************************************************************************
    //
    // resource management
    //
    // *********************************************************************************************************************************************************

    union SurfaceFlags {
        uint8_t u8 = 0;
        struct {
            uint8_t sr : 1; // the surface could be used as SRV
            uint8_t ua : 1; // the surface could be used as UAV
            uint8_t rt : 1; // the surface could be used as RTV
            uint8_t ds : 1; // the surface could be used as DSV
        };
    };

    enum class SurfaceType {
        BUFFER,
        TEXTURE,
    };

    struct TextureDesc {
        SurfaceFlags flags = {0};
        uint32_t     w = 1, h = 1, d = 1, a = 1, m = 1, s = 1; ///< width, height, depth, array, mipmaps, samples.
        PixelFormat  f = PixelFormat::UNKNOWN;                                    ///< format
    };

    struct BufferDesc {
        SurfaceFlags flags = {0};
        uint32_t     bytes = 0; ///< buffer size in bytes.
    };

    struct SamplerDesc {
        //
    };

    struct Surface : public RefCounter {
        //
    };

    struct Texture : public Surface {
        //
    };

    struct Buffer : public Surface {
        //
    };

    struct Sampler : public RefCounter {
        //
    };

    virtual AutoRef<Buffer>  createBuffer(const BufferDesc &) = 0;
    virtual AutoRef<Texture> createTexture(const TextureDesc &) = 0;
    virtual AutoRef<Sampler> createSampler(const SamplerDesc &) = 0;

    // *********************************************************************************************************************************************************
    //
    // shader management
    //
    // *********************************************************************************************************************************************************

    struct CompiledShaderBlob {
        SafeArrayAccessor<char> binary {};
        const char *            entry {};
    };

    struct ShaderCompileParameters {
        const char * source;
        size_t       length {}; // could be set to 0, if the source string is null-terminated.
        const char * entry {};
        const char * profile {};
    };

    struct Shader : public RefCounter {
        //
    };

    virtual auto compileShader(const ShaderCompileParameters &) -> CompiledShaderBlob = 0;

    virtual auto createShader(const CompiledShaderBlob &) -> AutoRef<Shader> = 0;

    // *********************************************************************************************************************************************************
    //
    // Pipeline management
    //
    // *********************************************************************************************************************************************************

    enum class PrimitiveTopology {
        POINT_LIST,
        LINE_LIST,
        LINE_STRIP,
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
    };

    struct VertexElement {
        const char * semanticName = nullptr;
        uint32_t     semanticIndex = 0;
        PixelFormat  format;
        uint32_t     vertexBufferIndex = 0; ///< the index of the vertex buffer to bind to.
        uint32_t     offset            = 0; ///< byte offset of the element.
    };
    struct VertexLayout {
        SafeArrayAccessor<const VertexElement> elements;
    };

    /// @brief A pipeline object is an immutable object that represents a complete GPU pipeline state.
    struct Pipeline : public RefCounter {
        //
    };


    struct GpuState : public RefCounter {
        virtual void reset() = 0; ///< reset the state to default values.
        // virtual void setViewport(const Rect<uint32_t> & viewport) = 0;
        // virtual void setScissorRect(const Rect<uint32_t> & scissorRect) = 0;

        /// @brief Generate an immutable pipeline object from the current state.
        virtual auto snapshot() -> AutoRef<Pipeline> = 0;

        // Shader
        virtual void setVertexShader(const CompiledShaderBlob & vertexShader) = 0;
        virtual void setPixelShader(const CompiledShaderBlob & pixelShader) = 0;
        virtual void setGeometryShader(const CompiledShaderBlob & geometryShader) = 0;
        virtual void setHullShader(const CompiledShaderBlob & hullShader) = 0;
        virtual void setDomainShader(const CompiledShaderBlob & domainShader) = 0;

        // Vertex Input
        virtual void setVertexBuffers(const SafeArrayAccessor<const Buffer *> & vertexBuffers) = 0;
        virtual void setIndexBuffer(const Buffer * indexBuffer) = 0;
        virtual void setPrimitiveTopology(const PrimitiveTopology & primitiveTopology) = 0;
        virtual void setVertexLayout(const VertexLayout & vertexLayout) = 0;

        // Output Merger
        // virtual void setBlendState(const BlendState & blendState) = 0;
        // virtual void setRasterizerState(const RasterizerState & rasterizerState) = 0;
        // virtual void setDepthStencilState(const DepthStencilState & depthStencilState) = 0;
        // virtual void setSamplerState(const SamplerState & samplerState) = 0;
    };

    virtual auto createGpuState() -> AutoRef<GpuState> = 0;

    // *********************************************************************************************************************************************************
    //
    // render pass management
    //
    // *********************************************************************************************************************************************************

    struct SubPass {
        DynaArray<size_t> inputs;
        DynaArray<size_t> colors;
        size_t depth = size_t(~0);
    };

    static constexpr Surface * BACK_BUFFER = (Surface *)(size_t(~0)); ///< special value for backbuffer.

    struct RenderPassCreateParameters {
        DynaArray<Surface *> renderTargets;
        DynaArray<SubPass> subPasses;
    };


    struct RenderPass : public RefCounter {
        virtual size_t subPassCount() const = 0; ///< return the number of subpasses.
    };

    virtual auto createRenderPass(const RenderPassCreateParameters &) -> AutoRef<RenderPass> = 0;

    // *********************************************************************************************************************************************************
    //
    // command list management
    //
    // *********************************************************************************************************************************************************

    enum class CommandListType {
        GRAPHICS,
        COMPUTE,
        DMA, // for copy operations
    };
    // enum class PrimitiveType {
    //     POINT_LIST,
    //     LINE_LIST,
    //     LINE_STRIP,
    //     TRIANGLE_LIST,
    //     TRIANGLE_STRIP,
    // };

    struct ClearRenderTargetParameters {
        size_t attachmentIndex = 0; ///< the index of the attachment (of the current sub pass) to clear.
        float    color[4] = {.0f, .0f, .0f, .0f}; ///< color value to clear.
        float    depth    = 1.0f; ///< depth value to clear.
        uint32_t stencil  = 0; ///< stencil value to clear.
    };

    struct CommandListCreateParameters {
        CommandListType type = CommandListType::GRAPHICS;
    };

    struct DrawParameters {
        AutoRef<Pipeline> pipeline;
        uint32_t instanceBase = 0; ///< the instance ID of the first instance to draw
        uint32_t instanceCount = 1; ///< the number of instances to draw
        uint32_t vertexBase = 0; ///< the value added to the vertex index before indexing into the vertex buffer
        uint32_t indexBase = 0; ///< the base index within the index buffer. Ignored if indexBuffer is null.
        uint32_t vertexOrIndexCount = 0; ///< the value added to the vertex index before indexing into the vertex buffer
    };

    struct ComputeParameters {
        AutoRef<Pipeline> pipeline;
        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t depth = 1;
    };

    // struct CopySurfaceParameters {
    //     Surface * source;
    //     uint64_t  sourceOffset = 0;
    //     uint64_t  sourceBytes  = uint64_t(~0);
    //     Surface * dest;
    //     uint64_t  destOffset = 0;
    // };

    /// A one-time use command list to record GPU rendering commands. Once the command list is submitted to GPU (via kickOff() method),
    /// it becomes inaccessible and should not be used anymore.
    struct CommandList : public RefCounter {
        // graphics commands (not valid on compute and copy command list)
        //@{
        virtual void begin(RenderPass &)                        = 0; ///< begin a new render pass. Must be paired with end() calls.
        virtual void next()                                     = 0; ///< move to the next subpass. Must be called between begin() and end() calls.
        virtual void end()                                      = 0; ///< end current render pass. Must be paired with begin() calls.
        virtual void clear(const ClearRenderTargetParameters &) = 0; ///< clear an attachment. must be called between begin() and end() calls.
        virtual void draw(const DrawParameters &)         = 0; ///< issue GPU draw command. must be called between begin() and end() calls.
        //@}


        virtual void comp(const ComputeParameters &)      = 0; ///< issue GPU compute command. Must be called outside of render pass.

        // virtual void copy(const CopySurfaceParameters &)  = 0; ///< issue GPU copy command. Must be called outside of render pass.
        // void         copy(Surface * from, Surface * to) { copy({from, 0, uint64_t(~0), to, 0}); } ///< helper to copy entire surface. Must be called outside of render pass.
    };

    struct KickOffParameters {
        SafeArrayAccessor<CommandList *> commandLists;
        SafeArrayAccessor<uint64_t> fences;
        SafeArrayAccessor<uint64_t> semaphores;
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
    virtual auto kickOff(const KickOffParameters &) -> Kicked = 0;

    /// Discard an array of command list. This is used to discard the command list that were created but not useful any more.
    virtual void discard(SafeArrayAccessor<CommandList *>) = 0;

    /// Block the calling CPU thread until the fence, if specified, is passed. If fence is 0, wait until GPU is idle.
    virtual void finish(uint64_t fence = 0) = 0;
    //@}

    // *********************************************************************************************************************************************************
    //
    // frame management
    //
    // *********************************************************************************************************************************************************

    // struct Frame {
    //     /// Backbuffer of the current frame. Can only be used during beginFrame() and present() calls. After present() is called,
    //     /// this pointer is no longer valid.
    //     AutoRef<Surface> backbuffer;
    //     // uint64_t fence = 0;
    //     // uint64_t semaphore = 0;
    // };
    // virtual auto beginFrame() -> Frame = 0;

    struct PresentParameters {
        //
    };
    virtual void present(const PresentParameters &) = 0;
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
