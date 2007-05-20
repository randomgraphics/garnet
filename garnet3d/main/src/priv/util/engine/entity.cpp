#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.Entity");
GN::Logger * GN::engine::EntityManager::sLogger = GN::getLogger("GN.engine.Entity");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityManager::EntityManager()
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityManager::~EntityManager()
{
    deleteAllEntities();
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityTypeId GN::engine::EntityManager::createEntityType( const StrA & name )
{
    if( mEntityTypes.validName(name) )
    {
        GN_ERROR(sLogger)( "entity type named '%s' does already exist!", name.cptr() );
        return 0;
    }

    return mEntityTypes.add( name );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::EntityManager::checkEntityType( EntityTypeId type, bool silence ) const
{
    if( mEntityTypes.validHandle( type ) ) return true;

    if( !silence )
    {
        GN_ERROR(sLogger)( "invalid entity type: %d", type );
    }

    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::EntityManager::checkEntity( const Entity * e, bool silence ) const
{
    struct Local
    {
        static inline bool failure( const Entity * e, bool silence )
        {
            if( !silence )
            {
                GN_ERROR(sLogger)( "invalid entity pointer : 0x%p.", e );
            }
            return false;
        }
    };

    if( 0 == e ) return Local::failure( e, silence );
    if( this !=&e->manager ) return Local::failure( e, silence );
    if( mEntityNames.end() == mEntityNames.find( e->name ) ) return Local::failure( e, silence );
    if( e != mEntityNames.find(e->name)->second ) return Local::failure( e, silence );
    if( !mEntityTypes.validHandle(e->type) ) return Local::failure( e, silence );
    if( !mEntityTypes[e->type].entities.validHandle( e->id ) ) return Local::failure( e, silence );
    if( e != mEntityTypes[e->type].entities[e->id] ) return Local::failure( e, silence );

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::EntityManager::deleteEntity( const Entity * e )
{
    if( 0 == e || !checkEntity( e ) ) return;

    EntityCategory & ec = mEntityTypes[e->type];

    // remove from ec
    ec.entities.remove( e->id );

    // remove from name list
    mEntityNames.erase( e->name );

    // delete the entity instance
    const EntityDeletor * ed = (const EntityDeletor*)e;
    delete ed;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::EntityManager::deleteEntityByName( const StrA & name )
{
    deleteEntity( getEntityByName( name ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::EntityManager::deleteAllEntities()
{
    std::map<StrA,Entity*>::const_iterator i;
    for( i = mEntityNames.begin(); i != mEntityNames.end(); ++i )
    {
        const EntityDeletor * ed = (const EntityDeletor*)(i->second);
        GN_ASSERT( ed );
        delete ed;
    }

    mEntityNames.clear();

    EntityTypeId type = mEntityTypes.first();
    while( type )
    {
        mEntityTypes[type].entities.clear();
        type = mEntityTypes.next( type );
    }
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::EntityManager::getEntityByName( const StrA & name, bool silence ) const
{
    std::map<StrA,Entity*>::const_iterator i = mEntityNames.find( name );

    if( mEntityNames.end() == i )
    {
        if( !silence )
        {
            GN_ERROR(sLogger)( "invalid entity name: %s.", name.cptr() );
        }
        return 0;
    }

    GN_ASSERT( i->second );

    return i->second;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityTypeId GN::engine::EntityManager::getEntityTypeByName( const StrA & name, bool silence ) const
{
    EntityTypeId type = mEntityTypes.name2handle( name );

    if( 0 == type && !silence )
    {
        GN_ERROR(sLogger)( "invalid entity name: %s.", name.cptr() );
    }

    return type;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::EntityManager::getFirstEntity() const
{
    if( mEntityTypes.empty() ) return 0;

    const EntityCategory & ec = mEntityTypes[mEntityTypes.first()];

    if( ec.entities.empty() ) return 0;

    return ec.entities[ec.entities.first()];
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::EntityManager::getFirstEntity( EntityTypeId type ) const
{
    if( !checkEntityType( type ) ) return 0;

    GN_ASSERT( !mEntityTypes.empty() );

    const EntityCategory & ec = mEntityTypes[mEntityTypes.first()];

    if( ec.entities.empty() ) return 0;

    return ec.entities[ec.entities.first()];
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::EntityManager::getNextEntity( const Entity * e )
{
    if( !checkEntity( e ) ) return 0;

    EntityCategory & ec1 = mEntityTypes[e->type];

    UIntPtr next = ec1.entities.next( e->id );

    if( 0 != next ) return ec1.entities[next];

    EntityTypeId nexttype = mEntityTypes.next( e->type );

    if( 0 == nexttype ) return 0;

    EntityCategory & ec2 = mEntityTypes[nexttype];

    if( ec2.entities.empty() ) return 0;

    return ec2.entities[ec2.entities.first()];
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::EntityManager::getNextEntityWithSameType( const Entity * e )
{
    if( !checkEntity( e ) ) return 0;

    EntityCategory & ec = mEntityTypes[e->type];

    UIntPtr next = ec.entities.next( e->id );

    return ( 0 != next ) ? ec.entities[next] : 0;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityTypeId GN::engine::EntityManager::getFirstEntityType() const
{
    return mEntityTypes.first();
}

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityTypeId GN::engine::EntityManager::getNextEntityType( EntityTypeId type ) const
{
    if( !checkEntityType( type ) ) return 0;
    return mEntityTypes.next( type );
}
