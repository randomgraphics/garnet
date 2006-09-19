//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::PluginTypeID
GN::PluginManager::getPluginTypeID( const StrA & type ) const
{
    GN_GUARD_SLOW;
    PluginTypeID id = mTypes.findIf( TypeEqual(type) );
    if ( 0 == id ) GN_ERROR(sLogger)( "Invalid plugin type '%s'!", type.cptr() );
    return id;
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const GN::StrA &
GN::PluginManager::getPluginTypeName( PluginTypeID id ) const
{
    GN_GUARD_SLOW;
    if ( !mTypes.validHandle(id) ) return StrA::EMPTYSTR;
    return mTypes.get(id).type;
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const GN::StrA &
GN::PluginManager::getPluginTypeDesc( PluginTypeID id ) const
{
    GN_GUARD_SLOW;
    if ( !mTypes.validHandle(id) ) return StrA::EMPTYSTR;
    return mTypes.get(id).desc;
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool GN::PluginManager::validID( PluginID id ) const
{
    GN_GUARD_SLOW;
    std::map<PluginID,PluginItem>::const_iterator i = mPlugins.find( id );
    return ( mPlugins.end() != i );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const GN::StrA &
GN::PluginManager::getPluginName( PluginID id ) const
{
    GN_GUARD_SLOW;
    std::map<PluginID,PluginItem>::const_iterator i = mPlugins.find( id );
    if ( mPlugins.end() == i ) return StrA::EMPTYSTR;
    GN_ASSERT(
        mNames.validHandle(id.name) &&
        mNames.get(id.name).count > 0 );
    return mNames.get(id.name).name;
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const GN::StrA &
GN::PluginManager::getPluginDesc( PluginID id ) const
{
    GN_GUARD_SLOW;
    std::map<PluginID,PluginItem>::const_iterator i = mPlugins.find( id );
    if ( mPlugins.end() == i ) return StrA::EMPTYSTR;
    return i->second.desc;
    GN_UNGUARD_SLOW;
}
