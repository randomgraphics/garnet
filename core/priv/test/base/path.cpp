#include "../testCommon.h"

#if GN_WINNT
#define PSC '\\'
#define PSS "\\"
#else
#define PSC '/'
#define PSS "/"
#endif


class PathTest : public CxxTest::TestSuite
{
public:

    void testToNative()
    {
#if GN_WINNT
        TS_ASSERT_EQUALS( "\\", GN::path::toNative("/") );
        TS_ASSERT_EQUALS( "\\a\\b", GN::path::toNative("/a//b/") );
#else
        TS_ASSERT_EQUALS( "/", GN::path::toNative("\\") );
        TS_ASSERT_EQUALS( "/a/b", GN::path::toNative("\\a\\\\b\\") );
#endif
    }

    void testJoin()
    {
        TS_ASSERT_EQUALS( PSS"a"PSS"b"PSS"c", GN::path::join( "\\a\\", "\\b\\", "\\c\\" ) );
        TS_ASSERT_EQUALS( PSS"a"PSS"b"PSS"c", GN::path::join( "/a/", "/b/", "/c/" ) );
    }

    void testExist()
    {
        TS_ASSERT( GN::path::exist("/") );
        TS_ASSERT( !GN::path::exist("haha,heihei,hoho,huhu,mama,papa") );
        TS_ASSERT( GN::path::exist("SConstruct") );

        TS_ASSERT( GN::path::exist("startup:\\SConstruct") );
        TS_ASSERT( GN::path::exist("startup:/SConstruct") );
        TS_ASSERT( GN::path::exist("startup:SConstruct") );

        /*TS_ASSERT( GN::path::exist("app:GnTest.exe") );
        TS_ASSERT( GN::path::exist("app:/GnTest.exe") );
        TS_ASSERT( GN::path::exist("app:\\GnTest.exe") );*/

        TS_ASSERT( GN::path::exist("pwd:") );
        TS_ASSERT( GN::path::exist("pwd:/") );
        TS_ASSERT( GN::path::exist("pwd:\\") );
    }

    void testIsDir()
    {
        TS_ASSERT( GN::path::isDir("/") );

        TS_ASSERT( GN::path::isDir("startup:") );
        TS_ASSERT( GN::path::isDir("startup:/") );
        TS_ASSERT( GN::path::isDir("startup:\\") );

        /*TS_ASSERT( GN::path::isDir("app:") );
        TS_ASSERT( GN::path::isDir("app:/") );
        TS_ASSERT( GN::path::isDir("app:\\") );*/

        TS_ASSERT( GN::path::isDir("pwd:") );
        TS_ASSERT( GN::path::isDir("pwd:/") );
        TS_ASSERT( GN::path::isDir("pwd:\\") );

        TS_ASSERT( !GN::path::isDir("haha,heihei,hoho,huhu,mama,papa") );
    }
};
