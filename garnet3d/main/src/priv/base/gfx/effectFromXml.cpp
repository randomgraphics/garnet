#include "pch.h"
#include "garnet/gfx/effect.h"
#include "garnet/base/xml.h"

// *****************************************************************************
// local functions
// *****************************************************************************

using namespace GN;
using namespace GN::gfx;

//
// get value of name attribute
// -----------------------------------------------------------------------------
static const StrA * sGetItemName( XmlElement & node, const char * nodeType )
{
    XmlAttrib * a = node.findAttrib( "name" );
    if( !a ) { GN_WARN( "ignore unnamed %s node.", nodeType ); return 0; }
    else return &a->name;
}

//
//
// -----------------------------------------------------------------------------
static void sParseTexture( EffectDesc & desc, XmlElement & node )
{
    GN_ASSERT( "texture" == node.name );

    const StrA * n = sGetItemName( node, "texture" );
    if( !n ) return;

    desc.textures[*n];
}

//
//
// -----------------------------------------------------------------------------
static void sParseUniform( EffectDesc & desc, XmlElement & node )
{
    const StrA * n = sGetItemName( node, "uniform" );
    if( !n ) return;

    EffectDesc::UniformDesc & ud = desc.uniforms[*n];
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
        else GN_WARN( "ignore unknown parameter node: %s", e->name.cptr() );
    }
}
//
//
// -----------------------------------------------------------------------------
static void sParseShader( EffectDesc & desc, XmlElement & node )
{
    GN_ASSERT( "shader" == node.name );

    const StrA * n = sGetItemName( node, "shader" );
    if( !n ) return;

    EffectDesc::ShaderDesc & sd = desc.shaders[*n];

    GN_UNUSED_PARAM( sd );
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

        if( "shader" != e->name )
        {
            GN_WARN( "ignore unknown shader node: %s", e->name.cptr() );
            continue;
        }

        sParseShader( desc, *e );
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
        else GN_WARN( "ignore unknown element: %s", e->name.cptr() );
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


