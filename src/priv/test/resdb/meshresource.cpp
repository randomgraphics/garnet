#include "pch.h"
#include "meshresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

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
// GN::gfx::MeshResource::Impl - Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResource::Impl::init( const MeshResourceDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MeshResource::Impl, () );

    // analyze vertex format
    VertexFormatProperties vfp;
    if( !vfp.analyze( desc.vtxfmt ) ) return failure();

    Gpu & gpu = database().gpu();

    // initialize vertex buffers
    if( desc.numvtx > 0 )
    {
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

            size_t vbsize = desc.strides[i] * desc.numvtx;

            // create GPU vertex buffer
            VtxBufDesc vbdesc = { vbsize, desc.dynavb };
            mVtxBufs[i].gpudata.attach( gpu.createVtxBuf( vbdesc ) );
            if( NULL == mVtxBufs[i].gpudata ) return failure();

            // copy vertices to vertex buffer
            const void * vertices = desc.vertices[i];
            if( vertices ) mVtxBufs[i].gpudata->update( 0, 0, vertices );
        }
    }

    // initialize index buffer
    if( desc.numidx > 0 )
    {
        IdxBufDesc ibd = { desc.numidx, desc.idx32, desc.dynaib };
        mIdxBuf.gpudata.attach( gpu.createIdxBuf( ibd ) );
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
void GN::gfx::MeshResource::Impl::quit()
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

// *****************************************************************************
// GN::gfx::MeshResource::Impl - public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::MeshResource::Impl::applyToContext( GpuContext & context ) const
{
    // vertex format
    context.vtxfmt = mDesc.vtxfmt;

    // verex buffers
    GN_CASSERT( GN_ARRAY_COUNT(context.vtxbufs) == GN_ARRAY_COUNT(mVtxBufs) );
    for( size_t i = 0; i < GN_ARRAY_COUNT(context.vtxbufs); ++i )
    {
        context.vtxbufs[i].vtxbuf = mVtxBufs[i].gpudata;
        context.vtxbufs[i].stride = (UInt16)mDesc.strides[i];
        context.vtxbufs[i].offset = (UInt32)mDesc.offsets[i];
    }

    // index buffers
    context.idxbuf = mIdxBuf.gpudata;
}

// *****************************************************************************
// GN::gfx::MeshResource::Impl - private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------


// *****************************************************************************
// GN::gfx::MeshResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
class MeshResourceInternal : public MeshResource
{
    MeshResourceInternal( GpuResourceDatabase & db, GpuResourceHandle handle )
        : MeshResource( db, handle )
    {
    }

    bool init( const void * parameters )
    {
        if( NULL == parameters )
        {
            GN_ERROR(sLogger)( "Null parameter pointer." );
            return false;
        }
        return mImpl->init( *(const MeshResourceDesc*)parameters );
    }

    static GpuResource *
    createInstance( GpuResourceDatabase & db,
                    GpuResourceHandle     handle,
                    const void          * parameters )
    {
        AutoObjPtr<MeshResourceInternal> m( new MeshResourceInternal( db, handle ) );
        if( !m->init( parameters ) ) return NULL;
        return m.detach();
    }

    static void deleteInstance( GpuResource * p )
    {
        delete GpuResource::castTo<MeshResourceInternal>( p );
    }

public:

    static bool checkAndRegisterFactory( GpuResourceDatabase & db )
    {
        if( db.hasResourceFactory( guid() ) ) return true;

        GpuResourceFactory factory = { &createInstance, &deleteInstance };

        return db.registerResourceFactory( guid(), "Mesh Resource", factory );
    }
};

//
//
// -----------------------------------------------------------------------------
GN::gfx::MeshResource::MeshResource( GpuResourceDatabase & db, GpuResourceHandle h )
    : GpuResource( db, h )
    , mImpl(NULL)
{
    mImpl = new Impl(*this);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::MeshResource::~MeshResource()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::MeshResource::guid()
{
    static const Guid MESH_GUID = { 0x892f15d5, 0x8e56, 0x4982, { 0x83, 0x1a, 0xc7, 0x1a, 0x11, 0x20, 0x4e, 0x4a } };
    return MESH_GUID;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::MeshResource::create(
    GpuResourceDatabase    & db,
    const char             * name,
    const MeshResourceDesc & desc )
{
    if( !MeshResourceInternal::checkAndRegisterFactory( db ) ) return NULL;

    return db.createResource( MeshResource::guid(), name, &desc );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::MeshResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    if( !MeshResourceInternal::checkAndRegisterFactory( db ) ) return NULL;

    GN_UNUSED_PARAM( filename );
    GN_UNIMPL();

    return NULL;
}


//
//
// -----------------------------------------------------------------------------
const MeshResourceDesc & GN::gfx::MeshResource::getDesc() const
{
    return mImpl->getDesc();
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::MeshResource::applyToContext( GpuContext & context ) const
{
    mImpl->applyToContext( context );
}
