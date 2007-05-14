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

    ///
    /// try find exising texture entity named "filename", if not found, create new one.
    ///
    Entity * loadTextureEntity( EntityManager & em, RenderEngine & re, const StrA & filename );

    ///
    /// create new texture entity. name must be unique.
    ///
    Entity * createTextureEntity( EntityManager & em, RenderEngine & re, const StrA & name, const gfx::TextureDesc & desc );

    ///
    /// ..
    ///
    void deleteTextureEntity( Entity * );

    ///
    /// ..
    ///
    void deleteAllTextureEntitys( EntityManager & em );

    //@}
}}

// *****************************************************************************
//                           End of texture.h
// *****************************************************************************
#endif // __GN_ENGINE_TEXTURE_H__
