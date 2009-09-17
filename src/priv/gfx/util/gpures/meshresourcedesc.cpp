#include "pch.h"
#include "meshresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

enum MeshFileType
{
    MESH_FILE_UNKNOWN,
    MESH_FILE_XML,
    MESH_FILE_BIN,
};

struct MeshBinaryFileHeader
{
    char         tag[16];      ///< must be "GARNET MESH BIN\0"
    UInt32       endian;       ///< endian type: 0x04030201 means little endian, else, big endian
    UInt32       version;      ///< mesh binary version must be 0x00010000
    UInt32       prim;         ///< primitive type
    UInt32       numvtx;       ///< number of vertices
    UInt32       numidx;       ///< number of indices. 0 means non-indexed mesh
    UInt8        idx32;        ///< true for 32-bit index buffer
    UInt8        dynavb;       ///< true for dynamic vertex buffer
    UInt8        dynaib;       ///< trur for dynamic index buffer
    UInt8        _padding;     ///< padding for 32-bit alignment
    VertexFormat vtxfmt;       ///< vertex format
    UInt32       vertices[GpuContext::MAX_VERTEX_BUFFERS]; ///< The offset of vertex buffer data, not including the header.
    UInt32       strides[GpuContext::MAX_VERTEX_BUFFERS];  ///< vertex buffer strides. 0 means using vertex size defined by vertex format.
    UInt32       offsets[GpuContext::MAX_VERTEX_BUFFERS];  ///< vertex buffer offset.
    UInt32       indices;                                  ///< The offset of index data. Ignored, if numidx is 0.
    UInt32       bytes;        ///< total binary size in bytes, not including this header.
};

static const char MESH_BINARY_TAG[] = "GARNET MESH BIN";
GN_CASSERT( sizeof(MESH_BINARY_TAG) == 16 );

//
//
// -----------------------------------------------------------------------------
MeshFileType sDetermineMeshFileType( File & )
{
    return MESH_FILE_UNKNOWN;
}

//
//
// -----------------------------------------------------------------------------
bool sLoadFromMeshBinaryFile( File & fp, DynaArray<UInt8> & meshdata, MeshResourceDesc & desc )
{
    MeshBinaryFileHeader header;

    if( !fp.read( &header, sizeof(header), NULL ) )
    {
        GN_ERROR(sLogger)( "Fail to read mesh header." );
        return false;
    }

    // verify header
    if( 0 == memcmp( header.tag, MESH_BINARY_TAG, sizeof(MESH_BINARY_TAG) ) )
    {
        GN_ERROR(sLogger)( "Not a garnet mesh binary." );
        return false;
    }
    if( 0x04030201 != header.endian )
    {
        GN_ERROR(sLogger)( "Unsupported endian." );
        return false;
    }
    if( 0x00010000 != header.version )
    {
        GN_ERROR(sLogger)( "Unsupported mesh version." );
        return false;
    }

    // analyze vertex format
    VertexFormatProperties vfp;
    if( !vfp.analyze( header.vtxfmt ) ) return false;

    // read mesh data
    meshdata.resize( header.bytes );
    if( !fp.read( meshdata.cptr(), header.bytes, NULL ) )
    {
        GN_ERROR(sLogger)( "fail to read mesh data." );
        return false;
    }
    const UInt8 * start = meshdata.cptr();

    desc.prim   = (PrimitiveType)header.prim;
    desc.numvtx = header.numvtx;
    desc.numidx = header.numidx;
    desc.idx32  = !!header.idx32;
    desc.dynavb = !!header.dynavb;
    desc.dynaib = !!header.dynaib;
    for( size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i )
    {
        if( vfp.used[i] )
        {
            desc.vertices[i] = header.vertices[i] + start;
            desc.strides[i] = header.strides[i];
            desc.offsets[i] = header.offsets[i];
        }
        else
        {
            desc.vertices[i] = NULL;
            desc.strides[i] = 0;
            desc.offsets[i] = 0;
        }
    }
    if( desc.numidx > 0 )
    {
        desc.indices = header.indices + start;
    }
    else
    {
        desc.indices = NULL;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool sLoadFromMeshXMLFile( File & fp, DynaArray<UInt8> & meshdata, MeshResourceDesc & desc )
{
    GN_UNUSED_PARAM( fp );
    GN_UNUSED_PARAM( meshdata );
    GN_UNUSED_PARAM( desc );
    GN_UNIMPL();
    return false;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::loadFromFile( File & fp, DynaArray<UInt8> & meshdata )
{
    clear();
    meshdata.clear();

    switch( sDetermineMeshFileType( fp ) )
    {
        case MESH_FILE_XML:
            return sLoadFromMeshXMLFile( fp, meshdata, *this );

        case MESH_FILE_BIN:
            return sLoadFromMeshBinaryFile( fp, meshdata, *this );

        case MESH_FILE_UNKNOWN:
        default:
            return false;
    };
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::loadFromFile( const char * filename, DynaArray<UInt8> & meshdata )
{
    GN_INFO(sLogger)( "Load mesh from file: %s", filename?filename:"<null filename>" );

    clear();
    meshdata.clear();

    AutoObjPtr<File> fp( fs::openFile( filename, "rb" ) );
    if( !fp ) return false;

    return loadFromFile( *fp, meshdata );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::saveToFile( File & fp ) const
{
    VertexFormatProperties vfp;
    if( !vfp.analyze( vtxfmt ) ) return false;

    MeshBinaryFileHeader header;
    memcpy( header.tag, MESH_BINARY_TAG, sizeof(MESH_BINARY_TAG) );
    header.endian = 0x04030201;
    header.version = 0x00010000;
    header.numvtx = this->numvtx;
    header.numidx = this->numidx;
    header.idx32  = this->idx32;
    header.dynavb = this->dynavb;
    header.dynaib = this->dynaib;
    header.vtxfmt = this->vtxfmt;

    // calculate mesh data size and vb/ib offsets
    size_t vbsizes[GpuContext::MAX_VERTEX_BUFFERS];
    header.bytes = 0;
    for( size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i )
    {
        if( vfp.used[i] )
        {
            size_t stride = this->strides[i];
            if( 0 == stride ) stride = vfp.minStrides[i];
            vbsizes[i] = numvtx * stride;

            header.vertices[i] = header.bytes;
            header.bytes += vbsizes[i];
        }
    }
    size_t ibsize;
    if( numidx > 0 )
    {
        ibsize = numidx * (this->idx32?4:2);
        header.indices = header.bytes;
        header.bytes  += ibsize;
    }
    else
    {
        ibsize = 0;
        header.indices = 0;
    }

    // write header
    if( !fp.write( &header, sizeof(header), NULL ) )
    {
        GN_ERROR(sLogger)( "Fail to write mesh header." );
        return false;
    }

    // write vertex buffers
    for( size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i )
    {
        if( vfp.used[i] )
        {
            if( !fp.write( this->vertices[i], vbsizes[i], NULL ) )
            {
                GN_ERROR(sLogger)( "Fail to write vertex buffer %i", i );
                return false;
            }
        }
    }

    // write index buffer
    if( numidx > 0 )
    {
        if( !fp.write( this->indices, ibsize, NULL ) )
        {
            GN_ERROR(sLogger)( "Fail to write index buffer" );
            return false;
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::saveToFile( const char * filename ) const
{
    GN_INFO(sLogger)( "Save mesh to file: %s", filename?filename:"<null filename>" );

    AutoObjPtr<File> fp( fs::openFile( filename, "wb" ) );
    if( !fp ) return false;
    return saveToFile( *fp );
}
