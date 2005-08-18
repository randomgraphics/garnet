#include "pch.h"
#include "nativeFileDevice.h"
#include <pcrecpp.h>

// ****************************************************************************
//                            local functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
static bool sPathExist( const GN::StrA & path )
{
#if GN_WINPC

    WIN32_FIND_DATAA wfd;
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

//!
//! Get current working directory
// ----------------------------------------------------------------------------
static inline void sPwd( GN::StrA & result )
{
#if GN_WINPC
    char buf[MAX_PATH+1];
    ::GetCurrentDirectoryA( MAX_PATH, buf );
    result = buf;
#else
    GN_UNIMPL();
    result = "/";
#endif
}

//
//
// ----------------------------------------------------------------------------
static bool sIsDir( const GN::StrA & path )
{
#if GN_WINPC

    WIN32_FIND_DATAA wfd;
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
static void sResursiveFind( std::vector<GN::StrA> & result,
                            const GN::StrA & dirPath,
                            const GN::StrA & pattern,
                            bool recursive,
                            bool useRegex )
{
    GN_GUARD;

    GN_UNIMPL();
    // validate dirPath
    GN_ASSERT( sPathExist(dirPath) && sIsDir(dirPath) );

#if GN_WINPC

    WIN32_FIND_DATAA wfd;
    HANDLE fh;

    GN::StrA findPattern = dirPath + "\\" + (useRegex ? "*.*" : pattern);

    fh = ::FindFirstFileA( findPattern.cstr(), &wfd );
    if( INVALID_HANDLE_VALUE == fh ) return;

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

//!
//! Split fullpath to root and relative path
// ----------------------------------------------------------------------------
static inline bool
sParsePath( GN::StrA * root, GN::StrA * relpath, const GN::StrA & fullpath )
{
    GN_GUARD;

    // compile a regex parser
    static const pcrecpp::RE sParser( "^(#|/|.:/?)?(.+)$" );
    //                                  1        2

    std::string s1, s2;
    if( !sParser.FullMatch( fullpath.cstr(), &s1, &s2 ) )
    {
        GN_ERROR( "'%s' is not a valid path!", fullpath.cstr() );
        return false;
    }
    if( root ) *root  = s1.c_str();
    if( relpath ) *relpath = s2.c_str();

    // success
    return true;

    GN_UNGUARD;
}

// ****************************************************************************
//                            public functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
GN::detail::NativeFileDevice::NativeFileDevice()
{
    sPwd(mStartupDir);
}

//
//
// ----------------------------------------------------------------------------
GN::AutoRef<GN::File>
GN::detail::NativeFileDevice::openFile( const StrA & path, int openmode ) const
{
    GN_GUARD;

    AutoRef<AnsiFile> file( new AnsiFile );

    if( !file->open( path, openmode ) )
        return AutoRef<File>::EMPTYPTR;

    // success
    return file;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::detail::NativeFileDevice::isExist( const StrA & path ) const
{
    GN_GUARD;

    return sPathExist( path );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::detail::NativeFileDevice::isDir( const StrA & path ) const
{
    GN_GUARD;

    return sIsDir( path );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void
GN::detail::NativeFileDevice::glob(
    std::vector<StrA> & result,
    const StrA & dirPath,
    const StrA & pattern,
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
    const StrA & relpath,
    const StrA & base ) const
{
    GN_GUARD;

    GN_UNIMPL();

    // if relative path has root already...
    StrA root;
    if( !sParsePath( &root, 0, relpath ) ) return StrA::EMPTYSTR;
    if( 1 == root.size() )
    {
        if( '#' == root[0] )
        {
            if( !relpath.empty() && '/' == relpath[0] )
                return mStartupDir + &relpath[1];
            else
                return mStartupDir + "/" + relpath;
        }
        else
        {
            GN_ASSERT( '/' == root[0] );
#if GN_WINNT
            sPwd( root );
            GN_ASSERT( root.size() >= 2 );
            return root[0] + ":" + relpath; // c:/thepath
#else
            // this is a absolute unix path, do nothing
            return relpath;
#endif
        }
    }
    else if ( !root.empty() )
    {
        // input path is already an absolute path, do nothing.
        return relpath;
    }

    // if base is empty, then set base to current working directory (pwd)
    StrA effectiveBase;
    if( base.empty() ) sPwd( effectiveBase );
    else
    {
#if GN_WINNT
        char buf[MAX_PATH+1];
        _fullpath( buf, base.cstr(), MAX_PATH );
        effectiveBase = buf;
#else
        GN_UNIMPL();
#endif
    }

    // success
    GN_ASSERT( relpath.empty() || '/' != relpath[0] );
    return base + "/" + relpath;

    GN_UNGUARD;
}
