#include "pch.h"
#include "codepageMSWIN.h"
#include <stdlib.h>

#if GN_MSWIN

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// Local functions
// *****************************************************************************

static const char * sEncodingToLocal( CharacterEncodingConverter::Encoding e )
{
    static const char * TABLE[] =
    {
        ".ascii",       // ASCII
        ".1252",        // ISO_8859_1
        ".UTF-7",       // UTF7
        ".UTF-8",       // UTF8
        ".UTF-16",      // UTF16
        ".GBK",         // GBK
        ".CHT",         // BIG5
    };
    GN_CASSERT( GN_ARRAY_COUNT(TABLE) == CharacterEncodingConverter::NUM_ENCODINGS );

    if( 0 <= e && e < GN_ARRAY_COUNT(TABLE) )
    {
        return TABLE[e];
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid character encoding: %d", e );
        return NULL;
    }
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::CECImplMSWIN::init(
    CharacterEncodingConverter::Encoding from,
    CharacterEncodingConverter::Encoding to )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::CECImplMSWIN, () );

    const char * fromstr = sEncodingToLocal( from );
    if( NULL == fromstr ) return failure();
    mLocaleFrom = _create_locale( LC_ALL, fromstr );
    if( !mLocaleFrom )
    {
        GN_ERROR(sLogger)( "_create_locale() failed." );
        return failure();
    }

    const char * tostr = sEncodingToLocal( to );
    if( NULL == tostr ) return failure();
    mLocaleTo = _create_locale( LC_ALL, tostr );
    if( !mLocaleTo )
    {
        GN_ERROR(sLogger)( "_create_locale() failed." );
        return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::CECImplMSWIN::quit()
{
    GN_GUARD;

    if( mLocaleFrom ) _free_locale( mLocaleFrom ), mLocaleFrom = 0;
    if( mLocaleTo ) _free_locale( mLocaleTo ), mLocaleTo = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
size_t
GN::CECImplMSWIN::convert(
    void            * /*destBuffer*/,
    size_t            /*destBufferSizeInBytes*/,
    const void      * /*sourceBuffer*/,
    size_t            /*sourceBufferSizeInBytes*/ )
{
    GN_UNIMPL_WARNING();
    return 0;
    /*size_t converted;

    errno_t err = ::mbstowcs_s(
        &converted,
        destBuffer,
        destBufferSizeInBytes/sizeof(wchar_t),
        sourceBuffer,
        sourceBufferSizeInBytes );

    if( 0 != err )
    {
        GN_ERROR(sLogger)( "mbstowcs_s failed." );
        return 0;
    }
    else
    {
        return converted * sizeof(wchar_t);
    }*/
}

#endif
