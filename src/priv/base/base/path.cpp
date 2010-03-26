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
void GN::fs::NormalizePathSeparator( GN::StrA & result, const GN::StrA & path )
{
    StrA tmp;

    // remove redundent separators, convert to unix style as well
    tmp.SetCaps( path.Size() );
    for( size_t i = 0; i < path.Size(); ++i )
    {
        char ch = path[i];

        if( '\\' == ch ) ch = '/';

        if( '/' == ch && !tmp.Empty() && '/' == tmp.GetLast() && 1 != i )
        {
            // ignore redundant path separator
            continue;
        }

        tmp.Append( ch );
    }

    // remove trailing separators.
    if( !tmp.Empty() )
    {
        tmp.TrimRight( '/' );
        if( tmp.Empty() ) tmp.Append( '/' );
    }

    // make sure no pending path separator
    GN_ASSERT( tmp.Empty() || 1 == tmp.Size() || '/' != tmp.GetLast() );

    // success
    result = tmp;
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::ParentPath( StrA & result, const StrA & path )
{
    struct Local
    {
        static bool isPathSeparator( char ch ) { return '/' == ch; }
    };
    StrA root, child;
    SplitPath( path, root, child );
    child.TrimRightUntil( Local::isPathSeparator );
    if( child.Size() > 1 ) child.PopBack();
    result = root + child;
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::ExtName( StrA & result, const StrA & path )
{
    StrA tmp;

    NormalizePathSeparator( tmp, path );

    size_t n = tmp.Size();

    while( n > 0 )
    {
        --n;
        if( '.' == tmp[n] )
        {
            tmp.SubString( result, n, 0 );
            return;
        }
    }

    // no extension found.
    result.Clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::BaseName( StrA & result, const StrA & path )
{
    StrA tmp;
    NormalizePathSeparator( tmp, path );

    StrA dir, ext;

    DirName( dir, tmp );
    ExtName( ext, tmp );

    size_t n1 = ( dir.Size() < tmp.Size() && '/' == tmp[dir.Size()] ) ? dir.Size()+1 : dir.Size();
    size_t n2 = tmp.Size() - n1 - ext.Size();

    result.Assign( tmp.ToRawPtr() + n1, n2 );
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::RelPath( StrA & result, const StrA & path, const StrA & base )
{
    GN_GUARD;

    struct Local
    {
        StrA             input;
        DynaArray<char>  buf;
        DynaArray<char*> parts;

        void split( const StrA & path )
        {
            parts.Clear();
            NormalizePathSeparator( input, path );
            buf.Resize( input.Size() + 1 );
            memcpy( buf.ToRawPtr(), input.ToRawPtr(), input.Size() + 1 );
            parts.Append( buf.ToRawPtr() );
            for( size_t i = 0; i < buf.Size() - 1; ++i )
            {
                if( '/' == buf[i] )
                {
                    buf[i] = 0;
                    parts.Append( buf.ToRawPtr() + i + 1 );
                }
            }
        }
    };

    // shortcut for empty input strings.
    if( path.Empty() || base.Empty() )
    {
        NormalizePathSeparator( result, path );
        return;
    }

    // split input path into components by PATH_SEPARATOR.
    Local p, b;
    p.split( path );
    b.split( base );

    // find the commen prefix between path and base.
    size_t n = math::GetMin( p.parts.Size(), b.parts.Size() );
    size_t i;
    for( i = 0; i < n; ++i )
    {
        const char * s1 = p.parts[i];
        const char * s2 = b.parts[i];
#if GN_MSWIN
        if( 0 != StringCompareI( s1, s2 ) ) break;
#else
        if( 0 != StringCompare( s1, s2 ) ) break;
#endif
    }
    // here, "i" should point to the first different part.

    // compose result path
    result.Clear();
    if( i > 0 ) for( size_t j = i; j < b.parts.Size(); ++j )
    {
        result.Append( ".." );
        result.Append( '/' );
    }
    for( size_t j = i; j < p.parts.Size(); ++j )
    {
        result.Append( p.parts[j] );
        result.Append( '/' );
    }
    result.TrimRight( '/' );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::JoinPath2(
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
    while( i < n && parts[i]->Empty() ) ++i;

    if( i < n ) tmp = *parts[i], ++i;

    for( ; i < n; ++i )
    {
        const StrA & p = *parts[i];
        if( p.Empty() ) continue; // ignore empty parts
        tmp.Append( '/' );
        tmp.Append( p );
    }

    NormalizePathSeparator( result, tmp );
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::SplitPath( const StrA & path, StrA & root, StrA & child )
{
    root.Clear();
    child.Clear();

    if( path.Empty() ) return;

    StrA tmpChild;

    size_t i = 0;
    size_t n = path.Size() - 1;
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
            path.SubString( root, 0, i+2 );
            path.SubString( tmpChild, i+2, 0 );
            hasroot = true;
            break;
        }

        ++i;
    }

    if( hasroot )
    {
        NormalizePathSeparator( child, tmpChild );
    }
    else
    {
        NormalizePathSeparator( child, path );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::GetCurrentDir( StrA & result )
{
#if GN_XENON
    result = "game:";
#elif GN_MSWIN
    char buf[MAX_PATH+1];
    char full[MAX_PATH+1];
    GetCurrentDirectoryA( MAX_PATH, buf );
    GetFullPathNameA( buf, MAX_PATH, full, 0 );
    result = full;
    result.TrimRight( '/' );
#elif GN_POSIX
    char buf[PATH_MAX+1];
    if( NULL == getcwd( buf, PATH_MAX ) )
    {
        static Logger * sLogger = GetLogger( "GN.base.path" );
        GN_ERROR(sLogger)( "getcwd() failed: fail to get current directory." );
        result = "";
    }
    else
    {
        buf[PATH_MAX] = 0;
        result = buf;
        result.TrimRight( '/' );
    }
#else
#error Unknown platform!
#endif
}

//
//
// -----------------------------------------------------------------------------
void GN::fs::GetCurrentDrive( StrA & result )
{
#if GN_XENON
    result.Clear();
#elif GN_MSWIN
    char buf[MAX_PATH+1];
    char full[MAX_PATH+1];
    GetCurrentDirectoryA( MAX_PATH, buf );
    GetFullPathNameA( buf, MAX_PATH, full, 0 );
    GN_ASSERT( ':' == full[1] );
    result.Assign( full, 2 );
#elif GN_POSIX
    result.Clear();
#else
#error Unknown platform!
#endif
}
