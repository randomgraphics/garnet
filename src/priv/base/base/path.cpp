#include "pch.h"

// *****************************************************************************
// Local types, functions and variables
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.base.path");

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::normalizePathSeparator( GN::StrA & result, const GN::StrA & path )
{
    StrA tmp;
    
    // remove redundent separators, convert to unix style as well
    tmp.setCaps( path.size() );
    for( size_t i = 0; i < path.size(); ++i )
    {
        char ch = path[i];

        if( '\\' == ch ) ch = '/';

        if( '/' == ch && !tmp.empty() && '/' == tmp.last() )
        {
            // ignore redundant path separator
            continue;
        }

        tmp.append( ch );
    }

    // remove trailing separators.
    tmp.trimRight( '/' );
    if( tmp.empty() ) tmp.append( '/' );

    // make sure no pending path separator
    GN_ASSERT( tmp.empty() || 1 == tmp.size() || '/' != tmp[0] );

    // success
    result = tmp;
}

//
//
// -----------------------------------------------------------------------------
void GN::parentPath( StrA & result, const StrA & path )
{
    normalizePathSeparator( result, path );
    result.trimRight( '/' );
    if( result.size() > 1 ) result.popback();
}

//
//
// -----------------------------------------------------------------------------
void GN::extName( StrA & result, const StrA & path )
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
void GN::baseName( StrA & result, const StrA & path )
{
    GN_UNIMPL_WARNING();
    result = path;
}

//
//
// -----------------------------------------------------------------------------
void GN::relPath( StrA & result, const StrA & path, const StrA & base )
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
    size_t n = min( p.parts.size(), b.parts.size() );
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
void GN::joinPath2(
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
void GN::splitPath( const StrA & path, StrA & root, StrA & child )
{
    StrA tmp;
    normalizePathSeparator( tmp, path );

    root.clear();
    child.clear();

    if( tmp.empty() ) return;

    size_t i = 0;
    size_t n = tmp.size() - 1;
    while( i < n )
    {
        char ch1 = tmp[i];
        char ch2 = tmp[i+1];

        if( '/' == ch1 )
        {
            root = "";
            child = path;
            return;
        }
        else if( ':' == ch1 && ':' == ch2 )
        {
            // found!
            path.subString( root, 0, i+2 );
            path.subString( child, i+2, 0 );
        }
    }
}
