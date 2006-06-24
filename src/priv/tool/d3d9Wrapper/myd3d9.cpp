#include "pch.h"
#include "myd3d9.h"
#include "mydevice9.h"

// *****************************************************************************
// Function Pointers
// *****************************************************************************

/*
CheckFullscreen
Direct3DShaderValidatorCreate9
PSGPError
PSGPSampleTexture
DebugSetLevel
DebugSetMute
*/

typedef IDirect3D9 * (WINAPI*FP_Direct3DCreate9)( UINT SdkVersion );
typedef int (WINAPI*FP_D3DPERF_BeginEvent)( D3DCOLOR col, LPCWSTR wszName );
typedef int (WINAPI*FP_D3DPERF_EndEvent)( void );
typedef void (WINAPI*FP_D3DPERF_SetMarker)( D3DCOLOR col, LPCWSTR wszName );
typedef void (WINAPI*FP_D3DPERF_SetRegion)( D3DCOLOR col, LPCWSTR wszName );
typedef BOOL (WINAPI*FP_D3DPERF_QueryRepeatFrame)( void );
typedef void (WINAPI*FP_D3DPERF_SetOptions)( DWORD dwOptions );
typedef DWORD (WINAPI*FP_D3DPERF_GetStatus)( void );

static HMODULE                       gD3D9Dll = 0;
static FP_Direct3DCreate9            gDirect3DCreate9;
static FP_D3DPERF_BeginEvent         gD3DPERF_BeginEvent;
static FP_D3DPERF_EndEvent           gD3DPERF_EndEvent;
static FP_D3DPERF_SetMarker          gD3DPERF_SetMarker;
static FP_D3DPERF_SetRegion          gD3DPERF_SetRegion;
static FP_D3DPERF_QueryRepeatFrame   gD3DPERF_QueryRepeatFrame;
static FP_D3DPERF_SetOptions         gD3DPERF_SetOptions;
static FP_D3DPERF_GetStatus          gD3DPERF_GetStatus;


// *****************************************************************************
// MyD3D9
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool MyD3D9::create( UINT sdkVersion )
{
    GN_GUARD;

    // check SDK version
    if( (sdkVersion&0xFF) != D3D_SDK_VERSION )
    {
        GN_ERROR( "unsupport SDK version %d (expecting %d)", (sdkVersion&0xFF), D3D_SDK_VERSION );
        return false;
    }

    GN_INFO( "create Direct3D9 object" );

    GN_ASSERT( !mObject );
    mObject = gDirect3DCreate9( sdkVersion );
    return !!mObject;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE MyD3D9::CreateDevice(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    IDirect3DDevice9** ppReturnedDeviceInterface )
{
    GN_GUARD;

    if( !ppReturnedDeviceInterface )
    {
        GN_ERROR( "ppReturnedDeviceInterface can't be NULL!" );
        return D3DERR_INVALIDCALL;
    }

    GN::AutoComPtr<MyDevice9> dev;
    dev.attach( new MyDevice9 );

    HRESULT hr = dev->create( this, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters );
    if( FAILED( hr ) ) return hr;

    // success
    *ppReturnedDeviceInterface = (IDirect3DDevice9*)dev.detach();
    return D3D_OK;

    GN_UNGUARD;
}

// *****************************************************************************
// DllMain
// *****************************************************************************

BOOL WINAPI DllMain(
    HINSTANCE /*hinstDLL*/,
    DWORD fdwReason,
    LPVOID /*lpvReserved*/ )
{
    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH :

            // load library
            gD3D9Dll = LoadLibraryA( "system32\\d3d9.dll" );
            if( 0 == gD3D9Dll )
            {
                GN_ERROR( "fail to load original D3D9.DLL" );
                return FALSE;
            }

            // get function pointers
            #define GETFP( NAME ) \
                g##NAME = (FP_##NAME)GetProcAddress( gD3D9Dll, #NAME ); \
                if( 0 == g##NAME ) { GN_ERROR( "Fail to get symbol of %s", #NAME ); return FALSE; }
            GETFP( Direct3DCreate9 );
            GETFP( D3DPERF_BeginEvent );
            GETFP( D3DPERF_EndEvent );
            GETFP( D3DPERF_SetMarker );
            GETFP( D3DPERF_SetRegion );
            GETFP( D3DPERF_QueryRepeatFrame );
            GETFP( D3DPERF_SetOptions );
            GETFP( D3DPERF_GetStatus );

            break;

        case DLL_PROCESS_DETACH :
            if( gD3D9Dll ) FreeLibrary( gD3D9Dll ), gD3D9Dll = 0;
            break;

        default : ; // do nothing
    }

    // success
    return TRUE;
}


// *****************************************************************************
// exported functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
MyD3D9 * WINAPI MyDirect3DCreate9( UINT sdkVersion )
{
    GN_GUARD;

    GN::AutoComPtr<MyD3D9> p;
    p.attach( new MyD3D9 );

    if( !p->create( sdkVersion ) ) return NULL;

    // success
    return p.detach();

    GN_UNGUARD;
}

int WINAPI MyD3DPERF_BeginEvent( D3DCOLOR col, LPCWSTR wszName )
{
    return gD3DPERF_BeginEvent( col, wszName );
}
int WINAPI MyD3DPERF_EndEvent( void )
{
    return gD3DPERF_EndEvent();
}
void WINAPI MyD3DPERF_SetMarker( D3DCOLOR col, LPCWSTR wszName )
{
    return gD3DPERF_SetMarker( col, wszName );
}
void WINAPI MyD3DPERF_SetRegion( D3DCOLOR col, LPCWSTR wszName )
{
    return gD3DPERF_SetRegion( col, wszName );
}
BOOL WINAPI MyD3DPERF_QueryRepeatFrame( void )
{
    return gD3DPERF_QueryRepeatFrame();
}
void WINAPI MyD3DPERF_SetOptions( DWORD dwOptions )
{
    gD3DPERF_SetOptions( dwOptions );
}
DWORD WINAPI MyD3DPERF_GetStatus( void )
{
    return gD3DPERF_GetStatus();
}
