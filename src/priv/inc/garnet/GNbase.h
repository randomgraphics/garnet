#ifndef __GN_BASE_GNBASE_H__
#define __GN_BASE_GNBASE_H__
// *****************************************************************************
//! \file    GNbase.h
//! \brief   General header of base module
//! \author  chenlee(2005.4.10)
// *****************************************************************************

//!
//! namespace for garnet library
//!
namespace GN
{
    //!
    //! Implementation details of garnet library
    //!
    namespace detail {}
}

// user configurable compile switches
#include "base/userConfig.h"

// basic defines
#include "base/basicDefines.h"

// global pragma
#include "base/pragma.h"

// standard numeric types
#include "base/numeric.h"

// type traits
#include "base/typeTraits.h"

// debug macros and functions
#include "base/debug.h"

// memory management
#include "base/memory.h"
#include "base/allocator.h"

// string types
#include "base/string.h"

// log functions and macros
#include "base/log.h"

// exceptions
#include "base/exception.h"

// single/double linked list
#include "base/linkedList.h"

// array types
#include "base/array.h"

// code page routines
#include "base/codepage.h"

// handle manager
#include "base/handle.h"

// misc.
#include "base/misc.h"
#include "base/refCounter.h"
#include "base/stdClass.h"
#include "base/singleton.h"
#include "base/sharedLib.h"

// file system
#include "base/file.h"

// math library
#include "base/math.h"
#include "base/geometry.h"

// delegate & callback
#include "base/delegate.h"
#include "base/sigslot.h"

// variant class
#include "base/variant.h"

// registry class
#include "base/registry.h"

// plugin manager
#include "base/plugin.h"

// resource manager
#include "base/resourceManager.h"

// high resolution clock
#include "base/clock.h"

// light-weight performance profiler
#include "base/profiler.h"

// *****************************************************************************
//                           END of GNbase.h
// *****************************************************************************
#endif
