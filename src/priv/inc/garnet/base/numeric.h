#ifndef __GN_BASE_NUMERIC_H__
#define __GN_BASE_NUMERIC_H__
// *****************************************************************************
/// \file
/// \brief   Define numeric types
/// \author  chenlee (2006.3.4)
// *****************************************************************************

#include <stddef.h>
#include <wchar.h>

///
/// unsigned 8 bit integer
///
typedef unsigned char UInt8;
GN_CASSERT( sizeof(UInt8) == 1 );

///
/// unsigned 16 bit integer
///
typedef unsigned short UInt16;
GN_CASSERT( sizeof(UInt16) == 2 );

///
/// unsigned 32 bit integer
///
typedef unsigned int UInt32;
GN_CASSERT( sizeof(UInt32) == 4 );

///
/// unsigned 64 bit integer
///
#if GN_MSVC
typedef unsigned __int64 UInt64;
#else
typedef unsigned long long UInt64;
#endif
GN_CASSERT( sizeof(UInt64) == 8 );

///
/// signed 8 bit integer
///
typedef signed char SInt8;

///
/// signed 16 bit integer
///
typedef signed short SInt16;

///
/// signed 32 bit integer
///
typedef signed int SInt32;

///
/// signed 64 bit integer
///
#if GN_MSVC
typedef signed __int64 SInt64;
#else
typedef signed long long SInt64;
#endif

///
/// unsigned char
///
typedef unsigned char UChar;

///
/// unsigned short
///
typedef unsigned short UShort;

///
/// unsigned integer
///
typedef unsigned int UInt;

///
/// unsigned long
///
typedef unsigned long ULong;

///
/// Bit fields. Used to hold bit flags.
///
typedef UInt32 BitFields;

///
/// unsigned integer type that can hold a pointer
///
#if GN_X64
typedef UInt64 UIntPtr;
#else
typedef UInt32 UIntPtr;
#endif
GN_CASSERT( sizeof(UIntPtr) == sizeof(void*) );

///
/// signed integer type that can hold a pointer
///
#if GN_X64
typedef SInt64 SIntPtr;
#else
typedef SInt32 SIntPtr;
#endif
GN_CASSERT( sizeof(SIntPtr) == sizeof(void*) );

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_NUMERIC_H__
