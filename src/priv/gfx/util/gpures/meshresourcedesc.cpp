#include "pch.h"
#include "meshresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpures");

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

struct MeshVertexPosition
{
    const float * x;
    size_t        strideX;
    const float * y;
    size_t        strideY;
    const float * z;
    size_t        strideZ;
};

//
//
// -----------------------------------------------------------------------------
const VertexElement * sFindPositionElement( const VertexFormat & vf )
{
    for( size_t i = 0; i < vf.numElements; ++i )
    {
        const VertexElement & e = vf.elements[i];

        if( ( 0 == StringCompareI( "position", e.binding ) ||
              0 == StringCompareI( "pos", e.binding ) ||
              0 == StringCompareI( "gl_vertex", e.binding ) )
            &&
            0 == e.bindingIndex )
        {
            return &e;
        }
    }

    GN_ERROR(sLogger)( "Position semantice is not found in vertex format." );
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
bool sGetMeshVertexPositions( MeshVertexPosition & pos, const MeshResourceDesc & desc )
{
    const VertexElement * positionElement = sFindPositionElement( desc.vtxfmt );
    if( NULL == positionElement ) return false;

    const float * vertices = (const float*)( ((const UInt8*)desc.vertices[positionElement->stream]) + positionElement->offset );

    if( ColorFormat::FLOAT1 == positionElement->format )
    {
        pos.x = vertices;
        pos.y = 0;
        pos.z = 0;
    }
    else if( ColorFormat::FLOAT2 == positionElement->format )
    {
        pos.x = vertices;
        pos.y = vertices + 1;
        pos.z = 0;
    }
    else if( ColorFormat::FLOAT3 == positionElement->format )
    {
        pos.x = vertices;
        pos.y = vertices + 1;
        pos.z = vertices + 2;
    }
    else if( ColorFormat::FLOAT4 == positionElement->format )
    {
        pos.x = vertices;
        pos.y = vertices + 1;
        pos.z = vertices + 2;
    }
    else
    {
        GN_ERROR(sLogger)( "AABB calculation failed: unsupported vertex format %s", positionElement->format.toString().GetRawPtr() );
        return false;
    }
    pos.strideX = pos.strideY = pos.strideZ = desc.strides[positionElement->stream];

    return true;
}

//
//
// -----------------------------------------------------------------------------
MeshFileType sDetermineMeshFileType( File & fp )
{
    size_t currentPos = fp.tell();

    char buf[16];

    size_t readen;
    if( !fp.read( buf, 16, &readen ) )
    {
        GN_ERROR(sLogger)( "Fail to read mesh file header." );
        return MESH_FILE_UNKNOWN;
    }

    if( !fp.seek( currentPos, FileSeek::SET ) )
    {
        GN_ERROR(sLogger)( "Fail to seek mesh file reading cursor back." );
        return MESH_FILE_UNKNOWN;
    }

    if( 0 == memcmp( buf, MESH_BINARY_TAG, sizeof(MESH_BINARY_TAG) ) )
    {
        return MESH_FILE_BIN;
    }
    else if( 0 == StringCompareI( buf, "<?xml", 5 ) )
    {
        return MESH_FILE_XML;
    }
    else
    {
        GN_ERROR(sLogger)( "Unknown mesh file type." );
        return MESH_FILE_UNKNOWN;
    }
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> sLoadFromMeshBinaryFile( File & fp, MeshResourceDesc & desc )
{
    MeshBinaryFileHeader header;

    if( !fp.read( &header, sizeof(header), NULL ) )
    {
        GN_ERROR(sLogger)( "Fail to read mesh header." );
        return AutoRef<Blob>::NULLREF;
    }

    // verify header
    if( 0 != memcmp( header.tag, MESH_BINARY_TAG, sizeof(MESH_BINARY_TAG) ) )
    {
        GN_ERROR(sLogger)( "Unrecognized binary tag" );
        return AutoRef<Blob>::NULLREF;
    }
    if( 0x04030201 != header.endian )
    {
        GN_ERROR(sLogger)( "Unsupported endian." );
        return AutoRef<Blob>::NULLREF;
    }
    if( 0x00010000 != header.version ) // version must be 1.0
    {
        GN_ERROR(sLogger)( "Unsupported mesh version." );
        return AutoRef<Blob>::NULLREF;
    }

    // analyze vertex format
    VertexFormatProperties vfp;
    if( !vfp.analyze( header.vtxfmt ) ) return AutoRef<Blob>::NULLREF;

    // read mesh data
    AutoRef<Blob> blob( new SimpleBlob(header.bytes) );
    if( !fp.read( blob->data(), header.bytes, NULL ) )
    {
        GN_ERROR(sLogger)( "fail to read mesh data." );
        return AutoRef<Blob>::NULLREF;
    }
    const UInt8 * start = (const UInt8*)blob->data();

    desc.prim   = (PrimitiveType)header.prim;
    desc.numvtx = header.numvtx;
    desc.numidx = header.numidx;
    desc.idx32  = !!header.idx32;
    desc.dynavb = !!header.dynavb;
    desc.dynaib = !!header.dynaib;
    desc.vtxfmt = header.vtxfmt;
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

    return blob;
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
static const XmlAttrib * sGetRequiredAttrib( const XmlElement & node, const char * attribName )
{
    const XmlAttrib * a = node.findAttrib( attribName );

    if( !a )
    {
        GN_ERROR(sLogger)(
            "Element <%s>: attribute \"%s\" is missing.",
            node.name.GetRawPtr(),
            attribName ? attribName : "!!!NULLPTR!!!" );
    }

    return a;
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
template<typename T>
static bool sGetIntAttrib( T & result, const XmlElement & node, const char * attribName )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    return a && String2Integer<T>( result, a->value.GetRawPtr() );
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
template<typename T>
static T sGetIntAttrib( const XmlElement & node, const char * attribName, T defaultValue )
{
    T result;

    if( !sGetIntAttrib<T>( result, node, attribName ) )
        return defaultValue;
    else
        return result;
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
template<typename T>
static bool sGetRequiredIntAttrib( T & result, const XmlElement & node, const char * attribName )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if( !a || !String2Integer<T>( result, a->value.GetRawPtr() ) )
    {
        GN_ERROR(sLogger)(
            "Element <%s>: attribute \"%s\" is missing or is not a valid integer.",
            node.name.GetRawPtr(),
            attribName ? attribName : "!!!NULLPTR!!!" );
        return false;
    }
    else
    {
        return true;
    }
}

//
// get value of boolean attribute
// -----------------------------------------------------------------------------
static bool sGetBoolAttrib( const XmlElement & node, const char * attribName, bool defaultValue )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if( !a ) return defaultValue;

    if( 0 == StringCompareI( "1", a->value.GetRawPtr() ) ||
        0 == StringCompareI( "true", a->value.GetRawPtr() ) )
    {
        return true;
    }
    else if( 0 == StringCompareI( "0", a->value.GetRawPtr() ) ||
             0 == StringCompareI( "false", a->value.GetRawPtr() ) )
    {
        return false;
    }
    else
    {
        return defaultValue;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sReadBinaryFile( UInt8 * dst, size_t length, const char * filename )
{
    AutoObjPtr<File> fp( fs::openFile( filename, "rb" ) );
    if( !fp ) return false;

    // skip binary file header
    if( !fp->seek( 32, FileSeek::SET ) )
    {
        GN_ERROR(sLogger)( "Fail to by-pass garnet binary file header: %s", filename );
        return false;
    }

    if( !fp->read( dst, length, NULL ) )
    {
        GN_ERROR(sLogger)( "Fail to read binary data from file: %s", filename );
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> sLoadFromMeshXMLFile( File & fp, MeshResourceDesc & desc )
{
    desc.Clear();

    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp.name(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.GetRawPtr() );
        return AutoRef<Blob>::NULLREF;
    }
    GN_ASSERT( xpr.root );

    const XmlElement * root = xpr.root->toElement();
    if( !root || root->name != "mesh" )
    {
        GN_ERROR(sLogger)( "Invalid root element." );
        return AutoRef<Blob>::NULLREF;
    }

    const XmlAttrib * a = root->findAttrib( "primtype" );
    if( !a || PrimitiveType::INVALID == (desc.prim = PrimitiveType::sFromString(a->value)) )
    {
        GN_ERROR(sLogger)( "Element <%s> attribute \"%s\": missing or invalid.", root->name.GetRawPtr(), "primtype" );
        return AutoRef<Blob>::NULLREF;
    }

    if( !sGetRequiredIntAttrib( desc.numvtx, *root, "numvtx" ) )
    {
        return AutoRef<Blob>::NULLREF;
    }

    if( !sGetRequiredIntAttrib( desc.numidx, *root, "numidx" ) )
    {
        return AutoRef<Blob>::NULLREF;
    }

    desc.idx32  = sGetBoolAttrib( *root, "idx32", false );
    desc.dynavb = sGetBoolAttrib( *root, "dynavb", false );
    desc.dynaib = sGetBoolAttrib( *root, "dynaib", false );

    // get vertex format
    const XmlElement * vtxfmtNode = root->findChildElement( "vtxfmt" );
    if( !vtxfmtNode )
    {
        GN_ERROR(sLogger)( "<vtxfmt> element is missing." );
        return AutoRef<Blob>::NULLREF;
    }
    for( const XmlNode * n = vtxfmtNode->child; n != NULL; n = n->next )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "attrib" != e->name )
        {
            GN_WARN(sLogger)( "Ignore unrecognized vertex format element: <%s>.", e->name.GetRawPtr() );
            continue;
        }

        if( desc.vtxfmt.numElements >= VertexFormat::MAX_VERTEX_ELEMENTS )
        {
            GN_ERROR(sLogger)( "Too many vertex elements." );
            return AutoRef<Blob>::NULLREF;
        }
        VertexElement & ve = desc.vtxfmt.elements[desc.vtxfmt.numElements];

        UInt8 bidx;

        if( !sGetRequiredIntAttrib( ve.stream, *e, "stream" ) ||
            !sGetRequiredIntAttrib( ve.offset, *e, "offset" ) ||
            NULL == ( a = sGetRequiredAttrib( *e, "binding" ) ) ||
            !sGetRequiredIntAttrib( bidx, *e, "bindingIndex" ) )
        {
            return AutoRef<Blob>::NULLREF;
        }

        ve.bindTo( a->value, bidx );

        a = e->findAttrib( "format" );
        if( !a || (ColorFormat::UNKNOWN == (ve.format = ColorFormat::sFromString(a->value)) ) )
        {
            GN_ERROR(sLogger)( "Missing or invalid format attribute." );
            return AutoRef<Blob>::NULLREF;
        }

        desc.vtxfmt.numElements++;
    }

    // parse vtxbuf and idxbuf elements, calculate mesh data size
    size_t meshDataSize = 0;
    for( const XmlNode * n = root->child; n != NULL; n = n->next )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "vtxbuf" == e->name )
        {
            UInt32 stream, offset, stride;
            if( !sGetRequiredIntAttrib( stream, *e, "stream" ) ||
                !sGetRequiredIntAttrib( offset, *e, "offset" ) ||
                !sGetRequiredIntAttrib( stride, *e, "stride" ) ||
                NULL == (a = sGetRequiredAttrib( *e, "ref" ) ) )
            {
                return AutoRef<Blob>::NULLREF;
            }

            if( stream >= GpuContext::MAX_VERTEX_BUFFERS )
            {
                GN_WARN(sLogger)( "vtxbuf stream is too large." );
                return AutoRef<Blob>::NULLREF;
            }

            desc.offsets[stream] = offset;
            desc.strides[stream] = stride;

            meshDataSize += stride * desc.numvtx;
        }
        else if( "idxbuf" == e->name )
        {
            meshDataSize += desc.numidx * (desc.idx32?4:2);
        }
        else if( "vtxfmt" == e->name )
        {
            // silently ignored, since it is handled already.
        }
        else
        {
            GN_WARN(sLogger)( "Ignore unrecognized element: <%s>.", e->name.GetRawPtr() );
        }
    }

    AutoRef<SimpleBlob> blob( new SimpleBlob(meshDataSize) );
    if( !blob )
    {
        GN_ERROR(sLogger)( "Out of memory" );
        return AutoRef<Blob>::NULLREF;
    }

    StrA basedir = fs::dirName( fp.name() );

    // parse vtxbuf and idxbuf elements, again, to read, calculate mesh data size
    SafeArrayAccessor<UInt8> meshData( (UInt8*)blob->data(), blob->size() );
    size_t offset = 0;
    for( const XmlNode * n = root->child; n != NULL; n = n->next )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "vtxbuf" == e->name )
        {
            UInt32 stream = sGetIntAttrib<UInt32>( *e, "stream", 0xFFFFFFFF );
            GN_ASSERT( stream < GpuContext::MAX_VERTEX_BUFFERS );

            a = sGetRequiredAttrib( *e, "ref" );
            GN_ASSERT( a );

            size_t vbsize = desc.strides[stream] * desc.numvtx;

            UInt8 * vb = meshData.subrange( offset, vbsize );

            if( !sReadBinaryFile( vb, vbsize, fs::resolvePath( basedir, a->value ) ) )
            {
                return AutoRef<Blob>::NULLREF;
            }

            desc.vertices[stream] = vb;

            offset += vbsize;
        }
        else if( "idxbuf" == e->name )
        {
            a = sGetRequiredAttrib( *e, "ref" );
            GN_ASSERT( a );

            size_t ibsize = desc.numidx * (desc.idx32?4:2);

            UInt8 * ib = meshData.subrange( offset, ibsize );

            if( !sReadBinaryFile( ib, ibsize, fs::resolvePath( basedir, a->value ) ) )
            {
                return AutoRef<Blob>::NULLREF;
            }

            desc.indices = ib;

            offset += ibsize;
        }
        else if( "vtxfmt" == e->name )
        {
            // silently ignored, since it is handled already.
        }
        else
        {
            GN_WARN(sLogger)( "Ignore unrecognized element: <%s>.", e->name.GetRawPtr() );
        }
    }

    return blob;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::MeshResourceDesc::CalculateBoundingBox( Box<float> & box ) const
{
    box.x = box.y = box.w = box.h = 0.0f;

    MeshVertexPosition positions;

    if( !sGetMeshVertexPositions( positions, *this ) ) return;

    GN::CalculateBoundingBox( box, positions.x, positions.strideX, positions.y, positions.strideY, positions.z, positions.strideZ, numvtx );
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::MeshResourceDesc::CalculateBoundingSphere( Sphere<float> & sphere ) const
{
    sphere.center.Set( 0, 0, 0 );
    sphere.radius = 0;

    MeshVertexPosition positions;

    if( !sGetMeshVertexPositions( positions, *this ) ) return;

    GN::CalculateBoundingSphere( sphere, positions.x, positions.strideX, positions.y, positions.strideY, positions.z, positions.strideZ, numvtx );
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::MeshResourceDesc::getVtxBufSize( size_t stream ) const
{
    if( stream >= GpuContext::MAX_VERTEX_BUFFERS )
    {
        GN_ERROR(sLogger)( "invalid stream index." );
        return 0;
    }

    VertexFormatProperties vfp;
    if( !vfp.analyze( vtxfmt ) ) return 0;

    if( !vfp.used[stream] ) return 0;

    size_t stride = strides[stream];
    if( 0 == stride ) stride = vfp.minStrides[stream];

    return numvtx * stride;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::MeshResourceDesc::getIdxBufSize() const
{
    return numidx * (idx32?4:2);
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> GN::gfx::MeshResourceDesc::loadFromFile( File & fp )
{
    Clear();

    switch( sDetermineMeshFileType( fp ) )
    {
        case MESH_FILE_XML:
            return sLoadFromMeshXMLFile( fp, *this );

        case MESH_FILE_BIN:
            return sLoadFromMeshBinaryFile( fp, *this );

        case MESH_FILE_UNKNOWN:
        default:
            return AutoRef<Blob>::NULLREF;
    };
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> GN::gfx::MeshResourceDesc::loadFromFile( const char * filename )
{
    GN_INFO(sLogger)( "Load mesh from file: %s", filename?filename:"<null filename>" );

    Clear();

    AutoObjPtr<File> fp( fs::openFile( filename, "rb" ) );
    if( !fp ) return AutoRef<Blob>::NULLREF;

    return loadFromFile( *fp );
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
    header.prim   = this->prim;
    header.numvtx = (UInt32)this->numvtx;
    header.numidx = (UInt32)this->numidx;
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
            header.bytes += (UInt32)vbsizes[i];
        }
        header.strides[i] = (UInt32)this->strides[i];
        header.offsets[i] = (UInt32)this->offsets[i];
    }
    size_t ibsize;
    if( numidx > 0 )
    {
        ibsize = numidx * (this->idx32?4:2);
        header.indices = header.bytes;
        header.bytes  += (UInt32)ibsize;
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
