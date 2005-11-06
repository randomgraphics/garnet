#ifndef __GN_GFXCOMMON_PCH_H__
#define __GN_GFXCOMMON_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNgfx.h"

#if GN_MSWIN
#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#endif

#include <glew.h>
#if GN_MSWIN
#include <wglew.h>
#else
#include <glxew.h>
#endif
#include <GL/glu.h>

#include "garnet/base/pragma.h"

//!
//! ogl module fatal error
//!
#define GNGFX_FATAL GN_FATAL

//!
//! ogl module error log macro
//!
#define GNGFX_ERROR GN_ERROR

//!
//! ogl module warning log macro
//!
#define GNGFX_WARN GN_WARN

//!
//! ogl module informational log macro
//!
#define GNGFX_INFO GN_INFO

//!
//! ogl module tracing macro
//!
#define GNGFX_TRACE GN_TRACE

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_GFXCOMMON_PCH_H__
