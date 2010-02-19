#include "pch.h"
#include "codepageXenon.h"

#if GN_XENON

static GN::Logger * sLogger = GN::GetLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::CECImplXenon::Init(
    CharacterEncodingConverter::Encoding from,
    CharacterEncodingConverter::Encoding to )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::CECImplXenon, () );

    // success
    mEncodingFrom = from;
    mEncodingTo   = to;
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::CECImplXenon::Quit()
{
    GN_GUARD;

    // standard Quit procedure
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
GN::CECImplXenon::convert(
    void       * destBuffer,
    size_t       destBufferSizeInBytes,
    const void * sourceBuffer,
    size_t       sourceBufferSizeInBytes )
{
    // convert from source encoding to widechar encoding
    DynaArray<wchar_t> tempBuffer;
    if( mEncodingFrom != CharacterEncodingConverter::UTF16 &&
        mEncodingFrom != CharacterEncodingConverter::UTF16_BE &&
        mEncodingFrom != CharacterEncodingConverter::WIDECHAR )
    {
        tempBuffer.resize( sourceBufferSizeInBytes );

        GN_MSW_CHECK_RETURN( MultiByteToWideChar(
                CP_ACP,
                0,
                (LPCSTR)sourceBuffer,
                sourceBufferSizeInBytes,
                (LPWSTR)destBuffer,
                destBufferSizeInBytes / sizeof(wchar_t) ),
            0 );

        sourceBuffer = tempBuffer.GetRawPtr();
        sourceBufferSizeInBytes = tempBuffer.size() * sizeof(wchar_t);
    }
    else
    {
        // source encoding is already UTF16_BE, do nothing
    }

    // convert from widechar encoding to target encoding
    if( mEncodingTo == CharacterEncodingConverter::UTF16 ||
        mEncodingTo == CharacterEncodingConverter::UTF16_BE ||
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
        return (size_t)WideCharToMultiByte(
            CP_ACP,
            0,
            (LPCWSTR)sourceBuffer,
            sourceBufferSizeInBytes / sizeof(wchar_t),
            (LPSTR)destBuffer,
            destBufferSizeInBytes,
            NULL,
            NULL);
    }
}

#endif
