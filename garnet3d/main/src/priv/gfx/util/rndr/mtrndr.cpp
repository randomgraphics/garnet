#include "pch.h"
#include "mtrndr.h"

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::init( const RendererOptions & )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadRenderer, () );

    // Do custom init here

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

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
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
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
const DispDesc & GN::gfx::MultiThreadRenderer::getDispDesc() const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::MultiThreadRenderer::getD3DDevice() const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::MultiThreadRenderer::getOGLRC() const
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
const RendererCaps & GN::gfx::MultiThreadRenderer::getCaps() const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::checkTextureFormatSupport( ColorFormat format, TextureUsages usages ) const
{
    GN_UNIMPL();
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
