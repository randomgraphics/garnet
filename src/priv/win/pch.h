#ifndef __GN_BASE_PCH_H__
#define __GN_BASE_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   precompiled header
// \author  chenlee (2005.4.18)
// *****************************************************************************

#include "garnet/GNwin.h"

#if GN_XENON
#include <xtl.h>
#elif GN_MSWIN
#define NOMINMAX
#include <windows.h>
#elif GN_POSIX
#include <X11/Xlib.h>
#include <unistd.h>
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <wchar.h>
#include <math.h>
#include <cwchar>
#include <string>
#include <vector>
#include <exception>
#include <iostream>

//!
//! win module fatal error
//!
#define GNWIN_FATAL GN_FATAL

//!
//! win module error log macro
//!
#define GNWIN_ERROR GN_ERROR

//!
//! win module warning log macro
//!
#define GNWIN_WARN GN_WARN

//!
//! win module informational log macro
//!
#define GNWIN_INFO GN_INFO

//!
//! win module tracing macro
//!
#define GNWIN_TRACE GN_TRACE

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_BASE_PCH_H__
