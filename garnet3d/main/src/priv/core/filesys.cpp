#include "pch.h"
#include "garnet/GNcore.h"
#include <pcrecpp.h>

#if GN_MSVC8
#pragma warning(disable:4996)
#include <SimpleGlob.h>
#pragma warning(default:4996)
#else
#include <SimpleGlob.h>
#endif

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

using namespace GN;

static Logger * sLogger = getLogger("GN.core.fs::FileSystem");

// *****************************************************************************
// local functions for posix platform
// *****************************************************************************

#if GN_POSIX

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsDir( const StrA & path )
{
    DIR * d = opendir( path.cptr() );
    if( 0 == d ) return false;
    closedir( d );
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsFile( const StrA & path )
{
    return sNativeExist( path ) && !sNativeIsFile( path );
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeExist( const StrA & path )
{
    if( isDir(path) ) return true;
    FILE * fp = fopen( path.cptr(), "r" );
    if( 0 == fp ) return false;
    fclose( fp );
    return true;
}

#endif

// *****************************************************************************
// local functions for win32 platform
// *****************************************************************************

#if GN_MSWIN && GN_PC

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsDir( const StrA & path )
{
#if GN_MSWIN
#if GN_PC
    return !!::PathIsDirectoryA( path.cptr() );
#else
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( path.cptr(), &wfd );
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
    DIR * d = opendir( path.cptr() );
    if( 0 == d ) return false;
    closedir( d );
    return true;
#endif
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeExist( const StrA & path )
{
#if GN_MSWIN
#if GN_PC
    return !!::PathFileExistsA( path.cptr() );
#else
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( path.cptr(), &wfd );
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
    if( sNativeIsDir(path) ) return true;
    FILE * fp = fopen( path.cptr(), "r" );
    if( 0 == fp ) return false;
    fclose( fp );
    return true;
#endif
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsFile( const StrA & path )
{
    return sNativeExist( path ) && !sNativeIsFile( path );
}

#endif

// *****************************************************************************
// local functions for Xenon platform
// *****************************************************************************

#if GN_MSWIN && GN_XENON

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsDir( const StrA & path )
{
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( path.cptr(), &wfd );
    if( INVALID_HANDLE_VALUE == fh )
    {
        return false;
    }
    else
    {
        ::FindClose( fh );
        return !!(FILE_ATTRIBUTE_DIRECTORY & wfd.dwFileAttributes);
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsFile( const StrA & path )
{
    return sNativeExist( path ) && !sNativeIsFile( path );
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeExist( const StrA & path )
{
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( path.cptr(), &wfd );
    if( INVALID_HANDLE_VALUE == fh )
    {
        return false;
    }
    else
    {
        ::FindClose( fh );
        return true;
    }
}

#endif

// *****************************************************************************
// "native::/" root object
// *****************************************************************************

class NativeFileSystem : public fs::FileSystem
{
public:

    bool exist( const StrA & path )
    {
        return sNativeExist( fs::FileSystem::toNative( path ) );
    }

    bool isDir( const StrA & path  )
    {
        return sNativeIsDir( fs::FileSystem::toNative( path ) );
    }

    bool isFile( const StrA & path )
    {
        return sNativeIsFile( fs::FileSystem::toNative( path ) );
    }

    void toNative( StrA & result, const StrA & path )
    {
        StrA tmp;

        // normalize path separators
        normalizePathSeparator( tmp, path );

        // convert path separators to native format
#if GN_MSWIN
        for( size_t i = 0; i < tmp.size(); ++i )
        {
            if( '/' == tmp[i] ) tmp[i] = '\\';
        }
#endif

        // convert to full path
#if GN_MSWIN
        char absPath[MAX_PATH+1];
        if( 0 == _fullpath( absPath, tmp.cptr(), MAX_PATH ) )
#else
        char absPath[PATH_MAX+1];
        if( 0 == realpath( tmp.cptr(), absPath ) )
#endif
        {
            GN_ERROR(sLogger)( "invalid path '%s'.", path.cptr() );
            result.clear();
            return;
        }

        // TODO: resolve embbed environments

        // done
        result = absPath;
    }

    std::vector<StrA> &
    glob(
        std::vector<StrA> & result,
        const StrA & dirName,
        const StrA & pattern,
        bool         recursive,
        bool         useRegex )
    {
        GN_GUARD;

        if( !exist(dirName) )
        {
            GN_WARN(sLogger)( "'%s' does not exist!", dirName.cptr() );
            return result;
        }

        if( !isDir(dirName) )
        {
            GN_WARN(sLogger)( "'%s' is not directory!", dirName.cptr() );
            return result;
        }

        recursiveFind( result, dirName, pattern, recursive, useRegex );

        return result;

        GN_UNGUARD;
    }

private:

    //
    //
    // -----------------------------------------------------------------------------
    void recursiveFind( std::vector<StrA> & result,
                        const StrA & dirName,
                        const StrA & pattern,
                        bool recursive,
                        bool useRegex )
    {
        GN_GUARD;

        using namespace GN;

        // validate dirName
        GN_ASSERT( exist(dirName) && isDir(dirName) );

        StrA curDir = fs::FileSystem::toNative( dirName );

        // search in sub-directories
        if( recursive )
        {
            // TODO: ignore links/junctions
            CSimpleGlobA sg( SG_GLOB_ONLYDIR | SG_GLOB_NODOT );
            StrA p = joinPath( curDir, "*" );
            sg.Add( p.cptr() );
            char ** dirs = sg.Files();
            int c = sg.FileCount();
            for( int i = 0; i < c; ++i, ++dirs )
            {
                p = joinPath( curDir, *dirs );
                recursiveFind( result, p, pattern, recursive, useRegex );
            }
        }

        // search in current directory
        CSimpleGlobA sg( SG_GLOB_ONLYFILE );
        StrA p = joinPath( curDir, (useRegex ? "*.*" : pattern) );
        sg.Add( p.cptr() );
        char ** files = sg.Files();
        int c = sg.FileCount();
        for( int i = 0; i < c; ++i, ++files )
        {
            result.push_back( joinPath( curDir, *files ) );
        }

        GN_UNGUARD;
    }
};

// *****************************************************************************
// "app::/" root object
// *****************************************************************************

class AppFileSystem : public fs::FileSystem
{
    NativeFileSystem & mNativeFs;
    StrA               mRootDir;

public:

    AppFileSystem( NativeFileSystem & nfs ) : mNativeFs( nfs )
    {
#if GN_XENON
        mRootDir = "game:";
#elif GN_MSWIN
        char buf[MAX_PATH+1];
        GN_MSW_CHECK( GetModuleFileNameA(0,buf,MAX_PATH) );
        mRootDir = parentPath( buf );
#elif GN_POSIX
        char linkName[MAX_PATH+1];
        char realPath[MAX_PATH+1];
        strPrintf( linkName, MAX_PATH, "/proc/%d/exe", getpid() );
        if( 0 == realpath( linkName, realPath) )
        {
            GN_ERROR(sLogger)( "Fail to get real path of file '%s'.", linkName );
        }
        else
        {
            mRootDir = parentPath( realPath );
        }
#else
#error Unknown platform!
#endif
    }

    bool exist( const StrA & path )
    {
        return mNativeFs.exist( joinPath( mRootDir, path ) );
    }

    bool isDir( const StrA & path  )
    {
        return mNativeFs.isDir( joinPath( mRootDir, path ) );
    }

    bool isFile( const StrA & path )
    {
        return mNativeFs.isFile( joinPath( mRootDir, path ) );
    }

    void toNative( StrA & result, const StrA & path )
    {
        mNativeFs.toNative( result, joinPath( mRootDir, path ) );
    }

    std::vector<StrA> &
    glob(
        std::vector<StrA> & result,
        const StrA & dirName,
        const StrA & pattern,
        bool         recursive,
        bool         useRegex )
    {
        return mNativeFs.glob(
            result,
            joinPath( mRootDir, dirName ),
            pattern,
            recursive,
            useRegex );
    }
};

// *****************************************************************************
// "startup::/" file system object
// *****************************************************************************

class StartupFileSystem : public fs::FileSystem
{
    NativeFileSystem & mNativeFs;
    StrA               mRootDir;

public:

    StartupFileSystem( NativeFileSystem & nfs ) : mNativeFs( nfs )
    {
#if GN_XENON
        return "game:";
#elif GN_MSWIN
        char buf[MAX_PATH+1];
        _getcwd( buf, MAX_PATH );
        buf[MAX_PATH] = 0;
        mRootDir = buf;
        mRootDir.trimRight( '\\' );
#elif GN_POSIX
        char buf[PATH_MAX+1];
        getcwd( buf, PATH_MAX );
        buf[PATH_MAX] = 0;
        mRootDir = buf;
        mRootDir.trimRight( '/' );
#else
#error Unknown platform!
#endif
    }

    bool exist( const StrA & path )
    {
        return mNativeFs.exist( joinPath( mRootDir, path ) );
    }

    bool isDir( const StrA & path  )
    {
        return mNativeFs.isDir( joinPath( mRootDir, path ) );
    }

    bool isFile( const StrA & path )
    {
        return mNativeFs.isFile( joinPath( mRootDir, path ) );
    }

    void toNative( StrA & result, const StrA & path )
    {
        mNativeFs.toNative( result, joinPath( mRootDir, path ) );
    }

    std::vector<StrA> &
    glob(
        std::vector<StrA> & result,
        const StrA & dirName,
        const StrA & pattern,
        bool         recursive,
        bool         useRegex )
    {
        return mNativeFs.glob(
            result,
            joinPath( mRootDir, dirName ),
            pattern,
            recursive,
            useRegex );
    }
};

// *****************************************************************************
// File system container
// *****************************************************************************


struct FileSystemContainer
{
    typedef std::map<StrA,fs::FileSystem*> Container;

    Container mFileSystems;

    NativeFileSystem  mNativeFs;
    AppFileSystem     mAppFs;
    StartupFileSystem mStartupFs;

    FileSystemContainer()
        : mAppFs( mNativeFs )
        , mStartupFs( mNativeFs )
    {
        // register default file systems
        registerFs( "native", &mNativeFs );
        registerFs( "app", &mAppFs );
        registerFs( "startup", &mStartupFs );
    }

    ~FileSystemContainer()
    {
    }

    bool registerFs( const StrA & name, fs::FileSystem * fs )
    {
        if( name.empty() )
        {
            GN_ERROR(sLogger)( "File system name can't be empty!" );
            return false;
        }

        if( 0 == fs )
        {
            GN_ERROR(sLogger)( "Null file system pointer!" );
            return false;
        }

        if( mFileSystems.end() != mFileSystems.find( name ) )
        {
            GN_ERROR(sLogger)( "File system '%s' already exists!", name.cptr() );
            return false;
        }

        // success
        mFileSystems[name] = fs;
        return true;
    }

    void UnregisterFs( const StrA & name )
    {
        Container::const_iterator i = mFileSystems.find( name );
        if( mFileSystems.end() != i )
        {
            mFileSystems.erase( name );
        }
    }

    fs::FileSystem * getFs( const StrA & name )
    {
        if( name.empty() ) return &mNativeFs;
        Container::const_iterator i = mFileSystems.find( name );
        return ( mFileSystems.end() != i ) ? i->second : NULL;
    }
};

FileSystemContainer & sGetFileSystemContainer()
{
    static FileSystemContainer sContainer;
    return sContainer;
}

// *****************************************************************************
// Public functions
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
GN_EXPORT bool fs::registerFileSystem( const StrA & name, FileSystem * root )
{
    return sGetFileSystemContainer().registerFs( name, root );
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT void fs::UnregisterFileSystem( const StrA & name )
{
    sGetFileSystemContainer().UnregisterFs( name );
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT fs::FileSystem * fs::getFileSystem( const StrA & name )
{
    return sGetFileSystemContainer().getFs( name );
}
