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
            virtual bool Exist( const StrA & ) = 0;

            ///
            /// if the path points to a directoy?
            ///
            virtual bool IsDir( const StrA & ) = 0;

            ///
            /// if the path points to a file
            ///
            virtual bool IsFile( const StrA & path ) = 0;

            ///
            /// if the path is absolute path
            ///
            virtual bool IsAbsPath( const StrA & path ) = 0;

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
            virtual void ToNativeDiskFilePath( StrA & result, const StrA & path ) = 0;

            ///
            /// Conver path to platform native format.
            ///
            inline StrA ToNativeDiskFilePath( const StrA & path )
            {
                StrA ret;
                ToNativeDiskFilePath( ret, path );
                return ret;
            }

            ///
            /// 查找指定文件
            ///
            /// \param result     返回搜索结果
            /// \param DirName    在什么目录下
            /// \param pattern    搜索什么文件
            /// \param recursive  是否递归搜索子目录
            /// \param useRegex   是否使用正则匹配
            /// \return           返回参数 result
            ///
            virtual DynaArray<StrA> &
            Glob(
                DynaArray<StrA> & result,
                const StrA & DirName,
                const StrA & pattern,
                bool         recursive,
                bool         useRegex ) = 0;

            ///
            /// open file. Note that meaning of mode is identical with standard fopen().
            ///
            virtual File * OpenFile( const StrA & path, const StrA & mode ) = 0;
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
        GN_EXPORT bool RegisterFileSystem( const StrA & name, FileSystem * fs );
        GN_EXPORT void UnregisterFileSystem( const StrA & name );
        GN_EXPORT FileSystem * GetFileSystem( const StrA & name );
        //@}

        /// path related utilites
        //@{

        ///
        /// 1. remove trailing separators
        /// 2. remove redundent separators
        /// 3. convert to unix style
        ///
        void NormalizePathSeparator( GN::StrA & result, const GN::StrA & path );

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
        void ParentPath( StrA &, const StrA & );

        ///
        /// Get the parent path (directory) of the path
        ///
        inline StrA ParentPath( const StrA & path ) { StrA ret; ParentPath(ret,path); return ret; }

        ///
        /// Get extension name of the path, with prefixing dot.
        /// Samples:
        ///     - "a.b.c" -> ".c"
        ///     - "a.c" -> ".c"
        ///     - ".c" -> ".c"
        ///     - "a." -> "."
        ///     - "a" -> ""
        ///
        void ExtName( StrA &, const StrA & );

        ///
        /// Get extension name of the path.
        ///
        inline StrA ExtName( const StrA & path ) { StrA ret; ExtName(ret,path); return ret; }

        ///
        /// get basename of a path (no directory, no extension)
        ///
        void BaseName( StrA & result, const StrA & path );

        ///
        /// get basename of a path
        ///
        inline StrA BaseName( const StrA & path ) { StrA result; BaseName( result, path ); return result; }

        ///
        /// get dirname of a path ( alias of ParentPath() )
        ///
        inline void DirName( StrA & result, const StrA & path ) { ParentPath( result, path ); }

        ///
        /// get dirname of a path ( alias of ParentPath() )
        ///
        inline StrA DirName( const StrA & path ) { return ParentPath( path ); }

        ///
        /// Convert a path to relative path from a base dir.
        ///
        void RelPath( StrA & result, const StrA & path, const StrA & base );

        ///
        /// Convert a path to relative path from and base dir.
        ///
        inline StrA RelPath( const StrA & path, const StrA & base )
        {
            StrA ret;
            RelPath( ret, path, base );
            return ret;
        }

        ///
        /// Join path
        ///
        void JoinPath2(
            StrA & result,
            const StrA & path1 = StrA::EMPTYSTR,
            const StrA & path2 = StrA::EMPTYSTR,
            const StrA & path3 = StrA::EMPTYSTR,
            const StrA & path4 = StrA::EMPTYSTR,
            const StrA & path5 = StrA::EMPTYSTR );

        ///
        /// Join path
        ///
        inline StrA JoinPath(
            const StrA & path1 = StrA::EMPTYSTR,
            const StrA & path2 = StrA::EMPTYSTR,
            const StrA & path3 = StrA::EMPTYSTR,
            const StrA & path4 = StrA::EMPTYSTR,
            const StrA & path5 = StrA::EMPTYSTR )
        {
            StrA ret;
            JoinPath2( ret, path1, path2, path3, path4, path5 );
            return ret;
        }

        ///
        /// split path to 2 parts: fs and local path
        ///
        /// fs and local path are separated by "::"
        ///
        void SplitPath( const StrA & path, StrA & fs, StrA & local );

        ///
        /// get current working directory
        ///
        void GetCurrentDir( StrA & );

        ///
        /// get current working directory
        ///
        inline StrA GetCurrentDir() { StrA s; GetCurrentDir(s); return s; }

        ///
        /// get current driver (for Windows only, return empty string on *nix system)
        ///
        void GetCurrentDrive( StrA & );

        ///
        /// get current driver (for Windows only, return empty string on *nix system)
        ///
        inline StrA GetCurrentDrive() { StrA s; GetCurrentDrive(s); return s; }

        ///
        /// resolve relative path to absolute path.
        ///
        /// Note that resolve will copy original relpath to result, if any of these is true:
        ///     - base and/or relpath are empty
        ///     - base and relpath belongs to different file system.
        ///     - relpath is already an absolute path, like "c:/haha.txt".
        ///
        GN_EXPORT void ResolvePath( StrA & result, const StrA & base, const StrA & relpath );

        ///
        /// resolve relative path to absolute path.
        ///
        inline StrA ResolvePath( const StrA & base, const StrA & relpath )
        {
            StrA r;
            ResolvePath( r, base, relpath );
            return r;
        };

        //@}

        /// \name FileSystem method wrappers. See FileSystem methods for details.
        //@{

        inline bool PathExist( const StrA & path )
        {
            StrA sys, child;
            SplitPath( path, sys, child );
            return GetFileSystem(sys)->Exist( child );
        }

        inline bool IsDir( const StrA & path )
        {
            StrA sys, child;
            SplitPath( path, sys, child );
            return GetFileSystem(sys)->IsDir( child );
        }

        inline bool IsFile( const StrA & path )
        {
            StrA sys, child;
            SplitPath( path, sys, child );
            return GetFileSystem(sys)->IsFile( child );
        }

        inline void ToNativeDiskFilePath( StrA & result, const StrA & path )
        {
            StrA sys, child;
            SplitPath( path, sys, child );
            GetFileSystem(sys)->ToNativeDiskFilePath( result, child );
        }

        inline StrA ToNativeDiskFilePath( const StrA & path )
        {
            StrA sys, child;
            SplitPath( path, sys, child );
            return GetFileSystem(sys)->ToNativeDiskFilePath( child );
        }

        inline bool IsAbsPath( const StrA & path )
        {
            StrA sys, child;
            SplitPath( path, sys, child );
            return GetFileSystem(sys)->IsAbsPath( child );
        }

        inline File * OpenFile( const StrA & path, const StrA & mode )
        {
            StrA sys, child;
            SplitPath( path, sys, child );
            return GetFileSystem(sys)->OpenFile( child, mode );
        }

        inline DynaArray<StrA> & Glob(
            DynaArray<StrA> & result,
            const StrA & dirName,
            const StrA & pattern,
            bool         recursive,
            bool         useRegex )
        {
            StrA sys, child;
            SplitPath( dirName, sys, child );
            return GetFileSystem(sys)->Glob(
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
