#include "pch.h"
#include "oglRenderer.h"
#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"

// *****************************************************************************
// local functions
// *****************************************************************************

struct EqualFormat
{
    const GN::gfx::VtxFmtDesc & format;

    EqualFormat( const GN::gfx::VtxFmtDesc & f ) : format(f) {}

    bool operator()( void * p ) const
    {
        GN_ASSERT( p );
        return format == ((GN::gfx::OGLVtxFmt*)p)->getFormat();
    }
};

// *****************************************************************************
// initialize
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::bufferClear()
{
    GN_GUARD;

    // delete all vertex buffer binding objects
    uint32_t h = mVtxFmts.first();
    while( h )
    {
        OGLVtxFmt * p = (OGLVtxFmt*)mVtxFmts[h];
        GN_ASSERT( p );
        delete p;
        h = mVtxFmts.next( h );
    }
    mVtxFmts.clear();

    mCurrentIdxBuf.clear();

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::OGLRenderer::createVtxFmt( const VtxFmtDesc & format )
{
    GN_GUARD;

    uint32_t h = mVtxFmts.findIf( EqualFormat(format) );

    if( 0 == h )
    {
        // create new vertex binding object
        AutoObjPtr<OGLVtxFmt> p( new OGLVtxFmt(*this) );
        if( !p->init( format ) ) return 0;
        h = mVtxFmts.add( p );
        p.detach();
    }

    // success
    return h;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf *
GN::gfx::OGLRenderer::createVtxBuf(
    size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader )
{
    GN_GUARD;

    if( GLEW_ARB_vertex_buffer_object )
    {
        AutoRef<OGLVtxBufVBO> p( new OGLVtxBufVBO(*this) );
        p->setLoader( loader );
        if( !p->init( bytes, dynamic, sysCopy ) ) return 0;
        return p.detach();
    }
    else
    {
        AutoRef<OGLVtxBufNormal> p( new OGLVtxBufNormal );
        p->setLoader( loader );
        if( !p->init( bytes, dynamic ) ) return 0;
        return p.detach();
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf *
GN::gfx::OGLRenderer::createIdxBuf(
    size_t numIdx, bool dynamic, bool /*sysCopy*/, const IdxBufLoader & loader )
{
    GN_GUARD;

    AutoRef<OGLIdxBuf> p( new OGLIdxBuf );
    p->setLoader( loader );
    if( !p->init( numIdx, dynamic ) ) return 0;

    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::bindVtxFmt( uint32_t handle )
{
    GN_GUARD;

    if( !mVtxFmts.validHandle(handle) )
    {
        GN_ERROR( "invalid vertex binding handle : %d", handle );
        return;
    }

    mCurrentDrawState.bindVtxFmt( handle );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::bindVtxBufs( const VtxBuf * const buffers[], size_t start, size_t count )
{
    GN_GUARD_SLOW;

    if( start >= MAX_VERTEX_STREAMS )
    {
        GN_ERROR( "Starting stream index is too large!" );
        return;
    }
    if( start + count > MAX_VERTEX_STREAMS )
    {
        GN_ERROR( "start+count can't exceed MAX_VERTEX_STREAMS!" );
        return;
    }

    for( size_t i = start; i < (start+count); ++i )
    {
        mCurrentDrawState.bindVtxBuf( i, buffers[i], 0 );
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::bindVtxBuf( size_t index, const VtxBuf * buffer, size_t stride )
{
    GN_GUARD_SLOW;

    if( index >= MAX_VERTEX_STREAMS )
    {
        GN_ERROR( "Stream index is too large!" );
        return;
    }

    mCurrentDrawState.bindVtxBuf( index, buffer, stride );

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::applyVtxFmt()
{
    GN_GUARD_SLOW;

    if( mVtxFmts.validHandle( mCurrentDrawState.vtxFmt ) )
    {
        OGLVtxFmt * p = (OGLVtxFmt *)mVtxFmts[mCurrentDrawState.vtxFmt];
        GN_ASSERT( p );
        p->bind();
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::applyVtxBufState( size_t startVtx )
{
    GN_GUARD_SLOW;

    if( mVtxFmts.validHandle( mCurrentDrawState.vtxFmt ) )
    {
        OGLVtxFmt * p = (OGLVtxFmt *)mVtxFmts[mCurrentDrawState.vtxFmt];

        GN_ASSERT( p );

        const VtxFmtDesc & vtxFmt = p->getFormat();

        for( size_t i = 0; i < vtxFmt.numStreams; ++i )
        {
            const OGLDrawState::VtxBufDesc & vbd = mCurrentDrawState.vtxBufs[i];

            p->bindBuffer(
                i,
                vbd.buf.empty() ? 0 : safeCast<const OGLBasicVtxBuf*>( vbd.buf.get() )->getVtxData(),
                startVtx,
                vbd.stride ? vbd.stride : vtxFmt.streams[i].stride );
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::setVtxBufUp( const void * data, size_t stride )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mVtxFmts.validHandle( mCurrentDrawState.vtxFmt ) );

    OGLVtxFmt * p = (OGLVtxFmt *)mVtxFmts[mCurrentDrawState.vtxFmt];

    GN_ASSERT( p );

    GN_ASSERT( 1 == p->getFormat().numStreams );

    p->bindBuffer(
        0, // stream index
        (const uint8_t* )data,
        0, // startVtx,
        stride );

    GN_UNGUARD_SLOW;
}

