#include "pch.h"
#include "d3dRenderer.h"
#include "d3dVtxBuf.h"

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3DRenderer::createVtxBuf(
    size_t numVtx, size_t stride, ResourceUsage usage, bool sysCopy )
{
    GN_GUARD;

    AutoRef<D3DVtxBuf> buf( new D3DVtxBuf(*this) );

    if( !buf->init( numVtx, stride, usage, sysCopy ) ) return 0;

    return buf.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3DRenderer::createIdxBuf(
    size_t numIdx, ResourceUsage usage, bool sysCopy )
{
    //GN_GUARD;

    GN_UNUSED_PARAM(numIdx);
    GN_UNUSED_PARAM(usage);
    GN_UNUSED_PARAM(sysCopy);

    GN_UNIMPL();
    return 0;

    //GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::bindVtxBufs( const VtxBuf * const buffers[], uint32_t start, uint32_t count )
{
    GN_GUARD_SLOW;

    uint32_t stage = start;

    for( uint32_t i = 0; i < count; ++i, ++stage )
    {
        if( buffers[i] )
        {
            GN_DX_CHECK( mDevice->SetStreamSource(
                stage,
                safeCast<const D3DVtxBuf*>(buffers[i])->getD3DVb(),
                0,
                buffers[i]->getStride() ) );
        }
        else
        {
            GN_DX_CHECK( mDevice->SetStreamSource( stage, 0, 0, 0 ) );
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::bindIdxBuf( const IdxBuf * )
{
    //GN_GUARD_SLOW;

    //GN_UNGUARD_SLOW;
}
