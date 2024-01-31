#ifndef __GN_BASE_PATH_H__
#define __GN_BASE_PATH_H__
// *****************************************************************************
/// \file
/// \brief   common platform independent path utilities
/// \author  chenlee (2018.05.07)
// *****************************************************************************

namespace GN {
namespace fs {

/// path related utilites
//@{

///
/// 1. remove trailing separators
/// 2. remove redundent separators
/// 3. convert to unix style
///
GN_API void normalizePathSeparator(std::string & result, const std::string & path);

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
GN_API void parentPath(std::string &, const std::string &);

///
/// Get the parent path (directory) of the path
///
inline std::string parentPath(const std::string & path) {
    std::string ret;
    parentPath(ret, path);
    return ret;
}

///
/// Get extension name of the path, with prefixing dot.
/// Samples:
///     - "a.b.c" -> ".c"
///     - "a.c" -> ".c"
///     - ".c" -> ".c"
///     - "a." -> "."
///     - "a" -> ""
///
GN_API void extName(std::string &, const std::string &);

///
/// Get extension name of the path.
///
inline std::string extName(const std::string & path) {
    std::string ret;
    extName(ret, path);
    return ret;
}

///
/// get basename of a path (no directory, no extension)
///
GN_API void baseName(std::string & result, const std::string & path);

///
/// get basename of a path
///
inline std::string baseName(const std::string & path) {
    std::string result;
    baseName(result, path);
    return result;
}

///
/// get dirname of a path ( alias of parentPath() )
///
inline void dirName(std::string & result, const std::string & path) { parentPath(result, path); }

///
/// get dirname of a path ( alias of parentPath() )
///
inline std::string dirName(const std::string & path) { return parentPath(path); }

///
/// Convert a path to relative path from a base dir.
///
GN_API void relPath(std::string & result, const std::string & path, const std::string & base);

///
/// Convert a path to relative path from and base dir.
///
inline std::string relPath(const std::string & path, const std::string & base) {
    std::string ret;
    relPath(ret, path, base);
    return ret;
}

///
/// Join path
///
GN_API void joinPath2(std::string & result, const std::string & path1 = ""s, const std::string & path2 = ""s, const std::string & path3 = ""s,
                      const std::string & path4 = ""s, const std::string & path5 = ""s);

///
/// Join path
///
inline std::string joinPath(const std::string & path1 = ""s, const std::string & path2 = ""s, const std::string & path3 = ""s,
                     const std::string & path4 = ""s, const std::string & path5 = ""s) {
    std::string ret;
    joinPath2(ret, path1, path2, path3, path4, path5);
    return ret;
}

///
/// split path to 2 parts: fs and local path
///
/// fs and local path are separated by "::"
///
GN_API void splitPath(const std::string & path, std::string & fs, std::string & local);

///
/// get current working directory
///
GN_API void getCurrentDir(std::string &);

///
/// get current working directory
///
inline std::string getCurrentDir() {
    std::string s;
    getCurrentDir(s);
    return s;
}

///
/// get current driver (for Windows only, return empty string on *nix system)
///
GN_API void getCurrentDrive(std::string &);

///
/// get current driver (for Windows only, return empty string on *nix system)
///
inline std::string getCurrentDrive() {
    std::string s;
    getCurrentDrive(s);
    return s;
}

///
/// resolve relative path to absolute path.
///
/// Note that resolve will copy original relpath to result, if any of these is true:
///     - base and/or relpath are empty
///     - base and relpath belongs to different file system.
///     - relpath is already an absolute path, like "c:/haha.txt".
///
GN_API void resolvePath(std::string & result, const std::string & base, const std::string & relpath);

///
/// resolve relative path to absolute path.
///
inline std::string resolvePath(const std::string & base, const std::string & relpath) {
    std::string r;
    resolvePath(r, base, relpath);
    return r;
}

//@}
} // namespace fs
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif
