#include "pch.h"

GN::Logger * GN::Registry::sLogger = GN::GetLogger( "GN.base.Registry" );

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
        GN_ASSERT( NULL == mNames.Find(name) );
        mNames[name] = key;
    }
    else if ( overwriteExisting )
    {
        // Override old value
        GN_ASSERT( mItems[key].name == name && *mNames.Find(name) == key );
        mItems[key].value = value;
    }
    else
    {
        GN_ERROR(sLogger)( "Item '%s' is already existed.!", name.ToRawPtr() );
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

    if( s.Empty() ) return;

    const char * ptr = s.ToRawPtr();
    const char * end = ptr + s.Size();

#define NOT_EOL (ptr < end && *ptr != '\n')

#define IS_SPACE ('\t' == *ptr || ' ' == *ptr)

    // get name

    while( NOT_EOL && IS_SPACE ) ++ptr;
    const char * name_s = ptr;

    while( NOT_EOL && !IS_SPACE ) ++ptr;
    const char * name_e = ptr;

    if( name_e == name_s ) return;

    // check "="

    while( NOT_EOL && IS_SPACE ) ++ptr;
    const char * assign_s = ptr;

    if( ptr < end && *ptr == '=' ) ++ptr;
    const char * assign_e = ptr;

    if( assign_s + 1 != assign_e ) return;

    // get value

    while( NOT_EOL && IS_SPACE ) ++ptr;
    const char * value_s = ptr;

    while( ptr < end && !IS_SPACE ) ++ptr;
    const char * value_e = ptr;

    if( value_e == value_s ) return;

    // add name and value into registry
    StrA name( name_s, name_e - name_s );
    StrA value( value_s, value_e - value_s );
    sets( name, value, true );

    GN_UNGUARD;
}
