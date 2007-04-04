#include "pch.h"
#include "garnet/gfx/mesh.h"
#include <pcrecpp.h>

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
bool GN::gfx::Mesh::loadFromXmlNode( const XmlNode & root, const StrA & basedir )
{
    GN_GUARD;

    Renderer & r = gRenderer;

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
    primtype = str2PrimitiveType( s );
    if( primtype >= NUM_PRIMITIVES ) return false;

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
    vtxfmt = r.createVtxFmt( vfd );
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

            // get optional vb properties
            bool dynamic = sGetOptionalBoolAttrib( *e, "dynamic", false );
            bool readback = sGetOptionalBoolAttrib( *e, "readback", false );

            // create new VB
            size_t bytes = vb.stride * numvtx;
            vb.buffer.attach( r.createVtxBuf( bytes, dynamic, readback ) );
            if( vb.buffer.empty() ) return false;

            // lock vb
            UInt8 * dst = (UInt8*)vb.buffer->lock( 0, 0, LOCK_WO );
            if( 0 == dst ) return false;
            AutoSurfaceUnlocker<VtxBuf> unlocker( vb.buffer );
 
            StrA ref;
            if( sGetStringAttrib( *e, "ref", ref, true ) )
            {
                // read mesh from external file

                ref = core::resolvePath( basedir, ref );

                // open vb file
                AutoObjPtr<File> fp( core::openFile( ref, "rb" ) );
                if( fp.empty() ) return false;

                // read data into vb
                if( !fp->read( dst, bytes, 0 ) ) return false;
            }
            else
            {
                // read embbed vertex buffer. Now can only support floating data.
                size_t numFloats = 0;
                for( size_t i = 0; i < vfd.count; ++i )
                {
                    switch( vfd.attribs[i].format )
                    {
                        case FMT_FLOAT1 : numFloats += 1; break;
                        case FMT_FLOAT2 : numFloats += 2; break;
                        case FMT_FLOAT3 : numFloats += 3; break;
                        case FMT_FLOAT4 : numFloats += 4; break;
                        default:
                            sPostError( *e, "Emmbed vertex buffer can't have data other then floats." );
                            return false;
                    }
                }

                // float array that can hold one vertex data
                DynaArray<float> vertex;
                vertex.resize( numFloats );

                // define pattern for "," separated float array.
                static pcrecpp::RE re( "\\s*([+-]?\\s*([0-9]+(\\.[0-9]*)?|[0-9]*\\.[0-9]+)([eE][+-]?[0-9]+)?)\\s*,?\\s*" );

                // parse vertex data
                pcrecpp::StringPiece text( e->text.cptr(), (int)e->text.size() );
                std::string substring;
                for( size_t i = 0; i < numvtx; ++i, dst += vb.stride )
                {
                    for( size_t j = 0; j < numFloats; ++j )
                    {
                        if( !re.Consume( &text, &substring ) ||
                            !str2Float( vertex[j], substring.c_str() ) )
                        {
                            sPostError( *e, strFormat( "vertex %dst has invalid data", i ) );
                            return false;
                        }
                    }

                    // copy to vertex buffer
                    memcpy( dst, vertex.cptr(), vertex.size()*sizeof(float) );
                 }
            }
        }

        // load index buffer
        else if( "idxbuf" == e->name )
        {
            // get optional vb properties
            bool dynamic = sGetOptionalBoolAttrib( *e, "dynamic", false );
            bool readback = sGetOptionalBoolAttrib( *e, "readback", false );

            // create new ib
            size_t numidx = calcVertexCount( primtype, numprim );
            size_t bytes = numidx * 2; // 16-bit index buffer
            idxbuf.attach( r.createIdxBuf( bytes, dynamic, readback ) );
            if( idxbuf.empty() ) return false;

            // lock ib
            UInt16 * dst = (UInt16*)idxbuf->lock( 0, 0, LOCK_WO );
            if( 0 == dst ) return false;
            AutoSurfaceUnlocker<IdxBuf> unlocker( idxbuf );

            // get ib file name
            StrA ref;
            if( sGetStringAttrib( *e, "ref", ref, true ) )
            {
                // read external ib adata
                ref = core::resolvePath( basedir, ref );

                AutoObjPtr<File> fp( core::openFile( ref, "rb" ) );
                if( fp.empty() ) return false;

                if( !fp->read( dst, bytes, 0 ) ) return false;
            }
            else
            {
                // read embedded ib data
                static pcrecpp::RE re( "\\s*([0-9]+)\\s*,?\\s*" );

                pcrecpp::StringPiece text( e->text.cptr(), (int)e->text.size() );
                std::string substring;
                for( size_t i = 0; i < numidx; ++i, ++dst )
                {
                    if( !re.Consume( &text, &substring ) ||
                        !str2UInt16( *dst, substring.c_str() ) )
                    {
                        sPostError( *e, strFormat( "index %d has invalid data", i ) );
                        return false;
                    }
                }
            }
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

// *****************************************************************************
// global functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::generateCubeMesh( Mesh & mesh, float edgeLength )
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
}
