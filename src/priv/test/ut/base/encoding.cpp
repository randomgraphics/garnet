#include "../testCommon.h"

class CharacterEncodingConversionTest : public CxxTest::TestSuite
{
public:

    void testNoEnoughSpaceInDest()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::ASCII,
            CharacterEncodingConverter::WIDECHAR );

        size_t converted;

        char ascii[] = "abcd";
        wchar_t wide[4] = { 0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE };
        converted = c( wide, ascii );
        TS_ASSERT_EQUALS( converted, 0 );
    }

    void testASCII_to_WIDECHAR()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::ASCII,
            CharacterEncodingConverter::WIDECHAR );

        size_t converted;

        char ascii[] = "abcd";
        wchar_t wide[5] = { 0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE };
        converted = c( wide, ascii );
        TS_ASSERT_EQUALS( converted, sizeof(wide) );
        TS_ASSERT_EQUALS( wide, L"abcd" );
    }

    void testGBK_to_WIDECHAR()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::GBK,
            CharacterEncodingConverter::WIDECHAR );

        size_t converted;

        char gbk[] = "你好吗";
        wchar_t wide[4] = { 0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE };
        converted = c( wide, gbk );
        TS_ASSERT_EQUALS( converted, sizeof(wide) );
        TS_ASSERT_EQUALS( wide, L"你好吗" );
    }

    void testWIDECHAR_to_GBK()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::WIDECHAR,
            CharacterEncodingConverter::GBK );

        size_t converted;

        wchar_t wide[] = L"你好吗";
        char gbk[7] = { -2, -2, -2, -2, -2, -2, -2 };
        converted = c( gbk, wide );
        TS_ASSERT_EQUALS( converted, sizeof(gbk) );
        TS_ASSERT_EQUALS( gbk, "你好吗" );
    }

    void testBIG5_to_GBK()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::BIG5,
            CharacterEncodingConverter::GBK );

        char big5[] = { 0xAF, 0x75, 0xA5, 0xD1, 0xAC, 0xFC, 0x00 }; // "真由美" in BIG5 encoding
        char gbk[7] = { -2, -2, -2, -2, -2, -2, -2 };
        size_t converted = c( gbk, big5 );
        TS_ASSERT_EQUALS( converted, 7 );
        TS_ASSERT_EQUALS( gbk, "真由美" );
    }

    void testGBK_to_BIG5()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::GBK,
            CharacterEncodingConverter::BIG5 );

        char gbk[] = "真由美";
        char big5[7] = { -2, -2, -2, -2, -2, -2, -2 };
        char golden[] = { 0xAF, 0x75, 0xA5, 0xD1, 0xAC, 0xFC, 0x00 }; // "真由美" in BIG5 encoding
        size_t converted = c( big5, gbk );
        TS_ASSERT_EQUALS( converted, 7 );
        TS_ASSERT_EQUALS( big5, golden );
    }
};
