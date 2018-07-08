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
    /// namespace for declaration of internal classes and functions that are reserved
    /// for garnet library.
    ///
    namespace internal {}
}

// garnet build specific defines.
#include "base/build.h"

// build specific defines.
#include "base/config.h"

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

// log functions and macros
#include "base/log.h"

// memory management
#include "base/memory.h"

// basic sync. primitives
#include "base/sync.h"

// single/double linked list
#include "base/link.h"

// smart pointer
#include "base/smartptr.h"

// string types
#include "base/string.h"

// math library
#include "base/math.h"
#include "base/geometry.h"

// misc.
#include "base/misc.h"

// various auto pointers
#include "base/autoptr.h"

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
#include "base/blob.h"
#include "base/stdClass.h"
#include "base/sharedLib.h"

// file system
#include "base/file.h"
#include "base/path.h"
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

// codepage
#include "base/codepage.h"

// *****************************************************************************
//                           END of GNbase.h
// *****************************************************************************
#endif
