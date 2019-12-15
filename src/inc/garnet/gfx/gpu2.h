#include "../GNwin.h"
#include <vector>

namespace GN
{
///
/// namespace for GFX module
///
namespace gfx
{
    struct Gpu2 : public RefCounter
    {
        //@{
        enum class GraphicsAPI
        {
            AUTO,
            D3D12,
            VULKAN,
        };
        struct CreationParameters
        {
            win::Window * window;
            GraphicsAPI   api = GraphicsAPI::AUTO;
            uint32_t      width = 0, height = 0; // back buffer size. set to 0 to use window size.
            bool          fullscreen = false;
            bool          debug = GN_BUILD_DEBUG_ENABLED;
        };
        static GN_API AutoRef<Gpu2> createGpu2(const CreationParameters &);
        //@}

        /// GPU pipeline
        //@{
        struct CompiledShaderBlob
        {
            const void * ptr = nullptr; ///< pointer to shader byte code. Null means the shader stage is disbaled.
            uint64_t sizeInBytes = 0; ///< size of the byte code
            const char * entry = nullptr; ///< entry point of the shader (only used in spir-v shader)
        };
        
        struct InputElement
        {
            const char * semantic;
            uint32_t     index;
            ColorFormat  format;
            uint32_t     slot; // input assembly slot: 0-15
            uint32_t     offset = (uint32_t)-1; // byte offset of the element. Set to -1 for auto alignment.
            bool         instanceData = false;
            uint32_t     instanceRate = 0;
        };
        struct PipelineCreationParameters
        {
            CompiledShaderBlob vs, hs, ds, gs, ps, cs;
            const InputElement * inputElements;
            uint32_t             numInputElements;
            const char * states; // pipeline state defined in JSON format.
        };
        virtual DynaArray<uint64_t> createPipelineStates(const PipelineCreationParameters *, size_t) = 0;
        virtual void deletePipelineStates(const uint64_t *, size_t) = 0;
        //@}

        struct Surface;

        /// GPU command list
        //@{
        enum class CommandListType
        {
            GRAPHICS,
            COMPUTE,
            DMA, // for copy operations
        };
        struct ClearParameters
        {
            float    color[4] = {.0f, .0f, .0f, .0f};
            float    depth = 1.0f;
            uint32_t stencil = 0;
            union
            {
                uint8_t u8 = 0xFF;
                struct
                {
                    uint8_t c : 1;
                    uint8_t d : 1;
                    uint8_t s : 1;
                };
            } flags;
        };
        struct DrawParameters
        {
            uint64_t pso;
            PrimitiveType prim;
            bool     indexed;
            uint32_t vertexOrIndexCount;
            uint32_t basevertex = 0;
            uint32_t baseindex = 0;
        };
        struct ComputeParameters
        {
            uint64_t pso;
            uint32_t groupX;
            uint32_t groupY = 1;
            uint32_t groupZ = 1;
        };
        struct CopyBufferRegionParameters
        {
            Surface * source;
            uint64_t  sourceOffset;
            uint64_t  sourceBytes;
            Surface * dest;
            uint64_t  destOffset;
        };
        struct CommandListCreationParameters
        {
            CommandListType type = CommandListType::GRAPHICS;
            uint64_t initialPipelineState = 0; // optional initial pipeline state. If empty, the default state is used.
        };
        struct CommandList : public RefCounter
        {
            virtual void reset(uint64_t initialPipelineState = 0) = 0;
            virtual void clear(const ClearParameters &) = 0;
            virtual void draw(const DrawParameters &) = 0;
            virtual void compute(const ComputeParameters &) = 0;
            virtual void copyBufferRegion(const CopyBufferRegionParameters &) = 0;
            void copySurface(Surface * from, Surface * to) { copyBufferRegion({from, 0, 0, to, 0}); }
        };
        virtual AutoRef<CommandList> createCommandList(const CommandListCreationParameters &) = 0;
        virtual void kickoff(CommandList &, uint64_t * fence = nullptr) = 0; ///< kick off command lists.
        virtual void finish(uint64_t fence = 0) = 0; // Block the calling CPU thread until the fence, if specified, is passsed. If fence is 0, wait all pending works from all engine to be done.
        //@}

        /// GPU memory pool
        //@{
        enum class MemoryType
        {
            DEFAULT = 0,
            UPLOAD,
            READBACK,
        };
        struct MemoryBlockCreationParameters
        {
            uint64_t sizeInMB;
            MemoryType type;
        };
        struct MemoryBlock : public RefCounter
        {
        };
        virtual AutoRef<MemoryBlock>  createMemoryBlock(const MemoryBlockCreationParameters &) = 0;
        //@}

        // /// descriptor pool
        // //@{
        // struct DescriptorPoolCreationParameters
        // {
        //     uint32_t capacity = 1024;
        // };
        // struct DescriptorPool : public RefCounter
        // {
        //     virtual uint32_t descSize() = 0; // returns size of one descriptor.
        // };
        // virtual AutoRef<DescriptorPool> createDescriptorPool(const DescriptorPoolCreationParameters &) = 0;
        // //@}

        // //@{
        // struct DescriptorCreationParameters
        // {
        // };
        // virtual void createDescriptors(const DescriptorCreationParameters *, size_t) = 0;
        // //@}

        /// GPU surface
        //@{
        enum class SurfaceDimension
        {
            BUFFER,
            TEXTURE,
        };
        union SurfaceFlags
        {
            uint8_t u8 = 0;
            struct
            {
                uint8_t sr : 1; // the surface could be used as SRV
                uint8_t ua : 1; // the surface could be used as UAV
                uint8_t rt : 1; // the surface could be used as RTV
                uint8_t ds : 1; // the surface could be used as DSV
            };
        };
        struct SurfaceCreationParameters
        {
            MemoryBlock * memory;
            uint64_t offset;
            SurfaceDimension dim;
            struct TextureDesc
            {
                uint32_t w, h = 1, d = 1, a = 1, m = 1, s = 1; // width, height, depth, array, mipmaps, samples.
                ColorFormat f; // format
            } t;
            struct BufferDesc
            {
                uint32_t bytes;
            } b;
            SurfaceFlags flags;
        };
        struct MappedSurfaceData
        {
            void * ptr;
            uint64_t slicePitch;
            uint64_t rawPitch;
            uint32_t subSurfaceId;
        };
        /// this could be a texture or a buffer.
        struct Surface : public RefCounter
        {
            virtual MappedSurfaceData map(uint32_t subSurfaceId) = 0;
            virtual void unmap(uint32_t subSurfaceId) = 0;
        };
        virtual AutoRef<Surface> createSurface(const SurfaceCreationParameters &) = 0;
        //@}

        //@{
        struct QueryCreationParameters
        {
        };
        struct Query : public RefCounter
        {
        };
        virtual AutoRef<Query> createQuery(const QueryCreationParameters &) = 0;
        //@}

        /// present
        //@{
        struct PresentParameters
        {

        };
        virtual void present(const PresentParameters &) = 0;
        //@}
    };

    struct ShaderCompileParameters
    {
        struct Options
        {
            bool debugable : 1; ///< set to true to generate shader binary with debug information.
            bool optimized : 1; ///< set to true to generate optimized shader.
            Options() : debugable(GN_BUILD_DEBUG_ENABLED), optimized(true) {}
        };

        const char * source;
        size_t       length = 0; // could be 0 for null-terminated string.
        const char * entry;
        const char * profile;
        Options      options;
    };
    GN_API DynaArray<uint8_t> compileHLSL(const ShaderCompileParameters &);
} // end of namespace gfx
} // end of namespace GN
