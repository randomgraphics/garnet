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
#endif

#ifndef GN_RETAIL
#define PIXPERF_BEGIN_EVENT( color, name )  D3DPERF_BeginEvent( color, name )
#define PIXPERF_END_EVENT()                 D3DPERF_EndEvent()
#define PIXPERF_SET_MARKER( color, name )   D3DPERF_SetMarker( color, name )
#define PIXPERF_SET_REGION( color, name )   D3DPERF_SetRegion( color, name )
#define PIXPERF_QUERY_REPEAT_FRAME()        D3DPERF_QueryRepeatFrame()
#define PIXPERF_SET_OPTIONS( options )      D3DPERF_SetOptions( options )
#define PIXPERF_GET_STATUS( color, name )   D3DPERF_GetStatus
#else
#define PIXPERF_BEGIN_EVENT( color, name )
#define PIXPERF_END_EVENT()
#define PIXPERF_SET_MARKER( color, name )
#define PIXPERF_SET_REGION( color, name )
#define PIXPERF_QUERY_REPEAT_FRAME()
#define PIXPERF_SET_OPTIONS( options )
#define PIXPERF_GET_STATUS( color, name )
#endif

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
