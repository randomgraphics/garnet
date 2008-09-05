// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "../rndr.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>

extern bool gD3D10EnablePixPerf; // global variable to switch on/off PIX perf calls.

#if 1//GN_BUILD_RETAIL // disable PIX tag in retail build.
#define PIXPERF_BEGIN_EVENT_EX( color, name )
#define PIXPERF_END_EVENT()
#define PIXPERF_SET_MARKER_EX( color, name )
#define PIXPERF_SCOPE_EVENT_EX( color, name )
#else
#define PIXPERF_BEGIN_EVENT_EX( color, name )   if( !gD3D10EnablePixPerf ) {} else D3DPERF_BeginEvent( color, GN_JOIN_DIRECT( L, name ) )
#define PIXPERF_END_EVENT()                     if( !gD3D10EnablePixPerf ) {} else D3DPERF_EndEvent()
#define PIXPERF_SET_MARKER_EX( color, name )    if( !gD3D10EnablePixPerf ) {} else D3DPERF_SetMarker( color, GN_JOIN_DIRECT( L, name ) )
#define PIXPERF_SCOPE_EVENT_EX( color, name )   PixPerfScopeEvent __pixScopeEvent__( color, GN_JOIN_DIRECT( L, name ) )
struct PixPerfScopeEvent
{
    PixPerfScopeEvent( D3DCOLOR color, const wchar_t * name )
    {
        if( gD3D10EnablePixPerf ) D3DPERF_BeginEvent( color, name );
    }
    ~PixPerfScopeEvent()
    {
        if( gD3D10EnablePixPerf ) D3DPERF_EndEvent();
    }
};
#endif // GN_BUILD_RETAIL

#define PIXPERF_BEGIN_EVENT( name ) PIXPERF_BEGIN_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_SCOPE_EVENT( name ) PIXPERF_SCOPE_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_SET_MARKER( name )  PIXPERF_SET_MARKER_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_FUNCTION_EVENT()    PIXPERF_SCOPE_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), GN_FUNCTION )

// *****************************************************************************
//                                     EOF
// *****************************************************************************
