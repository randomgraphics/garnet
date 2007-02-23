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
static const StrA & sGetStringAttrib( const XmlElement & node, const char * attribName, GN::StrA & default = StrA::EMPTYSTR )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a )
    {
        return default;
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

    ResourceId id = rm.getResourceId( fs::resolvePath( basedir, ref ) );
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
            gfx::EffectItemID binding = effptr->getTextureID( bindingstr );
            if( 0 == binding )
            {
                GN_DETAIL(sLogger)( "invalid texture binding '%s'.", bindingstr.cptr() );
                return false;
            }

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
        else if( "uniform" == e->name )
        {
            // load uniform binding
            const StrA & bindingstr = sGetStringAttrib( *e, "binding" );
            if( bindingstr.empty() )
            {
                GN_ERROR(sLogger)( "missing uniform binding attribute." );
                return false;
            }
            gfx::EffectItemID binding = effptr->getUniformID( bindingstr );
            if( 0 == binding )
            {
                GN_DETAIL(sLogger)( "invalid uniform binding '%s'.", bindingstr.cptr() );
                return false;
            }

            // add to uniform array
            UniItem & ui = uniforms[bindingstr];
            ui.binding = binding;
            ui.value.loadFromXmlNode( *e );
        }
        else if( "effect" != e->name && "mesh" != e->name )
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
bool GN::scene::Drawable::loadFromXmlFile( File & fp, const StrA & basedir )
{
    GN_GUARD;

    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp.name().cptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }
    GN_ASSERT( xpr.root );
    return loadFromXmlNode( *xpr.root, basedir );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::Drawable::loadFromXmlFile( const StrA & filename )
{
    GN_GUARD;

    GN_INFO(sLogger)( "Load drawable from '%s'", filename.cptr() );

    AutoObjPtr<File> fp( fs::openFile( filename, "rt" ) );
    if( !fp ) return false;

    StrA basedir = dirName( filename );

    return loadFromXmlFile( *fp, basedir );

    GN_UNGUARD;
}
