#ifndef __GN_BASE_STDINT_H__
#define __GN_BASE_STDINT_H__
// *****************************************************************************
//! file    stdint.h
//! brief   ISO C99: 7.18 Integer types <stdint.h>, for msvc.
//! author  chenlee (2005.4.17)
// *****************************************************************************

/* Exact integral types.  */

typedef signed char    int8_t;
typedef signed short   int16_t;
typedef signed int     int32_t;
typedef signed __int64 int64_t;

typedef unsigned char    uint8_t;
typedef unsigned short   uint16_t;
typedef unsigned int     uint32_t;
typedef unsigned __int64 uint64_t;

/* Small types.  */

typedef int8_t  int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

typedef uint8_t  uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

/* Fast types.  */

typedef int8_t  int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;

typedef uint8_t  uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

/* Types for `void *' pointers.  */
// Note: intptr_t and uintptr_t should be already defined by MSVC
GN_CASSERT_EX( sizeof(intptr_t) == sizeof(void*), intptr_size_checker );
GN_CASSERT_EX( sizeof(uintptr_t) == sizeof(void*), uintptr_size_checker );

/* Largest integral types.  */
typedef int64_t  intmax_t;
typedef uint64_t uintmax_t;

// *****************************************************************************
//                              End of stdint.h
// *****************************************************************************
#endif // __GN_BASE_STDINT_H__
