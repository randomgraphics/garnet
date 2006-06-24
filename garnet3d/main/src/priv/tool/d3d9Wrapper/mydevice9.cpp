#include "pch.h"
#include "mydevice9.h"
#include "myd3d9.h"

// *****************************************************************************
// MyDevice9
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::create(
    MyD3D9 * d3d,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters )
{
    GN_GUARD;

    GN_INFO( "create Direct3DDevice9 object" );

    // store d3d pointer
    GN_ASSERT( d3d );
    GN_ASSERT( !mD3D9 );
    mD3D9 = d3d;
    d3d->AddRef();

    // create device
    return d3d->obj()->CreateDevice( Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &mObject );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::GetDirect3D( IDirect3D9 ** ppD3D9 )
{
    if( !ppD3D9 )
    {
        GN_ERROR( "NULL parameter!" );
        return D3DERR_INVALIDCALL;
    };
    mD3D9->AddRef();
    *ppD3D9 = mD3D9;
    return D3D_OK;
}
