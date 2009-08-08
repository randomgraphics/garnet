#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.GpuMesh");

struct VertexFormatProperties
{
    /// minimal strides for each stream
    size_t minStrides[GpuContext::MAX_VERTEX_BUFFERS];

    /// true means that stream is referenced by the vertex format.
    bool used[GpuContext::MAX_VERTEX_BUFFERS];

    /// analyze vertex format
    bool analyze( const VertexFormat & vf )
    {
        memset( this, 0, sizeof(*this) );

        for( size_t i = 0; i < vf.numElements; ++i )
        {
            const VertexElement & e = vf.elements[i];

            if( e.stream > GpuContext::MAX_VERTEX_BUFFERS )
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
// GpuMesh class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::GpuMesh::init( const GpuMeshDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::GpuMesh, () );

    // analyze vertex format
    VertexFormatProperties vfp;
    if( !vfp.analyze( desc.vtxfmt ) ) return failure();

    // initialize vertex buffers
    for( size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i )
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
        VtxBufDesc vbd = { vbsize, desc.dynavb };
        AutoRef<VtxBuf> & vb = mVtxBufs[i].gpudata;
        vb.attach( mGpu.createVtxBuf( vbd ) );
        if( NULL == vb ) return failure();

        // copy vertices to vertex buffer
        const void * vertices = desc.vertices[i];
        if( vertices ) vb->update( 0, 0, vertices );

        mVtxBufs[i].stride = (UInt16)stride;
    }

    // initialize index buffer
    if( desc.numidx > 0 )
    {
        IdxBufDesc ibd = { desc.numidx, desc.idx32, desc.dynaib };
        mIdxBuf.gpudata.attach( mGpu.createIdxBuf( ibd ) );
        if( NULL == mIdxBuf.gpudata ) return failure();

        if( desc.indices ) mIdxBuf.gpudata->update( 0, 0, desc.indices );
    }

    // store descriptor, but clear data pointers
    mDesc = desc;
    memset( mDesc.vertices, 0, sizeof(mDesc.vertices) );
    mDesc.indices = NULL;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::GpuMesh::quit()
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
GN::gfx::GpuMesh::applyToDrawable( Drawable & drawable, const GpuMeshSubset * subset ) const
{
    // vertex format
    drawable.rc.vtxfmt = mDesc.vtxfmt;

    // verex buffers
    GN_CASSERT( GN_ARRAY_COUNT(drawable.rc.vtxbufs) == GN_ARRAY_COUNT(mVtxBufs) );
    for( size_t i = 0; i < GN_ARRAY_COUNT(drawable.rc.vtxbufs); ++i )
    {
        drawable.rc.vtxbufs[i].vtxbuf = mVtxBufs[i].gpudata;
        drawable.rc.vtxbufs[i].stride = mVtxBufs[i].stride;
        drawable.rc.vtxbufs[i].offset = 0;
    }

    // index buffers
    drawable.rc.idxbuf = mIdxBuf.gpudata;

    GpuMeshSubset fullmesh;
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
