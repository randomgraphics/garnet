#include "pch.h"
#include "nativeFileDevice.h"
#include <pcrecpp.h>

typedef std::vector<GN::StrA> StrVec;

// ****************************************************************************
//                            local functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
static bool sPathExist( const GN::StrA & path )
{
#if GN_WIN32

    WIN32_FIND_DATA wfd;
    HANDLE fh = ::FindFirstFileA( path.cstr(), &wfd );
    if( INVALID_HANDLE_VALUE == fh )
    {
        return false;
    }
    else
    {
        ::FindClose( fh );
        return true;
    }

#else
    GN_UNUSED_PARAM(path);
    GN_UNIMPL();
    return false;
#endif
}

//
//
// ----------------------------------------------------------------------------
bool sIsDir( const GN::StrA & path )
{
#if GN_WIN32

    WIN32_FIND_DATA wfd;
    HANDLE fh = ::FindFirstFileA( path.cstr(), &wfd );
    if( INVALID_HANDLE_VALUE == fh )
    {
        return false;
    }
    else
    {
        ::FindClose( fh );
        return !!(FILE_ATTRIBUTE_DIRECTORY & wfd.dwFileAttributes);
    }

#else
    GN_UNUSED_PARAM(path);
    GN_UNIMPL();
    return false;
#endif
}

//
//
// ----------------------------------------------------------------------------
static void sResursiveFind( StrVec & result,
                            const GN::StrA & dirPath,
                            const GN::StrA & pattern,
                            bool recursive,
                            bool useRegex )
{
    GN_GUARD;

    GN_UNIMPL();
    // validate dirPath
    GN_ASSERT( sPathExist(dirPath) && sIsDir(dirPath) );

#if GN_WIN32

    WIN32_FIND_DATA wfd;
    HANDLE fh;

    GN::StrA findPattern = dirPath + "\\" + (useRegex ? "*.*" : pattern);

    fh = ::FindFirstFileA( findPattern.cstr(), &wfd );
    if( FAILED(fh) ) return;

    std::auto_ptr<pcrecpp::RE> re;
    if( useRegex ) re.reset( new pcrecpp::RE( pattern.cstr() ) );

    do
    {
        if( FILE_ATTRIBUTE_DIRECTORY & wfd.dwFileAttributes )
        {
            if( recursive )
            {
                sResursiveFind( result, wfd.cFileName, pattern, recursive, useRegex );
            }
        }
        else if( !useRegex || re->FullMatch( wfd.cFileName ) )
        {
            result.push_back( wfd.cFileName ); // found one
        }
    } while( ::FindNextFileA( fh, &wfd ) );

    ::FindClose( fh );

#else
    GN_UNUSED_PARAM( result );
    GN_UNUSED_PARAM( dirPath );
    GN_UNUSED_PARAM( pattern );
    GN_UNUSED_PARAM( recursive );
    GN_UNUSED_PARAM( useRegex );
    GN_UNIMPL();
#endif

    GN_UNGUARD;
}

// ****************************************************************************
//                            public functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
GN::AutoRef<GN::File>
GN::detail::NativeFileDevice::openFile( const GN::StrA & path, int openmode ) const
{
    GN_GUARD;

    GN::AutoRef<GN::AnsiFile> file( new GN::AnsiFile );

    if( !file->open( path, openmode ) )
        return GN::AutoRef<GN::File>::EMPTYPTR;

    // success
    return file;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::detail::NativeFileDevice::isExist( const GN::StrA & path ) const
{
    GN_GUARD;

    return sPathExist( path );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::detail::NativeFileDevice::isDir( const GN::StrA & path ) const
{
    GN_GUARD;

    return sIsDir( path );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void
GN::detail::NativeFileDevice::findFiles(
    std::vector<GN::StrA> & result,
    const GN::StrA & dirPath,
    const GN::StrA & pattern,
    bool recursive,
    bool useRegex ) const
{
    GN_GUARD;

    if( !sPathExist(dirPath) )
    {
        GN_WARN( "'%s' does not exist!", dirPath.cstr() );
        return;
    }

    if( !sIsDir(dirPath) )
    {
        GN_WARN( "'%s' is not directory!", dirPath.cstr() );
        return;
    }

    sResursiveFind( result, dirPath, pattern, recursive, useRegex );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
GN::StrA GN::detail::NativeFileDevice::rel2abs(
    const GN::StrA & path,
    const GN::StrA & base ) const
{
    GN_GUARD;

    GN_UNIMPL();

    return base + "/" + path;

    GN_UNGUARD;
}
