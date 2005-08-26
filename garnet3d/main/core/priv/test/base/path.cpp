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

        TS_ASSERT_EQUALS( "a:\\b", GN::path::toNative("a:b") );
        TS_ASSERT_EQUALS( "a:", GN::path::toNative("a:") );
        TS_ASSERT_EQUALS( "a:", GN::path::toNative("a:/") );
        TS_ASSERT_EQUALS( "a:", GN::path::toNative("a:\\") );
        TS_ASSERT_EQUALS( "a\\:", GN::path::toNative("a/:") );
        TS_ASSERT_EQUALS( "a\\:", GN::path::toNative("a\\:") );

#else
        TS_ASSERT_EQUALS( "/", GN::path::toNative("\\") );
        TS_ASSERT_EQUALS( "/a/b", GN::path::toNative("\\a\\\\b\\") );

        TS_ASSERT_EQUALS( "a:/b", GN::path::toNative("a:b") );
        TS_ASSERT_EQUALS( "a:", GN::path::toNative("a:") );
        TS_ASSERT_EQUALS( "a:", GN::path::toNative("a:/") );
        TS_ASSERT_EQUALS( "a:", GN::path::toNative("a:\\") );
        TS_ASSERT_EQUALS( "a/:", GN::path::toNative("a/:") );
        TS_ASSERT_EQUALS( "a/:", GN::path::toNative("a\\:") );
#endif
    }

    void testJoin()
    {
        TS_ASSERT_EQUALS( PSS"a"PSS"b"PSS"c", GN::path::join( "\\a\\", "\\b\\", "\\c\\" ) );
        TS_ASSERT_EQUALS( PSS"a"PSS"b"PSS"c", GN::path::join( "/a/", "/b/", "/c/" ) );
    }

    void testPrefix()
    {
        GN::StrA s[3] ={
            GN::path::toNative("app:"),
            GN::path::toNative("startup:"),
            GN::path::toNative("pwd:")
        };

        GN_INFO( "appDir = %s", s[0].cstr() );
        GN_INFO( "startup = %s", s[1].cstr() );
        GN_INFO( "pwd = %s", s[2].cstr() );

        for( size_t i = 0; i < 3; ++i )
        {
            TS_ASSERT( !s[i].empty() );
            if( s[i].size() > 1 )
            {
                TS_ASSERT_DIFFERS( PSC, s[i].last() );
            }
            else
            {
                TS_ASSERT_EQUALS( PSS, s[i] );
            }
        }
    }

    void testExist()
    {
        TS_ASSERT( GN::path::exist("/") );
        TS_ASSERT( !GN::path::exist("haha,heihei,hoho,huhu,mama,papa") );
        TS_ASSERT( GN::path::exist("SConstruct") );

        TS_ASSERT( GN::path::exist("startup:\\SConstruct") );
        TS_ASSERT( GN::path::exist("startup:/SConstruct") );
        TS_ASSERT( GN::path::exist("startup:SConstruct") );

        TS_ASSERT( GN::path::exist("app:GNtest.exe") );
        TS_ASSERT( GN::path::exist("app:/GNtest.exe") );
        TS_ASSERT( GN::path::exist("app:\\GNtest.exe") );

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

        TS_ASSERT( GN::path::isDir("app:") );
        TS_ASSERT( GN::path::isDir("app:/") );
        TS_ASSERT( GN::path::isDir("app:\\") );

        TS_ASSERT( GN::path::isDir("pwd:") );
        TS_ASSERT( GN::path::isDir("pwd:/") );
        TS_ASSERT( GN::path::isDir("pwd:\\") );

        TS_ASSERT( !GN::path::isDir("haha,heihei,hoho,huhu,mama,papa") );
    }
};
