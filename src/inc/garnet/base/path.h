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
GN_API void normalizePathSeparator(GN::StrA & result, const GN::StrA & path);

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
GN_API void parentPath(StrA &, const StrA &);

///
/// Get the parent path (directory) of the path
///
inline StrA parentPath(const StrA & path) {
    StrA ret;
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
GN_API void extName(StrA &, const StrA &);

///
/// Get extension name of the path.
///
inline StrA extName(const StrA & path) {
    StrA ret;
    extName(ret, path);
    return ret;
}

///
/// get basename of a path (no directory, no extension)
///
GN_API void baseName(StrA & result, const StrA & path);

///
/// get basename of a path
///
inline StrA baseName(const StrA & path) {
    StrA result;
    baseName(result, path);
    return result;
}

///
/// get dirname of a path ( alias of parentPath() )
///
inline void dirName(StrA & result, const StrA & path) { parentPath(result, path); }

///
/// get dirname of a path ( alias of parentPath() )
///
inline StrA dirName(const StrA & path) { return parentPath(path); }

///
/// Convert a path to relative path from a base dir.
///
GN_API void relPath(StrA & result, const StrA & path, const StrA & base);

///
/// Convert a path to relative path from and base dir.
///
inline StrA relPath(const StrA & path, const StrA & base) {
    StrA ret;
    relPath(ret, path, base);
    return ret;
}

///
/// Join path
///
GN_API void joinPath2(StrA & result, const StrA & path1 = StrA::EMPTYSTR(), const StrA & path2 = StrA::EMPTYSTR(), const StrA & path3 = StrA::EMPTYSTR(),
                      const StrA & path4 = StrA::EMPTYSTR(), const StrA & path5 = StrA::EMPTYSTR());

///
/// Join path
///
inline StrA joinPath(const StrA & path1 = StrA::EMPTYSTR(), const StrA & path2 = StrA::EMPTYSTR(), const StrA & path3 = StrA::EMPTYSTR(),
                     const StrA & path4 = StrA::EMPTYSTR(), const StrA & path5 = StrA::EMPTYSTR()) {
    StrA ret;
    joinPath2(ret, path1, path2, path3, path4, path5);
    return ret;
}

///
/// split path to 2 parts: fs and local path
///
/// fs and local path are separated by "::"
///
GN_API void splitPath(const StrA & path, StrA & fs, StrA & local);

///
/// get current working directory
///
GN_API void getCurrentDir(StrA &);

///
/// get current working directory
///
inline StrA getCurrentDir() {
    StrA s;
    getCurrentDir(s);
    return s;
}

///
/// get current driver (for Windows only, return empty string on *nix system)
///
GN_API void getCurrentDrive(StrA &);

///
/// get current driver (for Windows only, return empty string on *nix system)
///
inline StrA getCurrentDrive() {
    StrA s;
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
GN_API void resolvePath(StrA & result, const StrA & base, const StrA & relpath);

///
/// resolve relative path to absolute path.
///
inline StrA resolvePath(const StrA & base, const StrA & relpath) {
    StrA r;
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
