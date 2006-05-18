#include "pch.h"
#include "garnet/gfx/effect.h"
#include "garnet/base/xml.h"

// *****************************************************************************
// local functions
// *****************************************************************************

using namespace GN;
using namespace GN::gfx;

//
// post error message
// -----------------------------------------------------------------------------
static void sPostError( const XmlNode & node, const StrA & msg )
{
    GN_UNUSED_PARAM( node );
    GN_ERROR( "%s", msg.cptr() );
}

//
// get value of specific attribute
// -----------------------------------------------------------------------------
static const char * sGetAttrib(
    XmlElement & node,
    const char * attribName,
    const char * defaultValue = NULL )
{
    XmlAttrib * a = node.findAttrib( attribName );
    return a ? a->name.cptr() : defaultValue;
}

//
// get value of specific attribute
// -----------------------------------------------------------------------------
template<typename T>
static T sGetIntAttrib( XmlElement & node, const char * attribName, T defaultValue )
{
    XmlAttrib * a = node.findAttrib( attribName );

    T result;

    if( !a || !str2Int<T>( result, a->value.cptr() ) )
        return defaultValue;
    else
        return result;
}

//
// get value of name attribute
// -----------------------------------------------------------------------------
static const char * sGetItemName( XmlElement & node, const char * nodeType )
{
    XmlAttrib * a = node.findAttrib( "name" );
    if( !a )
    {
        sPostError( node, strFormat("Unnamed %s node. Ignored.", nodeType) );
        return 0;
    }
    return a->name.cptr();
}

//
//
// -----------------------------------------------------------------------------
static void sParseTexture( EffectDesc & desc, XmlElement & node )
{
    GN_ASSERT( "texture" == node.name );

    const char * n = sGetItemName( node, "texture" );
    if( !n ) return;

    desc.textures[n];
}

//
//
// -----------------------------------------------------------------------------
static void sParseUniform( EffectDesc & desc, XmlElement & node )
{
    const char * n = sGetItemName( node, "uniform" );
    if( !n ) return;

    EffectDesc::UniformDesc & ud = desc.uniforms[n];
    ud.hasDefaultValue = false;

    // TODO: parse default uniform value
}

//
//
// -----------------------------------------------------------------------------
static void sParseParameters( EffectDesc & desc, XmlNode & root )
{
    for( XmlNode * n = root.child; n; n = n->sibling )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "texture" == e->name ) sParseTexture( desc, *e );
        else if( "uniform" == e->name ) sParseUniform( desc, *e );
        else sPostError( *e, "Unknown parameter. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseTexref( EffectDesc & desc, EffectDesc::ShaderDesc & sd, XmlElement & node )
{
    const char * name = sGetAttrib( node, "name" );
    if( !name ) return sPostError( node, "no reference name" );

    uint32_t stage = sGetIntAttrib( node, "stage", (uint32_t)-1 );
    if( (uint32_t)-1 == stage ) return sPostError( node, "no stage" );

    const EffectDesc::TextureDesc * td = desc.findTexture( name );
    if( !td ) return sPostError( node, "Invalid texture reference" );

    sd.textures[stage] = name;
}

//
//
// -----------------------------------------------------------------------------
static void sParseUniref( EffectDesc & desc, EffectDesc::ShaderDesc & sd, XmlElement & node )
{
    const char * name = sGetAttrib( node, "name" );
    if( !name ) return sPostError( node, "no reference name" );

    const char * binding = sGetAttrib( node, "binding" );
    if( !binding ) return sPostError( node, "no binding" );

    const EffectDesc::UniformDesc * ud = desc.findUniform( name );
    if( !ud ) return sPostError( node, "Invalid uniform reference" );

    sd.uniforms[binding] = name;
}

//
//
// -----------------------------------------------------------------------------
static void sParseConditions( EffectDesc::ShaderDesc & sd, XmlElement & node )
{
    GN_UNUSED_PARAM( sd );
    GN_UNUSED_PARAM( node );
    GN_TODO( "parse shader conditions" );
}

//
//
// -----------------------------------------------------------------------------
static void sParseCode( EffectDesc::ShaderDesc & sd, XmlElement & node )
{
    for( XmlNode * n = node.child; n; n = n->sibling )
    {
        XmlCdata * c = n->toCdata();
        if( c )
        {
            sd.code = c->text;
            return;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseShader( EffectDesc & desc, XmlElement & node )
{
    GN_ASSERT( "shader" == node.name );

    // get shader name
    const char * name = sGetItemName( node, "shader" );
    if( !name ) return;

    EffectDesc::ShaderDesc sd;

    // get shader type
    const char * type = sGetAttrib( node, "type" );
    if( !str2ShaderType( sd.type, type ) )
    {
        sPostError( node, "invalid shader type" );
        return;
    }

    // get shading language
    const char * lang = sGetAttrib( node, "lang" );
    if( !str2ShadingLanguage( sd.lang, lang ) )
    {
        sPostError( node, "invalid shading language" );
        return;
    }

    // get hints
    sd.hints = sGetAttrib( node, "hints", "" );

    // parse children
    for( XmlNode * n = node.child; n; n = n->sibling )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "texref" == e->name ) sParseTexref( desc, sd, *e );
        else if( "uniref" == e->name ) sParseUniref( desc, sd, *e );
        else if( "conditions" == e->name ) sParseConditions( sd, *e );
        else if( "code" == e->name ) sParseCode( sd, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }

    // success
    desc.shaders[name] = sd;
}

//
//
// -----------------------------------------------------------------------------
static void sParseShaders( EffectDesc & desc, XmlElement & node )
{
    GN_ASSERT( "shaders" == node.name );

    for( XmlNode * n = node.child; n; n = n->sibling )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "shader" == e->name ) sParseShader( desc, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseRsb( RenderStateBlockDesc & rsb, XmlElement & node )
{
    GN_UNUSED_PARAM( rsb );
    GN_UNUSED_PARAM( node );
    GN_TODO( "parse render states" );
}

//
//
// -----------------------------------------------------------------------------
static const char * sGetShaderRef(
    EffectDesc & desc,
    XmlElement & node,
    const char * attribName,
    ShaderType type )
{
    const char * ref = sGetAttrib( node, attribName );
    if( !ref )
    {
        sPostError( node, strFormat("attrib %s not found",attribName) );
        return 0;
    }

    const EffectDesc::ShaderDesc * sd = desc.findShader( ref );
    if( !sd ) { sPostError( node, "Invalid shader reference" ); return 0; }

    if( type != sd->type ) { sPostError( node, "Incompatible shader type." ); return 0; }

    return ref;
}

//
//
// -----------------------------------------------------------------------------
static void sParsePass( EffectDesc & desc, EffectDesc::TechniqueDesc & td, XmlElement & node )
{
    const char * vs = sGetShaderRef( desc, node, "vs", VERTEX_SHADER );
    const char * ps = sGetShaderRef( desc, node, "ps", PIXEL_SHADER );
    if( !vs || !ps ) return;

    td.passes.resize( td.passes.size() + 1 );
    EffectDesc::PassDesc & pd = td.passes.back();

    pd.shaders[VERTEX_SHADER] = vs;
    pd.shaders[PIXEL_SHADER] = ps;

    for( XmlNode * n = node.child; n; n = n->sibling )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "rsb" == e->name ) sParseRsb( pd.rsb, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseTechnique( EffectDesc & desc, XmlElement & node )
{
    GN_ASSERT( "technique" == node.name );

    // get technique name
    const char * name = sGetItemName( node, "technique" );
    if( !name ) return;

    EffectDesc::TechniqueDesc td;
    td.name = name;

    // parse children
    for( XmlNode * n = node.child; n; n = n->sibling )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "rsb" == e->name ) sParseRsb( td.rsb, *e );
        else if( "pass" == e->name ) sParsePass( desc, td, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseTechniques( EffectDesc & desc, XmlElement & node )
{
    GN_ASSERT( "techniques" == node.name );

    for( XmlNode * n = node.child; n; n = n->sibling )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "rsb" == e->name ) sParseRsb( desc.rsb, *e );
        else if( "technique" == e->name ) sParseTechnique( desc, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sDoParse( EffectDesc & desc, XmlNode * root )
{
    desc.clear();

    if( 0 == root ) return true; // empty effect

    XmlElement * e = root->toElement();

    if( 0 == e ||e->name != "effect" )
    {
        GN_ERROR( "root node must be \"<effect>\"." );
        return false;
    }

    for( XmlNode * n = e->child; n; n = n->sibling )
    {
        e = n->toElement();
        if( !e ) continue;

        if( "parameters" == e->name ) sParseParameters( desc, *e );
        else if( "shaders" == e->name ) sParseShaders( desc, *e );
        else if( "techniques" == e->name ) sParseTechniques( desc, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }

    return true;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectDesc::fromXml( const char * str, size_t size )
{
    GN_GUARD;

    XmlDocument doc;

    XmlParseResult xpr;

    if( !doc.parse( xpr, str, size ) )
    {
        GN_ERROR(
            "Fail to read XML string:\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }

    return sDoParse( *this, xpr.root );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectDesc::fromXml( File & fp )
{
    GN_GUARD;

    XmlDocument doc;

    XmlParseResult xpr;

    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(
            "Fail to read XML file %s:\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp.name().cptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }

    return sDoParse( *this, xpr.root );

    GN_UNGUARD;
}


