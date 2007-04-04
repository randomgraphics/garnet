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
#elif GN_POSIX
#include <dirent.h>
#include <unistd.h>
#endif

using namespace GN;

static Logger * sLogger = getLogger("GN.core::FileSystem");

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
static bool sNativeExist( const StrA & path )
{
    if( sNativeIsDir(path) ) return true;
    FILE * fp = fopen( path.cptr(), "r" );
    if( 0 == fp ) return false;
    fclose( fp );
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsFile( const StrA & path )
{
    return sNativeExist( path ) && !sNativeIsDir( path );
}

//
//
// -----------------------------------------------------------------------------
static bool sIsAbsPath( const StrA & path )
{
    return !path.empty() && '/' == path[0];
}

#endif

// *****************************************************************************
// local functions for win32 platform
// *****************************************************************************

#if GN_MSWIN && GN_PC

//
//
// -----------------------------------------------------------------------------
static bool sNativeExist( const StrA & path )
{
    return !!::PathFileExistsA( path.cptr() );
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsDir( const StrA & path )
{
    return !!::PathIsDirectoryA( path.cptr() );
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsFile( const StrA & path )
{
    return sNativeExist( path ) && !sNativeIsDir( path );
}

//
//
// -----------------------------------------------------------------------------
static bool sIsAbsPath( const StrA & path )
{
    return
        ( path.size() > 0 && '/' == path[0] ) ||
        ( path.size() > 1 && ( 'a' <= path[0] && path[0] <= 'z' || 'A' <= path[0] && path[0] <= 'Z' ) && ':' == path[1] );
}

#endif

// *****************************************************************************
// local functions for Xenon platform
// *****************************************************************************

#if GN_MSWIN && GN_XENON

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
    return sNativeExist( path ) && !sNativeIsDir( path );
}

//
//
// -----------------------------------------------------------------------------
static bool sIsAbsPath( const StrA & path )
{
    return
        ( path.size() > 0 && '/' == path[0] ) ||
        ( path.size() > 1 && ( 'a' <= path[0] && path[0] <= 'z' || 'A' <= path[0] && path[0] <= 'Z' ) && ':' == path[1] ) ||
        ( path.size() > 4 && "game:" == path.subString( 0, 5 ) );
}

#endif

// *****************************************************************************
// "native::/" root object
// *****************************************************************************

class NativeFileSystem : public core::FileSystem
{
public:

    bool exist( const StrA & path )
    {
        return sNativeExist( core::FileSystem::toNative( path ) );
    }

    bool isDir( const StrA & path  )
    {
        return sNativeIsDir( core::FileSystem::toNative( path ) );
    }

    bool isFile( const StrA & path )
    {
        return sNativeIsFile( core::FileSystem::toNative( path ) );
    }

    bool isAbsPath( const StrA & path )
    {
        return sIsAbsPath( path );
    }

    void toNative( StrA & result, const StrA & path )
    {
        StrA tmp;

        // normalize path separators
        normalizePathSeparator( tmp, path );

#if GN_XENON

        // currently do nothing
        result = path;

#elif GN_MSWIN
        // convert path separators to native format
        for( size_t i = 0; i < tmp.size(); ++i )
        {
            if( '/' == tmp[i] ) tmp[i] = '\\';
        }
        // convert to full path
        char absPath[MAX_PATH+1];
        if( 0 == _fullpath( absPath, tmp.cptr(), MAX_PATH ) )
        {
            GN_ERROR(sLogger)( "invalid path '%s'.", path.cptr() );
            result.clear();
            return;
        }
        result = absPath;
#else
        if( !tmp.empty() && '/' != tmp[0] )
        {
            char cwd[PATH_MAX+1];
            if( 0 == getcwd( cwd, PATH_MAX ) )
            {
                GN_ERROR(sLogger)( "getcwd() failed!" );
                result.clear();
                return;
            }
            joinPath2( result, cwd, tmp );
        }
        else
        {
            result = tmp;
        }
#endif

        // TODO: resolve embbed environments
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
            GN_TRACE(sLogger)( "'%s' does not exist!", dirName.cptr() );
            return result;
        }

        if( !isDir(dirName) )
        {
            GN_TRACE(sLogger)( "'%s' is not directory!", dirName.cptr() );
            return result;
        }

        recursiveFind( result, dirName, pattern, recursive, useRegex );

        return result;

        GN_UNGUARD;
    }

     File * openFile( const StrA & name, const StrA & mode )
     {
        StrA nativeName;
        toNative( nativeName, name );
        AutoObjPtr<DiskFile> fp( new DiskFile );
        if( !fp->open( nativeName, mode ) ) return false;
        return fp.detach();
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

        StrA curDir = core::FileSystem::toNative( dirName );

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
                core::resolvePath( p, curDir, *dirs );
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

class AppFileSystem : public core::FileSystem
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
        char linkName[PATH_MAX+1];
        char realPath[PATH_MAX+1];
        strPrintf( linkName, PATH_MAX, "/proc/%d/exe", getpid() );
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

    bool isAbsPath( const StrA & path )
    {
        return !path.empty() && '/' == path[0];
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

     File * openFile( const StrA & path, const StrA & mode )
     {
        return mNativeFs.openFile( joinPath( mRootDir, path ), mode );
     }
};

// *****************************************************************************
// "startup::/" file system object
// *****************************************************************************

class StartupFileSystem : public core::FileSystem
{
    NativeFileSystem & mNativeFs;
    StrA               mRootDir;

public:

    StartupFileSystem( NativeFileSystem & nfs )
        : mNativeFs( nfs )
        , mRootDir( getCurrentDir() )
    {
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

    bool isAbsPath( const StrA & path )
    {
        return !path.empty() && '/' == path[0];
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

     File * openFile( const StrA & path, const StrA & mode )
     {
        return mNativeFs.openFile( joinPath( mRootDir, path ), mode );
     }
};

// *****************************************************************************
// File system that has multiple root directory
// *****************************************************************************

class MultiRootsFileSystem : public core::FileSystem
{
    std::vector<StrA> mRoots;

    const StrA * findRoot( const StrA & path )
    {
        for( size_t i = 0; i < mRoots.size(); ++i )
        {
            if( core::exist( joinPath( mRoots[i], path ) ) ) return &mRoots[i];
        }
        return 0;
    }

public:    

    MultiRootsFileSystem()
    {
    }

    void addRoot( const StrA & root ) { mRoots.push_back( root ); }

    bool exist( const StrA & path )
    {
        return 0 != findRoot( path );
    }

    bool isDir( const StrA & path  )
    {
        const StrA * root = findRoot( path );
        if( !root ) return false;
        return core::isDir( joinPath( *root, path ) );
    }

    bool isFile( const StrA & path )
    {
        const StrA * root = findRoot( path );
        if( !root ) return false;
        return core::isFile( joinPath( *root, path ) );
    }

    bool isAbsPath( const StrA & path )
    {
        return !path.empty() && '/' == path[0];
    }

    void toNative( StrA & result, const StrA & path )
    {
        result.clear();
        const StrA * root = findRoot( path );
        if( !root ) return;
        core::toNative( result, joinPath( *root, path ) );
    }

    std::vector<StrA> &
    glob(
        std::vector<StrA> & result,
        const StrA & dirName,
        const StrA & pattern,
        bool         recursive,
        bool         useRegex )
    {
        if( dirName.empty() )
        {
            for( size_t i = 0; i < mRoots.size(); ++i )
            {
                const StrA & root = mRoots[i];

                std::vector<StrA> tmp;

                core::glob(
                    tmp,
                    joinPath( root, dirName ),
                    pattern,
                    recursive,
                    useRegex );

                for( size_t i = 0; i < tmp.size(); ++i )
                {
                    if( result.end() == std::find( result.begin(), result.end(), tmp[i] ) )
                    {
                        result.push_back( tmp[i] );
                    }
                }
            }
        }
        else
        {
            const StrA * root = findRoot( dirName );
            if( !root ) return result;

            core::glob(
                result,
                joinPath( *root, dirName ),
                pattern,
                recursive,
                useRegex );
        }

        return result;
    }

     File * openFile( const StrA & path, const StrA & mode )
     {
        const StrA * root = findRoot( path );
        if( !root )
        {
            GN_ERROR(sLogger)( "file '%s' not found!", path.cptr() );
            return 0;
        }
        return core::openFile( joinPath( *root, path ), mode );
     }
};

// *****************************************************************************
// Media file system, handle file path prefixed with "media::"
// *****************************************************************************

class MediaFileSystem : public MultiRootsFileSystem
{
public:    

    MediaFileSystem()
    {
#if GN_XENON
        addRoot( "game:/media" );
#endif
        addRoot( "startup::media" );
        addRoot( "app::media" );
        addRoot( "app::../media" );
    }
};

// *****************************************************************************
// Font file system, handle file path prefixed with "font::"
// *****************************************************************************

class FontFileSystem : public MultiRootsFileSystem
{
public:    

    FontFileSystem()
    {
        addRoot( "media::font" );
#if GN_MSWIN && !GN_XENON
        char windir[MAX_PATH+1];
        GetWindowsDirectoryA( windir, MAX_PATH );
        addRoot( joinPath( windir, "fonts" ) );
#endif
    }
};

// *****************************************************************************
// fake file system object
// *****************************************************************************

class FakeFileSystem : public core::FileSystem
{
public:

    FakeFileSystem()
    {
    }

    bool exist( const StrA & ) { return false; }
    bool isDir( const StrA & ) { return false; }
    bool isFile( const StrA & ) { return false; }
    void toNative( StrA & result, const StrA & path ) { result = path; }
    bool isAbsPath( const StrA & ) { return true; }
    std::vector<StrA> & glob( std::vector<StrA> & result, const StrA &, const StrA &, bool, bool )
    {
        return result;
    }
    File * openFile( const StrA &, const StrA & ) { return 0; }
};

// *****************************************************************************
// File system container
// *****************************************************************************


struct FileSystemContainer
{
    typedef std::map<StrA,core::FileSystem*> Container;

    Container mFileSystems;

    FakeFileSystem    mFakeFs;
    NativeFileSystem  mNativeFs;
    AppFileSystem     mAppFs;
    StartupFileSystem mStartupFs;
    MediaFileSystem   mMediaFs;
    FontFileSystem    mFontFs;

    FileSystemContainer()
        : mAppFs( mNativeFs )
        , mStartupFs( mNativeFs )
    {
        // register default file systems
        registerFs( "native::", &mNativeFs );
        registerFs( "app::", &mAppFs );
        registerFs( "startup::", &mStartupFs );
        registerFs( "media::", &mMediaFs );
        registerFs( "font::", &mFontFs );
    }

    ~FileSystemContainer()
    {
    }

    bool registerFs( const StrA & name, core::FileSystem * fs )
    {
        // check name
        if( name.empty() )
        {
            GN_ERROR(sLogger)( "File system name can't be empty!" );
            return false;
        }
        if( name.size() < 3 || ':' != name[name.size()-1] || ':' != name[name.size()-2] )
        {
            GN_ERROR(sLogger)( "File system name must be in format like \"your_fs_name::\"!" );
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

    core::FileSystem * getFs( const StrA & name )
    {
        if( name.empty() ) return &mNativeFs;
        Container::const_iterator i = mFileSystems.find( name );
        return ( mFileSystems.end() != i ) ? i->second : &mFakeFs;
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
GN_EXPORT bool core::registerFileSystem( const StrA & name, FileSystem * root )
{
    return sGetFileSystemContainer().registerFs( name, root );
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT void core::UnregisterFileSystem( const StrA & name )
{
    sGetFileSystemContainer().UnregisterFs( name );
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT core::FileSystem * core::getFileSystem( const StrA & name )
{
    return sGetFileSystemContainer().getFs( name );
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT void GN::core::resolvePath( StrA & result, const StrA & base, const StrA & relpath )
{
    // shortcut for empty path
    if( base.empty() || relpath.empty() )
    {
        result = relpath;
        return;
    }

    StrA basefs, basec, relfs, relc;
    splitPath( base, basefs, basec );
    splitPath( relpath, relfs, relc );

    if( !relfs.empty() && basefs != relfs )
    {
        result = relpath;
        return;
    }

    if( getFileSystem(relfs)->isAbsPath(relc) )
    {
        result = relpath;
        return;
    }

    joinPath2( result, base, relc );
}
