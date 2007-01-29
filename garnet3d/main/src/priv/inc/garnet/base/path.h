#ifndef __GN_BASE_PATH_H__
#define __GN_BASE_PATH_H__
// *****************************************************************************
//! \file    base/path.h
//! \brief   platform independent path classes
//! \author  chen@@CHENLI-HOMEPC (2007.1.29)
// *****************************************************************************

namespace GN
{
    ///
    /// namespace for path functions
    ///
    /// \Note some special path prefixes are supported:
    ///     - "app:"     : refer to application's directory
    ///     - "pwd:"     : refer to current working directory
    ///     - "startup:" : refer to application startup directory
    ///
    namespace path
    {
        ///
        /// 路径是否存在
        ///
        bool exist( const StrA & );

        ///
        /// if the path points to a directoy?
        ///
        bool isDir( const StrA & );

        ///
        /// if the path points to a file
        ///
        inline bool isFile( const StrA & path ) { return exist(path) && !isDir(path); }

        ///
        /// Conver path to platform native format. This function will do:
        ///   - Normalize path separators
        ///     - Convert all path separators to platform specific format.
        ///     - Remove redundant path separators, such as "c:\\path\" will be
        ///       convert to "c:\path".
        ///   - Resolve special path prefixes. Currently, 3 prefixes are supported:
        ///     - "app:"      : executable directory
        ///     - "startup:"  : startup directory
        ///     - "pwd:"      : current working directory
        ///   - Resolve embbed environment variable, like this:
        ///         "${windir}/system32" -> "c:\\windows\\system32"
        ///     - Note this feature is unimplemented yet.
        ///
        void toNative( StrA & result, const StrA & path );

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
        /// Return true for path like: "/....", "aaa:..."
        ///
        bool isAbsPath( const StrA & path );

        ///
        /// Return true for relative (root-less) path
        ///
        inline bool isRelPath( const StrA & path ) { return !isAbsPath(path); }

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
        ///     - "c:a/b" -> "c:/a"
        ///     - "c:a" -> "c:/"
        ///     - "c:" -> "c:/"
        ///
        void getParent( StrA &, const StrA & );

        ///
        /// Get the parent path (directory) of the path
        ///
        inline StrA getParent( const StrA & path ) { StrA ret; getParent(ret,path); return ret; }

        ///
        /// Get extension name of the path, with prefixing dot.
        /// Samples:
        ///     - "a.b.c" -> ".c"
        ///     - "a.c" -> ".c"
        ///     - ".c" -> ".c"
        ///     - "a." -> "."
        ///     - "a" -> ""
        ///
        void getExt( StrA &, const StrA & );

        ///
        /// Get extension name of the path.
        ///
        inline StrA getExt( const StrA & path ) { StrA ret; getExt(ret,path); return ret; }

        ///
        /// get basename of a path
        ///
        void baseName( StrA & result, const StrA & path );

        ///
        /// get basename of a path
        ///
        inline StrA baseName( const StrA & path ) { StrA result; baseName( result, path ); return result; }

        ///
        /// get dirname of a path ( alias of getParent() )
        ///
        inline void dirName( StrA & result, const StrA & path ) { getParent( result, path ); }

        ///
        /// get dirname of a path ( alias of getParent() )
        ///
        inline StrA dirName( const StrA & path ) { return getParent( path ); }

        ///
        /// Join path
        ///
        void joinTo(
            StrA & result,
            const StrA & path1 = StrA::EMPTYSTR,
            const StrA & path2 = StrA::EMPTYSTR,
            const StrA & path3 = StrA::EMPTYSTR,
            const StrA & path4 = StrA::EMPTYSTR,
            const StrA & path5 = StrA::EMPTYSTR );

        ///
        /// Join path
        ///
        inline StrA join(
            const StrA & path1 = StrA::EMPTYSTR,
            const StrA & path2 = StrA::EMPTYSTR,
            const StrA & path3 = StrA::EMPTYSTR,
            const StrA & path4 = StrA::EMPTYSTR,
            const StrA & path5 = StrA::EMPTYSTR )
        {
            StrA ret;
            joinTo( ret, path1, path2, path3, path4, path5 );
            return ret;
        }

        ///
        /// Resolve relative path to absolute path.
        ///
        /// This function will fail, if:
        /// - input path does not exist;
        /// - Read or search permission was denied for input path;
        /// - A loop exists in symbolic links encountered during resolve;
        ///
        /// \note This function will also convert input path to native format.
        ///
        bool resolve( StrA & result, const StrA & path );

        ///
        /// Resolve relative path to absolute path.
        ///
        /// \return Empty string, if failed.
        ///
        inline StrA resolve( const StrA & path )
        {
            StrA ret;
            if( resolve( ret, path ) ) return ret;
            return StrA::EMPTYSTR;
        }

        ///
        /// Convert a path to relative path from and base dir.
        ///
        void getRelative( StrA & result, const StrA & path, const StrA & base );

        ///
        /// Convert a path to relative path from and base dir.
        ///
        inline StrA getRelative( const StrA & path, const StrA & base )
        {
            StrA ret;
            getRelative( ret, path, base );
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
        std::vector<StrA> &
        glob(
            std::vector<StrA> & result,
            const StrA & dirName,
            const StrA & pattern,
            bool         recursive,
            bool         useRegex );
    }
}

// *****************************************************************************
//                           End of path.h
// *****************************************************************************
#endif // __GN_BASE_PATH_H__
