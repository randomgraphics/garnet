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
    deleteAllEntities();

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

//
//
// -----------------------------------------------------------------------------
bool GN::scene::World::Impl::hasEntityFactory( const Guid & type )
{
    return NULL != getManager( type );
}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::World::Impl::registerEntityFactory( const Guid & type, const char * desc, EntityFactory factory, const void * factoryParameter )
{
    if( hasEntityFactory( type ) )
    {
        GN_ERROR(sLogger)( "Entity type exisits already!" );
        return false;
    }

    if( NULL == factory.initializeEntity )
    {
        GN_ERROR(sLogger)( "Entity factory has NULL function pointer(s)." );
        return false;
    }

    if( mManagers.size() == mManagers.MAX_SIZE )
    {
        GN_ERROR(sLogger)( "Entity manager pool is full. Cannot register more entity types!" );
        return false;
    }

    mManagers.resize( mManagers.size() + 1 );

    EntityManager & mgr = mManagers.back();

    mgr.guid = type;
    mgr.desc = desc ? desc : "unnamed entity type";
    mgr.index = mManagers.size() - 1;
    mgr.factory = factory;
    mgr.param = factoryParameter;
    GN_ASSERT( mgr.entities.empty() );

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::unregisterEntityFactory( const Guid & )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
EntityFactory GN::scene::World::Impl::getEntityFactory( const Guid & type )
{
    EntityManager * mgr = getManager( type );

    if( mgr )
    {
        return mgr->factory;
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid entity type: %s", type.toStr() );
        EntityFactory dummy = { NULL };
        return dummy;
    }
}

//
//
// -----------------------------------------------------------------------------
Entity * GN::scene::World::Impl::createEntity( const Guid & type, const char * name )
{
    EntityManager * mgr = getManager( type );

    if( !mgr )
    {
        GN_ERROR(sLogger)( "Entity creation failed: invalid entity type: %s", type.toStr() );
        return NULL;
    }

    // compose a unique name for unnamed entity
    char unnamed[256];
    if( 0 == name || 0 == *name )
    {
        static int i = 0;
        ++i;
        strPrintf( unnamed, GN_ARRAY_COUNT(unnamed), "Unnamed %s #%d", mgr->desc.cptr(), i );
        name = unnamed;

        GN_ASSERT( !mgr->entities.validName( name ) );
    }

    if( mgr->entities.validName( name ) )
    {
        GN_ERROR(sLogger)( "Entity creation failed: entity named '%s' exists already." );
        return NULL;
    }

    // create new ID
    EntityID id( mgr->index, mgr->entities.add(name) );
    if( 0 == id.internalHandle() ) return false;

    // create new entity
    AutoObjPtr<EntityInternal> newEntity( new EntityInternal( mOwner, id.i32() ) );
    if( !newEntity ||
        !mgr->factory.initializeEntity( *newEntity, mgr->param ) )
    {
        mgr->entities.remove( id.internalHandle() );
        return NULL;
    }

    // done
    return newEntity.detach();
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::deleteEntity( const Guid & type, const char * name )
{
    EntityManager * mgr = getManager( type );

    if( !mgr )
    {
        GN_ERROR(sLogger)( "Entity deletion failed: invalid entity type: %s", type.toStr() );
        return;
    }

    if( !mgr->entities.validName( name ) )
    {
        GN_ERROR(sLogger)( "Entity deletion failed: invalid entity named '%s'.", name ? name : "<NULL_NAME>" );
        return;
    }

    UInt32 internalHandle = mgr->entities.name2handle( name );

    delete (EntityInternal*)mgr->entities[internalHandle];

    mgr->entities.remove( internalHandle );
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::deleteEntity( int id32 )
{
    EntityID id( id32 );

    if( id.managerIndex() >= mManagers.size() )
    {
        GN_ERROR(sLogger)( "Entity deletion failed: invalid ID" );
        return;
    }

    EntityManager & mgr = mManagers[id.managerIndex()];

    if( !mgr.entities.validHandle( id.internalHandle() ) )
    {
        GN_ERROR(sLogger)( "Entity deletion failed: invalid ID" );
        return;
    }

    delete (EntityInternal*)mgr.entities[id.internalHandle()];

    mgr.entities.remove( id.internalHandle() );
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::World::Impl::deleteEntity( Entity * entity )
{
    if( NULL == entity ) return;

    if( this != entity->world().mImpl )
    {
        GN_ERROR(sLogger)( "Entity deletion failed: the entity is from another world." );
        return;
    }

    deleteEntity( entity->id() );
}

//
//
// -----------------------------------------------------------------------------;
void GN::scene::World::Impl::deleteAllEntities()
{
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        EntityManager & m = mManagers[i];

        for( UInt32 h = m.entities.first(); h != 0; h = m.entities.next( h ) )
        {
            delete (EntityInternal*)m.entities[h];
        }
    }
}

//
//
// -----------------------------------------------------------------------------
Entity * GN::scene::World::Impl::findEntity( const Guid & type, const char * name )
{
    EntityManager * mgr = getManager( type );

    if( !mgr ) return NULL;

    if( !mgr->entities.validName( name ) ) return NULL;

    return mgr->entities[name];
}

//
//
// -----------------------------------------------------------------------------
Entity * GN::scene::World::Impl::findEntity( int id32 )
{
    EntityID id( id32 );

    if( id.managerIndex() >= mManagers.size() ) return NULL;

    EntityManager & mgr = mManagers[id.managerIndex()];

    if( !mgr.entities.validHandle( id.internalHandle() ) ) return NULL;

    return mgr.entities[id.internalHandle()];
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::scene::World::Impl::getEntityType( int id32 ) const
{
    EntityID id( id32 );

    if( id.managerIndex() >= mManagers.size() )
    {
        GN_ERROR(sLogger)( "Entity deletion failed: invalid ID" );
        static const Guid INVALID_TYPE = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
        return INVALID_TYPE;
    }

    return mManagers[id.managerIndex()].guid;
}

//
//
// -----------------------------------------------------------------------------
const char  * GN::scene::World::Impl::getEntityName( int id32 ) const
{
    EntityID id( id32 );

    if( id.managerIndex() >= mManagers.size() )
    {
        GN_ERROR(sLogger)( "Fail to get reosource name: invalid ID" );
        return NULL;
    }

    const EntityManager & mgr = mManagers[id.managerIndex()];

    const char * name = mgr.entities.handle2name(id.internalHandle());

    if( NULL == name )
    {
        GN_ERROR(sLogger)( "Fail to get reosource name: Invalid ID." );
    }

    return name;
}

// *****************************************************************************
// World::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::World::Impl::EntityManager *
GN::scene::World::Impl::getManager( const Guid & type ) const
{
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        const EntityManager & m = mManagers[i];
        if( type == m.guid )
        {
            return const_cast<EntityManager*>( &m );
        }
    }

    return NULL;
}

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
