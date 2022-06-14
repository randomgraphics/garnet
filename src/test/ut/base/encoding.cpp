#include "../testCommon.h"

class CharacterEncodingConversionTest : public CxxTest::TestSuite {
public:
    void testNoEnoughSpaceInDest() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        size_t converted;

        char    ascii[] = "abcd";
        wchar_t wide[4] = {0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE};
        converted       = c(wide, ascii);
        TS_ASSERT_EQUALS(converted, 0);
    }

    void testASCII_to_WIDECHAR() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        size_t converted;

        char    ascii[] = "abcd";
        wchar_t wide[5] = {0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE};
        converted       = c(wide, ascii);
        TS_ASSERT_EQUALS(converted, sizeof(wide));
        TS_ASSERT_EQUALS(wide, L"abcd");
    }

    void testGBK_to_WIDECHAR() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::GBK, CharacterEncodingConverter::WIDECHAR);

        size_t converted;

        unsigned char gbk[]    = {0xC4, 0xE3, 0xBA, 0xC3, 0xC2, 0xF0, 0x00}; // "你好吗" in GBK
        wchar_t       wide[]   = {2, 2, 2, 2};
        wchar_t       golden[] = L"你好吗";
        converted              = c(wide, gbk);
        TS_ASSERT_EQUALS(converted, sizeof(wide));
        TS_ASSERT_EQUALS((const char *) wide, (const char *) golden);
    }

    void testWIDECHAR_to_GBK() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::WIDECHAR, CharacterEncodingConverter::GBK);

        size_t converted;

        wchar_t       wide[]   = L"你好吗";
        unsigned char gbk[7]   = {2, 2, 2, 2, 2, 2, 2};
        unsigned char golden[] = {0xC4, 0xE3, 0xBA, 0xC3, 0xC2, 0xF0, 0x00}; // "你好吗" in GBK
        converted              = c(gbk, wide);
        TS_ASSERT_EQUALS(converted, sizeof(gbk));
        TS_ASSERT_EQUALS((const char *) gbk, (const char *) golden);
    }

    void testBIG5_to_GBK() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::BIG5, CharacterEncodingConverter::GBK);

        unsigned char big5[]    = {0xAF, 0x75, 0xA5, 0xD1, 0xAC, 0xFC, 0x00}; // "真由美" in BIG5 encoding
        unsigned char gbk[7]    = {2, 2, 2, 2, 2, 2, 2};
        unsigned char golden[]  = {0xD5, 0xE6, 0xD3, 0xC9, 0xC3, 0xC0, 0x00}; // "真由美" in GBK encoding
        size_t        converted = c(gbk, big5);
        TS_ASSERT_EQUALS(converted, 7);
        TS_ASSERT_EQUALS((const char *) gbk, (const char *) golden);
    }

    void testGBK_to_BIG5() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::GBK, CharacterEncodingConverter::BIG5);

        unsigned char gbk[]     = {0xD5, 0xE6, 0xD3, 0xC9, 0xC3, 0xC0, 0x00}; // "真由美" in GBK encoding
        unsigned char big5[7]   = {2, 2, 2, 2, 2, 2, 2};
        unsigned char golden[]  = {0xAF, 0x75, 0xA5, 0xD1, 0xAC, 0xFC, 0x00}; // "真由美" in BIG5 encoding
        size_t        converted = c(big5, gbk);
        TS_ASSERT_EQUALS(converted, 7);
        TS_ASSERT_EQUALS((const char *) big5, (const char *) golden);
    }
};
