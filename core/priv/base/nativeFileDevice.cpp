#include "pch.h"
#include "nativeFileDevice.h"
#include <pcrecpp.h>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
namespace bfs = boost::filesystem;

typedef std::vector<GN::StrA> StrVec;

// ****************************************************************************
//                            local functions
// ****************************************************************************

static void sResursiveFind( StrVec & result,
                            const bfs::path & dirPath,
                            const GN::StrA & pattern,
                            bool recursive,
                            bool useRegex )
{
    GN_GUARD;

    // validate dirPath
    GN_ASSERT( bfs::exists(dirPath) && bfs::is_directory(dirPath) );

    // default construction yields past-the-end
    bfs::directory_iterator end_itr;

    for ( bfs::directory_iterator itr( dirPath ); itr != end_itr; ++itr )
    {
        if( bfs::is_directory( *itr ) )
        {
            if( recursive )
            {
                sResursiveFind( result, *itr, pattern, recursive, useRegex );
            }
        }
        else
        {
            if( useRegex )
            {
                pcrecpp::RE re( pattern.cstr() );
                if( re.FullMatch( itr->leaf() ) )
                {
                    // found!
                    result.push_back( itr->leaf().c_str() );
                }
            }
            else
            {
                if( itr->leaf().c_str() == pattern )
                {
                    // found!
                    result.push_back( pattern );
                    return;
                }
            }
        }
    }

    GN_UNGUARD;
}

//! \brief convert string to boost path
static inline bfs::path
sStr2Path( const GN::StrA & str )
{
    return bfs::path( str.cstr(), bfs::native );
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

    GN_TRY
    {
        GN::AutoRef<GN::AnsiFile> file( new GN::AnsiFile );

        if( !file->open( path, openmode ) )
            return GN::AutoRef<GN::File>();

        // success
        return file;
    }
    GN_CATCH( const bfs::filesystem_error & e )
    {
        GN_ERROR( e.what() );
        return GN::AutoRef<GN::File>();
    }

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::detail::NativeFileDevice::isExist( const GN::StrA & path ) const
{
    GN_GUARD;

    GN_TRY
    {
        return bfs::exists( sStr2Path(path) );
    }
    GN_CATCH( const bfs::filesystem_error & e )
    {
        GN_ERROR( e.what() );
        return false;
    }

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::detail::NativeFileDevice::isDir( const GN::StrA & path ) const
{
    GN_GUARD;

    GN_TRY
    {
        return bfs::is_directory( sStr2Path(path) );
    }
    GN_CATCH( const bfs::filesystem_error & e )
    {
        GN_ERROR( e.what() );
        return false;
    }

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void
GN::detail::NativeFileDevice::findFiles(
    std::vector<GN::StrA> & result,
    const StrA & dirPath,
    const StrA & pattern,
    bool recursive,
    bool useRegex ) const
{
    GN_GUARD;

    GN_TRY
    {
        bfs::path d = sStr2Path(dirPath);

        if( !bfs::exists(d) )
        {
            GN_WARN( "'%s' does not exist!", dirPath.cstr() );
            return;
        }

        if( !bfs::is_directory(d) )
        {
            GN_WARN( "'%s' is not directory!", dirPath.cstr() );
            return;
        }

        sResursiveFind( result, d, pattern, recursive, useRegex );
    }
    GN_CATCH( const bfs::filesystem_error & e )
    {
        GN_ERROR( e.what() );
    }

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
GN::StrA GN::detail::NativeFileDevice::rel2abs( const GN::StrA & path ) const
{
    GN_GUARD;

    GN_TRY
    {
        static GN::StrA s_empty_path( "." );
        return bfs::system_complete(
            sStr2Path( path.empty() ? s_empty_path : path ) ).string().c_str();
    }
    GN_CATCH( const bfs::filesystem_error & e )
    {
        GN_ERROR( e.what() );
        return "";
    }

    GN_UNGUARD;
}
