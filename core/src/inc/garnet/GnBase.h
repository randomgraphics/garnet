#ifndef __GN_BASE_GNBASE_H__
#define __GN_BASE_GNBASE_H__
// ****************************************************************************
//! \file    GnBase.h
//! \brief   General header of base module
//! \author  chenlee(2005.4.10)
// ****************************************************************************

// ****************************************************************************
//! \name                 user configable compile switches
// ****************************************************************************

//@{

// GN_DEBUG
// ========
//
// Enable debug build. Default is false.
//
#ifndef GN_DEBUG
#define GN_DEBUG 0
#endif

// GN_STATIC
// =========
//
// Enable static build. Default is false.
//
#ifndef GN_STATIC
#define GN_STATIC 0
#endif

// GN_ENABLE_INLINE
// ================
//
// Enable inline function build. Enabled by default in non-debug build.
//
#ifndef GN_ENABLE_INLINE
#define GN_ENABLE_INLINE (!GN_DEBUG)
#endif

// GN_DISABLE_GUARD
// ================
//
// Disable function guard. Default is false.
//
// Note that function guard is enabled by default, even in release build.
//
// This flag will also disable slow function guard.
//
#ifndef GN_DISABLE_GUARD
#define GN_DISABLE_GUARD 0
#endif

// GN_ENABLE_SLOW_GUARD
// ====================
//
// Enable slow function guard. Default is false.
//
// We call function guards used in time-critical functions as slow function
// guard, or simplly "slow guard". By default, slow guards is enabled in
// debug build and disabled in release build.
//
// To enable slow guards in release build, define this flag.
//
// Note that you can disable slow guard in both debug and release build by
// defining GN_DISABLE_GUARD.
//
#ifndef GN_ENABLE_SLOW_GUARD
#define GN_ENABLE_SLOW_GUARD 0
#endif

// GN_ENABLE_PROFILE
// =================
//
// Enable performance profiler. Disabled by default.
//
#ifndef GN_ENABLE_PROFILE
#define GN_ENABLE_PROFILE 0
#endif

// GN_DEFAULT_LOCALE
// =================
//
// Define default locale. Default is CHS (Simplified Chinese).
//
#ifndef GN_LOCALE
#define GN_LOCALE "CHS"
#endif

// GN_LEFT_HAND
// ============
//
// 是否使用左手坐标系
//
#ifndef GN_LEFT_HAND
#define GN_LEFT_HAND 0
#endif

//@}

// *****************************************************************************
//! \name           Global defines
// *****************************************************************************

//@{

#define GN_MSVC 0
#define GN_ICL  0
#define GN_GCC  0
#define GN_BCB  0

// 辨识编译器
#if defined(_MSC_VER) && !defined(__ICL)
#undef GN_MSVC
#define GN_MSVC 1
#define GN_COMPILER "msvc"
#pragma warning( disable : 4127 ) // 条件表达式是常数
#pragma warning( disable : 4201 ) // nameless struct/union
#pragma warning( disable : 4251 ) // need DLL interface ...
#pragma warning( disable : 4511 ) // can't generate copy constructor
#pragma warning( disable : 4512 ) // can't generate assignment operator
#pragma warning( disable : 4714 ) // 标记为 __forceinline 的函数未内联
#pragma warning( disable : 4786 ) // symbol trunc to 255 ...
#if defined(NDEBUG)
#pragma warning( disable : 4702 ) // 无法访问的代码
#endif
#elif defined(__ICL)
#undef GN_MSVC
#undef GN_ICL
#define GN_MSVC 1                  // treat intel compiler as VC compiler
#define GN_ICL  1
#define GN_COMPILER "icl"
#pragma warning(disable:9)        // nested comment is not allowed
#pragma warning(disable:271)      // trailing comma is nonstandard
#pragma warning(disable:279)      // controlling expression is constant
#pragma warning(disable:373)      // xxx is inaccessible
#pragma warning(disable:383)      // reference to temporary
#pragma warning(disable:424)      // extra ";" ignored
#pragma warning(disable:444)      // destructor for xxx is not virtual
#pragma warning(disable:858)      // type qualifier on return type is meaningless
#pragma warning(disable:981)      // operands are evaluated in unspecified order
#pragma warning(disable:985)      // debug symbol greater than 255 chars
#pragma warning(disable:1418)     // external definition with no prior declaration overridden
#pragma warning(disable:1419)     // external declaration in primary source file
#pragma warning(disable:1572)     // floating-point comparison
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
#define GN_WIN32  0
#define GN_CYGWIN 0
#define GN_POSIX  0
#if defined( _WIN32 ) || defined( WIN32 )      // Win32
//! WIN32 platform
#undef GN_WIN32
#define GN_WIN32 1
#define GN_PLATFORM "win32"
#elif defined(__CYGWIN__)
//! Cygwin platform
#undef GN_CYGWIN
#undef GN_POSIX
#define GN_CYGWIN 1
#define GN_POSIX  1 // cygwin also provides some posix compabilities
#define GN_PLATFORM "cygwin"
#elif defined( __unix ) || defined( __unix__ )
//! posix-like platform
#undef GN_POSIX
#define GN_POSIX 1
#define GN_PLATFORM "posix"
#else
#error "unknown platform!"
#endif

// 定义语言属性
#if GN_MSVC
#pragma setlocale( GN_LOCALE )
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

// export/import function tag
#if GN_MSVC && !GN_STATIC
#define GN_IMPORT         __declspec(dllimport)
#define GN_EXPORT         __declspec(dllexport)
#else
#define GN_IMPORT
#define GN_EXPORT
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
#define GN_CONCATNATE(s1, s2)         GN_CONCATNATE_DIRECT(s1, s2)
#define GN_CONCATNATE_DIRECT(s1, s2)  s1##s2

//@}

//!
//! namespace for garnet library
//!
namespace GN{}

// define numeric types
#if GN_MSVC
#include "base/stdint.h"
#else
#include <stdint.h>
#endif

// macros and functions for debug
#include "base/debug.h"

// log functions and macros
#include "base/log.h"

// exceptions
#include "base/exception.h"

// string types
#include "base/string.h"

// file types
#include "base/file.h"

// math library
#include "base/math.h"
#include "base/geometry.h"

// misc.
#include "base/misc.h"

// functor & callback
#include "base/functor.h"
#include "base/sigslot.h"

// graphics
#include "base/graphics.h"
//#include "base/primitive.h"
//#include "base/vertex.h"
//#include "base/rsblock.h"

/* standard class definition
#include "garnet/types/_types_stdclass.h"

// reference counted auto pointer class
#include "garnet/types/_types_refcounter.h"

// reference counted auto pointer class
#include "garnet/types/_types_autoscope.h"

// singleton class
#include "garnet/types/_types_singleton.h"

*/
// ****************************************************************************
//                           END of GnBase.h
// ****************************************************************************
#endif
