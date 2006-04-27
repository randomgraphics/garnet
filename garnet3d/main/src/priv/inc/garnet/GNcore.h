#ifndef __GN_CORE_GNCORE_H__
#define __GN_CORE_GNCORE_H__
// *****************************************************************************
//! \file    GNcore.h
//! \brief   Public interface of garnet core module.
//! \author  chenlee (2005.8.7)
// *****************************************************************************

#include "GNbase.h"

namespace GN
{
    //!
    //! namespace for garnet core module
    //!
    namespace core
    {
        //!
        //! Global log signal
        //!
        extern GN_PUBLIC Signal2<void,const LogDesc &, const char *> gSigLog;
    }
}

// *****************************************************************************
//                           End of GNcore.h
// *****************************************************************************
#endif // __GN_CORE_GNCORE_H__
