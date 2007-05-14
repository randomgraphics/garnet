//
//
// -----------------------------------------------------------------------------
template<class T>
inline GN::engine::EntityT<T> *
GN::engine::EntityManager::createEntity( EntityTypeId type, const StrA & name, const T & data )
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

    EntityCategory & ec = mEntityTypes[type];

    // create new ID
    UIntPtr id = ec.entities.newItem();

    // create new entity
    EntityItem<T> * e = new (std::nothrow) EntityItem<T>( *this, name, type, id, data );
    if( 0 == e )
    {
        GN_ERROR(sLogger)( "out of memory!" );
        ec.entities.remove( id );
        return 0;
    }

    // add to name list
    mEntityNames.insert( std::make_pair( name, e ) );

    // success
    return e;
}
