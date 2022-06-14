#ifndef __GN_TEST_TESTCOMMON_H__
#define __GN_TEST_TESTCOMMON_H__
// *****************************************************************************
/// \file
/// \brief   common test header
/// \author  chenlee (2005.6.5)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "cxxtest/TestSuite.h"

#if GN_XBOX2
    #include <xtl.h>
#elif GN_WINPC
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
#endif

static GN::Logger * sLogger = GN::getLogger("GN.gfx.test.UT");

///
/// namespace of CxxTest framework
///
namespace CxxTest {
CXXTEST_TEMPLATE_INSTANTIATION
///
/// Equality check of multi-byte char c-style string
///
bool equals<const char *, const char *>(const char * x, const char * y) { return 0 == ::GN::str::compare(x, y); }

CXXTEST_TEMPLATE_INSTANTIATION
///
/// Equality check of multi-byte char c-style string
///
bool equals<const char *, char *>(const char * x, char * y) { return 0 == ::GN::str::compare(x, y); }

CXXTEST_TEMPLATE_INSTANTIATION
///
/// Equality check of multi-byte char c-style string
///
bool equals<char *, const char *>(char * x, const char * y) { return 0 == ::GN::str::compare(x, y); }

CXXTEST_TEMPLATE_INSTANTIATION
///
/// Equality check of multi-byte char c-style string
///
bool equals<char *, char *>(char * x, char * y) { return 0 == ::GN::str::compare(x, y); }

CXXTEST_TEMPLATE_INSTANTIATION
///
/// Equality check of wide char c-style string
///
bool equals<const wchar_t *, const wchar_t *>(const wchar_t * x, const wchar_t * y) { return 0 == ::GN::str::compare(x, y); }

CXXTEST_TEMPLATE_INSTANTIATION
///
/// Equality check of wide char c-style string
///
bool equals<wchar_t *, const wchar_t *>(wchar_t * x, const wchar_t * y) { return 0 == ::GN::str::compare(x, y); }

CXXTEST_TEMPLATE_INSTANTIATION
///
/// Equality check of wide char c-style string
///
bool equals<const wchar_t *, wchar_t *>(const wchar_t * x, wchar_t * y) { return 0 == ::GN::str::compare(x, y); }

CXXTEST_TEMPLATE_INSTANTIATION
///
/// Equality check of wide char c-style string
///
bool equals<wchar_t *, wchar_t *>(wchar_t * x, wchar_t * y) { return 0 == ::GN::str::compare(x, y); }
} // namespace CxxTest

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_TEST_TESTCOMMON_H__
