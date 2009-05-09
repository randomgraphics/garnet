#include "../testCommon.h"

class CharacterEncodingConversionTest : public CxxTest::TestSuite
{
public:

    void test1()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::ASCII,
            CharacterEncodingConverter::UTF16 );

        size_t converted;

        char ascii[] = "abcd";
        wchar_t utf16[6];
        converted = c( utf16, ascii );
        TS_ASSERT_EQUALS( converted, 10 );
        TS_ASSERT_EQUALS( utf16, L"abcd" );
    }

    void test2()
    {
        using namespace GN;

        CharacterEncodingConverter c(
            CharacterEncodingConverter::BIG5,
            CharacterEncodingConverter::GBK );

        char big5[] = "úR≤œÀá";
        char gbk[7];
        size_t converted = c( gbk, big5 );
        TS_ASSERT_EQUALS( converted, 7 );
        TS_ASSERT_EQUALS( gbk, "’Ê”…√¿" );
    }
};
