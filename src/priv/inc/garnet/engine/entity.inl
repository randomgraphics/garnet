//
//
// -----------------------------------------------------------------------------
template<class T>
inline GN::engine::EntityT<T> *
GN::engine::EntityManager::createEntity( EntityTypeId type, const StrA & name, const T & data, const Delegate1<void,T&> & dtor )
{
    if( !mEntityTypes.validHandle( type ) )
    {
        GN_ERROR(sLogger)( "invalid entity type: %d", type );
        return 0;
    }
    if( mEntityNames.end() != mEntityNames.find( name ) )
    {
        GN_ERROR(sLogger)( "entity named '%s' does exist." );
        return 0;
    }
    if( dtor.empty() )
    {
        GN_WARN(sLogger)( "entity named '%s' has no user data destructor." );
    }

    EntityCategory & ec = mEntityTypes[type];

    // create new ID
    UIntPtr id = ec.entities.newItem();

    // create new entity
    EntityItemT<T> * e = new (std::nothrow) EntityItemT<T>( *this, name, type, id, data, dtor );
    if( 0 == e )
    {
        GN_ERROR(sLogger)( "out of memory!" );
        ec.entities.remove( id );
        return 0;
    }

    // add to handle list
    ec.entities[id] = e;

    // add to name list
    mEntityNames.insert( std::make_pair( name, e ) );

    // success
    return e;
}

//
//
// -----------------------------------------------------------------------------
template< class T>
inline const T & GN::engine::entity2Object( const Entity * e, const T & nil )
{
    if( 0 == e ) return nil;
    const EntityT<T> * et = safeCastPtr<const EntityT<T> >(e);
    if( 0 == et )
    {
        static Logger * sLogger = getLogger("GN.engine.Entity");
        GN_ERROR(sLogger)( "incorrect entity type" );
        return nil;
    }
    return et->data;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::safeDeleteEntity( const Entity * & e )
{
    if( 0 == e ) return;
    e->manager.deleteEntity( e );
    e = 0;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::safeDeleteEntity( Entity * & e )
{
    if( 0 == e ) return;
    e->manager.deleteEntity( e );
    e = 0;
}

///
/// get next entity. Return NULL if there's no more entities
// -----------------------------------------------------------------------------
inline GN::engine::Entity * GN::engine::getNextEntity( const Entity * e )
{
    if( 0 == e )
    {
        static Logger * sLogger = getLogger("GN.engine.Entity");
        GN_ERROR(sLogger)( "NULL argument" );
        return 0;
    }
    return e->manager.getNextEntity( e );
}

///
/// get next entity with same type as input entity. Return NULL if there's no more entities
// -----------------------------------------------------------------------------
inline GN::engine::Entity * GN::engine::getNextEntityWithSameType( const Entity * e )
{
    if( 0 == e )
    {
        static Logger * sLogger = getLogger("GN.engine.Entity");
        GN_ERROR(sLogger)( "NULL argument" );
        return 0;
    }
    return e->manager.getNextEntityWithSameType( e );
}
