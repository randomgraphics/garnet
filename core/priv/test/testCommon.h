#ifndef __GN_TEST_TESTCOMMON_H__
#define __GN_TEST_TESTCOMMON_H__
// *****************************************************************************
//! \file    testCommon.h
//! \brief   common test header
//! \author  chenlee (2005.6.5)
// *****************************************************************************

#include "garnet/GnBase.h"
#include "cxxtest/TestSuite.h"

namespace CxxTest 
{
    CXXTEST_TEMPLATE_INSTANTIATION
    bool equals<const char *>( const char * x, const char * y )
    {
        return 0 == ::GN::strCmp(x,y);
    }

    CXXTEST_TEMPLATE_INSTANTIATION
    bool equals<const wchar_t *>( const wchar_t * x, const wchar_t * y )
    {
        return 0 == ::GN::strCmp(x,y);
    }

    CXXTEST_TEMPLATE_INSTANTIATION
    class ValueTraits< GN::StrA > 
    {
        GN::StrA s;
    public:
        ValueTraits( const GN::StrA & t ) : s(t) {}
        const char *asString() const { return s.cstr(); }
    };

    CXXTEST_TEMPLATE_INSTANTIATION
    class ValueTraits< GN::StrW > 
    {
        GN::StrA s;
    public:
        ValueTraits( const GN::StrW & t ) : s(GN::wcs2mbs(t)) {}
        const char *asString() const { return s.cstr(); }
    };

    CXXTEST_TEMPLATE_INSTANTIATION
    class ValueTraits< const wchar_t * > 
    {
        GN::StrA s;
    public:
        ValueTraits( const wchar_t * t ) : s(GN::wcs2mbs(t,0)) {}
        const char *asString() const { return s.cstr(); }
    };
};


// *****************************************************************************
//                           End of testCommon.h
// *****************************************************************************
#endif // __GN_TEST_TESTCOMMON_H__
