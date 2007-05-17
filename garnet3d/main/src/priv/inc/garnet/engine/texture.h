#ifndef __GN_ENGINE_TEXTURE_H__
#define __GN_ENGINE_TEXTURE_H__
// *****************************************************************************
//! \file    engine/texture.h
//! \brief   texture manager
//! \author  chenli@@FAREAST (2007.5.9)
// *****************************************************************************

namespace GN { namespace engine
{
    //@{

    EntityTypeId getTextureEntityType( EntityManager & em );

    inline GraphicsResource * entity2Texture( const Entity * e, GraphicsResource * nil = 0 )
    {
        GN_ASSERT( 0 == e || e->type == getTextureEntityType( e->manager ) );
        return entity2Object<GraphicsResource*>( e, nil );
    }

    ///
    /// try find exising texture entity named "filename", if not found, create new one.
    ///
    Entity * loadTextureEntityFromFile( EntityManager & em, RenderEngine & re, const StrA & filename );

    ///
    /// create new texture entity (content of the texture is leaving undefined)
    ///
    /// \return
    ///     Return NULL, if entity with this name does exist already.
    ///
    Entity * createTextureEntity( EntityManager & em, RenderEngine & re, const StrA & name, const gfx::TextureDesc & desc );

    //@}
}}

// *****************************************************************************
//                           End of texture.h
// *****************************************************************************
#endif // __GN_ENGINE_TEXTURE_H__
