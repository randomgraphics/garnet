#ifndef __GN_CORE_FILESYS_H__
#define __GN_CORE_FILESYS_H__
// *****************************************************************************
/// \file
/// \brief   platform independent file system
/// \author  chen@@CHENLI-HOMEPC (2007.1.29)
// *****************************************************************************

namespace GN
{
    ///
    /// sub namespace file system
    ///
    namespace fs
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
            /// Conver to disk file path in platform native format.
            /// If the path has no corresponding disk file. Return empty path.
            /// It does:
            ///   - normalize path separator:
            ///     - convert path separators to platform specific format.
            ///     - Remove redundant path separators, such as "c:\\path\" will be
            ///       convert to "c:\path".
            ///   - covert to full path name
            ///   - Resolve embbed environment variable, like this:
            ///     "${windir}/system32" -> "c:\\windows\\system32"
            ///
            virtual void toNativeDiskFilePath( StrA & result, const StrA & path ) = 0;

            ///
            /// Conver path to platform native format.
            ///
            inline StrA toNativeDiskFilePath( const StrA & path )
            {
                StrA ret;
                toNativeDiskFilePath( ret, path );
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
            virtual DynaArray<StrA> &
            glob(
                DynaArray<StrA> & result,
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
        /// there are several default file system objects:
        ///     - "native::"  : mapping to sys of native file system.
        ///     - "app::"     : mapping to application's executable directory
        ///     - "startup::" : mapping to application's startup directory
        ///     - "media::"   : mapping to startup::media, media::media and app::../media
        ///     - "font::"    : mappint to media::/font, and ${WINDIR}/fonts on Windows platform.
        ///
        /// \note
        //      - file system name must be end with "::"
        //      - if register same file system multiple times, only the last one is effective.
        ///
        //@{
        GN_API  bool registerFileSystem( const StrA & name, FileSystem * fs );
        GN_API  void UnregisterFileSystem( const StrA & name );
        GN_API  FileSystem * getFileSystem( const StrA & name );
        //@}

        /// path related utilites
        //@{

        ///
        /// 1. remove trailing separators
        /// 2. remove redundent separators
        /// 3. convert to unix style
        ///
        GN_API void normalizePathSeparator( GN::StrA & result, const GN::StrA & path );

        ///
        /// Get the parent path (directory) of the path.
        /// Samples:
        ///     - "a/b" -> "a"
        ///     - "a/b/" -> "a"
        ///     - "/a" -> "/"
        ///     - "/a/" -> "/"
        ///     - "a" -> ""
        ///     - "a/" -> ""
        ///     - "/" -> "/"
        ///     - "" -> ""
        ///     - "root::/a/b" -> "root::/a"
        ///     - "root::/" -> "root::/"
        ///     - "c:a/b" -> "c:a"
        ///     - "c:a" -> "c:"
        ///     - "c:" -> "c:"
        ///
        GN_API void parentPath( StrA &, const StrA & );

        ///
        /// Get the parent path (directory) of the path
        ///
        inline StrA parentPath( const StrA & path ) { StrA ret; parentPath(ret,path); return ret; }

        ///
        /// Get extension name of the path, with prefixing dot.
        /// Samples:
        ///     - "a.b.c" -> ".c"
        ///     - "a.c" -> ".c"
        ///     - ".c" -> ".c"
        ///     - "a." -> "."
        ///     - "a" -> ""
        ///
        GN_API void extName( StrA &, const StrA & );

        ///
        /// Get extension name of the path.
        ///
        inline StrA extName( const StrA & path ) { StrA ret; extName(ret,path); return ret; }

        ///
        /// get basename of a path (no directory, no extension)
        ///
        GN_API void baseName( StrA & result, const StrA & path );

        ///
        /// get basename of a path
        ///
        inline StrA baseName( const StrA & path ) { StrA result; baseName( result, path ); return result; }

        ///
        /// get dirname of a path ( alias of parentPath() )
        ///
        inline void dirName( StrA & result, const StrA & path ) { parentPath( result, path ); }

        ///
        /// get dirname of a path ( alias of parentPath() )
        ///
        inline StrA dirName( const StrA & path ) { return parentPath( path ); }

        ///
        /// Convert a path to relative path from a base dir.
        ///
        GN_API void relPath( StrA & result, const StrA & path, const StrA & base );

        ///
        /// Convert a path to relative path from and base dir.
        ///
        inline StrA relPath( const StrA & path, const StrA & base )
        {
            StrA ret;
            relPath( ret, path, base );
            return ret;
        }

        ///
        /// Join path
        ///
        GN_API void joinPath2(
            StrA & result,
            const StrA & path1 = StrA::EMPTYSTR(),
            const StrA & path2 = StrA::EMPTYSTR(),
            const StrA & path3 = StrA::EMPTYSTR(),
            const StrA & path4 = StrA::EMPTYSTR(),
            const StrA & path5 = StrA::EMPTYSTR() );

        ///
        /// Join path
        ///
        inline StrA joinPath(
            const StrA & path1 = StrA::EMPTYSTR(),
            const StrA & path2 = StrA::EMPTYSTR(),
            const StrA & path3 = StrA::EMPTYSTR(),
            const StrA & path4 = StrA::EMPTYSTR(),
            const StrA & path5 = StrA::EMPTYSTR() )
        {
            StrA ret;
            joinPath2( ret, path1, path2, path3, path4, path5 );
            return ret;
        }

        ///
        /// split path to 2 parts: fs and local path
        ///
        /// fs and local path are separated by "::"
        ///
        GN_API void splitPath( const StrA & path, StrA & fs, StrA & local );

        ///
        /// get current working directory
        ///
        GN_API void getCurrentDir( StrA & );

        ///
        /// get current working directory
        ///
        inline StrA getCurrentDir() { StrA s; getCurrentDir(s); return s; }

        ///
        /// get current driver (for Windows only, return empty string on *nix system)
        ///
        GN_API void getCurrentDrive( StrA & );

        ///
        /// get current driver (for Windows only, return empty string on *nix system)
        ///
        inline StrA getCurrentDrive() { StrA s; getCurrentDrive(s); return s; }

        ///
        /// resolve relative path to absolute path.
        ///
        /// Note that resolve will copy original relpath to result, if any of these is true:
        ///     - base and/or relpath are empty
        ///     - base and relpath belongs to different file system.
        ///     - relpath is already an absolute path, like "c:/haha.txt".
        ///
        GN_API void resolvePath( StrA & result, const StrA & base, const StrA & relpath );

        ///
        /// resolve relative path to absolute path.
        ///
        inline StrA resolvePath( const StrA & base, const StrA & relpath )
        {
            StrA r;
            resolvePath( r, base, relpath );
            return r;
        };

        //@}

        /// \name FileSystem method wrappers. See FileSystem methods for details.
        //@{

        inline bool pathExist( const StrA & path )
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

        inline void toNativeDiskFilePath( StrA & result, const StrA & path )
        {
            StrA sys, child;
            splitPath( path, sys, child );
            getFileSystem(sys)->toNativeDiskFilePath( result, child );
        }

        inline StrA toNativeDiskFilePath( const StrA & path )
        {
            StrA sys, child;
            splitPath( path, sys, child );
            return getFileSystem(sys)->toNativeDiskFilePath( child );
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

        inline DynaArray<StrA> & glob(
            DynaArray<StrA> & result,
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
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_CORE_FILESYS_H__
