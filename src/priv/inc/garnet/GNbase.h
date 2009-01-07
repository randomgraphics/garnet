#ifndef __GN_BASE_GNBASE_H__
#define __GN_BASE_GNBASE_H__
// *****************************************************************************
/// \file
/// \brief   General header of base module
/// \author  chenlee(2005.4.10)
// *****************************************************************************

/*!
    Define compile time configuable switches:

    Note:
        - All build switches are prefixed with GN_BUILD_
        - For all these build switches, define to zero means off, non-zero means on.

    GN_BUILD_DEBUG              Enable debug code

    GN_BUILD_PROFILE            Enable profile code

    GN_BUILD_RETAIL             Enable retail code

    GN_BUILD_STATIC             Enable static (non-dll) build.

    GN_BUILD_ENABLE_GUARD       Enable function guard.
                                Note that disable this flag will also disable slow guard.

    GN_BUILD_ENABLE_SLOW_GUARD  Enable function guards in time-critical code.
                                Note that this switch is ignored, if GN_BUILD_ENABLE_GUARD is off.

    GN_BUILD_ENABLE_LOG         Enable runtime log.

    GN_BUILD_ENABLE_ASSERT      Enable assert macro

    GN_BUILD_LEFT_HAND          Enable left hand coordinate system. Right hand coordinate system will
                                be used if this switch is off.

    GN_BUILD_ENABLE_PIX         Enable PIX tag (on Windows and Xbox platform)
*/

/// Main switch of build flavor. Possible values are:
///
///   0 - retail build (default)
///   1 - profile build
///   2 - debug build
///   3 - static retail build
///   4 - static profile build
///   5 - static debug build
///
#ifndef GN_BUILD_VARIANT
#define GN_BUILD_VARIANT 0
#endif

// define switches for retail build
#if 0 == GN_BUILD_VARIANT
#define GN_BUILD_DEBUG 0
#define GN_BUILD_PROFILE 0
#define GN_BUILD_RETAIL 1
#define GN_BUILD_STATIC  0
#define GN_BUILD_ENABLE_GUARD 1
#define GN_BUILD_ENABLE_SLOW_GUARD 0
#define GN_BUILD_ENABLE_LOG 1
#define GN_BUILD_ENABLE_ASSERT 0
#define GN_BUILD_LEFT_HAND 0
#define GN_BUILD_ENABLE_PIX 0

// define switches for profile build
#elif 1 == GN_BUILD_VARIANT
#define GN_BUILD_DEBUG 0
#define GN_BUILD_PROFILE 1
#define GN_BUILD_RETAIL 0
#define GN_BUILD_STATIC  0
#define GN_BUILD_ENABLE_GUARD 1
#define GN_BUILD_ENABLE_SLOW_GUARD 0
#define GN_BUILD_ENABLE_LOG 1
#define GN_BUILD_ENABLE_ASSERT 0
#define GN_BUILD_LEFT_HAND 0
#define GN_BUILD_ENABLE_PIX 1

// define switches for debug build
#elif 2 == GN_BUILD_VARIANT
#define GN_BUILD_DEBUG 1
#define GN_BUILD_PROFILE 1
#define GN_BUILD_RETAIL 0
#define GN_BUILD_STATIC  0
#define GN_BUILD_ENABLE_GUARD 1
#define GN_BUILD_ENABLE_SLOW_GUARD 1
#define GN_BUILD_ENABLE_LOG 1
#define GN_BUILD_ENABLE_ASSERT 1
#define GN_BUILD_LEFT_HAND 0
#define GN_BUILD_ENABLE_PIX 1

// define switches for static retail build
#elif 3 == GN_BUILD_VARIANT
#define GN_BUILD_DEBUG 0
#define GN_BUILD_PROFILE 0
#define GN_BUILD_RETAIL 1
#define GN_BUILD_STATIC  1
#define GN_BUILD_ENABLE_GUARD 1
#define GN_BUILD_ENABLE_SLOW_GUARD 0
#define GN_BUILD_ENABLE_LOG 1
#define GN_BUILD_ENABLE_ASSERT 0
#define GN_BUILD_LEFT_HAND 0
#define GN_BUILD_ENABLE_PIX 0

// define switches for static profile build
#elif 4 == GN_BUILD_VARIANT
#define GN_BUILD_DEBUG 0
#define GN_BUILD_PROFILE 1
#define GN_BUILD_RETAIL 0
#define GN_BUILD_STATIC  1
#define GN_BUILD_ENABLE_GUARD 1
#define GN_BUILD_ENABLE_SLOW_GUARD 0
#define GN_BUILD_ENABLE_LOG 1
#define GN_BUILD_ENABLE_ASSERT 0
#define GN_BUILD_LEFT_HAND 0
#define GN_BUILD_ENABLE_PIX 1

// define switches for static debug build
#elif 5 == GN_BUILD_VARIANT
#define GN_BUILD_DEBUG 1
#define GN_BUILD_PROFILE 1
#define GN_BUILD_RETAIL 0
#define GN_BUILD_STATIC  1
#define GN_BUILD_ENABLE_GUARD 1
#define GN_BUILD_ENABLE_SLOW_GUARD 1
#define GN_BUILD_ENABLE_LOG 1
#define GN_BUILD_ENABLE_ASSERT 1
#define GN_BUILD_LEFT_HAND 0
#define GN_BUILD_ENABLE_PIX 1

#else
#error Unknown build flavor. GN_BUILD_VARIANT must be: 0-retail, 1-profile, 2-debug, 3-stret, 4-stprof, 5-stdbg
#endif

///
/// namespace for garnet library
///
namespace GN
{
    ///
    /// namespace for implementation details of garnet library
    ///
    namespace detail {}
}

// basic defines
#include "base/basicDefines.h"

// global pragma
#include "base/pragma.h"

// standard numeric types
#include "base/numeric.h"

// endian routine
#include "base/endian.h"

// type traits
#include "base/typeTraits.h"

// debug macros and functions
#include "base/debug.h"

// memory management
#include "base/memory.h"
#include "base/allocator.h"

// string types
#include "base/string.h"

// misc.
#include "base/misc.h"

// basic sync. primitives
#include "base/sync.h"

// log functions and macros
#include "base/log.h"

// exceptions
#include "base/exception.h"

// memory pool
#include "base/mempool.h"

// delegate & callback
#include "base/delegate.h"
#include "base/sigslot.h"

// single/double linked list
#include "base/linkedList.h"

// array types
#include "base/array.h"

// hash map
#include "base/hashmap.h"

// code page routines
#include "base/codepage.h"

// handle manager
#include "base/handle.h"

// misc.
#include "base/refCounter.h"
#include "base/stdClass.h"
#include "base/singleton.h"
#include "base/sharedLib.h"

// file system
#include "base/file.h"
#include "base/path.h"
#include "base/filesys.h"

// math library
#include "base/math.h"
#include "base/geometry.h"

// variant class
#include "base/variant.h"

// registry class
#include "base/registry.h"

// resource manager
#include "base/resourceManager.h"

// high resolution clock
#include "base/clock.h"

// light-weight performance profiler
#include "base/profiler.h"

// XML parser
#include "base/xml.h"

// thread routines
#include "base/thread.h"

// *****************************************************************************
//                           END of GNbase.h
// *****************************************************************************
#endif
