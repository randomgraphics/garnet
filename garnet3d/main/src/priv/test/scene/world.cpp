#include "pch.h"
#include "world.h"
#include "spatialNode.h"
#include "visualNode.h"
#include "lightNode.h"

using namespace GN;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene");

// *****************************************************************************
// EntityInternal class
// *****************************************************************************

class EntityInternal : public Entity
{
public:

    EntityInternal( World & world, int id ) : Entity(world, id) {}

    ~EntityInternal() {}
};

// *****************************************************************************
// Build-in entity types
// *****************************************************************************

const Guid GN::scene::SPATIAL_ENTITY = { 0x2a28d02a, 0x64c1, 0x4e6e, { 0xb1, 0x9c, 0xdc, 0x5a, 0x78, 0xad, 0x30, 0x96 } };
const Guid GN::scene::VISUAL_ENTITY  = { 0x44125e51, 0x7037, 0x499d, { 0x8d, 0x15, 0x16, 0x01, 0x5a, 0xfd, 0xc3, 0xf3 } };
const Guid GN::scene::LIGHT_ENTITY   = { 0x64c543ac, 0x6c7c, 0x4781, { 0xbb, 0x28, 0x37, 0x56, 0x00, 0xba, 0xa6, 0xf5 } };

// *****************************************************************************
// World::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::clear()
{
    // delete all entities
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        EntityManager & m = mManagers[i];

        for( UInt32 h = m.entities.first(); h != 0; h = m.entities.next( h ) )
        {
            delete (EntityInternal*)m.entities[h];
        }
    }

    // clear everything
    mManagers.clear();

    // register built-in factories
    EntityFactory spatialFactory = { &sInitSpatialEntity };
    EntityFactory visualFactory  = { &sInitVisualEntity };
    EntityFactory lightFactory   = { &sInitLightEntity };
    registerEntityFactory( SPATIAL_ENTITY, "Build-in spatial entity", spatialFactory, this );
    registerEntityFactory( VISUAL_ENTITY, "Build-in visual entity", visualFactory, this );
    registerEntityFactory( LIGHT_ENTITY, "Build-in light entity", lightFactory, this );
}

#pragma warning( disable: 4100 ) // unreferneced formal parameter

//
//
// -----------------------------------------------------------------------------
bool GN::scene::World::Impl::hasEntityFactory( const Guid & type )
{
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::World::Impl::registerEntityFactory( const Guid & type, const char * desc, EntityFactory factory, const void * factoryParameter )
{
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::unregisterEntityFactory( const Guid & type )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
EntityFactory GN::scene::World::Impl::getEntityFactory( const Guid & type )
{
    GN_UNIMPL();
    EntityFactory dummy = { NULL };
    return dummy;
}

//
//
// -----------------------------------------------------------------------------
Entity * GN::scene::World::Impl::createEntity( const Guid & type, const char * name )
{
    GN_UNIMPL();
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::deleteEntity( const Guid & type, const char * name )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::deleteEntity( int id )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::deleteEntity( Entity * entity )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------;
void GN::scene::World::Impl::deleteAllEntities()
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
Entity * GN::scene::World::Impl::findEntity( const Guid & type, const char * name )
{
    GN_UNIMPL();
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
Entity * GN::scene::World::Impl::findEntity( int id )
{
    GN_UNIMPL();
    return NULL;
}

// *****************************************************************************
// World::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::scene::World::Impl::sInitSpatialEntity( Entity & entity, const void * parameters )
{
    GN_ASSERT( parameters );

    World::Impl * w = (World::Impl*)parameters;

    AutoObjPtr<SpatialNode> sn( newSpatialNode( entity, w->mSpatialGraph ) );
    if( !sn ) return false;

    entity.attachNode<SpatialNode>( sn.detach() );

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::World::Impl::sInitVisualEntity( Entity & entity, const void * parameters )
{
    GN_ASSERT( parameters );

    World::Impl * w = (World::Impl*)parameters;

    AutoObjPtr<SpatialNode> sn( newSpatialNode( entity, w->mSpatialGraph ) );
    if( !sn ) return false;

    AutoObjPtr<VisualNode> vn( newVisualNode( entity, w->mVisualGraph ) );
    if( !vn ) return false;

    entity.attachNode<SpatialNode>( sn.detach() );
    entity.attachNode<VisualNode>( vn.detach() );

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::World::Impl::sInitLightEntity( Entity & entity, const void * parameters )
{
    GN_ASSERT( parameters );

    World::Impl * w = (World::Impl*)parameters;

    AutoObjPtr<SpatialNode> sn( newSpatialNode( entity, w->mSpatialGraph ) );
    if( !sn ) return false;

    AutoObjPtr<LightNode> ln( newLightNode( entity, w->mVisualGraph ) );
    if( !ln ) return false;

    entity.attachNode<SpatialNode>( sn.detach() );
    entity.attachNode<LightNode>( ln.detach() );

    return true;
}

// *****************************************************************************
// World public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::World::World() : mImpl(NULL)
{
    mImpl = new Impl( *this );

    // this will register all built-in factories
    mImpl->clear();
}

//
//
// -----------------------------------------------------------------------------
GN::scene::World::~World()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
void          GN::scene::World::clear() { return mImpl->clear(); }

bool          GN::scene::World::hasEntityFactory( const Guid & type ) { return mImpl->hasEntityFactory( type ); }
bool          GN::scene::World::registerEntityFactory( const Guid & type, const char * desc, EntityFactory factory, const void * factoryParameter ) { return mImpl->registerEntityFactory( type, desc, factory, factoryParameter ); }
void          GN::scene::World::unregisterEntityFactory( const Guid & type ) { return mImpl->unregisterEntityFactory( type ); }
EntityFactory GN::scene::World::getEntityFactory( const Guid & type ) { return mImpl->getEntityFactory( type ); }

Entity      * GN::scene::World::createEntity( const Guid & type, const char * name ) { return mImpl->createEntity( type, name ); }
void          GN::scene::World::deleteEntity( const Guid & type, const char * name ) { return mImpl->deleteEntity( type, name ); }
void          GN::scene::World::deleteEntity( int id ) { return mImpl->deleteEntity( id ); }
void          GN::scene::World::deleteEntity( Entity * entity ) { return mImpl->deleteEntity( entity ); }
void          GN::scene::World::deleteAllEntities() { return mImpl->deleteAllEntities(); }
Entity      * GN::scene::World::findEntity( const Guid & type, const char * name ) { return mImpl->findEntity( type, name ); }
Entity      * GN::scene::World::findEntity( int id ) { return mImpl->findEntity( id ); }
