//
//
// -----------------------------------------------------------------------------
inline GN::scene::BaseResource *
GN::scene::ResourceManager::getResource( ResourceId id )
{
    if( 0 == id ) return 0;
    
    if(!validId(id) )
    {
        GN_ERROR(sLogger)( "invalid resource id: %d", id ); return 0;
    }

    return getResourceImpl( id );
}

//
//
// -----------------------------------------------------------------------------
inline GN::scene::BaseResource *
GN::scene::ResourceManager::getResource( const StrA & name )
{
    ResourceId id = getResourceId( name );
    if( 0 == id ) return 0;
    
    return getResourceImpl( id );
}

//
//
// -----------------------------------------------------------------------------
inline GN::scene::ResourceId
GN::scene::ResourceManager::getResourceId( const StrA & name )
{
    StrA realname;

    resolveName( realname, name );

    StringMap::const_iterator i = mResourceNames.find( realname );

    if( mResourceNames.end() == i )
    {
        const StrA & type = determineResourceType( realname );
        if( type.empty() )
        {
            GN_ERROR(sLogger)( "invalid resource name: %s", name.cptr() );
            return 0;
        }

        return addResource( realname, type );
    }
    else
    {
        // success
        GN_ASSERT( validId( i->second ) );
        return i->second;
    }
}

//
//
// -----------------------------------------------------------------------------
inline const GN::StrA &
GN::scene::ResourceManager::getResourceName( ResourceId id ) const
{
    return validId( id ) ? mResources[id]->name : StrA::EMPTYSTR;
}
