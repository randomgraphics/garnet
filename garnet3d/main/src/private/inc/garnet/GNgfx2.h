#ifndef __GN_GFX2_GFX2_H
#define __GN_GFX2_GFX2_H
// *****************************************************************************
/// \file
/// \brief   Graphics System Version 2
/// \author  chenlee(2013.03.23)
// *****************************************************************************

#include "GNbase.h"

namespace GN
{
///
/// namespace for GFX module
///
namespace gfx2
{
class GpuBlob : public RefCounter
{
};

/// this could be a texture or a buffer.
class GpuResource : public RefCounter
{
    /// immediate update, no dealy, no async. do hazard tracking yourself.
    virtual void * getDirectPointer(UINT subResourceId, UINT * pRowPitch, UINT * pSlicePitch) = 0;
};

class GpuResourceTable : public RefCounter
{
};

class GpuProgram : public RefCounter
{
};

class GpuPipeline : public RefCounter
{
};

class GpuRenderState : public RefCounter
{
};

struct GpuDrawParameters
{
};

class GpuCommandList : public RefCounter
{
public:
    virtual void   update(GpuResource * resource, UINT subResourceId, ...) = 0;
    virtual void   copy(GpuResource * source, UINT sourceSubResId, GpuResource * target, UINT targetSubResId, ...) = 0;
    virtual void   bind(GpuResourceTable * resources) = 0;
    virtual void   draw(GpuPipeline * pipeline, const GpuDrawParameters * dp) = 0;
    virtual uint64 mark() = 0; // insert a new fence into command list, returns fence id.
    virtual void   wait(uint64 fence) = 0; // insert a wait-for-fence into command list
};

class Gpu : public RefCounter
{
public:
    virtual AutoRef<GpuBlob>        compileProgram(...) = 0;
    virtual AutoRef<GpuCommandList> createCommandList(...) = 0;
    virtual AutoRef<GpuResource>    createResource(...) = 0;
    virtual AutoRef<GpuRenderState> createRenderState(...) = 0;
    virtual AutoRef<GpuPipeline>    createPipeline(GpuProgram * program, GpuRenderState * state) = 0;
    virtual AutoRef<GpuProgram>     createProgram(const void * compiledBlob, size_t sizeInBytes) = 0;
    virtual void                    submitCommandList(...) = 0;
    virtual int                     present(...) = 0; // returns a fence that marks completion of the present.
    virtual void                    wait(int fence) = 0; // block caller thread until the fence completes.
};

AutoRef<Gpu> createGpu(...);


} // end of namespace gfx2
} // end of namespace GN

// *****************************************************************************
//                         END of GNgfx2.h
// *****************************************************************************
#endif
