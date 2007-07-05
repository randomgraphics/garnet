#include "pch.h"
#include "d3d9QuadKernel.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9QuadKernel");

// *****************************************************************************
// D3D9QuadStream
// *****************************************************************************

GN::gfx::D3D9QuadStream::D3D9QuadStream( D3D9GraphicsSystem & gs )
    : D3D9UnstableResource( gs )
{
    clear();

    mDesc.format.attribs[0].set( "POSITION",  0, FMT_FLOAT3 );
    mDesc.format.attribs[1].set( "COLOR"   , 12, FMT_RGBA32 );
    mDesc.format.attribs[2].set( "TEXCOORD", 16, FMT_FLOAT2 );
    mDesc.format.count = 3;
    mDesc.format.stride = 32;
    mDesc.maxBytes = MAX_QUADS * sizeof(QuadVertex);
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9QuadStream::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9QuadStream, () );

    // Do custom init here

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadStream::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadStream::push( const void * data, size_t bytes )
{
    GN_GUARD_SLOW;

    // check parameter
    if( 0 == data )
    {
        GN_ERROR(sLogger)( "null data pointer." );
        return;
    }
    if( bytes > freeBytes() )
    {
        GN_ERROR(sLogger)( "no enough space." );
        return;
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// D3D9QuadKernel
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9QuadKernel::D3D9QuadKernel( D3D9GraphicsSystem & gs )
    : D3D9Kernel(gs)
    , mTarget0(gs)
    , mDepth(gs)
    , mTexture(gs,0)
    , mQuads(gs)
{
    // setup ports
    addPortRef( "TARGET0", &mTarget0 );
    addPortRef( "DEPTH", &mDepth );

    // setup streams
    addStreamRef( "QUADS", &mQuads );

    // setup parameters
    mOption = addParameter( "OPTION", KERNEL_PARAMETER_TYPE_INT, 1 );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9QuadKernel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9QuadKernel, () );

    // create shader

    // create decl

    // create idxbuf

    // initialize quads
    if( !mQuads.init() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadKernel::quit()
{
    GN_GUARD;

    mVs.clear();
    mPsTex.clear();
    mPsClr.clear();
    mDecl.clear();
    mIdxBuf.clear();
    mQuads.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadKernel::render( const KernelParameterSet &, KernelBinding )
{
}
