#ifndef __GN_BASE_BASICDEFINES_H__
#define __GN_BASE_BASICDEFINES_H__
// *****************************************************************************
//! \file    basicDefines.h
//! \brief   Basic defines and types
//! \author  chenlee (2005.7.24)
// *****************************************************************************

#define GN_MSVC 0 //!< If 1, means current compiler is msvc (or icl)
#define GN_ICL  0 //!< If 1, means current compiler is intel c++ compiler 
#define GN_GCC  0 //!< If 1, means current compierl is gcc/g++
#define GN_BCB  0 //!< If 1, means current compierl is boland c++ compiler

// 辨识编译器
#if defined(_MSC_VER) && !defined(__ICL)
#undef GN_MSVC
#define GN_MSVC 1
#define GN_COMPILER "msvc"
#elif defined(__ICL)
#undef GN_MSVC
#undef GN_ICL
#define GN_MSVC 1                  // treat intel compiler as VC compiler
#define GN_ICL  1
#define GN_COMPILER "icl"
#elif defined(__BORLANDC__)
#undef GN_BCB
#define GN_BCB 1
#define GN_COMPILER "bcb"
#elif defined(__GNUC__)
#undef GN_GCC
#define GN_GCC 1
#define GN_COMPILER "gcc"
#else
#error "Unknown compiler!"
#endif


// 辨识操作系统
#define GN_WIN32  0 //!< If 1, means current platform is Windows
#define GN_AMD64  0 //!< If 1, means current platform is Windows x64
#define GN_CYGWIN 0 //!< If 1, means current platform is Cygwin
#define GN_POSIX  0 //!< If 1, means current platform is POSIX compatible, such as Cygwin
#if defined( _WIN32 ) || defined( WIN32 )      // Win32
// Windows platform
#undef GN_WIN32
#define GN_WIN32 1
#if defined( _WIN64 ) || defined( WIN64 )
// Amd64 platform
#define GN_WIN64 1
#define GN_PLATFORM "amd64"
#else
// Win32 platform
#define GN_PLATFORM "win32"
#endif
#elif defined(__CYGWIN__)
// Cygwin platform
#undef GN_CYGWIN
#undef GN_POSIX
#define GN_CYGWIN 1
#define GN_POSIX  1 // cygwin also provides some posix compabilities
#define GN_PLATFORM "cygwin"
#elif defined( __unix ) || defined( __unix__ )
// posix-like platform
#undef GN_POSIX
#define GN_POSIX 1
#define GN_PLATFORM "posix"
#else
#error "unknown platform!"
#endif

//!
//! force inline macro
//!
#if GN_MSVC
#define GN_FORCE_INLINE   __forceinline
#else
#define GN_FORCE_INLINE   inline
#endif

//!
//! automatic inline macro
//!
#if GN_ENABLE_INLINE
#define GN_INLINE       GN_FORCE_INLINE
#else
#define GN_INLINE
#endif

// Export/Import function/variable tag

//! \def GN_EXPORT
//! Export function tag

//! \def GN_IMPORT
//! Import function tag

#if GN_MSVC && !GN_STATIC
#define GN_EXPORT       __declspec(dllexport)
#define GN_IMPORT       __declspec(dllimport)
#else
#define GN_IMPORT
#define GN_EXPORT
#endif

//!
//! Public/global function/variable tag
//!
#if defined(_LIB)
#define GN_PUBLIC // GN_PUBLIC is meaningless for static library
#elif defined(_GN_CORE)
#define GN_PUBLIC   GN_EXPORT
#else
#define GN_PUBLIC   GN_IMPORT
#endif

//!
//! C style function tag
//!
#ifdef __cplusplus
#define C_LINKAGE         "C"
#else
#define C_LINKAGE
#endif

//!
//! 函数名称
//!
#if GN_MSVC
#define GN_FUNCTION          __FUNCTION__
#elif defined( GN_GNUC )
#define GN_FUNCTION          __PRETTY_FUNCTION__
#else
#define GN_FUNCTION          ""
#endif

//!
//! 连接两个名称
//!
#define GN_JOIN(s1, s2)         GN_JOIN_DIRECT(s1, s2)
#define GN_JOIN_DIRECT(s1, s2)  s1##s2 //!< Auxillary macro used by GN_JOIN

//@}

// *****************************************************************************
//                           End of basicDefines.h
// *****************************************************************************
#endif // __GN_BASE_BASICDEFINES_H__
