#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.scene.Drawable");

using namespace GN;
using namespace GN::scene;

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
static ResourceId sLoadReference(
    const XmlElement & node,
    const StrA & basedir,
    const StrA & name )
{
    const XmlElement * e = node.findChildElement( name );
    if( !e )
    {
        GN_ERROR(sLogger)( "%s node is missing", name.cptr() );
        return 0;
    }

    const StrA & ref = sGetStringAttrib( *e, "ref" );
    if( ref.empty() )
    {
        GN_ERROR(sLogger)( "ref attribute of %s node is missing.", name.cptr() );
        return 0;
    }

    ResourceManager & rm = gSceneResMgr;

    ResourceId id = rm.getResourceId( core::resolvePath( basedir, ref ) );
    if( 0 == id )
    {
        GN_ERROR(sLogger)( "Invalid referencing to %s '%s'.", name.cptr(), ref.cptr() );
        return 0;
    }

    // sucess
    return id;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::scene::Drawable::loadFromXmlNode( const XmlNode & root, const StrA & basedir )
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

    ResourceManager & rm = gSceneResMgr;

    // load mesh
    mesh = sLoadReference( *eroot, basedir, "mesh" );
    if( 0 == mesh ) return false;

    // load effect
    effect = sLoadReference( *eroot, basedir, "effect" );
    if( 0 == effect ) return false;
    gfx::Effect * effptr = rm.getResourceT<gfx::Effect>( effect );
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

                ResourceId id;
                if( e->findAttrib( "ref" ) )
                {
                    id = sLoadReference( *eroot, basedir, "texture" );
                    if( 0 == id ) continue;
                }
                else
                {
                    id = 0;
                }

                // add to texture array
                TexItem & ti = textures[bindingstr];
                ti.binding = binding;
                ti.texid = id;
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
