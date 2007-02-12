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


class PathTest : public CxxTest::TestSuite
{
public:

    void testToNative()
    {
        using namespace GN;
        using namespace GN::fs;

        StrA pwd = toNative( getCurrentDir() );
        StrA d = getCurrentDrive();

        TS_ASSERT_EQUALS( d+PSS, toNative("/") );
        TS_ASSERT_EQUALS( d+PSS, toNative("\\") );

        TS_ASSERT_EQUALS( d+PSS"a"PSS"b", toNative("/a//b/") );
        TS_ASSERT_EQUALS( d+PSS"a"PSS"b", toNative("\\a\\\\b\\") );

#if GN_MSWIN
        TS_ASSERT_EQUALS( "A:"PSS"b", toNative("a:b") );
        TS_ASSERT_EQUALS( "A:"PSS, toNative("a:") );
        TS_ASSERT_EQUALS( "A:"PSS, toNative("a:/") );
        TS_ASSERT_EQUALS( "A:"PSS, toNative("a:\\") );
#elif GN_POSIX
        TS_ASSERT_EQUALS( pwd+PSS"a:b", toNative("a:b") );
        TS_ASSERT_EQUALS( pwd+PSS"a:", toNative("a:") );
        TS_ASSERT_EQUALS( pwd+PSS"a:", toNative("a:/") );
        TS_ASSERT_EQUALS( pwd+PSS"a:", toNative("a:\\") );
#endif

        TS_ASSERT_EQUALS( pwd+PSS"a"PSS":", toNative("a/:") );
        TS_ASSERT_EQUALS( pwd+PSS"a"PSS":", toNative("a\\:") );
    }

    void testJoin()
    {
        using namespace GN;
        TS_ASSERT_EQUALS( "", joinPath( "", "" ) );
        TS_ASSERT_EQUALS( "a", joinPath( "", "a" ) );
        TS_ASSERT_EQUALS( "a", joinPath( "a", "" ) );
        TS_ASSERT_EQUALS( "a", joinPath( "", "a", "" ) );
        TS_ASSERT_EQUALS( "a/b", joinPath( "a", "b" ) );
        TS_ASSERT_EQUALS( "a/b", joinPath( "", "a", "", "b", "" ) );
        TS_ASSERT_EQUALS( "/a/b/c", joinPath( "\\a\\", "\\b\\", "\\c\\" ) );
        TS_ASSERT_EQUALS( "/a/b/c", joinPath( "/a/", "/b/", "/c/" ) );
    }

    void testPrefix()
    {
        using namespace GN;
        using namespace GN::fs;

        StrA s[2] ={
            toNative("app::"),
            toNative("startup::"),
        };

        GN_INFO(sLogger)( "appDir = %s", s[0].cptr() );
        GN_INFO(sLogger)( "startup = %s", s[1].cptr() );

        for( size_t i = 0; i < 2; ++i )
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
        using namespace GN::fs;
        TS_ASSERT( exist("/") );
        TS_ASSERT( !exist("haha,heihei,hoho,huhu,mama,papa") );
        TS_ASSERT( exist("SConstruct") );

        TS_ASSERT( exist("startup::\\SConstruct") );
        TS_ASSERT( exist("startup::/SConstruct") );
        TS_ASSERT( exist("startup::SConstruct") );

        TS_ASSERT( exist("app::GNut"APPEXT) );
        TS_ASSERT( exist("app::/GNut"APPEXT) );
        TS_ASSERT( exist("app::\\GNut"APPEXT) );
    }

    void testIsDir()
    {
        using namespace GN::fs;
        TS_ASSERT( isDir("/") );

        TS_ASSERT( isDir("startup::") );
        TS_ASSERT( isDir("startup::/") );
        TS_ASSERT( isDir("startup::\\") );

        TS_ASSERT( isDir("app::") );
        TS_ASSERT( isDir("app::/") );
        TS_ASSERT( isDir("app::\\") );

        TS_ASSERT( !isDir("haha,heihei,hoho,huhu,mama,papa") );
    }

    void testIsFile()
    {
        using namespace GN::fs;
        TS_ASSERT( isFile("startup::\\SConstruct") );
        TS_ASSERT( isFile("app::GNut"APPEXT) );
        TS_ASSERT( !isFile("startup::") );
        TS_ASSERT( !isFile("app::") );
        TS_ASSERT( !isFile("haha,heihei,hoho,huhu,mama,papa") );
    }

    void testGetParent()
    {
        using namespace GN;
        TS_ASSERT_EQUALS( "a", parentPath("a/b") );
        TS_ASSERT_EQUALS( "a", parentPath("a/b/") );
        TS_ASSERT_EQUALS( "/", parentPath("/a") );
        TS_ASSERT_EQUALS( "/", parentPath("/a/") );
        TS_ASSERT_EQUALS( "", parentPath("a") );
        TS_ASSERT_EQUALS( "", parentPath("a/") );
        TS_ASSERT_EQUALS( "/", parentPath("/") );
        TS_ASSERT_EQUALS( "", parentPath("") );
        TS_ASSERT_EQUALS( "startup::a", parentPath("startup::a/b") );
        TS_ASSERT_EQUALS( "startup::/", parentPath("startup::/a") );
        TS_ASSERT_EQUALS( "startup::", parentPath("startup::a") );
        TS_ASSERT_EQUALS( "startup::/", parentPath("startup::\\") );
        TS_ASSERT_EQUALS( "startup::", parentPath("startup::") );
    }

    void testGetExt()
    {
        using namespace GN;
        TS_ASSERT_EQUALS( ".c", extName("a.b.c") );
        TS_ASSERT_EQUALS( ".c", extName("a.c") );
        TS_ASSERT_EQUALS( ".c", extName(".c") );
        TS_ASSERT_EQUALS( ".", extName("a.") );
        TS_ASSERT_EQUALS( "", extName("a") );
    }

    void testToRelative()
    {
        using namespace GN;
        TS_ASSERT_EQUALS( "..", relPath( "a/b/c", "a\\b\\c\\d" ) );
        TS_ASSERT_EQUALS( "", relPath( "a/b/c", "a\\b\\c" ) );
        TS_ASSERT_EQUALS( "c", relPath( "a/b/c", "a\\b" ) );
        TS_ASSERT_EQUALS( "a/b/c", relPath( "a/b/c", "" ) );
        TS_ASSERT_EQUALS( "a/b/c", relPath( "a/b/c", "d/e" ) );
        TS_ASSERT_EQUALS( "c", relPath( "c:/a/b/c", "c:/a/b" ) );
        TS_ASSERT_EQUALS( "c:/a/b", relPath( "c:/a/b", "d:/a" ) );
    }
};
