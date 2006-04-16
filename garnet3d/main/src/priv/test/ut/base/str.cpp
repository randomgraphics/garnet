#include "../testCommon.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>

class StringTest : public CxxTest::TestSuite
{
public:

    void testAppend()
    {
        GN::StrW s1;

        s1.append( L"1111", 3 );
        TS_ASSERT_EQUALS( s1, L"111" );

        s1.clear();
        s1.append( L"2222", 4 );
        TS_ASSERT_EQUALS( s1, L"2222" );

        s1.clear();
        s1.append( L"3333", 5 );
        TS_ASSERT_EQUALS( s1, L"3333" );

        s1.clear();
        s1.append( GN::StrW(L"4444") );
        TS_ASSERT_EQUALS( s1, L"4444" );

        s1.append( L'5' );
        TS_ASSERT_EQUALS( s1, L"44445" );
    }

    void testTrim()
    {
        GN::StrW s1( L" 123 " ), s2( L"   " );
        s1.trim( L' ' );
        s2.trim( L' ' );
        TS_ASSERT_EQUALS( s1, L"123" );
        TS_ASSERT_EQUALS( s2, L"" );
    }

    void testTrimLeft()
    {
        GN::StrW s1( L" 123 " ), s2( L"   " );
        s1.trimLeft( L' ' );
        s2.trimLeft( L' ' );
        TS_ASSERT_EQUALS( s1, L"123 " );
        TS_ASSERT_EQUALS( s2, L"" );
    }

    void testTrimRight()
    {
        GN::StrW s1( L" 123 " ), s2( L"   " );
        s1.trimRight( L' ' );
        s2.trimRight( L' ' );
        TS_ASSERT_EQUALS( s1, L" 123" );
        TS_ASSERT_EQUALS( s2, L"" );
    }

    void testTrimUntil()
    {
        struct Local
        {
            static bool stop0( wchar_t ch ) { return L'0' == ch; }
            static bool stop1( wchar_t ch ) { return L'1' == ch; }
            static bool stop3( wchar_t ch ) { return L'3' == ch; }
        };

        GN::StrW s1( L"12345" );
        s1.trimRightUntil( &Local::stop3 );
        TS_ASSERT_EQUALS( s1, L"123" );
        s1.trimRightUntil( &Local::stop1 );
        TS_ASSERT_EQUALS( s1, L"1" );
        s1.trimRightUntil( &Local::stop0 );
        TS_ASSERT_EQUALS( s1, L"" );
    }

    void testInsert()
    {
        GN::StrW s1( L"123" );
        s1.insert( 0, L'0' );
        TS_ASSERT_EQUALS( s1, L"0123" );
        s1.insert( 1, L' ' );
        TS_ASSERT_EQUALS( s1, L"0 123" );
        s1.insert( 5, L'4' );
        TS_ASSERT_EQUALS( s1, L"0 1234" );
        s1.insert( 100, L'5' );
        TS_ASSERT_EQUALS( s1, L"0 12345" );
    }

    void testSubString()
    {
        // TODO: implement this test case.
    }

    void testCtor()
    {
        {
            GN::Str<wchar_t> s(0);
            TS_ASSERT_EQUALS(s,L"");
        }
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

        // append
        s1.append((const wchar_t*)NULL);
        TS_ASSERT_EQUALS( s1, L"" );
        s1.append(L"a",2);
        TS_ASSERT_EQUALS( s1, L"a" );

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
        TS_ASSERT( s1==s1 );
        TS_ASSERT( !(s1<s1) );
        TS_ASSERT( s1!=s2 );
        TS_ASSERT( s1<s2 );
        TS_ASSERT( !(s2<s1) );
        TS_ASSERT( s2==s3 );
        TS_ASSERT( !(s2<s3) );
        TS_ASSERT( !(s3<s2) );

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
        TS_ASSERT_EQUALS( L'h', s2[0] );
        TS_ASSERT_EQUALS( L'\0', s2[4] );

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
        s1.format(0,100);
        TS_ASSERT_EQUALS(s1,L"");
#else
        TS_WARN( "wide-char string formatting is not implemented on cygwin" );
#endif
    }

    void testEquality()
    {
        GN::Str<wchar_t> s1, s2, s3, s4, s5;

        s1 = L"haha";
        s2 = L"hehe";
        s3 = L"hehe";
        s4 = L"Hehe";
        s5 = L"Hehe__";

        TS_ASSERT_EQUALS(  0, GN::strCmp<wchar_t>(0,0) );
        TS_ASSERT_EQUALS( -1, GN::strCmp<wchar_t>(0,L"") );
        TS_ASSERT_EQUALS(  1, GN::strCmp<wchar_t>(L"",0) );
        TS_ASSERT_EQUALS(  0, GN::strCmp(L"",L"") );
        TS_ASSERT_EQUALS(  0, GN::strCmp(L"abc",L"abc") );
        TS_ASSERT_EQUALS( -1, GN::strCmp(L"ABC",L"abc") );
        TS_ASSERT_EQUALS(  1, GN::strCmp(L"abcd",L"abc") );
        TS_ASSERT_EQUALS(  1, GN::strCmp(L"abc",L"ABC") );
        TS_ASSERT_EQUALS( -1, GN::strCmp(L"abc",L"abcd") );

        TS_ASSERT( -1 == GN::strCmpI<wchar_t>( 0, s1.cptr() ) );
        TS_ASSERT( 1 == GN::strCmpI<wchar_t>( s1.cptr(), 0 ) );
        TS_ASSERT( 1 == GN::strCmpI( s5.cptr(), s3.cptr() ) );
        TS_ASSERT( 0 == GN::strCmpI( s4.cptr(), s4.cptr() ) );
        TS_ASSERT( 0 == GN::strCmpI( s2.cptr(), s4.cptr() ) );
        TS_ASSERT( 0 == GN::strCmpI( s4.cptr(), s2.cptr() ) );
        TS_ASSERT( -1 == GN::strCmpI( s1.cptr(), s4.cptr() ) );
        TS_ASSERT( 1 == GN::strCmpI( s4.cptr(), s1.cptr() ) );

    }

    void testStrFunc()
    {
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
