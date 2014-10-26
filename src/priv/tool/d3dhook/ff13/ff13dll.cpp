#include "pch.h"
#include <d3d9.h>
#include <shlwapi.h>
#include "../apihijack.h"

static GN::Logger * sLogger = GN::getLogger("GN.d3dhook.ff13");

// -----------------------------------------------------------------------------
// IDirect3D9
// -----------------------------------------------------------------------------
struct IDirect3D9Vtbl
{
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IDirect3D9 * this_,  REFIID riid, void** ppvObj) ;
    ULONG (STDMETHODCALLTYPE *AddRef)(IDirect3D9 * this_) ;
    ULONG (STDMETHODCALLTYPE *Release)(IDirect3D9 * this_) ;
    HRESULT (STDMETHODCALLTYPE *RegisterSoftwareDevice)(IDirect3D9 * this_,  void* pInitializeFunction) ;
    UINT (STDMETHODCALLTYPE *GetAdapterCount)(IDirect3D9 * this_) ;
    HRESULT (STDMETHODCALLTYPE *GetAdapterIdentifier)(IDirect3D9 * this_,  UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier) ;
    UINT (STDMETHODCALLTYPE *GetAdapterModeCount)(IDirect3D9 * this_,  UINT Adapter,D3DFORMAT Format) ;
    HRESULT (STDMETHODCALLTYPE *EnumAdapterModes)(IDirect3D9 * this_,  UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) ;
    HRESULT (STDMETHODCALLTYPE *GetAdapterDisplayMode)(IDirect3D9 * this_,  UINT Adapter,D3DDISPLAYMODE* pMode) ;
    HRESULT (STDMETHODCALLTYPE *CheckDeviceType)(IDirect3D9 * this_,  UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) ;
    HRESULT (STDMETHODCALLTYPE *CheckDeviceFormat)(IDirect3D9 * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) ;
    HRESULT (STDMETHODCALLTYPE *CheckDeviceMultiSampleType)(IDirect3D9 * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) ;
    HRESULT (STDMETHODCALLTYPE *CheckDepthStencilMatch)(IDirect3D9 * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) ;
    HRESULT (STDMETHODCALLTYPE *CheckDeviceFormatConversion)(IDirect3D9 * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) ;
    HRESULT (STDMETHODCALLTYPE *GetDeviceCaps)(IDirect3D9 * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) ;
    HMONITOR (STDMETHODCALLTYPE *GetAdapterMonitor)(IDirect3D9 * this_,  UINT Adapter) ;
    HRESULT (STDMETHODCALLTYPE *CreateDevice)(IDirect3D9 * this_,  UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface) ;
};

// -----------------------------------------------------------------------------
// IDirect3D9Ex
// -----------------------------------------------------------------------------
struct IDirect3D9ExVtbl
{
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IDirect3D9Ex * this_,  REFIID riid, void** ppvObj) ;
    ULONG (STDMETHODCALLTYPE *AddRef)(IDirect3D9Ex * this_) ;
    ULONG (STDMETHODCALLTYPE *Release)(IDirect3D9Ex * this_) ;
    UINT (STDMETHODCALLTYPE *GetAdapterCount)(IDirect3D9Ex * this_) ;
    HRESULT (STDMETHODCALLTYPE *GetAdapterIdentifier)(IDirect3D9Ex * this_,  UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier) ;
    UINT (STDMETHODCALLTYPE *GetAdapterModeCount)(IDirect3D9Ex * this_,  UINT Adapter,D3DFORMAT Format) ;
    HRESULT (STDMETHODCALLTYPE *EnumAdapterModes)(IDirect3D9Ex * this_,  UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) ;
    HRESULT (STDMETHODCALLTYPE *GetAdapterDisplayMode)(IDirect3D9Ex * this_,  UINT Adapter,D3DDISPLAYMODE* pMode) ;
    HRESULT (STDMETHODCALLTYPE *CheckDeviceType)(IDirect3D9Ex * this_,  UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) ;
    HRESULT (STDMETHODCALLTYPE *CheckDeviceFormat)(IDirect3D9Ex * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) ;
    HRESULT (STDMETHODCALLTYPE *CheckDeviceMultiSampleType)(IDirect3D9Ex * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) ;
    HRESULT (STDMETHODCALLTYPE *CheckDepthStencilMatch)(IDirect3D9Ex * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) ;
    HRESULT (STDMETHODCALLTYPE *CheckDeviceFormatConversion)(IDirect3D9Ex * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) ;
    HRESULT (STDMETHODCALLTYPE *GetDeviceCaps)(IDirect3D9Ex * this_,  UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) ;
    HMONITOR (STDMETHODCALLTYPE *GetAdapterMonitor)(IDirect3D9Ex * this_,  UINT Adapter) ;
    HRESULT (STDMETHODCALLTYPE *CreateDevice)(IDirect3D9Ex * this_,  UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface) ;
    UINT (STDMETHODCALLTYPE *GetAdapterModeCountEx)(IDirect3D9Ex * this_,  UINT Adapter,CONST D3DDISPLAYMODEFILTER* pFilter ) ;
    HRESULT (STDMETHODCALLTYPE *EnumAdapterModesEx)(IDirect3D9Ex * this_,  UINT Adapter,CONST D3DDISPLAYMODEFILTER* pFilter,UINT Mode,D3DDISPLAYMODEEX* pMode) ;
    HRESULT (STDMETHODCALLTYPE *GetAdapterDisplayModeEx)(IDirect3D9Ex * this_,  UINT Adapter,D3DDISPLAYMODEEX* pMode,D3DDISPLAYROTATION* pRotation) ;
    HRESULT (STDMETHODCALLTYPE *CreateDeviceEx)(IDirect3D9Ex * this_,  UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,D3DDISPLAYMODEEX* pFullscreenDisplayMode,IDirect3DDevice9Ex** ppReturnedDeviceInterface) ;
    HRESULT (STDMETHODCALLTYPE *GetAdapterLUID)(IDirect3D9Ex * this_,  UINT Adapter,LUID * pLUID) ;
};

// -----------------------------------------------------------------------------
static void AdjustPP(HWND hFocusWindow, D3DPRESENT_PARAMETERS * pp)
{
    if (pp && pp->Windowed)
    {
        HWND window = pp->hDeviceWindow ? pp->hDeviceWindow : hFocusWindow;
        if (IsWindow(window))
        {
            // remove window border, make it full screen.
        	DWORD style = GetWindowLongA(window, GWL_STYLE);
        	style &= ~(WS_CAPTION | WS_BORDER | WS_THICKFRAME);
            style |= WS_POPUP;
        	SetWindowLongA(window, GWL_STYLE, style);
        	ShowWindow(window, SW_MAXIMIZE);
        	UpdateWindow(window);

            // adjust backbuffer size to match window size
            RECT rc;
            GetClientRect(window, &rc);
            pp->BackBufferWidth = rc.right - rc.left;
            pp->BackBufferHeight = rc.bottom - rc.top;
        }
    }


}

// -----------------------------------------------------------------------------
static IDirect3D9Vtbl g_IDirect3D9_Original;
HRESULT STDMETHODCALLTYPE IDirect3D9_CreateDevice_Hook(
    IDirect3D9 * this_,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pp,
    IDirect3DDevice9** ppReturnedDeviceInterface)
{
    AdjustPP(hFocusWindow, pp);
    return g_IDirect3D9_Original.CreateDevice(
        this_,
        Adapter,
        DeviceType,
        hFocusWindow,
        BehaviorFlags,
        pp,
        ppReturnedDeviceInterface);
}

// -----------------------------------------------------------------------------
static IDirect3D9ExVtbl g_IDirect3D9Ex_Original;
HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CreateDevice_Hook(
    IDirect3D9Ex * this_,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pp,
    IDirect3DDevice9** ppReturnedDeviceInterface)
{
    AdjustPP(hFocusWindow, pp);
    return g_IDirect3D9Ex_Original.CreateDevice(
        this_,
        Adapter,
        DeviceType,
        hFocusWindow,
        BehaviorFlags,
        pp,
        ppReturnedDeviceInterface);
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CreateDeviceEx_Hook(
    IDirect3D9Ex * this_,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pp,
    D3DDISPLAYMODEEX* pFullscreenDisplayMode,
    IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
    AdjustPP(hFocusWindow, pp);
    return g_IDirect3D9Ex_Original.CreateDeviceEx(
        this_,
        Adapter,
        DeviceType,
        hFocusWindow,
        BehaviorFlags,
        pp,
        pFullscreenDisplayMode,
        ppReturnedDeviceInterface);
}

// -----------------------------------------------------------------------------
static IDirect3D9 * Direct3DCreate9Hook(UINT SDKVersion);
static HRESULT Direct3DCreate9ExHook(UINT SDKVersion, IDirect3D9Ex **ppD3D);
static SDLLHook D3DHook =
{
    "d3d9.dll",
    false, NULL, // Default hook disabled, NULL function pointer.
    {
        { "Direct3DCreate9",   Direct3DCreate9Hook },
        { "Direct3DCreate9Ex", Direct3DCreate9ExHook },
        { NULL, NULL }
    }
};

// -----------------------------------------------------------------------------
static IDirect3D9 * Direct3DCreate9Hook(UINT SDKVersion)
{
    OutputDebugStringA(__FUNCTION__);

    typedef IDirect3D9 * (WINAPI * PFN_DIRECT3D_CREATE_9)(UINT SDKVersion);
    PFN_DIRECT3D_CREATE_9 realFunc = (PFN_DIRECT3D_CREATE_9)D3DHook.Functions[0].OrigFn;

    if (nullptr == realFunc) return nullptr;

    IDirect3D9 * d3d9 = realFunc(SDKVersion);

#if 0
    if (d3d9)
    {
        IDirect3D9Vtbl * vtable = *(IDirect3D9Vtbl**)d3d9;
        HANDLE process = ::GetCurrentProcess();
        DWORD oldProtection;
        if (::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
        {
            g_IDirect3D9_Original = *vtable;
            vtable->CreateDevice = IDirect3D9_CreateDevice_Hook;
            ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
        }
        else
        {
            GN_ERROR(sLogger)("Failed to change vtable page protection for interface IDirect3D9.");
        }
    }
#endif

    return d3d9;
}

// -----------------------------------------------------------------------------
static HRESULT Direct3DCreate9ExHook(UINT SDKVersion, IDirect3D9Ex **ppD3D)
{
    OutputDebugStringA(__FUNCTION__);

    typedef HRESULT (WINAPI * PFN_DIRECT3D_CREATE_9_EX)(UINT SDKVersion, IDirect3D9Ex **ppD3D);
    PFN_DIRECT3D_CREATE_9_EX realFunc = (PFN_DIRECT3D_CREATE_9_EX)D3DHook.Functions[1].OrigFn;

    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(SDKVersion, ppD3D);

#if 0
    if (SUCCEEDED(hr))
    {
        IDirect3D9ExVtbl * vtable = *(IDirect3D9ExVtbl**)(*ppD3D);
        HANDLE process = ::GetCurrentProcess();
        DWORD oldProtection;
        if (::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
        {
            g_IDirect3D9Ex_Original = *vtable;
            vtable->CreateDevice   = IDirect3D9Ex_CreateDevice_Hook;
            vtable->CreateDeviceEx = IDirect3D9Ex_CreateDeviceEx_Hook;
            ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
        }
        else
        {
            GN_ERROR(sLogger)("Failed to change vtable page protection for interface IDirect3D9.");
        }
    }
#endif

    return hr;
}

// -----------------------------------------------------------------------------
extern "C"
{
__declspec(dllexport) LRESULT CALLBACK CBTHookProc(int code, WPARAM wp, LPARAM lp)
{
    if (code >= 0)
    {
        // do your own work here.
    }

    // The hook itself does nothing. It's solo purpose is to inject the DLL into the target process.
    return CallNextHookEx(0, code, wp, lp);
}
}

// -----------------------------------------------------------------------------
BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID)
{
    if ( fdwReason == DLL_PROCESS_ATTACH )  // When initializing....
    {
        // We don't need thread notifications for what we're doing.  Thus,
        // get rid of them, thereby eliminating some of the overhead of
        // this DLL
        DisableThreadLibraryCalls( hModule );

        // Only hook the APIs if this is the right process.
        char Work[1024];
        GetModuleFileNameA( GetModuleHandle( NULL ), Work, sizeof(Work) );
        PathStripPathA( Work );

        if ( 0 == _stricmp( Work, "ffxiiiimg.exe" ) )
        {
            HookAPICalls( &D3DHook );
        }
    }
    return TRUE;
}
