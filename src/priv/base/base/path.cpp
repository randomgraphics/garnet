#include "pch.h"
#include <pcrecpp.h>
#include "simpleGlob.h"
#if GN_MSWIN
#if GN_PC
#include <shlwapi.h>
#if GN_MSVC
#pragma comment( lib, "shlwapi.lib" )
#endif
#endif
#include <direct.h>
#elif GN_POSIX
#include <dirent.h>
#include <unistd.h>
#endif

// *****************************************************************************
// Local types, functions and variables
// *****************************************************************************

#if GN_MSWIN
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

#if GN_MSVC
#define MAX_PATH_LENGTH MAX_PATH
#define getcwd _getcwd
#else
#define MAX_PATH_LENGTH PATH_MAX
#endif

//
//
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
static inline GN::StrA sGetAppDir()
{
#if GN_XENON
    return "game:";
#elif GN_MSWIN
    char buf[MAX_PATH_LENGTH+1];
    GN_MSW_CHECK_RV( GetModuleFileNameA(0,buf,MAX_PATH_LENGTH), GN::StrA::EMPTYSTR );
    return GN::path::getParent( buf );
#elif GN_POSIX
    char linkName[MAX_PATH_LENGTH+1];
    char realPath[MAX_PATH_LENGTH+1];
    GN::strPrintf( linkName, MAX_PATH_LENGTH, "/proc/%d/exe", getpid() );
    if( 0 == realpath( linkName, realPath) )
    {
        GN_ERROR( "Fail to get real path of file '%s'.", linkName );
        return GN::StrA::EMPTYSTR;
    }
    return GN::path::getParent( realPath );
#else
#error Unknown platform!
#endif
}

//
//
// -----------------------------------------------------------------------------
static void sNormalizePathSeparator( GN::StrA & result, const GN::StrA & path )
{
#if GN_MSWIN
    char from = '/';
    char to = '\\';
#else
    char from = '\\';
    char to = '/';
#endif

    // convert 'from' to 'to'
    result.setCaps( path.size() );
    for( size_t i = 0; i < path.size(); ++i )
    {
        char ch = path[i];

        if( from == ch )
        {
            // ignore redundant path separator
            if( 0 == i || result.last() != to ) result.append( to );
        }
        else if( to != ch || 0 == i || to != result.last() )
        {
            // ignore redundant path separator
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

    // try convert "aaa:..." to "aaa:/...",
    // but leave "aaa/bbb:cccc" as it is.
    bool foundSlash = false;
    for( size_t i = 0; i < result.size(); ++i )
    {
        char ch = result[i];
        foundSlash |= to==ch;
        if( ':' == ch &&
            !foundSlash &&
            result[i+1] != to )
        {
            result.insert( i+1, to );
            break;
        }
    }
}

//
// Return true for path like: "/", "aaa:/", but return false for path like "aaa/bbb:/".
// -----------------------------------------------------------------------------
static inline bool sIsRoot( const GN::StrA & path )
{
    if( path.last() != PATH_SEPARATOR ) return false;

    if( 1 == path.size() ) return true;

    GN_ASSERT( path.size() >= 2 );

    if( ':' != path[path.size()-2] ) return false;

    for( size_t i = 0; i < path.size()-2; ++i )
    {
        if( PATH_SEPARATOR == path[i] ) return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
static void sRecursiveFind( std::vector<GN::StrA> & result,
                            const GN::StrA & dirName,
                            const GN::StrA & pattern,
                            bool recursive,
                            bool useRegex )
{
    GN_GUARD;

    using namespace GN;

    // validate dirName
    GN_ASSERT( path::exist(dirName) && path::isDir(dirName) );

    StrA curDir = path::toNative( path::resolve( dirName ) );

    // search in sub-directories
    if( recursive )
    {
        // TODO: ignore links/junctions
        CSimpleGlobA sg( SG_GLOB_ONLYDIR | SG_GLOB_NODOT );
        StrA p = path::join( curDir, "*" );
        sg.Add( p.cptr() );
        char ** dirs = sg.Files();
        int c = sg.FileCount();
        for( int i = 0; i < c; ++i, ++dirs )
        {
            p = path::join( curDir, *dirs );
            sRecursiveFind( result, p, pattern, recursive, useRegex );
        }
    }

    // search in current directory
    CSimpleGlobA sg( SG_GLOB_ONLYFILE );
    StrA p = path::join( curDir, (useRegex ? "*.*" : pattern) );
    sg.Add( p.cptr() );
    char ** files = sg.Files();
    int c = sg.FileCount();
    for( int i = 0; i < c; ++i, ++files )
    {
        result.push_back( path::join( curDir, *files ) );
    }

    GN_UNGUARD;
}

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::StrA sStartupDir = sPwd();
void GN::path::toNative( StrA & result, const StrA & path )
{
    // handle path prefixes
    StrA tmp;
    size_t pos = 0;
    if( "app:" == path.subString(0,4) )
    {
        static GN::StrA sAppDir = sGetAppDir();
        pos = 4;
        tmp = sAppDir;
        tmp.append( PATH_SEPARATOR );
    }
    else if( "pwd:" == path.subString(0,4) )
    {
        pos = 4;
        tmp = sPwd();
        tmp.append( PATH_SEPARATOR );
    }
    else if( "startup:" == path.subString(0,8) )
    {
        pos = 8;
        tmp = sStartupDir;
        tmp.append( PATH_SEPARATOR );
    }

    tmp.append( path.subString( pos, (size_t)-1 ) );

    // normalize it
    sNormalizePathSeparator( result, tmp );

    // TODO: resolve embbed environment variables
}

//
//
// -----------------------------------------------------------------------------
bool GN::path::exist( const StrA & path )
{
    StrA native = toNative(path);

#if GN_MSWIN
#if GN_PC
    return !!::PathFileExistsA( native.cptr() );
#else
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( native.cptr(), &wfd );
    if( INVALID_HANDLE_VALUE == fh )
    {
        return false;
    }
    else
    {
        ::FindClose( fh );
        return true;
    }
#endif
#else
    if( isDir(native) ) return true;
    FILE * fp = fopen( native.cptr(), "r" );
    if( 0 == fp ) return false;
    fclose( fp );
    return true;
#endif
}

//
//
// -----------------------------------------------------------------------------
bool GN::path::isDir( const StrA & path )
{
    StrA native = toNative(path);

#if GN_MSWIN
#if GN_PC
    return !!::PathIsDirectoryA( native.cptr() );
#else
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( native.cptr(), &wfd );
    if( INVALID_HANDLE_VALUE == fh )
    {
        return false;
    }
    else
    {
        ::FindClose( fh );
        return !!(FILE_ATTRIBUTE_DIRECTORY & wfd.dwFileAttributes);
    }
#endif
#else
    DIR * d = opendir( native.cptr() );
    if( 0 == d ) return false;
    closedir( d );
    return true;
#endif
}

//
//
// -----------------------------------------------------------------------------
bool GN::path::isAbsPath( const StrA & path )
{
    if( path.empty() ) return false;

    StrA tmp;

    sNormalizePathSeparator( tmp, path );

    if( PATH_SEPARATOR == tmp[0] ) return true;

    for( size_t i = 1; i < tmp.size(); ++i )
    {
        if( PATH_SEPARATOR == tmp[i] ) return false;
        if( ':' == tmp[i] ) return true;
    }

    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::path::getParent( StrA & result, const StrA & path )
{
    struct Local
    {
        static inline bool sIsPathSeparator( char ch )
        {
            return ch == PATH_SEPARATOR;
        }
    };

    sNormalizePathSeparator( result, path );
    result.trimRightUntil( &Local::sIsPathSeparator );
    if( !sIsRoot(result) ) result.trimRight( PATH_SEPARATOR );
}

//
//
// -----------------------------------------------------------------------------
void GN::path::getExt( StrA & result, const StrA & path )
{
    StrA tmp;
    sNormalizePathSeparator( tmp, path );
    size_t n = tmp.size();
    while( n > 0 )
    {
        --n;
        if( '.' == tmp[n] )
        {
            tmp.subString( result, n, 0 );
            return;
        }
    }
    // no extension found.
    result.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::path::baseName( StrA & result, const StrA & path )
{
    GN_UNIMPL_WARNING();
    result = path;
}

//
//
// -----------------------------------------------------------------------------
void GN::path::joinTo(
    StrA & result,
    const StrA & path1,
    const StrA & path2,
    const StrA & path3,
    const StrA & path4,
    const StrA & path5 )
{
    StrA tmp;

    const StrA * parts[] = { &path1, &path2, &path3, &path4, &path5 };

    size_t n = sizeof(parts)/sizeof(parts[0]);

    size_t i = 0;

    // ignore leading empty parts
    while( i < n && parts[i]->empty() ) ++i;

    if( i < n ) tmp = *parts[i], ++i;

    for( ; i < n; ++i )
    {
        const StrA & p = *parts[i];
        if( p.empty() ) continue; // ignore empty parts
        tmp.append( PATH_SEPARATOR );
        tmp.append( p );
    }
    sNormalizePathSeparator( result, tmp );
}

//
//
// -----------------------------------------------------------------------------
bool GN::path::resolve( StrA & result, const StrA & path )
{
    GN_GUARD;

    StrA relPath = toNative(path);

#if (GN_MSWIN & GN_PC) || GN_POSIX

    char absPath[MAX_PATH_LENGTH+1];

#if GN_POSIX
    if( 0 == realpath( relPath.cptr(), absPath ) )
#else
    if( 0 == _fullpath( absPath, relPath.cptr(), MAX_PATH_LENGTH ) )
#endif
    {
        GN_ERROR( "invalid path '%s'.", path.cptr() );
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
// -----------------------------------------------------------------------------
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
        GN_WARN( "'%s' does not exist!", dirName.cptr() );
        return result;
    }

    if( !isDir(dirName) )
    {
        GN_WARN( "'%s' is not directory!", dirName.cptr() );
        return result;
    }

    sRecursiveFind( result, dirName, pattern, recursive, useRegex );

    return result;

    GN_UNGUARD;
}
