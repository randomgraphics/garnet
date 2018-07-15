// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "../gpupch.h"

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

#include <exception>
#include <stdlib.h>

// *****************************************************************************
//                                     EOF
// *****************************************************************************
