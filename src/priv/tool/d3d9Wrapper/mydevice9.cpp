#include "pch.h"
#include "mydevice9.h"
#include "myd3d9.h"

static GN::Logger * sLogger = GN::getLogger("GN.tool.d3d9wrapper.MyDevice9");

// *****************************************************************************
// MyDevice9
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
MyDevice9::~MyDevice9()
{
    GN::safeRelease( mD3D9 );
}

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
    HRESULT hr = myd3d->realobj()->CreateDevice( Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &mRealObject );
    if( FAILED(hr) ) return hr;

    // store device caps for future use
    realobj()->GetDeviceCaps( &mCaps );

    GN_ASSERT( mCaps.MaxStreams <= GN_ARRAY_COUNT(mVtxBufs) );

    // success
    return D3D_OK;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
ULONG MyDevice9::Release()
{
    ULONG c = BasicInterface<IDirect3DDevice9>::Release();

    if( c > 0 )
    {
        UInt32 r = 0;
        
        // calculate number of streams in device
        for( UInt32 i = 0; i < mCaps.MaxStreams; ++i )
        {
            if( mVtxBufs[i].stream ) ++r;
        }

        if( c == r )
        {
            // This means that NO ONE is referencing the device,
            // other then resources that are currently binding to it.
            // So it is time to unbind all resources, and delete
            // device instance as well.

            // unbind vertex buffers
            for( UInt32 i = 0; i < mCaps.MaxStreams; ++i )
            {
                if( mVtxBufs[i].stream )
                {
                    SetStreamSource( i, 0, 0, 0 );
                }
            }

            // Note: when program reaches here, the device instance should be deleted already.
        }
    }

    return c;
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
    // calculate FPS
    static DWORD last = GetTickCount();
    static DWORD count = 0;
    DWORD now = GetTickCount();
    ++count;
    if( now - last > 1000 )
    {
        printf( "FPS = %.2f\n", (float)count / ( now - last ) * 1000 );
        count = 0;
        last = now;
    }

    return realobj()->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}
