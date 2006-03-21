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

#if GN_DEBUG && !defined(D3D_DEBUG_INFO)
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#if GN_XENON
#define D3DUSAGE_DYNAMIC D3DUSAGE_CPU_CACHED_MEMORY
#define D3DLOCK_DISCARD 0
inline BOOL D3DXDebugMute( BOOL ) { return FALSE; } // Fake D3DXDebugMute() for Xenon
#endif

#ifndef GN_RETAIL
#if GN_XENON
#define PIXPERF_BEGIN_EVENT( color, name )  PIXBeginNamedEvent( color, name )
#define PIXPERF_END_EVENT()                 PIXEndNamedEvent()
#define PIXPERF_SET_MARKER( color, name )   PIXSetMarker( color, name )
#else
#define PIXPERF_BEGIN_EVENT( color, name )  D3DPERF_BeginEvent( color, GN_JOIN( L, name ) )
#define PIXPERF_END_EVENT()                 D3DPERF_EndEvent()
#define PIXPERF_SET_MARKER( color, name )   D3DPERF_SetMarker( color, GN_JOIN( L, name ) )
#endif
#else
#define PIXPERF_BEGIN_EVENT( color, name )
#define PIXPERF_END_EVENT()
#define PIXPERF_SET_MARKER( color, name )
#endif

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
