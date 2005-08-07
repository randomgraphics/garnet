#ifndef __GN_CORE_GNCORE_H__
#define __GN_CORE_GNCORE_H__
// *****************************************************************************
//! \file    GnCore.h
//! \brief   Public interface of garnet core module.
//! \author  chenlee (2005.8.7)
// *****************************************************************************

#include "GnBase.h"

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
    }
}

// *****************************************************************************
//                           End of GnCore.h
// *****************************************************************************
#endif // __GN_CORE_GNCORE_H__
