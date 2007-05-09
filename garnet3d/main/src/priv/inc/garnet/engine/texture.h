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

    GraphicsResourceId entity2Texture( EntityId );

    ///
    /// try find exising texture entity named "filename", if not found, create new one.
    ///
    EntityId loadTexture( EntityManager & em, RenderEngine & re, const StrA & filename );

    ///
    /// create new texture entity. name must be unique.
    ///
    EntityId createTexture( EntityManager & em, RenderEngine & re, const StrA & name, const gfx::TextureDesc & desc );

    ///
    /// ..
    ///
    void deleteTextureEntity( EntityManager & em, EntityId );

    ///
    /// ..
    ///
    void deleteAllTextureEntities( EntityManager & em );

    //@}
}}

// *****************************************************************************
//                           End of texture.h
// *****************************************************************************
#endif // __GN_ENGINE_TEXTURE_H__
