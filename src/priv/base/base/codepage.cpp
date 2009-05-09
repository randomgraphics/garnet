#include "pch.h"
#include "codepageMSWIN.h"

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// class MultiByteCharacterEncoding implementation class
// *****************************************************************************

#if GN_MSWIN

typedef GN::MBCEImplMSWIN MBCEImpl;

#else

struct MBCEImpl
{
    bool init( MultiByteCharacterEncoding::Enum )
    {
        GN_ERROR(sLogger)( "Character encoding class is not implemented on " GN_PLATFORM_NAME. );
        return false;
    }

    size_t
    toUTF16_LE(
        UnicodeEncoding   /*destEncoding*/,
        void            * /*destBuffer*/,
        size_t            /*destBufferSizeInBytes*/,
        const void      * /*sourceBuffer*/,
        size_t            /*sourceBufferSizeInBytes*/ )
    {
        return 0;
    }

    size_t
    fromUTF16_LE(
        void            * /*destBuffer*/,
        size_t            /*destBufferSizeInBytes*/,
        UnicodeEncoding   /*sourceEncoding*/,
        const void      * /*sourceBuffer*/,
        size_t            /*sourceBufferSizeInBytes*/ )
    {
        return 0;
    }
};

#endif


// *****************************************************************************
// class MultiByteCharacterEncoding
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::MultiByteCharacterEncoding::MultiByteCharacterEncoding( Enum e )
    : mImpl( new MBCEImpl )
{
    MBCEImpl * p = (MBCEImpl*)mImpl;

    if( !p->init( e ) )
    {
        delete p;
        mImpl = NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
GN::MultiByteCharacterEncoding::~MultiByteCharacterEncoding()
{
    delete (MBCEImpl*)mImpl;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::MultiByteCharacterEncoding::toUnicode(
    UnicodeEncoding   destEncoding,
    void            * destBuffer,
    size_t            destBufferSizeInBytes,
    const void      * sourceBuffer,
    size_t            sourceBufferSizeInBytes )
{
    if( !mImpl )
    {
        GN_ERROR(sLogger)( "MultiByteCharacterEncoding is not correctly initialized." );
        return 0;
    }

    MBCEImpl * p = (MBCEImpl*)mImpl;

    switch( destEncoding )
    {
        case UnicodeEncoding::UTF16_LE:
            return p->toUTF16_LE(
                destBuffer,
                destBufferSizeInBytes,
                sourceBuffer,
                sourceBufferSizeInBytes );

        case UnicodeEncoding::UTF7 :
        case UnicodeEncoding::UTF8 :
        case UnicodeEncoding::UTF16_BE :
            GN_UNIMPL_WARNING();
            return 0;

        default:
            GN_ERROR(sLogger)( "Invalid unicode encoding enumeration: %d", destEncoding.toRawEnum() );
            return 0;
    }
}

//
//
// -----------------------------------------------------------------------------
size_t GN::MultiByteCharacterEncoding::toUnicode(
    void            * destBuffer,
    size_t            destBufferSizeInBytes,
    UnicodeEncoding   sourceEncoding,
    const void      * sourceBuffer,
    size_t            sourceBufferSizeInBytes )
{
    if( !mImpl )
    {
        GN_ERROR(sLogger)( "MultiByteCharacterEncoding is not correctly initialized." );
        return 0;
    }

    MBCEImpl * p = (MBCEImpl*)mImpl;

    switch( sourceEncoding )
    {
        case UnicodeEncoding::UTF16_LE:
            return p->fromUTF16_LE(
                destBuffer,
                destBufferSizeInBytes,
                sourceBuffer,
                sourceBufferSizeInBytes );

        case UnicodeEncoding::UTF7 :
        case UnicodeEncoding::UTF8 :
        case UnicodeEncoding::UTF16_BE :
            GN_UNIMPL_WARNING();
            return 0;

        default:
            GN_ERROR(sLogger)( "Invalid unicode encoding enumeration: %d", sourceEncoding.toRawEnum() );
            return 0;
    }
}

// *****************************************************************************
// public utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::MultiByteCharacterEncoding::Enum GN::getCurrentSystemEncoding()
{
    return MultiByteCharacterEncoding::ISO_8859_1;
}

//
//
// -----------------------------------------------------------------------------
void GN::wcs2mbs( StrA & o, const wchar_t * i, size_t l )
{
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = strLen(i);

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

    size_t n = wcs.size() + 1;

    if( o )
    {
        if( os > n )
        {
            memcpy( o, wcs.cptr(), n );
            GN_ASSERT( 0 == o[n-1] );
            return n;
        }
        else if( os > 0 )
        {
            memcpy( o, wcs.cptr(), sizeof(wchar_t) * (os-1) );
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
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = strLen(i);

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
        o.mCount = ol+1;
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
        o.mCount = l;
    }
#endif
}

