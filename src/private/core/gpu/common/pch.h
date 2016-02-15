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
#elif GN_POSIX
#include <X11/Xlib.h>
#endif

#ifdef HAS_CG
#include <Cg/cg.h>
#endif

#include <exception>
#include <stdlib.h>

// *****************************************************************************
//                                     EOF
// *****************************************************************************
