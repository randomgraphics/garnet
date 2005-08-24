#include "../testCommon.h"

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
