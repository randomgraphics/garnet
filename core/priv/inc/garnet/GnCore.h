#ifndef __GN_CORE_GNCORE_H__
#define __GN_CORE_GNCORE_H__
// *****************************************************************************
//! \file    GnCore.h
//! \brief   Public interface of garnet core module.
//! \author  chenlee (2005.8.7)
// *****************************************************************************

#include "GnBase.h"

//! \name prof macros
//@{
#ifdef GN_ENABLE_PROFILE
//!
//! start a profile timer
//!
#define GN_PROF(name) ::GN::core::ScopeTimer GN_JOIN(__prof_,name)( ::GN::core::gProfilerMgr, #name )
//!
//! stop a previously defined profile timer
//!
//! Normally, you don't need to use this macro, because profile timer
//! will stop automatically when out of its life scope.
//!
#define GN_PROF_END(name) GN_JOIN(__prof_,name).end()
#else
#define GN_PROF(name)
#define GN_PROF_END(name)
#endif
//@}

namespace GN
{
    //!
    //! namespace for garnet core module
    //!
    namespace core
    {
        //!
        //! Global plugin manager
        //!
        GN_PUBLIC PluginManager   gPluginMgr;

#if GN_ENABLE_PROFILE
        //!
        //! Global profiler manager
        //!
        GN_PUBLIC ProfilerManager gProfiler;
#endif
    }
}

// *****************************************************************************
//                           End of GnCore.h
// *****************************************************************************
#endif // __GN_CORE_GNCORE_H__
