#include "pch.h"

#if GN_MSVC8
#pragma warning(disable:4996)
#include <SimpleGlob.h>
#pragma warning(default:4996)
#else
#include <SimpleGlob.h>
#endif

#if GN_WINPC
#include <shlwapi.h>
#if GN_MSVC
#pragma comment( lib, "shlwapi.lib" )
#endif
#elif GN_POSIX
#include <dirent.h>
#include <unistd.h>
#endif

using namespace GN;
using namespace GN::fs;

static Logger * sLogger = getLogger("GN.base.filesys");

// *****************************************************************************
// local functions for posix platform
// *****************************************************************************

#if GN_POSIX

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsDir( const StrA & path )
{
    DIR * d = opendir( path.rawptr() );
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
    FILE * fp = fopen( path.rawptr(), "r" );
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

#if GN_WINPC

//
//
// -----------------------------------------------------------------------------
static bool sNativeExist( const StrA & path )
{
    return !!::PathFileExistsA( path.rawptr() );
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsDir( const StrA & path )
{
    return !!::PathIsDirectoryA( path.rawptr() );
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
// local functions for Xbox platform
// *****************************************************************************

#if GN_XBOX2 || GN_XBOX3

//
//
// -----------------------------------------------------------------------------
static bool sNativeExist( const StrA & path )
{
    WIN32_FIND_DATAA wfd;
    HANDLE fh = ::FindFirstFileA( path.rawptr(), &wfd );
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
    HANDLE fh = ::FindFirstFileA( path.rawptr(), &wfd );
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

class NativeFileSystem : public FileSystem
{
public:

    bool exist( const StrA & path )
    {
        return sNativeExist( FileSystem::toNativeDiskFilePath( path ) );
    }

    bool isDir( const StrA & path  )
    {
        return sNativeIsDir( FileSystem::toNativeDiskFilePath( path ) );
    }

    bool isFile( const StrA & path )
    {
        return sNativeIsFile( FileSystem::toNativeDiskFilePath( path ) );
    }

    bool isAbsPath( const StrA & path )
    {
        return sIsAbsPath( path );
    }

    void toNativeDiskFilePath( StrA & result, const StrA & path )
    {
        StrA tmp;

        // normalize path separators
        normalizePathSeparator( tmp, path );

#if GN_XBOX2 || GN_XBOX3

        // convert path separators to native format
        for( size_t i = 0; i < tmp.size(); ++i )
        {
            if( '/' == tmp[i] ) tmp[i] = '\\';
        }

        // currently do nothing
        result = tmp;

#elif GN_WINPC
        // convert path separators to native format
        for( size_t i = 0; i < tmp.size(); ++i )
        {
            if( '/' == tmp[i] ) tmp[i] = '\\';
        }
        // convert to full path
        char absPath[MAX_PATH+1];
        if( 0 == _fullpath( absPath, tmp.rawptr(), MAX_PATH ) )
        {
            GN_ERROR(sLogger)( "invalid path '%s'.", path.rawptr() );
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

    DynaArray<StrA> &
    glob(
        DynaArray<StrA> & result,
        const StrA & dirName,
        const StrA & pattern,
        bool         recursive,
        bool         useRegex )
    {
        GN_GUARD;

        if( !exist(dirName) )
        {
            GN_TRACE(sLogger)( "'%s' does not exist!", dirName.rawptr() );
            return result;
        }

        if( !isDir(dirName) )
        {
            GN_TRACE(sLogger)( "'%s' is not directory!", dirName.rawptr() );
            return result;
        }

        recursiveFind( result, dirName, pattern, recursive, useRegex );

        return result;

        GN_UNGUARD;
    }

     File * openFile( const StrA & name, const StrA & mode )
     {
        StrA nativeName;
        toNativeDiskFilePath( nativeName, name );
        AutoObjPtr<DiskFile> fp( new DiskFile );
        if( !fp->open( nativeName, mode ) ) return false;
        return fp.detach();
     }

private:

    //
    //
    // -----------------------------------------------------------------------------
    void recursiveFind( DynaArray<StrA> & result,
                        const StrA & dirName,
                        const StrA & pattern,
                        bool recursive,
                        bool useRegex )
    {
        GN_GUARD;

        using namespace GN;

        // validate dirName
        GN_ASSERT( exist(dirName) && isDir(dirName) );

        StrA curDir = FileSystem::toNativeDiskFilePath( dirName );

        // search in sub-directories
        if( recursive )
        {
            // TODO: ignore links/junctions
            CSimpleGlobA sg( SG_GLOB_ONLYDIR | SG_GLOB_NODOT );
            StrA p = joinPath( curDir, "*" );
            sg.Add( p.rawptr() );
            char ** dirs = sg.Files();
            int c = sg.FileCount();
            for( int i = 0; i < c; ++i, ++dirs )
            {
                resolvePath( p, curDir, *dirs );
                recursiveFind( result, p, pattern, recursive, useRegex );
            }
        }

        // search in current directory
        CSimpleGlobA sg( SG_GLOB_ONLYFILE );
        StrA p = joinPath( curDir, (useRegex ? "*.*" : pattern) );
        sg.Add( p.rawptr() );
        char ** files = sg.Files();
        int c = sg.FileCount();
        for( int i = 0; i < c; ++i, ++files )
        {
            result.append( joinPath( curDir, *files ) );
        }

        GN_UNGUARD;
    }
};

// *****************************************************************************
// "app::/" root object
// *****************************************************************************

class AppFileSystem : public FileSystem
{
    NativeFileSystem & mNativeFs;
    StrA               mRootDir;

public:

    AppFileSystem( NativeFileSystem & nfs ) : mNativeFs( nfs )
    {
#if GN_XBOX2 || GN_XBOX3
        mRootDir = "game:";
#elif GN_WINPC
        char buf[MAX_PATH+1];
        GN_MSW_CHECK( GetModuleFileNameA(0,buf,MAX_PATH) );
        mRootDir = parentPath( buf );
#elif GN_POSIX
        char linkName[PATH_MAX+1];
        char realPath[PATH_MAX+1];
        str::formatTo( linkName, PATH_MAX, "/proc/%d/exe", getpid() );
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

    void toNativeDiskFilePath( StrA & result, const StrA & path )
    {
        mNativeFs.toNativeDiskFilePath( result, joinPath( mRootDir, path ) );
    }

    DynaArray<StrA> &
    glob(
        DynaArray<StrA> & result,
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

class StartupFileSystem : public FileSystem
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

    void toNativeDiskFilePath( StrA & result, const StrA & path )
    {
        mNativeFs.toNativeDiskFilePath( result, joinPath( mRootDir, path ) );
    }

    DynaArray<StrA> &
    glob(
        DynaArray<StrA> & result,
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

class MultiRootsFileSystem : public FileSystem
{
    DynaArray<StrA> mRoots;

    const StrA * findRoot( const StrA & path )
    {
        for( size_t i = 0; i < mRoots.size(); ++i )
        {
            if( GN::fs::pathExist( joinPath( mRoots[i], path ) ) ) return &mRoots[i];
        }
        return 0;
    }

public:

    MultiRootsFileSystem()
    {
    }

    void addRoot( const StrA & root ) { mRoots.append( root ); }

    bool exist( const StrA & path )
    {
        return 0 != findRoot( path );
    }

    bool isDir( const StrA & path  )
    {
        const StrA * root = findRoot( path );
        if( !root ) return false;
        return GN::fs::isDir( joinPath( *root, path ) );
    }

    bool isFile( const StrA & path )
    {
        const StrA * root = findRoot( path );
        if( !root ) return false;
        return GN::fs::isFile( joinPath( *root, path ) );
    }

    bool isAbsPath( const StrA & path )
    {
        return !path.empty() && '/' == path[0];
    }

    void toNativeDiskFilePath( StrA & result, const StrA & path )
    {
        result.clear();
        const StrA * root = findRoot( path );
        if( !root ) return;
        GN::fs::toNativeDiskFilePath( result, joinPath( *root, path ) );
    }

    DynaArray<StrA> &
    glob(
        DynaArray<StrA> & result,
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

                DynaArray<StrA> tmp;

                GN::fs::glob(
                    tmp,
                    joinPath( root, dirName ),
                    pattern,
                    recursive,
                    useRegex );

                for( size_t i = 0; i < tmp.size(); ++i )
                {
                    if( result.end() == std::find( result.begin(), result.end(), tmp[i] ) )
                    {
                        result.append( tmp[i] );
                    }
                }
            }
        }
        else
        {
            const StrA * root = findRoot( dirName );
            if( !root ) return result;

            GN::fs::glob(
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
            GN_ERROR(sLogger)( "file '%s' not found!", path.rawptr() );
            return 0;
        }
        return GN::fs::openFile( joinPath( *root, path ), mode );
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
#if GN_XBOX2 || GN_XBOX3
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
#if GN_WINPC
        char windir[MAX_PATH+1];
        GetWindowsDirectoryA( windir, MAX_PATH );
        addRoot( joinPath( windir, "fonts" ) );
#endif
    }
};

// *****************************************************************************
// fake file system object
// *****************************************************************************

class FakeFileSystem : public FileSystem
{
public:

    FakeFileSystem()
    {
    }

    bool exist( const StrA & ) { return false; }
    bool isDir( const StrA & ) { return false; }
    bool isFile( const StrA & ) { return false; }
    void toNativeDiskFilePath( StrA & result, const StrA & path ) { result = path; }
    bool isAbsPath( const StrA & ) { return true; }
    DynaArray<StrA> & glob( DynaArray<StrA> & result, const StrA &, const StrA &, bool, bool )
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
    typedef StringMap<char,FileSystem*> Container;

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

    bool registerFs( const StrA & name, FileSystem * fs )
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

        if( NULL != mFileSystems.find( name ) )
        {
            GN_ERROR(sLogger)( "File system '%s' already exists!", name.rawptr() );
            return false;
        }

        // success
        mFileSystems[name] = fs;
        return true;
    }

    void UnregisterFs( const StrA & name )
    {
        if( mFileSystems.find( name ) )
        {
            mFileSystems.remove( name );
        }
    }

    FileSystem * getFs( const StrA & name )
    {
        if( name.empty() ) return &mNativeFs;
        FileSystem ** ppfs = mFileSystems.find( name );
        return ( NULL != ppfs ) ? *ppfs : &mFakeFs;
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
GN_API bool GN::fs::registerFileSystem( const StrA & name, FileSystem * root )
{
    return sGetFileSystemContainer().registerFs( name, root );
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::UnregisterFileSystem( const StrA & name )
{
    sGetFileSystemContainer().UnregisterFs( name );
}

//
//
// -----------------------------------------------------------------------------
GN_API FileSystem * GN::fs::getFileSystem( const StrA & name )
{
    return sGetFileSystemContainer().getFs( name );
}
