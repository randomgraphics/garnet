#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "../gpupch.h"

#if GN_BUILD_DEBUG
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#include <xtl.h>
#include <xgraphics.h>
#include <xboxmath.h>
#if _XTL_VER < 1881
#error XDK build 1881 or newer is required.
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#if GN_BUILD_RETAIL // disable PIX tag in retail build.
#define PIXPERF_BEGIN_EVENT_EX( color, name )
#define PIXPERF_END_EVENT()
#define PIXPERF_SET_MARKER_EX( color, name )
#define PIXPERF_SCOPE_EVENT_EX( color, name )
#else
#define PIXPERF_BEGIN_EVENT_EX( color, name )   // PIXBeginNamedEvent( color, name )
#define PIXPERF_END_EVENT()                     // PIXEndNamedEvent()
#define PIXPERF_SET_MARKER_EX( color, name )    // PIXSetMarker( color, name )
#define PIXPERF_SCOPE_EVENT_EX( color, name )   // PixPerfScopeEvent( color, name )
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
#endif // GN_BUILD_RETAIL

#define PIXPERF_BEGIN_EVENT( name ) PIXPERF_BEGIN_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_SCOPE_EVENT( name ) PIXPERF_SCOPE_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_SET_MARKER( name )  PIXPERF_SET_MARKER_EX( D3DCOLOR_ARGB(255,255,0,0), name )
#define PIXPERF_FUNCTION_EVENT()    PIXPERF_SCOPE_EVENT_EX( D3DCOLOR_ARGB(255,255,0,0), GN_FUNCTION )

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__
