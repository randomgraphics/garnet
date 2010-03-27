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

struct MeshBinaryHeaderV1
{
    char   tag[2]; ///< must be "GN";
    UInt16 endian; ///< 0x0201 means file is in same endian as the host OS.
    UInt32 padding[7];
};
GN_CASSERT( 32 == sizeof(MeshBinaryHeaderV1) );

static const char MESH_BINARY_TAG_V1[] = "GN";

static const UInt16 MESH_BINARY_ENDIAN_TAG_V1 = 0x0201;

struct MeshBinaryFileHeaderV2
{
    char         tag[16];      ///< must be "GARNET MESH BIN\0"
    UInt32       endian;       ///< endian tag: 0x01020304 means file is in the same endian as the host OS.
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

static const char MESH_BINARY_TAG_V2[] = "GARNET MESH BIN";
GN_CASSERT( sizeof(MESH_BINARY_TAG_V2) == 16 );

static const UInt32 MESH_BINARY_ENDIAN_TAG_V2 = 0x01020304;

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
void sSwapVertexEndianInplace(
    void *               buffer,
    size_t               bufferSize, // buffer size in bytes
    const VertexFormat & format,
    size_t               stream,
    size_t               stride )
{
    if( stride == 0 ) stride = format.CalcStreamStride( stream );

    UInt8 * vertex = (UInt8*)buffer;

    size_t count = bufferSize / stride;

    for( size_t i = 0; i < count; ++i, vertex += stride )
    {
        for( size_t i = 0; i < format.numElements; ++i )
        {
            const VertexElement & e = format.elements[i];

            UInt8 * p = vertex + e.offset;

            switch( e.format.layout )
            {
                // 16 bits
                case ColorFormat::LAYOUT_4_4_4_4 :
                case ColorFormat::LAYOUT_5_5_5_1 :
                case ColorFormat::LAYOUT_5_6_5 :
                case ColorFormat::LAYOUT_16 :
                    SwapEndian8In16( p, p, 1 );
                    break;

                case ColorFormat::LAYOUT_16_16 :
                    SwapEndian8In16( p, p, 2 );
                    break;

                case ColorFormat::LAYOUT_16_16_16_16 :
                    SwapEndian8In16( p, p, 4 );
                    break;

                // 32 bits
                case ColorFormat::LAYOUT_10_11_11 :
                case ColorFormat::LAYOUT_11_11_10 :
                case ColorFormat::LAYOUT_10_10_10_2 :
                case ColorFormat::LAYOUT_32 :
                    SwapEndian8In32( p, p, 1 );
                    break;

                case ColorFormat::LAYOUT_32_32 :
                    SwapEndian8In32( p, p, 2 );
                    break;

                case ColorFormat::LAYOUT_32_32_32 :
                    SwapEndian8In32( p, p, 3 );
                    break;

                case ColorFormat::LAYOUT_32_32_32_32 :
                    SwapEndian8In32( p, p, 4 );
                    break;

                // other cases
                default:
                    break;
            }
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void sSwapIndexEndianInplace(
    void *               buffer,
    size_t               bufferSize, // buffer size in bytes
    bool                 idx32 )
{
    if( idx32 )
    {
        SwapEndian8In32( buffer, buffer, bufferSize / 4 );
    }
    else
    {
        SwapEndian8In16( buffer, buffer, bufferSize / 2 );
    }
}

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
        GN_ERROR(sLogger)( "AABB calculation failed: unsupported vertex format %s", positionElement->format.ToString().ToRawPtr() );
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
    size_t currentPos = fp.Tell();

    char buf[16];

    size_t readen;
    if( !fp.Read( buf, 16, &readen ) )
    {
        GN_ERROR(sLogger)( "Fail to read mesh file header." );
        return MESH_FILE_UNKNOWN;
    }

    if( !fp.Seek( currentPos, FileSeek::SET ) )
    {
        GN_ERROR(sLogger)( "Fail to seek mesh file reading cursor back." );
        return MESH_FILE_UNKNOWN;
    }

    if( 0 == memcmp( buf, MESH_BINARY_TAG_V2, sizeof(MESH_BINARY_TAG_V2) ) )
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
    MeshBinaryFileHeaderV2 header;

    if( !fp.Read( &header, sizeof(header), NULL ) )
    {
        GN_ERROR(sLogger)( "Fail to read mesh header." );
        return AutoRef<Blob>::NULLREF;
    }

    // verify header
    if( 0 != memcmp( header.tag, MESH_BINARY_TAG_V2, sizeof(MESH_BINARY_TAG_V2) ) )
    {
        GN_ERROR(sLogger)( "Unrecognized binary tag" );
        return AutoRef<Blob>::NULLREF;
    }
    if( MESH_BINARY_ENDIAN_TAG_V2 != header.endian )
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
    if( !fp.Read( blob->Data(), header.bytes, NULL ) )
    {
        GN_ERROR(sLogger)( "fail to read mesh data." );
        return AutoRef<Blob>::NULLREF;
    }
    UInt8 * start = (UInt8*)blob->Data();

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
    const XmlAttrib * a = node.FindAttrib( attribName );

    if( !a )
    {
        GN_ERROR(sLogger)(
            "Element <%s>: attribute \"%s\" is missing.",
            node.name.ToRawPtr(),
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
    const XmlAttrib * a = node.FindAttrib( attribName );
    return a && 0 != String2Integer<T>( result, a->value.ToRawPtr() );
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
    const XmlAttrib * a = node.FindAttrib( attribName );
    if( !a || 0 == String2Integer<T>( result, a->value.ToRawPtr() ) )
    {
        GN_ERROR(sLogger)(
            "Element <%s>: attribute \"%s\" is missing or is not a valid integer.",
            node.name.ToRawPtr(),
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
    const XmlAttrib * a = node.FindAttrib( attribName );
    if( !a ) return defaultValue;

    if( 0 == StringCompareI( "1", a->value.ToRawPtr() ) ||
        0 == StringCompareI( "true", a->value.ToRawPtr() ) )
    {
        return true;
    }
    else if( 0 == StringCompareI( "0", a->value.ToRawPtr() ) ||
             0 == StringCompareI( "false", a->value.ToRawPtr() ) )
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
static bool sReadV1BinaryFile( MeshBinaryHeaderV1 & header, UInt8 * dst, size_t length, const char * filename )
{
    AutoObjPtr<File> fp( fs::OpenFile( filename, "rb" ) );
    if( !fp ) return false;

    if( !fp->Read( &header, sizeof(header), NULL ) )
    {
        GN_ERROR(sLogger)( "Fail to read garnet binary file header: %s", filename );
        return false;
    }

    if( 0 != memcmp( header.tag, MESH_BINARY_TAG_V1, sizeof(header.tag) ) )
    {
        GN_ERROR(sLogger)( "Invalid garnet V1 binary file: %s", filename );
        return false;
    }

    if( !fp->Read( dst, length, NULL ) )
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
    if( !doc.Parse( xpr, fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp.Name(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.ToRawPtr() );
        return AutoRef<Blob>::NULLREF;
    }
    GN_ASSERT( xpr.root );

    const XmlElement * root = xpr.root->ToElement();
    if( !root || root->name != "mesh" )
    {
        GN_ERROR(sLogger)( "Invalid root element." );
        return AutoRef<Blob>::NULLREF;
    }

    const XmlAttrib * a = root->FindAttrib( "primtype" );
    if( !a || PrimitiveType::INVALID == (desc.prim = PrimitiveType::sFromString(a->value)) )
    {
        GN_ERROR(sLogger)( "Element <%s> attribute \"%s\": missing or invalid.", root->name.ToRawPtr(), "primtype" );
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
    const XmlElement * vtxfmtNode = root->FindChildElement( "vtxfmt" );
    if( !vtxfmtNode )
    {
        GN_ERROR(sLogger)( "<vtxfmt> element is missing." );
        return AutoRef<Blob>::NULLREF;
    }
    for( const XmlNode * n = vtxfmtNode->child; n != NULL; n = n->next )
    {
        const XmlElement * e = n->ToElement();
        if( !e ) continue;

        if( "attrib" != e->name )
        {
            GN_WARN(sLogger)( "Ignore unrecognized vertex format element: <%s>.", e->name.ToRawPtr() );
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

        ve.BindTo( a->value, bidx );

        a = e->FindAttrib( "format" );
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
        const XmlElement * e = n->ToElement();
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
            GN_WARN(sLogger)( "Ignore unrecognized element: <%s>.", e->name.ToRawPtr() );
        }
    }

    AutoRef<SimpleBlob> blob( new SimpleBlob(meshDataSize) );
    if( !blob )
    {
        GN_ERROR(sLogger)( "Out of memory" );
        return AutoRef<Blob>::NULLREF;
    }

    StrA basedir = fs::DirName( fp.Name() );

    // parse vtxbuf and idxbuf elements, again, to read, calculate mesh data size
    SafeArrayAccessor<UInt8> meshData( (UInt8*)blob->Data(), blob->Size() );
    size_t offset = 0;
    for( const XmlNode * n = root->child; n != NULL; n = n->next )
    {
        const XmlElement * e = n->ToElement();
        if( !e ) continue;

        if( "vtxbuf" == e->name )
        {
            UInt32 stream = sGetIntAttrib<UInt32>( *e, "stream", 0xFFFFFFFF );
            GN_ASSERT( stream < GpuContext::MAX_VERTEX_BUFFERS );

            a = sGetRequiredAttrib( *e, "ref" );
            GN_ASSERT( a );

            size_t vbsize = desc.strides[stream] * desc.numvtx;

            UInt8 * vb = meshData.SubRange( offset, vbsize );

            MeshBinaryHeaderV1 header;
            if( !sReadV1BinaryFile( header, vb, vbsize, fs::ResolvePath( basedir, a->value ) ) )
            {
                return AutoRef<Blob>::NULLREF;
            }

            if( header.endian != MESH_BINARY_ENDIAN_TAG_V1 )
            {
                sSwapVertexEndianInplace( vb, vbsize, desc.vtxfmt, stream, desc.strides[stream] );
            }

            desc.vertices[stream] = vb;

            offset += vbsize;
        }
        else if( "idxbuf" == e->name )
        {
            a = sGetRequiredAttrib( *e, "ref" );
            GN_ASSERT( a );

            size_t ibsize = desc.numidx * (desc.idx32?4:2);

            UInt8 * ib = meshData.SubRange( offset, ibsize );

            MeshBinaryHeaderV1 header;
            if( !sReadV1BinaryFile( header, ib, ibsize, fs::ResolvePath( basedir, a->value ) ) )
            {
                return AutoRef<Blob>::NULLREF;
            }

            if( header.endian != MESH_BINARY_ENDIAN_TAG_V1 )
            {
                sSwapIndexEndianInplace( ib, ibsize, desc.idx32 );
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
            GN_WARN(sLogger)( "Ignore unrecognized element: <%s>.", e->name.ToRawPtr() );
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
size_t GN::gfx::MeshResourceDesc::GetVtxBufSize( size_t stream ) const
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
size_t GN::gfx::MeshResourceDesc::GetIdxBufSize() const
{
    return numidx * (idx32?4:2);
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> GN::gfx::MeshResourceDesc::LoadFromFile( File & fp )
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
AutoRef<Blob> GN::gfx::MeshResourceDesc::LoadFromFile( const char * filename )
{
    GN_INFO(sLogger)( "Load mesh from file: %s", filename?filename:"<null filename>" );

    Clear();

    AutoObjPtr<File> fp( fs::OpenFile( filename, "rb" ) );
    if( !fp ) return AutoRef<Blob>::NULLREF;

    return LoadFromFile( *fp );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::SaveToFile( File & fp ) const
{
    VertexFormatProperties vfp;
    if( !vfp.analyze( vtxfmt ) ) return false;

    MeshBinaryFileHeaderV2 header;
    memcpy( header.tag, MESH_BINARY_TAG_V2, sizeof(MESH_BINARY_TAG_V2) );
    header.endian = MESH_BINARY_ENDIAN_TAG_V2;
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
    if( !fp.Write( &header, sizeof(header), NULL ) )
    {
        GN_ERROR(sLogger)( "Fail to write mesh header." );
        return false;
    }

    // write vertex buffers
    for( size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i )
    {
        if( vfp.used[i] )
        {
            if( !fp.Write( this->vertices[i], vbsizes[i], NULL ) )
            {
                GN_ERROR(sLogger)( "Fail to write vertex buffer %i", i );
                return false;
            }
        }
    }

    // write index buffer
    if( numidx > 0 )
    {
        if( !fp.Write( this->indices, ibsize, NULL ) )
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
bool GN::gfx::MeshResourceDesc::SaveToFile( const char * filename ) const
{
    GN_INFO(sLogger)( "Save mesh to file: %s", filename?filename:"<null filename>" );

    AutoObjPtr<File> fp( fs::OpenFile( filename, "wb" ) );
    if( !fp ) return false;
    return SaveToFile( *fp );
}
