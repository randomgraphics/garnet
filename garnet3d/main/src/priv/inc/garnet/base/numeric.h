#ifndef __GN_BASE_NUMERIC_H__
#define __GN_BASE_NUMERIC_H__
// *****************************************************************************
//! \file    base/numeric.h
//! \brief   Define numeric types
//! \author  chenlee (2006.3.4)
// *****************************************************************************

#include <stddef.h>
#include <wchar.h>

//!
//! unsigned 8 bit integer
//!
typedef unsigned char UInt8;

//!
//! unsigned 16 bit integer
//!
typedef unsigned short UInt16;

//!
//! unsigned 32 bit integer
//!
typedef unsigned int UInt32;

//!
//! unsigned 64 bit integer
//!
#if GN_MSVC
typedef unsigned __int64 UInt64;
#else
typedef uint64_t UInt64;
#endif

//!
//! signed 8 bit integer
//!
typedef signed char SInt8;

//!
//! signed 16 bit integer
//!
typedef signed short SInt16;

//!
//! signed 32 bit integer
//!
typedef signed int SInt32;

//!
//! signed 64 bit integer
//!
#if GN_MSVC
typedef signed __int64 SInt64;
#else
typedef int64_t SInt64;
#endif

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
//! Bit fields. Used to hold bit flags.
//!
typedef UInt32 BitFields;

//!
//! unsigned integer type that can hold a pointer
//!
typedef uintptr_t UIntPtr;

//!
//! signed integer type that can hold a pointer
//!
typedef intptr_t SIntPtr;

GN_CASSERT_EX( sizeof(SIntPtr) == sizeof(void*), int_ptr_size_checker );
GN_CASSERT_EX( sizeof(UIntPtr) == sizeof(void*), uint_ptr_size_checker );

// *****************************************************************************
//                           End of numeric.h
// *****************************************************************************
#endif // __GN_BASE_NUMERIC_H__
