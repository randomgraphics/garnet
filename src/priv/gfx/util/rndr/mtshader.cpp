#include "pch.h"
#include "mtshader.h"
#include "mtrndrCmd.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.rndr.mtshader");

using namespace GN::gfx;

// *****************************************************************************
// Local types and data
// *****************************************************************************

struct GpuProgramInitParam
{
    GpuProgram                    * gp;
    size_t                          count;
    const GpuProgramParameterDesc * params;
};

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadGpuProgram::init( GpuProgram * gp )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadGpuProgram, () );

    if( NULL == gp ) return failure();

    // store GPU program pointer
    mGpuProgram = gp;

    // get parameter informations
    volatile GpuProgramInitParam gpip;
    gpip.gp = mGpuProgram;
    mRenderer.postCommand1( CMD_GPU_PROGRAM_INIT, &gpip );
    mRenderer.waitForIdle();
    mNumParams = gpip.count;
    mParams    = gpip.params;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpuProgram::quit()
{
    GN_GUARD;

    if( mGpuProgram )
    {
        mRenderer.postCommand1( CMD_GPU_PROGRAM_DESTROY, mGpuProgram );
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from GpuProgram
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpuProgram::setParameter(
    size_t       index,
    const void * value,
    size_t       length )
{
    if( 0 == length )
    {
        if( index >= mNumParams )
        {
            GN_ERROR(sLogger)( "invalid parameter index : %d", index );
            return;
        }

        const GpuProgramParameterDesc & pd = mParams[index];

        length = pd.length;
    }

    void * tmpbuf = heapAlloc( length );
    if( NULL == tmpbuf )
    {
        GN_ERROR(sLogger)( "fail to allocate temporary buffer." );
        return;
    }
    memcpy( tmpbuf, value, length );
    mRenderer.postCommand4( CMD_GPU_PROGRAM_SET_PARAMETER, mGpuProgram, index, tmpbuf, length );
    mRenderer.waitForIdle();
}

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    void func_GPU_PROGRAM_DESTROY( Renderer &, void * p, size_t )
    {
        GpuProgram & gp = **(GpuProgram**)p;
        gp.decref();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GPU_PROGRAM_INIT( Renderer &, void * p, size_t )
    {
        GpuProgramInitParam * gpip = *(GpuProgramInitParam**)p;
        gpip->count  = gpip->gp->getNumParameters();
        gpip->params = gpip->gp->getParameters();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GPU_PROGRAM_SET_PARAMETER( Renderer &, void * p, size_t )
    {
        struct GpuProgramSetParam
        {
            GpuProgram * gp;
            size_t       index;
            void       * value;
            size_t       length;
        };

        GpuProgramSetParam * gpsp = (GpuProgramSetParam*)p;

        gpsp->gp->setParameter( gpsp->index, gpsp->value, gpsp->length );

        heapFree( gpsp->value );
    }
}}
