#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.Mesh");

struct VertexFormatProperties
{
    /// minimal strides for each stream
    size_t minStrides[RendererContext::MAX_VERTEX_BUFFERS];

    /// true means that stream is referenced by the vertex format.
    bool used[RendererContext::MAX_VERTEX_BUFFERS];

    /// analyze vertex format
    bool analyze( const VertexFormat & vf )
    {
        memset( this, 0, sizeof(*this) );

        for( size_t i = 0; i < vf.numElements; ++i )
        {
            const VertexElement & e = vf.elements[i];

            if( e.stream > RendererContext::MAX_VERTEX_BUFFERS )
            {
                GN_ERROR(sLogger)( "Invalid stream ID: %d", e.stream );
                return false;
            }

            used[e.stream] = true;

            size_t currentStride = minStrides[e.stream];

            size_t newStride = e.offset + e.format.getBytesPerBlock();

            if( newStride > currentStride ) minStrides[e.stream] = newStride;
        }

        return true;
    }
};


//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// Mesh class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Mesh::init( const MeshDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::Mesh, () );

    // analyze vertex format
    VertexFormatProperties vfp;
    if( !vfp.analyze( desc.vtxfmt ) ) return failure();

    // initialize vertex buffers
    for( size_t i = 0; i < RendererContext::MAX_VERTEX_BUFFERS; ++i )
    {
        if( !vfp.used[i] ) continue; // ignore unused vertex buffer

        // determine stride
        size_t stride;
        if( 0 == desc.strides[i] )
        {
            stride = vfp.minStrides[i];
        }
        else if( desc.strides[i] < vfp.minStrides[i] )
        {
            GN_ERROR(sLogger)( "stride for stream %u is too small.", i );
            return failure();
        }
        else
        {
            stride = desc.strides[i];
        }

        size_t vbsize = stride * desc.numvtx;

        // create GPU vertex buffer
        VtxBufDesc vbd = { vbsize, desc.dynavb, false };
        AutoRef<VtxBuf> & vb = mVtxBufs[i].gpudata;
        vb.attach( mRenderer.createVtxBuf( vbd ) );
        if( NULL == vb ) return failure();

        // copy vertices to vertex buffer
        const void * vertices = desc.vertices[i];
        if( vertices ) vb->update( 0, 0, vertices );
    }

    // initialize index buffer
    if( desc.numidx > 0 )
    {
        IdxBufDesc ibd = { desc.numidx, desc.idx32, desc.dynaib, false };
        mIdxBuf.gpudata.attach( mRenderer.createIdxBuf( ibd ) );
        if( NULL == mIdxBuf.gpudata ) return failure();

        if( desc.indices ) mIdxBuf.gpudata->update( 0, 0, desc.indices );
    }

    // success
    mDesc = desc;
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Mesh::quit()
{
    GN_GUARD;

    for( size_t i = 0; i < GN_ARRAY_COUNT(mVtxBufs); ++i )
    {
        mVtxBufs[i].gpudata.clear();
    }

    mIdxBuf.gpudata.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::Mesh::applyToDrawable( Drawable & drawable, const MeshSubset * subset ) const
{
    // vertex format
    drawable.rc.vtxfmt = mDesc.vtxfmt;

    // verex buffers
    GN_CASSERT( GN_ARRAY_COUNT(drawable.rc.vtxbufs) == GN_ARRAY_COUNT(mVtxBufs) );
    for( size_t i = 0; i < GN_ARRAY_COUNT(drawable.rc.vtxbufs); ++i )
    {
        drawable.rc.vtxbufs[i] = mVtxBufs[i].gpudata;
    }

    // index buffers
    drawable.rc.idxbuf = mIdxBuf.gpudata;

    MeshSubset fullmesh;
    if( NULL == subset )
    {
        fullmesh.startvtx = 0;
        fullmesh.numvtx = mDesc.numvtx;
        fullmesh.startidx = 0;
        fullmesh.numidx = mDesc.numidx;
        subset = &fullmesh;
    }
    else if( subset->startvtx >= mDesc.numvtx || (subset->startvtx+subset->numvtx) > mDesc.numvtx ||
             subset->startidx >= mDesc.numidx || (subset->startidx+subset->numidx) > mDesc.numidx )
    {
        GN_ERROR(sLogger)( "invalid mesh subset : out of range" );
    }

    // draw parameters
    drawable.prim      = mDesc.prim;
    drawable.startvtx  = subset->startvtx;
    drawable.numvtx    = subset->numvtx;
    drawable.startidx  = subset->startidx;
    drawable.numidx    = subset->numidx;
    drawable.basevtx   = 0;
}
