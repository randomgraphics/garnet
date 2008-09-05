#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#define GN_GFX2_D3D9_IMPL
#include "garnet/GNgfxsysD3D9.h"

#if GN_BUILD_RETAIL // disable PIX tag in retail build.
#define PIXPERF_BEGIN_EVENT_EX( color, name )
#define PIXPERF_END_EVENT()
#define PIXPERF_SET_MARKER_EX( color, name )
#define PIXPERF_SCOPE_EVENT_EX( color, name )
#else
#define PIXPERF_BEGIN_EVENT_EX( color, name )   D3DPERF_BeginEvent( color, GN_JOIN_DIRECT( L, name ) )
#define PIXPERF_END_EVENT()                     D3DPERF_EndEvent()
#define PIXPERF_SET_MARKER_EX( color, name )    D3DPERF_SetMarker( color, GN_JOIN_DIRECT( L, name ) )
#define PIXPERF_SCOPE_EVENT_EX( color, name )   PixPerfScopeEvent __pixScopeEvent__( color, GN_JOIN_DIRECT( L, name ) )
struct PixPerfScopeEvent
{
    PixPerfScopeEvent( D3DCOLOR color, const wchar_t * name )
    {
        D3DPERF_BeginEvent( color, name );
    }
    ~PixPerfScopeEvent()
    {
        D3DPERF_EndEvent();
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
#endif // __GN_PCH_H__
