#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#define GN_CORE_INTERNAL
#include "garnet/GNbase.h"
#include "garnet/GNwin.h"

#if GN_XBOX2
#include <xtl.h>
#elif GN_WINPC
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif HAS_X11
#include <X11/Xlib.h>
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_PCH_H__
