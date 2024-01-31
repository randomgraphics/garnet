#include "pch.h"

// *****************************************************************************
// Local types, functions and variables
// *****************************************************************************

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::normalizePathSeparator(std::string & result, const std::string & path) {
    std::string tmp;

    // remove redundent separators, convert to unix style as well
    tmp.setCaps(path.size());
    for (size_t i = 0; i < path.size(); ++i) {
        char ch = path[i];

        if ('\\' == ch) ch = '/';

        if ('/' == ch && !tmp.empty() && '/' == tmp.last() && 1 != i) {
            // ignore redundant path separator
            continue;
        }

        tmp.append(ch);
    }

    // remove trailing separators.
    if (!tmp.empty()) {
        tmp.trimRight('/');
        if (tmp.empty()) tmp.append('/');
    }

    // make sure no pending path separator
    GN_ASSERT(tmp.empty() || 1 == tmp.size() || '/' != tmp.last());

    // success
    result = tmp;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::parentPath(std::string & result, const std::string & path) {
    struct Local {
        static bool isPathSeparator(char ch) { return '/' == ch; }
    };
    std::string root, child;
    splitPath(path, root, child);
    child.trimRightUntil(Local::isPathSeparator);
    if (child.size() > 1) child.popback();
    result = root + child;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::extName(std::string & result, const std::string & path) {
    std::string tmp;

    normalizePathSeparator(tmp, path);

    size_t n = tmp.size();

    while (n > 0) {
        --n;
        if ('.' == tmp[n]) {
            tmp.subString(result, n, 0);
            return;
        }
    }

    // no extension found.
    result.clear();
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::baseName(std::string & result, const std::string & path) {
    std::string tmp;
    normalizePathSeparator(tmp, path);

    std::string dir, ext;

    dirName(dir, tmp);
    extName(ext, tmp);

    size_t n1 = (dir.size() < tmp.size() && '/' == tmp[dir.size()]) ? dir.size() + 1 : dir.size();
    size_t n2 = tmp.size() - n1 - ext.size();

    result.assign(tmp.data() + n1, n2);
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::relPath(std::string & result, const std::string & path, const std::string & base) {
    GN_GUARD;

    struct Local {
        std::string              input;
        DynaArray<char>   buf;
        DynaArray<char *> parts;

        void split(const std::string & path) {
            parts.clear();
            normalizePathSeparator(input, path);
            buf.resize(input.size() + 1);
            memcpy(buf.data(), input.data(), input.size() + 1);
            parts.append(buf.data());
            for (size_t i = 0; i < buf.size() - 1; ++i) {
                if ('/' == buf[i]) {
                    buf[i] = 0;
                    parts.append(buf.data() + i + 1);
                }
            }
        }
    };

    // shortcut for empty input strings.
    if (path.empty() || base.empty()) {
        normalizePathSeparator(result, path);
        return;
    }

    // split input path into components by PATH_SEPARATOR.
    Local p, b;
    p.split(path);
    b.split(base);

    // find the commen prefix between path and base.
    size_t n = math::getmin(p.parts.size(), b.parts.size());
    size_t i;
    for (i = 0; i < n; ++i) {
        const char * s1 = p.parts[i];
        const char * s2 = b.parts[i];
#if GN_MSWIN
        if (0 != str::compareI(s1, s2)) break;
#else
        if (0 != str::compare(s1, s2)) break;
#endif
    }
    // here, "i" should point to the first different part.

    // compose result path
    result.clear();
    if (i > 0)
        for (size_t j = i; j < b.parts.size(); ++j) {
            result.append("..");
            result.append('/');
        }
    for (size_t j = i; j < p.parts.size(); ++j) {
        result.append(p.parts[j]);
        result.append('/');
    }
    result.trimRight('/');

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::joinPath2(std::string & result, const std::string & path1, const std::string & path2, const std::string & path3, const std::string & path4, const std::string & path5) {
    std::string tmp;

    const std::string * parts[] = {&path1, &path2, &path3, &path4, &path5};

    size_t n = sizeof(parts) / sizeof(parts[0]);

    size_t i = 0;

    // ignore leading empty parts
    while (i < n && parts[i]->empty()) ++i;

    if (i < n) tmp = *parts[i], ++i;

    for (; i < n; ++i) {
        const std::string & p = *parts[i];
        if (p.empty()) continue; // ignore empty parts
        tmp.append('/');
        tmp.append(p);
    }

    normalizePathSeparator(result, tmp);
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::splitPath(const std::string & path, std::string & root, std::string & child) {
    root.clear();
    child.clear();

    if (path.empty()) return;

    std::string tmpChild;

    size_t i       = 0;
    size_t n       = path.size() - 1;
    bool   hasroot = false;
    while (i < n) {
        char ch1 = path[i];
        char ch2 = path[i + 1];

        if ('/' == ch1) {
            // found "/" first, no root
            break;
        } else if (':' == ch1 && ':' == ch2) {
            // found!
            path.subString(root, 0, i + 2);
            path.subString(tmpChild, i + 2, 0);
            hasroot = true;
            break;
        }

        ++i;
    }

    if (hasroot) {
        normalizePathSeparator(child, tmpChild);
    } else {
        normalizePathSeparator(child, path);
    }
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::getCurrentDir(std::string & result) {
#if GN_XBOX2 || GN_XBOX3
    result = "game:";
#elif GN_WINPC
    char buf[MAX_PATH + 1];
    char full[MAX_PATH + 1];
    GetCurrentDirectoryA(MAX_PATH, buf);
    GetFullPathNameA(buf, MAX_PATH, full, 0);
    result = full;
    result.trimRight('/');
#elif GN_POSIX
    char buf[FILENAME_MAX + 1];
    if (NULL == getcwd(buf, FILENAME_MAX)) {
        static Logger * sLogger = getLogger("GN.base.path");
        GN_ERROR(sLogger)("getcwd() failed: fail to get current directory.");
        result = "";
    } else {
        buf[FILENAME_MAX] = 0;
        result            = buf;
        result.trimRight('/');
    }
#else
    #error Unknown platform!
#endif
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::getCurrentDrive(std::string & result) {
#if GN_XBOX2 || GN_XBOX3
    result.clear();
#elif GN_WINPC
    char buf[MAX_PATH + 1];
    char full[MAX_PATH + 1];
    GetCurrentDirectoryA(MAX_PATH, buf);
    GetFullPathNameA(buf, MAX_PATH, full, 0);
    GN_ASSERT(':' == full[1]);
    result.assign(full, 2);
#elif GN_POSIX
    result.clear();
#else
    #error Unknown platform!
#endif
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::fs::resolvePath(std::string & result, const std::string & base, const std::string & relpath) {
    // shortcut for empty path
    if (base.empty() || relpath.empty()) {
        result = relpath;
        return;
    }

    std::string basefs, basec, relfs, relc;
    splitPath(base, basefs, basec);
    splitPath(relpath, relfs, relc);

    if (!relfs.empty() && basefs != relfs) {
        result = relpath;
        return;
    }

    if (getFileSystem(relfs)->isAbsPath(relc)) {
        result = relpath;
        return;
    }

    joinPath2(result, base, relc);
}
