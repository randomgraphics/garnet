#include "pch.h"
#include "garnet/gfx/renderable.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.Renderable");

// *****************************************************************************
// local code
// *****************************************************************************

using namespace GN;
using namespace GN::gfx;

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
static bool sLoadSubset(
    Effect                         * effect,
    Renderable::Subset             & ss,
    const XmlElement               & node,
    const StrA                     & basedir,
    ResourceManager<gfx::Mesh*>    & meshmgr,
    ResourceManager<gfx::Texture*> & texmgr )
{
    // load mesh
    XmlElement * e = node.findChildElement( "mesh" );
    if( e )
    {
        const StrA & ref = sGetStringAttrib( *e, "ref" );
        if( ref.empty() )
        {
            // embbed mesh
            AutoRef<Mesh> mesh( new Mesh );
            if( !mesh->loadFromXml( e, basedir, gRenderer ) ) return false;
            ss.mesh = mesh;
        }
        else
        {
            // external mesh
            ss.mesh.set( meshmgr.getResource( fs::resolvePath( basedir, ref ) ) );
            GN_ASSERT( ss.mesh );
        }
    }
    else
    {
        GN_ERROR(sLogger)( "mesh node is missing" );
        return false;
    }

    // load textures
    for( XmlNode * c = node.child; c; c = c->sibling )
    {
        XmlElement * e = c->toElement();
        if( !e ) continue;

        if( "texture" == e->name )
        {
            ss.textures.resize( ss.textures.size() + 1 );

            Renderable::TexItem & ti = ss.textures.back();

            // load texture ID
            const StrA & id = sGetStringAttrib( *e, "id" );
            if( id.empty() )
            {
                GN_ERROR(sLogger)( "id attribute of texture node is missing." );
                return false;
            }
            ti.id = effect->getTextureID( id );
            if( 0 == ti.id )
            {
                GN_DETAIL(sLogger)( "ignore unreferenced texture '%s'.", id.cptr() );
                continue;
            }

            // load texture itself
            const StrA & ref = sGetStringAttrib( *e, "ref" );
            if( !ref.empty() )
            {
                // load external texture
                ti.tex.set( texmgr.getResource( fs::resolvePath( basedir, ref ) ) );
                GN_ASSERT( ti.tex );
            }
        }
        else if( "effect" != e->name && "mesh" != e->name )
        {
            GN_ERROR(sLogger)( "Ignore unknown node '%s'.", e->name.cptr() );
        }
    }

    // success
    return true;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Renderable::draw() const
{
    GN_ASSERT( effect );

    Renderer & r = gRenderer;

    for( size_t i = 0; i < effect->getNumPasses(); ++i )
    {
        effect->passBegin( i );

        for( size_t i = 0; i < subsets.size(); ++i )
        {
            const Subset & ss = subsets[i];

            // bind textures
            for( size_t i = 0; i < ss.textures.size(); ++i )
            {
                const TexItem & ti = ss.textures[i];
                effect->setTexture( ti.id, ti.tex );
            }

            // bind mesh
            ss.mesh->updateContext( r );

            effect->commitChanges();

            ss.mesh->draw( r );
        }

        effect->passEnd();
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Renderable::loadFromXml(
    const XmlNode                  * root,
    const StrA                     & basedir,
    ResourceManager<gfx::Mesh*>    & meshmgr,
    ResourceManager<gfx::Effect*>  & effmgr,
    ResourceManager<gfx::Texture*> & texmgr )
{
    GN_GUARD;

    // check root node
    if( 0 == root )
    {
        GN_ERROR(sLogger)( "NULL parameter!" );
        return false;
    }
    const XmlElement * e = root->toElement();
    if( 0 == e || "renderable" != e->name )
    {
        GN_ERROR(sLogger)( "root node must be \"<renderable>\"." );
        return false;
    }

    // clear to empty
    clear();

    // load effect
    e = e->findChildElement( "effect" );
    if( e )
    {
        const StrA & ref = sGetStringAttrib( *e, "ref" );
        if( ref.empty() )
        {
            // do not support embedded effect, yet.
            GN_ERROR(sLogger)( "ref attribute of effect node is required." );
            return false;
        }
        else
        {
            // external effect
            effect.set( effmgr.getResource( fs::resolvePath( basedir, ref ) ) );
            GN_ASSERT( effect );
        }
    }
    else
    {
        GN_ERROR(sLogger)( "effect node is missing" );
        return false;
    }

    // load subsets
    for( XmlNode * c = root->child; c; c = c->sibling )
    {
        e = c->toElement();
        if( !e ) continue;

        if( "subset" == e->name )
        {
            subsets.resize( subsets.size() + 1 );
            if( !sLoadSubset( effect, subsets.back(), *e, basedir, meshmgr, texmgr ) )
            {
                // pop out problematic subset, then continue.
                subsets.pop_back();
            }
        }
        else if( "effect" != e->name )
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
bool GN::gfx::Renderable::loadFromXmlFile(
    File                           & fp,
    const StrA                     & basedir,
    ResourceManager<gfx::Mesh*>    & meshmgr,
    ResourceManager<gfx::Effect*>  & effmgr,
    ResourceManager<gfx::Texture*> & texmgr )
{
    GN_GUARD;

    XmlDocument doc;

    XmlParseResult xpr;

    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to read XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp.name().cptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }

    return loadFromXml( xpr.root, basedir, meshmgr, effmgr, texmgr );

    GN_UNGUARD;
}
