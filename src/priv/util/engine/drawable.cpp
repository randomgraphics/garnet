#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.Drawable");

using namespace GN;
using namespace GN::engine;

// *****************************************************************************
// local functions
// *****************************************************************************

//
// get string value of specific attribute
// -----------------------------------------------------------------------------
static const StrA & sGetStringAttrib( const XmlElement & node, const char * attribName, GN::StrA & defval = StrA::EMPTYSTR )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a )
    {
        return defval;
    }
    else
    {
        return a->value;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadRefAttrib( StrA & result, const XmlElement & node, const StrA & basedir, bool optional = false )
{
    const StrA & ref = sGetStringAttrib( node, "ref" );
    if( ref.empty() )
    {
        if( !optional ) GN_ERROR(sLogger)( "ref attribute of %s node is missing.", node.name.cptr() );
        return false;
    }

    core::resolvePath( result, basedir, ref );
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadRefElement(
    StrA             & result,
    const XmlElement & node,
    const StrA       & basedir,
    const StrA       & name )
{
    const XmlElement * e = node.findChildElement( name );

    if( !e )
    {
        GN_ERROR(sLogger)( "%s node is missing", name.cptr() );
        return false;
    }

    return sLoadRefAttrib( result, *e, basedir );
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Drawable::loadFromXmlNode(
    EntityManager & em,
    RenderEngine  & re,
    const XmlNode & root,
    const StrA    & basedir )
{
    GN_GUARD;

    // check root node
    const XmlElement * eroot = root.toElement();
    if( 0 == eroot || "drawable" != eroot->name )
    {
        GN_ERROR(sLogger)( "root node must be \"<drawable>\"." );
        return false;
    }

    // clear to empty
    clear();

    // load mesh
    StrA meshname;
    if( !sLoadRefElement( meshname, *eroot, basedir, "mesh" ) ) return false;
    mesh = loadMeshEntityFromFile( em, re, meshname );
    if( 0 == mesh ) return false;

    // load effect
    StrA effname;
    if( !sLoadRefElement( effname, *eroot, basedir, "effect" ) ) return false;
    effect = loadEffectEntityFromXmlFile( em,  re, effname );
    if( 0 == effect ) return false;

    Effect * effptr = entity2Object<Effect*>( effect, 0 );
    if( 0 == effptr ) return false;

    // load textures and uniforms
    for( const XmlNode * c = eroot->child; c; c = c->sibling )
    {
        const XmlElement * e = c->toElement();
        if( !e ) continue;

        if( "texture" == e->name )
        {
            // load texture binding
            const StrA & bindingstr = sGetStringAttrib( *e, "binding" );
            if( bindingstr.empty() )
            {
                GN_ERROR(sLogger)( "missing texture binding attribute." );
                return false;
            }
            gfx::EffectItemID binding;
            if( effptr->hasTexture( bindingstr, &binding ) )
            {
                GN_ASSERT( binding );

                // add to texture array
                TexItem & ti = textures[bindingstr];
                ti.binding = binding;

                StrA texname;
                if( sLoadRefAttrib( texname, *e, basedir, true ) )
                {
                    ti.texture = loadTextureEntityFromFile( em, re, texname );
                }
                else
                {
                    ti.texture = 0;
                }
            }
            else
            {
                GN_WARN(sLogger)( "ignore non-exist texture binding '%s'.", bindingstr.cptr() );
            }
        }
        else if( "uniform" == e->name )
        {
            // load uniform binding
            const StrA & bindingstr = sGetStringAttrib( *e, "binding" );
            if( bindingstr.empty() )
            {
                GN_ERROR(sLogger)( "missing uniform binding attribute." );
                return false;
            }

            gfx::EffectItemID binding;
            if( effptr->hasUniform( bindingstr, &binding ) )
            {
                GN_ASSERT( binding );

                // add to uniform array
                UniItem & ui = uniforms[bindingstr];
                ui.binding = binding;
                ui.value.loadFromXmlNode( *e );
            }
            else
            {
                GN_WARN(sLogger)( "ignore non-exist uniform binding '%s'.", bindingstr.cptr() );
            }
        }
        else if( "effect" != e->name && "mesh" != e->name )
        {
            GN_WARN(sLogger)( "Ignore unknown node '%s'.", e->name.cptr() );
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Drawable::loadFromXmlFile(
    EntityManager & em,
    RenderEngine  & re,
    const StrA    & filename )
{
    GN_GUARD;

    // open XML file
    AutoObjPtr<File> fp( core::openFile( filename, "rt" ) );
    if( !fp ) return false;

    // parse XML file
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

    StrA basedir = dirName( filename );

    return loadFromXmlNode( em, re, *xpr.root, basedir );

    GN_UNGUARD;
}
