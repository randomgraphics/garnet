#include "pch.h"
#include "codepageMSWIN.h"
#include <stdlib.h>

#if GN_MSWIN

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// Local functions
// *****************************************************************************

static const char * sEncodingToLocal( MultiByteCharacterEncoding::Enum e )
{
    static const char * TABLE[] =
    {
        ".1252", // ISO_8859_1
        "GBK",   // GBK
        "CHT",   // BIG5
    };
    GN_CASSERT( GN_ARRAY_COUNT(TABLE) == MultiByteCharacterEncoding::NUM_ENCODINGS );

    if( 0 <= e && e < GN_ARRAY_COUNT(TABLE) )
    {
        return TABLE[e];
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid multibyte character encoding: %d", e );
        return NULL;
    }
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::MBCEImplMSWIN::init( MultiByteCharacterEncoding::Enum e )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::MBCEImplMSWIN, () );

    const char * localstr = sEncodingToLocal( e );
    if( NULL == localstr ) return failure();

    mLocale = _create_locale( LC_ALL, localstr );
    if( !mLocale )
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
void GN::MBCEImplMSWIN::quit()
{
    GN_GUARD;

    if( mLocale ) _free_locale( mLocale ), mLocale = 0;

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
GN::MBCEImplMSWIN::toUTF16_LE(
    wchar_t         * destBuffer,
    size_t            destBufferSizeInBytes,
    const char      * sourceBuffer,
    size_t            sourceBufferSizeInBytes )
{
    size_t converted;

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
    }
}

size_t
GN::MBCEImplMSWIN::fromUTF16_LE(
    char            * destBuffer,
    size_t            destBufferSizeInBytes,
    const wchar_t   * sourceBuffer,
    size_t            sourceBufferSizeInBytes )
{
    size_t converted;

    errno_t err = ::wcstombs_s(
        &converted,
        destBuffer,
        destBufferSizeInBytes,
        sourceBuffer,
        sourceBufferSizeInBytes/sizeof(wchar_t) );

    if( 0 != err )
    {
        GN_ERROR(sLogger)( "mbstowcs_s failed." );
        return 0;
    }
    else
    {
        return converted;
    }
}

#endif
