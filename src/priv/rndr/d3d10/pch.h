#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNgfx.h"

#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>

//!
//! D3D10 error check routine
//!
#define GN_D3D10_CHECK_DO( func, something )                \
    if( true ) {                                            \
        HRESULT rr = func;                                  \
        if( FAILED(rr) )                                    \
        {                                                   \
            GN_ERROR( "D3D10 ERROR CODE : 0x%08X", rr );    \
            something                                       \
        }                                                   \
    } else void(0)

//!
//! D3D10 error check routine
//!
#if GN_DEBUG_BUILD
#define GN_D3D10_CHECK( func )         GN_D3D10_CHECK_DO( func, )
#else
#define GN_D3D10_CHECK( func )         func
#endif

//!
//! D3D10 error check routine
//!
#define GN_D3D10_CHECK_R( func )        GN_D3D10_CHECK_DO( func, return; )

//!
//! D3D10 error check routine
//!
#define GN_D3D10_CHECK_RV( func, rval ) GN_D3D10_CHECK_DO( func, return rval; )

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
