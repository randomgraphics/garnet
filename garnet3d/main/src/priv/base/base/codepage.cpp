#include "pch.h"
#include "codepageICONV.h"
#include "codepageMSWIN.h"
#include "codepageXenon.h"

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// class CharacterEncodingConverter implementation class
// *****************************************************************************

#if HAS_ICONV

typedef GN::CECImplICONV CECImpl;

#elif GN_XENON

typedef GN::CECImplXenon CECImpl;

#elif GN_MSWIN

typedef GN::CECImplMSWIN CECImpl;

#else

struct CECImpl
{
    bool init( CharacterEncodingConverter::Encoding, CharacterEncodingConverter::Encoding )
    {
        GN_ERROR(sLogger)( "Character encoding class is not implemented on " GN_PLATFORM_NAME "." );
        return false;
    }

    size_t
    convert(
        void       * /*destBuffer*/,
        size_t       /*destBufferSizeInBytes*/,
        const void * /*sourceBuffer*/,
        size_t       /*sourceBufferSizeInBytes*/ )
    {
        return 0;
    }
};

#endif


// *****************************************************************************
// class CharacterEncodingConverter
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::CharacterEncodingConverter::CharacterEncodingConverter( Encoding from, Encoding to )
    : mImpl( new CECImpl )
{
    CECImpl * p = (CECImpl*)mImpl;

    if( !p->init( from, to ) )
    {
        delete p;
        mImpl = NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
GN::CharacterEncodingConverter::~CharacterEncodingConverter()
{
    delete (CECImpl*)mImpl;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::CharacterEncodingConverter::convert(
    void            * destBuffer,
    size_t            destBufferSizeInBytes,
    const void      * sourceBuffer,
    size_t            sourceBufferSizeInBytes )
{
    if( !mImpl )
    {
        GN_ERROR(sLogger)( "CharacterEncodingConverter is not correctly initialized." );
        return 0;
    }

    CECImpl * p = (CECImpl*)mImpl;

    return p->convert(
        (wchar_t*)destBuffer,
        destBufferSizeInBytes,
        (const char *)sourceBuffer,
        sourceBufferSizeInBytes );
}

// *****************************************************************************
// public utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::CharacterEncodingConverter::Encoding GN::getCurrentSystemEncoding()
{
    return CharacterEncodingConverter::ISO_8859_1;
}

//
//
// -----------------------------------------------------------------------------
void GN::wcs2mbs( StrA & o, const wchar_t * i, size_t l )
{
    if ( 0 == i ) { o.Clear(); return; }
    if ( 0 == l ) l = StringLength(i);

    o.SetCaps( l + 1 );
#if GN_MSVC8
    size_t ol;
    ::wcstombs_s( &ol, o.mPtr, l+1, i, l );
    l = ol;
#else
    l = ::wcstombs( o.mPtr, i, l );
#endif
    if( (size_t)-1 == l || 0 == l )
    {
        o.Clear();
    }
    else
    {
        o.mPtr[l] = 0;
        o.mCount = l;
    }
}

//
//
// -----------------------------------------------------------------------------
size_t GN::mbs2wcs( wchar_t * o, size_t os, const char * i, size_t is )
{
    StrW wcs;
    mbs2wcs( wcs, i, is );

    size_t n = wcs.Size() + 1;

    if( o )
    {
        if( os > n )
        {
            memcpy( o, wcs.GetRawPtr(), n );
            GN_ASSERT( 0 == o[n-1] );
            return n;
        }
        else if( os > 0 )
        {
            memcpy( o, wcs.GetRawPtr(), sizeof(wchar_t) * (os-1) );
            o[os-1] = 0;
            return os;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return n;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::mbs2wcs( StrW & o, const char * i, size_t l )
{
    if ( 0 == i ) { o.Clear(); return; }
    if ( 0 == l ) l = StringLength(i);

    o.SetCaps( l + 1 );
#if GN_MSVC8
    size_t ol;
    if( 0 != ::mbstowcs_s( &ol, o.mPtr, l+1, i, l ) )
    {
        o.Clear();
    }
    else
    {
        o.mPtr[ol] = 0;
        while( ol > 0 && 0 == o.mPtr[ol] ) --ol;
        o.mCount = ol+1;
    }
#else
    l = ::mbstowcs( o.mPtr, i, l );
    if( (size_t)-1 == l || 0 == l )
    {
        o.Clear();
    }
    else
    {
        o.mPtr[l] = 0;
        o.mCount = l;
    }
#endif
}

