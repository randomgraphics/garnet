#ifndef __GN_BASE_USERCONFIG_H__
#define __GN_BASE_USERCONFIG_H__
// *****************************************************************************
//! \file    userConfig.h
//! \brief   User configuable compile switches
//! \author  chenlee (2005.7.24)
// *****************************************************************************

// GN_DEBUG
// ========
//!
//! Enable debug build. Default is false.
//!
#ifndef GN_DEBUG
#define GN_DEBUG 0
#endif

// GN_STATIC
// =========
//!
//! Enable static build. Default is false.
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
#define GN_ENABLE_INLINE (!GN_DEBUG)
#endif

// GN_DISABLE_GUARD
// ================
//!
//! Disable function guard. Default is false.
//!
//! Note that function guard is enabled by default, even in release build.
//!
//! This flag will also disable slow function guard.
//!
#ifndef GN_DISABLE_GUARD
#define GN_DISABLE_GUARD 0
#endif

// GN_ENABLE_SLOW_GUARD
// ====================
//!
//! Enable slow function guard. Default is false.
//!
//! We call function guards used in time-critical functions as slow function
//! guard, or simplly "slow guard", because those guards may incur heavy
//! performance penalty to application. By default, slow guards is only enabled
//! in debug build, and disabled in release build.
//!
//! To enable slow guards in release build, define this flag to non-zero value.
//!
//! Note that this flag has no effection when GN_DISABLE_GUARD is non-zero.
//!
#ifndef GN_ENABLE_SLOW_GUARD
#define GN_ENABLE_SLOW_GUARD 0
#endif

// GN_ENABLE_PROFILE
// =================
//!
//! Enable performance profiler. Disabled by default.
//!
#ifndef GN_ENABLE_PROFILE
#define GN_ENABLE_PROFILE 0
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
