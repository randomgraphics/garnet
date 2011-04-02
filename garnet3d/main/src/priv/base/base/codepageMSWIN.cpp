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
        ".1252",        // ASCII
        ".1252",        // ISO_8859_1

        // Unicode encodings are set to empty string intentionally.
        "",             // UTF7
        "",             // UTF8
        "",             // UTF16_LE
        "",             // UTF16_BE
        "",             // UTF16
        "",             // UTF32_LE
        "",             // UTF32_BE
        "",             // UTF32
        "",             // WIDECHAR

        "CHS",          // GBK
        "CHT",          // BIG5
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
    if( 0 != *fromstr )
    {
        mLocaleFrom = new std::locale( fromstr );//_create_locale( LC_ALL, fromstr );
        if( !mLocaleFrom )
        {
            GN_ERROR(sLogger)( "_create_locale() failed." );
            return failure();
        }
    }

    const char * tostr = sEncodingToLocal( to );
    if( NULL == tostr ) return failure();
    if( 0 != *tostr )
    {
        //mLocaleTo = _create_locale( LC_ALL, tostr );
        mLocaleTo = new std::locale( tostr );
        if( !mLocaleTo )
        {
            GN_ERROR(sLogger)( "_create_locale() failed." );
            return failure();
        }
    }

    // success
    mEncodingFrom = from;
    mEncodingTo   = to;
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::CECImplMSWIN::quit()
{
    GN_GUARD;

    safeDelete( mLocaleFrom );
    safeDelete( mLocaleTo );

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
    void       * destBuffer,
    size_t       destBufferSizeInBytes,
    const void * sourceBuffer,
    size_t       sourceBufferSizeInBytes )
{
    size_t converted = 0;

    // convert from source encoding to widechar encoding
    DynaArray<wchar_t> tempBuffer;
    if( NULL != mLocaleFrom )
    {
        tempBuffer.resize( sourceBufferSizeInBytes );

        /*errno_t err = ::_mbstowcs_s_l(
            &converted,
            tempBuffer.rawptr(),
            tempBuffer.size(),
            (const char *)sourceBuffer,
            sourceBufferSizeInBytes,
            (_locale_t)mLocaleFrom );*/
        mbstate_t    state = {0};
        const char * srcnext;
        wchar_t    * tempnext;
        int err = std::use_facet<std::codecvt<wchar_t,char,mbstate_t> >(*mLocaleFrom).in(
            state,
            (const char *)sourceBuffer,
            ((const char *)sourceBuffer)+sourceBufferSizeInBytes,
            srcnext,
            tempBuffer.rawptr(),
            tempBuffer.rawptr() + tempBuffer.size(),
            tempnext );

        if( std::codecvt_base::error == err )
        {
            GN_ERROR(sLogger)( "fail to convert input buffer to UNICODE." );
            return 0;
        }

        converted = tempnext - tempBuffer.rawptr();

        sourceBuffer = tempBuffer.rawptr();
        sourceBufferSizeInBytes = converted * sizeof(wchar_t);
    }
    else if( mEncodingFrom == CharacterEncodingConverter::UTF16 ||
             mEncodingFrom == CharacterEncodingConverter::UTF16_LE ||
             mEncodingFrom == CharacterEncodingConverter::WIDECHAR )
    {
        // source encoding is already UTF16_LE, do nothing
    }
    else
    {
        GN_ERROR(sLogger)( "Conversion from encoding \"%d\" is not supported yet.", mEncodingFrom );
        return 0;
    }

    // convert from widechar encoding to target encoding
    if( NULL != mLocaleTo )
    {
        converted = 0;

        /*char          * d = (char*)destBuffer;
        const wchar_t * s = (const wchar_t*)sourceBuffer;
        for( size_t i = 0; i < sourceBufferSizeInBytes / sizeof(wchar_t); ++i )
        {
            int retval;
            errno_t err = ::_wctomb_s_l(
                &retval,
                d,
                destBufferSizeInBytes,
                *s,
                (_locale_t)mLocaleTo );

            if( 0 != err )
            {
                GN_ERROR(sLogger)( "fail to convert from UNICODE to target encoding." );
                return 0;
            }

            GN_ASSERT( retval > 0 );

            ++s;
            d += retval;

            converted += (size_t)retval;
        }*/
        mbstate_t       state = {0};
        const wchar_t * srcnext;
        char          * dstnext;
        int err = std::use_facet<std::codecvt<wchar_t,char,mbstate_t> >(*mLocaleTo).out(
            state,
            (const wchar_t*)sourceBuffer,
            (const wchar_t*)sourceBuffer + sourceBufferSizeInBytes/sizeof(wchar_t),
            srcnext,
            (char *)destBuffer,
            ((char *)destBuffer)+destBufferSizeInBytes,
            dstnext );

        if( std::codecvt_base::error == err )
        {
            GN_ERROR(sLogger)( "fail to convert from UNICODE to target encoding." );
            return 0;
        }

        converted = dstnext - (char*)destBuffer;

        return converted;
    }
    else if( mEncodingTo == CharacterEncodingConverter::UTF16 ||
             mEncodingTo == CharacterEncodingConverter::UTF16_LE ||
             mEncodingTo == CharacterEncodingConverter::WIDECHAR )
    {
        // Target encoding is UNICODE too. A memory copy is enough.

        if( 0 == destBuffer )
        {
            // return number of bytes required to store conversion result
            return sourceBufferSizeInBytes;
        }

        if( 0 == sourceBuffer )
        {
            GN_ERROR(sLogger)( "NULL source buffer pointer!" );
            return 0;
        }

        if( sourceBufferSizeInBytes > destBufferSizeInBytes )
        {
            GN_ERROR(sLogger)( "There's no enough space in destination buffer." );
            return 0;
        }

        memcpy( destBuffer, sourceBuffer, sourceBufferSizeInBytes );

        return sourceBufferSizeInBytes;
    }
    else
    {
        GN_ERROR(sLogger)( "Conversion to encoding \"%d\" is not supported yet.", mEncodingTo );
        return 0;
    }
}

#endif
