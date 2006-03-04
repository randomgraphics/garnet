#include "pch.h"
#include "oglRenderer.h"
#if !GN_ENABLE_INLINE
#include "oglTextureMgr.inl"
#endif
#include "oglTexture.h"

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::supportTextureFormat( TexType, BitField, ClrFmt ) const
{
    GN_UNIMPL_WARNING();
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::OGLRenderer::createTexture( TexType  type,
                                     size_t   sx, size_t sy, size_t sz,
                                     size_t   faces,
                                     size_t   levels,
                                     ClrFmt   format,
                                     BitField usage,
                                     const TextureLoader & loader )
{
    GN_GUARD;

    AutoRef<OGLTexture> p( new OGLTexture(*this) );
    p->setLoader( loader );
    if( !p->init(type,sx,sy,sz,faces,levels,format,usage) ) 0;
    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// private function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::applyTexture() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( getDirtyTextureStages() > 0 );

    size_t maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);

    size_t numtex = min<size_t>( maxStages, getDirtyTextureStages() );

    const Texture * const * texlist = getCurrentTextures();

    // apply texture list
    size_t i;
    for ( i = 0; i < numtex; ++i )
    {
        // if null handle, then disable this texture stage
        if( texlist[i] )
        {
            chooseTextureStage( i );
            safeCast<const OGLTexture *>(texlist[i])->bind();
        }
        else
        {
            disableTextureStage( i );
        }
    }

    // disable remaining texture stages
    for( ; i < maxStages; ++i )
    {
        disableTextureStage( i );
    }

    clearDirtyTextureStages();

    GN_UNGUARD_SLOW;
}
