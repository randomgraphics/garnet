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
