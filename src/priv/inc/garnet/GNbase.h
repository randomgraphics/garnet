#ifndef __GN_BASE_GNBASE_H__
#define __GN_BASE_GNBASE_H__
// *****************************************************************************
/// \file
/// \brief   General header of base module
/// \author  chenlee(2005.4.10)
// *****************************************************************************

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

// user configurable configurations
#include "base/config.h"

// basic defines
#include "base/basicDefines.h"

// platform specific configurations
#if GN_MSWIN
#   if GN_X64
#       include "base/platform.mswin.x64.h"
#   elif GN_X86
#       include "base/platform.mswin.x86.h"
#   endif
#elif GN_XENON
#   include     "base/platform.xenon.ppc.h"
#elif GN_POSIX
#   if GN_CYGWIN
#       include "base/platform.cygwin.x86.h"
#   elif GN_X64
#       include "base/platform.posix.x64.h"
#   elif GN_PPC
#       include "base/platform.posix.ppc.h"
#   elif GN_X86
#       include "base/platform.posix.x86.h"
#   endif
#endif

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

// log functions and macros
#include "base/log.h"

// memory management
#include "base/memory.h"

// single/double linked list
#include "base/linkedList.h"

// string types
#include "base/string.h"

// math library
#include "base/math.h"
#include "base/geometry.h"

// misc.
#include "base/misc.h"

// various auto pointers
#include "base/autoptr.h"

// basic sync. primitives
#include "base/sync.h"

// exceptions
#include "base/exception.h"

// delegate & callback
#include "base/delegate.h"
#include "base/sigslot.h"

// array types
#include "base/array.h"

// dictionary type
#include "base/dict.h"

// hash map
#include "base/hashmap.h"

// code page routines
#include "base/codepage.h"

// handle manager
#include "base/handle.h"

// misc.
#include "base/refCounter.h"
#include "base/blob.h"
#include "base/stdClass.h"
#include "base/singleton.h"
#include "base/sharedLib.h"

// file system
#include "base/file.h"
#include "base/filesys.h"

// a general tree structure
#include "base/tree.h"

// variant class
#include "base/variant.h"

// registry class
#include "base/registry.h"

// high resolution clock
#include "base/clock.h"

// light-weight performance profiler
#include "base/profiler.h"

// XML parser
#include "base/xml.h"

// thread routines
#include "base/thread.h"

// codepage
#include "base/codepage.h"

// *****************************************************************************
//                           END of GNbase.h
// *****************************************************************************
#endif
