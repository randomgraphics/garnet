#include "pch.h"
#include "mydevice9.h"
#include "myd3d9.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.tool.d3d9Wrapper");

// *****************************************************************************
// MyDevice9
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::create(
    MyD3D9 * myd3d,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters )
{
    GN_GUARD;

    // store myd3d pointer
    GN_ASSERT( myd3d );
    GN_ASSERT( !mD3D9 );
    mD3D9 = myd3d;
    myd3d->AddRef();

	// Look for nvidia adapter
    UINT nAdapter = myd3d->GetAdapterCount();
    GN_ASSERT( nAdapter );
    GN_TRACE(sLogger)( "Looking for NVPerfHUD adapter in all adapters: total(%d), d3d(0x%p).", nAdapter, myd3d );
    for( UInt32 i = 0; i < nAdapter; ++i )
    {
        D3DADAPTER_IDENTIFIER9 Identifier;
        GN_DX9_CHECK( myd3d->GetAdapterIdentifier( i, 0, &Identifier ) );
        GN_TRACE(sLogger)( "Enumerating D3D adapters: %s", Identifier.Description );
        if( strstr(Identifier.Description,"NVPerfHUD") )
        {
            Adapter = i;
            DeviceType = D3DDEVTYPE_REF;
            break;
        }
    }

    // create device
    GN_TRACE(sLogger)( "create Direct3DDevice9 object: Adapter(%d), DeviceType(%d)", Adapter, DeviceType );
    return myd3d->obj()->CreateDevice( Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &mObject );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::GetDirect3D( IDirect3D9 ** ppD3D9 )
{
    if( !ppD3D9 )
    {
        GN_ERROR(sLogger)( "NULL parameter!" );
        return D3DERR_INVALIDCALL;
    };
    mD3D9->AddRef();
    *ppD3D9 = mD3D9;
    return D3D_OK;
}

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::Present(
    THIS_ CONST RECT* pSourceRect,
    CONST RECT* pDestRect,
    HWND hDestWindowOverride,
    CONST RGNDATA* pDirtyRegion)
{
    /* calculate FPS
    static DWORD last = GetTickCount();
    static DWORD count = 0;
    DWORD now = GetTickCount();
    ++count;
    if( now - last > 1000 )
    {
        printf( "FPS = %.2f\n", (float)count / ( now - last ) * 1000 );
        count = 0;
        last = now;
    }*/

    return obj()->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}
