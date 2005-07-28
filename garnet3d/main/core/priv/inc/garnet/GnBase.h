#ifndef __GN_BASE_GNBASE_H__
#define __GN_BASE_GNBASE_H__
// ****************************************************************************
//! \file    GnBase.h
//! \brief   General header of base module
//! \author  chenlee(2005.4.10)
// ****************************************************************************

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

// user configable compile switches
#include "base/userConfig.h"

// basic defines
#include "base/basicDefines.h"

// global pragma
#include "base/pragma.h"

// standard numeric types
#if GN_MSVC
#include "base/stdint.h"
#else
#include <stddef.h>
#include <wchar.h>
#include <stdint.h>
#endif

// macros and functions for debug
#include "base/debug.h"

// log functions and macros
#include "base/log.h"

// exceptions
#include "base/exception.h"

// memory management
#include "base/memory.h"

// string types
#include "base/string.h"

// handle manager
#include "base/handle.h"

// misc.
#include "base/misc.h"

// file system
#include "base/file.h"

// math library
#include "base/math.h"
#include "base/geometry.h"

// functor & callback
#include "base/functor.h"
#include "base/sigslot.h"

// graphics
#include "base/graphics.h"

// registry class
#include "base/registry.h"

// ****************************************************************************
//                           END of GnBase.h
// ****************************************************************************
#endif
