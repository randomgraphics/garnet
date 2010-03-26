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

        StrA pwd = ToNativeDiskFilePath( GetCurrentDir() );
        StrA d = GetCurrentDrive();

        TS_ASSERT_EQUALS( d+PSS, ToNativeDiskFilePath("/") );
        TS_ASSERT_EQUALS( d+PSS, ToNativeDiskFilePath("\\") );

        TS_ASSERT_EQUALS( d+PSS"a"PSS"b", ToNativeDiskFilePath("/a//b/") );
        TS_ASSERT_EQUALS( d+PSS"a"PSS"b", ToNativeDiskFilePath("\\a\\\\b\\") );

#if GN_MSWIN
        TS_ASSERT_EQUALS( "A:"PSS"b", ToNativeDiskFilePath("a:b") );
        TS_ASSERT_EQUALS( "A:"PSS, ToNativeDiskFilePath("a:") );
        TS_ASSERT_EQUALS( "A:"PSS, ToNativeDiskFilePath("a:/") );
        TS_ASSERT_EQUALS( "A:"PSS, ToNativeDiskFilePath("a:\\") );
#elif GN_POSIX
        TS_ASSERT_EQUALS( pwd+PSS"a:b", ToNativeDiskFilePath("a:b") );
        TS_ASSERT_EQUALS( pwd+PSS"a:", ToNativeDiskFilePath("a:") );
        TS_ASSERT_EQUALS( pwd+PSS"a:", ToNativeDiskFilePath("a:/") );
        TS_ASSERT_EQUALS( pwd+PSS"a:", ToNativeDiskFilePath("a:\\") );
#endif

        TS_ASSERT_EQUALS( pwd+PSS"a"PSS":", ToNativeDiskFilePath("a/:") );
        TS_ASSERT_EQUALS( pwd+PSS"a"PSS":", ToNativeDiskFilePath("a\\:") );
    }

    void testJoin()
    {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS( "", JoinPath( "", "" ) );
        TS_ASSERT_EQUALS( "a", JoinPath( "", "a" ) );
        TS_ASSERT_EQUALS( "a", JoinPath( "a", "" ) );
        TS_ASSERT_EQUALS( "a", JoinPath( "", "a", "" ) );
        TS_ASSERT_EQUALS( "a/b", JoinPath( "a", "b" ) );
        TS_ASSERT_EQUALS( "a/b", JoinPath( "", "a", "", "b", "" ) );
        TS_ASSERT_EQUALS( "/a/b/c", JoinPath( "\\a\\", "\\b\\", "\\c\\" ) );
        TS_ASSERT_EQUALS( "/a/b/c", JoinPath( "/a/", "/b/", "/c/" ) );
    }

    void testPrefix()
    {
        using namespace GN;
        using namespace GN::fs;

        StrA s[2] ={
            ToNativeDiskFilePath("app::"),
            ToNativeDiskFilePath("startup::"),
        };

        GN_INFO(sLogger)( "appDir = %s", s[0].ToRawPtr() );
        GN_INFO(sLogger)( "startup = %s", s[1].ToRawPtr() );

        for( size_t i = 0; i < 2; ++i )
        {
            TS_ASSERT( !s[i].Empty() );
            if( s[i].Size() > 1 )
            {
                TS_ASSERT_DIFFERS( PSC, s[i].GetLast() );
            }
            else
            {
                TS_ASSERT_EQUALS( PSS, s[i] );
            }
        }
    }

    void testPathExist()
    {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT( PathExist("/") );
        TS_ASSERT( !PathExist("haha,heihei,hoho,huhu,mama,papa") );
        TS_ASSERT( PathExist("SConstruct") );

        TS_ASSERT( PathExist("startup::\\SConstruct") );
        TS_ASSERT( PathExist("startup::/SConstruct") );
        TS_ASSERT( PathExist("startup::SConstruct") );

        TS_ASSERT( PathExist("app::GNut"APPEXT) );
        TS_ASSERT( PathExist("app::/GNut"APPEXT) );
        TS_ASSERT( PathExist("app::\\GNut"APPEXT) );
    }

    void testIsDir()
    {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT( IsDir("/") );

        TS_ASSERT( IsDir("startup::") );
        TS_ASSERT( IsDir("startup::/") );
        TS_ASSERT( IsDir("startup::\\") );

        TS_ASSERT( IsDir("app::") );
        TS_ASSERT( IsDir("app::/") );
        TS_ASSERT( IsDir("app::\\") );

        TS_ASSERT( !IsDir("haha,heihei,hoho,huhu,mama,papa") );
    }

    void testIsFile()
    {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT( IsFile("startup::\\SConstruct") );
        TS_ASSERT( IsFile("app::GNut"APPEXT) );
        TS_ASSERT( !IsFile("startup::") );
        TS_ASSERT( !IsFile("app::") );
        TS_ASSERT( !IsFile("haha,heihei,hoho,huhu,mama,papa") );
    }

    void testGetParent()
    {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS( "a", ParentPath("a/b") );
        TS_ASSERT_EQUALS( "a", ParentPath("a/b/") );
        TS_ASSERT_EQUALS( "/", ParentPath("/a") );
        TS_ASSERT_EQUALS( "/", ParentPath("/a/") );
        TS_ASSERT_EQUALS( "", ParentPath("a") );
        TS_ASSERT_EQUALS( "", ParentPath("a/") );
        TS_ASSERT_EQUALS( "/", ParentPath("/") );
        TS_ASSERT_EQUALS( "", ParentPath("") );
        TS_ASSERT_EQUALS( "startup::a", ParentPath("startup::a/b") );
        TS_ASSERT_EQUALS( "startup::/", ParentPath("startup::/a") );
        TS_ASSERT_EQUALS( "startup::", ParentPath("startup::a") );
        TS_ASSERT_EQUALS( "startup::/", ParentPath("startup::\\") );
        TS_ASSERT_EQUALS( "startup::", ParentPath("startup::") );
    }

    void testGetExt()
    {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS( ".c", ExtName("a.b.c") );
        TS_ASSERT_EQUALS( ".c", ExtName("a.c") );
        TS_ASSERT_EQUALS( ".c", ExtName(".c") );
        TS_ASSERT_EQUALS( ".", ExtName("a.") );
        TS_ASSERT_EQUALS( "", ExtName("a") );
    }

    void testToRelative()
    {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS( "..", RelPath( "a/b/c", "a\\b\\c\\d" ) );
        TS_ASSERT_EQUALS( "", RelPath( "a/b/c", "a\\b\\c" ) );
        TS_ASSERT_EQUALS( "c", RelPath( "a/b/c", "a\\b" ) );
        TS_ASSERT_EQUALS( "a/b/c", RelPath( "a/b/c", "" ) );
        TS_ASSERT_EQUALS( "a/b/c", RelPath( "a/b/c", "d/e" ) );
        TS_ASSERT_EQUALS( "c", RelPath( "c:/a/b/c", "c:/a/b" ) );
        TS_ASSERT_EQUALS( "c:/a/b", RelPath( "c:/a/b", "d:/a" ) );
    }

    void testBaseName()
    {
        using namespace GN;
        using namespace GN::fs;

        TS_ASSERT_EQUALS( "a.b", BaseName( "a.b.c" ) );
        TS_ASSERT_EQUALS( "a.b", BaseName( "media::a.b.c" ) );
        TS_ASSERT_EQUALS( "a.b", BaseName( "\\a.b.c" ) );
        TS_ASSERT_EQUALS( "a.b", BaseName( "c:\\a.b.c" ) );
    }
};
