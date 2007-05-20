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

#if 1
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

        // read file header
        GN::engine::BinaryFileHeader header;
        if( !fp->read( &header, sizeof(header), 0 ) ) return false;

        // check file header
        if( 'G' != header.tag[0] || 'N' != header.tag[1] )
        {
            GN_ERROR(sLogger)( "File %s is not a valid garnet binary file!", mFileName.cptr() );
            return false;
        }
        if( ( header.bytes + sizeof(header) ) < (mDataOffset+mDataBytes) )
        {
            GN_ERROR(sLogger)( "File %s is not large enough (maybe corrupted)!", mFileName.cptr() );
            return false;
        }

        // alloc data buffer
        AutoObjPtr<UInt32> buf( new UInt32[( mDataBytes + 3 ) / 4 + 1] );
        if( buf.empty() )
        {
            GN_ERROR(sLogger)( "out of memory!" );
            return false;
        }

        // store endian tag
        buf[0] = header.endian;

        // read file
        if( !fp->seek( (int)mDataOffset, FSEEK_SET ) ) return false;
        size_t readen;
        if( !fp->read( &buf[1], mDataBytes, &readen ) || mDataBytes != readen ) return false;

        // success
        outbuf = buf.detach();
        outbytes = mDataBytes + 4;
        return true;
    }

    bool decompress( const engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
    {
        GN_ASSERT( inbuf && inbytes >= 4 );

        const UInt32 * endian = (const UInt32*)inbuf;
        const UInt32 * data = endian + 1;

        size_t dwcount = ( inbytes - 1 ) / 4;
        AutoTypePtr<UInt32> buf( new UInt32[dwcount] );
        if( buf.empty() )
        {
            GN_ERROR(sLogger)( "out of memory!" );
            return false;
        }

        if( *endian == 0x0201 )
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

#else

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

    virtual bool load( const GN::engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    bool decompress( const GN::engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void *, size_t, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GN::engine::GraphicsResource &, const void * , size_t, int )
    {
        return true;
    }

    virtual void freebuf( void *, size_t )
    {
    }
};

class MeshVtxBufLoader : public BinFileLoader
{
public:

    ///
    /// ctor
    ///
    MeshVtxBufLoader( const StrA & filename, size_t offset, size_t bytes ) : BinFileLoader(filename,offset,bytes) {}
};

class MeshIdxBufLoader : public BinFileLoader
{
public:

    ///
    /// ctor
    ///
    MeshIdxBufLoader( const StrA & filename, size_t offset, size_t bytes ) : BinFileLoader(filename,offset,bytes) {}
};

#endif

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
    vtxfmt = engine.createVtxFmt( "mesh vtxfmt", vfd );
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
            // get vb file name
            StrA ref;
            if( !sGetStringAttrib( *e, "ref", ref ) ) return false;
            ref = core::resolvePath( basedir, ref );

            GN_INFO(sLogger)( "Load %s", ref.cptr() );

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

            size_t bytes = vb.stride * numvtx;

            // create vb
            vb.buffer = engine.createVtxBuf(
                ref,
                bytes,
                sGetOptionalBoolAttrib( *e, "dynamic", false ),
                sGetOptionalBoolAttrib( *e, "readback", false ) );
            if( 0 == vb.buffer ) return false;

            // load vb content
            AutoRef<MeshVtxBufLoader> loader( new MeshVtxBufLoader( ref, sizeof(BinaryFileHeader), bytes ) );
            engine.updateResource( vb.buffer, 0, loader );
        }

        // load index buffer
        else if( "idxbuf" == e->name )
        {
            // get ib file name
            StrA ref;
            if( !sGetStringAttrib( *e, "ref", ref ) ) return false;
            ref = core::resolvePath( basedir, ref );

            GN_INFO(sLogger)( "Load %s", ref.cptr() );

            size_t numidx = gfx::calcVertexCount( primtype, numprim );

            // create ib
            idxbuf = engine.createIdxBuf(
                ref,
                numidx,
                sGetOptionalBoolAttrib( *e, "dynamic", false ),
                sGetOptionalBoolAttrib( *e, "readback", false ) );
            if( 0 == idxbuf ) return false;

            // load ib content
            size_t bytes = numidx * 2; // 16-bit index buffer
            AutoRef<MeshIdxBufLoader> loader( new MeshIdxBufLoader( ref, sizeof(BinaryFileHeader), bytes ) );
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

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Mesh::loadFromBinaryStream( File & fp )
{
    GN_GUARD;

    size_t readen;

    // read chunk header
    BinaryFileHeader header;
    if( !fp.read( &header, sizeof(header), &readen ) || readen != sizeof(header) )
    {
        GN_ERROR(sLogger)( "fail to read binary chunk header!" );
        return false;
    }

    // verify chunk header
    static const char MESH_TAG[] = {"MESH V0.1"};
    if( 0 != strCmp( header.name, MESH_TAG, sizeof(MESH_TAG) ) )
    {
        GN_ERROR(sLogger)( "Not a mesh chunk!" );
        return false;
    }

    // read mesh binary header
    MeshBinaryHeader mbh;
    if( !fp.read( &mbh, sizeof(mbh), &readen ) || readen != sizeof(mbh) )
    {
        GN_ERROR(sLogger)( "Fail to read mesh header." );
        return false;
    }

    // load vertex format
    vtxfmt = engine.createVtxFmt( "mesh vertex format", mbh.vtxfmt );
    if( 0 == vtxfmt ) return false;

    // store other fields
    primtype  = mbh.primtype;
    numprim   = mbh.numprim;
    startvtx  = mbh.startvtx;
    minvtxidx = mbh.minvtxidx;
    numvtx    = mbh.numvtx;
    startidx  = mbh.startidx;

    size_t offset = sizeof(BinaryFileHeader) + sizeof(MeshBinaryHeader);

    // load vertex buffers
    size_t numStreams = mbh.vtxfmt.calcNumStreams();
    GN_ASSERT( numStreams < 15 );
    vtxbufs.resize( numStreams );
    for( size_t i = 0; i < numStreams; ++i )
    {
        const MeshBinaryHeader::VtxBufHeader & vbh = mbh.vtxbuf[i];

        MeshVtxBuf & vb = vtxbufs[i];
        vb.offset = vbh.offset;
        vb.stride = vbh.stride;

        // calculate vb size
        size_t bytes = numvtx * vb.stride;

        // create new VB
        vb.buffer = engine.createVtxBuf( "mesh vertex buffer", bytes, !!vbh.dynamic, !!vbh.readback );
        if( 0 == vb.buffer ) return false;

        // load vb content
        AutoRef<MeshVtxBufLoader> loader( new MeshVtxBufLoader( fp.name(), offset, bytes ) );
        engine.updateResource( vb.buffer, 0, loader );

        // update offset
        offset += bytes;
    }

    // load index buffer
    {
        // calculate index buffer size
        size_t numidx = calcVertexCount( primtype, numprim );

        // create index buffer
        const MeshBinaryHeader::IdxBufHeader & ibh = mbh.idxbuf;
        idxbuf = engine.createIdxBuf( "mesh idxbuf", numidx, !!ibh.dynamic, !!ibh.readback );
        if( 0 == idxbuf ) return false;

        // load ib content
        size_t bytes  = numidx * 2; // 16-bit index buffer
        AutoRef<MeshIdxBufLoader> ibloader( new MeshIdxBufLoader( fp.name(), offset, bytes ) );
        engine.updateResource( idxbuf, 0, ibloader );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Mesh::loadFromFile( const StrA & filename )
{
    GN_GUARD;

    GN_INFO(sLogger)( "Load %s", filename.cptr() );

    // open mesh file
    AutoObjPtr<File> fp( core::openFile( filename, "rb" ) );
    if( !fp ) return false;

    // read file header
    BinaryFileHeader binheader;
    if( !fp->read( &binheader, sizeof(binheader), 0 ) ) return 0;
    if( 'G' == binheader.tag[0] && 'N' == binheader.tag[1] )
    {
        // load as mesh binary
        fp->seek( 0, FSEEK_SET );
        return loadFromBinaryStream( *fp );
    }
    else
    {
        // reopen in text mode
        fp.clear();
        fp.attach( core::openFile( filename, "rt" ) );
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
    StrA fullpath;
    normalizePathSeparator( fullpath, core::toNative( filename ) );

    // check if the entity exists already
    Entity * e = em.getEntityByName( fullpath, true );
    if( e ) return e;

    GN_INFO(sLogger)( "Create mesh entity: %s", fullpath.cptr() );

    // load mesh
    AutoObjPtr<Mesh> mesh( new Mesh(re) );
    if( !mesh->loadFromFile( fullpath ) ) return 0;

    e = em.createEntity<Mesh*>( getMeshEntityType(em), fullpath, mesh.get(), &safeDelete<Mesh> );
    if( 0 == e ) return 0;

    // success
    mesh.detach();
    return e;
}

/*
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::generateCubeMeshEntity(
    EntityManager & em, RenderEngine & re, const StrA & name, float edgeLength )
{
}*/

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
