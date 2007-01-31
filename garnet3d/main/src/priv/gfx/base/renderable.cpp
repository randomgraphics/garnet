#include "pch.h"
#include "garnet/gfx/renderable.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.Renderable");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Renderable::init(
    const RenderableDesc & desc,
    ResourceManager<gfx::FatMesh*> * meshmgr,
    ResourceManager<gfx::Effect*> * effmgr,
    ResourceManager<gfx::Texture*> * texmgr )

{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::Renderable, () );

    // manager can't be NULL.
    if( !meshmgr || !effmgr || !texmgr )
    {
        GN_ERROR(sLogger)( "resource managers can't be NULL." );
        return failure();
    }

    // get resource handles.
    mSubsets.resize( desc.subsets.size() );
    for( size_t i = 0; i < desc.subsets.size(); ++i )
    {
        const RenderableSubsetDesc & sd = desc.subsets[i];
        Subset & ss = mSubsets[i];

        // get mesh handle
        ss.mesh = meshmgr->getResourceHandle( sd.mesh );
        if( 0 == ss.mesh ) return failure();

        // get effect handle
        ss.effect = effmgr->getResourceHandle( sd.effect );
        if( 0 == ss.effect ) return failure();

        // load effect
        Effect * eff = effmgr->getResource( ss.effect );
        GN_ASSERT( eff );

        // get texture ID and handles
        ss.textures.reserve( sd.textures.size() );
        for( std::map<StrA,StrA>::const_iterator t = sd.textures.begin(); t != sd.textures.end(); ++t )
        {
            TexItem ti;
            ti.id = eff->getTextureID( t->first );
            if( 0 == ti.id )
            {
                GN_WARN(sLogger)("ignore unused texture '%s'", t->first.cptr() );
            }
            else
            {
                ti.handle = texmgr->getResourceHandle( t->second );
                ss.textures.push_back( ti );
            };
        }
    }

    // store resource manager instances
    mEffectMgr = effmgr;
    mMeshMgr = meshmgr;
    mTextureMgr = texmgr;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Renderable::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Renderable::drawSubset( size_t i ) const
{
    GN_GUARD_SLOW;

    GN_ASSERT( i < mSubsets.size() );

    const Subset & ss = mSubsets[i];

    Effect * eff = mEffectMgr->getResource( ss.effect );
    GN_ASSERT( eff );

    // bind textures
    for( size_t i = 0; i < ss.textures.size(); ++i )
    {
        eff->setTexture( ss.textures[i].id, mTextureMgr->getResource( ss.textures[i].handle ) );
    }

    FatMesh * mesh = mMeshMgr->getResource( ss.mesh );

    // draw
    for( size_t i = 0; i < eff->getNumPasses(); ++i )
    {
        eff->passBegin( i );
        eff->commitChanges();
        mesh->draw( 0 );
        eff->passEnd();
    }

    GN_UNGUARD_SLOW;
}
