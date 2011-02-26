#include "pch.h"

using namespace GN;
using namespace GN::engine;

GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Component
// *****************************************************************************


// *****************************************************************************
// Entity
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity::Entity()
{
    mID = impl::onEntityCtor( this );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity::~Entity()
{
    impl::onEntityDtor( mID );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Component * GN::engine::Entity::getComponent( const Guid & type ) const
{
    Component ** pp = mComponents.find( type );
    if( pp )
    {
        GN_ASSERT( (*pp) );
        return *pp;
    }
    else
    {
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::Entity::setComponent( const Guid & type, Component * comp )
{
    if( NULL != comp && NULL != comp->mEntity )
    {
        GN_ERROR(sLogger)( "The component has attached to another entity." );
        return;
    }

    Component ** pp = mComponents.find( type );

    if( NULL != pp )
    {
        Component * old = *pp;
        GN_ASSERT( old );

        if( old != comp )
        {
            if( NULL != comp )
            {
                // replace existing component

                comp->mEntity = this;
                comp->incref();
                *pp = comp;

                old->mEntity = NULL;
                old->decref();
            }
            else
            {
                // remove existing component
                old->mEntity = NULL;
                old->decref();
                mComponents.remove( type );
            }
        }
    }
    else if( NULL != comp )
    {
        // add new component
        mComponents[type] = comp;
        comp->mEntity = this;
    }
    else
    {
        GN_ERROR(sLogger)( "NULL component pointer!" );
    }
}

// *****************************************************************************
// Global Engine functions
// *****************************************************************************

struct EngineWorld
{
    // Entity manager
    //@{
    typedef HandleManager<engine::Entity*,int> EntityHandleManager;
    Mutex               entityLock;
    EntityHandleManager entities;
    //@}

    // Graphics stuff
    //@{
    AutoObjPtr<gfx::Gpu>                 gpu;
    AutoObjPtr<gfx::GpuResourceDatabase> gdb;
    AutoObjPtr<gfx::SpriteRenderer>      spriteRenderer;
    AutoObjPtr<gfx::BitmapFont>          fontRenderer;
    //@}
};

static EngineWorld s_engine;

//
//
// -----------------------------------------------------------------------------
bool GN::engine::initialize()
{
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::shutdown()
{
    gfxShutdown();
    inputShutdown();
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::gfxInitialize()
{
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::gfxShutdown()
{
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::inputInitialize()
{
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::inputShutdown()
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Gpu * GN::engine::getGpu()
{
    return s_engine.gpu;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResourceDatabase * GN::engine::getGdb()
{
    return s_engine.gdb;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::SpriteRenderer * GN::engine::getSpriteRenderer()
{
    return s_engine.spriteRenderer;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::BitmapFont * GN::engine::getBmfRenderer()
{
    return s_engine.fontRenderer;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::findEntity( int id )
{
    ScopeMutex<> lock( s_engine.entityLock );

    if( s_engine.entities.validHandle( id ) )
    {
        return s_engine.entities[id];
    }
    else
    {
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
int GN::engine::impl::onEntityCtor( Entity * e )
{
    GN_ASSERT( e );

    ScopeMutex<> lock( s_engine.entityLock );

    return s_engine.entities.add( e );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::impl::onEntityDtor( int id )
{
    ScopeMutex<> lock( s_engine.entityLock );

    GN_VERIFY( s_engine.entities.remove( id ) );
}
