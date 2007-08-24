#ifndef __GN_BASE_USERCONFIG_H__
#define __GN_BASE_USERCONFIG_H__
// *****************************************************************************
/// \file
/// \brief   User configuable compile switches.
///          Please modify this file directly or define these macros in your
///          build script to meet your own requirements.
/// \author  chenlee (2005.7.24)
// *****************************************************************************

// GN_BUILD_VARIANT
// ================
///
/// Main switch of build variant. Possible values are:
///
///   0 - retail build (default)
///   1 - profile build
///   2 - debug build
///
#ifndef GN_BUILD_VARIANT
#define GN_BUILD_VARIANT 0
#else
// check for invalid build type
#if (GN_BUILD_VARIANT<0) || (GN_BUILD_VARIANT>2)
#error GN_BUILD_VARIANT must be: 0-retail, 1-profile, 2-debug
#endif
#endif

// GN_STATIC
// =========
///
/// Enable static build. Default is off.
///
#ifndef GN_STATIC
#define GN_STATIC 0
#endif

// GN_ENABLE_GUARD
// ================
///
/// Enable function guard. Default is on.
///
/// By default, function guard is enabled even in release build.
///
/// Disable this flag will disable slow function guard as well.
///
#ifndef GN_ENABLE_GUARD
#define GN_ENABLE_GUARD 1
#endif

// GN_ENABLE_SLOW_GUARD
// ====================
///
/// Enable slow function guard. Default is off.
///
/// We call function guards used in time-critical functions as slow function
/// guard, or simplly "slow guard", because those guards may incur heavy
/// performance penalty to application. By default, slow guards is only enabled
/// in debug build, and disabled in release build.
///
/// Note that this flag has no effection when GN_ENABLE_GUARD is off.
///
#ifndef GN_ENABLE_SLOW_GUARD
#define GN_ENABLE_SLOW_GUARD 0
#endif

// GN_ENABLE_LOG
// =============
///
/// Enable runtime log. Default is on.
///
#ifndef GN_ENABLE_LOG
#define GN_ENABLE_LOG 1
#endif

// GN_LEFT_HAND
// ============
///
/// 是否使用左手坐标系，缺省使用右手系
///
#ifndef GN_LEFT_HAND
#define GN_LEFT_HAND 0
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_USERCONFIG_H__
