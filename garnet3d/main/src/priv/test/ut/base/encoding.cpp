#include "../testCommon.h"

class CharacterEncodingConversionTest : public CxxTest::TestSuite
{
public:

    void testNoEnoughSpaceInDest()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::ASCII,
            CharacterEncodingConverter::UTF16 );

        size_t converted;

        char ascii[] = "abcd";
        wchar_t utf16[4];
        converted = c( utf16, ascii );
        TS_ASSERT_EQUALS( converted, 0 );
    }

    void testASCII_to_UTF16()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::ASCII,
            CharacterEncodingConverter::UTF16 );

        size_t converted;

        char ascii[] = "abcd";
        wchar_t utf16[5];
        converted = c( utf16, ascii );
        TS_ASSERT_EQUALS( converted, 10 );
        TS_ASSERT_EQUALS( utf16, L"abcd" );
    }

    void testBIG5_to_GBK()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::BIG5,
            CharacterEncodingConverter::GBK );

        char big5[] = "痷パ"; // { 0xAF, 0x75, 0xA5, 0xD1, 0xAC, 0xFC }
        char gbk[7];
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
        char big5[7];
        size_t converted = c( big5, gbk );
        TS_ASSERT_EQUALS( converted, 7 );
        TS_ASSERT_EQUALS( big5, "痷パ" );
    }
};
