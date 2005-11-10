#ifndef __GN_INPUT_PCH_H__
#define __GN_INPUT_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNinput.h"

#if GN_MSWIN
#define NOMINMAX
#include <windows.h>
#include <dinput.h>
#include <dxerr9.h>
#include <zmouse.h>  // for WM_MOUSEWHEEL
#elif GN_POSIX
#include <X11/Xlib.h>
#endif

#include <exception>
#include <malloc.h>
#include <stdlib.h>

#include "garnet/base/pragma.h"

//!
//! gfx module fatal error
//!
#define GNINPUT_FATAL GN_FATAL

//!
//! gfx module error log macro
//!
#define GNINPUT_ERROR GN_ERROR

//!
//! gfx module warning log macro
//!
#define GNINPUT_WARN GN_WARN

//!
//! gfx module informational log macro
//!
#define GNINPUT_INFO GN_INFO

//!
//! gfx module tracing macro
//!
#define GNINPUT_TRACE GN_TRACE

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_INPUT_PCH_H__
