#ifndef __GN_GFX_PCH_H__
#define __GN_GFX_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.9)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNgfx.h"

#if GN_MSWIN
#define NOMINMAX
#include <windows.h>
#endif

#include <exception>
#include <malloc.h>
#include <stdlib.h>

#include "garnet/base/pragma.h"

//!
//! gfx module fatal error
//!
#define GNGFX_FATAL GN_FATAL

//!
//! gfx module error log macro
//!
#define GNGFX_ERROR GN_ERROR

//!
//! gfx module warning log macro
//!
#define GNGFX_WARN GN_WARN

//!
//! gfx module informational log macro
//!
#define GNGFX_INFO GN_INFO

//!
//! gfx module tracing macro
//!
#define GNGFX_TRACE GN_TRACE


// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_GFX_PCH_H__
