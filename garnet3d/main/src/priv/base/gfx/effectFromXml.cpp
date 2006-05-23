#include "pch.h"
#include "garnet/gfx/effect.h"
#include "garnet/base/xml.h"
#include <pcrecpp.h>

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
    const XmlElement & node,
    const char * attribName,
    const char * defaultValue = NULL )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    return a ? a->value.cptr() : defaultValue;
}

//
// get value of specific attribute
// -----------------------------------------------------------------------------
template<typename T>
static T sGetIntAttrib( const XmlElement & node, const char * attribName, T defaultValue )
{
    const XmlAttrib * a = node.findAttrib( attribName );

    T result;

    if( !a || !str2Int<T>( result, a->value.cptr() ) )
        return defaultValue;
    else
        return result;
}

//
// get value of name attribute
// -----------------------------------------------------------------------------
static const char * sGetItemName( const XmlElement & node, const char * nodeType )
{
    XmlAttrib * a = node.findAttrib( "name" );
    if( !a )
    {
        sPostError( node, strFormat("Unnamed %s node. Ignored.", nodeType) );
        return 0;
    }
    return a->value.cptr();
}

//
//
// -----------------------------------------------------------------------------
static void sParseTexture( EffectDesc & desc, const XmlElement & node )
{
    GN_ASSERT( "texture" == node.name );

    const char * n = sGetItemName( node, "texture" );
    if( !n ) return;

    desc.textures[n];
}

//
//
// -----------------------------------------------------------------------------
static bool sParseFloats( float * buffer, size_t count, const XmlElement & node )
{
    if( node.text.empty() )
    {
        sPostError( node, "missing text" );
        return false;
    }

    static pcrecpp::RE re( "\\s*([+-]?\\s*([0-9]+(\\.[0-9]*)?|[0-9]*\\.[0-9]+)([eE][+-]?[0-9]+)?)\\s*,?\\s*" );

    std::string substring;
    pcrecpp::StringPiece text( node.text.cptr() );
    for( size_t i = 0; i < count; ++i )
    {
        if( !re.Consume( &text, &substring ) ||
            !str2Float( *buffer, substring.c_str() ) )
        {
            sPostError( node, strFormat("invalid float number : index(%d)",i) );
            return false;
        }

        ++buffer; // next float
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
static void sParseUniform( EffectDesc & desc, const XmlElement & node )
{
    const char * n = sGetItemName( node, "uniform" );
    if( !n ) return;

    EffectDesc::UniformDesc & ud = desc.uniforms[n];
    ud.hasDefaultValue = false;

    // parse uniform value
    const XmlElement * e = node.child ? node.child->toElement() : NULL;
    if( !e ) return;
    if( "matrix44" == e->name )
    {
        Matrix44f m;
        if( sParseFloats( m[0], 16, *e ) )
        {
            ud.hasDefaultValue = true;
            ud.defaultValue.setM( m );
        }
    }
    else if( "vector4" == e->name )
    {
        Vector4f v;
        if( sParseFloats( v, 4, *e ) )
        {
            ud.hasDefaultValue = true;
            ud.defaultValue.setV( v );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseParameters( EffectDesc & desc, const XmlNode & root )
{
    for( const XmlNode * n = root.child; n; n = n->sibling )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "texture" == e->name ) sParseTexture( desc, *e );
        else if( "uniform" == e->name ) sParseUniform( desc, *e );
        else sPostError( *e, "Unknown parameter. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseTexref( EffectDesc & desc, EffectDesc::ShaderDesc & sd, const XmlElement & node )
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
static void sParseUniref( EffectDesc & desc, EffectDesc::ShaderDesc & sd, const XmlElement & node )
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
static bool sParseConditionToken( EffectDesc::ShaderDesc & sd, const XmlElement & node )
{
    if( "token" != node.name )
    {
        sPostError( node, "node name must be \"token\" here.!" );
        return false;
    }

    // parse the token
    EffectDesc::Token t;
    StrA type = sGetAttrib( node, "type" );
    StrA value = sGetAttrib( node, "value" );
    if( "opcode" == type )
    {
        t.type = EffectDesc::OPCODE;
        t.opcode = EffectDesc::sStr2OpCode( value );
        if( EffectDesc::OPCODE_INVALID == t.opcode )
        {
            sPostError( node, strFormat( "invalid opcode: %s", value.cptr() ) );
            return false;
        }
    }
    else if( "values" == type )
    {
        t.setS( value );
    }
    else if( "valuei" == type )
    {
        t.type = EffectDesc::VALUEI;
        if( !str2Int32( t.valueI, value.cptr() ) )
        {
            sPostError( node, strFormat( "invalid integer: %s", value.cptr() ) );
            return false;
        }
    }
    else
    {
        sPostError( node, strFormat("invalid t type: %s",type.cptr()) );
        return false;
    }

    // append to token array
    sd.conditions.tokens.push_back( t );

    // parse child tokens
    for( const XmlNode * n = node.child; n; n = n->sibling )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;
        if( !sParseConditionToken( sd, *e ) ) return false;
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
static void sParseConditions( EffectDesc::ShaderDesc & sd, const XmlElement & node )
{
    GN_ASSERT( "conditions" == node.name );

    for( const XmlNode * n = node.child; n; n = n->sibling )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;
        if( !sParseConditionToken( sd, *e ) )
        {
            sd.conditions.tokens.clear();
            break;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseCode( EffectDesc::ShaderDesc & sd, const XmlElement & node )
{
    for( const XmlNode * n = node.child; n; n = n->sibling )
    {
        const XmlCdata * c = n->toCdata();
        if( c )
        {
            // get shader code
            sd.code = c->text;

            if( !sd.code.empty() )
            {
                // get hints
                sd.hints = sGetAttrib( node, "hints", "" );

                // get shading language
                const char * lang = sGetAttrib( node, "lang" );
                if( !str2ShadingLanguage( sd.lang, lang ) )
                {
                    sPostError( node, strFormat("invalid shading language: %s",lang?lang:"") );
                }
            }

            return;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseShader( EffectDesc & desc, const XmlElement & node )
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
        sPostError( node, strFormat("invalid shader type: %s",type?type:"") );
        return;
    }

    // parse children
    for( const XmlNode * n = node.child; n; n = n->sibling )
    {
        const XmlElement * e = n->toElement();
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
static void sParseShaders( EffectDesc & desc, const XmlElement & node )
{
    GN_ASSERT( "shaders" == node.name );

    for( const XmlNode * n = node.child; n; n = n->sibling )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "shader" == e->name ) sParseShader( desc, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseRsb( RenderStateBlockDesc & rsb, const XmlElement & node )
{
    RenderState rs;
    RenderStateValue rsv;
    for( const XmlAttrib * a = node.attrib; a; a = a->next )
    {
        if( !str2RenderState( rs, a->name.cptr() ) )
        {
            sPostError( node, strFormat( "invalid render state: %s", a->name.cptr() ) );
            continue;
        }

        const RenderStateDesc & rsdesc = getRenderStateDesc( rs );

        switch( rsdesc.valueType )
        {
            case RenderStateDesc::VT_ENUM:
                if( !str2RenderStateValue( rsv, a->value.cptr() ) )
                {
                    sPostError( node, strFormat(
                        "invalid render state value: %s (for render state %s)",
                        a->value.cptr(), a->name.cptr() ) );
                    continue;
                }
                rsb.set( rs, rsv );
                break;

            default: GN_UNEXPECTED();
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static const char * sGetShaderRef(
    EffectDesc & desc,
    const XmlElement & node,
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
static void sParsePass( EffectDesc & desc, EffectDesc::TechniqueDesc & td, const XmlElement & node )
{
    const char * vs = sGetShaderRef( desc, node, "vs", VERTEX_SHADER );
    const char * ps = sGetShaderRef( desc, node, "ps", PIXEL_SHADER );
    if( !vs || !ps ) return;

    td.passes.resize( td.passes.size() + 1 );
    EffectDesc::PassDesc & pd = td.passes.back();

    pd.shaders[VERTEX_SHADER] = vs;
    pd.shaders[PIXEL_SHADER] = ps;

    for( const XmlNode * n = node.child; n; n = n->sibling )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "rsb" == e->name ) sParseRsb( pd.rsb, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseTechnique( EffectDesc & desc, const XmlElement & node )
{
    GN_ASSERT( "technique" == node.name );

    // get technique name
    const char * name = sGetItemName( node, "technique" );
    if( !name ) return;

    EffectDesc::TechniqueDesc td;
    td.name = name;

    // parse children
    for( const XmlNode * n = node.child; n; n = n->sibling )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "rsb" == e->name ) sParseRsb( td.rsb, *e );
        else if( "pass" == e->name ) sParsePass( desc, td, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }

    desc.techniques.push_back( td );
}

//
//
// -----------------------------------------------------------------------------
static void sParseTechniques( EffectDesc & desc, const XmlElement & node )
{
    GN_ASSERT( "techniques" == node.name );

    for( const XmlNode * n = node.child; n; n = n->sibling )
    {
        const XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "rsb" == e->name ) sParseRsb( desc.rsb, *e );
        else if( "technique" == e->name ) sParseTechnique( desc, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sDoParse( EffectDesc & desc, const XmlNode * root )
{
    desc.clear();

    if( 0 == root ) return true; // empty effect

    const XmlElement * e = root->toElement();

    if( 0 == e ||e->name != "effect" )
    {
        GN_ERROR( "root node must be \"<effect>\"." );
        return false;
    }

    for( const XmlNode * n = e->child; n; n = n->sibling )
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


