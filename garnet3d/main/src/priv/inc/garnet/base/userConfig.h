#ifndef __GN_BASE_USERCONFIG_H__
#define __GN_BASE_USERCONFIG_H__
// *****************************************************************************
//! \file    userConfig.h
//! \brief   User configuable compile switches
//! \author  chenlee (2005.7.24)
// *****************************************************************************

// GN_DEBUG_BUILD
// ==============
//!
//! Enable debug build. Default is off.
//!
#ifndef GN_DEBUG_BUILD
#define GN_DEBUG_BUILD 0
#endif

// GN_PROFILE_BUILD
// ================
//!
//! Enable profile build. Default is on.
//!
#ifndef GN_PROFILE_BUILD
#define GN_PROFILE_BUILD 0
#endif

// GN_RETAIL_BUILD
// ===============
//!
//! Enable retail build. Default is on.
//!
#ifndef GN_RETAIL_BUILD
#define GN_RETAIL_BUILD 1
#endif

//
// check for invalid build type
//
#if 1 != (GN_DEBUG_BUILD + GN_PROFILE_BUILD + GN_RETAIL_BUILD)
#error Invalid build type
#endif

// GN_STATIC
// =========
//!
//! Enable static build. Default is off.
//!
#ifndef GN_STATIC
#define GN_STATIC 0
#endif

// GN_ENABLE_INLINE
// ================
//!
//! Enable inline function build. Enabled by default in non-debug build.
//!
#ifndef GN_ENABLE_INLINE
#define GN_ENABLE_INLINE (!GN_DEBUG_BUILD)
#endif

// GN_ENABLE_GUARD
// ================
//!
//! Enable function guard. Default is on.
//!
//! By default, function guard is enabled even in release build.
//!
//! Disable this flag will disable slow function guard as well.
//!
#ifndef GN_ENABLE_GUARD
#define GN_ENABLE_GUARD 1
#endif

// GN_ENABLE_SLOW_GUARD
// ====================
//!
//! Enable slow function guard. Default is off.
//!
//! We call function guards used in time-critical functions as slow function
//! guard, or simplly "slow guard", because those guards may incur heavy
//! performance penalty to application. By default, slow guards is only enabled
//! in debug build, and disabled in release build.
//!
//! Note that this flag has no effection when GN_ENABLE_GUARD is off.
//!
#ifndef GN_ENABLE_SLOW_GUARD
#define GN_ENABLE_SLOW_GUARD 0
#endif

// GN_DEFAULT_LOCALE
// =================
//!
//! Define default locale. Default is CHS (Simplified Chinese).
//!
#ifndef GN_LOCALE
#define GN_LOCALE "CHS"
#endif

// GN_LEFT_HAND
// ============
//!
//! 是否使用左手坐标系，缺省使用右手系
//!
#ifndef GN_LEFT_HAND
#define GN_LEFT_HAND 0
#endif

// *****************************************************************************
//                           End of userConfig.h
// *****************************************************************************
#endif // __GN_BASE_USERCONFIG_H__
