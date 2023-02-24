// script generated file. Do _NOT_ edit.

#include "pch.h"
#include "d3d9vtable.h"

D3D9VTables g_D3D9OriginVTables;
D3D9VTables g_D3D9HookedVTables;

// -----------------------------------------------------------------------------
// IDirect3D9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_QueryInterface_Hooked(IDirect3D9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3D9_AddRef_Hooked(IDirect3D9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::AddRef");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3D9_Release_Hooked(IDirect3D9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::Release");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_RegisterSoftwareDevice_Hooked(IDirect3D9 * ptr, void * pInitializeFunction) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::RegisterSoftwareDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].RegisterSoftwareDevice(ptr, pInitializeFunction);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3D9_GetAdapterCount_Hooked(IDirect3D9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::GetAdapterCount");
    UINT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterCount(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_GetAdapterIdentifier_Hooked(IDirect3D9 * ptr, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 * pIdentifier) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::GetAdapterIdentifier");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterIdentifier(ptr, Adapter, Flags, pIdentifier);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3D9_GetAdapterModeCount_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DFORMAT Format) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::GetAdapterModeCount");
    UINT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterModeCount(ptr, Adapter, Format);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_EnumAdapterModes_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::EnumAdapterModes");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].EnumAdapterModes(ptr, Adapter, Format, Mode, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_GetAdapterDisplayMode_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDISPLAYMODE * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::GetAdapterDisplayMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterDisplayMode(ptr, Adapter, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDeviceType_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat,
                                                                   D3DFORMAT BackBufferFormat, BOOL bWindowed) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::CheckDeviceType");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDeviceType(ptr, Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDeviceFormat_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat,
                                                                     DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::CheckDeviceFormat");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDeviceFormat(ptr, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDeviceMultiSampleType_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat,
                                                                              BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD * pQualityLevels) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::CheckDeviceMultiSampleType");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDeviceMultiSampleType(ptr, Adapter, DeviceType, SurfaceFormat, Windowed,
                                                                                              MultiSampleType, pQualityLevels);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDepthStencilMatch_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat,
                                                                          D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::CheckDepthStencilMatch");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDepthStencilMatch(ptr, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDeviceFormatConversion_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat,
                                                                               D3DFORMAT TargetFormat) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::CheckDeviceFormatConversion");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDeviceFormatConversion(ptr, Adapter, DeviceType, SourceFormat, TargetFormat);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_GetDeviceCaps_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 * pCaps) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::GetDeviceCaps");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetDeviceCaps(ptr, Adapter, DeviceType, pCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HMONITOR STDMETHODCALLTYPE IDirect3D9_GetAdapterMonitor_Hooked(IDirect3D9 * ptr, UINT Adapter) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::GetAdapterMonitor");
    HMONITOR result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterMonitor(ptr, Adapter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9_CreateDevice_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags,
                                                                D3DPRESENT_PARAMETERS * pPresentationParameters,
                                                                IDirect3DDevice9 **     ppReturnedDeviceInterface) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9::CreateDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CreateDevice(ptr, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters,
                                                                                ppReturnedDeviceInterface);
    if (ppReturnedDeviceInterface && *ppReturnedDeviceInterface) { RealToHooked9(*ppReturnedDeviceInterface); }
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DDevice9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_QueryInterface_Hooked(IDirect3DDevice9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DDevice9_AddRef_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::AddRef");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DDevice9_Release_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::Release");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_TestCooperativeLevel_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::TestCooperativeLevel");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].TestCooperativeLevel(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3DDevice9_GetAvailableTextureMem_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetAvailableTextureMem");
    UINT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetAvailableTextureMem(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_EvictManagedResources_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::EvictManagedResources");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].EvictManagedResources(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetDirect3D_Hooked(IDirect3DDevice9 * ptr, IDirect3D9 ** ppD3D9) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetDirect3D");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetDirect3D(ptr, ppD3D9);
    if (ppD3D9 && *ppD3D9) { RealToHooked9(*ppD3D9); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetDeviceCaps_Hooked(IDirect3DDevice9 * ptr, D3DCAPS9 * pCaps) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetDeviceCaps");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetDeviceCaps(ptr, pCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetDisplayMode_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, D3DDISPLAYMODE * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetDisplayMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetDisplayMode(ptr, iSwapChain, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetCreateParameters_Hooked(IDirect3DDevice9 * ptr, D3DDEVICE_CREATION_PARAMETERS * pParameters) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetCreateParameters");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetCreateParameters(ptr, pParameters);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetCursorProperties_Hooked(IDirect3DDevice9 * ptr, UINT XHotSpot, UINT YHotSpot,
                                                                             IDirect3DSurface9 * pCursorBitmap) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetCursorProperties");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetCursorProperties(ptr, XHotSpot, YHotSpot, pCursorBitmap);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DDevice9_SetCursorPosition_Hooked(IDirect3DDevice9 * ptr, int X, int Y, DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetCursorPosition");
    g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetCursorPosition(ptr, X, Y, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDirect3DDevice9_ShowCursor_Hooked(IDirect3DDevice9 * ptr, BOOL bShow) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::ShowCursor");
    BOOL result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].ShowCursor(ptr, bShow);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateAdditionalSwapChain_Hooked(IDirect3DDevice9 * ptr, D3DPRESENT_PARAMETERS * pPresentationParameters,
                                                                                   IDirect3DSwapChain9 ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateAdditionalSwapChain");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateAdditionalSwapChain(ptr, pPresentationParameters, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked9(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetSwapChain_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, IDirect3DSwapChain9 ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetSwapChain");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetSwapChain(ptr, iSwapChain, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked9(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3DDevice9_GetNumberOfSwapChains_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetNumberOfSwapChains");
    UINT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetNumberOfSwapChains(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_Reset_Hooked(IDirect3DDevice9 * ptr, D3DPRESENT_PARAMETERS * pPresentationParameters) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::Reset");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].Reset(ptr, pPresentationParameters);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_Present_Hooked(IDirect3DDevice9 * ptr, CONST RECT * pSourceRect, CONST RECT * pDestRect,
                                                                 HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::Present");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].Present(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetBackBuffer_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type,
                                                                       IDirect3DSurface9 ** ppBackBuffer) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetBackBuffer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetBackBuffer(ptr, iSwapChain, iBackBuffer, Type, ppBackBuffer);
    if (ppBackBuffer && *ppBackBuffer) { RealToHooked9(*ppBackBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetRasterStatus_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetRasterStatus");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetRasterStatus(ptr, iSwapChain, pRasterStatus);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetDialogBoxMode_Hooked(IDirect3DDevice9 * ptr, BOOL bEnableDialogs) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetDialogBoxMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetDialogBoxMode(ptr, bEnableDialogs);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DDevice9_SetGammaRamp_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetGammaRamp");
    g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetGammaRamp(ptr, iSwapChain, Flags, pRamp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DDevice9_GetGammaRamp_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, D3DGAMMARAMP * pRamp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetGammaRamp");
    g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetGammaRamp(ptr, iSwapChain, pRamp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateTexture_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, UINT Levels, DWORD Usage,
                                                                       D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 ** ppTexture, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateTexture");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateTexture(ptr, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
    if (ppTexture && *ppTexture) { RealToHooked9(*ppTexture); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateVolumeTexture_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, UINT Depth, UINT Levels,
                                                                             DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
                                                                             IDirect3DVolumeTexture9 ** ppVolumeTexture, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateVolumeTexture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateVolumeTexture(ptr, Width, Height, Depth, Levels, Usage, Format, Pool,
                                                                                             ppVolumeTexture, pSharedHandle);
    if (ppVolumeTexture && *ppVolumeTexture) { RealToHooked9(*ppVolumeTexture); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateCubeTexture_Hooked(IDirect3DDevice9 * ptr, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format,
                                                                           D3DPOOL Pool, IDirect3DCubeTexture9 ** ppCubeTexture, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateCubeTexture");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateCubeTexture(ptr, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
    if (ppCubeTexture && *ppCubeTexture) { RealToHooked9(*ppCubeTexture); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateVertexBuffer_Hooked(IDirect3DDevice9 * ptr, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool,
                                                                            IDirect3DVertexBuffer9 ** ppVertexBuffer, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateVertexBuffer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateVertexBuffer(ptr, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
    if (ppVertexBuffer && *ppVertexBuffer) { RealToHooked9(*ppVertexBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateIndexBuffer_Hooked(IDirect3DDevice9 * ptr, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
                                                                           IDirect3DIndexBuffer9 ** ppIndexBuffer, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateIndexBuffer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateIndexBuffer(ptr, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
    if (ppIndexBuffer && *ppIndexBuffer) { RealToHooked9(*ppIndexBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateRenderTarget_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                            D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
                                                                            IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateRenderTarget");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateRenderTarget(ptr, Width, Height, Format, MultiSample, MultisampleQuality,
                                                                                            Lockable, ppSurface, pSharedHandle);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateDepthStencilSurface_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                                   D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard,
                                                                                   IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateDepthStencilSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateDepthStencilSurface(ptr, Width, Height, Format, MultiSample, MultisampleQuality,
                                                                                                   Discard, ppSurface, pSharedHandle);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_UpdateSurface_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect,
                                                                       IDirect3DSurface9 * pDestinationSurface, CONST POINT * pDestPoint) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::UpdateSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].UpdateSurface(ptr, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_UpdateTexture_Hooked(IDirect3DDevice9 * ptr, IDirect3DBaseTexture9 * pSourceTexture,
                                                                       IDirect3DBaseTexture9 * pDestinationTexture) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::UpdateTexture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].UpdateTexture(ptr, pSourceTexture, pDestinationTexture);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetRenderTargetData_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9 * pRenderTarget,
                                                                             IDirect3DSurface9 * pDestSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetRenderTargetData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetRenderTargetData(ptr, pRenderTarget, pDestSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetFrontBufferData_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, IDirect3DSurface9 * pDestSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetFrontBufferData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetFrontBufferData(ptr, iSwapChain, pDestSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_StretchRect_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect,
                                                                     IDirect3DSurface9 * pDestSurface, CONST RECT * pDestRect, D3DTEXTUREFILTERTYPE Filter) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::StretchRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].StretchRect(ptr, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_ColorFill_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9 * pSurface, CONST RECT * pRect, D3DCOLOR color) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::ColorFill");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].ColorFill(ptr, pSurface, pRect, color);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateOffscreenPlainSurface_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                                     D3DPOOL Pool, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateOffscreenPlainSurface");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateOffscreenPlainSurface(ptr, Width, Height, Format, Pool, ppSurface, pSharedHandle);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetRenderTarget_Hooked(IDirect3DDevice9 * ptr, DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetRenderTarget");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetRenderTarget(ptr, RenderTargetIndex, pRenderTarget);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetRenderTarget_Hooked(IDirect3DDevice9 * ptr, DWORD RenderTargetIndex, IDirect3DSurface9 ** ppRenderTarget) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetRenderTarget");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetRenderTarget(ptr, RenderTargetIndex, ppRenderTarget);
    if (ppRenderTarget && *ppRenderTarget) { RealToHooked9(*ppRenderTarget); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetDepthStencilSurface_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9 * pNewZStencil) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetDepthStencilSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetDepthStencilSurface(ptr, pNewZStencil);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetDepthStencilSurface_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9 ** ppZStencilSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetDepthStencilSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetDepthStencilSurface(ptr, ppZStencilSurface);
    if (ppZStencilSurface && *ppZStencilSurface) { RealToHooked9(*ppZStencilSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_BeginScene_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::BeginScene");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].BeginScene(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_EndScene_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::EndScene");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].EndScene(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_Clear_Hooked(IDirect3DDevice9 * ptr, DWORD Count, CONST D3DRECT * pRects, DWORD Flags, D3DCOLOR Color,
                                                               float Z, DWORD Stencil) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::Clear");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].Clear(ptr, Count, pRects, Flags, Color, Z, Stencil);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetTransform_Hooked(IDirect3DDevice9 * ptr, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetTransform");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetTransform(ptr, State, pMatrix);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetTransform_Hooked(IDirect3DDevice9 * ptr, D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetTransform");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetTransform(ptr, State, pMatrix);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_MultiplyTransform_Hooked(IDirect3DDevice9 * ptr, D3DTRANSFORMSTATETYPE TransformType,
                                                                           CONST D3DMATRIX * Value) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::MultiplyTransform");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].MultiplyTransform(ptr, TransformType, Value);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetViewport_Hooked(IDirect3DDevice9 * ptr, CONST D3DVIEWPORT9 * pViewport) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetViewport");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetViewport(ptr, pViewport);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetViewport_Hooked(IDirect3DDevice9 * ptr, D3DVIEWPORT9 * pViewport) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetViewport");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetViewport(ptr, pViewport);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetMaterial_Hooked(IDirect3DDevice9 * ptr, CONST D3DMATERIAL9 * pMaterial) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetMaterial");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetMaterial(ptr, pMaterial);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetMaterial_Hooked(IDirect3DDevice9 * ptr, D3DMATERIAL9 * pMaterial) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetMaterial");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetMaterial(ptr, pMaterial);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetLight_Hooked(IDirect3DDevice9 * ptr, DWORD Index, CONST D3DLIGHT9 * pLight) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetLight");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetLight(ptr, Index, pLight);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetLight_Hooked(IDirect3DDevice9 * ptr, DWORD Index, D3DLIGHT9 * pLight) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetLight");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetLight(ptr, Index, pLight);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_LightEnable_Hooked(IDirect3DDevice9 * ptr, DWORD Index, BOOL Enable) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::LightEnable");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].LightEnable(ptr, Index, Enable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetLightEnable_Hooked(IDirect3DDevice9 * ptr, DWORD Index, BOOL * pEnable) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetLightEnable");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetLightEnable(ptr, Index, pEnable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetClipPlane_Hooked(IDirect3DDevice9 * ptr, DWORD Index, CONST float * pPlane) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetClipPlane");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetClipPlane(ptr, Index, pPlane);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetClipPlane_Hooked(IDirect3DDevice9 * ptr, DWORD Index, float * pPlane) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetClipPlane");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetClipPlane(ptr, Index, pPlane);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetRenderState_Hooked(IDirect3DDevice9 * ptr, D3DRENDERSTATETYPE State, DWORD Value) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetRenderState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetRenderState(ptr, State, Value);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetRenderState_Hooked(IDirect3DDevice9 * ptr, D3DRENDERSTATETYPE State, DWORD * pValue) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetRenderState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetRenderState(ptr, State, pValue);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateStateBlock_Hooked(IDirect3DDevice9 * ptr, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 ** ppSB) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateStateBlock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateStateBlock(ptr, Type, ppSB);
    if (ppSB && *ppSB) { RealToHooked9(*ppSB); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_BeginStateBlock_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::BeginStateBlock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].BeginStateBlock(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_EndStateBlock_Hooked(IDirect3DDevice9 * ptr, IDirect3DStateBlock9 ** ppSB) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::EndStateBlock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].EndStateBlock(ptr, ppSB);
    if (ppSB && *ppSB) { RealToHooked9(*ppSB); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetClipStatus_Hooked(IDirect3DDevice9 * ptr, CONST D3DCLIPSTATUS9 * pClipStatus) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetClipStatus");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetClipStatus(ptr, pClipStatus);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetClipStatus_Hooked(IDirect3DDevice9 * ptr, D3DCLIPSTATUS9 * pClipStatus) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetClipStatus");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetClipStatus(ptr, pClipStatus);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetTexture_Hooked(IDirect3DDevice9 * ptr, DWORD Stage, IDirect3DBaseTexture9 ** ppTexture) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetTexture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetTexture(ptr, Stage, ppTexture);
    if (ppTexture && *ppTexture) { RealToHooked9(*ppTexture); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetTexture_Hooked(IDirect3DDevice9 * ptr, DWORD Stage, IDirect3DBaseTexture9 * pTexture) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetTexture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetTexture(ptr, Stage, pTexture);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetTextureStageState_Hooked(IDirect3DDevice9 * ptr, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type,
                                                                              DWORD * pValue) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetTextureStageState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetTextureStageState(ptr, Stage, Type, pValue);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetTextureStageState_Hooked(IDirect3DDevice9 * ptr, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetTextureStageState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetTextureStageState(ptr, Stage, Type, Value);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetSamplerState_Hooked(IDirect3DDevice9 * ptr, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetSamplerState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetSamplerState(ptr, Sampler, Type, pValue);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetSamplerState_Hooked(IDirect3DDevice9 * ptr, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetSamplerState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetSamplerState(ptr, Sampler, Type, Value);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_ValidateDevice_Hooked(IDirect3DDevice9 * ptr, DWORD * pNumPasses) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::ValidateDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].ValidateDevice(ptr, pNumPasses);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPaletteEntries_Hooked(IDirect3DDevice9 * ptr, UINT PaletteNumber, CONST PALETTEENTRY * pEntries) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetPaletteEntries");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPaletteEntries(ptr, PaletteNumber, pEntries);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPaletteEntries_Hooked(IDirect3DDevice9 * ptr, UINT PaletteNumber, PALETTEENTRY * pEntries) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetPaletteEntries");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPaletteEntries(ptr, PaletteNumber, pEntries);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetCurrentTexturePalette_Hooked(IDirect3DDevice9 * ptr, UINT PaletteNumber) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetCurrentTexturePalette");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetCurrentTexturePalette(ptr, PaletteNumber);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetCurrentTexturePalette_Hooked(IDirect3DDevice9 * ptr, UINT * PaletteNumber) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetCurrentTexturePalette");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetCurrentTexturePalette(ptr, PaletteNumber);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetScissorRect_Hooked(IDirect3DDevice9 * ptr, CONST RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetScissorRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetScissorRect(ptr, pRect);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetScissorRect_Hooked(IDirect3DDevice9 * ptr, RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetScissorRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetScissorRect(ptr, pRect);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetSoftwareVertexProcessing_Hooked(IDirect3DDevice9 * ptr, BOOL bSoftware) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetSoftwareVertexProcessing");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetSoftwareVertexProcessing(ptr, bSoftware);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDirect3DDevice9_GetSoftwareVertexProcessing_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetSoftwareVertexProcessing");
    BOOL result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetSoftwareVertexProcessing(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetNPatchMode_Hooked(IDirect3DDevice9 * ptr, float nSegments) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetNPatchMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetNPatchMode(ptr, nSegments);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static float STDMETHODCALLTYPE IDirect3DDevice9_GetNPatchMode_Hooked(IDirect3DDevice9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetNPatchMode");
    float result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetNPatchMode(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawPrimitive_Hooked(IDirect3DDevice9 * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex,
                                                                       UINT PrimitiveCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::DrawPrimitive");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawPrimitive(ptr, PrimitiveType, StartVertex, PrimitiveCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawIndexedPrimitive_Hooked(IDirect3DDevice9 * ptr, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex,
                                                                              UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::DrawIndexedPrimitive");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawIndexedPrimitive(ptr, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
                                                                                              startIndex, primCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawPrimitiveUP_Hooked(IDirect3DDevice9 * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount,
                                                                         CONST void * pVertexStreamZeroData, UINT VertexStreamZeroStride) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::DrawPrimitiveUP");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawPrimitiveUP(ptr, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawIndexedPrimitiveUP_Hooked(IDirect3DDevice9 * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex,
                                                                                UINT NumVertices, UINT PrimitiveCount, CONST void * pIndexData,
                                                                                D3DFORMAT IndexDataFormat, CONST void * pVertexStreamZeroData,
                                                                                UINT VertexStreamZeroStride) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::DrawIndexedPrimitiveUP");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawIndexedPrimitiveUP(
        ptr, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_ProcessVertices_Hooked(IDirect3DDevice9 * ptr, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount,
                                                                         IDirect3DVertexBuffer9 * pDestBuffer, IDirect3DVertexDeclaration9 * pVertexDecl,
                                                                         DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::ProcessVertices");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].ProcessVertices(ptr, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateVertexDeclaration_Hooked(IDirect3DDevice9 * ptr, CONST D3DVERTEXELEMENT9 * pVertexElements,
                                                                                 IDirect3DVertexDeclaration9 ** ppDecl) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateVertexDeclaration");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateVertexDeclaration(ptr, pVertexElements, ppDecl);
    if (ppDecl && *ppDecl) { RealToHooked9(*ppDecl); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexDeclaration_Hooked(IDirect3DDevice9 * ptr, IDirect3DVertexDeclaration9 * pDecl) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetVertexDeclaration");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexDeclaration(ptr, pDecl);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexDeclaration_Hooked(IDirect3DDevice9 * ptr, IDirect3DVertexDeclaration9 ** ppDecl) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetVertexDeclaration");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexDeclaration(ptr, ppDecl);
    if (ppDecl && *ppDecl) { RealToHooked9(*ppDecl); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetFVF_Hooked(IDirect3DDevice9 * ptr, DWORD FVF) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetFVF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetFVF(ptr, FVF);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetFVF_Hooked(IDirect3DDevice9 * ptr, DWORD * pFVF) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetFVF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetFVF(ptr, pFVF);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateVertexShader_Hooked(IDirect3DDevice9 * ptr, CONST DWORD * pFunction,
                                                                            IDirect3DVertexShader9 ** ppShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateVertexShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateVertexShader(ptr, pFunction, ppShader);
    if (ppShader && *ppShader) { RealToHooked9(*ppShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexShader_Hooked(IDirect3DDevice9 * ptr, IDirect3DVertexShader9 * pShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetVertexShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexShader(ptr, pShader);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexShader_Hooked(IDirect3DDevice9 * ptr, IDirect3DVertexShader9 ** ppShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetVertexShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexShader(ptr, ppShader);
    if (ppShader && *ppShader) { RealToHooked9(*ppShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexShaderConstantF_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST float * pConstantData,
                                                                                  UINT Vector4fCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetVertexShaderConstantF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexShaderConstantF_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, float * pConstantData,
                                                                                  UINT Vector4fCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetVertexShaderConstantF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexShaderConstantI_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST int * pConstantData,
                                                                                  UINT Vector4iCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetVertexShaderConstantI");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexShaderConstantI_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, int * pConstantData,
                                                                                  UINT Vector4iCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetVertexShaderConstantI");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexShaderConstantB_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST BOOL * pConstantData,
                                                                                  UINT BoolCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetVertexShaderConstantB");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexShaderConstantB_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, BOOL * pConstantData,
                                                                                  UINT BoolCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetVertexShaderConstantB");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetStreamSource_Hooked(IDirect3DDevice9 * ptr, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData,
                                                                         UINT OffsetInBytes, UINT Stride) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetStreamSource");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetStreamSource(ptr, StreamNumber, pStreamData, OffsetInBytes, Stride);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetStreamSource_Hooked(IDirect3DDevice9 * ptr, UINT StreamNumber, IDirect3DVertexBuffer9 ** ppStreamData,
                                                                         UINT * pOffsetInBytes, UINT * pStride) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetStreamSource");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetStreamSource(ptr, StreamNumber, ppStreamData, pOffsetInBytes, pStride);
    if (ppStreamData && *ppStreamData) { RealToHooked9(*ppStreamData); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetStreamSourceFreq_Hooked(IDirect3DDevice9 * ptr, UINT StreamNumber, UINT Setting) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetStreamSourceFreq");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetStreamSourceFreq(ptr, StreamNumber, Setting);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetStreamSourceFreq_Hooked(IDirect3DDevice9 * ptr, UINT StreamNumber, UINT * pSetting) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetStreamSourceFreq");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetStreamSourceFreq(ptr, StreamNumber, pSetting);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetIndices_Hooked(IDirect3DDevice9 * ptr, IDirect3DIndexBuffer9 * pIndexData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetIndices");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetIndices(ptr, pIndexData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetIndices_Hooked(IDirect3DDevice9 * ptr, IDirect3DIndexBuffer9 ** ppIndexData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetIndices");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetIndices(ptr, ppIndexData);
    if (ppIndexData && *ppIndexData) { RealToHooked9(*ppIndexData); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreatePixelShader_Hooked(IDirect3DDevice9 * ptr, CONST DWORD * pFunction, IDirect3DPixelShader9 ** ppShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreatePixelShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreatePixelShader(ptr, pFunction, ppShader);
    if (ppShader && *ppShader) { RealToHooked9(*ppShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPixelShader_Hooked(IDirect3DDevice9 * ptr, IDirect3DPixelShader9 * pShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetPixelShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPixelShader(ptr, pShader);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPixelShader_Hooked(IDirect3DDevice9 * ptr, IDirect3DPixelShader9 ** ppShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetPixelShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPixelShader(ptr, ppShader);
    if (ppShader && *ppShader) { RealToHooked9(*ppShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPixelShaderConstantF_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST float * pConstantData,
                                                                                 UINT Vector4fCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetPixelShaderConstantF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPixelShaderConstantF_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, float * pConstantData,
                                                                                 UINT Vector4fCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetPixelShaderConstantF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPixelShaderConstantI_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST int * pConstantData,
                                                                                 UINT Vector4iCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetPixelShaderConstantI");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPixelShaderConstantI_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, int * pConstantData,
                                                                                 UINT Vector4iCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetPixelShaderConstantI");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPixelShaderConstantB_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST BOOL * pConstantData,
                                                                                 UINT BoolCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::SetPixelShaderConstantB");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPixelShaderConstantB_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, BOOL * pConstantData,
                                                                                 UINT BoolCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::GetPixelShaderConstantB");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawRectPatch_Hooked(IDirect3DDevice9 * ptr, UINT Handle, CONST float * pNumSegs,
                                                                       CONST D3DRECTPATCH_INFO * pRectPatchInfo) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::DrawRectPatch");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawRectPatch(ptr, Handle, pNumSegs, pRectPatchInfo);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawTriPatch_Hooked(IDirect3DDevice9 * ptr, UINT Handle, CONST float * pNumSegs,
                                                                      CONST D3DTRIPATCH_INFO * pTriPatchInfo) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::DrawTriPatch");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawTriPatch(ptr, Handle, pNumSegs, pTriPatchInfo);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DeletePatch_Hooked(IDirect3DDevice9 * ptr, UINT Handle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::DeletePatch");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DeletePatch(ptr, Handle);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateQuery_Hooked(IDirect3DDevice9 * ptr, D3DQUERYTYPE Type, IDirect3DQuery9 ** ppQuery) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9::CreateQuery");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateQuery(ptr, Type, ppQuery);
    if (ppQuery && *ppQuery) { RealToHooked9(*ppQuery); }
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DStateBlock9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DStateBlock9_QueryInterface_Hooked(IDirect3DStateBlock9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DStateBlock9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DStateBlock9_AddRef_Hooked(IDirect3DStateBlock9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DStateBlock9::AddRef");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DStateBlock9_Release_Hooked(IDirect3DStateBlock9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DStateBlock9::Release");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DStateBlock9_GetDevice_Hooked(IDirect3DStateBlock9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DStateBlock9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DStateBlock9_Capture_Hooked(IDirect3DStateBlock9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DStateBlock9::Capture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].Capture(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DStateBlock9_Apply_Hooked(IDirect3DStateBlock9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DStateBlock9::Apply");
    HRESULT result = g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].Apply(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DSwapChain9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_QueryInterface_Hooked(IDirect3DSwapChain9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DSwapChain9_AddRef_Hooked(IDirect3DSwapChain9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::AddRef");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DSwapChain9_Release_Hooked(IDirect3DSwapChain9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::Release");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_Present_Hooked(IDirect3DSwapChain9 * ptr, CONST RECT * pSourceRect, CONST RECT * pDestRect,
                                                                    HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion, DWORD dwFlags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::Present");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].Present(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetFrontBufferData_Hooked(IDirect3DSwapChain9 * ptr, IDirect3DSurface9 * pDestSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::GetFrontBufferData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetFrontBufferData(ptr, pDestSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetBackBuffer_Hooked(IDirect3DSwapChain9 * ptr, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type,
                                                                          IDirect3DSurface9 ** ppBackBuffer) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::GetBackBuffer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetBackBuffer(ptr, iBackBuffer, Type, ppBackBuffer);
    if (ppBackBuffer && *ppBackBuffer) { RealToHooked9(*ppBackBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetRasterStatus_Hooked(IDirect3DSwapChain9 * ptr, D3DRASTER_STATUS * pRasterStatus) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::GetRasterStatus");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetRasterStatus(ptr, pRasterStatus);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetDisplayMode_Hooked(IDirect3DSwapChain9 * ptr, D3DDISPLAYMODE * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::GetDisplayMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetDisplayMode(ptr, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetDevice_Hooked(IDirect3DSwapChain9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetPresentParameters_Hooked(IDirect3DSwapChain9 * ptr, D3DPRESENT_PARAMETERS * pPresentationParameters) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9::GetPresentParameters");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetPresentParameters(ptr, pPresentationParameters);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DResource9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DResource9_QueryInterface_Hooked(IDirect3DResource9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DResource9_AddRef_Hooked(IDirect3DResource9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::AddRef");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DResource9_Release_Hooked(IDirect3DResource9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::Release");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DResource9_GetDevice_Hooked(IDirect3DResource9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DResource9_SetPrivateData_Hooked(IDirect3DResource9 * ptr, REFGUID refguid, CONST void * pData, DWORD SizeOfData,
                                                                          DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DResource9_GetPrivateData_Hooked(IDirect3DResource9 * ptr, REFGUID refguid, void * pData, DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DResource9_FreePrivateData_Hooked(IDirect3DResource9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DResource9_SetPriority_Hooked(IDirect3DResource9 * ptr, DWORD PriorityNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::SetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].SetPriority(ptr, PriorityNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DResource9_GetPriority_Hooked(IDirect3DResource9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::GetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].GetPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DResource9_PreLoad_Hooked(IDirect3DResource9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::PreLoad");
    g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DResource9_GetType_Hooked(IDirect3DResource9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DResource9::GetType");
    D3DRESOURCETYPE result = g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DVertexDeclaration9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexDeclaration9_QueryInterface_Hooked(IDirect3DVertexDeclaration9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexDeclaration9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVertexDeclaration9_AddRef_Hooked(IDirect3DVertexDeclaration9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexDeclaration9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVertexDeclaration9_Release_Hooked(IDirect3DVertexDeclaration9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexDeclaration9::Release");
    return g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexDeclaration9_GetDevice_Hooked(IDirect3DVertexDeclaration9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexDeclaration9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexDeclaration9_GetDeclaration_Hooked(IDirect3DVertexDeclaration9 * ptr, D3DVERTEXELEMENT9 * pElement,
                                                                                   UINT * pNumElements) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexDeclaration9::GetDeclaration");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].GetDeclaration(ptr, pElement, pNumElements);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DVertexShader9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexShader9_QueryInterface_Hooked(IDirect3DVertexShader9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexShader9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVertexShader9_AddRef_Hooked(IDirect3DVertexShader9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexShader9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVertexShader9_Release_Hooked(IDirect3DVertexShader9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexShader9::Release");
    return g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexShader9_GetDevice_Hooked(IDirect3DVertexShader9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexShader9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexShader9_GetFunction_Hooked(IDirect3DVertexShader9 * ptr, void * pFunction, UINT * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexShader9::GetFunction");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].GetFunction(ptr, pFunction, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DPixelShader9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DPixelShader9_QueryInterface_Hooked(IDirect3DPixelShader9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DPixelShader9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DPixelShader9_AddRef_Hooked(IDirect3DPixelShader9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DPixelShader9::AddRef");
    return g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DPixelShader9_Release_Hooked(IDirect3DPixelShader9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DPixelShader9::Release");
    return g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DPixelShader9_GetDevice_Hooked(IDirect3DPixelShader9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DPixelShader9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DPixelShader9_GetFunction_Hooked(IDirect3DPixelShader9 * ptr, void * pFunction, UINT * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DPixelShader9::GetFunction");
    HRESULT result = g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].GetFunction(ptr, pFunction, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DBaseTexture9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_QueryInterface_Hooked(IDirect3DBaseTexture9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DBaseTexture9_AddRef_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::AddRef");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DBaseTexture9_Release_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::Release");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_GetDevice_Hooked(IDirect3DBaseTexture9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_SetPrivateData_Hooked(IDirect3DBaseTexture9 * ptr, REFGUID refguid, CONST void * pData, DWORD SizeOfData,
                                                                             DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_GetPrivateData_Hooked(IDirect3DBaseTexture9 * ptr, REFGUID refguid, void * pData, DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_FreePrivateData_Hooked(IDirect3DBaseTexture9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_SetPriority_Hooked(IDirect3DBaseTexture9 * ptr, DWORD PriorityNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::SetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].SetPriority(ptr, PriorityNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_GetPriority_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::GetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DBaseTexture9_PreLoad_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::PreLoad");
    g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DBaseTexture9_GetType_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::GetType");
    D3DRESOURCETYPE result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_SetLOD_Hooked(IDirect3DBaseTexture9 * ptr, DWORD LODNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::SetLOD");
    DWORD result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].SetLOD(ptr, LODNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_GetLOD_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::GetLOD");
    DWORD result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetLOD(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_GetLevelCount_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::GetLevelCount");
    DWORD result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetLevelCount(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_SetAutoGenFilterType_Hooked(IDirect3DBaseTexture9 * ptr, D3DTEXTUREFILTERTYPE FilterType) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::SetAutoGenFilterType");
    HRESULT result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].SetAutoGenFilterType(ptr, FilterType);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE IDirect3DBaseTexture9_GetAutoGenFilterType_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::GetAutoGenFilterType");
    D3DTEXTUREFILTERTYPE result = g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetAutoGenFilterType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DBaseTexture9_GenerateMipSubLevels_Hooked(IDirect3DBaseTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DBaseTexture9::GenerateMipSubLevels");
    g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GenerateMipSubLevels(ptr);
}

// -----------------------------------------------------------------------------
// IDirect3DTexture9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_QueryInterface_Hooked(IDirect3DTexture9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DTexture9_AddRef_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::AddRef");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DTexture9_Release_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::Release");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_GetDevice_Hooked(IDirect3DTexture9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_SetPrivateData_Hooked(IDirect3DTexture9 * ptr, REFGUID refguid, CONST void * pData, DWORD SizeOfData,
                                                                         DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_GetPrivateData_Hooked(IDirect3DTexture9 * ptr, REFGUID refguid, void * pData, DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_FreePrivateData_Hooked(IDirect3DTexture9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DTexture9_SetPriority_Hooked(IDirect3DTexture9 * ptr, DWORD PriorityNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::SetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].SetPriority(ptr, PriorityNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DTexture9_GetPriority_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DTexture9_PreLoad_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::PreLoad");
    g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DTexture9_GetType_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetType");
    D3DRESOURCETYPE result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DTexture9_SetLOD_Hooked(IDirect3DTexture9 * ptr, DWORD LODNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::SetLOD");
    DWORD result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].SetLOD(ptr, LODNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DTexture9_GetLOD_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetLOD");
    DWORD result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetLOD(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DTexture9_GetLevelCount_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetLevelCount");
    DWORD result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetLevelCount(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_SetAutoGenFilterType_Hooked(IDirect3DTexture9 * ptr, D3DTEXTUREFILTERTYPE FilterType) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::SetAutoGenFilterType");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].SetAutoGenFilterType(ptr, FilterType);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE IDirect3DTexture9_GetAutoGenFilterType_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetAutoGenFilterType");
    D3DTEXTUREFILTERTYPE result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetAutoGenFilterType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DTexture9_GenerateMipSubLevels_Hooked(IDirect3DTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GenerateMipSubLevels");
    g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GenerateMipSubLevels(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_GetLevelDesc_Hooked(IDirect3DTexture9 * ptr, UINT Level, D3DSURFACE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetLevelDesc");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetLevelDesc(ptr, Level, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_GetSurfaceLevel_Hooked(IDirect3DTexture9 * ptr, UINT Level, IDirect3DSurface9 ** ppSurfaceLevel) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::GetSurfaceLevel");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetSurfaceLevel(ptr, Level, ppSurfaceLevel);
    if (ppSurfaceLevel && *ppSurfaceLevel) { RealToHooked9(*ppSurfaceLevel); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_LockRect_Hooked(IDirect3DTexture9 * ptr, UINT Level, D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect,
                                                                   DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::LockRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].LockRect(ptr, Level, pLockedRect, pRect, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_UnlockRect_Hooked(IDirect3DTexture9 * ptr, UINT Level) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::UnlockRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].UnlockRect(ptr, Level);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_AddDirtyRect_Hooked(IDirect3DTexture9 * ptr, CONST RECT * pDirtyRect) {
    GN_D3DHOOK_CALLTRACE("IDirect3DTexture9::AddDirtyRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].AddDirtyRect(ptr, pDirtyRect);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DVolumeTexture9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_QueryInterface_Hooked(IDirect3DVolumeTexture9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVolumeTexture9_AddRef_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVolumeTexture9_Release_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::Release");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetDevice_Hooked(IDirect3DVolumeTexture9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_SetPrivateData_Hooked(IDirect3DVolumeTexture9 * ptr, REFGUID refguid, CONST void * pData,
                                                                               DWORD SizeOfData, DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetPrivateData_Hooked(IDirect3DVolumeTexture9 * ptr, REFGUID refguid, void * pData,
                                                                               DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_FreePrivateData_Hooked(IDirect3DVolumeTexture9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_SetPriority_Hooked(IDirect3DVolumeTexture9 * ptr, DWORD PriorityNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::SetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].SetPriority(ptr, PriorityNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetPriority_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DVolumeTexture9_PreLoad_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::PreLoad");
    g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetType_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetType");
    D3DRESOURCETYPE result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_SetLOD_Hooked(IDirect3DVolumeTexture9 * ptr, DWORD LODNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::SetLOD");
    DWORD result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].SetLOD(ptr, LODNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetLOD_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetLOD");
    DWORD result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLOD(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetLevelCount_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetLevelCount");
    DWORD result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLevelCount(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_SetAutoGenFilterType_Hooked(IDirect3DVolumeTexture9 * ptr, D3DTEXTUREFILTERTYPE FilterType) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::SetAutoGenFilterType");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].SetAutoGenFilterType(ptr, FilterType);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetAutoGenFilterType_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetAutoGenFilterType");
    D3DTEXTUREFILTERTYPE result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetAutoGenFilterType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DVolumeTexture9_GenerateMipSubLevels_Hooked(IDirect3DVolumeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GenerateMipSubLevels");
    g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GenerateMipSubLevels(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetLevelDesc_Hooked(IDirect3DVolumeTexture9 * ptr, UINT Level, D3DVOLUME_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetLevelDesc");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLevelDesc(ptr, Level, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetVolumeLevel_Hooked(IDirect3DVolumeTexture9 * ptr, UINT Level, IDirect3DVolume9 ** ppVolumeLevel) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::GetVolumeLevel");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetVolumeLevel(ptr, Level, ppVolumeLevel);
    if (ppVolumeLevel && *ppVolumeLevel) { RealToHooked9(*ppVolumeLevel); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_LockBox_Hooked(IDirect3DVolumeTexture9 * ptr, UINT Level, D3DLOCKED_BOX * pLockedVolume,
                                                                        CONST D3DBOX * pBox, DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::LockBox");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].LockBox(ptr, Level, pLockedVolume, pBox, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_UnlockBox_Hooked(IDirect3DVolumeTexture9 * ptr, UINT Level) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::UnlockBox");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].UnlockBox(ptr, Level);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_AddDirtyBox_Hooked(IDirect3DVolumeTexture9 * ptr, CONST D3DBOX * pDirtyBox) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolumeTexture9::AddDirtyBox");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].AddDirtyBox(ptr, pDirtyBox);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DCubeTexture9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_QueryInterface_Hooked(IDirect3DCubeTexture9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DCubeTexture9_AddRef_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::AddRef");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DCubeTexture9_Release_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::Release");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_GetDevice_Hooked(IDirect3DCubeTexture9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_SetPrivateData_Hooked(IDirect3DCubeTexture9 * ptr, REFGUID refguid, CONST void * pData, DWORD SizeOfData,
                                                                             DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_GetPrivateData_Hooked(IDirect3DCubeTexture9 * ptr, REFGUID refguid, void * pData, DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_FreePrivateData_Hooked(IDirect3DCubeTexture9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_SetPriority_Hooked(IDirect3DCubeTexture9 * ptr, DWORD PriorityNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::SetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].SetPriority(ptr, PriorityNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_GetPriority_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DCubeTexture9_PreLoad_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::PreLoad");
    g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DCubeTexture9_GetType_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetType");
    D3DRESOURCETYPE result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_SetLOD_Hooked(IDirect3DCubeTexture9 * ptr, DWORD LODNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::SetLOD");
    DWORD result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].SetLOD(ptr, LODNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_GetLOD_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetLOD");
    DWORD result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetLOD(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_GetLevelCount_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetLevelCount");
    DWORD result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetLevelCount(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_SetAutoGenFilterType_Hooked(IDirect3DCubeTexture9 * ptr, D3DTEXTUREFILTERTYPE FilterType) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::SetAutoGenFilterType");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].SetAutoGenFilterType(ptr, FilterType);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE IDirect3DCubeTexture9_GetAutoGenFilterType_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetAutoGenFilterType");
    D3DTEXTUREFILTERTYPE result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetAutoGenFilterType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DCubeTexture9_GenerateMipSubLevels_Hooked(IDirect3DCubeTexture9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GenerateMipSubLevels");
    g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GenerateMipSubLevels(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_GetLevelDesc_Hooked(IDirect3DCubeTexture9 * ptr, UINT Level, D3DSURFACE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetLevelDesc");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetLevelDesc(ptr, Level, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_GetCubeMapSurface_Hooked(IDirect3DCubeTexture9 * ptr, D3DCUBEMAP_FACES FaceType, UINT Level,
                                                                                IDirect3DSurface9 ** ppCubeMapSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::GetCubeMapSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetCubeMapSurface(ptr, FaceType, Level, ppCubeMapSurface);
    if (ppCubeMapSurface && *ppCubeMapSurface) { RealToHooked9(*ppCubeMapSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_LockRect_Hooked(IDirect3DCubeTexture9 * ptr, D3DCUBEMAP_FACES FaceType, UINT Level,
                                                                       D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::LockRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].LockRect(ptr, FaceType, Level, pLockedRect, pRect, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_UnlockRect_Hooked(IDirect3DCubeTexture9 * ptr, D3DCUBEMAP_FACES FaceType, UINT Level) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::UnlockRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].UnlockRect(ptr, FaceType, Level);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_AddDirtyRect_Hooked(IDirect3DCubeTexture9 * ptr, D3DCUBEMAP_FACES FaceType, CONST RECT * pDirtyRect) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCubeTexture9::AddDirtyRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].AddDirtyRect(ptr, FaceType, pDirtyRect);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DVertexBuffer9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_QueryInterface_Hooked(IDirect3DVertexBuffer9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVertexBuffer9_AddRef_Hooked(IDirect3DVertexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVertexBuffer9_Release_Hooked(IDirect3DVertexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::Release");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetDevice_Hooked(IDirect3DVertexBuffer9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_SetPrivateData_Hooked(IDirect3DVertexBuffer9 * ptr, REFGUID refguid, CONST void * pData,
                                                                              DWORD SizeOfData, DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetPrivateData_Hooked(IDirect3DVertexBuffer9 * ptr, REFGUID refguid, void * pData,
                                                                              DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_FreePrivateData_Hooked(IDirect3DVertexBuffer9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DVertexBuffer9_SetPriority_Hooked(IDirect3DVertexBuffer9 * ptr, DWORD PriorityNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::SetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].SetPriority(ptr, PriorityNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetPriority_Hooked(IDirect3DVertexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::GetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DVertexBuffer9_PreLoad_Hooked(IDirect3DVertexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::PreLoad");
    g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetType_Hooked(IDirect3DVertexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::GetType");
    D3DRESOURCETYPE result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_Lock_Hooked(IDirect3DVertexBuffer9 * ptr, UINT OffsetToLock, UINT SizeToLock, void ** ppbData,
                                                                    DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::Lock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].Lock(ptr, OffsetToLock, SizeToLock, ppbData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_Unlock_Hooked(IDirect3DVertexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::Unlock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].Unlock(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetDesc_Hooked(IDirect3DVertexBuffer9 * ptr, D3DVERTEXBUFFER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVertexBuffer9::GetDesc");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DIndexBuffer9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_QueryInterface_Hooked(IDirect3DIndexBuffer9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DIndexBuffer9_AddRef_Hooked(IDirect3DIndexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::AddRef");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DIndexBuffer9_Release_Hooked(IDirect3DIndexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::Release");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetDevice_Hooked(IDirect3DIndexBuffer9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_SetPrivateData_Hooked(IDirect3DIndexBuffer9 * ptr, REFGUID refguid, CONST void * pData, DWORD SizeOfData,
                                                                             DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetPrivateData_Hooked(IDirect3DIndexBuffer9 * ptr, REFGUID refguid, void * pData, DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_FreePrivateData_Hooked(IDirect3DIndexBuffer9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DIndexBuffer9_SetPriority_Hooked(IDirect3DIndexBuffer9 * ptr, DWORD PriorityNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::SetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].SetPriority(ptr, PriorityNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetPriority_Hooked(IDirect3DIndexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::GetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DIndexBuffer9_PreLoad_Hooked(IDirect3DIndexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::PreLoad");
    g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetType_Hooked(IDirect3DIndexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::GetType");
    D3DRESOURCETYPE result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_Lock_Hooked(IDirect3DIndexBuffer9 * ptr, UINT OffsetToLock, UINT SizeToLock, void ** ppbData,
                                                                   DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::Lock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].Lock(ptr, OffsetToLock, SizeToLock, ppbData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_Unlock_Hooked(IDirect3DIndexBuffer9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::Unlock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].Unlock(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetDesc_Hooked(IDirect3DIndexBuffer9 * ptr, D3DINDEXBUFFER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DIndexBuffer9::GetDesc");
    HRESULT result = g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DSurface9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_QueryInterface_Hooked(IDirect3DSurface9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DSurface9_AddRef_Hooked(IDirect3DSurface9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::AddRef");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DSurface9_Release_Hooked(IDirect3DSurface9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::Release");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetDevice_Hooked(IDirect3DSurface9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_SetPrivateData_Hooked(IDirect3DSurface9 * ptr, REFGUID refguid, CONST void * pData, DWORD SizeOfData,
                                                                         DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetPrivateData_Hooked(IDirect3DSurface9 * ptr, REFGUID refguid, void * pData, DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_FreePrivateData_Hooked(IDirect3DSurface9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DSurface9_SetPriority_Hooked(IDirect3DSurface9 * ptr, DWORD PriorityNew) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::SetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].SetPriority(ptr, PriorityNew);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DSurface9_GetPriority_Hooked(IDirect3DSurface9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::GetPriority");
    DWORD result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DSurface9_PreLoad_Hooked(IDirect3DSurface9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::PreLoad");
    g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DSurface9_GetType_Hooked(IDirect3DSurface9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::GetType");
    D3DRESOURCETYPE result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetContainer_Hooked(IDirect3DSurface9 * ptr, REFIID riid, void ** ppContainer) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::GetContainer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetContainer(ptr, riid, ppContainer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetDesc_Hooked(IDirect3DSurface9 * ptr, D3DSURFACE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::GetDesc");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_LockRect_Hooked(IDirect3DSurface9 * ptr, D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::LockRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].LockRect(ptr, pLockedRect, pRect, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_UnlockRect_Hooked(IDirect3DSurface9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::UnlockRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].UnlockRect(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetDC_Hooked(IDirect3DSurface9 * ptr, HDC * phdc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::GetDC");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetDC(ptr, phdc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_ReleaseDC_Hooked(IDirect3DSurface9 * ptr, HDC hdc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSurface9::ReleaseDC");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].ReleaseDC(ptr, hdc);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DVolume9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_QueryInterface_Hooked(IDirect3DVolume9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVolume9_AddRef_Hooked(IDirect3DVolume9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DVolume9_Release_Hooked(IDirect3DVolume9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::Release");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_GetDevice_Hooked(IDirect3DVolume9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_SetPrivateData_Hooked(IDirect3DVolume9 * ptr, REFGUID refguid, CONST void * pData, DWORD SizeOfData,
                                                                        DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::SetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_GetPrivateData_Hooked(IDirect3DVolume9 * ptr, REFGUID refguid, void * pData, DWORD * pSizeOfData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::GetPrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_FreePrivateData_Hooked(IDirect3DVolume9 * ptr, REFGUID refguid) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::FreePrivateData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].FreePrivateData(ptr, refguid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_GetContainer_Hooked(IDirect3DVolume9 * ptr, REFIID riid, void ** ppContainer) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::GetContainer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].GetContainer(ptr, riid, ppContainer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_GetDesc_Hooked(IDirect3DVolume9 * ptr, D3DVOLUME_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::GetDesc");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_LockBox_Hooked(IDirect3DVolume9 * ptr, D3DLOCKED_BOX * pLockedVolume, CONST D3DBOX * pBox, DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::LockBox");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].LockBox(ptr, pLockedVolume, pBox, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_UnlockBox_Hooked(IDirect3DVolume9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DVolume9::UnlockBox");
    HRESULT result = g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].UnlockBox(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DQuery9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DQuery9_QueryInterface_Hooked(IDirect3DQuery9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DQuery9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DQuery9_AddRef_Hooked(IDirect3DQuery9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DQuery9::AddRef");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DQuery9_Release_Hooked(IDirect3DQuery9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DQuery9::Release");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DQuery9_GetDevice_Hooked(IDirect3DQuery9 * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DQuery9::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3DQUERYTYPE STDMETHODCALLTYPE IDirect3DQuery9_GetType_Hooked(IDirect3DQuery9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DQuery9::GetType");
    D3DQUERYTYPE result = g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static DWORD STDMETHODCALLTYPE IDirect3DQuery9_GetDataSize_Hooked(IDirect3DQuery9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DQuery9::GetDataSize");
    DWORD result = g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].GetDataSize(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DQuery9_Issue_Hooked(IDirect3DQuery9 * ptr, DWORD dwIssueFlags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DQuery9::Issue");
    HRESULT result = g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].Issue(ptr, dwIssueFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DQuery9_GetData_Hooked(IDirect3DQuery9 * ptr, void * pData, DWORD dwSize, DWORD dwGetDataFlags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DQuery9::GetData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].GetData(ptr, pData, dwSize, dwGetDataFlags);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3D9Ex
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_QueryInterface_Hooked(IDirect3D9Ex * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::QueryInterface");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3D9Ex_AddRef_Hooked(IDirect3D9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::AddRef");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3D9Ex_Release_Hooked(IDirect3D9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::Release");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterCount_Hooked(IDirect3D9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetAdapterCount");
    UINT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterCount(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterIdentifier_Hooked(IDirect3D9Ex * ptr, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 * pIdentifier) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetAdapterIdentifier");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterIdentifier(ptr, Adapter, Flags, pIdentifier);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterModeCount_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DFORMAT Format) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetAdapterModeCount");
    UINT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterModeCount(ptr, Adapter, Format);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_EnumAdapterModes_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::EnumAdapterModes");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].EnumAdapterModes(ptr, Adapter, Format, Mode, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterDisplayMode_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDISPLAYMODE * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetAdapterDisplayMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterDisplayMode(ptr, Adapter, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDeviceType_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat,
                                                                     D3DFORMAT BackBufferFormat, BOOL bWindowed) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::CheckDeviceType");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceType(ptr, Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDeviceFormat_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat,
                                                                       DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::CheckDeviceFormat");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceFormat(ptr, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDeviceMultiSampleType_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType,
                                                                                D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType,
                                                                                DWORD * pQualityLevels) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::CheckDeviceMultiSampleType");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceMultiSampleType(ptr, Adapter, DeviceType, SurfaceFormat, Windowed,
                                                                                                MultiSampleType, pQualityLevels);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDepthStencilMatch_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat,
                                                                            D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::CheckDepthStencilMatch");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDepthStencilMatch(ptr, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDeviceFormatConversion_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType,
                                                                                 D3DFORMAT SourceFormat, D3DFORMAT TargetFormat) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::CheckDeviceFormatConversion");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceFormatConversion(ptr, Adapter, DeviceType, SourceFormat, TargetFormat);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetDeviceCaps_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 * pCaps) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetDeviceCaps");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetDeviceCaps(ptr, Adapter, DeviceType, pCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HMONITOR STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterMonitor_Hooked(IDirect3D9Ex * ptr, UINT Adapter) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetAdapterMonitor");
    HMONITOR result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterMonitor(ptr, Adapter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CreateDevice_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,
                                                                  DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters,
                                                                  IDirect3DDevice9 ** ppReturnedDeviceInterface) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::CreateDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CreateDevice(ptr, Adapter, DeviceType, hFocusWindow, BehaviorFlags,
                                                                                  pPresentationParameters, ppReturnedDeviceInterface);
    if (ppReturnedDeviceInterface && *ppReturnedDeviceInterface) { RealToHooked9(*ppReturnedDeviceInterface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterModeCountEx_Hooked(IDirect3D9Ex * ptr, UINT Adapter, CONST D3DDISPLAYMODEFILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetAdapterModeCountEx");
    UINT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterModeCountEx(ptr, Adapter, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_EnumAdapterModesEx_Hooked(IDirect3D9Ex * ptr, UINT Adapter, CONST D3DDISPLAYMODEFILTER * pFilter, UINT Mode,
                                                                        D3DDISPLAYMODEEX * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::EnumAdapterModesEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].EnumAdapterModesEx(ptr, Adapter, pFilter, Mode, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterDisplayModeEx_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDISPLAYMODEEX * pMode,
                                                                             D3DDISPLAYROTATION * pRotation) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetAdapterDisplayModeEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterDisplayModeEx(ptr, Adapter, pMode, pRotation);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CreateDeviceEx_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,
                                                                    DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters,
                                                                    D3DDISPLAYMODEEX *    pFullscreenDisplayMode,
                                                                    IDirect3DDevice9Ex ** ppReturnedDeviceInterface) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::CreateDeviceEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CreateDeviceEx(ptr, Adapter, DeviceType, hFocusWindow, BehaviorFlags,
                                                                                    pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
    if (ppReturnedDeviceInterface && *ppReturnedDeviceInterface) { RealToHooked9(*ppReturnedDeviceInterface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterLUID_Hooked(IDirect3D9Ex * ptr, UINT Adapter, LUID * pLUID) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9Ex::GetAdapterLUID");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterLUID(ptr, Adapter, pLUID);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DDevice9Ex
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_QueryInterface_Hooked(IDirect3DDevice9Ex * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DDevice9Ex_AddRef_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::AddRef");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DDevice9Ex_Release_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::Release");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_TestCooperativeLevel_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::TestCooperativeLevel");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].TestCooperativeLevel(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetAvailableTextureMem_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetAvailableTextureMem");
    UINT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetAvailableTextureMem(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_EvictManagedResources_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::EvictManagedResources");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].EvictManagedResources(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDirect3D_Hooked(IDirect3DDevice9Ex * ptr, IDirect3D9 ** ppD3D9) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetDirect3D");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDirect3D(ptr, ppD3D9);
    if (ppD3D9 && *ppD3D9) { RealToHooked9(*ppD3D9); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDeviceCaps_Hooked(IDirect3DDevice9Ex * ptr, D3DCAPS9 * pCaps) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetDeviceCaps");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDeviceCaps(ptr, pCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDisplayMode_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, D3DDISPLAYMODE * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetDisplayMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDisplayMode(ptr, iSwapChain, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetCreateParameters_Hooked(IDirect3DDevice9Ex * ptr, D3DDEVICE_CREATION_PARAMETERS * pParameters) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetCreateParameters");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetCreateParameters(ptr, pParameters);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetCursorProperties_Hooked(IDirect3DDevice9Ex * ptr, UINT XHotSpot, UINT YHotSpot,
                                                                               IDirect3DSurface9 * pCursorBitmap) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetCursorProperties");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetCursorProperties(ptr, XHotSpot, YHotSpot, pCursorBitmap);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DDevice9Ex_SetCursorPosition_Hooked(IDirect3DDevice9Ex * ptr, int X, int Y, DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetCursorPosition");
    g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetCursorPosition(ptr, X, Y, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDirect3DDevice9Ex_ShowCursor_Hooked(IDirect3DDevice9Ex * ptr, BOOL bShow) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::ShowCursor");
    BOOL result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ShowCursor(ptr, bShow);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateAdditionalSwapChain_Hooked(IDirect3DDevice9Ex * ptr, D3DPRESENT_PARAMETERS * pPresentationParameters,
                                                                                     IDirect3DSwapChain9 ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateAdditionalSwapChain");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateAdditionalSwapChain(ptr, pPresentationParameters, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked9(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetSwapChain_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, IDirect3DSwapChain9 ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetSwapChain");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetSwapChain(ptr, iSwapChain, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked9(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetNumberOfSwapChains_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetNumberOfSwapChains");
    UINT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetNumberOfSwapChains(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_Reset_Hooked(IDirect3DDevice9Ex * ptr, D3DPRESENT_PARAMETERS * pPresentationParameters) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::Reset");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].Reset(ptr, pPresentationParameters);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_Present_Hooked(IDirect3DDevice9Ex * ptr, CONST RECT * pSourceRect, CONST RECT * pDestRect,
                                                                   HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::Present");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].Present(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetBackBuffer_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type,
                                                                         IDirect3DSurface9 ** ppBackBuffer) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetBackBuffer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetBackBuffer(ptr, iSwapChain, iBackBuffer, Type, ppBackBuffer);
    if (ppBackBuffer && *ppBackBuffer) { RealToHooked9(*ppBackBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetRasterStatus_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetRasterStatus");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetRasterStatus(ptr, iSwapChain, pRasterStatus);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetDialogBoxMode_Hooked(IDirect3DDevice9Ex * ptr, BOOL bEnableDialogs) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetDialogBoxMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetDialogBoxMode(ptr, bEnableDialogs);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DDevice9Ex_SetGammaRamp_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetGammaRamp");
    g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetGammaRamp(ptr, iSwapChain, Flags, pRamp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDirect3DDevice9Ex_GetGammaRamp_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, D3DGAMMARAMP * pRamp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetGammaRamp");
    g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetGammaRamp(ptr, iSwapChain, pRamp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateTexture_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, UINT Levels, DWORD Usage,
                                                                         D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 ** ppTexture,
                                                                         HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateTexture");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateTexture(ptr, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
    if (ppTexture && *ppTexture) { RealToHooked9(*ppTexture); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateVolumeTexture_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, UINT Depth, UINT Levels,
                                                                               DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
                                                                               IDirect3DVolumeTexture9 ** ppVolumeTexture, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateVolumeTexture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVolumeTexture(ptr, Width, Height, Depth, Levels, Usage, Format, Pool,
                                                                                               ppVolumeTexture, pSharedHandle);
    if (ppVolumeTexture && *ppVolumeTexture) { RealToHooked9(*ppVolumeTexture); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateCubeTexture_Hooked(IDirect3DDevice9Ex * ptr, UINT EdgeLength, UINT Levels, DWORD Usage,
                                                                             D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9 ** ppCubeTexture,
                                                                             HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateCubeTexture");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateCubeTexture(ptr, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
    if (ppCubeTexture && *ppCubeTexture) { RealToHooked9(*ppCubeTexture); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateVertexBuffer_Hooked(IDirect3DDevice9Ex * ptr, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool,
                                                                              IDirect3DVertexBuffer9 ** ppVertexBuffer, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateVertexBuffer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexBuffer(ptr, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
    if (ppVertexBuffer && *ppVertexBuffer) { RealToHooked9(*ppVertexBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateIndexBuffer_Hooked(IDirect3DDevice9Ex * ptr, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
                                                                             IDirect3DIndexBuffer9 ** ppIndexBuffer, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateIndexBuffer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateIndexBuffer(ptr, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
    if (ppIndexBuffer && *ppIndexBuffer) { RealToHooked9(*ppIndexBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateRenderTarget_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                              D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
                                                                              IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateRenderTarget");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateRenderTarget(ptr, Width, Height, Format, MultiSample, MultisampleQuality,
                                                                                              Lockable, ppSurface, pSharedHandle);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateDepthStencilSurface_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                                     D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard,
                                                                                     IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateDepthStencilSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateDepthStencilSurface(ptr, Width, Height, Format, MultiSample,
                                                                                                     MultisampleQuality, Discard, ppSurface, pSharedHandle);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_UpdateSurface_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect,
                                                                         IDirect3DSurface9 * pDestinationSurface, CONST POINT * pDestPoint) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::UpdateSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].UpdateSurface(ptr, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_UpdateTexture_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DBaseTexture9 * pSourceTexture,
                                                                         IDirect3DBaseTexture9 * pDestinationTexture) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::UpdateTexture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].UpdateTexture(ptr, pSourceTexture, pDestinationTexture);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetRenderTargetData_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9 * pRenderTarget,
                                                                               IDirect3DSurface9 * pDestSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetRenderTargetData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderTargetData(ptr, pRenderTarget, pDestSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetFrontBufferData_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, IDirect3DSurface9 * pDestSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetFrontBufferData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetFrontBufferData(ptr, iSwapChain, pDestSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_StretchRect_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect,
                                                                       IDirect3DSurface9 * pDestSurface, CONST RECT * pDestRect, D3DTEXTUREFILTERTYPE Filter) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::StretchRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].StretchRect(ptr, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ColorFill_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9 * pSurface, CONST RECT * pRect,
                                                                     D3DCOLOR color) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::ColorFill");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ColorFill(ptr, pSurface, pRect, color);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                                       D3DPOOL Pool, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateOffscreenPlainSurface");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateOffscreenPlainSurface(ptr, Width, Height, Format, Pool, ppSurface, pSharedHandle);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetRenderTarget_Hooked(IDirect3DDevice9Ex * ptr, DWORD RenderTargetIndex,
                                                                           IDirect3DSurface9 * pRenderTarget) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetRenderTarget");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetRenderTarget(ptr, RenderTargetIndex, pRenderTarget);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetRenderTarget_Hooked(IDirect3DDevice9Ex * ptr, DWORD RenderTargetIndex,
                                                                           IDirect3DSurface9 ** ppRenderTarget) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetRenderTarget");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderTarget(ptr, RenderTargetIndex, ppRenderTarget);
    if (ppRenderTarget && *ppRenderTarget) { RealToHooked9(*ppRenderTarget); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetDepthStencilSurface_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9 * pNewZStencil) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetDepthStencilSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetDepthStencilSurface(ptr, pNewZStencil);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDepthStencilSurface_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9 ** ppZStencilSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetDepthStencilSurface");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDepthStencilSurface(ptr, ppZStencilSurface);
    if (ppZStencilSurface && *ppZStencilSurface) { RealToHooked9(*ppZStencilSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_BeginScene_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::BeginScene");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].BeginScene(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_EndScene_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::EndScene");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].EndScene(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_Clear_Hooked(IDirect3DDevice9Ex * ptr, DWORD Count, CONST D3DRECT * pRects, DWORD Flags, D3DCOLOR Color,
                                                                 float Z, DWORD Stencil) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::Clear");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].Clear(ptr, Count, pRects, Flags, Color, Z, Stencil);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetTransform_Hooked(IDirect3DDevice9Ex * ptr, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetTransform");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetTransform(ptr, State, pMatrix);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetTransform_Hooked(IDirect3DDevice9Ex * ptr, D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetTransform");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetTransform(ptr, State, pMatrix);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_MultiplyTransform_Hooked(IDirect3DDevice9Ex * ptr, D3DTRANSFORMSTATETYPE TransformType,
                                                                             CONST D3DMATRIX * Value) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::MultiplyTransform");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].MultiplyTransform(ptr, TransformType, Value);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetViewport_Hooked(IDirect3DDevice9Ex * ptr, CONST D3DVIEWPORT9 * pViewport) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetViewport");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetViewport(ptr, pViewport);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetViewport_Hooked(IDirect3DDevice9Ex * ptr, D3DVIEWPORT9 * pViewport) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetViewport");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetViewport(ptr, pViewport);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetMaterial_Hooked(IDirect3DDevice9Ex * ptr, CONST D3DMATERIAL9 * pMaterial) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetMaterial");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetMaterial(ptr, pMaterial);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetMaterial_Hooked(IDirect3DDevice9Ex * ptr, D3DMATERIAL9 * pMaterial) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetMaterial");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetMaterial(ptr, pMaterial);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetLight_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, CONST D3DLIGHT9 * pLight) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetLight");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetLight(ptr, Index, pLight);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetLight_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, D3DLIGHT9 * pLight) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetLight");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetLight(ptr, Index, pLight);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_LightEnable_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, BOOL Enable) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::LightEnable");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].LightEnable(ptr, Index, Enable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetLightEnable_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, BOOL * pEnable) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetLightEnable");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetLightEnable(ptr, Index, pEnable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetClipPlane_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, CONST float * pPlane) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetClipPlane");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetClipPlane(ptr, Index, pPlane);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetClipPlane_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, float * pPlane) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetClipPlane");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetClipPlane(ptr, Index, pPlane);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetRenderState_Hooked(IDirect3DDevice9Ex * ptr, D3DRENDERSTATETYPE State, DWORD Value) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetRenderState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetRenderState(ptr, State, Value);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetRenderState_Hooked(IDirect3DDevice9Ex * ptr, D3DRENDERSTATETYPE State, DWORD * pValue) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetRenderState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderState(ptr, State, pValue);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateStateBlock_Hooked(IDirect3DDevice9Ex * ptr, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 ** ppSB) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateStateBlock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateStateBlock(ptr, Type, ppSB);
    if (ppSB && *ppSB) { RealToHooked9(*ppSB); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_BeginStateBlock_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::BeginStateBlock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].BeginStateBlock(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_EndStateBlock_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DStateBlock9 ** ppSB) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::EndStateBlock");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].EndStateBlock(ptr, ppSB);
    if (ppSB && *ppSB) { RealToHooked9(*ppSB); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetClipStatus_Hooked(IDirect3DDevice9Ex * ptr, CONST D3DCLIPSTATUS9 * pClipStatus) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetClipStatus");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetClipStatus(ptr, pClipStatus);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetClipStatus_Hooked(IDirect3DDevice9Ex * ptr, D3DCLIPSTATUS9 * pClipStatus) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetClipStatus");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetClipStatus(ptr, pClipStatus);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetTexture_Hooked(IDirect3DDevice9Ex * ptr, DWORD Stage, IDirect3DBaseTexture9 ** ppTexture) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetTexture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetTexture(ptr, Stage, ppTexture);
    if (ppTexture && *ppTexture) { RealToHooked9(*ppTexture); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetTexture_Hooked(IDirect3DDevice9Ex * ptr, DWORD Stage, IDirect3DBaseTexture9 * pTexture) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetTexture");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetTexture(ptr, Stage, pTexture);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetTextureStageState_Hooked(IDirect3DDevice9Ex * ptr, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type,
                                                                                DWORD * pValue) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetTextureStageState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetTextureStageState(ptr, Stage, Type, pValue);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetTextureStageState_Hooked(IDirect3DDevice9Ex * ptr, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type,
                                                                                DWORD Value) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetTextureStageState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetTextureStageState(ptr, Stage, Type, Value);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetSamplerState_Hooked(IDirect3DDevice9Ex * ptr, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetSamplerState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetSamplerState(ptr, Sampler, Type, pValue);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetSamplerState_Hooked(IDirect3DDevice9Ex * ptr, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetSamplerState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetSamplerState(ptr, Sampler, Type, Value);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ValidateDevice_Hooked(IDirect3DDevice9Ex * ptr, DWORD * pNumPasses) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::ValidateDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ValidateDevice(ptr, pNumPasses);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPaletteEntries_Hooked(IDirect3DDevice9Ex * ptr, UINT PaletteNumber, CONST PALETTEENTRY * pEntries) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetPaletteEntries");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPaletteEntries(ptr, PaletteNumber, pEntries);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPaletteEntries_Hooked(IDirect3DDevice9Ex * ptr, UINT PaletteNumber, PALETTEENTRY * pEntries) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetPaletteEntries");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPaletteEntries(ptr, PaletteNumber, pEntries);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetCurrentTexturePalette_Hooked(IDirect3DDevice9Ex * ptr, UINT PaletteNumber) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetCurrentTexturePalette");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetCurrentTexturePalette(ptr, PaletteNumber);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetCurrentTexturePalette_Hooked(IDirect3DDevice9Ex * ptr, UINT * PaletteNumber) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetCurrentTexturePalette");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetCurrentTexturePalette(ptr, PaletteNumber);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetScissorRect_Hooked(IDirect3DDevice9Ex * ptr, CONST RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetScissorRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetScissorRect(ptr, pRect);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetScissorRect_Hooked(IDirect3DDevice9Ex * ptr, RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetScissorRect");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetScissorRect(ptr, pRect);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Hooked(IDirect3DDevice9Ex * ptr, BOOL bSoftware) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetSoftwareVertexProcessing");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetSoftwareVertexProcessing(ptr, bSoftware);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetSoftwareVertexProcessing");
    BOOL result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetSoftwareVertexProcessing(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetNPatchMode_Hooked(IDirect3DDevice9Ex * ptr, float nSegments) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetNPatchMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetNPatchMode(ptr, nSegments);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static float STDMETHODCALLTYPE IDirect3DDevice9Ex_GetNPatchMode_Hooked(IDirect3DDevice9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetNPatchMode");
    float result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetNPatchMode(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawPrimitive_Hooked(IDirect3DDevice9Ex * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex,
                                                                         UINT PrimitiveCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::DrawPrimitive");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawPrimitive(ptr, PrimitiveType, StartVertex, PrimitiveCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawIndexedPrimitive_Hooked(IDirect3DDevice9Ex * ptr, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex,
                                                                                UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::DrawIndexedPrimitive");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawIndexedPrimitive(ptr, PrimitiveType, BaseVertexIndex, MinVertexIndex,
                                                                                                NumVertices, startIndex, primCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawPrimitiveUP_Hooked(IDirect3DDevice9Ex * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount,
                                                                           CONST void * pVertexStreamZeroData, UINT VertexStreamZeroStride) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::DrawPrimitiveUP");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawPrimitiveUP(ptr, PrimitiveType, PrimitiveCount, pVertexStreamZeroData,
                                                                                           VertexStreamZeroStride);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Hooked(IDirect3DDevice9Ex * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex,
                                                                                  UINT NumVertices, UINT PrimitiveCount, CONST void * pIndexData,
                                                                                  D3DFORMAT IndexDataFormat, CONST void * pVertexStreamZeroData,
                                                                                  UINT VertexStreamZeroStride) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::DrawIndexedPrimitiveUP");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawIndexedPrimitiveUP(
        ptr, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ProcessVertices_Hooked(IDirect3DDevice9Ex * ptr, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount,
                                                                           IDirect3DVertexBuffer9 * pDestBuffer, IDirect3DVertexDeclaration9 * pVertexDecl,
                                                                           DWORD Flags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::ProcessVertices");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ProcessVertices(ptr, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateVertexDeclaration_Hooked(IDirect3DDevice9Ex * ptr, CONST D3DVERTEXELEMENT9 * pVertexElements,
                                                                                   IDirect3DVertexDeclaration9 ** ppDecl) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateVertexDeclaration");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexDeclaration(ptr, pVertexElements, ppDecl);
    if (ppDecl && *ppDecl) { RealToHooked9(*ppDecl); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexDeclaration_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DVertexDeclaration9 * pDecl) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetVertexDeclaration");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexDeclaration(ptr, pDecl);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexDeclaration_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DVertexDeclaration9 ** ppDecl) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetVertexDeclaration");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexDeclaration(ptr, ppDecl);
    if (ppDecl && *ppDecl) { RealToHooked9(*ppDecl); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetFVF_Hooked(IDirect3DDevice9Ex * ptr, DWORD FVF) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetFVF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetFVF(ptr, FVF);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetFVF_Hooked(IDirect3DDevice9Ex * ptr, DWORD * pFVF) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetFVF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetFVF(ptr, pFVF);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateVertexShader_Hooked(IDirect3DDevice9Ex * ptr, CONST DWORD * pFunction,
                                                                              IDirect3DVertexShader9 ** ppShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateVertexShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexShader(ptr, pFunction, ppShader);
    if (ppShader && *ppShader) { RealToHooked9(*ppShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexShader_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DVertexShader9 * pShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetVertexShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShader(ptr, pShader);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexShader_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DVertexShader9 ** ppShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetVertexShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShader(ptr, ppShader);
    if (ppShader && *ppShader) { RealToHooked9(*ppShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexShaderConstantF_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST float * pConstantData,
                                                                                    UINT Vector4fCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetVertexShaderConstantF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexShaderConstantF_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, float * pConstantData,
                                                                                    UINT Vector4fCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetVertexShaderConstantF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexShaderConstantI_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST int * pConstantData,
                                                                                    UINT Vector4iCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetVertexShaderConstantI");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexShaderConstantI_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, int * pConstantData,
                                                                                    UINT Vector4iCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetVertexShaderConstantI");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexShaderConstantB_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST BOOL * pConstantData,
                                                                                    UINT BoolCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetVertexShaderConstantB");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexShaderConstantB_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, BOOL * pConstantData,
                                                                                    UINT BoolCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetVertexShaderConstantB");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetStreamSource_Hooked(IDirect3DDevice9Ex * ptr, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData,
                                                                           UINT OffsetInBytes, UINT Stride) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetStreamSource");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetStreamSource(ptr, StreamNumber, pStreamData, OffsetInBytes, Stride);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetStreamSource_Hooked(IDirect3DDevice9Ex * ptr, UINT StreamNumber, IDirect3DVertexBuffer9 ** ppStreamData,
                                                                           UINT * pOffsetInBytes, UINT * pStride) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetStreamSource");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetStreamSource(ptr, StreamNumber, ppStreamData, pOffsetInBytes, pStride);
    if (ppStreamData && *ppStreamData) { RealToHooked9(*ppStreamData); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetStreamSourceFreq_Hooked(IDirect3DDevice9Ex * ptr, UINT StreamNumber, UINT Setting) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetStreamSourceFreq");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetStreamSourceFreq(ptr, StreamNumber, Setting);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetStreamSourceFreq_Hooked(IDirect3DDevice9Ex * ptr, UINT StreamNumber, UINT * pSetting) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetStreamSourceFreq");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetStreamSourceFreq(ptr, StreamNumber, pSetting);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetIndices_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DIndexBuffer9 * pIndexData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetIndices");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetIndices(ptr, pIndexData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetIndices_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DIndexBuffer9 ** ppIndexData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetIndices");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetIndices(ptr, ppIndexData);
    if (ppIndexData && *ppIndexData) { RealToHooked9(*ppIndexData); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreatePixelShader_Hooked(IDirect3DDevice9Ex * ptr, CONST DWORD * pFunction,
                                                                             IDirect3DPixelShader9 ** ppShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreatePixelShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreatePixelShader(ptr, pFunction, ppShader);
    if (ppShader && *ppShader) { RealToHooked9(*ppShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPixelShader_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DPixelShader9 * pShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetPixelShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShader(ptr, pShader);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPixelShader_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DPixelShader9 ** ppShader) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetPixelShader");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShader(ptr, ppShader);
    if (ppShader && *ppShader) { RealToHooked9(*ppShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPixelShaderConstantF_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST float * pConstantData,
                                                                                   UINT Vector4fCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetPixelShaderConstantF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPixelShaderConstantF_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, float * pConstantData,
                                                                                   UINT Vector4fCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetPixelShaderConstantF");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPixelShaderConstantI_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST int * pConstantData,
                                                                                   UINT Vector4iCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetPixelShaderConstantI");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPixelShaderConstantI_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, int * pConstantData,
                                                                                   UINT Vector4iCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetPixelShaderConstantI");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPixelShaderConstantB_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST BOOL * pConstantData,
                                                                                   UINT BoolCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetPixelShaderConstantB");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPixelShaderConstantB_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, BOOL * pConstantData,
                                                                                   UINT BoolCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetPixelShaderConstantB");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawRectPatch_Hooked(IDirect3DDevice9Ex * ptr, UINT Handle, CONST float * pNumSegs,
                                                                         CONST D3DRECTPATCH_INFO * pRectPatchInfo) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::DrawRectPatch");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawRectPatch(ptr, Handle, pNumSegs, pRectPatchInfo);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawTriPatch_Hooked(IDirect3DDevice9Ex * ptr, UINT Handle, CONST float * pNumSegs,
                                                                        CONST D3DTRIPATCH_INFO * pTriPatchInfo) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::DrawTriPatch");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawTriPatch(ptr, Handle, pNumSegs, pTriPatchInfo);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DeletePatch_Hooked(IDirect3DDevice9Ex * ptr, UINT Handle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::DeletePatch");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DeletePatch(ptr, Handle);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateQuery_Hooked(IDirect3DDevice9Ex * ptr, D3DQUERYTYPE Type, IDirect3DQuery9 ** ppQuery) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateQuery");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateQuery(ptr, Type, ppQuery);
    if (ppQuery && *ppQuery) { RealToHooked9(*ppQuery); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetConvolutionMonoKernel_Hooked(IDirect3DDevice9Ex * ptr, UINT width, UINT height, float * rows,
                                                                                    float * columns) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetConvolutionMonoKernel");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetConvolutionMonoKernel(ptr, width, height, rows, columns);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ComposeRects_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9 * pSrc, IDirect3DSurface9 * pDst,
                                                                        IDirect3DVertexBuffer9 * pSrcRectDescs, UINT NumRects,
                                                                        IDirect3DVertexBuffer9 * pDstRectDescs, D3DCOMPOSERECTSOP Operation, int Xoffset,
                                                                        int Yoffset) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::ComposeRects");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ComposeRects(ptr, pSrc, pDst, pSrcRectDescs, NumRects, pDstRectDescs, Operation,
                                                                                        Xoffset, Yoffset);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_PresentEx_Hooked(IDirect3DDevice9Ex * ptr, CONST RECT * pSourceRect, CONST RECT * pDestRect,
                                                                     HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion, DWORD dwFlags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::PresentEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].PresentEx(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetGPUThreadPriority_Hooked(IDirect3DDevice9Ex * ptr, INT * pPriority) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetGPUThreadPriority");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetGPUThreadPriority(ptr, pPriority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetGPUThreadPriority_Hooked(IDirect3DDevice9Ex * ptr, INT Priority) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetGPUThreadPriority");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetGPUThreadPriority(ptr, Priority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_WaitForVBlank_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::WaitForVBlank");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].WaitForVBlank(ptr, iSwapChain);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CheckResourceResidency_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DResource9 ** pResourceArray,
                                                                                  UINT32 NumResources) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CheckResourceResidency");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CheckResourceResidency(ptr, pResourceArray, NumResources);
    if (pResourceArray && *pResourceArray) { RealToHooked9(*pResourceArray); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetMaximumFrameLatency_Hooked(IDirect3DDevice9Ex * ptr, UINT MaxLatency) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::SetMaximumFrameLatency");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetMaximumFrameLatency(ptr, MaxLatency);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetMaximumFrameLatency_Hooked(IDirect3DDevice9Ex * ptr, UINT * pMaxLatency) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetMaximumFrameLatency");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetMaximumFrameLatency(ptr, pMaxLatency);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CheckDeviceState_Hooked(IDirect3DDevice9Ex * ptr, HWND hDestinationWindow) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CheckDeviceState");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CheckDeviceState(ptr, hDestinationWindow);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateRenderTargetEx_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                                D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
                                                                                IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle, DWORD Usage) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateRenderTargetEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateRenderTargetEx(ptr, Width, Height, Format, MultiSample, MultisampleQuality,
                                                                                                Lockable, ppSurface, pSharedHandle, Usage);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateOffscreenPlainSurfaceEx_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                                         D3DPOOL Pool, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle,
                                                                                         DWORD Usage) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateOffscreenPlainSurfaceEx");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateOffscreenPlainSurfaceEx(ptr, Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateDepthStencilSurfaceEx_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format,
                                                                                       D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard,
                                                                                       IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle, DWORD Usage) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::CreateDepthStencilSurfaceEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateDepthStencilSurfaceEx(
        ptr, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage);
    if (ppSurface && *ppSurface) { RealToHooked9(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ResetEx_Hooked(IDirect3DDevice9Ex * ptr, D3DPRESENT_PARAMETERS * pPresentationParameters,
                                                                   D3DDISPLAYMODEEX * pFullscreenDisplayMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::ResetEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ResetEx(ptr, pPresentationParameters, pFullscreenDisplayMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDisplayModeEx_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, D3DDISPLAYMODEEX * pMode,
                                                                            D3DDISPLAYROTATION * pRotation) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Ex::GetDisplayModeEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDisplayModeEx(ptr, iSwapChain, pMode, pRotation);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DSwapChain9Ex
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_QueryInterface_Hooked(IDirect3DSwapChain9Ex * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DSwapChain9Ex_AddRef_Hooked(IDirect3DSwapChain9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::AddRef");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DSwapChain9Ex_Release_Hooked(IDirect3DSwapChain9Ex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::Release");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_Present_Hooked(IDirect3DSwapChain9Ex * ptr, CONST RECT * pSourceRect, CONST RECT * pDestRect,
                                                                      HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion, DWORD dwFlags) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::Present");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].Present(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetFrontBufferData_Hooked(IDirect3DSwapChain9Ex * ptr, IDirect3DSurface9 * pDestSurface) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetFrontBufferData");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetFrontBufferData(ptr, pDestSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetBackBuffer_Hooked(IDirect3DSwapChain9Ex * ptr, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type,
                                                                            IDirect3DSurface9 ** ppBackBuffer) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetBackBuffer");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetBackBuffer(ptr, iBackBuffer, Type, ppBackBuffer);
    if (ppBackBuffer && *ppBackBuffer) { RealToHooked9(*ppBackBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetRasterStatus_Hooked(IDirect3DSwapChain9Ex * ptr, D3DRASTER_STATUS * pRasterStatus) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetRasterStatus");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetRasterStatus(ptr, pRasterStatus);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetDisplayMode_Hooked(IDirect3DSwapChain9Ex * ptr, D3DDISPLAYMODE * pMode) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetDisplayMode");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDisplayMode(ptr, pMode);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetDevice_Hooked(IDirect3DSwapChain9Ex * ptr, IDirect3DDevice9 ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetDevice");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked9(*ppDevice); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetPresentParameters_Hooked(IDirect3DSwapChain9Ex * ptr,
                                                                                   D3DPRESENT_PARAMETERS * pPresentationParameters) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetPresentParameters");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetPresentParameters(ptr, pPresentationParameters);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetLastPresentCount_Hooked(IDirect3DSwapChain9Ex * ptr, UINT * pLastPresentCount) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetLastPresentCount");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetLastPresentCount(ptr, pLastPresentCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetPresentStats_Hooked(IDirect3DSwapChain9Ex * ptr, D3DPRESENTSTATS * pPresentationStatistics) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetPresentStats");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetPresentStats(ptr, pPresentationStatistics);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetDisplayModeEx_Hooked(IDirect3DSwapChain9Ex * ptr, D3DDISPLAYMODEEX * pMode,
                                                                               D3DDISPLAYROTATION * pRotation) {
    GN_D3DHOOK_CALLTRACE("IDirect3DSwapChain9Ex::GetDisplayModeEx");
    HRESULT result = g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDisplayModeEx(ptr, pMode, pRotation);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3D9ExOverlayExtension
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9ExOverlayExtension_QueryInterface_Hooked(IDirect3D9ExOverlayExtension * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9ExOverlayExtension::QueryInterface");
    return g_D3D9OriginVTables._IDirect3D9ExOverlayExtension.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3D9ExOverlayExtension_AddRef_Hooked(IDirect3D9ExOverlayExtension * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9ExOverlayExtension::AddRef");
    return g_D3D9OriginVTables._IDirect3D9ExOverlayExtension.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3D9ExOverlayExtension_Release_Hooked(IDirect3D9ExOverlayExtension * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9ExOverlayExtension::Release");
    return g_D3D9OriginVTables._IDirect3D9ExOverlayExtension.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3D9ExOverlayExtension_CheckDeviceOverlayType_Hooked(IDirect3D9ExOverlayExtension * ptr, UINT Adapter,
                                                                                            D3DDEVTYPE DevType, UINT OverlayWidth, UINT OverlayHeight,
                                                                                            D3DFORMAT OverlayFormat, D3DDISPLAYMODEEX * pDisplayMode,
                                                                                            D3DDISPLAYROTATION DisplayRotation, D3DOVERLAYCAPS * pOverlayCaps) {
    GN_D3DHOOK_CALLTRACE("IDirect3D9ExOverlayExtension::CheckDeviceOverlayType");
    HRESULT result = g_D3D9OriginVTables._IDirect3D9ExOverlayExtension.tables[INDEX].CheckDeviceOverlayType(
        ptr, Adapter, DevType, OverlayWidth, OverlayHeight, OverlayFormat, pDisplayMode, DisplayRotation, pOverlayCaps);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DDevice9Video
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Video_QueryInterface_Hooked(IDirect3DDevice9Video * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Video::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DDevice9Video_AddRef_Hooked(IDirect3DDevice9Video * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Video::AddRef");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DDevice9Video_Release_Hooked(IDirect3DDevice9Video * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Video::Release");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Video_GetContentProtectionCaps_Hooked(IDirect3DDevice9Video * ptr, CONST GUID * pCryptoType,
                                                                                       CONST GUID * pDecodeProfile, D3DCONTENTPROTECTIONCAPS * pCaps) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Video::GetContentProtectionCaps");
    HRESULT result = g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].GetContentProtectionCaps(ptr, pCryptoType, pDecodeProfile, pCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Video_CreateAuthenticatedChannel_Hooked(IDirect3DDevice9Video * ptr, D3DAUTHENTICATEDCHANNELTYPE ChannelType,
                                                                                         IDirect3DAuthenticatedChannel9 ** ppAuthenticatedChannel,
                                                                                         HANDLE *                          pChannelHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Video::CreateAuthenticatedChannel");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].CreateAuthenticatedChannel(ptr, ChannelType, ppAuthenticatedChannel, pChannelHandle);
    if (ppAuthenticatedChannel && *ppAuthenticatedChannel) { RealToHooked9(*ppAuthenticatedChannel); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Video_CreateCryptoSession_Hooked(IDirect3DDevice9Video * ptr, CONST GUID * pCryptoType,
                                                                                  CONST GUID * pDecodeProfile, IDirect3DCryptoSession9 ** ppCryptoSession,
                                                                                  HANDLE * pCryptoHandle) {
    GN_D3DHOOK_CALLTRACE("IDirect3DDevice9Video::CreateCryptoSession");
    HRESULT result =
        g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].CreateCryptoSession(ptr, pCryptoType, pDecodeProfile, ppCryptoSession, pCryptoHandle);
    if (ppCryptoSession && *ppCryptoSession) { RealToHooked9(*ppCryptoSession); }
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DAuthenticatedChannel9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_QueryInterface_Hooked(IDirect3DAuthenticatedChannel9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DAuthenticatedChannel9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_AddRef_Hooked(IDirect3DAuthenticatedChannel9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DAuthenticatedChannel9::AddRef");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_Release_Hooked(IDirect3DAuthenticatedChannel9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DAuthenticatedChannel9::Release");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_GetCertificateSize_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT * pCertificateSize) {
    GN_D3DHOOK_CALLTRACE("IDirect3DAuthenticatedChannel9::GetCertificateSize");
    HRESULT result = g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].GetCertificateSize(ptr, pCertificateSize);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_GetCertificate_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT CertifacteSize,
                                                                                      BYTE * ppCertificate) {
    GN_D3DHOOK_CALLTRACE("IDirect3DAuthenticatedChannel9::GetCertificate");
    HRESULT result = g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].GetCertificate(ptr, CertifacteSize, ppCertificate);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_NegotiateKeyExchange_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT DataSize, VOID * pData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DAuthenticatedChannel9::NegotiateKeyExchange");
    HRESULT result = g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].NegotiateKeyExchange(ptr, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_Query_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT InputSize, CONST VOID * pInput,
                                                                             UINT OutputSize, VOID * pOutput) {
    GN_D3DHOOK_CALLTRACE("IDirect3DAuthenticatedChannel9::Query");
    HRESULT result = g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Query(ptr, InputSize, pInput, OutputSize, pOutput);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_Configure_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT InputSize, CONST VOID * pInput,
                                                                                 D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT * pOutput) {
    GN_D3DHOOK_CALLTRACE("IDirect3DAuthenticatedChannel9::Configure");
    HRESULT result = g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Configure(ptr, InputSize, pInput, pOutput);
    return result;
}

// -----------------------------------------------------------------------------
// IDirect3DCryptoSession9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_QueryInterface_Hooked(IDirect3DCryptoSession9 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DCryptoSession9_AddRef_Hooked(IDirect3DCryptoSession9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::AddRef");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDirect3DCryptoSession9_Release_Hooked(IDirect3DCryptoSession9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::Release");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_GetCertificateSize_Hooked(IDirect3DCryptoSession9 * ptr, UINT * pCertificateSize) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::GetCertificateSize");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].GetCertificateSize(ptr, pCertificateSize);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_GetCertificate_Hooked(IDirect3DCryptoSession9 * ptr, UINT CertifacteSize, BYTE * ppCertificate) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::GetCertificate");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].GetCertificate(ptr, CertifacteSize, ppCertificate);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_NegotiateKeyExchange_Hooked(IDirect3DCryptoSession9 * ptr, UINT DataSize, VOID * pData) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::NegotiateKeyExchange");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].NegotiateKeyExchange(ptr, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_EncryptionBlt_Hooked(IDirect3DCryptoSession9 * ptr, IDirect3DSurface9 * pSrcSurface,
                                                                              IDirect3DSurface9 * pDstSurface, UINT DstSurfaceSize, VOID * pIV) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::EncryptionBlt");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].EncryptionBlt(ptr, pSrcSurface, pDstSurface, DstSurfaceSize, pIV);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_DecryptionBlt_Hooked(IDirect3DCryptoSession9 * ptr, IDirect3DSurface9 * pSrcSurface,
                                                                              IDirect3DSurface9 * pDstSurface, UINT SrcSurfaceSize,
                                                                              D3DENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo, VOID * pContentKey, VOID * pIV) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::DecryptionBlt");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].DecryptionBlt(ptr, pSrcSurface, pDstSurface, SrcSurfaceSize,
                                                                                              pEncryptedBlockInfo, pContentKey, pIV);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_GetSurfacePitch_Hooked(IDirect3DCryptoSession9 * ptr, IDirect3DSurface9 * pSrcSurface,
                                                                                UINT * pSurfacePitch) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::GetSurfacePitch");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].GetSurfacePitch(ptr, pSrcSurface, pSurfacePitch);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_StartSessionKeyRefresh_Hooked(IDirect3DCryptoSession9 * ptr, VOID * pRandomNumber,
                                                                                       UINT RandomNumberSize) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::StartSessionKeyRefresh");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].StartSessionKeyRefresh(ptr, pRandomNumber, RandomNumberSize);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_FinishSessionKeyRefresh_Hooked(IDirect3DCryptoSession9 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::FinishSessionKeyRefresh");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].FinishSessionKeyRefresh(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_GetEncryptionBltKey_Hooked(IDirect3DCryptoSession9 * ptr, VOID * pReadbackKey, UINT KeySize) {
    GN_D3DHOOK_CALLTRACE("IDirect3DCryptoSession9::GetEncryptionBltKey");
    HRESULT result = g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].GetEncryptionBltKey(ptr, pReadbackKey, KeySize);
    return result;
}

// -----------------------------------------------------------------------------
void RealToHooked9(const IID & iid, void * p) {
    if (false) {
    } else if (__uuidof(IDirect3D9) == iid)
        RealToHooked9_IDirect3D9((IDirect3D9 *) p);
    else if (__uuidof(IDirect3DDevice9) == iid)
        RealToHooked9_IDirect3DDevice9((IDirect3DDevice9 *) p);
    else if (__uuidof(IDirect3DStateBlock9) == iid)
        RealToHooked9_IDirect3DStateBlock9((IDirect3DStateBlock9 *) p);
    else if (__uuidof(IDirect3DSwapChain9) == iid)
        RealToHooked9_IDirect3DSwapChain9((IDirect3DSwapChain9 *) p);
    else if (__uuidof(IDirect3DResource9) == iid)
        RealToHooked9_IDirect3DResource9((IDirect3DResource9 *) p);
    else if (__uuidof(IDirect3DVertexDeclaration9) == iid)
        RealToHooked9_IDirect3DVertexDeclaration9((IDirect3DVertexDeclaration9 *) p);
    else if (__uuidof(IDirect3DVertexShader9) == iid)
        RealToHooked9_IDirect3DVertexShader9((IDirect3DVertexShader9 *) p);
    else if (__uuidof(IDirect3DPixelShader9) == iid)
        RealToHooked9_IDirect3DPixelShader9((IDirect3DPixelShader9 *) p);
    else if (__uuidof(IDirect3DBaseTexture9) == iid)
        RealToHooked9_IDirect3DBaseTexture9((IDirect3DBaseTexture9 *) p);
    else if (__uuidof(IDirect3DTexture9) == iid)
        RealToHooked9_IDirect3DTexture9((IDirect3DTexture9 *) p);
    else if (__uuidof(IDirect3DVolumeTexture9) == iid)
        RealToHooked9_IDirect3DVolumeTexture9((IDirect3DVolumeTexture9 *) p);
    else if (__uuidof(IDirect3DCubeTexture9) == iid)
        RealToHooked9_IDirect3DCubeTexture9((IDirect3DCubeTexture9 *) p);
    else if (__uuidof(IDirect3DVertexBuffer9) == iid)
        RealToHooked9_IDirect3DVertexBuffer9((IDirect3DVertexBuffer9 *) p);
    else if (__uuidof(IDirect3DIndexBuffer9) == iid)
        RealToHooked9_IDirect3DIndexBuffer9((IDirect3DIndexBuffer9 *) p);
    else if (__uuidof(IDirect3DSurface9) == iid)
        RealToHooked9_IDirect3DSurface9((IDirect3DSurface9 *) p);
    else if (__uuidof(IDirect3DVolume9) == iid)
        RealToHooked9_IDirect3DVolume9((IDirect3DVolume9 *) p);
    else if (__uuidof(IDirect3DQuery9) == iid)
        RealToHooked9_IDirect3DQuery9((IDirect3DQuery9 *) p);
    else if (__uuidof(IDirect3D9Ex) == iid)
        RealToHooked9_IDirect3D9Ex((IDirect3D9Ex *) p);
    else if (__uuidof(IDirect3DDevice9Ex) == iid)
        RealToHooked9_IDirect3DDevice9Ex((IDirect3DDevice9Ex *) p);
    else if (__uuidof(IDirect3DSwapChain9Ex) == iid)
        RealToHooked9_IDirect3DSwapChain9Ex((IDirect3DSwapChain9Ex *) p);
    else if (__uuidof(IDirect3D9ExOverlayExtension) == iid)
        RealToHooked9_IDirect3D9ExOverlayExtension((IDirect3D9ExOverlayExtension *) p);
    else if (__uuidof(IDirect3DDevice9Video) == iid)
        RealToHooked9_IDirect3DDevice9Video((IDirect3DDevice9Video *) p);
    else if (__uuidof(IDirect3DAuthenticatedChannel9) == iid)
        RealToHooked9_IDirect3DAuthenticatedChannel9((IDirect3DAuthenticatedChannel9 *) p);
    else if (__uuidof(IDirect3DCryptoSession9) == iid)
        RealToHooked9_IDirect3DCryptoSession9((IDirect3DCryptoSession9 *) p);
    else {
        HOOK_WARN_LOG("unrecognized interface UUID: <xxxx-xxxx-xxxxx...>");
    }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void SetupD3D9HookedVTables() {
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].QueryInterface                           = IDirect3D9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].AddRef                                   = IDirect3D9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].Release                                  = IDirect3D9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].RegisterSoftwareDevice                   = IDirect3D9_RegisterSoftwareDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterCount                          = IDirect3D9_GetAdapterCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterIdentifier                     = IDirect3D9_GetAdapterIdentifier_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterModeCount                      = IDirect3D9_GetAdapterModeCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].EnumAdapterModes                         = IDirect3D9_EnumAdapterModes_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterDisplayMode                    = IDirect3D9_GetAdapterDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDeviceType                          = IDirect3D9_CheckDeviceType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDeviceFormat                        = IDirect3D9_CheckDeviceFormat_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDeviceMultiSampleType               = IDirect3D9_CheckDeviceMultiSampleType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDepthStencilMatch                   = IDirect3D9_CheckDepthStencilMatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDeviceFormatConversion              = IDirect3D9_CheckDeviceFormatConversion_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetDeviceCaps                            = IDirect3D9_GetDeviceCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterMonitor                        = IDirect3D9_GetAdapterMonitor_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CreateDevice                             = IDirect3D9_CreateDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].QueryInterface                     = IDirect3DDevice9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].AddRef                             = IDirect3DDevice9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].Release                            = IDirect3DDevice9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].TestCooperativeLevel               = IDirect3DDevice9_TestCooperativeLevel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetAvailableTextureMem             = IDirect3DDevice9_GetAvailableTextureMem_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].EvictManagedResources              = IDirect3DDevice9_EvictManagedResources_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetDirect3D                        = IDirect3DDevice9_GetDirect3D_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetDeviceCaps                      = IDirect3DDevice9_GetDeviceCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetDisplayMode                     = IDirect3DDevice9_GetDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetCreateParameters                = IDirect3DDevice9_GetCreateParameters_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetCursorProperties                = IDirect3DDevice9_SetCursorProperties_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetCursorPosition                  = IDirect3DDevice9_SetCursorPosition_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].ShowCursor                         = IDirect3DDevice9_ShowCursor_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateAdditionalSwapChain          = IDirect3DDevice9_CreateAdditionalSwapChain_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetSwapChain                       = IDirect3DDevice9_GetSwapChain_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetNumberOfSwapChains              = IDirect3DDevice9_GetNumberOfSwapChains_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].Reset                              = IDirect3DDevice9_Reset_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].Present                            = IDirect3DDevice9_Present_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetBackBuffer                      = IDirect3DDevice9_GetBackBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetRasterStatus                    = IDirect3DDevice9_GetRasterStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetDialogBoxMode                   = IDirect3DDevice9_SetDialogBoxMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetGammaRamp                       = IDirect3DDevice9_SetGammaRamp_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetGammaRamp                       = IDirect3DDevice9_GetGammaRamp_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateTexture                      = IDirect3DDevice9_CreateTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateVolumeTexture                = IDirect3DDevice9_CreateVolumeTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateCubeTexture                  = IDirect3DDevice9_CreateCubeTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateVertexBuffer                 = IDirect3DDevice9_CreateVertexBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateIndexBuffer                  = IDirect3DDevice9_CreateIndexBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateRenderTarget                 = IDirect3DDevice9_CreateRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateDepthStencilSurface          = IDirect3DDevice9_CreateDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].UpdateSurface                      = IDirect3DDevice9_UpdateSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].UpdateTexture                      = IDirect3DDevice9_UpdateTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetRenderTargetData                = IDirect3DDevice9_GetRenderTargetData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetFrontBufferData                 = IDirect3DDevice9_GetFrontBufferData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].StretchRect                        = IDirect3DDevice9_StretchRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].ColorFill                          = IDirect3DDevice9_ColorFill_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateOffscreenPlainSurface        = IDirect3DDevice9_CreateOffscreenPlainSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetRenderTarget                    = IDirect3DDevice9_SetRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetRenderTarget                    = IDirect3DDevice9_GetRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetDepthStencilSurface             = IDirect3DDevice9_SetDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetDepthStencilSurface             = IDirect3DDevice9_GetDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].BeginScene                         = IDirect3DDevice9_BeginScene_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].EndScene                           = IDirect3DDevice9_EndScene_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].Clear                              = IDirect3DDevice9_Clear_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetTransform                       = IDirect3DDevice9_SetTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetTransform                       = IDirect3DDevice9_GetTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].MultiplyTransform                  = IDirect3DDevice9_MultiplyTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetViewport                        = IDirect3DDevice9_SetViewport_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetViewport                        = IDirect3DDevice9_GetViewport_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetMaterial                        = IDirect3DDevice9_SetMaterial_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetMaterial                        = IDirect3DDevice9_GetMaterial_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetLight                           = IDirect3DDevice9_SetLight_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetLight                           = IDirect3DDevice9_GetLight_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].LightEnable                        = IDirect3DDevice9_LightEnable_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetLightEnable                     = IDirect3DDevice9_GetLightEnable_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetClipPlane                       = IDirect3DDevice9_SetClipPlane_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetClipPlane                       = IDirect3DDevice9_GetClipPlane_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetRenderState                     = IDirect3DDevice9_SetRenderState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetRenderState                     = IDirect3DDevice9_GetRenderState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateStateBlock                   = IDirect3DDevice9_CreateStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].BeginStateBlock                    = IDirect3DDevice9_BeginStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].EndStateBlock                      = IDirect3DDevice9_EndStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetClipStatus                      = IDirect3DDevice9_SetClipStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetClipStatus                      = IDirect3DDevice9_GetClipStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetTexture                         = IDirect3DDevice9_GetTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetTexture                         = IDirect3DDevice9_SetTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetTextureStageState               = IDirect3DDevice9_GetTextureStageState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetTextureStageState               = IDirect3DDevice9_SetTextureStageState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetSamplerState                    = IDirect3DDevice9_GetSamplerState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetSamplerState                    = IDirect3DDevice9_SetSamplerState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].ValidateDevice                     = IDirect3DDevice9_ValidateDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPaletteEntries                  = IDirect3DDevice9_SetPaletteEntries_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPaletteEntries                  = IDirect3DDevice9_GetPaletteEntries_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetCurrentTexturePalette           = IDirect3DDevice9_SetCurrentTexturePalette_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetCurrentTexturePalette           = IDirect3DDevice9_GetCurrentTexturePalette_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetScissorRect                     = IDirect3DDevice9_SetScissorRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetScissorRect                     = IDirect3DDevice9_GetScissorRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetSoftwareVertexProcessing        = IDirect3DDevice9_SetSoftwareVertexProcessing_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetSoftwareVertexProcessing        = IDirect3DDevice9_GetSoftwareVertexProcessing_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetNPatchMode                      = IDirect3DDevice9_SetNPatchMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetNPatchMode                      = IDirect3DDevice9_GetNPatchMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawPrimitive                      = IDirect3DDevice9_DrawPrimitive_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawIndexedPrimitive               = IDirect3DDevice9_DrawIndexedPrimitive_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawPrimitiveUP                    = IDirect3DDevice9_DrawPrimitiveUP_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawIndexedPrimitiveUP             = IDirect3DDevice9_DrawIndexedPrimitiveUP_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].ProcessVertices                    = IDirect3DDevice9_ProcessVertices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateVertexDeclaration            = IDirect3DDevice9_CreateVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexDeclaration               = IDirect3DDevice9_SetVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexDeclaration               = IDirect3DDevice9_GetVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetFVF                             = IDirect3DDevice9_SetFVF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetFVF                             = IDirect3DDevice9_GetFVF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateVertexShader                 = IDirect3DDevice9_CreateVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexShader                    = IDirect3DDevice9_SetVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexShader                    = IDirect3DDevice9_GetVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantF           = IDirect3DDevice9_SetVertexShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantF           = IDirect3DDevice9_GetVertexShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantI           = IDirect3DDevice9_SetVertexShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantI           = IDirect3DDevice9_GetVertexShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantB           = IDirect3DDevice9_SetVertexShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantB           = IDirect3DDevice9_GetVertexShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetStreamSource                    = IDirect3DDevice9_SetStreamSource_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetStreamSource                    = IDirect3DDevice9_GetStreamSource_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetStreamSourceFreq                = IDirect3DDevice9_SetStreamSourceFreq_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetStreamSourceFreq                = IDirect3DDevice9_GetStreamSourceFreq_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetIndices                         = IDirect3DDevice9_SetIndices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetIndices                         = IDirect3DDevice9_GetIndices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreatePixelShader                  = IDirect3DDevice9_CreatePixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPixelShader                     = IDirect3DDevice9_SetPixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPixelShader                     = IDirect3DDevice9_GetPixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantF            = IDirect3DDevice9_SetPixelShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantF            = IDirect3DDevice9_GetPixelShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantI            = IDirect3DDevice9_SetPixelShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantI            = IDirect3DDevice9_GetPixelShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantB            = IDirect3DDevice9_SetPixelShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantB            = IDirect3DDevice9_GetPixelShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawRectPatch                      = IDirect3DDevice9_DrawRectPatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawTriPatch                       = IDirect3DDevice9_DrawTriPatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DeletePatch                        = IDirect3DDevice9_DeletePatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateQuery                        = IDirect3DDevice9_CreateQuery_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].QueryInterface                 = IDirect3DStateBlock9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].AddRef                         = IDirect3DStateBlock9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].Release                        = IDirect3DStateBlock9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].GetDevice                      = IDirect3DStateBlock9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].Capture                        = IDirect3DStateBlock9_Capture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].Apply                          = IDirect3DStateBlock9_Apply_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].QueryInterface                  = IDirect3DSwapChain9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].AddRef                          = IDirect3DSwapChain9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].Release                         = IDirect3DSwapChain9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].Present                         = IDirect3DSwapChain9_Present_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetFrontBufferData              = IDirect3DSwapChain9_GetFrontBufferData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetBackBuffer                   = IDirect3DSwapChain9_GetBackBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetRasterStatus                 = IDirect3DSwapChain9_GetRasterStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetDisplayMode                  = IDirect3DSwapChain9_GetDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetDevice                       = IDirect3DSwapChain9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetPresentParameters            = IDirect3DSwapChain9_GetPresentParameters_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].QueryInterface                   = IDirect3DResource9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].AddRef                           = IDirect3DResource9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].Release                          = IDirect3DResource9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].GetDevice                        = IDirect3DResource9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].SetPrivateData                   = IDirect3DResource9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].GetPrivateData                   = IDirect3DResource9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].FreePrivateData                  = IDirect3DResource9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].SetPriority                      = IDirect3DResource9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].GetPriority                      = IDirect3DResource9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].PreLoad                          = IDirect3DResource9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].GetType                          = IDirect3DResource9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].QueryInterface          = IDirect3DVertexDeclaration9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].AddRef                  = IDirect3DVertexDeclaration9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].Release                 = IDirect3DVertexDeclaration9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].GetDevice               = IDirect3DVertexDeclaration9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].GetDeclaration          = IDirect3DVertexDeclaration9_GetDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].QueryInterface               = IDirect3DVertexShader9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].AddRef                       = IDirect3DVertexShader9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].Release                      = IDirect3DVertexShader9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].GetDevice                    = IDirect3DVertexShader9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].GetFunction                  = IDirect3DVertexShader9_GetFunction_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].QueryInterface                = IDirect3DPixelShader9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].AddRef                        = IDirect3DPixelShader9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].Release                       = IDirect3DPixelShader9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].GetDevice                     = IDirect3DPixelShader9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].GetFunction                   = IDirect3DPixelShader9_GetFunction_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].QueryInterface                = IDirect3DBaseTexture9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].AddRef                        = IDirect3DBaseTexture9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].Release                       = IDirect3DBaseTexture9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetDevice                     = IDirect3DBaseTexture9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].SetPrivateData                = IDirect3DBaseTexture9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetPrivateData                = IDirect3DBaseTexture9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].FreePrivateData               = IDirect3DBaseTexture9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].SetPriority                   = IDirect3DBaseTexture9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetPriority                   = IDirect3DBaseTexture9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].PreLoad                       = IDirect3DBaseTexture9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetType                       = IDirect3DBaseTexture9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].SetLOD                        = IDirect3DBaseTexture9_SetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetLOD                        = IDirect3DBaseTexture9_GetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetLevelCount                 = IDirect3DBaseTexture9_GetLevelCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].SetAutoGenFilterType          = IDirect3DBaseTexture9_SetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetAutoGenFilterType          = IDirect3DBaseTexture9_GetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GenerateMipSubLevels          = IDirect3DBaseTexture9_GenerateMipSubLevels_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].QueryInterface                    = IDirect3DTexture9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].AddRef                            = IDirect3DTexture9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].Release                           = IDirect3DTexture9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetDevice                         = IDirect3DTexture9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].SetPrivateData                    = IDirect3DTexture9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetPrivateData                    = IDirect3DTexture9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].FreePrivateData                   = IDirect3DTexture9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].SetPriority                       = IDirect3DTexture9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetPriority                       = IDirect3DTexture9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].PreLoad                           = IDirect3DTexture9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetType                           = IDirect3DTexture9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].SetLOD                            = IDirect3DTexture9_SetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetLOD                            = IDirect3DTexture9_GetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetLevelCount                     = IDirect3DTexture9_GetLevelCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].SetAutoGenFilterType              = IDirect3DTexture9_SetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetAutoGenFilterType              = IDirect3DTexture9_GetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GenerateMipSubLevels              = IDirect3DTexture9_GenerateMipSubLevels_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetLevelDesc                      = IDirect3DTexture9_GetLevelDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetSurfaceLevel                   = IDirect3DTexture9_GetSurfaceLevel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].LockRect                          = IDirect3DTexture9_LockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].UnlockRect                        = IDirect3DTexture9_UnlockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].AddDirtyRect                      = IDirect3DTexture9_AddDirtyRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].QueryInterface              = IDirect3DVolumeTexture9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].AddRef                      = IDirect3DVolumeTexture9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].Release                     = IDirect3DVolumeTexture9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetDevice                   = IDirect3DVolumeTexture9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].SetPrivateData              = IDirect3DVolumeTexture9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetPrivateData              = IDirect3DVolumeTexture9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].FreePrivateData             = IDirect3DVolumeTexture9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].SetPriority                 = IDirect3DVolumeTexture9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetPriority                 = IDirect3DVolumeTexture9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].PreLoad                     = IDirect3DVolumeTexture9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetType                     = IDirect3DVolumeTexture9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].SetLOD                      = IDirect3DVolumeTexture9_SetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLOD                      = IDirect3DVolumeTexture9_GetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLevelCount               = IDirect3DVolumeTexture9_GetLevelCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].SetAutoGenFilterType        = IDirect3DVolumeTexture9_SetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetAutoGenFilterType        = IDirect3DVolumeTexture9_GetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GenerateMipSubLevels        = IDirect3DVolumeTexture9_GenerateMipSubLevels_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLevelDesc                = IDirect3DVolumeTexture9_GetLevelDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetVolumeLevel              = IDirect3DVolumeTexture9_GetVolumeLevel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].LockBox                     = IDirect3DVolumeTexture9_LockBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].UnlockBox                   = IDirect3DVolumeTexture9_UnlockBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].AddDirtyBox                 = IDirect3DVolumeTexture9_AddDirtyBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].QueryInterface                = IDirect3DCubeTexture9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].AddRef                        = IDirect3DCubeTexture9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].Release                       = IDirect3DCubeTexture9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetDevice                     = IDirect3DCubeTexture9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].SetPrivateData                = IDirect3DCubeTexture9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetPrivateData                = IDirect3DCubeTexture9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].FreePrivateData               = IDirect3DCubeTexture9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].SetPriority                   = IDirect3DCubeTexture9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetPriority                   = IDirect3DCubeTexture9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].PreLoad                       = IDirect3DCubeTexture9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetType                       = IDirect3DCubeTexture9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].SetLOD                        = IDirect3DCubeTexture9_SetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetLOD                        = IDirect3DCubeTexture9_GetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetLevelCount                 = IDirect3DCubeTexture9_GetLevelCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].SetAutoGenFilterType          = IDirect3DCubeTexture9_SetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetAutoGenFilterType          = IDirect3DCubeTexture9_GetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GenerateMipSubLevels          = IDirect3DCubeTexture9_GenerateMipSubLevels_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetLevelDesc                  = IDirect3DCubeTexture9_GetLevelDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetCubeMapSurface             = IDirect3DCubeTexture9_GetCubeMapSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].LockRect                      = IDirect3DCubeTexture9_LockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].UnlockRect                    = IDirect3DCubeTexture9_UnlockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].AddDirtyRect                  = IDirect3DCubeTexture9_AddDirtyRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].QueryInterface               = IDirect3DVertexBuffer9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].AddRef                       = IDirect3DVertexBuffer9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].Release                      = IDirect3DVertexBuffer9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetDevice                    = IDirect3DVertexBuffer9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].SetPrivateData               = IDirect3DVertexBuffer9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetPrivateData               = IDirect3DVertexBuffer9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].FreePrivateData              = IDirect3DVertexBuffer9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].SetPriority                  = IDirect3DVertexBuffer9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetPriority                  = IDirect3DVertexBuffer9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].PreLoad                      = IDirect3DVertexBuffer9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetType                      = IDirect3DVertexBuffer9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].Lock                         = IDirect3DVertexBuffer9_Lock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].Unlock                       = IDirect3DVertexBuffer9_Unlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetDesc                      = IDirect3DVertexBuffer9_GetDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].QueryInterface                = IDirect3DIndexBuffer9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].AddRef                        = IDirect3DIndexBuffer9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].Release                       = IDirect3DIndexBuffer9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetDevice                     = IDirect3DIndexBuffer9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].SetPrivateData                = IDirect3DIndexBuffer9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetPrivateData                = IDirect3DIndexBuffer9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].FreePrivateData               = IDirect3DIndexBuffer9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].SetPriority                   = IDirect3DIndexBuffer9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetPriority                   = IDirect3DIndexBuffer9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].PreLoad                       = IDirect3DIndexBuffer9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetType                       = IDirect3DIndexBuffer9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].Lock                          = IDirect3DIndexBuffer9_Lock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].Unlock                        = IDirect3DIndexBuffer9_Unlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetDesc                       = IDirect3DIndexBuffer9_GetDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].QueryInterface                    = IDirect3DSurface9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].AddRef                            = IDirect3DSurface9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].Release                           = IDirect3DSurface9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetDevice                         = IDirect3DSurface9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].SetPrivateData                    = IDirect3DSurface9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetPrivateData                    = IDirect3DSurface9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].FreePrivateData                   = IDirect3DSurface9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].SetPriority                       = IDirect3DSurface9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetPriority                       = IDirect3DSurface9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].PreLoad                           = IDirect3DSurface9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetType                           = IDirect3DSurface9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetContainer                      = IDirect3DSurface9_GetContainer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetDesc                           = IDirect3DSurface9_GetDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].LockRect                          = IDirect3DSurface9_LockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].UnlockRect                        = IDirect3DSurface9_UnlockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetDC                             = IDirect3DSurface9_GetDC_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].ReleaseDC                         = IDirect3DSurface9_ReleaseDC_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].QueryInterface                     = IDirect3DVolume9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].AddRef                             = IDirect3DVolume9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].Release                            = IDirect3DVolume9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].GetDevice                          = IDirect3DVolume9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].SetPrivateData                     = IDirect3DVolume9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].GetPrivateData                     = IDirect3DVolume9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].FreePrivateData                    = IDirect3DVolume9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].GetContainer                       = IDirect3DVolume9_GetContainer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].GetDesc                            = IDirect3DVolume9_GetDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].LockBox                            = IDirect3DVolume9_LockBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].UnlockBox                          = IDirect3DVolume9_UnlockBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].QueryInterface                      = IDirect3DQuery9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].AddRef                              = IDirect3DQuery9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].Release                             = IDirect3DQuery9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].GetDevice                           = IDirect3DQuery9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].GetType                             = IDirect3DQuery9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].GetDataSize                         = IDirect3DQuery9_GetDataSize_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].Issue                               = IDirect3DQuery9_Issue_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].GetData                             = IDirect3DQuery9_GetData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].QueryInterface                         = IDirect3D9Ex_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].AddRef                                 = IDirect3D9Ex_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].Release                                = IDirect3D9Ex_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterCount                        = IDirect3D9Ex_GetAdapterCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterIdentifier                   = IDirect3D9Ex_GetAdapterIdentifier_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterModeCount                    = IDirect3D9Ex_GetAdapterModeCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].EnumAdapterModes                       = IDirect3D9Ex_EnumAdapterModes_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterDisplayMode                  = IDirect3D9Ex_GetAdapterDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceType                        = IDirect3D9Ex_CheckDeviceType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceFormat                      = IDirect3D9Ex_CheckDeviceFormat_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceMultiSampleType             = IDirect3D9Ex_CheckDeviceMultiSampleType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDepthStencilMatch                 = IDirect3D9Ex_CheckDepthStencilMatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceFormatConversion            = IDirect3D9Ex_CheckDeviceFormatConversion_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetDeviceCaps                          = IDirect3D9Ex_GetDeviceCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterMonitor                      = IDirect3D9Ex_GetAdapterMonitor_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CreateDevice                           = IDirect3D9Ex_CreateDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterModeCountEx                  = IDirect3D9Ex_GetAdapterModeCountEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].EnumAdapterModesEx                     = IDirect3D9Ex_EnumAdapterModesEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterDisplayModeEx                = IDirect3D9Ex_GetAdapterDisplayModeEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CreateDeviceEx                         = IDirect3D9Ex_CreateDeviceEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterLUID                         = IDirect3D9Ex_GetAdapterLUID_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].QueryInterface                   = IDirect3DDevice9Ex_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].AddRef                           = IDirect3DDevice9Ex_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].Release                          = IDirect3DDevice9Ex_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].TestCooperativeLevel             = IDirect3DDevice9Ex_TestCooperativeLevel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetAvailableTextureMem           = IDirect3DDevice9Ex_GetAvailableTextureMem_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].EvictManagedResources            = IDirect3DDevice9Ex_EvictManagedResources_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDirect3D                      = IDirect3DDevice9Ex_GetDirect3D_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDeviceCaps                    = IDirect3DDevice9Ex_GetDeviceCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDisplayMode                   = IDirect3DDevice9Ex_GetDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetCreateParameters              = IDirect3DDevice9Ex_GetCreateParameters_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetCursorProperties              = IDirect3DDevice9Ex_SetCursorProperties_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetCursorPosition                = IDirect3DDevice9Ex_SetCursorPosition_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ShowCursor                       = IDirect3DDevice9Ex_ShowCursor_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateAdditionalSwapChain        = IDirect3DDevice9Ex_CreateAdditionalSwapChain_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetSwapChain                     = IDirect3DDevice9Ex_GetSwapChain_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetNumberOfSwapChains            = IDirect3DDevice9Ex_GetNumberOfSwapChains_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].Reset                            = IDirect3DDevice9Ex_Reset_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].Present                          = IDirect3DDevice9Ex_Present_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetBackBuffer                    = IDirect3DDevice9Ex_GetBackBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetRasterStatus                  = IDirect3DDevice9Ex_GetRasterStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetDialogBoxMode                 = IDirect3DDevice9Ex_SetDialogBoxMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetGammaRamp                     = IDirect3DDevice9Ex_SetGammaRamp_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetGammaRamp                     = IDirect3DDevice9Ex_GetGammaRamp_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateTexture                    = IDirect3DDevice9Ex_CreateTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVolumeTexture              = IDirect3DDevice9Ex_CreateVolumeTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateCubeTexture                = IDirect3DDevice9Ex_CreateCubeTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexBuffer               = IDirect3DDevice9Ex_CreateVertexBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateIndexBuffer                = IDirect3DDevice9Ex_CreateIndexBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateRenderTarget               = IDirect3DDevice9Ex_CreateRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateDepthStencilSurface        = IDirect3DDevice9Ex_CreateDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].UpdateSurface                    = IDirect3DDevice9Ex_UpdateSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].UpdateTexture                    = IDirect3DDevice9Ex_UpdateTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderTargetData              = IDirect3DDevice9Ex_GetRenderTargetData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetFrontBufferData               = IDirect3DDevice9Ex_GetFrontBufferData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].StretchRect                      = IDirect3DDevice9Ex_StretchRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ColorFill                        = IDirect3DDevice9Ex_ColorFill_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateOffscreenPlainSurface      = IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetRenderTarget                  = IDirect3DDevice9Ex_SetRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderTarget                  = IDirect3DDevice9Ex_GetRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetDepthStencilSurface           = IDirect3DDevice9Ex_SetDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDepthStencilSurface           = IDirect3DDevice9Ex_GetDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].BeginScene                       = IDirect3DDevice9Ex_BeginScene_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].EndScene                         = IDirect3DDevice9Ex_EndScene_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].Clear                            = IDirect3DDevice9Ex_Clear_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetTransform                     = IDirect3DDevice9Ex_SetTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetTransform                     = IDirect3DDevice9Ex_GetTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].MultiplyTransform                = IDirect3DDevice9Ex_MultiplyTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetViewport                      = IDirect3DDevice9Ex_SetViewport_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetViewport                      = IDirect3DDevice9Ex_GetViewport_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetMaterial                      = IDirect3DDevice9Ex_SetMaterial_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetMaterial                      = IDirect3DDevice9Ex_GetMaterial_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetLight                         = IDirect3DDevice9Ex_SetLight_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetLight                         = IDirect3DDevice9Ex_GetLight_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].LightEnable                      = IDirect3DDevice9Ex_LightEnable_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetLightEnable                   = IDirect3DDevice9Ex_GetLightEnable_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetClipPlane                     = IDirect3DDevice9Ex_SetClipPlane_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetClipPlane                     = IDirect3DDevice9Ex_GetClipPlane_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetRenderState                   = IDirect3DDevice9Ex_SetRenderState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderState                   = IDirect3DDevice9Ex_GetRenderState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateStateBlock                 = IDirect3DDevice9Ex_CreateStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].BeginStateBlock                  = IDirect3DDevice9Ex_BeginStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].EndStateBlock                    = IDirect3DDevice9Ex_EndStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetClipStatus                    = IDirect3DDevice9Ex_SetClipStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetClipStatus                    = IDirect3DDevice9Ex_GetClipStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetTexture                       = IDirect3DDevice9Ex_GetTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetTexture                       = IDirect3DDevice9Ex_SetTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetTextureStageState             = IDirect3DDevice9Ex_GetTextureStageState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetTextureStageState             = IDirect3DDevice9Ex_SetTextureStageState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetSamplerState                  = IDirect3DDevice9Ex_GetSamplerState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetSamplerState                  = IDirect3DDevice9Ex_SetSamplerState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ValidateDevice                   = IDirect3DDevice9Ex_ValidateDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPaletteEntries                = IDirect3DDevice9Ex_SetPaletteEntries_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPaletteEntries                = IDirect3DDevice9Ex_GetPaletteEntries_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetCurrentTexturePalette         = IDirect3DDevice9Ex_SetCurrentTexturePalette_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetCurrentTexturePalette         = IDirect3DDevice9Ex_GetCurrentTexturePalette_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetScissorRect                   = IDirect3DDevice9Ex_SetScissorRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetScissorRect                   = IDirect3DDevice9Ex_GetScissorRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetSoftwareVertexProcessing      = IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetSoftwareVertexProcessing      = IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetNPatchMode                    = IDirect3DDevice9Ex_SetNPatchMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetNPatchMode                    = IDirect3DDevice9Ex_GetNPatchMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawPrimitive                    = IDirect3DDevice9Ex_DrawPrimitive_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawIndexedPrimitive             = IDirect3DDevice9Ex_DrawIndexedPrimitive_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawPrimitiveUP                  = IDirect3DDevice9Ex_DrawPrimitiveUP_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawIndexedPrimitiveUP           = IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ProcessVertices                  = IDirect3DDevice9Ex_ProcessVertices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexDeclaration          = IDirect3DDevice9Ex_CreateVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexDeclaration             = IDirect3DDevice9Ex_SetVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexDeclaration             = IDirect3DDevice9Ex_GetVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetFVF                           = IDirect3DDevice9Ex_SetFVF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetFVF                           = IDirect3DDevice9Ex_GetFVF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexShader               = IDirect3DDevice9Ex_CreateVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShader                  = IDirect3DDevice9Ex_SetVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShader                  = IDirect3DDevice9Ex_GetVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantF         = IDirect3DDevice9Ex_SetVertexShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantF         = IDirect3DDevice9Ex_GetVertexShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantI         = IDirect3DDevice9Ex_SetVertexShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantI         = IDirect3DDevice9Ex_GetVertexShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantB         = IDirect3DDevice9Ex_SetVertexShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantB         = IDirect3DDevice9Ex_GetVertexShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetStreamSource                  = IDirect3DDevice9Ex_SetStreamSource_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetStreamSource                  = IDirect3DDevice9Ex_GetStreamSource_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetStreamSourceFreq              = IDirect3DDevice9Ex_SetStreamSourceFreq_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetStreamSourceFreq              = IDirect3DDevice9Ex_GetStreamSourceFreq_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetIndices                       = IDirect3DDevice9Ex_SetIndices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetIndices                       = IDirect3DDevice9Ex_GetIndices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreatePixelShader                = IDirect3DDevice9Ex_CreatePixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShader                   = IDirect3DDevice9Ex_SetPixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShader                   = IDirect3DDevice9Ex_GetPixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantF          = IDirect3DDevice9Ex_SetPixelShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantF          = IDirect3DDevice9Ex_GetPixelShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantI          = IDirect3DDevice9Ex_SetPixelShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantI          = IDirect3DDevice9Ex_GetPixelShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantB          = IDirect3DDevice9Ex_SetPixelShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantB          = IDirect3DDevice9Ex_GetPixelShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawRectPatch                    = IDirect3DDevice9Ex_DrawRectPatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawTriPatch                     = IDirect3DDevice9Ex_DrawTriPatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DeletePatch                      = IDirect3DDevice9Ex_DeletePatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateQuery                      = IDirect3DDevice9Ex_CreateQuery_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetConvolutionMonoKernel         = IDirect3DDevice9Ex_SetConvolutionMonoKernel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ComposeRects                     = IDirect3DDevice9Ex_ComposeRects_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].PresentEx                        = IDirect3DDevice9Ex_PresentEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetGPUThreadPriority             = IDirect3DDevice9Ex_GetGPUThreadPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetGPUThreadPriority             = IDirect3DDevice9Ex_SetGPUThreadPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].WaitForVBlank                    = IDirect3DDevice9Ex_WaitForVBlank_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CheckResourceResidency           = IDirect3DDevice9Ex_CheckResourceResidency_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetMaximumFrameLatency           = IDirect3DDevice9Ex_SetMaximumFrameLatency_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetMaximumFrameLatency           = IDirect3DDevice9Ex_GetMaximumFrameLatency_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CheckDeviceState                 = IDirect3DDevice9Ex_CheckDeviceState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateRenderTargetEx             = IDirect3DDevice9Ex_CreateRenderTargetEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateOffscreenPlainSurfaceEx    = IDirect3DDevice9Ex_CreateOffscreenPlainSurfaceEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateDepthStencilSurfaceEx      = IDirect3DDevice9Ex_CreateDepthStencilSurfaceEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ResetEx                          = IDirect3DDevice9Ex_ResetEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDisplayModeEx                 = IDirect3DDevice9Ex_GetDisplayModeEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].QueryInterface                = IDirect3DSwapChain9Ex_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].AddRef                        = IDirect3DSwapChain9Ex_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].Release                       = IDirect3DSwapChain9Ex_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].Present                       = IDirect3DSwapChain9Ex_Present_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetFrontBufferData            = IDirect3DSwapChain9Ex_GetFrontBufferData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetBackBuffer                 = IDirect3DSwapChain9Ex_GetBackBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetRasterStatus               = IDirect3DSwapChain9Ex_GetRasterStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDisplayMode                = IDirect3DSwapChain9Ex_GetDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDevice                     = IDirect3DSwapChain9Ex_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetPresentParameters          = IDirect3DSwapChain9Ex_GetPresentParameters_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetLastPresentCount           = IDirect3DSwapChain9Ex_GetLastPresentCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetPresentStats               = IDirect3DSwapChain9Ex_GetPresentStats_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDisplayModeEx              = IDirect3DSwapChain9Ex_GetDisplayModeEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9ExOverlayExtension.tables[INDEX].QueryInterface         = IDirect3D9ExOverlayExtension_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9ExOverlayExtension.tables[INDEX].AddRef                 = IDirect3D9ExOverlayExtension_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9ExOverlayExtension.tables[INDEX].Release                = IDirect3D9ExOverlayExtension_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9ExOverlayExtension.tables[INDEX].CheckDeviceOverlayType = IDirect3D9ExOverlayExtension_CheckDeviceOverlayType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].QueryInterface                = IDirect3DDevice9Video_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].AddRef                        = IDirect3DDevice9Video_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].Release                       = IDirect3DDevice9Video_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].GetContentProtectionCaps      = IDirect3DDevice9Video_GetContentProtectionCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].CreateAuthenticatedChannel    = IDirect3DDevice9Video_CreateAuthenticatedChannel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].CreateCryptoSession           = IDirect3DDevice9Video_CreateCryptoSession_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].QueryInterface       = IDirect3DAuthenticatedChannel9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].AddRef               = IDirect3DAuthenticatedChannel9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Release              = IDirect3DAuthenticatedChannel9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].GetCertificateSize   = IDirect3DAuthenticatedChannel9_GetCertificateSize_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].GetCertificate       = IDirect3DAuthenticatedChannel9_GetCertificate_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].NegotiateKeyExchange = IDirect3DAuthenticatedChannel9_NegotiateKeyExchange_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Query                = IDirect3DAuthenticatedChannel9_Query_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Configure            = IDirect3DAuthenticatedChannel9_Configure_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].QueryInterface              = IDirect3DCryptoSession9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].AddRef                      = IDirect3DCryptoSession9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].Release                     = IDirect3DCryptoSession9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].GetCertificateSize          = IDirect3DCryptoSession9_GetCertificateSize_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].GetCertificate              = IDirect3DCryptoSession9_GetCertificate_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].NegotiateKeyExchange        = IDirect3DCryptoSession9_NegotiateKeyExchange_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].EncryptionBlt               = IDirect3DCryptoSession9_EncryptionBlt_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].DecryptionBlt               = IDirect3DCryptoSession9_DecryptionBlt_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].GetSurfacePitch             = IDirect3DCryptoSession9_GetSurfacePitch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].StartSessionKeyRefresh      = IDirect3DCryptoSession9_StartSessionKeyRefresh_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].FinishSessionKeyRefresh     = IDirect3DCryptoSession9_FinishSessionKeyRefresh_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].GetEncryptionBltKey         = IDirect3DCryptoSession9_GetEncryptionBltKey_Hooked<INDEX>;
}

// -----------------------------------------------------------------------------
void SetupD3D9HookedVTables() {
    SetupD3D9HookedVTables<0>();
    SetupD3D9HookedVTables<1>();
    SetupD3D9HookedVTables<2>();
    SetupD3D9HookedVTables<3>();
    SetupD3D9HookedVTables<4>();
    SetupD3D9HookedVTables<5>();
    SetupD3D9HookedVTables<6>();
    SetupD3D9HookedVTables<7>();
    SetupD3D9HookedVTables<8>();
    SetupD3D9HookedVTables<9>();
    SetupD3D9HookedVTables<10>();
    SetupD3D9HookedVTables<11>();
    SetupD3D9HookedVTables<12>();
    SetupD3D9HookedVTables<13>();
    SetupD3D9HookedVTables<14>();
    SetupD3D9HookedVTables<15>();
}
