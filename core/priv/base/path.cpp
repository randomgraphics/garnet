#include "pch.h"
#include <pcrecpp.h>
#if GN_WINNT
#if GN_WINPC
#include <shlwapi.h>
#endif
#include <direct.h>
#elif GN_POSIX
#include <dirent.h>
#include <unistd.h>
#endif

// ****************************************************************************
// Local types, functions and variables
// ****************************************************************************

#if GN_WINNT
#define MAX_PATH_LENGTH MAX_PATH
#define PATH_SEPARATOR '\\'
#else
#define MAX_PATH_LENGTH PATH_MAX
#define PATH_SEPARATOR '/'
#endif

//
//
// ----------------------------------------------------------------------------
static inline GN::StrA sPwd()
{
#if GN_XENON
    return "game:";
#else
    char buf[MAX_PATH_LENGTH+1];
    getcwd( buf, MAX_PATH_LENGTH );
    buf[MAX_PATH_LENGTH] = 0;
    GN::StrA ret(buf);
    ret.trimRight( PATH_SEPARATOR );
    return ret;
#endif
}

//
//
// ----------------------------------------------------------------------------
struct AppDir
{
    GN::StrA appDir;
    GN::StrA startupDir;
    
    AppDir()
    {
        startupDir = sPwd();
    }
};

static AppDir sAppDir;

//
// 
// ----------------------------------------------------------------------------
static void sNormalizePathSeparator(
    GN::StrA & result, const GN::StrA & path, char from, char to )
{
    // convert 'from' to 'to'
    result.setCaps( path.size() );
    for( size_t i = 0; i < path.size(); ++i )
    {
        const char & ch = path[i];

        if( from == ch )
        {
            // ignore redundant path separator
            if( 0 == i || result[i-1] != to ) result.append( to );
        }
        else
        {
            result.append( ch );
        }
    }

    // Detect pending path separator
    if( !result.empty() )
    {
        result.trimRight( to );
        if( result.empty() )
            result.append( to ); // special case for "/" and "\\"
    }
}

//
//
// ----------------------------------------------------------------------------
static void sResursiveFind( std::vector<GN::StrA> & result,
                            const GN::StrA & dirName,
                            const GN::StrA & pattern,
                            bool recursive,
                            bool useRegex )
{
    GN_GUARD;

    // validate dirName
    GN_ASSERT( GN::path::exist(dirName) && GN::path::isDir(dirName) );

#if GN_WINNT

    WIN32_FIND_DATAA wfd;
    HANDLE fh;

    GN::StrA findPattern = GN::path::resolve(dirName) + "\\" + (useRegex ? "*.*" : pattern);
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
    GN_UNUSED_PARAM( dirName );
    GN_UNUSED_PARAM( pattern );
    GN_UNUSED_PARAM( recursive );
    GN_UNUSED_PARAM( useRegex );
    GN_UNIMPL();
#endif

    GN_UNGUARD;
}

// ****************************************************************************
// Public functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::path::toNative( StrA & result, const StrA & path )
{
#if GN_WINNT
    sNormalizePathSeparator( result, path, '/', '\\' );
#else
    sNormalizePathSeparator( result, path, '\\', '/' );
#endif

    // handle path prefixes
    StrA root;
    size_t pos;
    if( "app:" == result.subString(0,4) )
    {
        pos = 4;
        root = sAppDir.appDir;
    }
    else if( "pwd:" == path.subString(0,4) )
    {
        pos = 4;
        root = sPwd();
    }
    else if( "startup:" == path.subString(0,8) )
    {
        pos = 8;
        root = sAppDir.startupDir;
    }
    else return; // ======================>

    if( result.size() <= pos || result[pos] != PATH_SEPARATOR )
    {
        root.append( PATH_SEPARATOR );
    }
    result = root + result.subString(pos,(size_t)-1);
}

//
//
// ----------------------------------------------------------------------------
bool GN::path::exist( const StrA & path )
{
    StrA native = toNative(path);
    
#if GN_WINPC
    return !!::PathFileExistsA( native.cstr() );
#elif GN_WINNT
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( native.cstr(), &wfd );
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
    if( isDir(native) ) return true;
    FILE * fp = fopen( native.cstr(), "r" );
    if( 0 == fp ) return false;
    fclose( fp );
    return true;
#endif
}

//
//
// ----------------------------------------------------------------------------
bool GN::path::isDir( const StrA & path )
{
    StrA native = toNative(path);
    
#if GN_WINPC
    return !!::PathIsDirectoryA( native.cstr() );
#elif GN_WINNT
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( native.cstr(), &wfd );
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
    DIR * d = opendir( native.cstr() );
    if( 0 == d ) return false;
    closedir( d );
    return true;
#endif
}

//
//
// ----------------------------------------------------------------------------
void GN::path::getParent( StrA & result, const StrA &  path )
{
    GN_UNUSED_PARAM(result);
    GN_UNUSED_PARAM(path);
    GN_UNIMPL();
}

//
//
// ----------------------------------------------------------------------------
void GN::path::join(
    StrA & result,
    const StrA & path1,
    const StrA & path2,
    const StrA & path3,
    const StrA & path4 )
{
    result.clear();
    const StrA * parts[] = { &path1, &path2, &path3, &path4 };
    for( size_t i = 0; i < sizeof(parts)/sizeof(parts[0]); ++i )
    {
        const StrA & p = *parts[i];
        if( p.empty() ) continue;
        result.append( PATH_SEPARATOR );
        result.append( p );
    }
}

//
//
// ----------------------------------------------------------------------------
bool GN::path::resolve( StrA & result, const StrA & path )
{
    GN_GUARD;

    StrA relPath = toNative(path);

#if GN_WINPC || GN_POSIX

    char absPath[MAX_PATH_LENGTH+1];

#if GN_POSIX
    if( 0 == realpath( relPath.cstr(), absPath ) )
#else
    if( 0 == _fullpath( absPath, relPath.cstr(), MAX_PATH_LENGTH ) )
#endif
    {
        GN_ERROR( "invalid path '%s'.", path.cstr() );
        return false;
    }

    // success
    absPath[MAX_PATH_LENGTH] = 0;
    result.assign( absPath );
    return true;

#else
    result = relPath;
    return true;
#endif

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
std::vector<GN::StrA> &
GN::path::glob(
    std::vector<StrA> & result,
    const StrA & dirName,
    const StrA & pattern,
    bool         recursive,
    bool         useRegex )
{
    GN_GUARD;

    if( !exist(dirName) )
    {
        GN_WARN( "'%s' does not exist!", dirName.cstr() );
        return result;
    }

    if( !isDir(dirName) )
    {
        GN_WARN( "'%s' is not directory!", dirName.cstr() );
        return result;
    }

    sResursiveFind( result, dirName, pattern, recursive, useRegex );
    return result;

    GN_UNGUARD;
}
