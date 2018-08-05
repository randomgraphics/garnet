#ifndef __GN_GFX2_GFX2_H
#define __GN_GFX2_GFX2_H
// *****************************************************************************
/// \file
/// \brief   Graphics System Version 2
/// \author  chenlee(2013.03.23)
// *****************************************************************************

#include "GNbase.h"
#include <vector>

namespace GN {
///
/// namespace for GFX module
///
namespace gfx {
    enum class Primitive {
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
    };

    struct DrawParameters {
        const GpuPipeline * pipeline;
        uint32              indexcount;
        uint32              basevertex;
        uint32              baseindex;
        uint32              primitive;
    };

    enum class ShadingLanguage {
        HLSL,
        GLSL,
    };

    struct GpuProgramSource {
        ShadingLanguage lang;      ///< shading language
        bool            debugable; ///< set to true to generate shader binary with debug information.
        bool            optimized; ///< set to true to generate optimized shader.
        const char *    vs;
        const char *    ts;
        const char *    ds;
        const char *    gs;
        const char *    ps;
        const char *    cs;
    };

    struct CommandListCreationParameters {

    };

    struct GpuResourceCreationParameters {
    };

    struct RenderStateCreationParameters {

    };

    struct GpuQueryCreationParameters {

    };

    /// this could be a texture or a buffer.
    class GpuResource : public RefCounter {
        /// For immediate update, no dealy, no async. do hazard tracking yourself.
        virtual void * getDirectPointer(uint32 subResourceId, uint32 * pRowPitch, uint32 * pSlicePitch) = 0;
    };

    class GpuResourceTable : public RefCounter {
    };

    class GpuProgram : public RefCounter {
    };

    class GpuPipeline : public RefCounter {
    };

    class RenderState : public RefCounter {
    };

    class GpuQuery : public RefCounter {

    };

    struct CopyParameters {
        GpuResource * source;
        GpuResource * dest;
        uint32        sourceSubresourceId;
        uint32        destSubresourceId;
    };

    struct PresentParameters {

    };

    class CommandList : public RefCounter {
    public:
        virtual void   copy(const CopyParameters &) = 0;
        virtual void   draw(const DrawParameters *) = 0;
        virtual uint64 mark() = 0; // insert a new fence into command list, returns fence id.
        virtual void   wait(uint64 fence) = 0; // insert a wait-for-fence into command list
    };

    class Gpu : public RefCounter {
    public:
        virtual std::vector<uint8>   compileGpuProgram(const GpuProgramSource &) = 0;
        virtual AutoRef<CommandList> createCommandList(const CommandListCreationParameters &) = 0;
        virtual AutoRef<GpuResource> createResource(const GpuResourceCreationParameters &) = 0;
        virtual AutoRef<RenderState> createRenderState(const RenderStateCreationParameters &) = 0;
        virtual AutoRef<GpuPipeline> createPipeline(GpuProgram & program, RenderState & state) = 0;
        virtual AutoRef<GpuProgram>  createProgram(const void * compiledBlob, size_t sizeInBytes) = 0;
        virtual AutoRef<GpuQuery>    createQuery(const GpuQueryCreationParameters &) = 0;
        virtual void                 kickoff(CommandList &) = 0; ///< kick off command list.
        virtual void                 present(const PresentParameters &) = 0;
        virtual void                 wait(uint64 fence) = 0; ///< block caller thread until the fence completes.
        virtual bool                 testFence(uint64 fence) = 0;
    };

    AutoRef<Gpu> createGpu(...);
} // end of namespace gfx2
} // end of namespace GN

// *****************************************************************************
//                         END of GNgfx2.h
// *****************************************************************************
#endif
