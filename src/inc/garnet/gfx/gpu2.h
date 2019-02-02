namespace GN
{
///
/// namespace for GFX module
///
namespace gfx
{
    enum class ShadingLanguage
    {
        HLSL,
        GLSL,
    };

    struct GpuProgramSource
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

    struct CommandListCreationParameters
    {

    };

    class GpuMemPool : public RefCounter
    {

    };

    struct GpuResourceCreationParameters
    {
        union
        {
            struct TextureDesc
            {

            };

            struct BufferDesc
            {

            };
        };
        GpuMemPool * pool = nullptr;
        uint64_t     offset = 0; // ignored if pool is null.
    };

    struct GpuPipelineCreationParameters
    {
        const uint8_t * compiledGpuProgramBinary;
        uint64_t        compiledGpuProgramSizeInBytes;
        const char *    states; // render states defined in json format.
    };

    struct GpuQueryCreationParameters
    {
    };

    /// this could be a texture or a buffer.
    class GpuResource : public RefCounter
    {
        /// For immediate update, no dealy, no async. do hazard tracking yourself.
        virtual void * getDirectPointer(uint32_t subResourceId, uint32_t * pRowPitch, uint32_t * pSlicePitch) = 0;
    };

    class GpuPipeline : public RefCounter
    {
    };

    class GpuQuery : public RefCounter
    {
    };

    struct CopyParameters
    {
        GpuResource * source;
        uint32_t      sourceSubresourceId;
        GpuResource * dest;
        uint32_t      destSubresourceId;
    };

    struct DrawParameters
    {
        const GpuPipeline * pipeline;
        uint32_t            indexcount;
        uint32_t            basevertex;
        uint32_t            baseindex;
        uint32_t            primitive;
    };

    struct PresentParameters
    {

    };

    class CommandList : public RefCounter
    {
    public:
        virtual void     copy(const CopyParameters &) = 0;
        virtual void     draw(const DrawParameters *) = 0;
        virtual uint64_t mark() = 0; // insert a new fence into command list, returns fence id.
        virtual void     wait(uint64_t fence) = 0; // insert a wait-for-fence into command list
    };

    class Gpu2 : public RefCounter
    {
    public:
        virtual AutoRef<GpuMemPool>  createMemoryPool(uint64_t sizeInMB) = 0;
        virtual std::vector<uint8>   compileGpuProgram(const GpuProgramSource &) = 0;
        virtual AutoRef<CommandList> createCommandList(const CommandListCreationParameters &) = 0;
        virtual AutoRef<GpuResource> createResource(const GpuResourceCreationParameters &) = 0;
        virtual AutoRef<GpuPipeline> createPipeline(const GpuPipelineCreationParameters &) = 0;
        virtual AutoRef<GpuQuery>    createQuery(const GpuQueryCreationParameters &) = 0;
        virtual void                 kickoff(CommandList &) = 0; ///< kick off command list.
        virtual void                 present(const PresentParameters &) = 0;
        virtual void                 wait(uint64_t fence) = 0; ///< block caller thread until the fence completes.
        virtual bool                 testFence(uint64_t fence) = 0;
    };

    AutoRef<Gpu2> createGpu2(...);
} // end of namespace gfx
} // end of namespace GN
