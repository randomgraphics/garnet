//
//
// -----------------------------------------------------------------------------
inline GN::scene::ResourceId
GN::scene::ResourceManager::getResourceId( const StrA & name ) const
{
    StrA realname;

    resolveName( realname, name );

    StringMap::const_iterator i = mResourceNames.find( realname );

    if( mResourceNames.end() == i )
    {
        GN_ERROR(sLogger)( "invalid resource name: %s", name.cptr() );
        return 0;
    }

    // success
    GN_ASSERT( validId( i->second ) );
    return i->second;
}

//
//
// -----------------------------------------------------------------------------
inline const GN::StrA &
GN::scene::ResourceManager::getResourceName( ResourceId id ) const
{
    return validId( id ) ? mResources[id]->name : StrA::EMPTYSTR;
}

//
//
// -----------------------------------------------------------------------------
inline GN::scene::BaseResource *
GN::scene::ResourceManager::getResourceImpl( ResourceId id, const char * name )
{
    GN_GUARD_SLOW;

    if( !validId(id) )
    {
        if( name )
            GN_ERROR(sLogger)( "Invalid resource name: %s", name );
        else
            GN_ERROR(sLogger)( "Invalid resource ID: %d", id );
        return 0;
    }

    ResourceDesc * desc = mResources[id];
    GN_ASSERT( desc );

    if( desc->disposed )
    {
        // get resource type descriptor
        GN_ASSERT( desc->type < mResourceTypes.size() );
        const ResourceTypeDesc & rtd = mResourceTypes[desc->type];
        GN_ASSERT( rtd.creator && rtd.deletor );

        // recreate the resource
        GN_TRACE(sLogger)( "Load %s named '%s'", rtd.type.cptr(), desc->name.cptr() );
        desc->data = rtd.creator( desc->name );

        // receation failed, fallback to null resource
        if( !desc->data )
        {
            // create null resource instance
            GN_TRACE(sLogger)( "Fallback to null instance for %s named '%s'", rtd.type.cptr(), desc->name.cptr() );
            desc->data = rtd.creator( StrA::EMPTYSTR );
        }

        desc->disposed = false;
    }

    // move desc to head of LRU list.
    GN_ASSERT( desc->prev );
    GN_ASSERT( desc->next );
    ResourceDesc * prev = desc->prev;
    ResourceDesc * next = desc->next;
    prev->next = desc->next;
    next->prev = desc->prev;
    desc->next = mLRUHead.next;
    desc->next->prev = desc;
    mLRUHead.next = desc;

    // success
    return desc->data;

    GN_UNGUARD_SLOW;
}
