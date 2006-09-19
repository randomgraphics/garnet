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

#if GN_DEBUG_BUILD
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#if GN_XENON
#include <xtl.h>
#include <xgraphics.h>
#include <xboxmath.h>
#if _XTL_VER < 1881
#error XDK build 1881 or newer is required.
#endif
#elif GN_PC
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#if GN_DEBUG_BUILD && !defined(D3D_DEBUG_INFO)
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#if GN_XENON
inline BOOL D3DXDebugMute( BOOL ) { return FALSE; } // Fake D3DXDebugMute() for Xenon
#ifndef D3DLOCK_DISCARD
#define D3DLOCK_DISCARD 0
#endif
#ifndef D3DUSAGE_DYNAMIC
#define D3DUSAGE_DYNAMIC 0
#endif
#endif

#if GN_RETAIL_BUILD // disable PIX tag in retail build.
#define PIXPERF_BEGIN_EVENT_EX( color, name )
#define PIXPERF_END_EVENT()
#define PIXPERF_SET_MARKER_EX( color, name )
#define PIXPERF_SCOPE_EVENT_EX( color, name )
#else
#if GN_XENON
#define PIXPERF_BEGIN_EVENT_EX( color, name )   //PIXBeginNamedEvent( color, name )
#define PIXPERF_END_EVENT()                     //PIXEndNamedEvent()
#define PIXPERF_SET_MARKER_EX( color, name )    //PIXSetMarker( color, name )
#define PIXPERF_SCOPE_EVENT_EX( color, name )   //PixPerfScopeEvent( color, name )
struct PixPerfScopeEvent
{
    PixPerfScopeEvent( D3DCOLOR color, const char * name )
    {
        PIXBeginNamedEvent( color, name );
    }
    ~PixPerfScopeEvent()
    {
        PIXEndNamedEvent();
    }
};
#else // GN_XENON
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
#endif // GN_XENON
#endif // GN_RETAIL_BUILD

#define PIXPERF_BEGIN_EVENT( name ) PIXPERF_BEGIN_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_SCOPE_EVENT( name ) PIXPERF_SCOPE_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_SET_MARKER( name )  PIXPERF_SET_MARKER_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_FUNCTION_EVENT()    PIXPERF_SCOPE_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), GN_FUNCTION )

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
