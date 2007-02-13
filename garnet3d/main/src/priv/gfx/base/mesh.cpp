#include "pch.h"
#include "garnet/gfx/mesh.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.Mesh");

using namespace GN;

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
        sPostError( node, strFormat( "attribute '%s' is missing!" ) );
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
static bool sGetOptionalBoolAttrib( const XmlElement & node, const char * attribName, bool default )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if( !a ) return default;

    return 0 == strCmpI( "yes", a->name.cptr() )
        || 0 == strCmpI( "true", a->name.cptr() )
        || 0 == strCmp( "1", a->name.cptr() );
}

//
// get string value of specific attribute
// -----------------------------------------------------------------------------
static bool sGetStringAttrib( const XmlElement & node, const char * attribName, GN::StrA & result )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a )
    {
        sPostError( node, strFormat( "attribute '%s' is missing!" ) );
        return false;
    }
    else
    {
        result = a->value;
        return true;
    }
}

// *****************************************************************************
// public functions
// *****************************************************************************

/*
    <mesh
    	primType  = "TRIANGLE_LIST"
    	primCount = "100"
    	startVtx  = "0"
    	minVtxIdx = "0"
    	numVtx    = "30"
    	startIdx  = "0"
    	>

    	<vtxfmt>
    		...
    	</vtxfmt>
    	
    	<vtxbuf
    		stream  = "0"
    		offset  = "0"
    		stride  = "32"
    		dynamic = "true" // optional, default is false.
    		syscopy = "true" // optional, default is false.
    		ref="vb0.bin"
    	/>

    	<vtxbuf
    		stream = "1"
    		offset = "100"
    		stride = "16"
    		ref    = "vb1.bin"
    	/>

    	<idxbuf
    		dynamic = "true" // optional, default is false.
    		syscopy = "true" // optional, default is false.
    		ref="ib0.bin"
    	/> <!-- this is optional -->
    </mesh>
*/

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Mesh::loadFromXml( const XmlNode * root, const StrA & meshdir, Renderer & r )
{
    GN_GUARD;

    // check root node
    if( 0 == root )
    {
        GN_ERROR(sLogger)( "NULL parameter!" );
        return false;
    }
    const XmlElement * e = root->toElement();
    if( 0 == e || "mesh" != e->name )
    {
        GN_ERROR(sLogger)( "root node must be \"<mesh>\"." );
        return false;
    }

    // clear to empty
    clear();

    // get primitive type
    StrA s;
    if( !sGetStringAttrib( *e, "primType", s ) ) return false;
    primType = str2PrimitiveType( s );
    if( primType >= NUM_PRIMITIVES ) return false;

    // get primitive count
    if( !sGetIntAttrib( *e, "primCount", primCount ) ) return false;

    // get start vertex index
    if( !sGetIntAttrib( *e, "startVtx", startVtx ) ) return false;

    // get minimal vertex index
    if( !sGetIntAttrib( *e, "minVtxIdx", minVtxIdx ) ) return false;

    // get vertex count
    if( !sGetIntAttrib( *e, "numVtx", numVtx ) ) return false;

    // get start index
    if( !sGetIntAttrib( *e, "startIdx", startIdx ) ) return false;

    // load vertex format
    XmlNode * vfnode = e->findChildElement( "vtxfmt" );
    if( !vfnode )
    {
        GN_ERROR(sLogger)( "No valid vertex format definition found!" );
        return false;
    }
    VtxFmtDesc vfd;
    if( !vfd.loadFromXml( vfnode ) ) return false;
    vtxfmt = r.createVtxFmt( vfd );
    if( 0 == vtxfmt ) return false;

    // handle child elements
    size_t numStreams = vfd.calcNumStreams();
    for( XmlNode * c = root->child; c; c = c->sibling )
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
                GN_ERROR(sLogger)( "stream index (%d) is too large.", stream );
                return false;
            }

            if( vtxbufs.size() < (stream+1) ) vtxbufs.resize( stream + 1 );
            MeshVtxBuf & vb = vtxbufs[stream];

            // get stream offset and stride
            if( !sGetIntAttrib( *e, "offset", vb.offset ) ) return false;
            if( !sGetIntAttrib( *e, "stride", vb.stride ) ) return false;

            // get optional vb properties
            bool dynamic = sGetOptionalBoolAttrib( *e, "dynamic", false );
            bool syscopy = sGetOptionalBoolAttrib( *e, "syscopy", false );

            // get vb file name
            StrA ref;
            if( !sGetStringAttrib( *e, "ref", ref ) ) return false;
            ref = fs::resolvePath( meshdir, ref );

            // open vb file
            AutoObjPtr<File> fp( fs::openFile( ref, "rb" ) );
            if( fp.empty() ) return false;

            // create new VB
            size_t bytes = vb.stride * numVtx;
            vb.buffer.attach( r.createVtxBuf( bytes, dynamic, syscopy ) );
            if( vb.buffer.empty() ) return false;

            // lock vb
            void * dst = vb.buffer->lock( 0, 0, LOCK_WO );
            if( 0 == dst ) return false;
            AutoBufferUnlocker<VtxBuf> unlocker( vb.buffer );

            // read data into vb
            if( !fp->read( dst, bytes, 0 ) ) return false;
        }

        // load index buffer
        else if( "idxbuf" == e->name )
        {
            // get optional vb properties
            bool dynamic = sGetOptionalBoolAttrib( *e, "dynamic", false );
            bool syscopy = sGetOptionalBoolAttrib( *e, "syscopy", false );

            // get ib file name
            StrA ref;
            if( !sGetStringAttrib( *e, "ref", ref ) ) return false;
            ref = fs::resolvePath( meshdir, ref );

            // open ib file
            AutoObjPtr<File> fp( fs::openFile( ref, "rb" ) );
            if( fp.empty() ) return false;

            // create new ib
            size_t bytes = calcVertexCount( primType, primCount ) * 2; // 16-bit index buffer
            idxbuf.attach( r.createIdxBuf( bytes, dynamic, syscopy ) );
            if( idxbuf.empty() ) return false;

            // lock ib
            void * dst = idxbuf->lock( 0, 0, LOCK_WO );
            if( 0 == dst ) return false;
            AutoBufferUnlocker<IdxBuf> unlocker( idxbuf );

            // read data into ib
            if( !fp->read( dst, bytes, 0 ) ) return false;
        }

        else if( "vtxfmt" != e->name )
        {
            GN_ERROR(sLogger)( "Ignore unknown node '%s'.", e->name.cptr() );
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Mesh::loadFromXmlFile( File & fp, const StrA & meshdir, Renderer & r )
{
    GN_GUARD;

    XmlDocument doc;

    XmlParseResult xpr;

    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to read XML file:\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }

    return loadFromXml( xpr.root, meshdir, r );

    GN_UNGUARD;
}
