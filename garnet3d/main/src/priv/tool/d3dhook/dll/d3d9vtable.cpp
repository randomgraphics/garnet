// script generated file. Do _NOT_ edit.

#include "pch.h"
#include "d3d9vtable.h"

D3D9VTables g_D3D9OriginVTables;
D3D9VTables g_D3D9HookedVTables;

// -----------------------------------------------------------------------------
// IDirect3D9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_QueryInterface_Hooked(IDirect3D9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3D9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3D9_AddRef_Hooked(IDirect3D9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3D9::AddRef");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3D9_Release_Hooked(IDirect3D9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3D9::Release");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_RegisterSoftwareDevice_Hooked(IDirect3D9 * ptr, void* pInitializeFunction)
{
    calltrace::AutoTrace trace("IDirect3D9::RegisterSoftwareDevice");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].RegisterSoftwareDevice(ptr, pInitializeFunction);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3D9_GetAdapterCount_Hooked(IDirect3D9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3D9::GetAdapterCount");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterCount(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_GetAdapterIdentifier_Hooked(IDirect3D9 * ptr, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
{
    calltrace::AutoTrace trace("IDirect3D9::GetAdapterIdentifier");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterIdentifier(ptr, Adapter, Flags, pIdentifier);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3D9_GetAdapterModeCount_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DFORMAT Format)
{
    calltrace::AutoTrace trace("IDirect3D9::GetAdapterModeCount");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterModeCount(ptr, Adapter, Format);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_EnumAdapterModes_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace("IDirect3D9::EnumAdapterModes");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].EnumAdapterModes(ptr, Adapter, Format, Mode, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_GetAdapterDisplayMode_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace("IDirect3D9::GetAdapterDisplayMode");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterDisplayMode(ptr, Adapter, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDeviceType_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
    calltrace::AutoTrace trace("IDirect3D9::CheckDeviceType");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDeviceType(ptr, Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDeviceFormat_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
    calltrace::AutoTrace trace("IDirect3D9::CheckDeviceFormat");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDeviceFormat(ptr, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDeviceMultiSampleType_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
    calltrace::AutoTrace trace("IDirect3D9::CheckDeviceMultiSampleType");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDeviceMultiSampleType(ptr, Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDepthStencilMatch_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
    calltrace::AutoTrace trace("IDirect3D9::CheckDepthStencilMatch");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDepthStencilMatch(ptr, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_CheckDeviceFormatConversion_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
    calltrace::AutoTrace trace("IDirect3D9::CheckDeviceFormatConversion");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CheckDeviceFormatConversion(ptr, Adapter, DeviceType, SourceFormat, TargetFormat);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_GetDeviceCaps_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
    calltrace::AutoTrace trace("IDirect3D9::GetDeviceCaps");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetDeviceCaps(ptr, Adapter, DeviceType, pCaps);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HMONITOR STDMETHODCALLTYPE IDirect3D9_GetAdapterMonitor_Hooked(IDirect3D9 * ptr, UINT Adapter)
{
    calltrace::AutoTrace trace("IDirect3D9::GetAdapterMonitor");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].GetAdapterMonitor(ptr, Adapter);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9_CreateDevice_Hooked(IDirect3D9 * ptr, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
    calltrace::AutoTrace trace("IDirect3D9::CreateDevice");
    return g_D3D9OriginVTables._IDirect3D9.tables[INDEX].CreateDevice(ptr, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
}

// -----------------------------------------------------------------------------
// IDirect3DDevice9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_QueryInterface_Hooked(IDirect3DDevice9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DDevice9_AddRef_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::AddRef");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DDevice9_Release_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::Release");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_TestCooperativeLevel_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::TestCooperativeLevel");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].TestCooperativeLevel(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3DDevice9_GetAvailableTextureMem_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetAvailableTextureMem");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetAvailableTextureMem(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_EvictManagedResources_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::EvictManagedResources");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].EvictManagedResources(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetDirect3D_Hooked(IDirect3DDevice9 * ptr, IDirect3D9** ppD3D9)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetDirect3D");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetDirect3D(ptr, ppD3D9);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetDeviceCaps_Hooked(IDirect3DDevice9 * ptr, D3DCAPS9* pCaps)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetDeviceCaps");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetDeviceCaps(ptr, pCaps);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetDisplayMode_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetDisplayMode");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetDisplayMode(ptr, iSwapChain, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetCreationParameters_Hooked(IDirect3DDevice9 * ptr, D3DDEVICE_CREATION_PARAMETERS * pParameters)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetCreationParameters");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetCreationParameters(ptr, pParameters);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetCursorProperties_Hooked(IDirect3DDevice9 * ptr, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetCursorProperties");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetCursorProperties(ptr, XHotSpot, YHotSpot, pCursorBitmap);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DDevice9_SetCursorPosition_Hooked(IDirect3DDevice9 * ptr, int X, int Y, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetCursorPosition");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetCursorPosition(ptr, X, Y, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static BOOL STDMETHODCALLTYPE IDirect3DDevice9_ShowCursor_Hooked(IDirect3DDevice9 * ptr, BOOL bShow)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::ShowCursor");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].ShowCursor(ptr, bShow);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateAdditionalSwapChain_Hooked(IDirect3DDevice9 * ptr, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** ppSwapChain)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateAdditionalSwapChain");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateAdditionalSwapChain(ptr, pPresentationParameters, ppSwapChain);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetSwapChain_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, IDirect3DSwapChain9** ppSwapChain)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetSwapChain");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetSwapChain(ptr, iSwapChain, ppSwapChain);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3DDevice9_GetNumberOfSwapChains_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetNumberOfSwapChains");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetNumberOfSwapChains(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_Reset_Hooked(IDirect3DDevice9 * ptr, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::Reset");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].Reset(ptr, pPresentationParameters);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_Present_Hooked(IDirect3DDevice9 * ptr, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::Present");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].Present(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetBackBuffer_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetBackBuffer");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetBackBuffer(ptr, iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetRasterStatus_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetRasterStatus");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetRasterStatus(ptr, iSwapChain, pRasterStatus);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetDialogBoxMode_Hooked(IDirect3DDevice9 * ptr, BOOL bEnableDialogs)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetDialogBoxMode");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetDialogBoxMode(ptr, bEnableDialogs);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DDevice9_SetGammaRamp_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetGammaRamp");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetGammaRamp(ptr, iSwapChain, Flags, pRamp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DDevice9_GetGammaRamp_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetGammaRamp");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetGammaRamp(ptr, iSwapChain, pRamp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateTexture_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateTexture(ptr, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateVolumeTexture_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateVolumeTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateVolumeTexture(ptr, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateCubeTexture_Hooked(IDirect3DDevice9 * ptr, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateCubeTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateCubeTexture(ptr, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateVertexBuffer_Hooked(IDirect3DDevice9 * ptr, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateVertexBuffer");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateVertexBuffer(ptr, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateIndexBuffer_Hooked(IDirect3DDevice9 * ptr, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateIndexBuffer");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateIndexBuffer(ptr, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateRenderTarget_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateRenderTarget");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateRenderTarget(ptr, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateDepthStencilSurface_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateDepthStencilSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateDepthStencilSurface(ptr, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_UpdateSurface_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::UpdateSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].UpdateSurface(ptr, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_UpdateTexture_Hooked(IDirect3DDevice9 * ptr, IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::UpdateTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].UpdateTexture(ptr, pSourceTexture, pDestinationTexture);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetRenderTargetData_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetRenderTargetData");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetRenderTargetData(ptr, pRenderTarget, pDestSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetFrontBufferData_Hooked(IDirect3DDevice9 * ptr, UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetFrontBufferData");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetFrontBufferData(ptr, iSwapChain, pDestSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_StretchRect_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::StretchRect");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].StretchRect(ptr, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_ColorFill_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::ColorFill");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].ColorFill(ptr, pSurface, pRect, color);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateOffscreenPlainSurface_Hooked(IDirect3DDevice9 * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateOffscreenPlainSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateOffscreenPlainSurface(ptr, Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetRenderTarget_Hooked(IDirect3DDevice9 * ptr, DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetRenderTarget");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetRenderTarget(ptr, RenderTargetIndex, pRenderTarget);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetRenderTarget_Hooked(IDirect3DDevice9 * ptr, DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetRenderTarget");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetRenderTarget(ptr, RenderTargetIndex, ppRenderTarget);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetDepthStencilSurface_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9* pNewZStencil)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetDepthStencilSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetDepthStencilSurface(ptr, pNewZStencil);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetDepthStencilSurface_Hooked(IDirect3DDevice9 * ptr, IDirect3DSurface9** ppZStencilSurface)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetDepthStencilSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetDepthStencilSurface(ptr, ppZStencilSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_BeginScene_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::BeginScene");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].BeginScene(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_EndScene_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::EndScene");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].EndScene(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_Clear_Hooked(IDirect3DDevice9 * ptr, DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::Clear");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].Clear(ptr, Count, pRects, Flags, Color, Z, Stencil);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetTransform_Hooked(IDirect3DDevice9 * ptr, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetTransform");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetTransform(ptr, State, pMatrix);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetTransform_Hooked(IDirect3DDevice9 * ptr, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetTransform");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetTransform(ptr, State, pMatrix);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_MultiplyTransform_Hooked(IDirect3DDevice9 * ptr, D3DTRANSFORMSTATETYPE TransformType, CONST D3DMATRIX* Value)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::MultiplyTransform");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].MultiplyTransform(ptr, TransformType, Value);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetViewport_Hooked(IDirect3DDevice9 * ptr, CONST D3DVIEWPORT9* pViewport)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetViewport");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetViewport(ptr, pViewport);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetViewport_Hooked(IDirect3DDevice9 * ptr, D3DVIEWPORT9* pViewport)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetViewport");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetViewport(ptr, pViewport);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetMaterial_Hooked(IDirect3DDevice9 * ptr, CONST D3DMATERIAL9* pMaterial)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetMaterial");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetMaterial(ptr, pMaterial);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetMaterial_Hooked(IDirect3DDevice9 * ptr, D3DMATERIAL9* pMaterial)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetMaterial");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetMaterial(ptr, pMaterial);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetLight_Hooked(IDirect3DDevice9 * ptr, DWORD Index, CONST D3DLIGHT9* pLight)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetLight");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetLight(ptr, Index, pLight);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetLight_Hooked(IDirect3DDevice9 * ptr, DWORD Index, D3DLIGHT9* pLight)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetLight");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetLight(ptr, Index, pLight);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_LightEnable_Hooked(IDirect3DDevice9 * ptr, DWORD Index, BOOL Enable)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::LightEnable");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].LightEnable(ptr, Index, Enable);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetLightEnable_Hooked(IDirect3DDevice9 * ptr, DWORD Index, BOOL* pEnable)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetLightEnable");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetLightEnable(ptr, Index, pEnable);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetClipPlane_Hooked(IDirect3DDevice9 * ptr, DWORD Index, CONST float* pPlane)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetClipPlane");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetClipPlane(ptr, Index, pPlane);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetClipPlane_Hooked(IDirect3DDevice9 * ptr, DWORD Index, float* pPlane)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetClipPlane");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetClipPlane(ptr, Index, pPlane);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetRenderState_Hooked(IDirect3DDevice9 * ptr, D3DRENDERSTATETYPE State, DWORD Value)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetRenderState");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetRenderState(ptr, State, Value);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetRenderState_Hooked(IDirect3DDevice9 * ptr, D3DRENDERSTATETYPE State, DWORD* pValue)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetRenderState");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetRenderState(ptr, State, pValue);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateStateBlock_Hooked(IDirect3DDevice9 * ptr, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateStateBlock");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateStateBlock(ptr, Type, ppSB);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_BeginStateBlock_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::BeginStateBlock");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].BeginStateBlock(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_EndStateBlock_Hooked(IDirect3DDevice9 * ptr, IDirect3DStateBlock9** ppSB)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::EndStateBlock");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].EndStateBlock(ptr, ppSB);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetClipStatus_Hooked(IDirect3DDevice9 * ptr, CONST D3DCLIPSTATUS9* pClipStatus)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetClipStatus");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetClipStatus(ptr, pClipStatus);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetClipStatus_Hooked(IDirect3DDevice9 * ptr, D3DCLIPSTATUS9* pClipStatus)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetClipStatus");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetClipStatus(ptr, pClipStatus);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetTexture_Hooked(IDirect3DDevice9 * ptr, DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetTexture(ptr, Stage, ppTexture);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetTexture_Hooked(IDirect3DDevice9 * ptr, DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetTexture(ptr, Stage, pTexture);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetTextureStageState_Hooked(IDirect3DDevice9 * ptr, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetTextureStageState");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetTextureStageState(ptr, Stage, Type, pValue);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetTextureStageState_Hooked(IDirect3DDevice9 * ptr, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetTextureStageState");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetTextureStageState(ptr, Stage, Type, Value);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetSamplerState_Hooked(IDirect3DDevice9 * ptr, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetSamplerState");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetSamplerState(ptr, Sampler, Type, pValue);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetSamplerState_Hooked(IDirect3DDevice9 * ptr, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetSamplerState");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetSamplerState(ptr, Sampler, Type, Value);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_ValidateDevice_Hooked(IDirect3DDevice9 * ptr, DWORD* pNumPasses)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::ValidateDevice");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].ValidateDevice(ptr, pNumPasses);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPaletteEntries_Hooked(IDirect3DDevice9 * ptr, UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetPaletteEntries");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPaletteEntries(ptr, PaletteNumber, pEntries);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPaletteEntries_Hooked(IDirect3DDevice9 * ptr, UINT PaletteNumber, PALETTEENTRY* pEntries)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetPaletteEntries");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPaletteEntries(ptr, PaletteNumber, pEntries);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetCurrentTexturePalette_Hooked(IDirect3DDevice9 * ptr, UINT PaletteNumber)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetCurrentTexturePalette");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetCurrentTexturePalette(ptr, PaletteNumber);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetCurrentTexturePalette_Hooked(IDirect3DDevice9 * ptr, UINT * PaletteNumber)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetCurrentTexturePalette");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetCurrentTexturePalette(ptr, PaletteNumber);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetScissorRect_Hooked(IDirect3DDevice9 * ptr, CONST RECT* pRect)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetScissorRect");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetScissorRect(ptr, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetScissorRect_Hooked(IDirect3DDevice9 * ptr, RECT* pRect)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetScissorRect");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetScissorRect(ptr, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetSoftwareVertexProcessing_Hooked(IDirect3DDevice9 * ptr, BOOL bSoftware)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetSoftwareVertexProcessing");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetSoftwareVertexProcessing(ptr, bSoftware);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static BOOL STDMETHODCALLTYPE IDirect3DDevice9_GetSoftwareVertexProcessing_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetSoftwareVertexProcessing");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetSoftwareVertexProcessing(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetNPatchMode_Hooked(IDirect3DDevice9 * ptr, float nSegments)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetNPatchMode");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetNPatchMode(ptr, nSegments);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static float STDMETHODCALLTYPE IDirect3DDevice9_GetNPatchMode_Hooked(IDirect3DDevice9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetNPatchMode");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetNPatchMode(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawPrimitive_Hooked(IDirect3DDevice9 * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::DrawPrimitive");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawPrimitive(ptr, PrimitiveType, StartVertex, PrimitiveCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawIndexedPrimitive_Hooked(IDirect3DDevice9 * ptr, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::DrawIndexedPrimitive");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawIndexedPrimitive(ptr, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawPrimitiveUP_Hooked(IDirect3DDevice9 * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::DrawPrimitiveUP");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawPrimitiveUP(ptr, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawIndexedPrimitiveUP_Hooked(IDirect3DDevice9 * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::DrawIndexedPrimitiveUP");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawIndexedPrimitiveUP(ptr, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_ProcessVertices_Hooked(IDirect3DDevice9 * ptr, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::ProcessVertices");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].ProcessVertices(ptr, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateVertexDeclaration_Hooked(IDirect3DDevice9 * ptr, CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateVertexDeclaration");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateVertexDeclaration(ptr, pVertexElements, ppDecl);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexDeclaration_Hooked(IDirect3DDevice9 * ptr, IDirect3DVertexDeclaration9* pDecl)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetVertexDeclaration");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexDeclaration(ptr, pDecl);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexDeclaration_Hooked(IDirect3DDevice9 * ptr, IDirect3DVertexDeclaration9** ppDecl)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetVertexDeclaration");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexDeclaration(ptr, ppDecl);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetFVF_Hooked(IDirect3DDevice9 * ptr, DWORD FVF)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetFVF");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetFVF(ptr, FVF);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetFVF_Hooked(IDirect3DDevice9 * ptr, DWORD* pFVF)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetFVF");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetFVF(ptr, pFVF);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateVertexShader_Hooked(IDirect3DDevice9 * ptr, CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateVertexShader");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateVertexShader(ptr, pFunction, ppShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexShader_Hooked(IDirect3DDevice9 * ptr, IDirect3DVertexShader9* pShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetVertexShader");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexShader(ptr, pShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexShader_Hooked(IDirect3DDevice9 * ptr, IDirect3DVertexShader9** ppShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetVertexShader");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexShader(ptr, ppShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexShaderConstantF_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetVertexShaderConstantF");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexShaderConstantF_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetVertexShaderConstantF");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexShaderConstantI_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetVertexShaderConstantI");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexShaderConstantI_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetVertexShaderConstantI");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetVertexShaderConstantB_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetVertexShaderConstantB");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetVertexShaderConstantB_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetVertexShaderConstantB");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetStreamSource_Hooked(IDirect3DDevice9 * ptr, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetStreamSource");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetStreamSource(ptr, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetStreamSource_Hooked(IDirect3DDevice9 * ptr, UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetStreamSource");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetStreamSource(ptr, StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetStreamSourceFreq_Hooked(IDirect3DDevice9 * ptr, UINT StreamNumber, UINT Setting)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetStreamSourceFreq");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetStreamSourceFreq(ptr, StreamNumber, Setting);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetStreamSourceFreq_Hooked(IDirect3DDevice9 * ptr, UINT StreamNumber, UINT* pSetting)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetStreamSourceFreq");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetStreamSourceFreq(ptr, StreamNumber, pSetting);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetIndices_Hooked(IDirect3DDevice9 * ptr, IDirect3DIndexBuffer9* pIndexData)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetIndices");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetIndices(ptr, pIndexData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetIndices_Hooked(IDirect3DDevice9 * ptr, IDirect3DIndexBuffer9** ppIndexData)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetIndices");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetIndices(ptr, ppIndexData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreatePixelShader_Hooked(IDirect3DDevice9 * ptr, CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreatePixelShader");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreatePixelShader(ptr, pFunction, ppShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPixelShader_Hooked(IDirect3DDevice9 * ptr, IDirect3DPixelShader9* pShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetPixelShader");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPixelShader(ptr, pShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPixelShader_Hooked(IDirect3DDevice9 * ptr, IDirect3DPixelShader9** ppShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetPixelShader");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPixelShader(ptr, ppShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPixelShaderConstantF_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetPixelShaderConstantF");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPixelShaderConstantF_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetPixelShaderConstantF");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPixelShaderConstantI_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetPixelShaderConstantI");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPixelShaderConstantI_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetPixelShaderConstantI");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_SetPixelShaderConstantB_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::SetPixelShaderConstantB");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_GetPixelShaderConstantB_Hooked(IDirect3DDevice9 * ptr, UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::GetPixelShaderConstantB");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawRectPatch_Hooked(IDirect3DDevice9 * ptr, UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::DrawRectPatch");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawRectPatch(ptr, Handle, pNumSegs, pRectPatchInfo);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DrawTriPatch_Hooked(IDirect3DDevice9 * ptr, UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::DrawTriPatch");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DrawTriPatch(ptr, Handle, pNumSegs, pTriPatchInfo);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_DeletePatch_Hooked(IDirect3DDevice9 * ptr, UINT Handle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::DeletePatch");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].DeletePatch(ptr, Handle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9_CreateQuery_Hooked(IDirect3DDevice9 * ptr, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
    calltrace::AutoTrace trace("IDirect3DDevice9::CreateQuery");
    return g_D3D9OriginVTables._IDirect3DDevice9.tables[INDEX].CreateQuery(ptr, Type, ppQuery);
}

// -----------------------------------------------------------------------------
// IDirect3DStateBlock9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DStateBlock9_QueryInterface_Hooked(IDirect3DStateBlock9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DStateBlock9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DStateBlock9_AddRef_Hooked(IDirect3DStateBlock9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DStateBlock9::AddRef");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DStateBlock9_Release_Hooked(IDirect3DStateBlock9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DStateBlock9::Release");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DStateBlock9_GetDevice_Hooked(IDirect3DStateBlock9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DStateBlock9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DStateBlock9_Capture_Hooked(IDirect3DStateBlock9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DStateBlock9::Capture");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].Capture(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DStateBlock9_Apply_Hooked(IDirect3DStateBlock9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DStateBlock9::Apply");
    return g_D3D9OriginVTables._IDirect3DStateBlock9.tables[INDEX].Apply(ptr);
}

// -----------------------------------------------------------------------------
// IDirect3DSwapChain9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_QueryInterface_Hooked(IDirect3DSwapChain9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DSwapChain9_AddRef_Hooked(IDirect3DSwapChain9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::AddRef");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DSwapChain9_Release_Hooked(IDirect3DSwapChain9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::Release");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_Present_Hooked(IDirect3DSwapChain9 * ptr, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::Present");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].Present(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetFrontBufferData_Hooked(IDirect3DSwapChain9 * ptr, IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::GetFrontBufferData");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetFrontBufferData(ptr, pDestSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetBackBuffer_Hooked(IDirect3DSwapChain9 * ptr, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::GetBackBuffer");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetBackBuffer(ptr, iBackBuffer, Type, ppBackBuffer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetRasterStatus_Hooked(IDirect3DSwapChain9 * ptr, D3DRASTER_STATUS* pRasterStatus)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::GetRasterStatus");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetRasterStatus(ptr, pRasterStatus);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetDisplayMode_Hooked(IDirect3DSwapChain9 * ptr, D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::GetDisplayMode");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetDisplayMode(ptr, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetDevice_Hooked(IDirect3DSwapChain9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9_GetPresentParameters_Hooked(IDirect3DSwapChain9 * ptr, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9::GetPresentParameters");
    return g_D3D9OriginVTables._IDirect3DSwapChain9.tables[INDEX].GetPresentParameters(ptr, pPresentationParameters);
}

// -----------------------------------------------------------------------------
// IDirect3DResource9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DResource9_QueryInterface_Hooked(IDirect3DResource9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DResource9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DResource9_AddRef_Hooked(IDirect3DResource9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DResource9::AddRef");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DResource9_Release_Hooked(IDirect3DResource9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DResource9::Release");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DResource9_GetDevice_Hooked(IDirect3DResource9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DResource9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DResource9_SetPrivateData_Hooked(IDirect3DResource9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DResource9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DResource9_GetPrivateData_Hooked(IDirect3DResource9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DResource9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DResource9_FreePrivateData_Hooked(IDirect3DResource9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DResource9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DResource9_SetPriority_Hooked(IDirect3DResource9 * ptr, DWORD PriorityNew)
{
    calltrace::AutoTrace trace("IDirect3DResource9::SetPriority");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].SetPriority(ptr, PriorityNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DResource9_GetPriority_Hooked(IDirect3DResource9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DResource9::GetPriority");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].GetPriority(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DResource9_PreLoad_Hooked(IDirect3DResource9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DResource9::PreLoad");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DResource9_GetType_Hooked(IDirect3DResource9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DResource9::GetType");
    return g_D3D9OriginVTables._IDirect3DResource9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
// IDirect3DVertexDeclaration9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexDeclaration9_QueryInterface_Hooked(IDirect3DVertexDeclaration9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DVertexDeclaration9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVertexDeclaration9_AddRef_Hooked(IDirect3DVertexDeclaration9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexDeclaration9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVertexDeclaration9_Release_Hooked(IDirect3DVertexDeclaration9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexDeclaration9::Release");
    return g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexDeclaration9_GetDevice_Hooked(IDirect3DVertexDeclaration9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DVertexDeclaration9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexDeclaration9_GetDeclaration_Hooked(IDirect3DVertexDeclaration9 * ptr, D3DVERTEXELEMENT9* pElement, UINT* pNumElements)
{
    calltrace::AutoTrace trace("IDirect3DVertexDeclaration9::GetDeclaration");
    return g_D3D9OriginVTables._IDirect3DVertexDeclaration9.tables[INDEX].GetDeclaration(ptr, pElement, pNumElements);
}

// -----------------------------------------------------------------------------
// IDirect3DVertexShader9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexShader9_QueryInterface_Hooked(IDirect3DVertexShader9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DVertexShader9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVertexShader9_AddRef_Hooked(IDirect3DVertexShader9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexShader9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVertexShader9_Release_Hooked(IDirect3DVertexShader9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexShader9::Release");
    return g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexShader9_GetDevice_Hooked(IDirect3DVertexShader9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DVertexShader9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexShader9_GetFunction_Hooked(IDirect3DVertexShader9 * ptr, void* pFunction, UINT* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DVertexShader9::GetFunction");
    return g_D3D9OriginVTables._IDirect3DVertexShader9.tables[INDEX].GetFunction(ptr, pFunction, pSizeOfData);
}

// -----------------------------------------------------------------------------
// IDirect3DPixelShader9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DPixelShader9_QueryInterface_Hooked(IDirect3DPixelShader9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DPixelShader9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DPixelShader9_AddRef_Hooked(IDirect3DPixelShader9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DPixelShader9::AddRef");
    return g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DPixelShader9_Release_Hooked(IDirect3DPixelShader9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DPixelShader9::Release");
    return g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DPixelShader9_GetDevice_Hooked(IDirect3DPixelShader9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DPixelShader9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DPixelShader9_GetFunction_Hooked(IDirect3DPixelShader9 * ptr, void* pFunction, UINT* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DPixelShader9::GetFunction");
    return g_D3D9OriginVTables._IDirect3DPixelShader9.tables[INDEX].GetFunction(ptr, pFunction, pSizeOfData);
}

// -----------------------------------------------------------------------------
// IDirect3DBaseTexture9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_QueryInterface_Hooked(IDirect3DBaseTexture9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DBaseTexture9_AddRef_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::AddRef");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DBaseTexture9_Release_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::Release");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_GetDevice_Hooked(IDirect3DBaseTexture9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_SetPrivateData_Hooked(IDirect3DBaseTexture9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_GetPrivateData_Hooked(IDirect3DBaseTexture9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_FreePrivateData_Hooked(IDirect3DBaseTexture9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_SetPriority_Hooked(IDirect3DBaseTexture9 * ptr, DWORD PriorityNew)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::SetPriority");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].SetPriority(ptr, PriorityNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_GetPriority_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::GetPriority");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetPriority(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DBaseTexture9_PreLoad_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::PreLoad");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DBaseTexture9_GetType_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::GetType");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_SetLOD_Hooked(IDirect3DBaseTexture9 * ptr, DWORD LODNew)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::SetLOD");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].SetLOD(ptr, LODNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_GetLOD_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::GetLOD");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetLOD(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DBaseTexture9_GetLevelCount_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::GetLevelCount");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetLevelCount(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DBaseTexture9_SetAutoGenFilterType_Hooked(IDirect3DBaseTexture9 * ptr, D3DTEXTUREFILTERTYPE FilterType)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::SetAutoGenFilterType");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].SetAutoGenFilterType(ptr, FilterType);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE IDirect3DBaseTexture9_GetAutoGenFilterType_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::GetAutoGenFilterType");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GetAutoGenFilterType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DBaseTexture9_GenerateMipSubLevels_Hooked(IDirect3DBaseTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DBaseTexture9::GenerateMipSubLevels");
    return g_D3D9OriginVTables._IDirect3DBaseTexture9.tables[INDEX].GenerateMipSubLevels(ptr);
}

// -----------------------------------------------------------------------------
// IDirect3DTexture9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_QueryInterface_Hooked(IDirect3DTexture9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DTexture9_AddRef_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::AddRef");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DTexture9_Release_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::Release");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_GetDevice_Hooked(IDirect3DTexture9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_SetPrivateData_Hooked(IDirect3DTexture9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_GetPrivateData_Hooked(IDirect3DTexture9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_FreePrivateData_Hooked(IDirect3DTexture9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DTexture9_SetPriority_Hooked(IDirect3DTexture9 * ptr, DWORD PriorityNew)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::SetPriority");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].SetPriority(ptr, PriorityNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DTexture9_GetPriority_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetPriority");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetPriority(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DTexture9_PreLoad_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::PreLoad");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DTexture9_GetType_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetType");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DTexture9_SetLOD_Hooked(IDirect3DTexture9 * ptr, DWORD LODNew)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::SetLOD");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].SetLOD(ptr, LODNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DTexture9_GetLOD_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetLOD");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetLOD(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DTexture9_GetLevelCount_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetLevelCount");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetLevelCount(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_SetAutoGenFilterType_Hooked(IDirect3DTexture9 * ptr, D3DTEXTUREFILTERTYPE FilterType)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::SetAutoGenFilterType");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].SetAutoGenFilterType(ptr, FilterType);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE IDirect3DTexture9_GetAutoGenFilterType_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetAutoGenFilterType");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetAutoGenFilterType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DTexture9_GenerateMipSubLevels_Hooked(IDirect3DTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GenerateMipSubLevels");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GenerateMipSubLevels(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_GetLevelDesc_Hooked(IDirect3DTexture9 * ptr, UINT Level, D3DSURFACE_DESC * pDesc)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetLevelDesc");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetLevelDesc(ptr, Level, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_GetSurfaceLevel_Hooked(IDirect3DTexture9 * ptr, UINT Level, IDirect3DSurface9** ppSurfaceLevel)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::GetSurfaceLevel");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].GetSurfaceLevel(ptr, Level, ppSurfaceLevel);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_LockRect_Hooked(IDirect3DTexture9 * ptr, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::LockRect");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].LockRect(ptr, Level, pLockedRect, pRect, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_UnlockRect_Hooked(IDirect3DTexture9 * ptr, UINT Level)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::UnlockRect");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].UnlockRect(ptr, Level);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DTexture9_AddDirtyRect_Hooked(IDirect3DTexture9 * ptr, CONST RECT* pDirtyRect)
{
    calltrace::AutoTrace trace("IDirect3DTexture9::AddDirtyRect");
    return g_D3D9OriginVTables._IDirect3DTexture9.tables[INDEX].AddDirtyRect(ptr, pDirtyRect);
}

// -----------------------------------------------------------------------------
// IDirect3DVolumeTexture9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_QueryInterface_Hooked(IDirect3DVolumeTexture9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVolumeTexture9_AddRef_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVolumeTexture9_Release_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::Release");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetDevice_Hooked(IDirect3DVolumeTexture9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_SetPrivateData_Hooked(IDirect3DVolumeTexture9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetPrivateData_Hooked(IDirect3DVolumeTexture9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_FreePrivateData_Hooked(IDirect3DVolumeTexture9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_SetPriority_Hooked(IDirect3DVolumeTexture9 * ptr, DWORD PriorityNew)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::SetPriority");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].SetPriority(ptr, PriorityNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetPriority_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetPriority");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetPriority(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DVolumeTexture9_PreLoad_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::PreLoad");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetType_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetType");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_SetLOD_Hooked(IDirect3DVolumeTexture9 * ptr, DWORD LODNew)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::SetLOD");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].SetLOD(ptr, LODNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetLOD_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetLOD");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLOD(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetLevelCount_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetLevelCount");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLevelCount(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_SetAutoGenFilterType_Hooked(IDirect3DVolumeTexture9 * ptr, D3DTEXTUREFILTERTYPE FilterType)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::SetAutoGenFilterType");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].SetAutoGenFilterType(ptr, FilterType);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetAutoGenFilterType_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetAutoGenFilterType");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetAutoGenFilterType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DVolumeTexture9_GenerateMipSubLevels_Hooked(IDirect3DVolumeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GenerateMipSubLevels");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GenerateMipSubLevels(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetLevelDesc_Hooked(IDirect3DVolumeTexture9 * ptr, UINT Level, D3DVOLUME_DESC * pDesc)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetLevelDesc");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLevelDesc(ptr, Level, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_GetVolumeLevel_Hooked(IDirect3DVolumeTexture9 * ptr, UINT Level, IDirect3DVolume9** ppVolumeLevel)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::GetVolumeLevel");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].GetVolumeLevel(ptr, Level, ppVolumeLevel);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_LockBox_Hooked(IDirect3DVolumeTexture9 * ptr, UINT Level, D3DLOCKED_BOX* pLockedVolume, CONST D3DBOX* pBox, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::LockBox");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].LockBox(ptr, Level, pLockedVolume, pBox, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_UnlockBox_Hooked(IDirect3DVolumeTexture9 * ptr, UINT Level)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::UnlockBox");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].UnlockBox(ptr, Level);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolumeTexture9_AddDirtyBox_Hooked(IDirect3DVolumeTexture9 * ptr, CONST D3DBOX* pDirtyBox)
{
    calltrace::AutoTrace trace("IDirect3DVolumeTexture9::AddDirtyBox");
    return g_D3D9OriginVTables._IDirect3DVolumeTexture9.tables[INDEX].AddDirtyBox(ptr, pDirtyBox);
}

// -----------------------------------------------------------------------------
// IDirect3DCubeTexture9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_QueryInterface_Hooked(IDirect3DCubeTexture9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DCubeTexture9_AddRef_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::AddRef");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DCubeTexture9_Release_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::Release");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_GetDevice_Hooked(IDirect3DCubeTexture9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_SetPrivateData_Hooked(IDirect3DCubeTexture9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_GetPrivateData_Hooked(IDirect3DCubeTexture9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_FreePrivateData_Hooked(IDirect3DCubeTexture9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_SetPriority_Hooked(IDirect3DCubeTexture9 * ptr, DWORD PriorityNew)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::SetPriority");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].SetPriority(ptr, PriorityNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_GetPriority_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetPriority");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetPriority(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DCubeTexture9_PreLoad_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::PreLoad");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DCubeTexture9_GetType_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetType");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_SetLOD_Hooked(IDirect3DCubeTexture9 * ptr, DWORD LODNew)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::SetLOD");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].SetLOD(ptr, LODNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_GetLOD_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetLOD");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetLOD(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DCubeTexture9_GetLevelCount_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetLevelCount");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetLevelCount(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_SetAutoGenFilterType_Hooked(IDirect3DCubeTexture9 * ptr, D3DTEXTUREFILTERTYPE FilterType)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::SetAutoGenFilterType");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].SetAutoGenFilterType(ptr, FilterType);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE IDirect3DCubeTexture9_GetAutoGenFilterType_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetAutoGenFilterType");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetAutoGenFilterType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DCubeTexture9_GenerateMipSubLevels_Hooked(IDirect3DCubeTexture9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GenerateMipSubLevels");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GenerateMipSubLevels(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_GetLevelDesc_Hooked(IDirect3DCubeTexture9 * ptr, UINT Level, D3DSURFACE_DESC * pDesc)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetLevelDesc");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetLevelDesc(ptr, Level, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_GetCubeMapSurface_Hooked(IDirect3DCubeTexture9 * ptr, D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::GetCubeMapSurface");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].GetCubeMapSurface(ptr, FaceType, Level, ppCubeMapSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_LockRect_Hooked(IDirect3DCubeTexture9 * ptr, D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::LockRect");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].LockRect(ptr, FaceType, Level, pLockedRect, pRect, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_UnlockRect_Hooked(IDirect3DCubeTexture9 * ptr, D3DCUBEMAP_FACES FaceType, UINT Level)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::UnlockRect");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].UnlockRect(ptr, FaceType, Level);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCubeTexture9_AddDirtyRect_Hooked(IDirect3DCubeTexture9 * ptr, D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect)
{
    calltrace::AutoTrace trace("IDirect3DCubeTexture9::AddDirtyRect");
    return g_D3D9OriginVTables._IDirect3DCubeTexture9.tables[INDEX].AddDirtyRect(ptr, FaceType, pDirtyRect);
}

// -----------------------------------------------------------------------------
// IDirect3DVertexBuffer9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_QueryInterface_Hooked(IDirect3DVertexBuffer9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVertexBuffer9_AddRef_Hooked(IDirect3DVertexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVertexBuffer9_Release_Hooked(IDirect3DVertexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::Release");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetDevice_Hooked(IDirect3DVertexBuffer9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_SetPrivateData_Hooked(IDirect3DVertexBuffer9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetPrivateData_Hooked(IDirect3DVertexBuffer9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_FreePrivateData_Hooked(IDirect3DVertexBuffer9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DVertexBuffer9_SetPriority_Hooked(IDirect3DVertexBuffer9 * ptr, DWORD PriorityNew)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::SetPriority");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].SetPriority(ptr, PriorityNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetPriority_Hooked(IDirect3DVertexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::GetPriority");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetPriority(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DVertexBuffer9_PreLoad_Hooked(IDirect3DVertexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::PreLoad");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetType_Hooked(IDirect3DVertexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::GetType");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_Lock_Hooked(IDirect3DVertexBuffer9 * ptr, UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::Lock");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].Lock(ptr, OffsetToLock, SizeToLock, ppbData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_Unlock_Hooked(IDirect3DVertexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::Unlock");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].Unlock(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVertexBuffer9_GetDesc_Hooked(IDirect3DVertexBuffer9 * ptr, D3DVERTEXBUFFER_DESC * pDesc)
{
    calltrace::AutoTrace trace("IDirect3DVertexBuffer9::GetDesc");
    return g_D3D9OriginVTables._IDirect3DVertexBuffer9.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// IDirect3DIndexBuffer9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_QueryInterface_Hooked(IDirect3DIndexBuffer9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DIndexBuffer9_AddRef_Hooked(IDirect3DIndexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::AddRef");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DIndexBuffer9_Release_Hooked(IDirect3DIndexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::Release");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetDevice_Hooked(IDirect3DIndexBuffer9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_SetPrivateData_Hooked(IDirect3DIndexBuffer9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetPrivateData_Hooked(IDirect3DIndexBuffer9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_FreePrivateData_Hooked(IDirect3DIndexBuffer9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DIndexBuffer9_SetPriority_Hooked(IDirect3DIndexBuffer9 * ptr, DWORD PriorityNew)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::SetPriority");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].SetPriority(ptr, PriorityNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetPriority_Hooked(IDirect3DIndexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::GetPriority");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetPriority(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DIndexBuffer9_PreLoad_Hooked(IDirect3DIndexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::PreLoad");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetType_Hooked(IDirect3DIndexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::GetType");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_Lock_Hooked(IDirect3DIndexBuffer9 * ptr, UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::Lock");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].Lock(ptr, OffsetToLock, SizeToLock, ppbData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_Unlock_Hooked(IDirect3DIndexBuffer9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::Unlock");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].Unlock(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DIndexBuffer9_GetDesc_Hooked(IDirect3DIndexBuffer9 * ptr, D3DINDEXBUFFER_DESC * pDesc)
{
    calltrace::AutoTrace trace("IDirect3DIndexBuffer9::GetDesc");
    return g_D3D9OriginVTables._IDirect3DIndexBuffer9.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// IDirect3DSurface9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_QueryInterface_Hooked(IDirect3DSurface9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DSurface9_AddRef_Hooked(IDirect3DSurface9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::AddRef");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DSurface9_Release_Hooked(IDirect3DSurface9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::Release");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetDevice_Hooked(IDirect3DSurface9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_SetPrivateData_Hooked(IDirect3DSurface9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetPrivateData_Hooked(IDirect3DSurface9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_FreePrivateData_Hooked(IDirect3DSurface9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DSurface9_SetPriority_Hooked(IDirect3DSurface9 * ptr, DWORD PriorityNew)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::SetPriority");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].SetPriority(ptr, PriorityNew);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DSurface9_GetPriority_Hooked(IDirect3DSurface9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::GetPriority");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetPriority(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DSurface9_PreLoad_Hooked(IDirect3DSurface9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::PreLoad");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].PreLoad(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DRESOURCETYPE STDMETHODCALLTYPE IDirect3DSurface9_GetType_Hooked(IDirect3DSurface9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::GetType");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetContainer_Hooked(IDirect3DSurface9 * ptr, REFIID riid, void** ppContainer)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::GetContainer");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetContainer(ptr, riid, ppContainer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetDesc_Hooked(IDirect3DSurface9 * ptr, D3DSURFACE_DESC * pDesc)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::GetDesc");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_LockRect_Hooked(IDirect3DSurface9 * ptr, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::LockRect");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].LockRect(ptr, pLockedRect, pRect, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_UnlockRect_Hooked(IDirect3DSurface9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::UnlockRect");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].UnlockRect(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_GetDC_Hooked(IDirect3DSurface9 * ptr, HDC * phdc)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::GetDC");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].GetDC(ptr, phdc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSurface9_ReleaseDC_Hooked(IDirect3DSurface9 * ptr, HDC hdc)
{
    calltrace::AutoTrace trace("IDirect3DSurface9::ReleaseDC");
    return g_D3D9OriginVTables._IDirect3DSurface9.tables[INDEX].ReleaseDC(ptr, hdc);
}

// -----------------------------------------------------------------------------
// IDirect3DVolume9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_QueryInterface_Hooked(IDirect3DVolume9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVolume9_AddRef_Hooked(IDirect3DVolume9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::AddRef");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DVolume9_Release_Hooked(IDirect3DVolume9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::Release");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_GetDevice_Hooked(IDirect3DVolume9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_SetPrivateData_Hooked(IDirect3DVolume9 * ptr, REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::SetPrivateData");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].SetPrivateData(ptr, refguid, pData, SizeOfData, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_GetPrivateData_Hooked(IDirect3DVolume9 * ptr, REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::GetPrivateData");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].GetPrivateData(ptr, refguid, pData, pSizeOfData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_FreePrivateData_Hooked(IDirect3DVolume9 * ptr, REFGUID refguid)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::FreePrivateData");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].FreePrivateData(ptr, refguid);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_GetContainer_Hooked(IDirect3DVolume9 * ptr, REFIID riid, void** ppContainer)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::GetContainer");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].GetContainer(ptr, riid, ppContainer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_GetDesc_Hooked(IDirect3DVolume9 * ptr, D3DVOLUME_DESC * pDesc)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::GetDesc");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_LockBox_Hooked(IDirect3DVolume9 * ptr, D3DLOCKED_BOX * pLockedVolume, CONST D3DBOX* pBox, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::LockBox");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].LockBox(ptr, pLockedVolume, pBox, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DVolume9_UnlockBox_Hooked(IDirect3DVolume9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DVolume9::UnlockBox");
    return g_D3D9OriginVTables._IDirect3DVolume9.tables[INDEX].UnlockBox(ptr);
}

// -----------------------------------------------------------------------------
// IDirect3DQuery9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DQuery9_QueryInterface_Hooked(IDirect3DQuery9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DQuery9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DQuery9_AddRef_Hooked(IDirect3DQuery9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DQuery9::AddRef");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DQuery9_Release_Hooked(IDirect3DQuery9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DQuery9::Release");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DQuery9_GetDevice_Hooked(IDirect3DQuery9 * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DQuery9::GetDevice");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static D3DQUERYTYPE STDMETHODCALLTYPE IDirect3DQuery9_GetType_Hooked(IDirect3DQuery9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DQuery9::GetType");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].GetType(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static DWORD STDMETHODCALLTYPE IDirect3DQuery9_GetDataSize_Hooked(IDirect3DQuery9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DQuery9::GetDataSize");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].GetDataSize(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DQuery9_Issue_Hooked(IDirect3DQuery9 * ptr, DWORD dwIssueFlags)
{
    calltrace::AutoTrace trace("IDirect3DQuery9::Issue");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].Issue(ptr, dwIssueFlags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DQuery9_GetData_Hooked(IDirect3DQuery9 * ptr, void* pData, DWORD dwSize, DWORD dwGetDataFlags)
{
    calltrace::AutoTrace trace("IDirect3DQuery9::GetData");
    return g_D3D9OriginVTables._IDirect3DQuery9.tables[INDEX].GetData(ptr, pData, dwSize, dwGetDataFlags);
}

// -----------------------------------------------------------------------------
// IDirect3D9Ex
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_QueryInterface_Hooked(IDirect3D9Ex * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::QueryInterface");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3D9Ex_AddRef_Hooked(IDirect3D9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::AddRef");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3D9Ex_Release_Hooked(IDirect3D9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::Release");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterCount_Hooked(IDirect3D9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetAdapterCount");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterCount(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterIdentifier_Hooked(IDirect3D9Ex * ptr, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetAdapterIdentifier");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterIdentifier(ptr, Adapter, Flags, pIdentifier);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterModeCount_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DFORMAT Format)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetAdapterModeCount");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterModeCount(ptr, Adapter, Format);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_EnumAdapterModes_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::EnumAdapterModes");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].EnumAdapterModes(ptr, Adapter, Format, Mode, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterDisplayMode_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetAdapterDisplayMode");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterDisplayMode(ptr, Adapter, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDeviceType_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::CheckDeviceType");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceType(ptr, Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDeviceFormat_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::CheckDeviceFormat");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceFormat(ptr, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDeviceMultiSampleType_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::CheckDeviceMultiSampleType");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceMultiSampleType(ptr, Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDepthStencilMatch_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::CheckDepthStencilMatch");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDepthStencilMatch(ptr, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CheckDeviceFormatConversion_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::CheckDeviceFormatConversion");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceFormatConversion(ptr, Adapter, DeviceType, SourceFormat, TargetFormat);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetDeviceCaps_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetDeviceCaps");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetDeviceCaps(ptr, Adapter, DeviceType, pCaps);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HMONITOR STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterMonitor_Hooked(IDirect3D9Ex * ptr, UINT Adapter)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetAdapterMonitor");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterMonitor(ptr, Adapter);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CreateDevice_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::CreateDevice");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CreateDevice(ptr, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterModeCountEx_Hooked(IDirect3D9Ex * ptr, UINT Adapter, CONST D3DDISPLAYMODEFILTER* pFilter)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetAdapterModeCountEx");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterModeCountEx(ptr, Adapter, pFilter);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_EnumAdapterModesEx_Hooked(IDirect3D9Ex * ptr, UINT Adapter, CONST D3DDISPLAYMODEFILTER* pFilter, UINT Mode, D3DDISPLAYMODEEX* pMode)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::EnumAdapterModesEx");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].EnumAdapterModesEx(ptr, Adapter, pFilter, Mode, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterDisplayModeEx_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetAdapterDisplayModeEx");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterDisplayModeEx(ptr, Adapter, pMode, pRotation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_CreateDeviceEx_Hooked(IDirect3D9Ex * ptr, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::CreateDeviceEx");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].CreateDeviceEx(ptr, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9Ex_GetAdapterLUID_Hooked(IDirect3D9Ex * ptr, UINT Adapter, LUID * pLUID)
{
    calltrace::AutoTrace trace("IDirect3D9Ex::GetAdapterLUID");
    return g_D3D9OriginVTables._IDirect3D9Ex.tables[INDEX].GetAdapterLUID(ptr, Adapter, pLUID);
}

// -----------------------------------------------------------------------------
// IDirect3DDevice9Ex
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_QueryInterface_Hooked(IDirect3DDevice9Ex * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DDevice9Ex_AddRef_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::AddRef");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DDevice9Ex_Release_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::Release");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_TestCooperativeLevel_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::TestCooperativeLevel");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].TestCooperativeLevel(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetAvailableTextureMem_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetAvailableTextureMem");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetAvailableTextureMem(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_EvictManagedResources_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::EvictManagedResources");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].EvictManagedResources(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDirect3D_Hooked(IDirect3DDevice9Ex * ptr, IDirect3D9** ppD3D9)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetDirect3D");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDirect3D(ptr, ppD3D9);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDeviceCaps_Hooked(IDirect3DDevice9Ex * ptr, D3DCAPS9* pCaps)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetDeviceCaps");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDeviceCaps(ptr, pCaps);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDisplayMode_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetDisplayMode");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDisplayMode(ptr, iSwapChain, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetCreationParameters_Hooked(IDirect3DDevice9Ex * ptr, D3DDEVICE_CREATION_PARAMETERS * pParameters)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetCreationParameters");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetCreationParameters(ptr, pParameters);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetCursorProperties_Hooked(IDirect3DDevice9Ex * ptr, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetCursorProperties");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetCursorProperties(ptr, XHotSpot, YHotSpot, pCursorBitmap);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DDevice9Ex_SetCursorPosition_Hooked(IDirect3DDevice9Ex * ptr, int X, int Y, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetCursorPosition");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetCursorPosition(ptr, X, Y, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static BOOL STDMETHODCALLTYPE IDirect3DDevice9Ex_ShowCursor_Hooked(IDirect3DDevice9Ex * ptr, BOOL bShow)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::ShowCursor");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ShowCursor(ptr, bShow);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateAdditionalSwapChain_Hooked(IDirect3DDevice9Ex * ptr, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** ppSwapChain)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateAdditionalSwapChain");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateAdditionalSwapChain(ptr, pPresentationParameters, ppSwapChain);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetSwapChain_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, IDirect3DSwapChain9** ppSwapChain)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetSwapChain");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetSwapChain(ptr, iSwapChain, ppSwapChain);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static UINT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetNumberOfSwapChains_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetNumberOfSwapChains");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetNumberOfSwapChains(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_Reset_Hooked(IDirect3DDevice9Ex * ptr, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::Reset");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].Reset(ptr, pPresentationParameters);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_Present_Hooked(IDirect3DDevice9Ex * ptr, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::Present");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].Present(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetBackBuffer_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetBackBuffer");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetBackBuffer(ptr, iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetRasterStatus_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetRasterStatus");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetRasterStatus(ptr, iSwapChain, pRasterStatus);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetDialogBoxMode_Hooked(IDirect3DDevice9Ex * ptr, BOOL bEnableDialogs)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetDialogBoxMode");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetDialogBoxMode(ptr, bEnableDialogs);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DDevice9Ex_SetGammaRamp_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetGammaRamp");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetGammaRamp(ptr, iSwapChain, Flags, pRamp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void STDMETHODCALLTYPE IDirect3DDevice9Ex_GetGammaRamp_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetGammaRamp");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetGammaRamp(ptr, iSwapChain, pRamp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateTexture_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateTexture(ptr, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateVolumeTexture_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateVolumeTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVolumeTexture(ptr, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateCubeTexture_Hooked(IDirect3DDevice9Ex * ptr, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateCubeTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateCubeTexture(ptr, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateVertexBuffer_Hooked(IDirect3DDevice9Ex * ptr, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateVertexBuffer");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexBuffer(ptr, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateIndexBuffer_Hooked(IDirect3DDevice9Ex * ptr, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateIndexBuffer");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateIndexBuffer(ptr, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateRenderTarget_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateRenderTarget");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateRenderTarget(ptr, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateDepthStencilSurface_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateDepthStencilSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateDepthStencilSurface(ptr, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_UpdateSurface_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::UpdateSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].UpdateSurface(ptr, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_UpdateTexture_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::UpdateTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].UpdateTexture(ptr, pSourceTexture, pDestinationTexture);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetRenderTargetData_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetRenderTargetData");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderTargetData(ptr, pRenderTarget, pDestSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetFrontBufferData_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetFrontBufferData");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetFrontBufferData(ptr, iSwapChain, pDestSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_StretchRect_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::StretchRect");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].StretchRect(ptr, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ColorFill_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::ColorFill");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ColorFill(ptr, pSurface, pRect, color);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateOffscreenPlainSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateOffscreenPlainSurface(ptr, Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetRenderTarget_Hooked(IDirect3DDevice9Ex * ptr, DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetRenderTarget");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetRenderTarget(ptr, RenderTargetIndex, pRenderTarget);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetRenderTarget_Hooked(IDirect3DDevice9Ex * ptr, DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetRenderTarget");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderTarget(ptr, RenderTargetIndex, ppRenderTarget);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetDepthStencilSurface_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9* pNewZStencil)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetDepthStencilSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetDepthStencilSurface(ptr, pNewZStencil);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDepthStencilSurface_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9** ppZStencilSurface)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetDepthStencilSurface");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDepthStencilSurface(ptr, ppZStencilSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_BeginScene_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::BeginScene");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].BeginScene(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_EndScene_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::EndScene");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].EndScene(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_Clear_Hooked(IDirect3DDevice9Ex * ptr, DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::Clear");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].Clear(ptr, Count, pRects, Flags, Color, Z, Stencil);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetTransform_Hooked(IDirect3DDevice9Ex * ptr, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetTransform");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetTransform(ptr, State, pMatrix);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetTransform_Hooked(IDirect3DDevice9Ex * ptr, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetTransform");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetTransform(ptr, State, pMatrix);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_MultiplyTransform_Hooked(IDirect3DDevice9Ex * ptr, D3DTRANSFORMSTATETYPE TransformType, CONST D3DMATRIX* Value)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::MultiplyTransform");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].MultiplyTransform(ptr, TransformType, Value);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetViewport_Hooked(IDirect3DDevice9Ex * ptr, CONST D3DVIEWPORT9* pViewport)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetViewport");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetViewport(ptr, pViewport);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetViewport_Hooked(IDirect3DDevice9Ex * ptr, D3DVIEWPORT9* pViewport)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetViewport");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetViewport(ptr, pViewport);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetMaterial_Hooked(IDirect3DDevice9Ex * ptr, CONST D3DMATERIAL9* pMaterial)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetMaterial");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetMaterial(ptr, pMaterial);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetMaterial_Hooked(IDirect3DDevice9Ex * ptr, D3DMATERIAL9* pMaterial)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetMaterial");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetMaterial(ptr, pMaterial);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetLight_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, CONST D3DLIGHT9* pLight)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetLight");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetLight(ptr, Index, pLight);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetLight_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, D3DLIGHT9* pLight)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetLight");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetLight(ptr, Index, pLight);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_LightEnable_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, BOOL Enable)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::LightEnable");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].LightEnable(ptr, Index, Enable);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetLightEnable_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, BOOL* pEnable)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetLightEnable");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetLightEnable(ptr, Index, pEnable);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetClipPlane_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, CONST float* pPlane)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetClipPlane");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetClipPlane(ptr, Index, pPlane);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetClipPlane_Hooked(IDirect3DDevice9Ex * ptr, DWORD Index, float* pPlane)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetClipPlane");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetClipPlane(ptr, Index, pPlane);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetRenderState_Hooked(IDirect3DDevice9Ex * ptr, D3DRENDERSTATETYPE State, DWORD Value)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetRenderState");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetRenderState(ptr, State, Value);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetRenderState_Hooked(IDirect3DDevice9Ex * ptr, D3DRENDERSTATETYPE State, DWORD* pValue)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetRenderState");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderState(ptr, State, pValue);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateStateBlock_Hooked(IDirect3DDevice9Ex * ptr, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateStateBlock");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateStateBlock(ptr, Type, ppSB);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_BeginStateBlock_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::BeginStateBlock");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].BeginStateBlock(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_EndStateBlock_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DStateBlock9** ppSB)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::EndStateBlock");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].EndStateBlock(ptr, ppSB);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetClipStatus_Hooked(IDirect3DDevice9Ex * ptr, CONST D3DCLIPSTATUS9* pClipStatus)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetClipStatus");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetClipStatus(ptr, pClipStatus);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetClipStatus_Hooked(IDirect3DDevice9Ex * ptr, D3DCLIPSTATUS9* pClipStatus)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetClipStatus");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetClipStatus(ptr, pClipStatus);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetTexture_Hooked(IDirect3DDevice9Ex * ptr, DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetTexture(ptr, Stage, ppTexture);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetTexture_Hooked(IDirect3DDevice9Ex * ptr, DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetTexture");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetTexture(ptr, Stage, pTexture);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetTextureStageState_Hooked(IDirect3DDevice9Ex * ptr, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetTextureStageState");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetTextureStageState(ptr, Stage, Type, pValue);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetTextureStageState_Hooked(IDirect3DDevice9Ex * ptr, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetTextureStageState");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetTextureStageState(ptr, Stage, Type, Value);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetSamplerState_Hooked(IDirect3DDevice9Ex * ptr, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetSamplerState");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetSamplerState(ptr, Sampler, Type, pValue);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetSamplerState_Hooked(IDirect3DDevice9Ex * ptr, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetSamplerState");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetSamplerState(ptr, Sampler, Type, Value);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ValidateDevice_Hooked(IDirect3DDevice9Ex * ptr, DWORD* pNumPasses)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::ValidateDevice");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ValidateDevice(ptr, pNumPasses);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPaletteEntries_Hooked(IDirect3DDevice9Ex * ptr, UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetPaletteEntries");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPaletteEntries(ptr, PaletteNumber, pEntries);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPaletteEntries_Hooked(IDirect3DDevice9Ex * ptr, UINT PaletteNumber, PALETTEENTRY* pEntries)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetPaletteEntries");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPaletteEntries(ptr, PaletteNumber, pEntries);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetCurrentTexturePalette_Hooked(IDirect3DDevice9Ex * ptr, UINT PaletteNumber)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetCurrentTexturePalette");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetCurrentTexturePalette(ptr, PaletteNumber);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetCurrentTexturePalette_Hooked(IDirect3DDevice9Ex * ptr, UINT * PaletteNumber)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetCurrentTexturePalette");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetCurrentTexturePalette(ptr, PaletteNumber);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetScissorRect_Hooked(IDirect3DDevice9Ex * ptr, CONST RECT* pRect)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetScissorRect");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetScissorRect(ptr, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetScissorRect_Hooked(IDirect3DDevice9Ex * ptr, RECT* pRect)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetScissorRect");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetScissorRect(ptr, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Hooked(IDirect3DDevice9Ex * ptr, BOOL bSoftware)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetSoftwareVertexProcessing");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetSoftwareVertexProcessing(ptr, bSoftware);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static BOOL STDMETHODCALLTYPE IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetSoftwareVertexProcessing");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetSoftwareVertexProcessing(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetNPatchMode_Hooked(IDirect3DDevice9Ex * ptr, float nSegments)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetNPatchMode");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetNPatchMode(ptr, nSegments);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static float STDMETHODCALLTYPE IDirect3DDevice9Ex_GetNPatchMode_Hooked(IDirect3DDevice9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetNPatchMode");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetNPatchMode(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawPrimitive_Hooked(IDirect3DDevice9Ex * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::DrawPrimitive");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawPrimitive(ptr, PrimitiveType, StartVertex, PrimitiveCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawIndexedPrimitive_Hooked(IDirect3DDevice9Ex * ptr, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::DrawIndexedPrimitive");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawIndexedPrimitive(ptr, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawPrimitiveUP_Hooked(IDirect3DDevice9Ex * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::DrawPrimitiveUP");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawPrimitiveUP(ptr, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Hooked(IDirect3DDevice9Ex * ptr, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::DrawIndexedPrimitiveUP");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawIndexedPrimitiveUP(ptr, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ProcessVertices_Hooked(IDirect3DDevice9Ex * ptr, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::ProcessVertices");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ProcessVertices(ptr, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateVertexDeclaration_Hooked(IDirect3DDevice9Ex * ptr, CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateVertexDeclaration");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexDeclaration(ptr, pVertexElements, ppDecl);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexDeclaration_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DVertexDeclaration9* pDecl)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetVertexDeclaration");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexDeclaration(ptr, pDecl);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexDeclaration_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DVertexDeclaration9** ppDecl)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetVertexDeclaration");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexDeclaration(ptr, ppDecl);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetFVF_Hooked(IDirect3DDevice9Ex * ptr, DWORD FVF)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetFVF");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetFVF(ptr, FVF);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetFVF_Hooked(IDirect3DDevice9Ex * ptr, DWORD* pFVF)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetFVF");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetFVF(ptr, pFVF);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateVertexShader_Hooked(IDirect3DDevice9Ex * ptr, CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateVertexShader");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexShader(ptr, pFunction, ppShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexShader_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DVertexShader9* pShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetVertexShader");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShader(ptr, pShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexShader_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DVertexShader9** ppShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetVertexShader");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShader(ptr, ppShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexShaderConstantF_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetVertexShaderConstantF");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexShaderConstantF_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetVertexShaderConstantF");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexShaderConstantI_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetVertexShaderConstantI");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexShaderConstantI_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetVertexShaderConstantI");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetVertexShaderConstantB_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetVertexShaderConstantB");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetVertexShaderConstantB_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetVertexShaderConstantB");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetStreamSource_Hooked(IDirect3DDevice9Ex * ptr, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetStreamSource");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetStreamSource(ptr, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetStreamSource_Hooked(IDirect3DDevice9Ex * ptr, UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetStreamSource");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetStreamSource(ptr, StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetStreamSourceFreq_Hooked(IDirect3DDevice9Ex * ptr, UINT StreamNumber, UINT Setting)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetStreamSourceFreq");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetStreamSourceFreq(ptr, StreamNumber, Setting);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetStreamSourceFreq_Hooked(IDirect3DDevice9Ex * ptr, UINT StreamNumber, UINT* pSetting)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetStreamSourceFreq");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetStreamSourceFreq(ptr, StreamNumber, pSetting);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetIndices_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DIndexBuffer9* pIndexData)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetIndices");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetIndices(ptr, pIndexData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetIndices_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DIndexBuffer9** ppIndexData)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetIndices");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetIndices(ptr, ppIndexData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreatePixelShader_Hooked(IDirect3DDevice9Ex * ptr, CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreatePixelShader");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreatePixelShader(ptr, pFunction, ppShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPixelShader_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DPixelShader9* pShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetPixelShader");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShader(ptr, pShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPixelShader_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DPixelShader9** ppShader)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetPixelShader");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShader(ptr, ppShader);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPixelShaderConstantF_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetPixelShaderConstantF");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPixelShaderConstantF_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetPixelShaderConstantF");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantF(ptr, StartRegister, pConstantData, Vector4fCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPixelShaderConstantI_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetPixelShaderConstantI");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPixelShaderConstantI_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetPixelShaderConstantI");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantI(ptr, StartRegister, pConstantData, Vector4iCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetPixelShaderConstantB_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetPixelShaderConstantB");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetPixelShaderConstantB_Hooked(IDirect3DDevice9Ex * ptr, UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetPixelShaderConstantB");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantB(ptr, StartRegister, pConstantData, BoolCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawRectPatch_Hooked(IDirect3DDevice9Ex * ptr, UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::DrawRectPatch");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawRectPatch(ptr, Handle, pNumSegs, pRectPatchInfo);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DrawTriPatch_Hooked(IDirect3DDevice9Ex * ptr, UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::DrawTriPatch");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DrawTriPatch(ptr, Handle, pNumSegs, pTriPatchInfo);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_DeletePatch_Hooked(IDirect3DDevice9Ex * ptr, UINT Handle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::DeletePatch");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].DeletePatch(ptr, Handle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateQuery_Hooked(IDirect3DDevice9Ex * ptr, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateQuery");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateQuery(ptr, Type, ppQuery);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetConvolutionMonoKernel_Hooked(IDirect3DDevice9Ex * ptr, UINT width, UINT height, float* rows, float* columns)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetConvolutionMonoKernel");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetConvolutionMonoKernel(ptr, width, height, rows, columns);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ComposeRects_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DSurface9* pSrc, IDirect3DSurface9* pDst, IDirect3DVertexBuffer9* pSrcRectDescs, UINT NumRects, IDirect3DVertexBuffer9* pDstRectDescs, D3DCOMPOSERECTSOP Operation, int Xoffset, int Yoffset)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::ComposeRects");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ComposeRects(ptr, pSrc, pDst, pSrcRectDescs, NumRects, pDstRectDescs, Operation, Xoffset, Yoffset);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_PresentEx_Hooked(IDirect3DDevice9Ex * ptr, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::PresentEx");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].PresentEx(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetGPUThreadPriority_Hooked(IDirect3DDevice9Ex * ptr, INT* pPriority)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetGPUThreadPriority");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetGPUThreadPriority(ptr, pPriority);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetGPUThreadPriority_Hooked(IDirect3DDevice9Ex * ptr, INT Priority)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetGPUThreadPriority");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetGPUThreadPriority(ptr, Priority);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_WaitForVBlank_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::WaitForVBlank");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].WaitForVBlank(ptr, iSwapChain);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CheckResourceResidency_Hooked(IDirect3DDevice9Ex * ptr, IDirect3DResource9** pResourceArray, UINT32 NumResources)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CheckResourceResidency");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CheckResourceResidency(ptr, pResourceArray, NumResources);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_SetMaximumFrameLatency_Hooked(IDirect3DDevice9Ex * ptr, UINT MaxLatency)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::SetMaximumFrameLatency");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].SetMaximumFrameLatency(ptr, MaxLatency);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetMaximumFrameLatency_Hooked(IDirect3DDevice9Ex * ptr, UINT* pMaxLatency)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetMaximumFrameLatency");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetMaximumFrameLatency(ptr, pMaxLatency);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CheckDeviceState_Hooked(IDirect3DDevice9Ex * ptr, HWND hDestinationWindow)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CheckDeviceState");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CheckDeviceState(ptr, hDestinationWindow);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateRenderTargetEx_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateRenderTargetEx");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateRenderTargetEx(ptr, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, Usage);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateOffscreenPlainSurfaceEx_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateOffscreenPlainSurfaceEx");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateOffscreenPlainSurfaceEx(ptr, Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_CreateDepthStencilSurfaceEx_Hooked(IDirect3DDevice9Ex * ptr, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::CreateDepthStencilSurfaceEx");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].CreateDepthStencilSurfaceEx(ptr, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_ResetEx_Hooked(IDirect3DDevice9Ex * ptr, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX * pFullscreenDisplayMode)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::ResetEx");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].ResetEx(ptr, pPresentationParameters, pFullscreenDisplayMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Ex_GetDisplayModeEx_Hooked(IDirect3DDevice9Ex * ptr, UINT iSwapChain, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Ex::GetDisplayModeEx");
    return g_D3D9OriginVTables._IDirect3DDevice9Ex.tables[INDEX].GetDisplayModeEx(ptr, iSwapChain, pMode, pRotation);
}

// -----------------------------------------------------------------------------
// IDirect3DSwapChain9Ex
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_QueryInterface_Hooked(IDirect3DSwapChain9Ex * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DSwapChain9Ex_AddRef_Hooked(IDirect3DSwapChain9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::AddRef");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DSwapChain9Ex_Release_Hooked(IDirect3DSwapChain9Ex * ptr)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::Release");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_Present_Hooked(IDirect3DSwapChain9Ex * ptr, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::Present");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].Present(ptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetFrontBufferData_Hooked(IDirect3DSwapChain9Ex * ptr, IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetFrontBufferData");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetFrontBufferData(ptr, pDestSurface);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetBackBuffer_Hooked(IDirect3DSwapChain9Ex * ptr, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetBackBuffer");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetBackBuffer(ptr, iBackBuffer, Type, ppBackBuffer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetRasterStatus_Hooked(IDirect3DSwapChain9Ex * ptr, D3DRASTER_STATUS* pRasterStatus)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetRasterStatus");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetRasterStatus(ptr, pRasterStatus);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetDisplayMode_Hooked(IDirect3DSwapChain9Ex * ptr, D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetDisplayMode");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDisplayMode(ptr, pMode);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetDevice_Hooked(IDirect3DSwapChain9Ex * ptr, IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetDevice");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDevice(ptr, ppDevice);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetPresentParameters_Hooked(IDirect3DSwapChain9Ex * ptr, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetPresentParameters");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetPresentParameters(ptr, pPresentationParameters);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetLastPresentCount_Hooked(IDirect3DSwapChain9Ex * ptr, UINT* pLastPresentCount)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetLastPresentCount");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetLastPresentCount(ptr, pLastPresentCount);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetPresentStats_Hooked(IDirect3DSwapChain9Ex * ptr, D3DPRESENTSTATS* pPresentationStatistics)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetPresentStats");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetPresentStats(ptr, pPresentationStatistics);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DSwapChain9Ex_GetDisplayModeEx_Hooked(IDirect3DSwapChain9Ex * ptr, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation)
{
    calltrace::AutoTrace trace("IDirect3DSwapChain9Ex::GetDisplayModeEx");
    return g_D3D9OriginVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDisplayModeEx(ptr, pMode, pRotation);
}

// -----------------------------------------------------------------------------
// IDirect3D9ExOverlayExtension
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9ExOverlayExtension_QueryInterface_Hooked(IDirect3D9ExOverlayExtension * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3D9ExOverlayExtension::QueryInterface");
    return g_D3D9OriginVTables._IDirect3D9ExOverlayExtension.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3D9ExOverlayExtension_AddRef_Hooked(IDirect3D9ExOverlayExtension * ptr)
{
    calltrace::AutoTrace trace("IDirect3D9ExOverlayExtension::AddRef");
    return g_D3D9OriginVTables._IDirect3D9ExOverlayExtension.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3D9ExOverlayExtension_Release_Hooked(IDirect3D9ExOverlayExtension * ptr)
{
    calltrace::AutoTrace trace("IDirect3D9ExOverlayExtension::Release");
    return g_D3D9OriginVTables._IDirect3D9ExOverlayExtension.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3D9ExOverlayExtension_CheckDeviceOverlayType_Hooked(IDirect3D9ExOverlayExtension * ptr, UINT Adapter, D3DDEVTYPE DevType, UINT OverlayWidth, UINT OverlayHeight, D3DFORMAT OverlayFormat, D3DDISPLAYMODEEX* pDisplayMode, D3DDISPLAYROTATION DisplayRotation, D3DOVERLAYCAPS* pOverlayCaps)
{
    calltrace::AutoTrace trace("IDirect3D9ExOverlayExtension::CheckDeviceOverlayType");
    return g_D3D9OriginVTables._IDirect3D9ExOverlayExtension.tables[INDEX].CheckDeviceOverlayType(ptr, Adapter, DevType, OverlayWidth, OverlayHeight, OverlayFormat, pDisplayMode, DisplayRotation, pOverlayCaps);
}

// -----------------------------------------------------------------------------
// IDirect3DDevice9Video
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Video_QueryInterface_Hooked(IDirect3DDevice9Video * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Video::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DDevice9Video_AddRef_Hooked(IDirect3DDevice9Video * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Video::AddRef");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DDevice9Video_Release_Hooked(IDirect3DDevice9Video * ptr)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Video::Release");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Video_GetContentProtectionCaps_Hooked(IDirect3DDevice9Video * ptr, CONST GUID* pCryptoType, CONST GUID* pDecodeProfile, D3DCONTENTPROTECTIONCAPS* pCaps)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Video::GetContentProtectionCaps");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].GetContentProtectionCaps(ptr, pCryptoType, pDecodeProfile, pCaps);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Video_CreateAuthenticatedChannel_Hooked(IDirect3DDevice9Video * ptr, D3DAUTHENTICATEDCHANNELTYPE ChannelType, IDirect3DAuthenticatedChannel9** ppAuthenticatedChannel, HANDLE* pChannelHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Video::CreateAuthenticatedChannel");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].CreateAuthenticatedChannel(ptr, ChannelType, ppAuthenticatedChannel, pChannelHandle);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DDevice9Video_CreateCryptoSession_Hooked(IDirect3DDevice9Video * ptr, CONST GUID* pCryptoType, CONST GUID* pDecodeProfile, IDirect3DCryptoSession9** ppCryptoSession, HANDLE* pCryptoHandle)
{
    calltrace::AutoTrace trace("IDirect3DDevice9Video::CreateCryptoSession");
    return g_D3D9OriginVTables._IDirect3DDevice9Video.tables[INDEX].CreateCryptoSession(ptr, pCryptoType, pDecodeProfile, ppCryptoSession, pCryptoHandle);
}

// -----------------------------------------------------------------------------
// IDirect3DAuthenticatedChannel9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_QueryInterface_Hooked(IDirect3DAuthenticatedChannel9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DAuthenticatedChannel9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_AddRef_Hooked(IDirect3DAuthenticatedChannel9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DAuthenticatedChannel9::AddRef");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_Release_Hooked(IDirect3DAuthenticatedChannel9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DAuthenticatedChannel9::Release");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_GetCertificateSize_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT* pCertificateSize)
{
    calltrace::AutoTrace trace("IDirect3DAuthenticatedChannel9::GetCertificateSize");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].GetCertificateSize(ptr, pCertificateSize);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_GetCertificate_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT CertifacteSize, BYTE* ppCertificate)
{
    calltrace::AutoTrace trace("IDirect3DAuthenticatedChannel9::GetCertificate");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].GetCertificate(ptr, CertifacteSize, ppCertificate);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_NegotiateKeyExchange_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT DataSize, VOID* pData)
{
    calltrace::AutoTrace trace("IDirect3DAuthenticatedChannel9::NegotiateKeyExchange");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].NegotiateKeyExchange(ptr, DataSize, pData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_Query_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT InputSize, CONST VOID* pInput, UINT OutputSize, VOID* pOutput)
{
    calltrace::AutoTrace trace("IDirect3DAuthenticatedChannel9::Query");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Query(ptr, InputSize, pInput, OutputSize, pOutput);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DAuthenticatedChannel9_Configure_Hooked(IDirect3DAuthenticatedChannel9 * ptr, UINT InputSize, CONST VOID* pInput, D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT* pOutput)
{
    calltrace::AutoTrace trace("IDirect3DAuthenticatedChannel9::Configure");
    return g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Configure(ptr, InputSize, pInput, pOutput);
}

// -----------------------------------------------------------------------------
// IDirect3DCryptoSession9
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_QueryInterface_Hooked(IDirect3DCryptoSession9 * ptr, const IID & iid, void ** pp)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::QueryInterface");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DCryptoSession9_AddRef_Hooked(IDirect3DCryptoSession9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::AddRef");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static ULONG STDMETHODCALLTYPE IDirect3DCryptoSession9_Release_Hooked(IDirect3DCryptoSession9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::Release");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_GetCertificateSize_Hooked(IDirect3DCryptoSession9 * ptr, UINT* pCertificateSize)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::GetCertificateSize");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].GetCertificateSize(ptr, pCertificateSize);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_GetCertificate_Hooked(IDirect3DCryptoSession9 * ptr, UINT CertifacteSize, BYTE* ppCertificate)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::GetCertificate");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].GetCertificate(ptr, CertifacteSize, ppCertificate);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_NegotiateKeyExchange_Hooked(IDirect3DCryptoSession9 * ptr, UINT DataSize, VOID* pData)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::NegotiateKeyExchange");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].NegotiateKeyExchange(ptr, DataSize, pData);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_EncryptionBlt_Hooked(IDirect3DCryptoSession9 * ptr, IDirect3DSurface9* pSrcSurface, IDirect3DSurface9* pDstSurface, UINT DstSurfaceSize, VOID* pIV)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::EncryptionBlt");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].EncryptionBlt(ptr, pSrcSurface, pDstSurface, DstSurfaceSize, pIV);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_DecryptionBlt_Hooked(IDirect3DCryptoSession9 * ptr, IDirect3DSurface9* pSrcSurface, IDirect3DSurface9* pDstSurface, UINT SrcSurfaceSize, D3DENCRYPTED_BLOCK_INFO* pEncryptedBlockInfo, VOID* pContentKey, VOID* pIV)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::DecryptionBlt");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].DecryptionBlt(ptr, pSrcSurface, pDstSurface, SrcSurfaceSize, pEncryptedBlockInfo, pContentKey, pIV);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_GetSurfacePitch_Hooked(IDirect3DCryptoSession9 * ptr, IDirect3DSurface9* pSrcSurface, UINT* pSurfacePitch)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::GetSurfacePitch");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].GetSurfacePitch(ptr, pSrcSurface, pSurfacePitch);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_StartSessionKeyRefresh_Hooked(IDirect3DCryptoSession9 * ptr, VOID* pRandomNumber, UINT RandomNumberSize)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::StartSessionKeyRefresh");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].StartSessionKeyRefresh(ptr, pRandomNumber, RandomNumberSize);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_FinishSessionKeyRefresh_Hooked(IDirect3DCryptoSession9 * ptr)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::FinishSessionKeyRefresh");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].FinishSessionKeyRefresh(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static HRESULT STDMETHODCALLTYPE IDirect3DCryptoSession9_GetEncryptionBltKey_Hooked(IDirect3DCryptoSession9 * ptr, VOID* pReadbackKey, UINT KeySize)
{
    calltrace::AutoTrace trace("IDirect3DCryptoSession9::GetEncryptionBltKey");
    return g_D3D9OriginVTables._IDirect3DCryptoSession9.tables[INDEX].GetEncryptionBltKey(ptr, pReadbackKey, KeySize);
}

// -----------------------------------------------------------------------------
void RealToHooked9(const IID & iid, void * p)
{
    if (false) {}
    else if (__uuidof(IDirect3D9) == iid) RealToHooked9_IDirect3D9((IDirect3D9*)p);
    else if (__uuidof(IDirect3DDevice9) == iid) RealToHooked9_IDirect3DDevice9((IDirect3DDevice9*)p);
    else if (__uuidof(IDirect3DStateBlock9) == iid) RealToHooked9_IDirect3DStateBlock9((IDirect3DStateBlock9*)p);
    else if (__uuidof(IDirect3DSwapChain9) == iid) RealToHooked9_IDirect3DSwapChain9((IDirect3DSwapChain9*)p);
    else if (__uuidof(IDirect3DResource9) == iid) RealToHooked9_IDirect3DResource9((IDirect3DResource9*)p);
    else if (__uuidof(IDirect3DVertexDeclaration9) == iid) RealToHooked9_IDirect3DVertexDeclaration9((IDirect3DVertexDeclaration9*)p);
    else if (__uuidof(IDirect3DVertexShader9) == iid) RealToHooked9_IDirect3DVertexShader9((IDirect3DVertexShader9*)p);
    else if (__uuidof(IDirect3DPixelShader9) == iid) RealToHooked9_IDirect3DPixelShader9((IDirect3DPixelShader9*)p);
    else if (__uuidof(IDirect3DBaseTexture9) == iid) RealToHooked9_IDirect3DBaseTexture9((IDirect3DBaseTexture9*)p);
    else if (__uuidof(IDirect3DTexture9) == iid) RealToHooked9_IDirect3DTexture9((IDirect3DTexture9*)p);
    else if (__uuidof(IDirect3DVolumeTexture9) == iid) RealToHooked9_IDirect3DVolumeTexture9((IDirect3DVolumeTexture9*)p);
    else if (__uuidof(IDirect3DCubeTexture9) == iid) RealToHooked9_IDirect3DCubeTexture9((IDirect3DCubeTexture9*)p);
    else if (__uuidof(IDirect3DVertexBuffer9) == iid) RealToHooked9_IDirect3DVertexBuffer9((IDirect3DVertexBuffer9*)p);
    else if (__uuidof(IDirect3DIndexBuffer9) == iid) RealToHooked9_IDirect3DIndexBuffer9((IDirect3DIndexBuffer9*)p);
    else if (__uuidof(IDirect3DSurface9) == iid) RealToHooked9_IDirect3DSurface9((IDirect3DSurface9*)p);
    else if (__uuidof(IDirect3DVolume9) == iid) RealToHooked9_IDirect3DVolume9((IDirect3DVolume9*)p);
    else if (__uuidof(IDirect3DQuery9) == iid) RealToHooked9_IDirect3DQuery9((IDirect3DQuery9*)p);
    else if (__uuidof(IDirect3D9Ex) == iid) RealToHooked9_IDirect3D9Ex((IDirect3D9Ex*)p);
    else if (__uuidof(IDirect3DDevice9Ex) == iid) RealToHooked9_IDirect3DDevice9Ex((IDirect3DDevice9Ex*)p);
    else if (__uuidof(IDirect3DSwapChain9Ex) == iid) RealToHooked9_IDirect3DSwapChain9Ex((IDirect3DSwapChain9Ex*)p);
    else if (__uuidof(IDirect3D9ExOverlayExtension) == iid) RealToHooked9_IDirect3D9ExOverlayExtension((IDirect3D9ExOverlayExtension*)p);
    else if (__uuidof(IDirect3DDevice9Video) == iid) RealToHooked9_IDirect3DDevice9Video((IDirect3DDevice9Video*)p);
    else if (__uuidof(IDirect3DAuthenticatedChannel9) == iid) RealToHooked9_IDirect3DAuthenticatedChannel9((IDirect3DAuthenticatedChannel9*)p);
    else if (__uuidof(IDirect3DCryptoSession9) == iid) RealToHooked9_IDirect3DCryptoSession9((IDirect3DCryptoSession9*)p);
    else
    {
        HOOK_WARN_LOG("unrecognized interface UUID: <xxxx-xxxx-xxxxx...>");
    }
}

// -----------------------------------------------------------------------------
template<UINT INDEX> static void SetupD3D9HookedVTables()
{
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].QueryInterface = IDirect3D9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].AddRef = IDirect3D9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].Release = IDirect3D9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].RegisterSoftwareDevice = IDirect3D9_RegisterSoftwareDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterCount = IDirect3D9_GetAdapterCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterIdentifier = IDirect3D9_GetAdapterIdentifier_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterModeCount = IDirect3D9_GetAdapterModeCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].EnumAdapterModes = IDirect3D9_EnumAdapterModes_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterDisplayMode = IDirect3D9_GetAdapterDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDeviceType = IDirect3D9_CheckDeviceType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDeviceFormat = IDirect3D9_CheckDeviceFormat_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDeviceMultiSampleType = IDirect3D9_CheckDeviceMultiSampleType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDepthStencilMatch = IDirect3D9_CheckDepthStencilMatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CheckDeviceFormatConversion = IDirect3D9_CheckDeviceFormatConversion_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetDeviceCaps = IDirect3D9_GetDeviceCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].GetAdapterMonitor = IDirect3D9_GetAdapterMonitor_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9.tables[INDEX].CreateDevice = IDirect3D9_CreateDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].QueryInterface = IDirect3DDevice9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].AddRef = IDirect3DDevice9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].Release = IDirect3DDevice9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].TestCooperativeLevel = IDirect3DDevice9_TestCooperativeLevel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetAvailableTextureMem = IDirect3DDevice9_GetAvailableTextureMem_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].EvictManagedResources = IDirect3DDevice9_EvictManagedResources_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetDirect3D = IDirect3DDevice9_GetDirect3D_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetDeviceCaps = IDirect3DDevice9_GetDeviceCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetDisplayMode = IDirect3DDevice9_GetDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetCreationParameters = IDirect3DDevice9_GetCreationParameters_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetCursorProperties = IDirect3DDevice9_SetCursorProperties_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetCursorPosition = IDirect3DDevice9_SetCursorPosition_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].ShowCursor = IDirect3DDevice9_ShowCursor_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateAdditionalSwapChain = IDirect3DDevice9_CreateAdditionalSwapChain_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetSwapChain = IDirect3DDevice9_GetSwapChain_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetNumberOfSwapChains = IDirect3DDevice9_GetNumberOfSwapChains_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].Reset = IDirect3DDevice9_Reset_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].Present = IDirect3DDevice9_Present_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetBackBuffer = IDirect3DDevice9_GetBackBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetRasterStatus = IDirect3DDevice9_GetRasterStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetDialogBoxMode = IDirect3DDevice9_SetDialogBoxMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetGammaRamp = IDirect3DDevice9_SetGammaRamp_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetGammaRamp = IDirect3DDevice9_GetGammaRamp_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateTexture = IDirect3DDevice9_CreateTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateVolumeTexture = IDirect3DDevice9_CreateVolumeTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateCubeTexture = IDirect3DDevice9_CreateCubeTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateVertexBuffer = IDirect3DDevice9_CreateVertexBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateIndexBuffer = IDirect3DDevice9_CreateIndexBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateRenderTarget = IDirect3DDevice9_CreateRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateDepthStencilSurface = IDirect3DDevice9_CreateDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].UpdateSurface = IDirect3DDevice9_UpdateSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].UpdateTexture = IDirect3DDevice9_UpdateTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetRenderTargetData = IDirect3DDevice9_GetRenderTargetData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetFrontBufferData = IDirect3DDevice9_GetFrontBufferData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].StretchRect = IDirect3DDevice9_StretchRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].ColorFill = IDirect3DDevice9_ColorFill_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateOffscreenPlainSurface = IDirect3DDevice9_CreateOffscreenPlainSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetRenderTarget = IDirect3DDevice9_SetRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetRenderTarget = IDirect3DDevice9_GetRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetDepthStencilSurface = IDirect3DDevice9_SetDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetDepthStencilSurface = IDirect3DDevice9_GetDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].BeginScene = IDirect3DDevice9_BeginScene_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].EndScene = IDirect3DDevice9_EndScene_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].Clear = IDirect3DDevice9_Clear_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetTransform = IDirect3DDevice9_SetTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetTransform = IDirect3DDevice9_GetTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].MultiplyTransform = IDirect3DDevice9_MultiplyTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetViewport = IDirect3DDevice9_SetViewport_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetViewport = IDirect3DDevice9_GetViewport_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetMaterial = IDirect3DDevice9_SetMaterial_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetMaterial = IDirect3DDevice9_GetMaterial_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetLight = IDirect3DDevice9_SetLight_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetLight = IDirect3DDevice9_GetLight_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].LightEnable = IDirect3DDevice9_LightEnable_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetLightEnable = IDirect3DDevice9_GetLightEnable_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetClipPlane = IDirect3DDevice9_SetClipPlane_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetClipPlane = IDirect3DDevice9_GetClipPlane_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetRenderState = IDirect3DDevice9_SetRenderState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetRenderState = IDirect3DDevice9_GetRenderState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateStateBlock = IDirect3DDevice9_CreateStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].BeginStateBlock = IDirect3DDevice9_BeginStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].EndStateBlock = IDirect3DDevice9_EndStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetClipStatus = IDirect3DDevice9_SetClipStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetClipStatus = IDirect3DDevice9_GetClipStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetTexture = IDirect3DDevice9_GetTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetTexture = IDirect3DDevice9_SetTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetTextureStageState = IDirect3DDevice9_GetTextureStageState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetTextureStageState = IDirect3DDevice9_SetTextureStageState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetSamplerState = IDirect3DDevice9_GetSamplerState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetSamplerState = IDirect3DDevice9_SetSamplerState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].ValidateDevice = IDirect3DDevice9_ValidateDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPaletteEntries = IDirect3DDevice9_SetPaletteEntries_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPaletteEntries = IDirect3DDevice9_GetPaletteEntries_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetCurrentTexturePalette = IDirect3DDevice9_SetCurrentTexturePalette_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetCurrentTexturePalette = IDirect3DDevice9_GetCurrentTexturePalette_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetScissorRect = IDirect3DDevice9_SetScissorRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetScissorRect = IDirect3DDevice9_GetScissorRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetSoftwareVertexProcessing = IDirect3DDevice9_SetSoftwareVertexProcessing_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetSoftwareVertexProcessing = IDirect3DDevice9_GetSoftwareVertexProcessing_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetNPatchMode = IDirect3DDevice9_SetNPatchMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetNPatchMode = IDirect3DDevice9_GetNPatchMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawPrimitive = IDirect3DDevice9_DrawPrimitive_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawIndexedPrimitive = IDirect3DDevice9_DrawIndexedPrimitive_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawPrimitiveUP = IDirect3DDevice9_DrawPrimitiveUP_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawIndexedPrimitiveUP = IDirect3DDevice9_DrawIndexedPrimitiveUP_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].ProcessVertices = IDirect3DDevice9_ProcessVertices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateVertexDeclaration = IDirect3DDevice9_CreateVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexDeclaration = IDirect3DDevice9_SetVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexDeclaration = IDirect3DDevice9_GetVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetFVF = IDirect3DDevice9_SetFVF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetFVF = IDirect3DDevice9_GetFVF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateVertexShader = IDirect3DDevice9_CreateVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexShader = IDirect3DDevice9_SetVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexShader = IDirect3DDevice9_GetVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantF = IDirect3DDevice9_SetVertexShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantF = IDirect3DDevice9_GetVertexShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantI = IDirect3DDevice9_SetVertexShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantI = IDirect3DDevice9_GetVertexShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetVertexShaderConstantB = IDirect3DDevice9_SetVertexShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetVertexShaderConstantB = IDirect3DDevice9_GetVertexShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetStreamSource = IDirect3DDevice9_SetStreamSource_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetStreamSource = IDirect3DDevice9_GetStreamSource_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetStreamSourceFreq = IDirect3DDevice9_SetStreamSourceFreq_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetStreamSourceFreq = IDirect3DDevice9_GetStreamSourceFreq_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetIndices = IDirect3DDevice9_SetIndices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetIndices = IDirect3DDevice9_GetIndices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreatePixelShader = IDirect3DDevice9_CreatePixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPixelShader = IDirect3DDevice9_SetPixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPixelShader = IDirect3DDevice9_GetPixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantF = IDirect3DDevice9_SetPixelShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantF = IDirect3DDevice9_GetPixelShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantI = IDirect3DDevice9_SetPixelShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantI = IDirect3DDevice9_GetPixelShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].SetPixelShaderConstantB = IDirect3DDevice9_SetPixelShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].GetPixelShaderConstantB = IDirect3DDevice9_GetPixelShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawRectPatch = IDirect3DDevice9_DrawRectPatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DrawTriPatch = IDirect3DDevice9_DrawTriPatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].DeletePatch = IDirect3DDevice9_DeletePatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9.tables[INDEX].CreateQuery = IDirect3DDevice9_CreateQuery_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].QueryInterface = IDirect3DStateBlock9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].AddRef = IDirect3DStateBlock9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].Release = IDirect3DStateBlock9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].GetDevice = IDirect3DStateBlock9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].Capture = IDirect3DStateBlock9_Capture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DStateBlock9.tables[INDEX].Apply = IDirect3DStateBlock9_Apply_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].QueryInterface = IDirect3DSwapChain9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].AddRef = IDirect3DSwapChain9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].Release = IDirect3DSwapChain9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].Present = IDirect3DSwapChain9_Present_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetFrontBufferData = IDirect3DSwapChain9_GetFrontBufferData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetBackBuffer = IDirect3DSwapChain9_GetBackBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetRasterStatus = IDirect3DSwapChain9_GetRasterStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetDisplayMode = IDirect3DSwapChain9_GetDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetDevice = IDirect3DSwapChain9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9.tables[INDEX].GetPresentParameters = IDirect3DSwapChain9_GetPresentParameters_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].QueryInterface = IDirect3DResource9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].AddRef = IDirect3DResource9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].Release = IDirect3DResource9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].GetDevice = IDirect3DResource9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].SetPrivateData = IDirect3DResource9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].GetPrivateData = IDirect3DResource9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].FreePrivateData = IDirect3DResource9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].SetPriority = IDirect3DResource9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].GetPriority = IDirect3DResource9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].PreLoad = IDirect3DResource9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DResource9.tables[INDEX].GetType = IDirect3DResource9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].QueryInterface = IDirect3DVertexDeclaration9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].AddRef = IDirect3DVertexDeclaration9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].Release = IDirect3DVertexDeclaration9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].GetDevice = IDirect3DVertexDeclaration9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexDeclaration9.tables[INDEX].GetDeclaration = IDirect3DVertexDeclaration9_GetDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].QueryInterface = IDirect3DVertexShader9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].AddRef = IDirect3DVertexShader9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].Release = IDirect3DVertexShader9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].GetDevice = IDirect3DVertexShader9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexShader9.tables[INDEX].GetFunction = IDirect3DVertexShader9_GetFunction_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].QueryInterface = IDirect3DPixelShader9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].AddRef = IDirect3DPixelShader9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].Release = IDirect3DPixelShader9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].GetDevice = IDirect3DPixelShader9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DPixelShader9.tables[INDEX].GetFunction = IDirect3DPixelShader9_GetFunction_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].QueryInterface = IDirect3DBaseTexture9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].AddRef = IDirect3DBaseTexture9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].Release = IDirect3DBaseTexture9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetDevice = IDirect3DBaseTexture9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].SetPrivateData = IDirect3DBaseTexture9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetPrivateData = IDirect3DBaseTexture9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].FreePrivateData = IDirect3DBaseTexture9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].SetPriority = IDirect3DBaseTexture9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetPriority = IDirect3DBaseTexture9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].PreLoad = IDirect3DBaseTexture9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetType = IDirect3DBaseTexture9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].SetLOD = IDirect3DBaseTexture9_SetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetLOD = IDirect3DBaseTexture9_GetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetLevelCount = IDirect3DBaseTexture9_GetLevelCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].SetAutoGenFilterType = IDirect3DBaseTexture9_SetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GetAutoGenFilterType = IDirect3DBaseTexture9_GetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DBaseTexture9.tables[INDEX].GenerateMipSubLevels = IDirect3DBaseTexture9_GenerateMipSubLevels_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].QueryInterface = IDirect3DTexture9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].AddRef = IDirect3DTexture9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].Release = IDirect3DTexture9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetDevice = IDirect3DTexture9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].SetPrivateData = IDirect3DTexture9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetPrivateData = IDirect3DTexture9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].FreePrivateData = IDirect3DTexture9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].SetPriority = IDirect3DTexture9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetPriority = IDirect3DTexture9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].PreLoad = IDirect3DTexture9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetType = IDirect3DTexture9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].SetLOD = IDirect3DTexture9_SetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetLOD = IDirect3DTexture9_GetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetLevelCount = IDirect3DTexture9_GetLevelCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].SetAutoGenFilterType = IDirect3DTexture9_SetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetAutoGenFilterType = IDirect3DTexture9_GetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GenerateMipSubLevels = IDirect3DTexture9_GenerateMipSubLevels_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetLevelDesc = IDirect3DTexture9_GetLevelDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].GetSurfaceLevel = IDirect3DTexture9_GetSurfaceLevel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].LockRect = IDirect3DTexture9_LockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].UnlockRect = IDirect3DTexture9_UnlockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DTexture9.tables[INDEX].AddDirtyRect = IDirect3DTexture9_AddDirtyRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].QueryInterface = IDirect3DVolumeTexture9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].AddRef = IDirect3DVolumeTexture9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].Release = IDirect3DVolumeTexture9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetDevice = IDirect3DVolumeTexture9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].SetPrivateData = IDirect3DVolumeTexture9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetPrivateData = IDirect3DVolumeTexture9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].FreePrivateData = IDirect3DVolumeTexture9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].SetPriority = IDirect3DVolumeTexture9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetPriority = IDirect3DVolumeTexture9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].PreLoad = IDirect3DVolumeTexture9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetType = IDirect3DVolumeTexture9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].SetLOD = IDirect3DVolumeTexture9_SetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLOD = IDirect3DVolumeTexture9_GetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLevelCount = IDirect3DVolumeTexture9_GetLevelCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].SetAutoGenFilterType = IDirect3DVolumeTexture9_SetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetAutoGenFilterType = IDirect3DVolumeTexture9_GetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GenerateMipSubLevels = IDirect3DVolumeTexture9_GenerateMipSubLevels_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetLevelDesc = IDirect3DVolumeTexture9_GetLevelDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].GetVolumeLevel = IDirect3DVolumeTexture9_GetVolumeLevel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].LockBox = IDirect3DVolumeTexture9_LockBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].UnlockBox = IDirect3DVolumeTexture9_UnlockBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolumeTexture9.tables[INDEX].AddDirtyBox = IDirect3DVolumeTexture9_AddDirtyBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].QueryInterface = IDirect3DCubeTexture9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].AddRef = IDirect3DCubeTexture9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].Release = IDirect3DCubeTexture9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetDevice = IDirect3DCubeTexture9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].SetPrivateData = IDirect3DCubeTexture9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetPrivateData = IDirect3DCubeTexture9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].FreePrivateData = IDirect3DCubeTexture9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].SetPriority = IDirect3DCubeTexture9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetPriority = IDirect3DCubeTexture9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].PreLoad = IDirect3DCubeTexture9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetType = IDirect3DCubeTexture9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].SetLOD = IDirect3DCubeTexture9_SetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetLOD = IDirect3DCubeTexture9_GetLOD_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetLevelCount = IDirect3DCubeTexture9_GetLevelCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].SetAutoGenFilterType = IDirect3DCubeTexture9_SetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetAutoGenFilterType = IDirect3DCubeTexture9_GetAutoGenFilterType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GenerateMipSubLevels = IDirect3DCubeTexture9_GenerateMipSubLevels_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetLevelDesc = IDirect3DCubeTexture9_GetLevelDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].GetCubeMapSurface = IDirect3DCubeTexture9_GetCubeMapSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].LockRect = IDirect3DCubeTexture9_LockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].UnlockRect = IDirect3DCubeTexture9_UnlockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCubeTexture9.tables[INDEX].AddDirtyRect = IDirect3DCubeTexture9_AddDirtyRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].QueryInterface = IDirect3DVertexBuffer9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].AddRef = IDirect3DVertexBuffer9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].Release = IDirect3DVertexBuffer9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetDevice = IDirect3DVertexBuffer9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].SetPrivateData = IDirect3DVertexBuffer9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetPrivateData = IDirect3DVertexBuffer9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].FreePrivateData = IDirect3DVertexBuffer9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].SetPriority = IDirect3DVertexBuffer9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetPriority = IDirect3DVertexBuffer9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].PreLoad = IDirect3DVertexBuffer9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetType = IDirect3DVertexBuffer9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].Lock = IDirect3DVertexBuffer9_Lock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].Unlock = IDirect3DVertexBuffer9_Unlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVertexBuffer9.tables[INDEX].GetDesc = IDirect3DVertexBuffer9_GetDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].QueryInterface = IDirect3DIndexBuffer9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].AddRef = IDirect3DIndexBuffer9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].Release = IDirect3DIndexBuffer9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetDevice = IDirect3DIndexBuffer9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].SetPrivateData = IDirect3DIndexBuffer9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetPrivateData = IDirect3DIndexBuffer9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].FreePrivateData = IDirect3DIndexBuffer9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].SetPriority = IDirect3DIndexBuffer9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetPriority = IDirect3DIndexBuffer9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].PreLoad = IDirect3DIndexBuffer9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetType = IDirect3DIndexBuffer9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].Lock = IDirect3DIndexBuffer9_Lock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].Unlock = IDirect3DIndexBuffer9_Unlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DIndexBuffer9.tables[INDEX].GetDesc = IDirect3DIndexBuffer9_GetDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].QueryInterface = IDirect3DSurface9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].AddRef = IDirect3DSurface9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].Release = IDirect3DSurface9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetDevice = IDirect3DSurface9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].SetPrivateData = IDirect3DSurface9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetPrivateData = IDirect3DSurface9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].FreePrivateData = IDirect3DSurface9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].SetPriority = IDirect3DSurface9_SetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetPriority = IDirect3DSurface9_GetPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].PreLoad = IDirect3DSurface9_PreLoad_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetType = IDirect3DSurface9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetContainer = IDirect3DSurface9_GetContainer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetDesc = IDirect3DSurface9_GetDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].LockRect = IDirect3DSurface9_LockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].UnlockRect = IDirect3DSurface9_UnlockRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].GetDC = IDirect3DSurface9_GetDC_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSurface9.tables[INDEX].ReleaseDC = IDirect3DSurface9_ReleaseDC_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].QueryInterface = IDirect3DVolume9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].AddRef = IDirect3DVolume9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].Release = IDirect3DVolume9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].GetDevice = IDirect3DVolume9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].SetPrivateData = IDirect3DVolume9_SetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].GetPrivateData = IDirect3DVolume9_GetPrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].FreePrivateData = IDirect3DVolume9_FreePrivateData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].GetContainer = IDirect3DVolume9_GetContainer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].GetDesc = IDirect3DVolume9_GetDesc_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].LockBox = IDirect3DVolume9_LockBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DVolume9.tables[INDEX].UnlockBox = IDirect3DVolume9_UnlockBox_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].QueryInterface = IDirect3DQuery9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].AddRef = IDirect3DQuery9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].Release = IDirect3DQuery9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].GetDevice = IDirect3DQuery9_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].GetType = IDirect3DQuery9_GetType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].GetDataSize = IDirect3DQuery9_GetDataSize_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].Issue = IDirect3DQuery9_Issue_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DQuery9.tables[INDEX].GetData = IDirect3DQuery9_GetData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].QueryInterface = IDirect3D9Ex_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].AddRef = IDirect3D9Ex_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].Release = IDirect3D9Ex_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterCount = IDirect3D9Ex_GetAdapterCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterIdentifier = IDirect3D9Ex_GetAdapterIdentifier_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterModeCount = IDirect3D9Ex_GetAdapterModeCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].EnumAdapterModes = IDirect3D9Ex_EnumAdapterModes_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterDisplayMode = IDirect3D9Ex_GetAdapterDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceType = IDirect3D9Ex_CheckDeviceType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceFormat = IDirect3D9Ex_CheckDeviceFormat_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceMultiSampleType = IDirect3D9Ex_CheckDeviceMultiSampleType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDepthStencilMatch = IDirect3D9Ex_CheckDepthStencilMatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CheckDeviceFormatConversion = IDirect3D9Ex_CheckDeviceFormatConversion_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetDeviceCaps = IDirect3D9Ex_GetDeviceCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterMonitor = IDirect3D9Ex_GetAdapterMonitor_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CreateDevice = IDirect3D9Ex_CreateDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterModeCountEx = IDirect3D9Ex_GetAdapterModeCountEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].EnumAdapterModesEx = IDirect3D9Ex_EnumAdapterModesEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterDisplayModeEx = IDirect3D9Ex_GetAdapterDisplayModeEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].CreateDeviceEx = IDirect3D9Ex_CreateDeviceEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9Ex.tables[INDEX].GetAdapterLUID = IDirect3D9Ex_GetAdapterLUID_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].QueryInterface = IDirect3DDevice9Ex_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].AddRef = IDirect3DDevice9Ex_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].Release = IDirect3DDevice9Ex_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].TestCooperativeLevel = IDirect3DDevice9Ex_TestCooperativeLevel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetAvailableTextureMem = IDirect3DDevice9Ex_GetAvailableTextureMem_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].EvictManagedResources = IDirect3DDevice9Ex_EvictManagedResources_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDirect3D = IDirect3DDevice9Ex_GetDirect3D_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDeviceCaps = IDirect3DDevice9Ex_GetDeviceCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDisplayMode = IDirect3DDevice9Ex_GetDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetCreationParameters = IDirect3DDevice9Ex_GetCreationParameters_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetCursorProperties = IDirect3DDevice9Ex_SetCursorProperties_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetCursorPosition = IDirect3DDevice9Ex_SetCursorPosition_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ShowCursor = IDirect3DDevice9Ex_ShowCursor_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateAdditionalSwapChain = IDirect3DDevice9Ex_CreateAdditionalSwapChain_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetSwapChain = IDirect3DDevice9Ex_GetSwapChain_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetNumberOfSwapChains = IDirect3DDevice9Ex_GetNumberOfSwapChains_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].Reset = IDirect3DDevice9Ex_Reset_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].Present = IDirect3DDevice9Ex_Present_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetBackBuffer = IDirect3DDevice9Ex_GetBackBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetRasterStatus = IDirect3DDevice9Ex_GetRasterStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetDialogBoxMode = IDirect3DDevice9Ex_SetDialogBoxMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetGammaRamp = IDirect3DDevice9Ex_SetGammaRamp_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetGammaRamp = IDirect3DDevice9Ex_GetGammaRamp_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateTexture = IDirect3DDevice9Ex_CreateTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVolumeTexture = IDirect3DDevice9Ex_CreateVolumeTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateCubeTexture = IDirect3DDevice9Ex_CreateCubeTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexBuffer = IDirect3DDevice9Ex_CreateVertexBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateIndexBuffer = IDirect3DDevice9Ex_CreateIndexBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateRenderTarget = IDirect3DDevice9Ex_CreateRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateDepthStencilSurface = IDirect3DDevice9Ex_CreateDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].UpdateSurface = IDirect3DDevice9Ex_UpdateSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].UpdateTexture = IDirect3DDevice9Ex_UpdateTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderTargetData = IDirect3DDevice9Ex_GetRenderTargetData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetFrontBufferData = IDirect3DDevice9Ex_GetFrontBufferData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].StretchRect = IDirect3DDevice9Ex_StretchRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ColorFill = IDirect3DDevice9Ex_ColorFill_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateOffscreenPlainSurface = IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetRenderTarget = IDirect3DDevice9Ex_SetRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderTarget = IDirect3DDevice9Ex_GetRenderTarget_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetDepthStencilSurface = IDirect3DDevice9Ex_SetDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDepthStencilSurface = IDirect3DDevice9Ex_GetDepthStencilSurface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].BeginScene = IDirect3DDevice9Ex_BeginScene_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].EndScene = IDirect3DDevice9Ex_EndScene_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].Clear = IDirect3DDevice9Ex_Clear_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetTransform = IDirect3DDevice9Ex_SetTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetTransform = IDirect3DDevice9Ex_GetTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].MultiplyTransform = IDirect3DDevice9Ex_MultiplyTransform_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetViewport = IDirect3DDevice9Ex_SetViewport_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetViewport = IDirect3DDevice9Ex_GetViewport_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetMaterial = IDirect3DDevice9Ex_SetMaterial_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetMaterial = IDirect3DDevice9Ex_GetMaterial_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetLight = IDirect3DDevice9Ex_SetLight_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetLight = IDirect3DDevice9Ex_GetLight_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].LightEnable = IDirect3DDevice9Ex_LightEnable_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetLightEnable = IDirect3DDevice9Ex_GetLightEnable_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetClipPlane = IDirect3DDevice9Ex_SetClipPlane_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetClipPlane = IDirect3DDevice9Ex_GetClipPlane_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetRenderState = IDirect3DDevice9Ex_SetRenderState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetRenderState = IDirect3DDevice9Ex_GetRenderState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateStateBlock = IDirect3DDevice9Ex_CreateStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].BeginStateBlock = IDirect3DDevice9Ex_BeginStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].EndStateBlock = IDirect3DDevice9Ex_EndStateBlock_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetClipStatus = IDirect3DDevice9Ex_SetClipStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetClipStatus = IDirect3DDevice9Ex_GetClipStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetTexture = IDirect3DDevice9Ex_GetTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetTexture = IDirect3DDevice9Ex_SetTexture_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetTextureStageState = IDirect3DDevice9Ex_GetTextureStageState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetTextureStageState = IDirect3DDevice9Ex_SetTextureStageState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetSamplerState = IDirect3DDevice9Ex_GetSamplerState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetSamplerState = IDirect3DDevice9Ex_SetSamplerState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ValidateDevice = IDirect3DDevice9Ex_ValidateDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPaletteEntries = IDirect3DDevice9Ex_SetPaletteEntries_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPaletteEntries = IDirect3DDevice9Ex_GetPaletteEntries_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetCurrentTexturePalette = IDirect3DDevice9Ex_SetCurrentTexturePalette_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetCurrentTexturePalette = IDirect3DDevice9Ex_GetCurrentTexturePalette_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetScissorRect = IDirect3DDevice9Ex_SetScissorRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetScissorRect = IDirect3DDevice9Ex_GetScissorRect_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetSoftwareVertexProcessing = IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetSoftwareVertexProcessing = IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetNPatchMode = IDirect3DDevice9Ex_SetNPatchMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetNPatchMode = IDirect3DDevice9Ex_GetNPatchMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawPrimitive = IDirect3DDevice9Ex_DrawPrimitive_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawIndexedPrimitive = IDirect3DDevice9Ex_DrawIndexedPrimitive_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawPrimitiveUP = IDirect3DDevice9Ex_DrawPrimitiveUP_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawIndexedPrimitiveUP = IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ProcessVertices = IDirect3DDevice9Ex_ProcessVertices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexDeclaration = IDirect3DDevice9Ex_CreateVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexDeclaration = IDirect3DDevice9Ex_SetVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexDeclaration = IDirect3DDevice9Ex_GetVertexDeclaration_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetFVF = IDirect3DDevice9Ex_SetFVF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetFVF = IDirect3DDevice9Ex_GetFVF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateVertexShader = IDirect3DDevice9Ex_CreateVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShader = IDirect3DDevice9Ex_SetVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShader = IDirect3DDevice9Ex_GetVertexShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantF = IDirect3DDevice9Ex_SetVertexShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantF = IDirect3DDevice9Ex_GetVertexShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantI = IDirect3DDevice9Ex_SetVertexShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantI = IDirect3DDevice9Ex_GetVertexShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetVertexShaderConstantB = IDirect3DDevice9Ex_SetVertexShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetVertexShaderConstantB = IDirect3DDevice9Ex_GetVertexShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetStreamSource = IDirect3DDevice9Ex_SetStreamSource_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetStreamSource = IDirect3DDevice9Ex_GetStreamSource_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetStreamSourceFreq = IDirect3DDevice9Ex_SetStreamSourceFreq_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetStreamSourceFreq = IDirect3DDevice9Ex_GetStreamSourceFreq_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetIndices = IDirect3DDevice9Ex_SetIndices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetIndices = IDirect3DDevice9Ex_GetIndices_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreatePixelShader = IDirect3DDevice9Ex_CreatePixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShader = IDirect3DDevice9Ex_SetPixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShader = IDirect3DDevice9Ex_GetPixelShader_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantF = IDirect3DDevice9Ex_SetPixelShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantF = IDirect3DDevice9Ex_GetPixelShaderConstantF_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantI = IDirect3DDevice9Ex_SetPixelShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantI = IDirect3DDevice9Ex_GetPixelShaderConstantI_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetPixelShaderConstantB = IDirect3DDevice9Ex_SetPixelShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetPixelShaderConstantB = IDirect3DDevice9Ex_GetPixelShaderConstantB_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawRectPatch = IDirect3DDevice9Ex_DrawRectPatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DrawTriPatch = IDirect3DDevice9Ex_DrawTriPatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].DeletePatch = IDirect3DDevice9Ex_DeletePatch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateQuery = IDirect3DDevice9Ex_CreateQuery_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetConvolutionMonoKernel = IDirect3DDevice9Ex_SetConvolutionMonoKernel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ComposeRects = IDirect3DDevice9Ex_ComposeRects_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].PresentEx = IDirect3DDevice9Ex_PresentEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetGPUThreadPriority = IDirect3DDevice9Ex_GetGPUThreadPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetGPUThreadPriority = IDirect3DDevice9Ex_SetGPUThreadPriority_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].WaitForVBlank = IDirect3DDevice9Ex_WaitForVBlank_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CheckResourceResidency = IDirect3DDevice9Ex_CheckResourceResidency_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].SetMaximumFrameLatency = IDirect3DDevice9Ex_SetMaximumFrameLatency_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetMaximumFrameLatency = IDirect3DDevice9Ex_GetMaximumFrameLatency_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CheckDeviceState = IDirect3DDevice9Ex_CheckDeviceState_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateRenderTargetEx = IDirect3DDevice9Ex_CreateRenderTargetEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateOffscreenPlainSurfaceEx = IDirect3DDevice9Ex_CreateOffscreenPlainSurfaceEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].CreateDepthStencilSurfaceEx = IDirect3DDevice9Ex_CreateDepthStencilSurfaceEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].ResetEx = IDirect3DDevice9Ex_ResetEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Ex.tables[INDEX].GetDisplayModeEx = IDirect3DDevice9Ex_GetDisplayModeEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].QueryInterface = IDirect3DSwapChain9Ex_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].AddRef = IDirect3DSwapChain9Ex_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].Release = IDirect3DSwapChain9Ex_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].Present = IDirect3DSwapChain9Ex_Present_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetFrontBufferData = IDirect3DSwapChain9Ex_GetFrontBufferData_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetBackBuffer = IDirect3DSwapChain9Ex_GetBackBuffer_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetRasterStatus = IDirect3DSwapChain9Ex_GetRasterStatus_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDisplayMode = IDirect3DSwapChain9Ex_GetDisplayMode_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDevice = IDirect3DSwapChain9Ex_GetDevice_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetPresentParameters = IDirect3DSwapChain9Ex_GetPresentParameters_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetLastPresentCount = IDirect3DSwapChain9Ex_GetLastPresentCount_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetPresentStats = IDirect3DSwapChain9Ex_GetPresentStats_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DSwapChain9Ex.tables[INDEX].GetDisplayModeEx = IDirect3DSwapChain9Ex_GetDisplayModeEx_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9ExOverlayExtension.tables[INDEX].QueryInterface = IDirect3D9ExOverlayExtension_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9ExOverlayExtension.tables[INDEX].AddRef = IDirect3D9ExOverlayExtension_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9ExOverlayExtension.tables[INDEX].Release = IDirect3D9ExOverlayExtension_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3D9ExOverlayExtension.tables[INDEX].CheckDeviceOverlayType = IDirect3D9ExOverlayExtension_CheckDeviceOverlayType_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].QueryInterface = IDirect3DDevice9Video_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].AddRef = IDirect3DDevice9Video_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].Release = IDirect3DDevice9Video_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].GetContentProtectionCaps = IDirect3DDevice9Video_GetContentProtectionCaps_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].CreateAuthenticatedChannel = IDirect3DDevice9Video_CreateAuthenticatedChannel_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DDevice9Video.tables[INDEX].CreateCryptoSession = IDirect3DDevice9Video_CreateCryptoSession_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].QueryInterface = IDirect3DAuthenticatedChannel9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].AddRef = IDirect3DAuthenticatedChannel9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Release = IDirect3DAuthenticatedChannel9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].GetCertificateSize = IDirect3DAuthenticatedChannel9_GetCertificateSize_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].GetCertificate = IDirect3DAuthenticatedChannel9_GetCertificate_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].NegotiateKeyExchange = IDirect3DAuthenticatedChannel9_NegotiateKeyExchange_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Query = IDirect3DAuthenticatedChannel9_Query_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9.tables[INDEX].Configure = IDirect3DAuthenticatedChannel9_Configure_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].QueryInterface = IDirect3DCryptoSession9_QueryInterface_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].AddRef = IDirect3DCryptoSession9_AddRef_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].Release = IDirect3DCryptoSession9_Release_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].GetCertificateSize = IDirect3DCryptoSession9_GetCertificateSize_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].GetCertificate = IDirect3DCryptoSession9_GetCertificate_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].NegotiateKeyExchange = IDirect3DCryptoSession9_NegotiateKeyExchange_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].EncryptionBlt = IDirect3DCryptoSession9_EncryptionBlt_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].DecryptionBlt = IDirect3DCryptoSession9_DecryptionBlt_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].GetSurfacePitch = IDirect3DCryptoSession9_GetSurfacePitch_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].StartSessionKeyRefresh = IDirect3DCryptoSession9_StartSessionKeyRefresh_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].FinishSessionKeyRefresh = IDirect3DCryptoSession9_FinishSessionKeyRefresh_Hooked<INDEX>;
    g_D3D9HookedVTables._IDirect3DCryptoSession9.tables[INDEX].GetEncryptionBltKey = IDirect3DCryptoSession9_GetEncryptionBltKey_Hooked<INDEX>;
}

// -----------------------------------------------------------------------------
void SetupD3D9HookedVTables()
{
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
