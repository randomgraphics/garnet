#include "pch.h"
#include "garnet/base/plugin.h"
#if !GN_ENABLE_INLINE
#include "garnet/base/plugin.inl"
#endif

GN::PluginID GN::PluginID::INVALID = {0};

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::PluginManager::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( PluginManager, () );

    // Do custom init here

    // success
    return selfOk();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::PluginManager::quit()
{
    GN_GUARD;

    // remove all plugins
    mTypes.clear();
    mNames.clear();
    mPlugins.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Plugin Management
// *****************************************************************************

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
    
    PluginID id;

    id.type = type;

    NameHandle h = mNames.first();

    while( 0 != h )
    {
        if ( mNames[h].name == name )
        {
            id.name = h;
            if ( validID(id) ) return id;  // found
            else return PluginID::INVALID; // not found
        }
        h = mNames.next(h);
    }

    GN_ERROR( "invalid plugin name '%s'!", name.cstr() );

    return PluginID::INVALID; // not found

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::PluginTypeID
GN::PluginManager::registerPluginType( const StrA & type, const StrA & desc )
{
    GN_GUARD;

    PluginTypeID id = getPluginTypeID( type );
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
    PluginCreationFunc factoryFunc,
    bool overrideExistingPlugin )
{
    GN_GUARD;

    if ( !mTypes.validHandle( type ) )
    {
        GN_ERROR( "Invalid plugin type ID!" );
        return PluginID::INVALID;
    }

    if ( 0 == factoryFunc )
    {
        GN_ERROR( "NULL factory function!" );
        return PluginID::INVALID;
    }

    PluginID id = getPluginID( type, name );
    if ( PluginID::INVALID == id )
    {
        // This is a new plugin
        
        GN_ASSERT( mTypes.validHandle( type ) );
        id.type = type;
        id.name = mNames.findIf( NameEqual(name) );
        if ( 0 == id.name )
        {
            id.name = mNames.add( NameItem(name) );
        }
        else
        {
            GN_ASSERT( mNames[id.name].name == name );
            GN_ASSERT( mNames[id.name].count > 0 );
            ++mNames[id.name].count;
        }

        GN_ASSERT( mPlugins.end() == mPlugins.find(id) );
    }
    else
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

    GN_ASSERT( mNames[id.name].name == name );
    GN_ASSERT( mNames[id.name].count > 0 );

    // success
    PluginItem & item = mPlugins[id];
    item.desc = desc;
    item.factory = factoryFunc;
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
GN::AutoRef<GN::PluginBase>
GN::PluginManager::createInstance( PluginID id, void * param ) const
{
    GN_GUARD;

    GN::AutoRef<GN::PluginBase> ret;
    
    std::map<PluginID,PluginItem>::const_iterator i = mPlugins.find( id );
    if ( mPlugins.end() == i )
    {
        GN_ERROR( "Invalid plugin ID!" );
        return ret;
    }

    GN_ASSERT( i->second.factory );
    return i->second.factory( param );

    GN_UNGUARD;
}
