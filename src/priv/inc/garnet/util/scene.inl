// ****************************************************************************
// Entity class
// ****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline GN::gfx::GpuResourceDatabase &
GN::util::Entity::getGdb() const
{
    return getWorld().getGdb();
}

// ****************************************************************************
// World class
// ****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline GN::util::Entity *
GN::util::World::findOrCreateEntity( const char * name )
{
    Entity * e = findEntity( name );
    if( !e ) e = createEntity( name );
    return e;
}
