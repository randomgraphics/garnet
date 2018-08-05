// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "../gpupch.h"
#include "garnet/GNd3d.h"
#include <d3dcompiler.h>

extern bool gD3D11EnablePixPerf; // global variable to switch on/off PIX perf calls.

// D3D11_1 perf markers
#if (GN_BUILD_DEBUG_ENABLED || GN_BUILD_PROFILING_ENABLED) && GN_BUILD_HAS_D3D11_1
#define PIXPERF_BEGIN_EVENT_EX( context, color, name )   if( gD3D11EnablePixPerf ) { D3DUdaHelper uda(context); uda.BeginEvent( name ); } else void(0)
#define PIXPERF_END_EVENT( context )                     if( gD3D11EnablePixPerf ) { D3DUdaHelper uda(context); uda.EndEvent(); } else void(0)
#define PIXPERF_SET_MARKER_EX( context, color, name )    if( gD3D11EnablePixPerf ) { D3DUdaHelper uda(context); uda.SetMarker( name ); } else void(0)
#define PIXPERF_SCOPE_EVENT_EX( context, color, name )   PixPerfScopeEvent __pixScopeEvent__( context, name )
struct D3DUdaHelper
{
    ID3DUserDefinedAnnotation * uda;

    D3DUdaHelper( ID3D11DeviceContext * cxt = nullptr ) : uda(nullptr)
    {
        SetContext( cxt );
    }

    D3DUdaHelper( ID3D11DeviceContext & cxt ) : uda(nullptr)
    {
        SetContext( &cxt );
    }

    ~D3DUdaHelper()
    {
        SetContext( nullptr );
    }

    void SetContext( ID3D11DeviceContext * cxt )
    {
        if (uda) uda->Release(), uda = nullptr;
        if (cxt)
        {
            if (FAILED(cxt->QueryInterface<ID3DUserDefinedAnnotation>(&uda)))
            {
                uda = nullptr;
            }
        }
    }

    INT BeginEvent( LPCWSTR name )
    {
        return uda ? uda->BeginEvent(name) : -1;
    }

    INT EndEvent()
    {
        return uda ? uda->EndEvent() : -1;
    }

    void SetMarker( LPCWSTR name )
    {
        if (uda) uda->SetMarker(name);
    }
};
struct PixPerfScopeEvent
{
    D3DUdaHelper uda;

    PixPerfScopeEvent( ID3D11DeviceContext * context, const wchar_t * name )
    {
        if( gD3D11EnablePixPerf )
        {
            uda.SetContext( context );
            uda.BeginEvent( name );
        }
    }

    PixPerfScopeEvent( ID3D11DeviceContext & context, const wchar_t * name )
    {
        if( gD3D11EnablePixPerf )
        {
            uda.SetContext( &context );
            uda.BeginEvent( name );
        }
    }

    ~PixPerfScopeEvent()
    {
        if( gD3D11EnablePixPerf )
        {
            uda.EndEvent();
        }
    }
};

// D3D9 perf markers
#elif (GN_BUILD_DEBUG_ENABLED || GN_BUILD_PROFILING_ENABLED) && GN_BUILD_HAS_D3D9
#include <d3d9.h>
#define PIXPERF_BEGIN_EVENT_EX( context, color, name )   if( !gD3D11EnablePixPerf ) {} else D3DPERF_BeginEvent( color, name )
#define PIXPERF_END_EVENT( context )                     if( !gD3D11EnablePixPerf ) {} else D3DPERF_EndEvent()
#define PIXPERF_SET_MARKER_EX( context, color, name )    if( !gD3D11EnablePixPerf ) {} else D3DPERF_SetMarker( color, name )
#define PIXPERF_SCOPE_EVENT_EX( context, color, name )   PixPerfScopeEvent __pixScopeEvent__( color, name )
struct PixPerfScopeEvent
{
    PixPerfScopeEvent( UINT color, const wchar_t * name )
    {
        if( gD3D11EnablePixPerf ) D3DPERF_BeginEvent( color, name );
    }
    ~PixPerfScopeEvent()
    {
        if( gD3D11EnablePixPerf ) D3DPERF_EndEvent();
    }
};

#else
#define PIXPERF_BEGIN_EVENT_EX( context, color, name )
#define PIXPERF_END_EVENT( context )
#define PIXPERF_SET_MARKER_EX( context, color, name )
#define PIXPERF_SCOPE_EVENT_EX( context, color, name )
#endif

#define PIXPERF_BEGIN_EVENT( context, name ) PIXPERF_BEGIN_EVENT_EX( context, D3DCOLOR_ARGB(255,255,0,0), GN_JOIN_DIRECT(L, name) )
#define PIXPERF_SCOPE_EVENT( context, name ) PIXPERF_SCOPE_EVENT_EX( context, D3DCOLOR_ARGB(255,255,0,0), GN_JOIN_DIRECT(L, name) )
#define PIXPERF_SET_MARKER( context, name )  PIXPERF_SET_MARKER_EX( context, D3DCOLOR_ARGB(255,255,0,0), GN_JOIN_DIRECT(L, name) )
#define PIXPERF_FUNCTION_EVENT( context )    PIXPERF_SCOPE_EVENT_EX( context, D3DCOLOR_ARGB(255,255,0,0), GN_JOIN(L, GN_FUNCTION) )

// *****************************************************************************
//                                     EOF
// *****************************************************************************
