#include "pch.h"
#include <pcrecpp.h>

GN::Logger * GN::Registry::sLogger = GN::getLogger( "GN.base.Registry" );

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Registry::ItemKey GN::Registry::set(
    const StrA & name, const Variant & value, bool overwriteExisting )
{
    GN_GUARD;

    ItemKey key = name2Key( name );

    if (  0 == key )
    {
        // insert a new item to 
        Item i;
        i.name = name;
        i.value = value;
        key = mItems.add( i );
        if( 0 == key ) return 0;
        GN_ASSERT( mNames.end() == mNames.find(name) );
        mNames[name] = key;
    }
    else if ( overwriteExisting )
    {
        // Override old value
        GN_ASSERT( mItems[key].name == name && mNames.find(name)->second == key );
        mItems[key].value = value;
    }
    else
    {
        GN_ERROR(sLogger)( "Item '%s' is already existed.!", name.cptr() );
        return 0;
    }

    // success
    return key;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::Registry::importFromStr( const StrA & s )
{
    GN_GUARD;

    if( s.empty() ) return;

    static const char * pattern = "[\n\t ]*(\\w+)[\t ]*=[\t ]*(\\w*)";
    pcrecpp::RE re( pattern );

    pcrecpp::StringPiece sp( s.cptr(), (int)s.size() );

    std::string name, value;
    while( re.FindAndConsume( &sp, &name, &value ) )
    {
        sets( name.c_str(), value.c_str(), true );
    }

    GN_UNGUARD;
}
