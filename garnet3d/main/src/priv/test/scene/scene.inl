
// ****************************************************************************
// World class
// ****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline GN::scene::Entity *
GN::scene::World::findOrCreateEntity( const Guid & type, const char * name )
{
    Entity * e = findEntity( type, name );
    if( !e ) e = createEntity( type, name );
    return e;
}
