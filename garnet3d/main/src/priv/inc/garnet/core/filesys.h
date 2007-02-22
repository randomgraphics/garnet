#ifndef __GN_CORE_FILESYS_H__
#define __GN_CORE_FILESYS_H__
// *****************************************************************************
//! \file    core/filesys.h
//! \brief   platform independent file system
//! \author  chen@@CHENLI-HOMEPC (2007.1.29)
// *****************************************************************************

namespace GN
{
    ///
    /// namespace for garnet file system
    ///
    namespace fs
    {
    }
}

namespace GN { namespace fs
{
    ///
    /// Abstract file system class
    ///
    struct FileSystem : public NoCopy
    {
        ///
        /// 路径是否存在
        ///
        virtual bool exist( const StrA & ) = 0;

        ///
        /// if the path points to a directoy?
        ///
        virtual bool isDir( const StrA & ) = 0;

        ///
        /// if the path points to a file
        ///
        virtual bool isFile( const StrA & path ) = 0;

        ///
        /// if the path is absolute path
        ///
        virtual bool isAbsPath( const StrA & path ) = 0;

        ///
        /// Conver path to platform native format.
        ///   - normalize path separator:
        ///     - convert path separators to platform specific format.
        ///     - Remove redundant path separators, such as "c:\\path\" will be
        ///       convert to "c:\path".
        ///   - covert to full path name
        ///   - Resolve embbed environment variable, like this:
        ///     "${windir}/system32" -> "c:\\windows\\system32"
        ///
        virtual void toNative( StrA & result, const StrA & path ) = 0;

        ///
        /// Conver path to platform native format.
        ///
        inline StrA toNative( const StrA & path )
        {
            StrA ret;
            toNative( ret, path );
            return ret;
        }

        ///
        /// 查找指定文件
        ///
        /// \param result     返回搜索结果
        /// \param dirName    在什么目录下
        /// \param pattern    搜索什么文件
        /// \param recursive  是否递归搜索子目录
        /// \param useRegex   是否使用正则匹配
        /// \return           返回参数 result
        ///
        virtual std::vector<StrA> &
        glob(
            std::vector<StrA> & result,
            const StrA & dirName,
            const StrA & pattern,
            bool         recursive,
            bool         useRegex ) = 0;

        ///
        /// open file. Note that meaning of mode is identical with standard fopen().
        ///
        virtual File * openFile( const StrA & path, const StrA & mode ) = 0;
    };

    /// \name managing file system objects
    ///
    /// there are 4 default file system objects:
    ///     - "native::"  : mapping to sys of native file system.
    ///     - "app::"     : mapping to application's executable directory
    ///     - "startup::" : mapping to application's startup directory
    ///     - "media::"   : mapping to startup::media, media::media and app::../media
    ///
    /// \note
    //      - file system name must be end with "::"
    //      - if register same file system multiple times, only the last one is effective.
    ///
    //@{
    GN_EXPORT bool registerFileSystem( const StrA & name, FileSystem * fs );
    GN_EXPORT void UnregisterFileSystem( const StrA & name );
    GN_EXPORT FileSystem * getFileSystem( const StrA & name );
    //@}

    ///
    /// resolve relative path to absolute path.
    ///
    /// Note that resolve will copy original relpath to result, if any of these is true:
    ///     - base and/or relpath are empty
    ///     - base and relpath belongs to different file system.
    ///     - relpath is already an absolute path, like "c:/haha.txt".
    ///
    GN_EXPORT void resolvePath( StrA & result, const StrA & base, const StrA & relpath );

    ///
    /// resolve relative path to absolute path.
    ///
    inline StrA resolvePath( const StrA & base, const StrA & relpath )
    {
        StrA r;
        resolvePath( r, base, relpath );
        return r;
    };

    /// \name FileSystem method wrappers. See FileSystem methods for details.
    //@{

    inline bool exist( const StrA & path )
    {
        StrA sys, child;
        splitPath( path, sys, child );
        return getFileSystem(sys)->exist( child );
    }

    inline bool isDir( const StrA & path )
    {
        StrA sys, child;
        splitPath( path, sys, child );
        return getFileSystem(sys)->isDir( child );
    }

    inline bool isFile( const StrA & path )
    {
        StrA sys, child;
        splitPath( path, sys, child );
        return getFileSystem(sys)->isFile( child );
    }

    inline void toNative( StrA & result, const StrA & path )
    {
        StrA sys, child;
        splitPath( path, sys, child );
        getFileSystem(sys)->toNative( result, child );
    }

    inline StrA toNative( const StrA & path )
    {
        StrA sys, child;
        splitPath( path, sys, child );
        return getFileSystem(sys)->toNative( child );
    }

    inline bool isAbsPath( const StrA & path )
    {
        StrA sys, child;
        splitPath( path, sys, child );
        return getFileSystem(sys)->isAbsPath( child );
    }

    inline File * openFile( const StrA & path, const StrA & mode )
    {
        StrA sys, child;
        splitPath( path, sys, child );
        return getFileSystem(sys)->openFile( child, mode );
    }

    inline std::vector<StrA> & glob(
        std::vector<StrA> & result,
        const StrA & dirName,
        const StrA & pattern,
        bool         recursive,
        bool         useRegex )
    {
        StrA sys, child;
        splitPath( dirName, sys, child );
        return getFileSystem(sys)->glob(
            result,
            child,
            pattern,
            recursive,
            useRegex );
    }

    //@}
}}

// *****************************************************************************
//                           End of filesys.h
// *****************************************************************************
#endif // __GN_CORE_FILESYS_H__
