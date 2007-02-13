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
// get string value of specific attribute
// -----------------------------------------------------------------------------
template<typename T>
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
    	
    	<vtxbuf stream="0" ref="vb0.bin">

    	<vtxbuf stream="1" ref="vb1.bin">

    	<idxbuf ref="ib0.bin"/> <!-- this is optional -->
    </mesh>
*/

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Mesh::loadFromXml( const XmlNode * root, const StrA & meshdir, Renderer & r )
{
    if( 0 == root )
    {
        GN_ERROR(sLogger)( "NULL parameter!" );
        return false;
    }

    const XmlElement * e = root->toElement();
    if( 0 == e || e->name != "mesh" )
    {
        GN_ERROR(sLogger)( "root node must be \"<mesh>\"." );
        return false;
    }

    clear();

    StrA s;

    // get primitive type
    if( sGetStringAttrib( *e, "primType", s ) ) return false;
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

    // handle children
    for( XmlNode * c = root->child; c; c = c->sibling )
    {
        e = c->toElement();
        if( !e ) continue;

        // load vertex format
        VtxFmtDesc vfd;
        if( "vtxfmt" == e->name )
        {
            if( !vfd.loadFromXml( e ) ) return false;

            vtxfmt = r.createVtxFmt( vfd );
            if( 0 == vtxfmt ) return false;
        }

        // load vertex buffer
        else if( "vtxbuf" == e->name )
        {
            // get vertex stream index
            size_t stream;
            if( !sGetIntAttrib( *e, "stream", stream ) ) return false;
            if( stream > 0xFF )
            {
                GN_ERROR(sLogger)( "stream index (%d) is too large.", stream );
                return false;
            }

            // get vb file name
            StrA ref;
            if( !sGetStringAttrib( *e, "ref", ref ) ) return false;
            ref = fs::resolvePath( meshdir, ref );

            // open vb file
            AutoObjPtr<File> fp( fs::openFile( ref ) );
            if( fp.empty() ) return false;

            if( vtxbufs.size() < (stream+1) ) vtxbufs.resize( stream + 1 );
            AutoRef<VtxBuf> & vb = vtxbufs[stream];
        }

        // load index buffer
        else if( "idxbuf" == e->name )
        {
        }

        else
        {
            GN_ERROR(sLogger)( "Ignore unknown node '%s'.", e->name.cptr() );
        }
    }

    // success
    return true;
}
