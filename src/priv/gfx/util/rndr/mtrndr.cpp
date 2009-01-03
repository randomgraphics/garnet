#include "pch.h"
#include "mtrndr.h"
#include "mtshader.h"
#include "mttexture.h"
#include "mtrndrCmd.h"

#pragma warning( disable : 4100 ) // unused parameters
#pragma warning( disable : 4715 ) // no return value


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
    ThreadProcedure proc = makeDelegate( this, &GN::gfx::MultiThreadRenderer::threadProc );
    mThread = createThread( proc, (void*)&ro, TP_NORMAL );
    if( NULL == mThread ) return failure();

    // wait for the renderer creation
    while( 2 == mRendererCreationStatus ) sleepCurrentThread(0);
    if( 1 != mRendererCreationStatus ) return failure();

    // initialize front end variables
    postCommand1( CMD_GET_RENDERER_OPTIONS, &mRendererOptions );
    postCommand1( CMD_GET_DISP_DESC, &mDispDesc );
    postCommand1( CMD_GET_D3D_DEVICE, &mD3DDevice );
    postCommand1( CMD_GET_OGL_RC, &mOGLRC );
    postCommand1( CMD_GET_CAPS, &mCaps );
    waitForIdle();

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
void GN::gfx::MultiThreadRenderer::waitForIdle()
{
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::postCommand( UInt32 cmd, const void * data, size_t length )
{
}

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
    mRenderer = createSingleThreadRenderer( *ro );
    if( NULL == mRenderer )
    {
        mRendererCreationStatus = 0;
        return 0;
    }
    mRendererCreationStatus = 1;

    // enter command loop

    // success
    return 0;
}

// *****************************************************************************
// Rendering function wrappers
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::checkTextureFormatSupport( ColorFormat format, TextureUsages usages ) const
{
    MultiThreadRenderer * nonConstPtr = const_cast<GN::gfx::MultiThreadRenderer*>(this);
    bool result;
    nonConstPtr->postCommand( CMD_CHECK_TEXTURE_FORMAT_SUPPORT, &result, sizeof(result) );
    nonConstPtr->waitForIdle();
    return result;
}

//
//
// -----------------------------------------------------------------------------
ColorFormat GN::gfx::MultiThreadRenderer::getDefaultTextureFormat( TextureUsages usages ) const
{
    MultiThreadRenderer * nonConstPtr = const_cast<GN::gfx::MultiThreadRenderer*>(this);
    ColorFormat result;
    nonConstPtr->postCommand( CMD_GET_DEFAULT_TEXTURE_FORMAT, &result, sizeof(result) );
    nonConstPtr->waitForIdle();
    return result;
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
