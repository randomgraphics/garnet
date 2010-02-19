#include "pch.h"
#include "mtshader.h"
#include "mtgpuCmd.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.util.gpu.mtshader");

using namespace GN::gfx;

// *****************************************************************************
// Local types and data
// *****************************************************************************

struct GpuProgramInitParam
{
    GpuProgram                    * gp;
    const GpuProgramParameterDesc * params;
};

struct UniformInitParam
{
    Uniform * uniform;
    size_t    size;
};

struct UniformUpdateParam
{
    Uniform * uniform;
    size_t    offset;
    size_t    length;
    void    * data;
};

// *****************************************************************************
// MultiThreadUniform
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadUniform::Init( Uniform * uni )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadUniform, () );

    GN_ASSERT( uni );

    mUniform = uni;
    mSize    = uni->size();
    mFrontEndData = (UInt8*)HeapAlloc(mSize);
    if( NULL == mFrontEndData ) return Failure();

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadUniform::Quit()
{
    GN_GUARD;

    if( mUniform )
    {
        mGpu.postCommand1( CMD_UNIFORM_DESTROY, mUniform );
        mUniform = NULL;
    }

    SafeHeapFree( mFrontEndData );

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadUniform::update( size_t offset, size_t length, const void * data )
{
    if( offset >= mSize || (offset+length) > mSize )
    {
        GN_ERROR(GetLogger("GN.gfx.Uniform"))( "Out of range!" );
        return;
    }
    if( NULL == data )
    {
        GN_ERROR(GetLogger("GN.gfx.Uniform"))( "Null pointer!" );
        return;
    }

    memcpy( mFrontEndData + offset, data, length );

    size_t cmdsize = sizeof(UniformUpdateParam) + length;
    if( cmdsize <= mGpu.getRingBufferSize() )
    {
        UniformUpdateParam * p = (UniformUpdateParam*)mGpu.beginPostCommand( CMD_UNIFORM_UPDATE, cmdsize );
        if( NULL == p ) return;

        p->uniform = mUniform;
        p->offset  = offset;
        p->length  = length;
        p->data    = p + 1;
        memcpy( p->data, data, length );

        mGpu.endPostCommand();
    }
    else
    {
        GN_UNIMPL();
    }
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadGpuProgram::Init( GpuProgram * gp )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadGpuProgram, () );

    GN_ASSERT( gp );

    // store GPU program pointer
    mGpuProgram = gp;

    // get parameter informations
    volatile GpuProgramInitParam gpip;
    gpip.gp = mGpuProgram;
    mGpu.postCommand1( CMD_GPU_PROGRAM_INIT, &gpip );
    mGpu.waitForIdle();
    mParamDesc = gpip.params;

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpuProgram::Quit()
{
    GN_GUARD;

    if( mGpuProgram )
    {
        mGpu.postCommand1( CMD_GPU_PROGRAM_DESTROY, mGpuProgram );
    }

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    void func_GPU_PROGRAM_DESTROY( Gpu &, void * p, size_t )
    {
        GpuProgram & gp = **(GpuProgram**)p;
        gp.decref();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GPU_PROGRAM_INIT( Gpu &, void * p, size_t )
    {
        GpuProgramInitParam * gpip = *(GpuProgramInitParam**)p;
        gpip->params = &gpip->gp->getParameterDesc();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_UNIFORM_DESTROY( Gpu &, void * p, size_t )
    {
        Uniform & u = **(Uniform**)p;
        u.decref();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_UNIFORM_UPDATE( Gpu &, void * p, size_t )
    {
        const UniformUpdateParam & uup = *(const UniformUpdateParam*)p;
        uup.uniform->update( uup.offset, uup.length, uup.data );
    }
}}
