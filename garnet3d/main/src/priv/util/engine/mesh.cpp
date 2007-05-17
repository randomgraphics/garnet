#include "pch.h"
#include <pcrecpp.h>

static GN::Logger * sLogger = GN::getLogger("GN.engine.Mesh");

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// local classes and functions
// *****************************************************************************

//
// post error message
// -----------------------------------------------------------------------------
static void sPostError( const XmlNode & node, const StrA & msg )
{
    GN_UNUSED_PARAM( node );
    GN_ERROR(sLogger)( "%s", msg.cptr() );
}

//
// get integer value of specific attribute
// -----------------------------------------------------------------------------
template<typename T>
static bool sGetIntAttrib( const XmlElement & node, const char * attribName, T & result )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a || !str2Int<T>( result, a->value.cptr() ) )
    {
        sPostError( node, strFormat( "attribute '%s' is missing!", attribName ) );
        return false;
    }
    else
    {
        return true;
    }
}

//
// get integer value of specific attribute
// -----------------------------------------------------------------------------
static bool sGetOptionalBoolAttrib( const XmlElement & node, const char * attribName, bool defval )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if( !a ) return defval;

    return 0 == strCmpI( "yes", a->name.cptr() )
        || 0 == strCmpI( "true", a->name.cptr() )
        || 0 == strCmp( "1", a->name.cptr() );
}

//
// get string value of specific attribute
// -----------------------------------------------------------------------------
static bool sGetStringAttrib( const XmlElement & node, const char * attribName, GN::StrA & result, bool silence = false )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a )
    {
        if( !silence ) sPostError( node, strFormat( "attribute '%s' is missing!", attribName ) );
        return false;
    }
    else
    {
        result = a->value;
        return true;
    }
}

struct BinFileHeader
{
    char   tag[2]; // 'G','N'
    UInt16 endian; // 0x0201
};
GN_CASSERT( sizeof(BinFileHeader) == 4 );

class BinFileLoader : public GN::engine::GraphicsResourceLoader
{
    const StrA   mFileName;
    const size_t mDataOffset;
    const size_t mDataBytes;

public:

    ///
    /// ctor
    ///
    BinFileLoader( const StrA & filename, size_t offset, size_t bytes )
        : mFileName(filename)
        , mDataOffset(offset)
        , mDataBytes(bytes)
    {
    }

    ///
    /// dtor
    ///
    ~BinFileLoader()
    {
    }

    virtual bool load( const engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        // open vb file
        AutoObjPtr<File> fp( core::openFile( mFileName, "rb" ) );
        if( fp.empty() ) return false;

        // alloc data buffer
        size_t bytes = sizeof(BinFileHeader) + mDataBytes;
        AutoObjPtr<UInt8> buf( new UInt8[bytes] );
        if( buf.empty() ) return false;

        // read file
        if( !fp->seek( (int)mDataOffset, FSEEK_SET ) ) return false;
        size_t readen;
        if( !fp->read( buf, bytes, &readen ) || bytes != readen ) return false;

        // check file header
        if( 'G' != buf[0] || 'N' != buf[1] )
        {
            GN_ERROR(sLogger)( "%s is not a valid garnet binary file!", mFileName.cptr() );
            return false;
        }

        // success
        outbuf = buf.detach();
        outbytes = bytes;
        return true;
    }

    bool decompress( const engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
    {
        GN_ASSERT( inbuf && inbytes >= 4 );

        const BinFileHeader * header = (const BinFileHeader*)inbuf;
        const UInt32 * data = (const UInt32 *)( header + 1 );

        GN_ASSERT( 'G' == header->tag[0] && 'N' == header->tag[1] );

        size_t dwcount = ( inbytes - sizeof(BinFileHeader) + 3 ) / 4;
        AutoTypePtr<UInt32> buf( new UInt32[dwcount] );
        if( buf.empty() ) return false;

        if( header->endian == 0x0201 )
        {
            // file and machine endian are same.
            memcpy( buf, data, dwcount * 4 );
        }
        else
        {
            // file and machine has different endian. do endian swap.
            swap8in32( buf, data, dwcount );
        }

        // success
        outbuf   = buf.detach();
        outbytes = dwcount * 4;
        return true;
    }

    virtual void freebuf( void * inbuf, size_t )
    {
        safeDeleteArray( inbuf );
    }
};

class MeshVtxBufLoader : public BinFileLoader
{
public:

    ///
    /// ctor
    ///
    MeshVtxBufLoader( const StrA & filename, size_t offset, size_t bytes ) : BinFileLoader(filename,offset,bytes) {}

    virtual bool copy( engine::GraphicsResource & gfxres, const void * inbuf, size_t inbytes, int )
    {
        using namespace GN::gfx;
        VtxBuf * vb = gfxres.vtxbuf;
        void * data = vb->lock( 0, 0, LOCK_DISCARD );
        if( 0 == data ) return false;
        memcpy( data, inbuf, inbytes );
        vb->unlock();
        return true;
    }
};

class MeshIdxBufLoader : public BinFileLoader
{
public:

    ///
    /// ctor
    ///
    MeshIdxBufLoader( const StrA & filename, size_t offset, size_t bytes ) : BinFileLoader(filename,offset,bytes) {}

    virtual bool copy( engine::GraphicsResource & gfxres, const void * inbuf, size_t inbytes, int )
    {
        using namespace GN::gfx;
        IdxBuf * ib = gfxres.idxbuf;
        void * data = ib->lock( 0, 0, LOCK_DISCARD );
        if( 0 == data ) return false;
        memcpy( data, inbuf, inbytes );
        ib->unlock();
        return true;
    }
};

// *****************************************************************************
// public methods
// *****************************************************************************

/*
    <mesh
        primtype  = "TRIANGLE_LIST"
        numprim   = "100"
        startvtx  = "0"
        minvtxidx = "0"
        numvtx    = "30"
        startidx  = "0"
        >

        <vtxfmt>
            ...
        </vtxfmt>
        
        <vtxbuf>
            ...
        </vtxbuf>

        <idxbuf> <!-- this is optional -->
            ...
        </idxbuf>
    </mesh>
*/

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Mesh::loadFromXmlNode( const XmlNode & root, const StrA & basedir )
{
    GN_GUARD;

    // check root node
    const XmlElement * e = root.toElement();
    if( 0 == e || "mesh" != e->name )
    {
        sPostError( *e, "root node must be \"<mesh>\"." );
        return false;
    }

    // clear to empty
    clear();

    // get primitive type
    StrA s;
    if( !sGetStringAttrib( *e, "primtype", s ) ) return false;
    primtype = gfx::str2PrimitiveType( s );
    if( primtype >= gfx::NUM_PRIMITIVES ) return false;

    // get primitive count
    if( !sGetIntAttrib( *e, "numprim", numprim ) ) return false;

    // get start vertex index
    if( !sGetIntAttrib( *e, "startvtx", startvtx ) ) return false;

    // get minimal vertex index
    if( !sGetIntAttrib( *e, "minvtxidx", minvtxidx ) ) return false;

    // get vertex count
    if( !sGetIntAttrib( *e, "numvtx", numvtx ) ) return false;

    // get start index
    if( !sGetIntAttrib( *e, "startidx", startidx ) ) return false;

    // load vertex format
    XmlNode * vfnode = e->findChildElement( "vtxfmt" );
    if( !vfnode )
    {
        sPostError( *e, "No valid vertex format definition found!" );
        return false;
    }
    VtxFmtDesc vfd;
    if( !vfd.loadFromXml( vfnode ) ) return false;
    vtxfmt = createVtxFmt( engine, vfd );
    if( 0 == vtxfmt ) return false;

    // handle child elements
    size_t numStreams = vfd.calcNumStreams();
    for( XmlNode * c = root.child; c; c = c->sibling )
    {
        e = c->toElement();
        if( !e ) continue;

        // load vertex buffer
        if( "vtxbuf" == e->name )
        {
            // get vertex stream index
            size_t stream;
            if( !sGetIntAttrib( *e, "stream", stream ) ) return false;
            if( stream >= numStreams )
            {
                sPostError( *e, strFormat( "stream index (%d) is too large.", stream ) );
                return false;
            }

            if( vtxbufs.size() < (stream+1) ) vtxbufs.resize( stream + 1 );
            MeshVtxBuf & vb = vtxbufs[stream];

            // get stream offset and stride
            if( !sGetIntAttrib( *e, "offset", vb.offset ) ) return false;
            if( !sGetIntAttrib( *e, "stride", vb.stride ) ) return false;

            // get vb file name
            StrA ref;
            if( !sGetStringAttrib( *e, "ref", ref ) ) return false;
            ref = core::resolvePath( basedir, ref );

            // compose vb descriptor
            GraphicsResourceDesc grd;
            grd.name = ref;
            grd.type = GRT_VTXBUF;
            grd.vd.dynamic  = sGetOptionalBoolAttrib( *e, "dynamic", false );
            grd.vd.readback = sGetOptionalBoolAttrib( *e, "readback", false );
            grd.vd.bytes = (UInt32)(vb.stride * numvtx);

            // create vb
            vb.buffer = engine.allocResource( grd );
            if( 0 == vb.buffer ) return false;

            // load vb content
            AutoRef<MeshVtxBufLoader> loader( new MeshVtxBufLoader( ref, 0, grd.vd.bytes ) );
            engine.updateResource( vb.buffer, 0, loader );
        }

        // load index buffer
        else if( "idxbuf" == e->name )
        {
            // get ib file name
            StrA ref;
            if( !sGetStringAttrib( *e, "ref", ref ) ) return false;
            ref = core::resolvePath( basedir, ref );

            // compose ib descriptor
            GraphicsResourceDesc grd;
            grd.name = ref;
            grd.type = GRT_IDXBUF;
            grd.id.dynamic  = sGetOptionalBoolAttrib( *e, "dynamic", false );
            grd.id.readback = sGetOptionalBoolAttrib( *e, "readback", false );
            grd.id.numidx = (UInt32)gfx::calcVertexCount( primtype, numprim );

            // create ib
            idxbuf = engine.allocResource( grd );
            if( 0 == idxbuf ) return false;

            // load ib content
            size_t bytes = grd.id.numidx * 2; // 16-bit index buffer
            AutoRef<MeshIdxBufLoader> loader( new MeshIdxBufLoader( ref, 0, bytes ) );
            engine.updateResource( idxbuf, 0, loader );
        }

        else if( "vtxfmt" != e->name )
        {
            sPostError( *e, strFormat( "Ignore unknown node '%s'.", e->name.cptr() ) );
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

/*
//
// -----------------------------------------------------------------------------
bool GN::engine::Mesh::loadFromBinaryStream( const StrA & meshname, File & fp )
{
    GN_GUARD;

    size_t readen;

    // read chunk header
    struct ChunkHeader
    {
        char   tag[22]; // up to 22 characters to idenity chunk type
        UInt16 endian;  // 0x0201 means little endian; else, big endian
        UInt64 bytes;   // chunk size in bytes, not including this header.
    } header;
    if( !fp.read( &header, sizeof(header), &readen ) || readen != sizeof(header) )
    {
        GN_ERROR(sLogger)( "fail to read binary chunk header!" );
        return false;
    }

    // verify chunk header
    static const char MESH_TAG[] = {"GARNET MESH V0.1"};
    if( 0 != strCmp( header.tag, MESH_TAG, sizeof(MESH_TAG) ) )
    {
        GN_ERROR(sLogger)( "Not a mesh chunk!" );
        return false;
    }

    size_t readen;

    // read mesh binary header
    MeshBinaryHeader mbh;
    if( !fp.read( &mbh, sizeof(mbh), &readen ) || readen != sizeof(mbh) )
    {
        GN_ERROR(sLogger)( "Fail to read mesh header." );
        return false;
    }
    header.bytes -= sizeof(mbh);

    GraphicsResourceDesc grd;

    // load vertex format
    grd.type = GRT_VTXFMT;
    grd.fd = mbh.vtxfmt;
    vtxfmt = engine.allocResource( grd );
    if( 0 == vtxfmt ) return false;

    // store other fields
    primtype  = mbh.primtype;
    numprim   = mbh.numprim;
    startvtx  = mbh.startvtx;
    minvtxidx = mbh.minvtxidx;
    numvtx    = mbh.numvtx;
    startidx  = mbh.startidx;

    // create vertex buffers
    size_t numStreams = mbh->vtxfmt.calcNumStreams();
    vtxbufs.resize( numStreams );
    for( size_t i = 0; i < numStreams; ++i )
    {
        MeshVtxBufBinaryHeader * mvbbh = (const MeshVtxBufBinaryHeader *)ptr;
        ptr += sizeof(MeshVtxBufBinaryHeader);
        if( ptr > end )
        {
            GN_ERROR(sLogger)( "fail to read vertex buffer %d", i );
            return false;
        }

        MeshVtxBuf & vb = vtxbufs[i];
        vb.offset = mvbbh->offset;
        vb.stride = mvbbh->stride;

        // calculate vb size
        size_t bytes = numvtx * vb.stride;
        if( (ptr+bytes) > end )
        {
            GN_ERROR(sLogger)( "fail to read vertex buffer %d", i );
            return false;
        }

        // create new VB
        vb.buffer.attach( r.createVtxBuf( bytes, !!mvbbh->dynamic, !!mvbbh->readback ) );
        if( vb.buffer.empty() ) return false;

        // fill data
        UInt8 * dst = (UInt8*)vb.buffer->lock( 0, 0, LOCK_WO );
        if( 0 == dst ) return false;
        AutoSurfaceUnlocker<VtxBuf> unlocker( vb.buffer );
        memcpy( dst, ptr, bytes );
        ptr += bytes;
        GN_ASSERT( ptr <= end );
    }

    // check index buffer header
    const MeshIdxBufBinaryHeader * mibbh = (const MeshIdxBufBinaryHeader*)ptr;
    ptr += sizeof(MeshIdxBufBinaryHeader);
    if( ptr > end )
    {
        GN_ERROR(sLogger)( "fail to read index buffer header." );
        return false;
    }

    // calculate index buffer size
    size_t numidx = calcVertexCount( primtype, numprim );
    size_t bytes  = numidx * 2; // 16-bit index buffer
    if( (ptr+bytes) > end )
    {
        GN_ERROR(sLogger)( "fail to read index buffer data." );
        return false;
    }

    // create index buffer
    idxbuf.attach( r.createIdxBuf( bytes, !!mibbh->dynamic, !!mibbh->readback ) );
    if( idxbuf.empty() ) return false;

    // fill index buffer
    UInt16 * dst = (UInt16*)idxbuf->lock( 0, 0, LOCK_WO );
    if( 0 == dst ) return false;
    AutoSurfaceUnlocker<IdxBuf> unlocker( idxbuf );
    memcpy( dst, ptr, bytes );

    // success
    return true;

    GN_UNGUARD;
}*/

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Mesh::loadFromFile( const StrA & filename )
{
    GN_GUARD;

    // open mesh file
    AutoObjPtr<File> fp( core::openFile( filename, "rb" ) );
    if( !fp ) return false;

    // read file header
    static const char bintag[] = { "GARNET MESH" };
    StackArray<char,sizeof(bintag)> buf;
    if( !fp->read( buf.cptr(), sizeof(bintag), 0 ) ) return 0;

    if( 0 == strCmp( bintag, buf.cptr(), sizeof(bintag)-1 ) )
    {
        // load as mesh binary
        //if( !mesh->loadFromBinaryStream( *fp ) ) return 0;
        GN_UNIMPL();
        return false;
    }
    else
    {
        AutoObjPtr<File> fp( core::openFile( filename, "rt" ) );
        if( !fp ) return false;

        StrA basedir = dirName( filename );

        XmlDocument doc;
        XmlParseResult xpr;
        if( !doc.parse( xpr, *fp ) )
        {
            GN_ERROR(sLogger)(
                "Fail to parse XML file (%s):\n"
                "    line   : %d\n"
                "    column : %d\n"
                "    error  : %s",
                filename.cptr(),
                xpr.errLine,
                xpr.errColumn,
                xpr.errInfo.cptr() );
            return false;
        }
        GN_ASSERT( xpr.root );
        return loadFromXmlNode( *xpr.root, basedir );
    }

    GN_UNGUARD;
}

// *****************************************************************************
// global functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityTypeId GN::engine::getMeshEntityType( EntityManager & em )
{
    static EntityTypeId type = em.createEntityType( "mesh" );
    return type;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::loadMeshEntityFromFile(
    EntityManager & em, RenderEngine & re, const StrA & filename )
{
    GN_TODO( "convert filename to absolute/full path" );

    // check if the entity exists already
    Entity * e = em.getEntityByName( filename, true );
    if( e ) return e;

    // load mesh
    AutoObjPtr<Mesh> mesh( new Mesh(re) );
    if( !mesh->loadFromFile( filename ) ) return 0;

    // success
    return em.createEntity( getMeshEntityType(em), filename, mesh.get() );
}

/*
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::generateCubeMeshEntity(
    EntityManager & em, RenderEngine & re, const StrA & name, float edgeLength )
{
}*/

//
//
// -----------------------------------------------------------------------------
void GN::engine::deleteMeshEntity( Entity * e )
{
    if( 0 == e ) return;

    Mesh * object = entity2Object<Mesh*>( e, 0 );
    if( 0 == object ) return;

    delete object;

    e->manager.eraseEntity( e );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::deleteAllMeshEntities( EntityManager & )
{
    GN_UNIMPL();
}

/*
//
// -----------------------------------------------------------------------------
bool GN::engine::generateCubeMesh( Mesh & mesh, float edgeLength )
{
    GN_GUARD;

    // clear to empty
    mesh.clear();

    Renderer & r = gRenderer;

    // create vertex format
    mesh.vtxfmt = r.createVtxFmt( VtxFmtDesc::XYZ_NORM_UV );
    if( 0 == mesh.vtxfmt ) return false;

    struct CubeVertex
    {
        float p[3];
        float n[3];
        float t[2];
    };
    GN_CASSERT( sizeof(CubeVertex) == 32 );

    // create vertex buffer
    mesh.vtxbufs.resize( 1 );
    mesh.vtxbufs[0].buffer.attach( r.createVtxBuf( 24*sizeof(CubeVertex) ) );
    if( !mesh.vtxbufs[0].buffer ) return false;
    mesh.vtxbufs[0].offset = 0;
    mesh.vtxbufs[0].stride = sizeof(CubeVertex);

    // create index buffer
    mesh.idxbuf.attach( r.createIdxBuf( 36 * sizeof(UInt16) ) );
    if( !mesh.idxbuf ) return false;

    // lock the buffers
    CubeVertex * vb = (CubeVertex*)mesh.vtxbufs[0].buffer->lock( 0, 0, LOCK_DISCARD );
    if( 0 == vb ) return false;
    AutoSurfaceUnlocker<VtxBuf> vbunlocker( mesh.vtxbufs[0].buffer );

    UInt16 * ib = mesh.idxbuf->lock( 0, 0, LOCK_DISCARD );
    if( 0 == ib ) return false;
    AutoSurfaceUnlocker<IdxBuf> ibunlocker( mesh.idxbuf );

    // fill mesh data
    createBox(
        edgeLength, edgeLength, edgeLength,
        vb->p, sizeof(CubeVertex),
        vb->t, sizeof(CubeVertex),
        vb->n, sizeof(CubeVertex),
        0, 0, // tangent
        0, 0, // binormal
        ib, 0 );

    // set draw parameters
    mesh.primtype = TRIANGLE_LIST;
    mesh.numprim = 12;
    mesh.startvtx = 0;
    mesh.minvtxidx = 0;
    mesh.numvtx = 24;
    mesh.startidx = 0;

    // success
    return true;

    GN_UNGUARD;
}*/
