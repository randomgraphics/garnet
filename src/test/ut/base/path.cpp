#include "../testCommon.h"

#if GN_MSWIN
    #define PSC '\\'
    #define PSS "\\"
#else
    #define PSC '/'
    #define PSS "/"
#endif

#if GN_MSWIN || GN_CYGWIN
    #define APPEXT ".exe"
#else
    #define APPEXT ""
#endif

class PathTest : public CxxTest::TestSuite {
public:
    void testToNative() {
        using namespace GN;
        using namespace GN::fs;

        StrA pwd = toNativeDiskFilePath(getCurrentDir());
        StrA d   = getCurrentDrive();

        TS_ASSERT_EQUALS(d + PSS, toNativeDiskFilePath("/"));
        TS_ASSERT_EQUALS(d + PSS, toNativeDiskFilePath("\\"));

        TS_ASSERT_EQUALS(d + PSS "a" PSS "b", toNativeDiskFilePath("/a//b/"));
        TS_ASSERT_EQUALS(d + PSS "a" PSS "b", toNativeDiskFilePath("\\a\\\\b\\"));

#if GN_MSWIN
        TS_ASSERT_EQUALS("A:" PSS "b", toNativeDiskFilePath("a:b"));
        TS_ASSERT_EQUALS("A:" PSS, toNativeDiskFilePath("a:"));
        TS_ASSERT_EQUALS("A:" PSS, toNativeDiskFilePath("a:/"));
        TS_ASSERT_EQUALS("A:" PSS, toNativeDiskFilePath("a:\\"));
#elif GN_POSIX
        TS_ASSERT_EQUALS(pwd + PSS "a:b", toNativeDiskFilePath("a:b"));
        TS_ASSERT_EQUALS(pwd + PSS "a:", toNativeDiskFilePath("a:"));
        TS_ASSERT_EQUALS(pwd + PSS "a:", toNativeDiskFilePath("a:/"));
        TS_ASSERT_EQUALS(pwd + PSS "a:", toNativeDiskFilePath("a:\\"));
#endif

        TS_ASSERT_EQUALS(pwd + PSS "a" PSS ":", toNativeDiskFilePath("a/:"));
        TS_ASSERT_EQUALS(pwd + PSS "a" PSS ":", toNativeDiskFilePath("a\\:"));
    }

    void testJoin() {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS("", joinPath("", ""));
        TS_ASSERT_EQUALS("a", joinPath("", "a"));
        TS_ASSERT_EQUALS("a", joinPath("a", ""));
        TS_ASSERT_EQUALS("a", joinPath("", "a", ""));
        TS_ASSERT_EQUALS("a/b", joinPath("a", "b"));
        TS_ASSERT_EQUALS("a/b", joinPath("", "a", "", "b", ""));
        TS_ASSERT_EQUALS("/a/b/c", joinPath("\\a\\", "\\b\\", "\\c\\"));
        TS_ASSERT_EQUALS("/a/b/c", joinPath("/a/", "/b/", "/c/"));
    }

    void testPrefix() {
        using namespace GN;
        using namespace GN::fs;

        StrA s[2] = {
            toNativeDiskFilePath("app::"),
            toNativeDiskFilePath("startup::"),
        };

        GN_INFO(sLogger)("appDir = %s", s[0].rawptr());
        GN_INFO(sLogger)("startup = %s", s[1].rawptr());

        for (size_t i = 0; i < 2; ++i) {
            TS_ASSERT(!s[i].empty());
            if (s[i].size() > 1) {
                TS_ASSERT_DIFFERS(PSC, s[i].last());
            } else {
                TS_ASSERT_EQUALS(PSS, s[i]);
            }
        }
    }

    void testPathExist() {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT(pathExist("/"));
        TS_ASSERT(!pathExist("haha,heihei,hoho,huhu,mama,papa"));
        TS_ASSERT(pathExist("CMakeLists.txt"));

        TS_ASSERT(pathExist("startup::\\CMakeLists.txt"));
        TS_ASSERT(pathExist("startup::/CMakeLists.txt"));
        TS_ASSERT(pathExist("startup::CMakeLists.txt"));

        TS_ASSERT(pathExist("app::GNtest-unit-tests" APPEXT));
        TS_ASSERT(pathExist("app::/GNtest-unit-tests" APPEXT));
        TS_ASSERT(pathExist("app::\\GNtest-unit-tests" APPEXT));
    }

    void testIsDir() {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT(isDir("/"));

        TS_ASSERT(isDir("startup::"));
        TS_ASSERT(isDir("startup::/"));
        TS_ASSERT(isDir("startup::\\"));

        TS_ASSERT(isDir("app::"));
        TS_ASSERT(isDir("app::/"));
        TS_ASSERT(isDir("app::\\"));

        TS_ASSERT(!isDir("haha,heihei,hoho,huhu,mama,papa"));
    }

    void testIsFile() {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT(isFile("startup::\\CMakeLists.txt"));
        TS_ASSERT(isFile("app::GNtest-unit-tests" APPEXT));
        TS_ASSERT(!isFile("startup::"));
        TS_ASSERT(!isFile("app::"));
        TS_ASSERT(!isFile("haha,heihei,hoho,huhu,mama,papa"));
    }

    void testGetParent() {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS("a", parentPath("a/b"));
        TS_ASSERT_EQUALS("a", parentPath("a/b/"));
        TS_ASSERT_EQUALS("/", parentPath("/a"));
        TS_ASSERT_EQUALS("/", parentPath("/a/"));
        TS_ASSERT_EQUALS("", parentPath("a"));
        TS_ASSERT_EQUALS("", parentPath("a/"));
        TS_ASSERT_EQUALS("/", parentPath("/"));
        TS_ASSERT_EQUALS("", parentPath(""));
        TS_ASSERT_EQUALS("startup::a", parentPath("startup::a/b"));
        TS_ASSERT_EQUALS("startup::/", parentPath("startup::/a"));
        TS_ASSERT_EQUALS("startup::", parentPath("startup::a"));
        TS_ASSERT_EQUALS("startup::/", parentPath("startup::\\"));
        TS_ASSERT_EQUALS("startup::", parentPath("startup::"));
    }

    void testGetExt() {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS(".c", extName("a.b.c"));
        TS_ASSERT_EQUALS(".c", extName("a.c"));
        TS_ASSERT_EQUALS(".c", extName(".c"));
        TS_ASSERT_EQUALS(".", extName("a."));
        TS_ASSERT_EQUALS("", extName("a"));
    }

    void testToRelative() {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS("..", relPath("a/b/c", "a\\b\\c\\d"));
        TS_ASSERT_EQUALS("", relPath("a/b/c", "a\\b\\c"));
        TS_ASSERT_EQUALS("c", relPath("a/b/c", "a\\b"));
        TS_ASSERT_EQUALS("a/b/c", relPath("a/b/c", ""));
        TS_ASSERT_EQUALS("a/b/c", relPath("a/b/c", "d/e"));
        TS_ASSERT_EQUALS("c", relPath("c:/a/b/c", "c:/a/b"));
        TS_ASSERT_EQUALS("c:/a/b", relPath("c:/a/b", "d:/a"));
    }

    void testBaseName() {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS("a.b", baseName("a.b.c"));
        TS_ASSERT_EQUALS("a.b", baseName("media::a.b.c"));
        TS_ASSERT_EQUALS("a.b", baseName("\\a.b.c"));
        TS_ASSERT_EQUALS("a.b", baseName("c:\\a.b.c"));
    }
};
