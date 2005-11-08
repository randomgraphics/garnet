#ifndef __GN_WIN_GNWIN_H__
#define __GN_WIN_GNWIN_H__
// *****************************************************************************
//! \file    GNwin.h
//! \brief   Main interface of win module.
//! \author  chenlee (2005.10.31)
// *****************************************************************************

#include "GNbase.h"

#if GN_MSWIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif GN_POSIX
#include <X11/Xlib.h>
#endif

namespace GN
{
    //!
    //! Namespace for win module
    //!
    namespace win
    {
    }
}

#include "win/winTypes.h"
#include "win/winUtils.h"
#include "win/mswWindow.h"
#include "win/xWindow.h"

// *****************************************************************************
//                           End of GNwin.h
// *****************************************************************************
#endif // __GN_WIN_GNWIN_H__
