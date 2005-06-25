#include "pch.h"
#include "garnet/base/registry.h"

//
//
// -----------------------------------------------------------------------------
const GN::Variant * GN::Registry::getKey( const StrA & name ) const
{
    Variant def;
    const Variant & ret = getKey( name, def );
    return (&ret==&def) ? NULL : &ret;
}

//
//
// -----------------------------------------------------------------------------
const GN::Variant &
GN::Registry::getKey( const StrA & name, const Variant & defval ) const
{
    std::map<StrA,Variant>::const_iterator i = mKeys.find(name);
    return (i==mKeys.end()) ? defval : (*i).second;
}

//
//
// -----------------------------------------------------------------------------
bool GN::Registry::setKey( const StrA & name, const Variant & value, bool overwriteExisting )
{
    std::map<StrA,Variant>::iterator i = mKeys.find(name);
    if ( mKeys.end() == i )
    {
        // This is a new key
        mKeys[name] = value;
    }
    else if ( overwriteExisting )
    {
        // Override old value
        (*i).second = value;
    }
    else
    {
        GN_ERROR( "can't overwrite exising key '%s'!", name.cstr() );
        return false; // error!
    }

    // success
    return true;
}
