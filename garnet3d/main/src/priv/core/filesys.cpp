#include "pch.h"

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
using namespace GN::fs;

static Logger * sLogger = GetLogger("GN.base.filesys");

// *****************************************************************************
// local functions for posix platform
// *****************************************************************************

#if GN_POSIX

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsDir( const StrA & path )
{
    DIR * d = opendir( path.ToRawPtr() );
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
    FILE * fp = fopen( path.ToRawPtr(), "r" );
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
    return !path.Empty() && '/' == path[0];
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
    return !!::PathFileExistsA( path.ToRawPtr() );
}

//
//
// -----------------------------------------------------------------------------
static bool sNativeIsDir( const StrA & path )
{
    return !!::PathIsDirectoryA( path.ToRawPtr() );
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
        ( path.Size() > 0 && '/' == path[0] ) ||
        ( path.Size() > 1 && ( 'a' <= path[0] && path[0] <= 'z' || 'A' <= path[0] && path[0] <= 'Z' ) && ':' == path[1] );
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
    HANDLE fh = ::FindFirstFileA( path.ToRawPtr(), &wfd );
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
    HANDLE fh = ::FindFirstFileA( path.ToRawPtr(), &wfd );
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
        ( path.Size() > 0 && '/' == path[0] ) ||
        ( path.Size() > 1 && ( 'a' <= path[0] && path[0] <= 'z' || 'A' <= path[0] && path[0] <= 'Z' ) && ':' == path[1] ) ||
        ( path.Size() > 4 && "game:" == path.SubString( 0, 5 ) );
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

#if GN_XENON

        // convert path separators to native format
        for( size_t i = 0; i < tmp.Size(); ++i )
        {
            if( '/' == tmp[i] ) tmp[i] = '\\';
        }

        // currently do nothing
        result = tmp;

#elif GN_MSWIN
        // convert path separators to native format
        for( size_t i = 0; i < tmp.Size(); ++i )
        {
            if( '/' == tmp[i] ) tmp[i] = '\\';
        }
        // convert to full path
        char absPath[MAX_PATH+1];
        if( 0 == _fullpath( absPath, tmp.ToRawPtr(), MAX_PATH ) )
        {
            GN_ERROR(sLogger)( "invalid path '%s'.", path.ToRawPtr() );
            result.Clear();
            return;
        }
        result = absPath;
#else
        if( !tmp.Empty() && '/' != tmp[0] )
        {
            char cwd[PATH_MAX+1];
            if( 0 == getcwd( cwd, PATH_MAX ) )
            {
                GN_ERROR(sLogger)( "getcwd() failed!" );
                result.Clear();
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
            GN_TRACE(sLogger)( "'%s' does not exist!", dirName.ToRawPtr() );
            return result;
        }

        if( !isDir(dirName) )
        {
            GN_TRACE(sLogger)( "'%s' is not directory!", dirName.ToRawPtr() );
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
        if( !fp->Open( nativeName, mode ) ) return false;
        return fp.Detach();
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
            sg.Add( p.ToRawPtr() );
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
        sg.Add( p.ToRawPtr() );
        char ** files = sg.Files();
        int c = sg.FileCount();
        for( int i = 0; i < c; ++i, ++files )
        {
            result.Append( joinPath( curDir, *files ) );
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
#if GN_XENON
        mRootDir = "game:";
#elif GN_MSWIN
        char buf[MAX_PATH+1];
        GN_MSW_CHECK( GetModuleFileNameA(0,buf,MAX_PATH) );
        mRootDir = parentPath( buf );
#elif GN_POSIX
        char linkName[PATH_MAX+1];
        char realPath[PATH_MAX+1];
        StringPrintf( linkName, PATH_MAX, "/proc/%d/exe", getpid() );
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
        return !path.Empty() && '/' == path[0];
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
        return !path.Empty() && '/' == path[0];
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
        for( size_t i = 0; i < mRoots.Size(); ++i )
        {
            if( GN::fs::pathExist( joinPath( mRoots[i], path ) ) ) return &mRoots[i];
        }
        return 0;
    }

public:

    MultiRootsFileSystem()
    {
    }

    void addRoot( const StrA & root ) { mRoots.Append( root ); }

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
        return !path.Empty() && '/' == path[0];
    }

    void toNativeDiskFilePath( StrA & result, const StrA & path )
    {
        result.Clear();
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
        if( dirName.Empty() )
        {
            for( size_t i = 0; i < mRoots.Size(); ++i )
            {
                const StrA & root = mRoots[i];

                DynaArray<StrA> tmp;

                GN::fs::glob(
                    tmp,
                    joinPath( root, dirName ),
                    pattern,
                    recursive,
                    useRegex );

                for( size_t i = 0; i < tmp.Size(); ++i )
                {
                    if( result.End() == std::find( result.Begin(), result.End(), tmp[i] ) )
                    {
                        result.Append( tmp[i] );
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
            GN_ERROR(sLogger)( "file '%s' not found!", path.ToRawPtr() );
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
    typedef GN::StringMap<char,FileSystem*> Container;

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
        if( name.Empty() )
        {
            GN_ERROR(sLogger)( "File system name can't be empty!" );
            return false;
        }
        if( name.Size() < 3 || ':' != name[name.Size()-1] || ':' != name[name.Size()-2] )
        {
            GN_ERROR(sLogger)( "File system name must be in format like \"your_fs_name::\"!" );
            return false;
        }

        if( 0 == fs )
        {
            GN_ERROR(sLogger)( "Null file system pointer!" );
            return false;
        }

        if( NULL != mFileSystems.Find( name ) )
        {
            GN_ERROR(sLogger)( "File system '%s' already exists!", name.ToRawPtr() );
            return false;
        }

        // success
        mFileSystems[name] = fs;
        return true;
    }

    void UnregisterFs( const StrA & name )
    {
        if( mFileSystems.Find( name ) )
        {
            mFileSystems.Remove( name );
        }
    }

    FileSystem * getFs( const StrA & name )
    {
        if( name.Empty() ) return &mNativeFs;
        FileSystem ** ppfs = mFileSystems.Find( name );
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
GN_EXPORT bool GN::fs::registerFileSystem( const StrA & name, FileSystem * root )
{
    return sGetFileSystemContainer().registerFs( name, root );
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT void GN::fs::UnregisterFileSystem( const StrA & name )
{
    sGetFileSystemContainer().UnregisterFs( name );
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT FileSystem * GN::fs::getFileSystem( const StrA & name )
{
    return sGetFileSystemContainer().getFs( name );
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT void GN::fs::resolvePath( StrA & result, const StrA & base, const StrA & relpath )
{
    // shortcut for empty path
    if( base.Empty() || relpath.Empty() )
    {
        result = relpath;
        return;
    }

    StrA basefs, basec, relfs, relc;
    splitPath( base, basefs, basec );
    splitPath( relpath, relfs, relc );

    if( !relfs.Empty() && basefs != relfs )
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
