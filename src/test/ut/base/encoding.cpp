#include "../testCommon.h"

class CharacterEncodingConversionTest : public CxxTest::TestSuite {
public:
    /// Test partial conversion when destination buffer is not large enough
    /// According to updated comments: "fill it as much as possible, return the converted bytes"
    void testNoEnoughSpaceInDest() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        size_t converted;

        char ascii[] = "abcd";
        // wide[4] can hold 4 wchar_t, but we need 5 (4 chars + null terminator)
        // So it should fill 4 wchar_t without null terminator
        wchar_t wide[4] = {0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE};
        converted       = c(wide, ascii);
        // Should return bytes filled (4 wchar_t = 4 * sizeof(wchar_t) bytes)
        TS_ASSERT_EQUALS(converted, 4 * sizeof(wchar_t));
        // Verify partial conversion - first 4 characters converted
        TS_ASSERT_EQUALS(wide[0], L'a');
        TS_ASSERT_EQUALS(wide[1], L'b');
        TS_ASSERT_EQUALS(wide[2], L'c');
        TS_ASSERT_EQUALS(wide[3], L'd');
        // Note: wide[3] should NOT be null-terminated since buffer is too small

        // fill wide with garbage data and do another test
        memset(wide, 0xFF, sizeof(wide));

        // set dest buffer size of 3, in this case, only 3 character should be
        // converted and dest buffer should not be null terminated.
        converted = c.convert(wide, 3 * sizeof(wchar_t), ascii, sizeof(ascii));
        TS_ASSERT_EQUALS(3 * sizeof(wchar_t), converted);

        // double check to ensure no dest buffer overrun
        TS_ASSERT_EQUALS(wide[0], L'a');
        TS_ASSERT_EQUALS(wide[1], L'b');
        TS_ASSERT_EQUALS(wide[2], L'c');
        TS_ASSERT_EQUALS(wide[3], (wchar_t) 0xFFFF);
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

    /// Test sEncoding2Str static method for all encoding types
    void testEncoding2Str() {
        using namespace GN;
        using CEC = CharacterEncodingConverter;

        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::ASCII), "ascii");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::ISO_8859_1), "iso-8859-1");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::UTF7), "utf-7");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::UTF8), "utf-8");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::UTF16_LE), "utf-16-le");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::UTF16_BE), "utf-16-be");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::UTF16), "utf-16");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::UTF32_LE), "utf-32-le");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::UTF32_BE), "utf-32-be");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::UTF32), "utf-32");
        // WIDECHAR depends on sizeof(wchar_t)
        const char * widecharStr = CEC::sEncoding2Str(CEC::WIDECHAR);
        TS_ASSERT(0 == GN::str::compare(widecharStr, "utf-16") || 0 == GN::str::compare(widecharStr, "utf-32"));
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::GBK), "gbk");
        TS_ASSERT_EQUALS(CEC::sEncoding2Str(CEC::BIG5), "big5");
        // Test invalid encoding
        TS_ASSERT_EQUALS(CEC::sEncoding2Str((CEC::Encoding) 999), "=[unknown]=");
    }

    /// Test UTF8 to WIDECHAR conversion
    void testUTF8_to_WIDECHAR() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::UTF8, CharacterEncodingConverter::WIDECHAR);

        // "Hello" in UTF-8
        char    utf8[] = "Hello";
        wchar_t wide[6];
        size_t  converted = c(wide, utf8);
        TS_ASSERT(converted > 0);
        TS_ASSERT_EQUALS(wide, L"Hello");
    }

    /// Test WIDECHAR to UTF8 conversion
    void testWIDECHAR_to_UTF8() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::WIDECHAR, CharacterEncodingConverter::UTF8);

        wchar_t wide[]    = L"Hello";
        char    utf8[10]  = {0};
        size_t  converted = c(utf8, wide);
        TS_ASSERT(converted > 0);
        TS_ASSERT_EQUALS(utf8, "Hello");
    }

    /// Test NULL destBuffer (size query mode)
    /// According to comments: "If destBuffer is NULL, return number of bytes required"
    void testNullDestBufferSizeQuery() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        char   ascii[]      = "abcd";
        size_t requiredSize = c.convert(nullptr, 0, ascii, sizeof(ascii));
        TS_ASSERT(requiredSize > 0);
        TS_ASSERT(requiredSize >= sizeof(ascii) * sizeof(wchar_t)); // At least enough for wide chars
    }

    /// Test empty string conversion
    void testEmptyString() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        char    empty[]   = "";
        wchar_t wide[1]   = {0xEFFE};
        size_t  converted = c(wide, empty);
        TS_ASSERT(converted > 0);
        TS_ASSERT_EQUALS(wide[0], L'\0');
    }

    /// Test round-trip conversion: ASCII -> WIDECHAR -> ASCII
    void testRoundTripASCII() {
        using namespace GN;

        char original[] = "Hello World";

        // ASCII -> WIDECHAR
        CharacterEncodingConverter c1(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);
        wchar_t                    wide[20];
        size_t                     converted1 = c1(wide, original);
        TS_ASSERT(converted1 > 0);

        // WIDECHAR -> ASCII
        CharacterEncodingConverter c2(CharacterEncodingConverter::WIDECHAR, CharacterEncodingConverter::ASCII);
        char                       result[20] = {0};
        size_t                     converted2 = c2(result, wide);
        TS_ASSERT(converted2 > 0);
        TS_ASSERT_EQUALS(result, original);
    }

    /// Test round-trip conversion: UTF8 -> WIDECHAR -> UTF8
    void testRoundTripUTF8() {
        using namespace GN;

        char original[] = "Hello World";

        // UTF8 -> WIDECHAR
        CharacterEncodingConverter c1(CharacterEncodingConverter::UTF8, CharacterEncodingConverter::WIDECHAR);
        wchar_t                    wide[20]   = {};
        size_t                     converted1 = c1(wide, original);
        TS_ASSERT(converted1 > 0);

        // WIDECHAR -> UTF8
        CharacterEncodingConverter c2(CharacterEncodingConverter::WIDECHAR, CharacterEncodingConverter::UTF8);
        char                       result[20] = {(char) 0x7F};
        size_t                     converted2 = c2(result, wide);
        TS_ASSERT_LESS_THAN(0, converted2);
        TS_ASSERT_EQUALS(result, original);
    }

    /// Test ISO_8859_1 to WIDECHAR conversion
    void testISO_8859_1_to_WIDECHAR() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ISO_8859_1, CharacterEncodingConverter::WIDECHAR);

        // "Hello" in ISO-8859-1 (same as ASCII for basic Latin)
        char    iso8859[] = "Hello";
        wchar_t wide[6];
        size_t  converted = c(wide, iso8859);
        TS_ASSERT(converted > 0);
        TS_ASSERT_EQUALS(wide, L"Hello");
    }

    /// Test UTF16_LE to UTF8 conversion
    void testUTF16LE_to_UTF8() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::UTF16_LE, CharacterEncodingConverter::UTF8);

        // "Hi" in UTF-16 LE: 0x0048, 0x0069, 0x0000
        unsigned char utf16le[] = {0x48, 0x00, 0x69, 0x00, 0x00, 0x00};
        char          utf8[10]  = {0};
        size_t        converted = c(utf8, utf16le, sizeof(utf16le));
        TS_ASSERT(converted > 0);
        TS_ASSERT_EQUALS(utf8, "Hi");
    }

    /// Test UTF8 to UTF16_LE conversion
    void testUTF8_to_UTF16LE() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::UTF8, CharacterEncodingConverter::UTF16_LE);

        char          utf8[]      = "Hi";
        unsigned char utf16le[10] = {0};
        size_t        converted   = c(utf16le, utf8);
        TS_ASSERT(converted > 0);
        // Verify first two bytes (little endian)
        TS_ASSERT_EQUALS(utf16le[0], 0x48);
        TS_ASSERT_EQUALS(utf16le[1], 0x00);
        TS_ASSERT_EQUALS(utf16le[2], 0x69);
        TS_ASSERT_EQUALS(utf16le[3], 0x00);
    }

    /// Test convert method with explicit size parameters (not using operator())
    void testConvertMethodExplicit() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        char    ascii[] = {'t', 'e', 's', 't', '\0', 'a'};
        wchar_t wide[]  = {0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE};

        // input size is 0
        size_t converted = c.convert(wide, sizeof(wide), ascii, 0);
        TS_ASSERT_EQUALS(converted, 0);
        TS_ASSERT_EQUALS(wide[0], (wchar_t) 0xEFFE);

        // input size is 1
        converted = c.convert(wide, sizeof(wide), ascii, 1);
        TS_ASSERT_EQUALS(converted, 1 * sizeof(wchar_t));
        TS_ASSERT_EQUALS(wide[0], L't');
        TS_ASSERT_EQUALS(wide[1], (wchar_t) 0xEFFE);

        // input size is 2
        converted = c.convert(wide, sizeof(wide), ascii, 2);
        TS_ASSERT_EQUALS(converted, 2 * sizeof(wchar_t));
        TS_ASSERT_EQUALS(wide[0], L't');
        TS_ASSERT_EQUALS(wide[1], L'e');
        TS_ASSERT_EQUALS(wide[2], (wchar_t) 0xEFFE);

        // input size is 4
        converted = c.convert(wide, sizeof(wide), ascii, 4);
        TS_ASSERT_EQUALS(converted, 4 * sizeof(wchar_t));
        TS_ASSERT_EQUALS(wide[0], L't');
        TS_ASSERT_EQUALS(wide[1], L'e');
        TS_ASSERT_EQUALS(wide[2], L's');
        TS_ASSERT_EQUALS(wide[3], L't');
        TS_ASSERT_EQUALS(wide[4], (wchar_t) 0xEFFE);

        // input size is 5 (including an null terminator)
        converted = c.convert(wide, sizeof(wide), ascii, 5);
        TS_ASSERT_EQUALS(converted, 5 * sizeof(wchar_t));
        TS_ASSERT_EQUALS(wide[0], L't');
        TS_ASSERT_EQUALS(wide[1], L'e');
        TS_ASSERT_EQUALS(wide[2], L's');
        TS_ASSERT_EQUALS(wide[3], L't');
        TS_ASSERT_EQUALS(wide[4], L'\0');
        TS_ASSERT_EQUALS(wide[5], (wchar_t) 0xEFFE);

        // input size is 6. the conversion will pass over the null terminator.
        converted = c.convert(wide, sizeof(wide), ascii, 6);
        TS_ASSERT_EQUALS(converted, 6 * sizeof(wchar_t));
        TS_ASSERT_EQUALS(wide[0], L't');
        TS_ASSERT_EQUALS(wide[1], L'e');
        TS_ASSERT_EQUALS(wide[2], L's');
        TS_ASSERT_EQUALS(wide[3], L't');
        TS_ASSERT_EQUALS(wide[4], L'\0');
        TS_ASSERT_EQUALS(wide[5], L'a');
        TS_ASSERT_EQUALS(wide[6], (wchar_t) 0xEFFE);
    }

    /// Test null termination behavior
    /// According to updated comments: "destBuffer is null terminated, only if source buffer is null terminated
    /// and destBuffer has enough space to hold the terminator"
    void testNullTermination() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        // Enough space - should be null-terminated
        char    ascii[] = "test"; // null-terminated
        wchar_t wide[10];
        size_t  converted = c(wide, ascii);
        TS_ASSERT(converted > 0);
        // Find null terminator
        size_t len = 0;
        while (len < 10 && wide[len] != L'\0') ++len;
        TS_ASSERT(len == 4); // "test" has 4 characters + null terminator
        TS_ASSERT_EQUALS(wide[4], L'\0');
    }

    /// Test UTF8 with multi-byte characters (e.g., emoji or non-ASCII)
    void testUTF8MultiByte() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::UTF8, CharacterEncodingConverter::WIDECHAR);

        // "café" in UTF-8 (é is 2 bytes: 0xC3 0xA9)
        unsigned char utf8[] = {0x63, 0x61, 0x66, 0xC3, 0xA9, 0x00}; // "café"
        wchar_t       wide[10];
        size_t        converted = c(wide, utf8);
        TS_ASSERT(converted > 0);
        TS_ASSERT_EQUALS(wide, L"café");
    }

    /// Test that conversion stops at null terminator even if sourceBufferSizeInBytes hasn't been reached
    /// According to updated comments: "the conversion will stop when null terminator is encountered,
    /// regardless if the end of the source buffer is reached"
    void testNullTerminatorStopsConversion() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        // Source buffer with null terminator in the middle
        // "ab\0cd" creates: {'a', 'b', '\0', 'c', 'd', '\0'}
        char    ascii[] = "ab\0cd"; // null terminator after "ab"
        wchar_t wide[10];
        // Pass size that includes "cd", but conversion should stop at first null terminator
        size_t converted = c.convert(wide, sizeof(wide), ascii, sizeof(ascii));
        TS_ASSERT(converted > 0);
        // Should only convert "ab" (2 chars + null terminator)
        TS_ASSERT_EQUALS(wide[0], L'a');
        TS_ASSERT_EQUALS(wide[1], L'b');
        TS_ASSERT_EQUALS(wide[2], L'\0');
        // "cd" should not be converted (conversion stopped at first null terminator)
    }

    /// Test conversion with source buffer that is NOT null-terminated
    void testNonNullTerminatedSource() {
        using namespace GN;

        CharacterEncodingConverter c(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);

        // Source buffer without null terminator
        char    ascii[]   = {'a', 'b', 'c'}; // No null terminator
        wchar_t wide[4]   = {0xEFFE, 0xEFFE, 0xEFFE, 0xEFFE};
        size_t  converted = c.convert(wide, sizeof(wide), ascii, sizeof(ascii));
        TS_ASSERT(converted > 0);
        // Should convert all 4 characters
        TS_ASSERT_EQUALS(wide[0], L'a');
        TS_ASSERT_EQUALS(wide[1], L'b');
        TS_ASSERT_EQUALS(wide[2], L'c');
        TS_ASSERT_EQUALS(wide[3], (wchar_t) 0xEFFE);
        // Should NOT be null-terminated since source wasn't null-terminated.
    }
};
