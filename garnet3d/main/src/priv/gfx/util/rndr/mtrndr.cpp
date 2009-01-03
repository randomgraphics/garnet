#include "pch.h"
#include "mtrndr.h"
#include "mtrndrCmd.h"

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// Local classes and data types
// *****************************************************************************

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::init( const RendererOptions & ro )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadRenderer, () );

    // create thread
    mRendererCreationStatus = 2;
    Delegate1<UInt32,void*> proc( this, &GN::gfx::MultiThreadRenderer::threadProc );
    mThread = createThread( proc, &ro, TP_NORMAL );
    if( NULL == mThread ) return failure();

    // wait for the renderer creation
    while( 2 == mRendererCreationStatus ) sleepCurrentThread(0);
    if( 1 != mRendererCreationStatus ) return failure();

    // setup front variables
    mRendererOptions = ro;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::quit()
{
    GN_GUARD;

    if( mThread )
    {
        postCommand( CMD_DESTROY, 0, 0 );
        KickOff();
        waitForIdle();
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
UInt32 GN::gfx::MultiThreadRenderer::threadProc( void * param )
{
    // create the renderer instance
    GN_ASSERT( 2 == mRendererCreationStatus );
    const RendererOptions * ro = (const RendererOptions*)param;
    mRenderer = createSingleThreadRenderer( ro );
    if( NULL == mRenderer )
    {
        mRendererCreationStatus = 0;
        return -1U;
    }
    mRendererCreationStatus = 1;

    // enter command loop

    // success
    return 0;
}

// *****************************************************************************
// Rendering function wrappers
// *****************************************************************************

#pragma warning( disable : 4100 ) // unused parameters
#pragma warning( disable : 4715 ) // no return value

//
//
// -----------------------------------------------------------------------------
const RendererOptions & GN::gfx::MultiThreadRenderer::getOptions() const
{
    postCommand( CMD_GET_RENDERER_OPTIONS, &mRendererOptions, sizeof(mRendererOptions) );
    waitForIdle();
    return mRendererOptions;
}

//
//
// -----------------------------------------------------------------------------
const DispDesc & GN::gfx::MultiThreadRenderer::getDispDesc() const
{
    postCommand( CMD_GET_DISP_DESC, &mDispDecs, sizeof(mDispDesc) );
    waitForIdle();
    return mDispDesc;
}

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::MultiThreadRenderer::getD3DDevice() const
{
    void * device;
    postCommand( CMD_GET_D3D_DEVICE, &device, sizeof(device) );
    waitForIdle();
    return device;
}

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::MultiThreadRenderer::getOGLRC() const
{
    void * rc;
    postCommand( CMD_GET_OGL_RC, &rc, sizeof(rc) );
    waitForIdle();
    return rc;
}


//
//
// -----------------------------------------------------------------------------
const RendererCaps & GN::gfx::MultiThreadRenderer::getCaps() const
{
    postCommand( CMD_GET_CAPS, &mCaps, sizeof(mCaps) );
    waitForIdle();
    return mCaps;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::checkTextureFormatSupport( ColorFormat format, TextureUsages usages ) const
{
    postCommand( CMD_CHECK_TEXTURE_FORMAT_SUPPORT, 0, 0 );
    waitForIdle();
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
ColorFormat GN::gfx::MultiThreadRenderer::getDefaultTextureFormat( TextureUsages usages ) const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
CompiledGpuProgram * GN::gfx::MultiThreadRenderer::compileGpuProgram( const GpuProgramDesc & desc )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
GpuProgram * GN::gfx::MultiThreadRenderer::createGpuProgram( const void * compiledGpuProgramBinary, size_t length )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
Texture * GN::gfx::MultiThreadRenderer::createTexture( const TextureDesc & desc )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
VtxBuf * GN::gfx::MultiThreadRenderer::createVtxBuf( const VtxBufDesc & )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
IdxBuf * GN::gfx::MultiThreadRenderer::createIdxBuf( const IdxBufDesc & desc )
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::bindContext( const RendererContext & )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::rebindContext()
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
const RendererContext & GN::gfx::MultiThreadRenderer::getContext() const
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::present()
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::clearScreen( const Vector4f & c,
                          float            z,
                          UInt8            s,
                          BitFields        flags )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::drawIndexed( PrimitiveType prim,
                          size_t        numprim,
                          size_t        startvtx,
                          size_t        minvtxidx,
                          size_t        numvtx,
                          size_t        startidx )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::draw( PrimitiveType prim,
                   size_t        numprim,
                   size_t        startvtx )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::drawIndexedUp(
                     PrimitiveType  prim,
                     size_t         numprim,
                     size_t         numvtx,
                     const void *   vertexData,
                     size_t         strideInBytes,
                     const UInt16 * indexData )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::drawUp(
    PrimitiveType prim,
    size_t        numprim,
    const void *  vertexData,
    size_t        strideInBytes )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::MultiThreadRenderer::drawLines(
    BitFields         options,
    const void *      positions,
    size_t            stride,
    size_t            count,
    UInt32            rgba,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::enableParameterCheck( bool enable )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::dumpNextFrame( size_t startBatchIndex, size_t numBatches )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::setUserData( const Guid & id, const void * data, size_t length )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
const void * GN::gfx::MultiThreadRenderer::getUserData( const Guid & id, size_t * length ) const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::hasUserData( const Guid & id ) const
{
    GN_UNIMPL();
}
