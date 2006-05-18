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
    GN_UNUSED_PARAM( desc );
    GN_UNUSED_PARAM( sd );
    GN_UNUSED_PARAM( node );
}

//
//
// -----------------------------------------------------------------------------
static void sParseUniref( EffectDesc & desc, EffectDesc::ShaderDesc & sd, XmlElement & node )
{
    GN_UNUSED_PARAM( desc );
    GN_UNUSED_PARAM( sd );
    GN_UNUSED_PARAM( node );
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
    EffectDesc::ShaderDesc & sd = desc.shaders[name];

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
        //else if( "conditions" == e->name ) sParseConditions( desc, sd, *e );
        //else if( "code" == e->name ) sParseCode( sd, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }

    // get shader code

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
static void sParseTechniques( EffectDesc &, XmlElement & )
{
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


