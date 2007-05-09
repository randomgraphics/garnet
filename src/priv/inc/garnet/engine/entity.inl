//
//
// -----------------------------------------------------------------------------
//template<class T>
inline GN::engine::EntityId
GN::engine::EntityManager::newEntity( EntityTypeId type, const T & data, const StrA & name )
{
    if( 0 == mTypes.validHandle( type ) )
    {
        GN_ERROR(sLogger)( "invalid entity type: %d", type );
        return 0;
    }

    
}
