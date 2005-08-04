#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/base/plugin.inl"
#endif

GN::PluginID GN::PluginID::INVALID(0);

//
//
// -----------------------------------------------------------------------------
void GN::PluginManager::reset()
{
    GN_GUARD;

    // remove all plugins
    mTypes.clear();
    mNames.clear();
    mPlugins.clear();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::PluginID
GN::PluginManager::getPluginID( PluginTypeID type, const StrA & name ) const
{
    GN_GUARD;

    // check type ID
    if ( !mTypes.validHandle( type ) )
    {
        GN_ERROR( "invalid type ID" );
        return PluginID::INVALID;
    }
    
    PluginID id( type, mNames.findIf( NameEqual(name) ) );

    if ( validID(id) )
    {
        return id;
    }
    else
    {
        GN_ERROR( "invalid plugin name '%s'!", name.cstr() );
        return PluginID::INVALID;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::PluginTypeID
GN::PluginManager::registerPluginType( const StrA & type, const StrA & desc )
{
    GN_GUARD;

    PluginTypeID id( getPluginTypeID( type ) );
    if ( id > 0 )
    {
        GN_ERROR( "Plugin type '%s' already exists!", type.cstr() );
        return 0;
    }

    return mTypes.add( TypeItem(type,desc) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::PluginManager::removePluginType( PluginTypeID type )
{
    GN_GUARD;

    if ( !mTypes.validHandle(type) )
    {
        GN_ERROR( "Invalid plugin type ID '%d'!", type );
        return;
    }

    // remove type item
    mTypes.remove( type );

    std::map<PluginID,PluginItem>::iterator i, j;
    PluginID id;
    for( i = mPlugins.begin(); i != mPlugins.end(); )
    {
        j = i;
        ++j;
        id = i->first;
        if ( id.type == type )
        {
            // remove unused name item
            GN_ASSERT( mNames.validHandle(id.name) && mNames[id.name].count > 0 );
            --mNames[id.name].count;
            if ( 0 == mNames[id.name].count ) mNames.remove( id.name );

            // remove plugin item
            mPlugins.erase( i );
        }
        i = j;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::PluginID GN::PluginManager::registerPlugin(
    PluginTypeID type,
    const StrA & name,
    const StrA & desc,
    const PluginFactory & factory,
    bool overrideExistingPlugin )
{
    GN_GUARD;

    if ( !mTypes.validHandle( type ) )
    {
        GN_ERROR( "Invalid plugin type ID!" );
        return PluginID::INVALID;
    }

    if ( factory.empty() )
    {
        GN_ERROR( "NULL factory function!" );
        return PluginID::INVALID;
    }

    PluginID id( type, mNames.findIf( NameEqual(name) ) );

    if ( validID(id) )
    {
        // This is a existing plugin
        
        if ( !overrideExistingPlugin )
        {
            GN_ERROR( "Plugin '%s::%s' already exist",
                getPluginTypeName(type).cstr(),
                getPluginName(id).cstr() );
            return PluginID::INVALID;
        }

        GN_ASSERT( mPlugins.end() != mPlugins.find(id) );
    }
    else
    {
        // This is a new plugin
        
        if ( 0 == id.name )
        {
            id.name = mNames.add( NameItem(name) );
        }
        else
        {
            GN_ASSERT( mNames[id.name].count > 0 );
            ++mNames[id.name].count;
        }

        GN_ASSERT( mTypes.validHandle( id.type ) );
        GN_ASSERT( mNames.validHandle( id.name ) );
        GN_ASSERT( mPlugins.end() == mPlugins.find(id) );
    }

    GN_ASSERT( mNames[id.name].name == name );
    GN_ASSERT( mNames[id.name].count > 0 );

    // success
    PluginItem & item = mPlugins[id];
    item.desc = desc;
    item.factory = factory;
    return id;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::PluginManager::removePlugin( PluginID id )
{
    GN_GUARD;

    std::map<PluginID,PluginItem>::iterator i = mPlugins.find( id );
    if ( mPlugins.end() == i )
    {
        GN_ERROR( "Invalid plugin ID!" );
        return;
    }

    // remove plugin item
    mPlugins.erase( i );

    // remove unused name item
    GN_ASSERT( mNames.validHandle(id.name) && mNames[id.name].count > 0 );
    --mNames[id.name].count;
    if ( 0 == mNames[id.name].count ) mNames.remove( id.name );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::PluginBase *
GN::PluginManager::doInstanceCreation( PluginID id, void * param ) const
{
    GN_GUARD;

    std::map<PluginID,PluginItem>::const_iterator i = mPlugins.find( id );
    if ( mPlugins.end() == i )
    {
        GN_ERROR( "Invalid plugin ID!" );
        return 0;
    }

    GN_ASSERT( !i->second.factory.empty() );

    PluginBase * p = i->second.factory( param );
    if ( p ) p->mID = id;

    return p;

    GN_UNGUARD;
}
