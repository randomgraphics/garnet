#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.Texture");

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityTypeId GN::engine::getTextureEntityType( EntityManager & em )
{
    static EntityTypeId type = em.createEntityType( "texture" );
    return type;
}

/*
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::loadTextureEntity(
    EntityManager & em, RenderEngine & re, const StrA & filename )
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::createTextureEntity(
    EntityManager & em,
    RenderEngine & re,
    const StrA & name,
    const gfx::TextureDesc & desc )
{
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::deleteTextureEntity( Entity * )
{
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::deleteAllTextureEntitys( EntityManager & em )
{
}*/
