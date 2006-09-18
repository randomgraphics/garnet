#ifndef __GN_BASE_NUMERIC_H__
#define __GN_BASE_NUMERIC_H__
// *****************************************************************************
//! \file    base/numeric.h
//! \brief   Define numeric types
//! \author  chenlee (2006.3.4)
// *****************************************************************************

#include <stddef.h>
#include <wchar.h>
#if GN_GCC
#include <stdint.h>
#else
#include "stdint.h"
#endif

namespace GN
{
    //!
    //! unsigned char
    //!
    typedef unsigned char UChar;

    //!
    //! unsigned short
    //!
    typedef unsigned short UShort;

    //!
    //! unsigned integer
    //!
    typedef unsigned int UInt;

    //!
    //! unsigned long
    //!
    typedef unsigned long ULong;

    //!
    //! Bit field type. Normally used to hold bit flags.
    //!
    typedef int32_t BitField;
}

// *****************************************************************************
//                           End of numeric.h
// *****************************************************************************
#endif // __GN_BASE_NUMERIC_H__
