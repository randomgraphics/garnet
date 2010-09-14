#ifndef __GN_BASE_NUMERIC_H__
#define __GN_BASE_NUMERIC_H__
// *****************************************************************************
/// \file
/// \brief   Define numeric types
/// \author  chenlee (2006.3.4)
// *****************************************************************************

#include <stddef.h>
#include <wchar.h>

#if GN_POSIX
#include <stdint.h>
#endif

///
/// unsigned 8 bit integer
///
typedef unsigned char uint8;
GN_CASSERT( sizeof(uint8) == 1 );

///
/// unsigned 16 bit integer
///
typedef unsigned short uint16;
GN_CASSERT( sizeof(uint16) == 2 );

///
/// unsigned 32 bit integer
///
typedef unsigned int uint32;
GN_CASSERT( sizeof(uint32) == 4 );

///
/// unsigned 64 bit integer
///
#if GN_MSVC
typedef unsigned __int64 uint64;
#else
typedef unsigned long long uint64;
#endif
GN_CASSERT( sizeof(uint64) == 8 );

///
/// signed 8 bit integer
///
typedef signed char sint8;

///
/// signed 16 bit integer
///
typedef signed short sint16;

///
/// signed 32 bit integer
///
typedef signed int sint32;

///
/// signed 64 bit integer
///
#if GN_MSVC
typedef signed __int64 sint64;
#else
typedef signed long long sint64;
#endif

///
/// signed integer type that can hold a pointer
///
GN_CASSERT( sizeof(intptr_t) == sizeof(void*) );

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_NUMERIC_H__
