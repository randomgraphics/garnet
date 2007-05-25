#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/vertexFormat.inl"
#endif

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
//
// *****************************************************************************

static Logger * sLogger = getLogger("GN.gfx.base.VertexFormatDesc");

static VtxFmtDesc sBuildXyzNormUv()
{
    VtxFmtDesc vfd;
    vfd.count = 3;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vfd.attribs[1].set( 0, 12, VTXSEM_NML0, FMT_FLOAT3 );
    vfd.attribs[2].set( 0, 24, VTXSEM_TEX0, FMT_FLOAT2 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 32 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

static VtxFmtDesc sBuildXyzUv()
{
    VtxFmtDesc vfd;
    vfd.count = 2;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vfd.attribs[1].set( 0, 12, VTXSEM_TEX0, FMT_FLOAT2 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 20 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

static VtxFmtDesc sBuildXyz()
{
    VtxFmtDesc vfd;
    vfd.count = 1;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 12 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

static VtxFmtDesc sBuildXyzwUv()
{
    VtxFmtDesc vfd;
    vfd.count = 2;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT4 );
    vfd.attribs[1].set( 0, 16, VTXSEM_TEX0, FMT_FLOAT2 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 24 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

static VtxFmtDesc sBuildXyzw()
{
    VtxFmtDesc vfd;
    vfd.count = 1;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT4 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 16 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

// *****************************************************************************
//
// *****************************************************************************

const VtxFmtDesc VtxFmtDesc::XYZ_NORM_UV = sBuildXyzNormUv();
const VtxFmtDesc VtxFmtDesc::XYZ_UV = sBuildXyzUv();
const VtxFmtDesc VtxFmtDesc::XYZ = sBuildXyz();
const VtxFmtDesc VtxFmtDesc::XYZW_UV = sBuildXyzwUv();
const VtxFmtDesc VtxFmtDesc::XYZW = sBuildXyzw();

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::VtxFmtDesc::validate() const
{
    if( 0 == count || count > MAX_VERTEX_ATTRIBUTES )
    {
        GN_ERROR(sLogger)( "numAttributes must be in range (0,MAX_VERTEX_ATTRIBUTES]." );
        return false;
    }
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::VtxFmtDesc::addAttrib(
    size_t stream,
    size_t offset,
    VtxSem semantic,
    ClrFmt format )
{
    if( count == MAX_VERTEX_ATTRIBUTES )
    {
        GN_ERROR(sLogger)( "Can't have more attributes." );
        return false;
    }
    // TODO: check input parameters
    attribs[count].set( stream, offset, semantic, format );
    ++count;

    GN_ASSERT( validate() );
    return true;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::VtxFmtDesc::AttribDesc *
GN::gfx::VtxFmtDesc::findAttrib( VtxSem semantic ) const
{
    for( size_t i = 0; i < count; ++i )
    {
        if( attribs[i].semantic == semantic ) return &attribs[i];
    }
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::VtxFmtDesc::calcNumStreams() const
{
    if( !validate() )
    {
        GN_ERROR(sLogger)( "invalid vertex format!" );
        return 0;
    }
    size_t n = 0;
    for( size_t i = 0; i < count; ++i )
    {
        const AttribDesc & a =  attribs[i];
        if( a.stream >= n ) n = a.stream + 1;
    }
    return n;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::VtxFmtDesc::calcStreamStride( size_t idx ) const
{
    if( !validate() )
    {
        GN_ERROR(sLogger)( "invalid vertex format!" );
        return 0;
    }
    if( idx >= calcNumStreams() )
    {
        GN_ERROR(sLogger)( "invalid stream index: %d!", idx );
        return 0;
    }
    size_t s = 0;
    for( size_t i = 0; i < count; ++i )
    {
        const AttribDesc & a =  attribs[i];
        if( a.stream == idx ) s += getClrFmtDesc(a.format).bits / 8;
    }
    return s;
}

// *****************************************************************************
// XML loader
// *****************************************************************************

/*

    XML definition is like this:

    <xml version="1.0" standalone="yes">
    <vtxfmt>
    	<attrib
    		stream   = "0"
    		offset   = "0"
    		semantic = "POS0"
    		format   = "FMT_FLOAT3"
    	/>
    	<attrib
    		stream   = "0"
    		offset   = "12"
    		semantic = "NRML"
    		format   = "FMT_FLOAT3"
    	/>
    	<attrib
    		stream   = "0"
    		offset   = "24"
    		semantic = "TEX0"
    		format   = "FMT_FLOAT2"
    	/>
    </vtxfmt>
    </xml>
*/

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

    // success
    return true;
}

//
// parse vertex attribute
// -----------------------------------------------------------------------------
static bool sParseAttribute( VtxFmtDesc & desc, const XmlElement & node )
{
    size_t stream;
    size_t offset;
    VtxSem semantic;
    ClrFmt format;

    // stream
    if( !sGetIntAttrib( node, "stream", stream ) ) return false;

    // offset
    if( !sGetIntAttrib( node, "offset", offset ) ) return false;

    // semantic
    const XmlAttrib * a = node.findAttrib( "semantic" );
    if( !a ) { sPostError( node, "semantic is missing" ); return false; }
    if( 4 != a->value.size() )
    {
        sPostError( node, "semantic must be FOURCC." );
        return false;
    }
    semantic.fromStr( a->value.cptr() );

    // format
    a = node.findAttrib( "format" );
    if( !a ) { sPostError( node, "format is missing" ); return false; }
    format = str2ClrFmt( a->value );
    if( FMT_UNKNOWN == format )
    {
        sPostError( node, strFormat( "invalid format : %s", a->value.cptr() ) );
        return false;
    }

    return desc.addAttrib( stream, offset, semantic, format );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::VtxFmtDesc::loadFromXml( const XmlNode * root )
{
    if( 0 == root )
    {
        GN_ERROR(sLogger)( "NULL parameter!" );
        return false;
    }

    const XmlElement * e = root->toElement();
    if( 0 == e || e->name != "vtxfmt" )
    {
        GN_ERROR(sLogger)( "root node must be \"<vtxfmt>\"." );
        return false;
    }

    clear();

    for( const XmlNode * n = e->child; n; n = n->sibling )
    {
        e = n->toElement();
        if( !e ) continue;

        if( "attrib" == e->name ) sParseAttribute( *this, *e );
        else sPostError( *e, strFormat( "Unknown node '%s'. Ignored", e->name.cptr() ) );
    }

    return true;
}
