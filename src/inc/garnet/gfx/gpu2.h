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
        struct PipelineCreationParameters
        {
            const uint8_t * compiledGpuProgramBinary;
            uint64_t        compiledGpuProgramSizeInBytes;
            const char *    states; // pipeline state defined in JSON format.
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
            COPY,
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
            uint32_t indexcount;
            uint32_t basevertex;
            uint32_t baseindex;
            uint32_t primitive;
        };
        struct ComputeParameters
        {
            uint64_t pso;
            uint32_t groupX;
            uint32_t groupY = 1;
            uint32_t groupZ = 1;
        };
        struct CopyParameters
        {
            Surface * source;
            uint32_t  sourceSubresourceId;
            Surface * dest;
            uint32_t  destSubresourceId;
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
            virtual void copy(const CopyParameters &) = 0;
        };
        virtual AutoRef<CommandList> createCommandList(const CommandListCreationParameters &) = 0;
        virtual void kickoff(CommandList &, uint64_t * fence = nullptr) = 0; ///< kick off command lists.
        virtual void wait(uint64_t fence) = 0; // insert a wait-for-fence into command list
        //@}

        /// GPU memory pool
        //@{
        struct MemPoolCreationParameters
        {
            uint64_t sizeInMB;
        };
        struct MemPool : public RefCounter
        {

        };
        virtual AutoRef<MemPool>  createMemoryPool(const MemPoolCreationParameters &) = 0;
        //@}

        /// GPU program compilation
        //@{
        enum ShadingLanguage
        {
            HLSL,
            GLSL,
        };
        struct ProgramSource
        {
            ShadingLanguage lang; ///< shading language
            bool            debugable = false; ///< set to true to generate shader binary with debug information.
            bool            optimized = true;  ///< set to true to generate optimized shader.
            const char *    vs = nullptr;
            const char *    ts = nullptr;
            const char *    ds = nullptr;
            const char *    gs = nullptr;
            const char *    ps = nullptr;
            const char *    cs = nullptr;
        };
        virtual std::vector<uint8> compileProgram(const ProgramSource &) = 0;
        //@}

        enum class SurfaceDimension
        {
            BUFFER,
            TEX1D,
            TEX2D,
            TEX3D,
        };

        union SurfaceUsage
        {
            uint32_t u32 = 0;
            struct
            {
                uint32_t sr : 1; // shader resource
                uint32_t ua : 1; // unordered access
                uint32_t rt : 1; // render target
                uint32_t ds : 1; // depth stencil
            };
        };

        /// GPU surface
        //@{
        struct SurfaceCreationParameters
        {
            union
            {
                SurfaceDimension dim;
                SurfaceUsage usage;
                struct TextureDesc
                {
                    ColorFormat f;
                    uint32_t w, h, d, a, m; // width, height, depth, layers, mipmaps.
                } t;
                struct BufferDesc
                {
                    uint32_t bytes;
                } b;
            };
            MemPool * pool = nullptr;
            uint64_t  offset = 0; // ignored if pool is null.
        };
        /// this could be a texture or a buffer.
        struct Surface : public RefCounter
        {
            /// For immediate update, no dealy, no async. do hazard tracking yourself.
            virtual void * getPersistentPointer(uint32_t subResourceId, uint32_t * pRowPitch, uint32_t * pSlicePitch) = 0;
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
} // end of namespace gfx
} // end of namespace GN
