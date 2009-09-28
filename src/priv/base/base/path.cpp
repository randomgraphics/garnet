#include "pch.h"

// *****************************************************************************
// Local types, functions and variables
// *****************************************************************************

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::fs::normalizePathSeparator( GN::StrA & result, const GN::StrA & path )
{
    StrA tmp;

    // remove redundent separators, convert to unix style as well
    tmp.setCaps( path.size() );
    for( size_t i = 0; i < path.size(); ++i )
    {
        char ch = path[i];

        if( '\\' == ch ) ch = '/';

        if( '/' == ch && !tmp.empty() && '/' == tmp.last() && 1 != i )
        {
            // ignore redundant path separator
            continue;
        }

        tmp.append( ch );
    }

    // remove trailing separators.
    if( !tmp.empty() )
    {
        tmp.trimRight( '/' );
        if( tmp.empty() ) tmp.append( '/' );
    }

    // make sure no pending path separator
    GN_ASSERT( tmp.empty() || 1 == tmp.size() || '/' != tmp.last() );

    // success
    result = tmp;
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::parentPath( StrA & result, const StrA & path )
{
    struct Local
    {
        static bool isPathSeparator( char ch ) { return '/' == ch; }
    };
    StrA root, child;
    splitPath( path, root, child );
    child.trimRightUntil( Local::isPathSeparator );
    if( child.size() > 1 ) child.popback();
    result = root + child;
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::extName( StrA & result, const StrA & path )
{
    StrA tmp;

    normalizePathSeparator( tmp, path );

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
void GN::fs::baseName( StrA & result, const StrA & path )
{
    StrA tmp;
    normalizePathSeparator( tmp, path );

    StrA dir, ext;

    dirName( dir, tmp );
    extName( ext, tmp );

    size_t n1 = ( dir.size() < tmp.size() && '/' == tmp[dir.size()] ) ? dir.size()+1 : dir.size();
    size_t n2 = tmp.size() - n1 - ext.size();

    result.assign( tmp.cptr() + n1, n2 );
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::relPath( StrA & result, const StrA & path, const StrA & base )
{
    GN_GUARD;

    struct Local
    {
        StrA             input;
        DynaArray<char>  buf;
        DynaArray<char*> parts;

        void split( const StrA & path )
        {
            parts.clear();
            normalizePathSeparator( input, path );
            buf.resize( input.size() + 1 );
            memcpy( buf.cptr(), input.cptr(), input.size() + 1 );
            parts.append( buf.cptr() );
            for( size_t i = 0; i < buf.size() - 1; ++i )
            {
                if( '/' == buf[i] )
                {
                    buf[i] = 0;
                    parts.append( buf.cptr() + i + 1 );
                }
            }
        }
    };

    // shortcut for empty input strings.
    if( path.empty() || base.empty() )
    {
        normalizePathSeparator( result, path );
        return;
    }

    // split input path into components by PATH_SEPARATOR.
    Local p, b;
    p.split( path );
    b.split( base );

    // find the commen prefix between path and base.
    size_t n = math::getmin( p.parts.size(), b.parts.size() );
    size_t i;
    for( i = 0; i < n; ++i )
    {
        const char * s1 = p.parts[i];
        const char * s2 = b.parts[i];
#if GN_MSWIN
        if( 0 != strCmpI( s1, s2 ) ) break;
#else
        if( 0 != strCmp( s1, s2 ) ) break;
#endif
    }
    // here, "i" should point to the first different part.

    // compose result path
    result.clear();
    if( i > 0 ) for( size_t j = i; j < b.parts.size(); ++j )
    {
        result.append( ".." );
        result.append( '/' );
    }
    for( size_t j = i; j < p.parts.size(); ++j )
    {
        result.append( p.parts[j] );
        result.append( '/' );
    }
    result.trimRight( '/' );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::joinPath2(
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
        tmp.append( '/' );
        tmp.append( p );
    }

    normalizePathSeparator( result, tmp );
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::splitPath( const StrA & path, StrA & root, StrA & child )
{
    root.clear();
    child.clear();

    if( path.empty() ) return;

    StrA tmpChild;

    size_t i = 0;
    size_t n = path.size() - 1;
    bool hasroot = false;
    while( i < n )
    {
        char ch1 = path[i];
        char ch2 = path[i+1];

        if( '/' == ch1 )
        {
            // found "/" first, no root
            break;
        }
        else if( ':' == ch1 && ':' == ch2 )
        {
            // found!
            path.subString( root, 0, i+2 );
            path.subString( tmpChild, i+2, 0 );
            hasroot = true;
            break;
        }

        ++i;
    }

    if( hasroot )
    {
        normalizePathSeparator( child, tmpChild );
    }
    else
    {
        normalizePathSeparator( child, path );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::getCurrentDir( StrA & result )
{
#if GN_XENON
    result = "game:";
#elif GN_MSWIN
    char buf[MAX_PATH+1];
    char full[MAX_PATH+1];
    GetCurrentDirectoryA( MAX_PATH, buf );
    GetFullPathNameA( buf, MAX_PATH, full, 0 );
    result = full;
    result.trimRight( '/' );
#elif GN_POSIX
    char buf[PATH_MAX+1];
    if( NULL == getcwd( buf, PATH_MAX ) )
    {
        static Logger * sLogger = getLogger( "GN.base.path" );
        GN_ERROR(sLogger)( "getcwd() failed: fail to get current directory." );
        result = "";
    }
    else
    {
        buf[PATH_MAX] = 0;
        result = buf;
        result.trimRight( '/' );
    }
#else
#error Unknown platform!
#endif
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::getCurrentDrive( StrA & result )
{
#if GN_XENON
    result.clear();
#elif GN_MSWIN
    char buf[MAX_PATH+1];
    char full[MAX_PATH+1];
    GetCurrentDirectoryA( MAX_PATH, buf );
    GetFullPathNameA( buf, MAX_PATH, full, 0 );
    GN_ASSERT( ':' == full[1] );
    result.assign( full, 2 );
#elif GN_POSIX
    result.clear();
#else
#error Unknown platform!
#endif
}
