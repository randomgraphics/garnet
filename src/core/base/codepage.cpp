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

#elif GN_XBOX2

typedef GN::CECImplXenon CECImpl;

#elif GN_WINPC

typedef GN::CECImplMSWIN CECImpl;

#else

struct CECImpl
{
    bool init( CharacterEncodingConverter::Encoding, CharacterEncodingConverter::Encoding )
    {
        GN_ERROR(sLogger)( "Character encoding class is not implemented on current platform." );
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
GN_API const char * GN::CharacterEncodingConverter::sEncoding2Str( Encoding e )
{
    switch( e )
    {
        case ASCII      : return "ascii";
        case ISO_8859_1 : return "iso-8859-1";
        case UTF7       : return "utf-7";
        case UTF8       : return "utf-8";
        case UTF16_LE   : return "utf-16-le";
        case UTF16_BE   : return "utf-16-be";
        case UTF16      : return "utf-16";
        case UTF32_LE   : return "utf-32-le";
        case UTF32_BE   : return "utf-32-be";
        case UTF32      : return "utf-32";
        case WIDECHAR   : return 2 == sizeof(wchar_t) ? "utf-16" : "utf-32";
        case GBK        : return "gbk";
        case BIG5       : return "big5";
        default         : return "=[unknown]=";
    }
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::CharacterEncodingConverter::CharacterEncodingConverter( Encoding from, Encoding to )
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
GN_API GN::CharacterEncodingConverter::~CharacterEncodingConverter()
{
    delete (CECImpl*)mImpl;
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::CharacterEncodingConverter::convert(
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
GN_API size_t GN::wcs2utf8(char * obuf, size_t ocount, const wchar_t * ibuf, size_t icount)
{
    static auto cec = CharacterEncodingConverter(CharacterEncodingConverter::WIDECHAR, CharacterEncodingConverter::UTF8);
    return cec.convert(obuf, ocount, ibuf, icount * sizeof(wchar_t));
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::utf82wcs(wchar_t * obuf, size_t ocount, const char * ibuf, size_t icount)
{
    static auto cec = CharacterEncodingConverter(CharacterEncodingConverter::UTF8, CharacterEncodingConverter::WIDECHAR);
    return cec.convert(obuf, ocount * sizeof(wchar_t), ibuf, icount) / sizeof(wchar_t);
}

//
//
// -----------------------------------------------------------------------------
GN_API StrA GN::wcs2utf8(const wchar_t * ibuf, size_t icount)
{
    // TODO: Let CEC returns StrA directly to avoid extra memory copy
    static auto cec = CharacterEncodingConverter(CharacterEncodingConverter::WIDECHAR, CharacterEncodingConverter::UTF8);
    DynaArray<char> buffer((icount + 1) * sizeof(wchar_t));
    cec.convert(buffer.rawptr(), buffer.size(), ibuf, icount * sizeof(wchar_t));
    return buffer.rawptr();
}

//
//
// -----------------------------------------------------------------------------
GN_API StrW GN::utf82wcs(const char * ibuf, size_t icount)
{
    // TODO: Let CEC returns StrW directly to avoid extra memory copy
    static auto cec = CharacterEncodingConverter(CharacterEncodingConverter::UTF8, CharacterEncodingConverter::WIDECHAR);
    DynaArray<wchar_t> buffer(icount + 1);
    cec.convert(buffer.rawptr(), buffer.size() * sizeof(wchar_t), ibuf, icount);
    return buffer.rawptr();
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::wcs2mbs( StrA & o, const wchar_t * i, size_t l )
{
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = str::length(i);

    o.setCaps( l + 1 );
#if GN_MSVC8
    size_t ol;
    ::wcstombs_s( &ol, o.mPtr, l+1, i, l );
    l = ol;
#else
    l = ::wcstombs( o.mPtr, i, l );
#endif
    if( (size_t)-1 == l || 0 == l )
    {
        o.clear();
    }
    else
    {
        o.mPtr[l] = 0;
        o.setSize( l );
    }
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::mbs2wcs( wchar_t * o, size_t os, const char * i, size_t is )
{
    StrW wcs;
    mbs2wcs( wcs, i, is );

    size_t n = wcs.size() + 1;

    if( o )
    {
        if( os > n )
        {
            memcpy( o, wcs.rawptr(), n );
            GN_ASSERT( 0 == o[n-1] );
            return n;
        }
        else if( os > 0 )
        {
            memcpy( o, wcs.rawptr(), sizeof(wchar_t) * (os-1) );
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
GN_API void GN::mbs2wcs( StrW & o, const char * i, size_t l )
{
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = str::length(i);

    o.setCaps( l + 1 );
#if GN_MSVC8
    size_t ol;
    if( 0 != ::mbstowcs_s( &ol, o.mPtr, l+1, i, l ) )
    {
        o.clear();
    }
    else
    {
        o.mPtr[ol] = 0;
        while( ol > 0 && 0 == o.mPtr[ol] ) --ol;
        o.setSize( ol+1 );
    }
#else
    l = ::mbstowcs( o.mPtr, i, l );
    if( (size_t)-1 == l || 0 == l )
    {
        o.clear();
    }
    else
    {
        o.mPtr[l] = 0;
        o.setSize( l );
    }
#endif
}

