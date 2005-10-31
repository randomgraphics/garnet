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

#if GN_DEBUG
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#if GN_XENON
#include <xtl.h>
#include <xgraphics.h>
#include <xboxmath.h>
#elif GN_PC
#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include "garnet/base/pragma.h"

//!
//! d3d module fatal error
//!
#define GND3D_FATAL GN_FATAL

//!
//! d3d module error log macro
//!
#define GND3D_ERROR GN_ERROR

//!
//! d3d module warning log macro
//!
#define GND3D_WARN GN_WARN

//!
//! d3d module informational log macro
//!
#define GND3D_INFO GN_INFO

//!
//! d3d module tracing macro
//!
#define GND3D_TRACE GN_TRACE

//!
//! DX error check routine
//!
#define DX_CHECK_DO( func, something )              \
    if( true ) {                                    \
        HRESULT rr = func;                          \
        if( FAILED(rr) )                            \
        {                                           \
            GND3D_ERROR( DXGetErrorString9A(rr) );  \
            something                               \
        }                                           \
    } else void(0)

//!
//! DX error check routine
//!
#if GN_DEBUG
#define DX_CHECK( func )         DX_CHECK_DO( func, )
#else
#define DX_CHECK( func )         func
#endif

//!
//! DX error check routine
//!
#define DX_CHECK_R( func )        DX_CHECK_DO( func, return; )

//!
//! DX error check routine
//!
#define DX_CHECK_RV( func, rval ) DX_CHECK_DO( func, return rval; )

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_GFXCOMMON_PCH_H__
