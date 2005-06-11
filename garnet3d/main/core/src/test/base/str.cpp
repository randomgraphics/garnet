#include "../testCommon.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>

class StringTest : public CxxTest::TestSuite
{
public:

    void testCtor()
    {
        {
            GN::Str<wchar_t> s;
            TS_ASSERT_EQUALS(s,L"");
        }
        {
            GN::Str<wchar_t> s(L"haha");
            TS_ASSERT_EQUALS(s,L"haha");
        }
        {
            GN::Str<wchar_t> s(L"haha",0);
            TS_ASSERT_EQUALS(s,L"haha");
        }
        {
            GN::Str<wchar_t> s(L"haha",3);
            TS_ASSERT_EQUALS(s,L"hah");
        }
        {
            GN::Str<wchar_t> s(L"haha",5);
            TS_ASSERT_EQUALS(s,L"haha");
        }
        {
            GN::Str<wchar_t> s1(L"haha");
            GN::Str<wchar_t> s2(s1);
            TS_ASSERT_EQUALS(s1,s2);
        }
    }

    void testMethod()
    {
        GN::Str<wchar_t> s1, s2, s3;

        // assignment
        s1.assign(0,1);
        TS_ASSERT_EQUALS( s1, L"" );
        s1.assign( L"haha" );
        TS_ASSERT_EQUALS( s1, L"haha" );
        s1.assign( L"haha", 3 );
        TS_ASSERT_EQUALS( s1, L"hah" );
        s1.assign( L"haha", 5 );
        TS_ASSERT_EQUALS( s1, L"haha" );

        // caps
        s1.setCaps(1);
        TS_ASSERT_EQUALS( s1, L"haha" );
        TS_ASSERT_LESS_EQUALS( 4, s1.getCaps() );

        // equality
        s1 = L"haha";
        s2 = L"hehe";
        s3 = L"hehe";
        TS_ASSERT(s1 != s2);
        TS_ASSERT(s2 == s3);

        // size(),clear(),empty()
        TS_ASSERT_EQUALS( 4, s1.size() );
        TS_ASSERT( !s1.empty() );
        s1.clear(); s1.clear();
        TS_ASSERT( s1.empty() );
        TS_ASSERT_EQUALS( 0, s1.size() );

        // toStl
#if !GN_CYGWIN // cygwin has no support to std::wstring
        std::wstring ws;
        s2.toSTL( ws );
        TS_ASSERT_EQUALS( s2, ws.c_str() );
        ws = s3.toSTL();
        TS_ASSERT_EQUALS( s3, ws.c_str() );
#endif

        // []
        TS_ASSERT_EQUALS( 'h', s2[0] );
        TS_ASSERT_EQUALS( 0, s2[4] );

        // +, +=
        s1 += s2;
        TS_ASSERT_EQUALS( s1, s2 );
        s1 += L"abcd";
        TS_ASSERT_EQUALS( s1, L"heheabcd" );
        s1 = s2 + s3;
        TS_ASSERT_EQUALS( s1, L"hehehehe" );

        // format
#if !GN_CYGWIN
        s1.format(L"haha%d",100);
        TS_ASSERT_EQUALS(s1,L"haha100");
#else
        TS_WARN( "wide-char string formatting is not implemented on cygwin" );
#endif
    }

    void strfunc_test()
    {
        TS_ASSERT_EQUALS(  0, GN::strCmp<wchar_t>(0,0) );
        TS_ASSERT_EQUALS( -1, GN::strCmp<wchar_t>(0,L"") );
        TS_ASSERT_EQUALS(  1, GN::strCmp<wchar_t>(L"",0) );
        TS_ASSERT_EQUALS(  0, GN::strCmp(L"",L"") );
        TS_ASSERT_EQUALS(  0, GN::strCmp(L"abc",L"abc") );
        TS_ASSERT_EQUALS( -1, GN::strCmp(L"ABC",L"abc") );
        TS_ASSERT_EQUALS(  1, GN::strCmp(L"abcd",L"abc") );
        TS_ASSERT_EQUALS(  1, GN::strCmp(L"abc",L"ABC") );
        TS_ASSERT_EQUALS( -1, GN::strCmp(L"abc",L"abcd") );

        GN::StrA s1("abcd");
        GN::StrW s2;

        s2 = GN::mbs2wcs(s1);
        TS_ASSERT_EQUALS( s2, L"abcd" );
        s1 = "xyzw";
        GN::mbs2wcs( s2, s1 );
        TS_ASSERT_EQUALS( s2, L"xyzw" );

        s2 = L"bcda";
        s1 = GN::wcs2mbs(s2);
        TS_ASSERT_EQUALS( s1, "bcda" );
        s2 = L"wzyx";;
        s1 = GN::wcs2mbs(s2);
        TS_ASSERT_EQUALS( s1, "wzyx" );
    }
};
