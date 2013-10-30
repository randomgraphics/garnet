// script generated file. Do _NOT_ edit.

#include "pch.h"
#include "d3d9hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::RegisterSoftwareDevice(
    void* pInitializeFunction)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::RegisterSoftwareDevice");
    if (_RegisterSoftwareDevice_pre_ptr._value) { (this->*_RegisterSoftwareDevice_pre_ptr._value)(pInitializeFunction); }
    HRESULT ret = GetRealObj()->RegisterSoftwareDevice(pInitializeFunction);
    if (_RegisterSoftwareDevice_post_ptr._value) { (this->*_RegisterSoftwareDevice_post_ptr._value)(ret, pInitializeFunction); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE Direct3D9Hook::GetAdapterCount()
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::GetAdapterCount");
    if (_GetAdapterCount_pre_ptr._value) { (this->*_GetAdapterCount_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetAdapterCount();
    if (_GetAdapterCount_post_ptr._value) { (this->*_GetAdapterCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::GetAdapterIdentifier(
    UINT Adapter,
    DWORD Flags,
    D3DADAPTER_IDENTIFIER9* pIdentifier)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::GetAdapterIdentifier");
    if (_GetAdapterIdentifier_pre_ptr._value) { (this->*_GetAdapterIdentifier_pre_ptr._value)(Adapter, Flags, pIdentifier); }
    HRESULT ret = GetRealObj()->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
    if (_GetAdapterIdentifier_post_ptr._value) { (this->*_GetAdapterIdentifier_post_ptr._value)(ret, Adapter, Flags, pIdentifier); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE Direct3D9Hook::GetAdapterModeCount(
    UINT Adapter,
    D3DFORMAT Format)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::GetAdapterModeCount");
    if (_GetAdapterModeCount_pre_ptr._value) { (this->*_GetAdapterModeCount_pre_ptr._value)(Adapter, Format); }
    UINT ret = GetRealObj()->GetAdapterModeCount(Adapter, Format);
    if (_GetAdapterModeCount_post_ptr._value) { (this->*_GetAdapterModeCount_post_ptr._value)(ret, Adapter, Format); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::EnumAdapterModes(
    UINT Adapter,
    D3DFORMAT Format,
    UINT Mode,
    D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::EnumAdapterModes");
    if (_EnumAdapterModes_pre_ptr._value) { (this->*_EnumAdapterModes_pre_ptr._value)(Adapter, Format, Mode, pMode); }
    HRESULT ret = GetRealObj()->EnumAdapterModes(Adapter, Format, Mode, pMode);
    if (_EnumAdapterModes_post_ptr._value) { (this->*_EnumAdapterModes_post_ptr._value)(ret, Adapter, Format, Mode, pMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::GetAdapterDisplayMode(
    UINT Adapter,
    D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::GetAdapterDisplayMode");
    if (_GetAdapterDisplayMode_pre_ptr._value) { (this->*_GetAdapterDisplayMode_pre_ptr._value)(Adapter, pMode); }
    HRESULT ret = GetRealObj()->GetAdapterDisplayMode(Adapter, pMode);
    if (_GetAdapterDisplayMode_post_ptr._value) { (this->*_GetAdapterDisplayMode_post_ptr._value)(ret, Adapter, pMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::CheckDeviceType(
    UINT Adapter,
    D3DDEVTYPE DevType,
    D3DFORMAT AdapterFormat,
    D3DFORMAT BackBufferFormat,
    BOOL bWindowed)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::CheckDeviceType");
    if (_CheckDeviceType_pre_ptr._value) { (this->*_CheckDeviceType_pre_ptr._value)(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed); }
    HRESULT ret = GetRealObj()->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
    if (_CheckDeviceType_post_ptr._value) { (this->*_CheckDeviceType_post_ptr._value)(ret, Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::CheckDeviceFormat(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT AdapterFormat,
    DWORD Usage,
    D3DRESOURCETYPE RType,
    D3DFORMAT CheckFormat)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::CheckDeviceFormat");
    if (_CheckDeviceFormat_pre_ptr._value) { (this->*_CheckDeviceFormat_pre_ptr._value)(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat); }
    HRESULT ret = GetRealObj()->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    if (_CheckDeviceFormat_post_ptr._value) { (this->*_CheckDeviceFormat_post_ptr._value)(ret, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::CheckDeviceMultiSampleType(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT SurfaceFormat,
    BOOL Windowed,
    D3DMULTISAMPLE_TYPE MultiSampleType,
    DWORD* pQualityLevels)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::CheckDeviceMultiSampleType");
    if (_CheckDeviceMultiSampleType_pre_ptr._value) { (this->*_CheckDeviceMultiSampleType_pre_ptr._value)(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels); }
    HRESULT ret = GetRealObj()->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
    if (_CheckDeviceMultiSampleType_post_ptr._value) { (this->*_CheckDeviceMultiSampleType_post_ptr._value)(ret, Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::CheckDepthStencilMatch(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT AdapterFormat,
    D3DFORMAT RenderTargetFormat,
    D3DFORMAT DepthStencilFormat)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::CheckDepthStencilMatch");
    if (_CheckDepthStencilMatch_pre_ptr._value) { (this->*_CheckDepthStencilMatch_pre_ptr._value)(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
    HRESULT ret = GetRealObj()->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    if (_CheckDepthStencilMatch_post_ptr._value) { (this->*_CheckDepthStencilMatch_post_ptr._value)(ret, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::CheckDeviceFormatConversion(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT SourceFormat,
    D3DFORMAT TargetFormat)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::CheckDeviceFormatConversion");
    if (_CheckDeviceFormatConversion_pre_ptr._value) { (this->*_CheckDeviceFormatConversion_pre_ptr._value)(Adapter, DeviceType, SourceFormat, TargetFormat); }
    HRESULT ret = GetRealObj()->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
    if (_CheckDeviceFormatConversion_post_ptr._value) { (this->*_CheckDeviceFormatConversion_post_ptr._value)(ret, Adapter, DeviceType, SourceFormat, TargetFormat); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::GetDeviceCaps(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DCAPS9* pCaps)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::GetDeviceCaps");
    if (_GetDeviceCaps_pre_ptr._value) { (this->*_GetDeviceCaps_pre_ptr._value)(Adapter, DeviceType, pCaps); }
    HRESULT ret = GetRealObj()->GetDeviceCaps(Adapter, DeviceType, pCaps);
    if (_GetDeviceCaps_post_ptr._value) { (this->*_GetDeviceCaps_post_ptr._value)(ret, Adapter, DeviceType, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HMONITOR STDMETHODCALLTYPE Direct3D9Hook::GetAdapterMonitor(
    UINT Adapter)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::GetAdapterMonitor");
    if (_GetAdapterMonitor_pre_ptr._value) { (this->*_GetAdapterMonitor_pre_ptr._value)(Adapter); }
    HMONITOR ret = GetRealObj()->GetAdapterMonitor(Adapter);
    if (_GetAdapterMonitor_post_ptr._value) { (this->*_GetAdapterMonitor_post_ptr._value)(ret, Adapter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9Hook::CreateDevice(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    IDirect3DDevice9** ppReturnedDeviceInterface)
{
    calltrace::AutoTrace trace(L"Direct3D9Hook::CreateDevice");
    if (_CreateDevice_pre_ptr._value) { (this->*_CreateDevice_pre_ptr._value)(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface); }
    HRESULT ret = GetRealObj()->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    if ( ppReturnedDeviceInterface && *ppReturnedDeviceInterface) { *ppReturnedDeviceInterface = RealToHooked9( *ppReturnedDeviceInterface ); }
    if (_CreateDevice_post_ptr._value) { (this->*_CreateDevice_post_ptr._value)(ret, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::TestCooperativeLevel()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::TestCooperativeLevel");
    if (_TestCooperativeLevel_pre_ptr._value) { (this->*_TestCooperativeLevel_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->TestCooperativeLevel();
    if (_TestCooperativeLevel_post_ptr._value) { (this->*_TestCooperativeLevel_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE Direct3DDevice9Hook::GetAvailableTextureMem()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetAvailableTextureMem");
    if (_GetAvailableTextureMem_pre_ptr._value) { (this->*_GetAvailableTextureMem_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetAvailableTextureMem();
    if (_GetAvailableTextureMem_post_ptr._value) { (this->*_GetAvailableTextureMem_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::EvictManagedResources()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::EvictManagedResources");
    if (_EvictManagedResources_pre_ptr._value) { (this->*_EvictManagedResources_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->EvictManagedResources();
    if (_EvictManagedResources_post_ptr._value) { (this->*_EvictManagedResources_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetDirect3D(
    IDirect3D9** ppD3D9)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetDirect3D");
    if (_GetDirect3D_pre_ptr._value) { (this->*_GetDirect3D_pre_ptr._value)(ppD3D9); }
    HRESULT ret = GetRealObj()->GetDirect3D(ppD3D9);
    if ( ppD3D9 && *ppD3D9) { *ppD3D9 = RealToHooked9( *ppD3D9 ); }
    if (_GetDirect3D_post_ptr._value) { (this->*_GetDirect3D_post_ptr._value)(ret, ppD3D9); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetDeviceCaps(
    D3DCAPS9* pCaps)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetDeviceCaps");
    if (_GetDeviceCaps_pre_ptr._value) { (this->*_GetDeviceCaps_pre_ptr._value)(pCaps); }
    HRESULT ret = GetRealObj()->GetDeviceCaps(pCaps);
    if (_GetDeviceCaps_post_ptr._value) { (this->*_GetDeviceCaps_post_ptr._value)(ret, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetDisplayMode(
    UINT iSwapChain,
    D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetDisplayMode");
    if (_GetDisplayMode_pre_ptr._value) { (this->*_GetDisplayMode_pre_ptr._value)(iSwapChain, pMode); }
    HRESULT ret = GetRealObj()->GetDisplayMode(iSwapChain, pMode);
    if (_GetDisplayMode_post_ptr._value) { (this->*_GetDisplayMode_post_ptr._value)(ret, iSwapChain, pMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetCreationParameters(
    D3DDEVICE_CREATION_PARAMETERS * pParameters)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetCreationParameters");
    if (_GetCreationParameters_pre_ptr._value) { (this->*_GetCreationParameters_pre_ptr._value)(pParameters); }
    HRESULT ret = GetRealObj()->GetCreationParameters(pParameters);
    if (_GetCreationParameters_post_ptr._value) { (this->*_GetCreationParameters_post_ptr._value)(ret, pParameters); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetCursorProperties(
    UINT XHotSpot,
    UINT YHotSpot,
    IDirect3DSurface9* pCursorBitmap)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetCursorProperties");
    if (_SetCursorProperties_pre_ptr._value) { (this->*_SetCursorProperties_pre_ptr._value)(XHotSpot, YHotSpot, pCursorBitmap); }
    HRESULT ret = GetRealObj()->SetCursorProperties(XHotSpot, YHotSpot, HookedToReal(pCursorBitmap));
    if (_SetCursorProperties_post_ptr._value) { (this->*_SetCursorProperties_post_ptr._value)(ret, XHotSpot, YHotSpot, pCursorBitmap); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE Direct3DDevice9Hook::SetCursorPosition(
    int X,
    int Y,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetCursorPosition");
    if (_SetCursorPosition_pre_ptr._value) { (this->*_SetCursorPosition_pre_ptr._value)(X, Y, Flags); }
    GetRealObj()->SetCursorPosition(X, Y, Flags);
    if (_SetCursorPosition_post_ptr._value) { (this->*_SetCursorPosition_post_ptr._value)(X, Y, Flags); }
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE Direct3DDevice9Hook::ShowCursor(
    BOOL bShow)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::ShowCursor");
    if (_ShowCursor_pre_ptr._value) { (this->*_ShowCursor_pre_ptr._value)(bShow); }
    BOOL ret = GetRealObj()->ShowCursor(bShow);
    if (_ShowCursor_post_ptr._value) { (this->*_ShowCursor_post_ptr._value)(ret, bShow); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateAdditionalSwapChain(
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    IDirect3DSwapChain9** ppSwapChain)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateAdditionalSwapChain");
    if (_CreateAdditionalSwapChain_pre_ptr._value) { (this->*_CreateAdditionalSwapChain_pre_ptr._value)(pPresentationParameters, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
    if ( ppSwapChain && *ppSwapChain) { *ppSwapChain = RealToHooked9( *ppSwapChain ); }
    if (_CreateAdditionalSwapChain_post_ptr._value) { (this->*_CreateAdditionalSwapChain_post_ptr._value)(ret, pPresentationParameters, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetSwapChain(
    UINT iSwapChain,
    IDirect3DSwapChain9** ppSwapChain)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetSwapChain");
    if (_GetSwapChain_pre_ptr._value) { (this->*_GetSwapChain_pre_ptr._value)(iSwapChain, ppSwapChain); }
    HRESULT ret = GetRealObj()->GetSwapChain(iSwapChain, ppSwapChain);
    if ( ppSwapChain && *ppSwapChain) { *ppSwapChain = RealToHooked9( *ppSwapChain ); }
    if (_GetSwapChain_post_ptr._value) { (this->*_GetSwapChain_post_ptr._value)(ret, iSwapChain, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE Direct3DDevice9Hook::GetNumberOfSwapChains()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetNumberOfSwapChains");
    if (_GetNumberOfSwapChains_pre_ptr._value) { (this->*_GetNumberOfSwapChains_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetNumberOfSwapChains();
    if (_GetNumberOfSwapChains_post_ptr._value) { (this->*_GetNumberOfSwapChains_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::Reset(
    D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::Reset");
    if (_Reset_pre_ptr._value) { (this->*_Reset_pre_ptr._value)(pPresentationParameters); }
    HRESULT ret = GetRealObj()->Reset(pPresentationParameters);
    if (_Reset_post_ptr._value) { (this->*_Reset_post_ptr._value)(ret, pPresentationParameters); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::Present(
    CONST RECT* pSourceRect,
    CONST RECT* pDestRect,
    HWND hDestWindowOverride,
    CONST RGNDATA* pDirtyRegion)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::Present");
    if (_Present_pre_ptr._value) { (this->*_Present_pre_ptr._value)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }
    HRESULT ret = GetRealObj()->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    if (_Present_post_ptr._value) { (this->*_Present_post_ptr._value)(ret, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetBackBuffer(
    UINT iSwapChain,
    UINT iBackBuffer,
    D3DBACKBUFFER_TYPE Type,
    IDirect3DSurface9** ppBackBuffer)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetBackBuffer");
    if (_GetBackBuffer_pre_ptr._value) { (this->*_GetBackBuffer_pre_ptr._value)(iSwapChain, iBackBuffer, Type, ppBackBuffer); }
    HRESULT ret = GetRealObj()->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
    if ( ppBackBuffer && *ppBackBuffer) { *ppBackBuffer = RealToHooked9( *ppBackBuffer ); }
    if (_GetBackBuffer_post_ptr._value) { (this->*_GetBackBuffer_post_ptr._value)(ret, iSwapChain, iBackBuffer, Type, ppBackBuffer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetRasterStatus(
    UINT iSwapChain,
    D3DRASTER_STATUS* pRasterStatus)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetRasterStatus");
    if (_GetRasterStatus_pre_ptr._value) { (this->*_GetRasterStatus_pre_ptr._value)(iSwapChain, pRasterStatus); }
    HRESULT ret = GetRealObj()->GetRasterStatus(iSwapChain, pRasterStatus);
    if (_GetRasterStatus_post_ptr._value) { (this->*_GetRasterStatus_post_ptr._value)(ret, iSwapChain, pRasterStatus); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetDialogBoxMode(
    BOOL bEnableDialogs)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetDialogBoxMode");
    if (_SetDialogBoxMode_pre_ptr._value) { (this->*_SetDialogBoxMode_pre_ptr._value)(bEnableDialogs); }
    HRESULT ret = GetRealObj()->SetDialogBoxMode(bEnableDialogs);
    if (_SetDialogBoxMode_post_ptr._value) { (this->*_SetDialogBoxMode_post_ptr._value)(ret, bEnableDialogs); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE Direct3DDevice9Hook::SetGammaRamp(
    UINT iSwapChain,
    DWORD Flags,
    CONST D3DGAMMARAMP* pRamp)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetGammaRamp");
    if (_SetGammaRamp_pre_ptr._value) { (this->*_SetGammaRamp_pre_ptr._value)(iSwapChain, Flags, pRamp); }
    GetRealObj()->SetGammaRamp(iSwapChain, Flags, pRamp);
    if (_SetGammaRamp_post_ptr._value) { (this->*_SetGammaRamp_post_ptr._value)(iSwapChain, Flags, pRamp); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE Direct3DDevice9Hook::GetGammaRamp(
    UINT iSwapChain,
    D3DGAMMARAMP* pRamp)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetGammaRamp");
    if (_GetGammaRamp_pre_ptr._value) { (this->*_GetGammaRamp_pre_ptr._value)(iSwapChain, pRamp); }
    GetRealObj()->GetGammaRamp(iSwapChain, pRamp);
    if (_GetGammaRamp_post_ptr._value) { (this->*_GetGammaRamp_post_ptr._value)(iSwapChain, pRamp); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateTexture(
    UINT Width,
    UINT Height,
    UINT Levels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DTexture9** ppTexture,
    HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateTexture");
    if (_CreateTexture_pre_ptr._value) { (this->*_CreateTexture_pre_ptr._value)(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle); }
    HRESULT ret = GetRealObj()->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
    if ( ppTexture && *ppTexture) { *ppTexture = RealToHooked9( *ppTexture ); }
    if (_CreateTexture_post_ptr._value) { (this->*_CreateTexture_post_ptr._value)(ret, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateVolumeTexture(
    UINT Width,
    UINT Height,
    UINT Depth,
    UINT Levels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DVolumeTexture9** ppVolumeTexture,
    HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateVolumeTexture");
    if (_CreateVolumeTexture_pre_ptr._value) { (this->*_CreateVolumeTexture_pre_ptr._value)(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle); }
    HRESULT ret = GetRealObj()->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
    if ( ppVolumeTexture && *ppVolumeTexture) { *ppVolumeTexture = RealToHooked9( *ppVolumeTexture ); }
    if (_CreateVolumeTexture_post_ptr._value) { (this->*_CreateVolumeTexture_post_ptr._value)(ret, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateCubeTexture(
    UINT EdgeLength,
    UINT Levels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DCubeTexture9** ppCubeTexture,
    HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateCubeTexture");
    if (_CreateCubeTexture_pre_ptr._value) { (this->*_CreateCubeTexture_pre_ptr._value)(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle); }
    HRESULT ret = GetRealObj()->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
    if ( ppCubeTexture && *ppCubeTexture) { *ppCubeTexture = RealToHooked9( *ppCubeTexture ); }
    if (_CreateCubeTexture_post_ptr._value) { (this->*_CreateCubeTexture_post_ptr._value)(ret, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateVertexBuffer(
    UINT Length,
    DWORD Usage,
    DWORD FVF,
    D3DPOOL Pool,
    IDirect3DVertexBuffer9** ppVertexBuffer,
    HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateVertexBuffer");
    if (_CreateVertexBuffer_pre_ptr._value) { (this->*_CreateVertexBuffer_pre_ptr._value)(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle); }
    HRESULT ret = GetRealObj()->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
    if ( ppVertexBuffer && *ppVertexBuffer) { *ppVertexBuffer = RealToHooked9( *ppVertexBuffer ); }
    if (_CreateVertexBuffer_post_ptr._value) { (this->*_CreateVertexBuffer_post_ptr._value)(ret, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateIndexBuffer(
    UINT Length,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DIndexBuffer9** ppIndexBuffer,
    HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateIndexBuffer");
    if (_CreateIndexBuffer_pre_ptr._value) { (this->*_CreateIndexBuffer_pre_ptr._value)(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle); }
    HRESULT ret = GetRealObj()->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
    if ( ppIndexBuffer && *ppIndexBuffer) { *ppIndexBuffer = RealToHooked9( *ppIndexBuffer ); }
    if (_CreateIndexBuffer_post_ptr._value) { (this->*_CreateIndexBuffer_post_ptr._value)(ret, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateRenderTarget(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DMULTISAMPLE_TYPE MultiSample,
    DWORD MultisampleQuality,
    BOOL Lockable,
    IDirect3DSurface9** ppSurface,
    HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateRenderTarget");
    if (_CreateRenderTarget_pre_ptr._value) { (this->*_CreateRenderTarget_pre_ptr._value)(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle); }
    HRESULT ret = GetRealObj()->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
    if ( ppSurface && *ppSurface) { *ppSurface = RealToHooked9( *ppSurface ); }
    if (_CreateRenderTarget_post_ptr._value) { (this->*_CreateRenderTarget_post_ptr._value)(ret, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateDepthStencilSurface(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DMULTISAMPLE_TYPE MultiSample,
    DWORD MultisampleQuality,
    BOOL Discard,
    IDirect3DSurface9** ppSurface,
    HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateDepthStencilSurface");
    if (_CreateDepthStencilSurface_pre_ptr._value) { (this->*_CreateDepthStencilSurface_pre_ptr._value)(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle); }
    HRESULT ret = GetRealObj()->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
    if ( ppSurface && *ppSurface) { *ppSurface = RealToHooked9( *ppSurface ); }
    if (_CreateDepthStencilSurface_post_ptr._value) { (this->*_CreateDepthStencilSurface_post_ptr._value)(ret, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::UpdateSurface(
    IDirect3DSurface9* pSourceSurface,
    CONST RECT* pSourceRect,
    IDirect3DSurface9* pDestinationSurface,
    CONST POINT* pDestPoint)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::UpdateSurface");
    if (_UpdateSurface_pre_ptr._value) { (this->*_UpdateSurface_pre_ptr._value)(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint); }
    HRESULT ret = GetRealObj()->UpdateSurface(HookedToReal(pSourceSurface), pSourceRect, HookedToReal(pDestinationSurface), pDestPoint);
    if (_UpdateSurface_post_ptr._value) { (this->*_UpdateSurface_post_ptr._value)(ret, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::UpdateTexture(
    IDirect3DBaseTexture9* pSourceTexture,
    IDirect3DBaseTexture9* pDestinationTexture)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::UpdateTexture");
    if (_UpdateTexture_pre_ptr._value) { (this->*_UpdateTexture_pre_ptr._value)(pSourceTexture, pDestinationTexture); }
    HRESULT ret = GetRealObj()->UpdateTexture(HookedToReal(pSourceTexture), HookedToReal(pDestinationTexture));
    if (_UpdateTexture_post_ptr._value) { (this->*_UpdateTexture_post_ptr._value)(ret, pSourceTexture, pDestinationTexture); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetRenderTargetData(
    IDirect3DSurface9* pRenderTarget,
    IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetRenderTargetData");
    if (_GetRenderTargetData_pre_ptr._value) { (this->*_GetRenderTargetData_pre_ptr._value)(pRenderTarget, pDestSurface); }
    HRESULT ret = GetRealObj()->GetRenderTargetData(HookedToReal(pRenderTarget), HookedToReal(pDestSurface));
    if (_GetRenderTargetData_post_ptr._value) { (this->*_GetRenderTargetData_post_ptr._value)(ret, pRenderTarget, pDestSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetFrontBufferData(
    UINT iSwapChain,
    IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetFrontBufferData");
    if (_GetFrontBufferData_pre_ptr._value) { (this->*_GetFrontBufferData_pre_ptr._value)(iSwapChain, pDestSurface); }
    HRESULT ret = GetRealObj()->GetFrontBufferData(iSwapChain, HookedToReal(pDestSurface));
    if (_GetFrontBufferData_post_ptr._value) { (this->*_GetFrontBufferData_post_ptr._value)(ret, iSwapChain, pDestSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::StretchRect(
    IDirect3DSurface9* pSourceSurface,
    CONST RECT* pSourceRect,
    IDirect3DSurface9* pDestSurface,
    CONST RECT* pDestRect,
    D3DTEXTUREFILTERTYPE Filter)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::StretchRect");
    if (_StretchRect_pre_ptr._value) { (this->*_StretchRect_pre_ptr._value)(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter); }
    HRESULT ret = GetRealObj()->StretchRect(HookedToReal(pSourceSurface), pSourceRect, HookedToReal(pDestSurface), pDestRect, Filter);
    if (_StretchRect_post_ptr._value) { (this->*_StretchRect_post_ptr._value)(ret, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::ColorFill(
    IDirect3DSurface9* pSurface,
    CONST RECT* pRect,
    D3DCOLOR color)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::ColorFill");
    if (_ColorFill_pre_ptr._value) { (this->*_ColorFill_pre_ptr._value)(pSurface, pRect, color); }
    HRESULT ret = GetRealObj()->ColorFill(HookedToReal(pSurface), pRect, color);
    if (_ColorFill_post_ptr._value) { (this->*_ColorFill_post_ptr._value)(ret, pSurface, pRect, color); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateOffscreenPlainSurface(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DSurface9** ppSurface,
    HANDLE* pSharedHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateOffscreenPlainSurface");
    if (_CreateOffscreenPlainSurface_pre_ptr._value) { (this->*_CreateOffscreenPlainSurface_pre_ptr._value)(Width, Height, Format, Pool, ppSurface, pSharedHandle); }
    HRESULT ret = GetRealObj()->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
    if ( ppSurface && *ppSurface) { *ppSurface = RealToHooked9( *ppSurface ); }
    if (_CreateOffscreenPlainSurface_post_ptr._value) { (this->*_CreateOffscreenPlainSurface_post_ptr._value)(ret, Width, Height, Format, Pool, ppSurface, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetRenderTarget(
    DWORD RenderTargetIndex,
    IDirect3DSurface9* pRenderTarget)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetRenderTarget");
    if (_SetRenderTarget_pre_ptr._value) { (this->*_SetRenderTarget_pre_ptr._value)(RenderTargetIndex, pRenderTarget); }
    HRESULT ret = GetRealObj()->SetRenderTarget(RenderTargetIndex, HookedToReal(pRenderTarget));
    if (_SetRenderTarget_post_ptr._value) { (this->*_SetRenderTarget_post_ptr._value)(ret, RenderTargetIndex, pRenderTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetRenderTarget(
    DWORD RenderTargetIndex,
    IDirect3DSurface9** ppRenderTarget)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetRenderTarget");
    if (_GetRenderTarget_pre_ptr._value) { (this->*_GetRenderTarget_pre_ptr._value)(RenderTargetIndex, ppRenderTarget); }
    HRESULT ret = GetRealObj()->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
    if ( ppRenderTarget && *ppRenderTarget) { *ppRenderTarget = RealToHooked9( *ppRenderTarget ); }
    if (_GetRenderTarget_post_ptr._value) { (this->*_GetRenderTarget_post_ptr._value)(ret, RenderTargetIndex, ppRenderTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetDepthStencilSurface(
    IDirect3DSurface9* pNewZStencil)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetDepthStencilSurface");
    if (_SetDepthStencilSurface_pre_ptr._value) { (this->*_SetDepthStencilSurface_pre_ptr._value)(pNewZStencil); }
    HRESULT ret = GetRealObj()->SetDepthStencilSurface(HookedToReal(pNewZStencil));
    if (_SetDepthStencilSurface_post_ptr._value) { (this->*_SetDepthStencilSurface_post_ptr._value)(ret, pNewZStencil); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetDepthStencilSurface(
    IDirect3DSurface9** ppZStencilSurface)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetDepthStencilSurface");
    if (_GetDepthStencilSurface_pre_ptr._value) { (this->*_GetDepthStencilSurface_pre_ptr._value)(ppZStencilSurface); }
    HRESULT ret = GetRealObj()->GetDepthStencilSurface(ppZStencilSurface);
    if ( ppZStencilSurface && *ppZStencilSurface) { *ppZStencilSurface = RealToHooked9( *ppZStencilSurface ); }
    if (_GetDepthStencilSurface_post_ptr._value) { (this->*_GetDepthStencilSurface_post_ptr._value)(ret, ppZStencilSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::BeginScene()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::BeginScene");
    if (_BeginScene_pre_ptr._value) { (this->*_BeginScene_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->BeginScene();
    if (_BeginScene_post_ptr._value) { (this->*_BeginScene_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::EndScene()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::EndScene");
    if (_EndScene_pre_ptr._value) { (this->*_EndScene_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->EndScene();
    if (_EndScene_post_ptr._value) { (this->*_EndScene_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::Clear(
    DWORD Count,
    CONST D3DRECT* pRects,
    DWORD Flags,
    D3DCOLOR Color,
    float Z,
    DWORD Stencil)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::Clear");
    if (_Clear_pre_ptr._value) { (this->*_Clear_pre_ptr._value)(Count, pRects, Flags, Color, Z, Stencil); }
    HRESULT ret = GetRealObj()->Clear(Count, pRects, Flags, Color, Z, Stencil);
    if (_Clear_post_ptr._value) { (this->*_Clear_post_ptr._value)(ret, Count, pRects, Flags, Color, Z, Stencil); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetTransform(
    D3DTRANSFORMSTATETYPE State,
    CONST D3DMATRIX* pMatrix)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetTransform");
    if (_SetTransform_pre_ptr._value) { (this->*_SetTransform_pre_ptr._value)(State, pMatrix); }
    HRESULT ret = GetRealObj()->SetTransform(State, pMatrix);
    if (_SetTransform_post_ptr._value) { (this->*_SetTransform_post_ptr._value)(ret, State, pMatrix); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetTransform(
    D3DTRANSFORMSTATETYPE State,
    D3DMATRIX* pMatrix)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetTransform");
    if (_GetTransform_pre_ptr._value) { (this->*_GetTransform_pre_ptr._value)(State, pMatrix); }
    HRESULT ret = GetRealObj()->GetTransform(State, pMatrix);
    if (_GetTransform_post_ptr._value) { (this->*_GetTransform_post_ptr._value)(ret, State, pMatrix); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::MultiplyTransform(
    D3DTRANSFORMSTATETYPE TransformType,
    CONST D3DMATRIX* Value)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::MultiplyTransform");
    if (_MultiplyTransform_pre_ptr._value) { (this->*_MultiplyTransform_pre_ptr._value)(TransformType, Value); }
    HRESULT ret = GetRealObj()->MultiplyTransform(TransformType, Value);
    if (_MultiplyTransform_post_ptr._value) { (this->*_MultiplyTransform_post_ptr._value)(ret, TransformType, Value); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetViewport(
    CONST D3DVIEWPORT9* pViewport)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetViewport");
    if (_SetViewport_pre_ptr._value) { (this->*_SetViewport_pre_ptr._value)(pViewport); }
    HRESULT ret = GetRealObj()->SetViewport(pViewport);
    if (_SetViewport_post_ptr._value) { (this->*_SetViewport_post_ptr._value)(ret, pViewport); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetViewport(
    D3DVIEWPORT9* pViewport)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetViewport");
    if (_GetViewport_pre_ptr._value) { (this->*_GetViewport_pre_ptr._value)(pViewport); }
    HRESULT ret = GetRealObj()->GetViewport(pViewport);
    if (_GetViewport_post_ptr._value) { (this->*_GetViewport_post_ptr._value)(ret, pViewport); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetMaterial(
    CONST D3DMATERIAL9* pMaterial)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetMaterial");
    if (_SetMaterial_pre_ptr._value) { (this->*_SetMaterial_pre_ptr._value)(pMaterial); }
    HRESULT ret = GetRealObj()->SetMaterial(pMaterial);
    if (_SetMaterial_post_ptr._value) { (this->*_SetMaterial_post_ptr._value)(ret, pMaterial); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetMaterial(
    D3DMATERIAL9* pMaterial)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetMaterial");
    if (_GetMaterial_pre_ptr._value) { (this->*_GetMaterial_pre_ptr._value)(pMaterial); }
    HRESULT ret = GetRealObj()->GetMaterial(pMaterial);
    if (_GetMaterial_post_ptr._value) { (this->*_GetMaterial_post_ptr._value)(ret, pMaterial); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetLight(
    DWORD Index,
    CONST D3DLIGHT9* pLight)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetLight");
    if (_SetLight_pre_ptr._value) { (this->*_SetLight_pre_ptr._value)(Index, pLight); }
    HRESULT ret = GetRealObj()->SetLight(Index, pLight);
    if (_SetLight_post_ptr._value) { (this->*_SetLight_post_ptr._value)(ret, Index, pLight); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetLight(
    DWORD Index,
    D3DLIGHT9* pLight)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetLight");
    if (_GetLight_pre_ptr._value) { (this->*_GetLight_pre_ptr._value)(Index, pLight); }
    HRESULT ret = GetRealObj()->GetLight(Index, pLight);
    if (_GetLight_post_ptr._value) { (this->*_GetLight_post_ptr._value)(ret, Index, pLight); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::LightEnable(
    DWORD Index,
    BOOL Enable)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::LightEnable");
    if (_LightEnable_pre_ptr._value) { (this->*_LightEnable_pre_ptr._value)(Index, Enable); }
    HRESULT ret = GetRealObj()->LightEnable(Index, Enable);
    if (_LightEnable_post_ptr._value) { (this->*_LightEnable_post_ptr._value)(ret, Index, Enable); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetLightEnable(
    DWORD Index,
    BOOL* pEnable)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetLightEnable");
    if (_GetLightEnable_pre_ptr._value) { (this->*_GetLightEnable_pre_ptr._value)(Index, pEnable); }
    HRESULT ret = GetRealObj()->GetLightEnable(Index, pEnable);
    if (_GetLightEnable_post_ptr._value) { (this->*_GetLightEnable_post_ptr._value)(ret, Index, pEnable); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetClipPlane(
    DWORD Index,
    CONST float* pPlane)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetClipPlane");
    if (_SetClipPlane_pre_ptr._value) { (this->*_SetClipPlane_pre_ptr._value)(Index, pPlane); }
    HRESULT ret = GetRealObj()->SetClipPlane(Index, pPlane);
    if (_SetClipPlane_post_ptr._value) { (this->*_SetClipPlane_post_ptr._value)(ret, Index, pPlane); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetClipPlane(
    DWORD Index,
    float* pPlane)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetClipPlane");
    if (_GetClipPlane_pre_ptr._value) { (this->*_GetClipPlane_pre_ptr._value)(Index, pPlane); }
    HRESULT ret = GetRealObj()->GetClipPlane(Index, pPlane);
    if (_GetClipPlane_post_ptr._value) { (this->*_GetClipPlane_post_ptr._value)(ret, Index, pPlane); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetRenderState(
    D3DRENDERSTATETYPE State,
    DWORD Value)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetRenderState");
    if (_SetRenderState_pre_ptr._value) { (this->*_SetRenderState_pre_ptr._value)(State, Value); }
    HRESULT ret = GetRealObj()->SetRenderState(State, Value);
    if (_SetRenderState_post_ptr._value) { (this->*_SetRenderState_post_ptr._value)(ret, State, Value); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetRenderState(
    D3DRENDERSTATETYPE State,
    DWORD* pValue)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetRenderState");
    if (_GetRenderState_pre_ptr._value) { (this->*_GetRenderState_pre_ptr._value)(State, pValue); }
    HRESULT ret = GetRealObj()->GetRenderState(State, pValue);
    if (_GetRenderState_post_ptr._value) { (this->*_GetRenderState_post_ptr._value)(ret, State, pValue); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateStateBlock(
    D3DSTATEBLOCKTYPE Type,
    IDirect3DStateBlock9** ppSB)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateStateBlock");
    if (_CreateStateBlock_pre_ptr._value) { (this->*_CreateStateBlock_pre_ptr._value)(Type, ppSB); }
    HRESULT ret = GetRealObj()->CreateStateBlock(Type, ppSB);
    if ( ppSB && *ppSB) { *ppSB = RealToHooked9( *ppSB ); }
    if (_CreateStateBlock_post_ptr._value) { (this->*_CreateStateBlock_post_ptr._value)(ret, Type, ppSB); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::BeginStateBlock()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::BeginStateBlock");
    if (_BeginStateBlock_pre_ptr._value) { (this->*_BeginStateBlock_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->BeginStateBlock();
    if (_BeginStateBlock_post_ptr._value) { (this->*_BeginStateBlock_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::EndStateBlock(
    IDirect3DStateBlock9** ppSB)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::EndStateBlock");
    if (_EndStateBlock_pre_ptr._value) { (this->*_EndStateBlock_pre_ptr._value)(ppSB); }
    HRESULT ret = GetRealObj()->EndStateBlock(ppSB);
    if ( ppSB && *ppSB) { *ppSB = RealToHooked9( *ppSB ); }
    if (_EndStateBlock_post_ptr._value) { (this->*_EndStateBlock_post_ptr._value)(ret, ppSB); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetClipStatus(
    CONST D3DCLIPSTATUS9* pClipStatus)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetClipStatus");
    if (_SetClipStatus_pre_ptr._value) { (this->*_SetClipStatus_pre_ptr._value)(pClipStatus); }
    HRESULT ret = GetRealObj()->SetClipStatus(pClipStatus);
    if (_SetClipStatus_post_ptr._value) { (this->*_SetClipStatus_post_ptr._value)(ret, pClipStatus); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetClipStatus(
    D3DCLIPSTATUS9* pClipStatus)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetClipStatus");
    if (_GetClipStatus_pre_ptr._value) { (this->*_GetClipStatus_pre_ptr._value)(pClipStatus); }
    HRESULT ret = GetRealObj()->GetClipStatus(pClipStatus);
    if (_GetClipStatus_post_ptr._value) { (this->*_GetClipStatus_post_ptr._value)(ret, pClipStatus); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetTexture(
    DWORD Stage,
    IDirect3DBaseTexture9** ppTexture)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetTexture");
    if (_GetTexture_pre_ptr._value) { (this->*_GetTexture_pre_ptr._value)(Stage, ppTexture); }
    HRESULT ret = GetRealObj()->GetTexture(Stage, ppTexture);
    if ( ppTexture && *ppTexture) { *ppTexture = RealToHooked9( *ppTexture ); }
    if (_GetTexture_post_ptr._value) { (this->*_GetTexture_post_ptr._value)(ret, Stage, ppTexture); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetTexture(
    DWORD Stage,
    IDirect3DBaseTexture9* pTexture)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetTexture");
    if (_SetTexture_pre_ptr._value) { (this->*_SetTexture_pre_ptr._value)(Stage, pTexture); }
    HRESULT ret = GetRealObj()->SetTexture(Stage, HookedToReal(pTexture));
    if (_SetTexture_post_ptr._value) { (this->*_SetTexture_post_ptr._value)(ret, Stage, pTexture); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetTextureStageState(
    DWORD Stage,
    D3DTEXTURESTAGESTATETYPE Type,
    DWORD* pValue)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetTextureStageState");
    if (_GetTextureStageState_pre_ptr._value) { (this->*_GetTextureStageState_pre_ptr._value)(Stage, Type, pValue); }
    HRESULT ret = GetRealObj()->GetTextureStageState(Stage, Type, pValue);
    if (_GetTextureStageState_post_ptr._value) { (this->*_GetTextureStageState_post_ptr._value)(ret, Stage, Type, pValue); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetTextureStageState(
    DWORD Stage,
    D3DTEXTURESTAGESTATETYPE Type,
    DWORD Value)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetTextureStageState");
    if (_SetTextureStageState_pre_ptr._value) { (this->*_SetTextureStageState_pre_ptr._value)(Stage, Type, Value); }
    HRESULT ret = GetRealObj()->SetTextureStageState(Stage, Type, Value);
    if (_SetTextureStageState_post_ptr._value) { (this->*_SetTextureStageState_post_ptr._value)(ret, Stage, Type, Value); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetSamplerState(
    DWORD Sampler,
    D3DSAMPLERSTATETYPE Type,
    DWORD* pValue)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetSamplerState");
    if (_GetSamplerState_pre_ptr._value) { (this->*_GetSamplerState_pre_ptr._value)(Sampler, Type, pValue); }
    HRESULT ret = GetRealObj()->GetSamplerState(Sampler, Type, pValue);
    if (_GetSamplerState_post_ptr._value) { (this->*_GetSamplerState_post_ptr._value)(ret, Sampler, Type, pValue); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetSamplerState(
    DWORD Sampler,
    D3DSAMPLERSTATETYPE Type,
    DWORD Value)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetSamplerState");
    if (_SetSamplerState_pre_ptr._value) { (this->*_SetSamplerState_pre_ptr._value)(Sampler, Type, Value); }
    HRESULT ret = GetRealObj()->SetSamplerState(Sampler, Type, Value);
    if (_SetSamplerState_post_ptr._value) { (this->*_SetSamplerState_post_ptr._value)(ret, Sampler, Type, Value); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::ValidateDevice(
    DWORD* pNumPasses)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::ValidateDevice");
    if (_ValidateDevice_pre_ptr._value) { (this->*_ValidateDevice_pre_ptr._value)(pNumPasses); }
    HRESULT ret = GetRealObj()->ValidateDevice(pNumPasses);
    if (_ValidateDevice_post_ptr._value) { (this->*_ValidateDevice_post_ptr._value)(ret, pNumPasses); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetPaletteEntries(
    UINT PaletteNumber,
    CONST PALETTEENTRY* pEntries)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetPaletteEntries");
    if (_SetPaletteEntries_pre_ptr._value) { (this->*_SetPaletteEntries_pre_ptr._value)(PaletteNumber, pEntries); }
    HRESULT ret = GetRealObj()->SetPaletteEntries(PaletteNumber, pEntries);
    if (_SetPaletteEntries_post_ptr._value) { (this->*_SetPaletteEntries_post_ptr._value)(ret, PaletteNumber, pEntries); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetPaletteEntries(
    UINT PaletteNumber,
    PALETTEENTRY* pEntries)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetPaletteEntries");
    if (_GetPaletteEntries_pre_ptr._value) { (this->*_GetPaletteEntries_pre_ptr._value)(PaletteNumber, pEntries); }
    HRESULT ret = GetRealObj()->GetPaletteEntries(PaletteNumber, pEntries);
    if (_GetPaletteEntries_post_ptr._value) { (this->*_GetPaletteEntries_post_ptr._value)(ret, PaletteNumber, pEntries); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetCurrentTexturePalette(
    UINT PaletteNumber)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetCurrentTexturePalette");
    if (_SetCurrentTexturePalette_pre_ptr._value) { (this->*_SetCurrentTexturePalette_pre_ptr._value)(PaletteNumber); }
    HRESULT ret = GetRealObj()->SetCurrentTexturePalette(PaletteNumber);
    if (_SetCurrentTexturePalette_post_ptr._value) { (this->*_SetCurrentTexturePalette_post_ptr._value)(ret, PaletteNumber); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetCurrentTexturePalette(
    UINT * PaletteNumber)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetCurrentTexturePalette");
    if (_GetCurrentTexturePalette_pre_ptr._value) { (this->*_GetCurrentTexturePalette_pre_ptr._value)(PaletteNumber); }
    HRESULT ret = GetRealObj()->GetCurrentTexturePalette(PaletteNumber);
    if (_GetCurrentTexturePalette_post_ptr._value) { (this->*_GetCurrentTexturePalette_post_ptr._value)(ret, PaletteNumber); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetScissorRect(
    CONST RECT* pRect)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetScissorRect");
    if (_SetScissorRect_pre_ptr._value) { (this->*_SetScissorRect_pre_ptr._value)(pRect); }
    HRESULT ret = GetRealObj()->SetScissorRect(pRect);
    if (_SetScissorRect_post_ptr._value) { (this->*_SetScissorRect_post_ptr._value)(ret, pRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetScissorRect(
    RECT* pRect)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetScissorRect");
    if (_GetScissorRect_pre_ptr._value) { (this->*_GetScissorRect_pre_ptr._value)(pRect); }
    HRESULT ret = GetRealObj()->GetScissorRect(pRect);
    if (_GetScissorRect_post_ptr._value) { (this->*_GetScissorRect_post_ptr._value)(ret, pRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetSoftwareVertexProcessing(
    BOOL bSoftware)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetSoftwareVertexProcessing");
    if (_SetSoftwareVertexProcessing_pre_ptr._value) { (this->*_SetSoftwareVertexProcessing_pre_ptr._value)(bSoftware); }
    HRESULT ret = GetRealObj()->SetSoftwareVertexProcessing(bSoftware);
    if (_SetSoftwareVertexProcessing_post_ptr._value) { (this->*_SetSoftwareVertexProcessing_post_ptr._value)(ret, bSoftware); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE Direct3DDevice9Hook::GetSoftwareVertexProcessing()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetSoftwareVertexProcessing");
    if (_GetSoftwareVertexProcessing_pre_ptr._value) { (this->*_GetSoftwareVertexProcessing_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->GetSoftwareVertexProcessing();
    if (_GetSoftwareVertexProcessing_post_ptr._value) { (this->*_GetSoftwareVertexProcessing_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetNPatchMode(
    float nSegments)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetNPatchMode");
    if (_SetNPatchMode_pre_ptr._value) { (this->*_SetNPatchMode_pre_ptr._value)(nSegments); }
    HRESULT ret = GetRealObj()->SetNPatchMode(nSegments);
    if (_SetNPatchMode_post_ptr._value) { (this->*_SetNPatchMode_post_ptr._value)(ret, nSegments); }
    return ret;
}

// -----------------------------------------------------------------------------
float STDMETHODCALLTYPE Direct3DDevice9Hook::GetNPatchMode()
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetNPatchMode");
    if (_GetNPatchMode_pre_ptr._value) { (this->*_GetNPatchMode_pre_ptr._value)(); }
    float ret = GetRealObj()->GetNPatchMode();
    if (_GetNPatchMode_post_ptr._value) { (this->*_GetNPatchMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::DrawPrimitive(
    D3DPRIMITIVETYPE PrimitiveType,
    UINT StartVertex,
    UINT PrimitiveCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::DrawPrimitive");
    if (_DrawPrimitive_pre_ptr._value) { (this->*_DrawPrimitive_pre_ptr._value)(PrimitiveType, StartVertex, PrimitiveCount); }
    HRESULT ret = GetRealObj()->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    if (_DrawPrimitive_post_ptr._value) { (this->*_DrawPrimitive_post_ptr._value)(ret, PrimitiveType, StartVertex, PrimitiveCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::DrawIndexedPrimitive(
    D3DPRIMITIVETYPE PrimitiveType,
    INT BaseVertexIndex,
    UINT MinVertexIndex,
    UINT NumVertices,
    UINT startIndex,
    UINT primCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::DrawIndexedPrimitive");
    if (_DrawIndexedPrimitive_pre_ptr._value) { (this->*_DrawIndexedPrimitive_pre_ptr._value)(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); }
    HRESULT ret = GetRealObj()->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
    if (_DrawIndexedPrimitive_post_ptr._value) { (this->*_DrawIndexedPrimitive_post_ptr._value)(ret, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::DrawPrimitiveUP(
    D3DPRIMITIVETYPE PrimitiveType,
    UINT PrimitiveCount,
    CONST void* pVertexStreamZeroData,
    UINT VertexStreamZeroStride)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::DrawPrimitiveUP");
    if (_DrawPrimitiveUP_pre_ptr._value) { (this->*_DrawPrimitiveUP_pre_ptr._value)(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
    HRESULT ret = GetRealObj()->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    if (_DrawPrimitiveUP_post_ptr._value) { (this->*_DrawPrimitiveUP_post_ptr._value)(ret, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::DrawIndexedPrimitiveUP(
    D3DPRIMITIVETYPE PrimitiveType,
    UINT MinVertexIndex,
    UINT NumVertices,
    UINT PrimitiveCount,
    CONST void* pIndexData,
    D3DFORMAT IndexDataFormat,
    CONST void* pVertexStreamZeroData,
    UINT VertexStreamZeroStride)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::DrawIndexedPrimitiveUP");
    if (_DrawIndexedPrimitiveUP_pre_ptr._value) { (this->*_DrawIndexedPrimitiveUP_pre_ptr._value)(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
    HRESULT ret = GetRealObj()->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    if (_DrawIndexedPrimitiveUP_post_ptr._value) { (this->*_DrawIndexedPrimitiveUP_post_ptr._value)(ret, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::ProcessVertices(
    UINT SrcStartIndex,
    UINT DestIndex,
    UINT VertexCount,
    IDirect3DVertexBuffer9* pDestBuffer,
    IDirect3DVertexDeclaration9* pVertexDecl,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::ProcessVertices");
    if (_ProcessVertices_pre_ptr._value) { (this->*_ProcessVertices_pre_ptr._value)(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags); }
    HRESULT ret = GetRealObj()->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, HookedToReal(pDestBuffer), HookedToReal(pVertexDecl), Flags);
    if (_ProcessVertices_post_ptr._value) { (this->*_ProcessVertices_post_ptr._value)(ret, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateVertexDeclaration(
    CONST D3DVERTEXELEMENT9* pVertexElements,
    IDirect3DVertexDeclaration9** ppDecl)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateVertexDeclaration");
    if (_CreateVertexDeclaration_pre_ptr._value) { (this->*_CreateVertexDeclaration_pre_ptr._value)(pVertexElements, ppDecl); }
    HRESULT ret = GetRealObj()->CreateVertexDeclaration(pVertexElements, ppDecl);
    if ( ppDecl && *ppDecl) { *ppDecl = RealToHooked9( *ppDecl ); }
    if (_CreateVertexDeclaration_post_ptr._value) { (this->*_CreateVertexDeclaration_post_ptr._value)(ret, pVertexElements, ppDecl); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetVertexDeclaration(
    IDirect3DVertexDeclaration9* pDecl)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetVertexDeclaration");
    if (_SetVertexDeclaration_pre_ptr._value) { (this->*_SetVertexDeclaration_pre_ptr._value)(pDecl); }
    HRESULT ret = GetRealObj()->SetVertexDeclaration(HookedToReal(pDecl));
    if (_SetVertexDeclaration_post_ptr._value) { (this->*_SetVertexDeclaration_post_ptr._value)(ret, pDecl); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetVertexDeclaration(
    IDirect3DVertexDeclaration9** ppDecl)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetVertexDeclaration");
    if (_GetVertexDeclaration_pre_ptr._value) { (this->*_GetVertexDeclaration_pre_ptr._value)(ppDecl); }
    HRESULT ret = GetRealObj()->GetVertexDeclaration(ppDecl);
    if ( ppDecl && *ppDecl) { *ppDecl = RealToHooked9( *ppDecl ); }
    if (_GetVertexDeclaration_post_ptr._value) { (this->*_GetVertexDeclaration_post_ptr._value)(ret, ppDecl); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetFVF(
    DWORD FVF)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetFVF");
    if (_SetFVF_pre_ptr._value) { (this->*_SetFVF_pre_ptr._value)(FVF); }
    HRESULT ret = GetRealObj()->SetFVF(FVF);
    if (_SetFVF_post_ptr._value) { (this->*_SetFVF_post_ptr._value)(ret, FVF); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetFVF(
    DWORD* pFVF)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetFVF");
    if (_GetFVF_pre_ptr._value) { (this->*_GetFVF_pre_ptr._value)(pFVF); }
    HRESULT ret = GetRealObj()->GetFVF(pFVF);
    if (_GetFVF_post_ptr._value) { (this->*_GetFVF_post_ptr._value)(ret, pFVF); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateVertexShader(
    CONST DWORD* pFunction,
    IDirect3DVertexShader9** ppShader)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateVertexShader");
    if (_CreateVertexShader_pre_ptr._value) { (this->*_CreateVertexShader_pre_ptr._value)(pFunction, ppShader); }
    HRESULT ret = GetRealObj()->CreateVertexShader(pFunction, ppShader);
    if ( ppShader && *ppShader) { *ppShader = RealToHooked9( *ppShader ); }
    if (_CreateVertexShader_post_ptr._value) { (this->*_CreateVertexShader_post_ptr._value)(ret, pFunction, ppShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetVertexShader(
    IDirect3DVertexShader9* pShader)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetVertexShader");
    if (_SetVertexShader_pre_ptr._value) { (this->*_SetVertexShader_pre_ptr._value)(pShader); }
    HRESULT ret = GetRealObj()->SetVertexShader(HookedToReal(pShader));
    if (_SetVertexShader_post_ptr._value) { (this->*_SetVertexShader_post_ptr._value)(ret, pShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetVertexShader(
    IDirect3DVertexShader9** ppShader)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetVertexShader");
    if (_GetVertexShader_pre_ptr._value) { (this->*_GetVertexShader_pre_ptr._value)(ppShader); }
    HRESULT ret = GetRealObj()->GetVertexShader(ppShader);
    if ( ppShader && *ppShader) { *ppShader = RealToHooked9( *ppShader ); }
    if (_GetVertexShader_post_ptr._value) { (this->*_GetVertexShader_post_ptr._value)(ret, ppShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetVertexShaderConstantF(
    UINT StartRegister,
    CONST float* pConstantData,
    UINT Vector4fCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetVertexShaderConstantF");
    if (_SetVertexShaderConstantF_pre_ptr._value) { (this->*_SetVertexShaderConstantF_pre_ptr._value)(StartRegister, pConstantData, Vector4fCount); }
    HRESULT ret = GetRealObj()->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    if (_SetVertexShaderConstantF_post_ptr._value) { (this->*_SetVertexShaderConstantF_post_ptr._value)(ret, StartRegister, pConstantData, Vector4fCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetVertexShaderConstantF(
    UINT StartRegister,
    float* pConstantData,
    UINT Vector4fCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetVertexShaderConstantF");
    if (_GetVertexShaderConstantF_pre_ptr._value) { (this->*_GetVertexShaderConstantF_pre_ptr._value)(StartRegister, pConstantData, Vector4fCount); }
    HRESULT ret = GetRealObj()->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    if (_GetVertexShaderConstantF_post_ptr._value) { (this->*_GetVertexShaderConstantF_post_ptr._value)(ret, StartRegister, pConstantData, Vector4fCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetVertexShaderConstantI(
    UINT StartRegister,
    CONST int* pConstantData,
    UINT Vector4iCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetVertexShaderConstantI");
    if (_SetVertexShaderConstantI_pre_ptr._value) { (this->*_SetVertexShaderConstantI_pre_ptr._value)(StartRegister, pConstantData, Vector4iCount); }
    HRESULT ret = GetRealObj()->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    if (_SetVertexShaderConstantI_post_ptr._value) { (this->*_SetVertexShaderConstantI_post_ptr._value)(ret, StartRegister, pConstantData, Vector4iCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetVertexShaderConstantI(
    UINT StartRegister,
    int* pConstantData,
    UINT Vector4iCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetVertexShaderConstantI");
    if (_GetVertexShaderConstantI_pre_ptr._value) { (this->*_GetVertexShaderConstantI_pre_ptr._value)(StartRegister, pConstantData, Vector4iCount); }
    HRESULT ret = GetRealObj()->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    if (_GetVertexShaderConstantI_post_ptr._value) { (this->*_GetVertexShaderConstantI_post_ptr._value)(ret, StartRegister, pConstantData, Vector4iCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetVertexShaderConstantB(
    UINT StartRegister,
    CONST BOOL* pConstantData,
    UINT BoolCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetVertexShaderConstantB");
    if (_SetVertexShaderConstantB_pre_ptr._value) { (this->*_SetVertexShaderConstantB_pre_ptr._value)(StartRegister, pConstantData, BoolCount); }
    HRESULT ret = GetRealObj()->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    if (_SetVertexShaderConstantB_post_ptr._value) { (this->*_SetVertexShaderConstantB_post_ptr._value)(ret, StartRegister, pConstantData, BoolCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetVertexShaderConstantB(
    UINT StartRegister,
    BOOL* pConstantData,
    UINT BoolCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetVertexShaderConstantB");
    if (_GetVertexShaderConstantB_pre_ptr._value) { (this->*_GetVertexShaderConstantB_pre_ptr._value)(StartRegister, pConstantData, BoolCount); }
    HRESULT ret = GetRealObj()->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    if (_GetVertexShaderConstantB_post_ptr._value) { (this->*_GetVertexShaderConstantB_post_ptr._value)(ret, StartRegister, pConstantData, BoolCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetStreamSource(
    UINT StreamNumber,
    IDirect3DVertexBuffer9* pStreamData,
    UINT OffsetInBytes,
    UINT Stride)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetStreamSource");
    if (_SetStreamSource_pre_ptr._value) { (this->*_SetStreamSource_pre_ptr._value)(StreamNumber, pStreamData, OffsetInBytes, Stride); }
    HRESULT ret = GetRealObj()->SetStreamSource(StreamNumber, HookedToReal(pStreamData), OffsetInBytes, Stride);
    if (_SetStreamSource_post_ptr._value) { (this->*_SetStreamSource_post_ptr._value)(ret, StreamNumber, pStreamData, OffsetInBytes, Stride); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetStreamSource(
    UINT StreamNumber,
    IDirect3DVertexBuffer9** ppStreamData,
    UINT* pOffsetInBytes,
    UINT* pStride)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetStreamSource");
    if (_GetStreamSource_pre_ptr._value) { (this->*_GetStreamSource_pre_ptr._value)(StreamNumber, ppStreamData, pOffsetInBytes, pStride); }
    HRESULT ret = GetRealObj()->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
    if ( ppStreamData && *ppStreamData) { *ppStreamData = RealToHooked9( *ppStreamData ); }
    if (_GetStreamSource_post_ptr._value) { (this->*_GetStreamSource_post_ptr._value)(ret, StreamNumber, ppStreamData, pOffsetInBytes, pStride); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetStreamSourceFreq(
    UINT StreamNumber,
    UINT Setting)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetStreamSourceFreq");
    if (_SetStreamSourceFreq_pre_ptr._value) { (this->*_SetStreamSourceFreq_pre_ptr._value)(StreamNumber, Setting); }
    HRESULT ret = GetRealObj()->SetStreamSourceFreq(StreamNumber, Setting);
    if (_SetStreamSourceFreq_post_ptr._value) { (this->*_SetStreamSourceFreq_post_ptr._value)(ret, StreamNumber, Setting); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetStreamSourceFreq(
    UINT StreamNumber,
    UINT* pSetting)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetStreamSourceFreq");
    if (_GetStreamSourceFreq_pre_ptr._value) { (this->*_GetStreamSourceFreq_pre_ptr._value)(StreamNumber, pSetting); }
    HRESULT ret = GetRealObj()->GetStreamSourceFreq(StreamNumber, pSetting);
    if (_GetStreamSourceFreq_post_ptr._value) { (this->*_GetStreamSourceFreq_post_ptr._value)(ret, StreamNumber, pSetting); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetIndices(
    IDirect3DIndexBuffer9* pIndexData)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetIndices");
    if (_SetIndices_pre_ptr._value) { (this->*_SetIndices_pre_ptr._value)(pIndexData); }
    HRESULT ret = GetRealObj()->SetIndices(HookedToReal(pIndexData));
    if (_SetIndices_post_ptr._value) { (this->*_SetIndices_post_ptr._value)(ret, pIndexData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetIndices(
    IDirect3DIndexBuffer9** ppIndexData)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetIndices");
    if (_GetIndices_pre_ptr._value) { (this->*_GetIndices_pre_ptr._value)(ppIndexData); }
    HRESULT ret = GetRealObj()->GetIndices(ppIndexData);
    if ( ppIndexData && *ppIndexData) { *ppIndexData = RealToHooked9( *ppIndexData ); }
    if (_GetIndices_post_ptr._value) { (this->*_GetIndices_post_ptr._value)(ret, ppIndexData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreatePixelShader(
    CONST DWORD* pFunction,
    IDirect3DPixelShader9** ppShader)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreatePixelShader");
    if (_CreatePixelShader_pre_ptr._value) { (this->*_CreatePixelShader_pre_ptr._value)(pFunction, ppShader); }
    HRESULT ret = GetRealObj()->CreatePixelShader(pFunction, ppShader);
    if ( ppShader && *ppShader) { *ppShader = RealToHooked9( *ppShader ); }
    if (_CreatePixelShader_post_ptr._value) { (this->*_CreatePixelShader_post_ptr._value)(ret, pFunction, ppShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetPixelShader(
    IDirect3DPixelShader9* pShader)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetPixelShader");
    if (_SetPixelShader_pre_ptr._value) { (this->*_SetPixelShader_pre_ptr._value)(pShader); }
    HRESULT ret = GetRealObj()->SetPixelShader(HookedToReal(pShader));
    if (_SetPixelShader_post_ptr._value) { (this->*_SetPixelShader_post_ptr._value)(ret, pShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetPixelShader(
    IDirect3DPixelShader9** ppShader)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetPixelShader");
    if (_GetPixelShader_pre_ptr._value) { (this->*_GetPixelShader_pre_ptr._value)(ppShader); }
    HRESULT ret = GetRealObj()->GetPixelShader(ppShader);
    if ( ppShader && *ppShader) { *ppShader = RealToHooked9( *ppShader ); }
    if (_GetPixelShader_post_ptr._value) { (this->*_GetPixelShader_post_ptr._value)(ret, ppShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetPixelShaderConstantF(
    UINT StartRegister,
    CONST float* pConstantData,
    UINT Vector4fCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetPixelShaderConstantF");
    if (_SetPixelShaderConstantF_pre_ptr._value) { (this->*_SetPixelShaderConstantF_pre_ptr._value)(StartRegister, pConstantData, Vector4fCount); }
    HRESULT ret = GetRealObj()->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    if (_SetPixelShaderConstantF_post_ptr._value) { (this->*_SetPixelShaderConstantF_post_ptr._value)(ret, StartRegister, pConstantData, Vector4fCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetPixelShaderConstantF(
    UINT StartRegister,
    float* pConstantData,
    UINT Vector4fCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetPixelShaderConstantF");
    if (_GetPixelShaderConstantF_pre_ptr._value) { (this->*_GetPixelShaderConstantF_pre_ptr._value)(StartRegister, pConstantData, Vector4fCount); }
    HRESULT ret = GetRealObj()->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    if (_GetPixelShaderConstantF_post_ptr._value) { (this->*_GetPixelShaderConstantF_post_ptr._value)(ret, StartRegister, pConstantData, Vector4fCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetPixelShaderConstantI(
    UINT StartRegister,
    CONST int* pConstantData,
    UINT Vector4iCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetPixelShaderConstantI");
    if (_SetPixelShaderConstantI_pre_ptr._value) { (this->*_SetPixelShaderConstantI_pre_ptr._value)(StartRegister, pConstantData, Vector4iCount); }
    HRESULT ret = GetRealObj()->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    if (_SetPixelShaderConstantI_post_ptr._value) { (this->*_SetPixelShaderConstantI_post_ptr._value)(ret, StartRegister, pConstantData, Vector4iCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetPixelShaderConstantI(
    UINT StartRegister,
    int* pConstantData,
    UINT Vector4iCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetPixelShaderConstantI");
    if (_GetPixelShaderConstantI_pre_ptr._value) { (this->*_GetPixelShaderConstantI_pre_ptr._value)(StartRegister, pConstantData, Vector4iCount); }
    HRESULT ret = GetRealObj()->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    if (_GetPixelShaderConstantI_post_ptr._value) { (this->*_GetPixelShaderConstantI_post_ptr._value)(ret, StartRegister, pConstantData, Vector4iCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::SetPixelShaderConstantB(
    UINT StartRegister,
    CONST BOOL* pConstantData,
    UINT BoolCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::SetPixelShaderConstantB");
    if (_SetPixelShaderConstantB_pre_ptr._value) { (this->*_SetPixelShaderConstantB_pre_ptr._value)(StartRegister, pConstantData, BoolCount); }
    HRESULT ret = GetRealObj()->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    if (_SetPixelShaderConstantB_post_ptr._value) { (this->*_SetPixelShaderConstantB_post_ptr._value)(ret, StartRegister, pConstantData, BoolCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::GetPixelShaderConstantB(
    UINT StartRegister,
    BOOL* pConstantData,
    UINT BoolCount)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::GetPixelShaderConstantB");
    if (_GetPixelShaderConstantB_pre_ptr._value) { (this->*_GetPixelShaderConstantB_pre_ptr._value)(StartRegister, pConstantData, BoolCount); }
    HRESULT ret = GetRealObj()->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    if (_GetPixelShaderConstantB_post_ptr._value) { (this->*_GetPixelShaderConstantB_post_ptr._value)(ret, StartRegister, pConstantData, BoolCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::DrawRectPatch(
    UINT Handle,
    CONST float* pNumSegs,
    CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::DrawRectPatch");
    if (_DrawRectPatch_pre_ptr._value) { (this->*_DrawRectPatch_pre_ptr._value)(Handle, pNumSegs, pRectPatchInfo); }
    HRESULT ret = GetRealObj()->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
    if (_DrawRectPatch_post_ptr._value) { (this->*_DrawRectPatch_post_ptr._value)(ret, Handle, pNumSegs, pRectPatchInfo); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::DrawTriPatch(
    UINT Handle,
    CONST float* pNumSegs,
    CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::DrawTriPatch");
    if (_DrawTriPatch_pre_ptr._value) { (this->*_DrawTriPatch_pre_ptr._value)(Handle, pNumSegs, pTriPatchInfo); }
    HRESULT ret = GetRealObj()->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
    if (_DrawTriPatch_post_ptr._value) { (this->*_DrawTriPatch_post_ptr._value)(ret, Handle, pNumSegs, pTriPatchInfo); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::DeletePatch(
    UINT Handle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::DeletePatch");
    if (_DeletePatch_pre_ptr._value) { (this->*_DeletePatch_pre_ptr._value)(Handle); }
    HRESULT ret = GetRealObj()->DeletePatch(Handle);
    if (_DeletePatch_post_ptr._value) { (this->*_DeletePatch_post_ptr._value)(ret, Handle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9Hook::CreateQuery(
    D3DQUERYTYPE Type,
    IDirect3DQuery9** ppQuery)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9Hook::CreateQuery");
    if (_CreateQuery_pre_ptr._value) { (this->*_CreateQuery_pre_ptr._value)(Type, ppQuery); }
    HRESULT ret = GetRealObj()->CreateQuery(Type, ppQuery);
    if ( ppQuery && *ppQuery) { *ppQuery = RealToHooked9( *ppQuery ); }
    if (_CreateQuery_post_ptr._value) { (this->*_CreateQuery_post_ptr._value)(ret, Type, ppQuery); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DStateBlock9Hook::GetDevice(
    IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace(L"Direct3DStateBlock9Hook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked9( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DStateBlock9Hook::Capture()
{
    calltrace::AutoTrace trace(L"Direct3DStateBlock9Hook::Capture");
    if (_Capture_pre_ptr._value) { (this->*_Capture_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Capture();
    if (_Capture_post_ptr._value) { (this->*_Capture_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DStateBlock9Hook::Apply()
{
    calltrace::AutoTrace trace(L"Direct3DStateBlock9Hook::Apply");
    if (_Apply_pre_ptr._value) { (this->*_Apply_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Apply();
    if (_Apply_post_ptr._value) { (this->*_Apply_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9Hook::Present(
    CONST RECT* pSourceRect,
    CONST RECT* pDestRect,
    HWND hDestWindowOverride,
    CONST RGNDATA* pDirtyRegion,
    DWORD dwFlags)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9Hook::Present");
    if (_Present_pre_ptr._value) { (this->*_Present_pre_ptr._value)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags); }
    HRESULT ret = GetRealObj()->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
    if (_Present_post_ptr._value) { (this->*_Present_post_ptr._value)(ret, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9Hook::GetFrontBufferData(
    IDirect3DSurface9* pDestSurface)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9Hook::GetFrontBufferData");
    if (_GetFrontBufferData_pre_ptr._value) { (this->*_GetFrontBufferData_pre_ptr._value)(pDestSurface); }
    HRESULT ret = GetRealObj()->GetFrontBufferData(HookedToReal(pDestSurface));
    if (_GetFrontBufferData_post_ptr._value) { (this->*_GetFrontBufferData_post_ptr._value)(ret, pDestSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9Hook::GetBackBuffer(
    UINT iBackBuffer,
    D3DBACKBUFFER_TYPE Type,
    IDirect3DSurface9** ppBackBuffer)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9Hook::GetBackBuffer");
    if (_GetBackBuffer_pre_ptr._value) { (this->*_GetBackBuffer_pre_ptr._value)(iBackBuffer, Type, ppBackBuffer); }
    HRESULT ret = GetRealObj()->GetBackBuffer(iBackBuffer, Type, ppBackBuffer);
    if ( ppBackBuffer && *ppBackBuffer) { *ppBackBuffer = RealToHooked9( *ppBackBuffer ); }
    if (_GetBackBuffer_post_ptr._value) { (this->*_GetBackBuffer_post_ptr._value)(ret, iBackBuffer, Type, ppBackBuffer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9Hook::GetRasterStatus(
    D3DRASTER_STATUS* pRasterStatus)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9Hook::GetRasterStatus");
    if (_GetRasterStatus_pre_ptr._value) { (this->*_GetRasterStatus_pre_ptr._value)(pRasterStatus); }
    HRESULT ret = GetRealObj()->GetRasterStatus(pRasterStatus);
    if (_GetRasterStatus_post_ptr._value) { (this->*_GetRasterStatus_post_ptr._value)(ret, pRasterStatus); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9Hook::GetDisplayMode(
    D3DDISPLAYMODE* pMode)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9Hook::GetDisplayMode");
    if (_GetDisplayMode_pre_ptr._value) { (this->*_GetDisplayMode_pre_ptr._value)(pMode); }
    HRESULT ret = GetRealObj()->GetDisplayMode(pMode);
    if (_GetDisplayMode_post_ptr._value) { (this->*_GetDisplayMode_post_ptr._value)(ret, pMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9Hook::GetDevice(
    IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9Hook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked9( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9Hook::GetPresentParameters(
    D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9Hook::GetPresentParameters");
    if (_GetPresentParameters_pre_ptr._value) { (this->*_GetPresentParameters_pre_ptr._value)(pPresentationParameters); }
    HRESULT ret = GetRealObj()->GetPresentParameters(pPresentationParameters);
    if (_GetPresentParameters_post_ptr._value) { (this->*_GetPresentParameters_post_ptr._value)(ret, pPresentationParameters); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DResource9Hook::GetDevice(
    IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace(L"Direct3DResource9Hook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked9( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DResource9Hook::SetPrivateData(
    REFGUID refguid,
    CONST void* pData,
    DWORD SizeOfData,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DResource9Hook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(refguid, pData, SizeOfData, Flags); }
    HRESULT ret = GetRealObj()->SetPrivateData(refguid, pData, SizeOfData, Flags);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, refguid, pData, SizeOfData, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DResource9Hook::GetPrivateData(
    REFGUID refguid,
    void* pData,
    DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace(L"Direct3DResource9Hook::GetPrivateData");
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(refguid, pData, pSizeOfData); }
    HRESULT ret = GetRealObj()->GetPrivateData(refguid, pData, pSizeOfData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, refguid, pData, pSizeOfData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DResource9Hook::FreePrivateData(
    REFGUID refguid)
{
    calltrace::AutoTrace trace(L"Direct3DResource9Hook::FreePrivateData");
    if (_FreePrivateData_pre_ptr._value) { (this->*_FreePrivateData_pre_ptr._value)(refguid); }
    HRESULT ret = GetRealObj()->FreePrivateData(refguid);
    if (_FreePrivateData_post_ptr._value) { (this->*_FreePrivateData_post_ptr._value)(ret, refguid); }
    return ret;
}

// -----------------------------------------------------------------------------
DWORD STDMETHODCALLTYPE Direct3DResource9Hook::SetPriority(
    DWORD PriorityNew)
{
    calltrace::AutoTrace trace(L"Direct3DResource9Hook::SetPriority");
    if (_SetPriority_pre_ptr._value) { (this->*_SetPriority_pre_ptr._value)(PriorityNew); }
    DWORD ret = GetRealObj()->SetPriority(PriorityNew);
    if (_SetPriority_post_ptr._value) { (this->*_SetPriority_post_ptr._value)(ret, PriorityNew); }
    return ret;
}

// -----------------------------------------------------------------------------
DWORD STDMETHODCALLTYPE Direct3DResource9Hook::GetPriority()
{
    calltrace::AutoTrace trace(L"Direct3DResource9Hook::GetPriority");
    if (_GetPriority_pre_ptr._value) { (this->*_GetPriority_pre_ptr._value)(); }
    DWORD ret = GetRealObj()->GetPriority();
    if (_GetPriority_post_ptr._value) { (this->*_GetPriority_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE Direct3DResource9Hook::PreLoad()
{
    calltrace::AutoTrace trace(L"Direct3DResource9Hook::PreLoad");
    if (_PreLoad_pre_ptr._value) { (this->*_PreLoad_pre_ptr._value)(); }
    GetRealObj()->PreLoad();
    if (_PreLoad_post_ptr._value) { (this->*_PreLoad_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
D3DRESOURCETYPE STDMETHODCALLTYPE Direct3DResource9Hook::GetType()
{
    calltrace::AutoTrace trace(L"Direct3DResource9Hook::GetType");
    if (_GetType_pre_ptr._value) { (this->*_GetType_pre_ptr._value)(); }
    D3DRESOURCETYPE ret = GetRealObj()->GetType();
    if (_GetType_post_ptr._value) { (this->*_GetType_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVertexDeclaration9Hook::GetDevice(
    IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace(L"Direct3DVertexDeclaration9Hook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked9( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVertexDeclaration9Hook::GetDeclaration(
    D3DVERTEXELEMENT9* pElement,
    UINT* pNumElements)
{
    calltrace::AutoTrace trace(L"Direct3DVertexDeclaration9Hook::GetDeclaration");
    if (_GetDeclaration_pre_ptr._value) { (this->*_GetDeclaration_pre_ptr._value)(pElement, pNumElements); }
    HRESULT ret = GetRealObj()->GetDeclaration(pElement, pNumElements);
    if (_GetDeclaration_post_ptr._value) { (this->*_GetDeclaration_post_ptr._value)(ret, pElement, pNumElements); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVertexShader9Hook::GetDevice(
    IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace(L"Direct3DVertexShader9Hook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked9( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVertexShader9Hook::GetFunction(
    void* pFunction,
    UINT* pSizeOfData)
{
    calltrace::AutoTrace trace(L"Direct3DVertexShader9Hook::GetFunction");
    if (_GetFunction_pre_ptr._value) { (this->*_GetFunction_pre_ptr._value)(pFunction, pSizeOfData); }
    HRESULT ret = GetRealObj()->GetFunction(pFunction, pSizeOfData);
    if (_GetFunction_post_ptr._value) { (this->*_GetFunction_post_ptr._value)(ret, pFunction, pSizeOfData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DPixelShader9Hook::GetDevice(
    IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace(L"Direct3DPixelShader9Hook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked9( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DPixelShader9Hook::GetFunction(
    void* pFunction,
    UINT* pSizeOfData)
{
    calltrace::AutoTrace trace(L"Direct3DPixelShader9Hook::GetFunction");
    if (_GetFunction_pre_ptr._value) { (this->*_GetFunction_pre_ptr._value)(pFunction, pSizeOfData); }
    HRESULT ret = GetRealObj()->GetFunction(pFunction, pSizeOfData);
    if (_GetFunction_post_ptr._value) { (this->*_GetFunction_post_ptr._value)(ret, pFunction, pSizeOfData); }
    return ret;
}

// -----------------------------------------------------------------------------
DWORD STDMETHODCALLTYPE Direct3DBaseTexture9Hook::SetLOD(
    DWORD LODNew)
{
    calltrace::AutoTrace trace(L"Direct3DBaseTexture9Hook::SetLOD");
    if (_SetLOD_pre_ptr._value) { (this->*_SetLOD_pre_ptr._value)(LODNew); }
    DWORD ret = GetRealObj()->SetLOD(LODNew);
    if (_SetLOD_post_ptr._value) { (this->*_SetLOD_post_ptr._value)(ret, LODNew); }
    return ret;
}

// -----------------------------------------------------------------------------
DWORD STDMETHODCALLTYPE Direct3DBaseTexture9Hook::GetLOD()
{
    calltrace::AutoTrace trace(L"Direct3DBaseTexture9Hook::GetLOD");
    if (_GetLOD_pre_ptr._value) { (this->*_GetLOD_pre_ptr._value)(); }
    DWORD ret = GetRealObj()->GetLOD();
    if (_GetLOD_post_ptr._value) { (this->*_GetLOD_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWORD STDMETHODCALLTYPE Direct3DBaseTexture9Hook::GetLevelCount()
{
    calltrace::AutoTrace trace(L"Direct3DBaseTexture9Hook::GetLevelCount");
    if (_GetLevelCount_pre_ptr._value) { (this->*_GetLevelCount_pre_ptr._value)(); }
    DWORD ret = GetRealObj()->GetLevelCount();
    if (_GetLevelCount_post_ptr._value) { (this->*_GetLevelCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DBaseTexture9Hook::SetAutoGenFilterType(
    D3DTEXTUREFILTERTYPE FilterType)
{
    calltrace::AutoTrace trace(L"Direct3DBaseTexture9Hook::SetAutoGenFilterType");
    if (_SetAutoGenFilterType_pre_ptr._value) { (this->*_SetAutoGenFilterType_pre_ptr._value)(FilterType); }
    HRESULT ret = GetRealObj()->SetAutoGenFilterType(FilterType);
    if (_SetAutoGenFilterType_post_ptr._value) { (this->*_SetAutoGenFilterType_post_ptr._value)(ret, FilterType); }
    return ret;
}

// -----------------------------------------------------------------------------
D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE Direct3DBaseTexture9Hook::GetAutoGenFilterType()
{
    calltrace::AutoTrace trace(L"Direct3DBaseTexture9Hook::GetAutoGenFilterType");
    if (_GetAutoGenFilterType_pre_ptr._value) { (this->*_GetAutoGenFilterType_pre_ptr._value)(); }
    D3DTEXTUREFILTERTYPE ret = GetRealObj()->GetAutoGenFilterType();
    if (_GetAutoGenFilterType_post_ptr._value) { (this->*_GetAutoGenFilterType_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE Direct3DBaseTexture9Hook::GenerateMipSubLevels()
{
    calltrace::AutoTrace trace(L"Direct3DBaseTexture9Hook::GenerateMipSubLevels");
    if (_GenerateMipSubLevels_pre_ptr._value) { (this->*_GenerateMipSubLevels_pre_ptr._value)(); }
    GetRealObj()->GenerateMipSubLevels();
    if (_GenerateMipSubLevels_post_ptr._value) { (this->*_GenerateMipSubLevels_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DTexture9Hook::GetLevelDesc(
    UINT Level,
    D3DSURFACE_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"Direct3DTexture9Hook::GetLevelDesc");
    if (_GetLevelDesc_pre_ptr._value) { (this->*_GetLevelDesc_pre_ptr._value)(Level, pDesc); }
    HRESULT ret = GetRealObj()->GetLevelDesc(Level, pDesc);
    if (_GetLevelDesc_post_ptr._value) { (this->*_GetLevelDesc_post_ptr._value)(ret, Level, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DTexture9Hook::GetSurfaceLevel(
    UINT Level,
    IDirect3DSurface9** ppSurfaceLevel)
{
    calltrace::AutoTrace trace(L"Direct3DTexture9Hook::GetSurfaceLevel");
    if (_GetSurfaceLevel_pre_ptr._value) { (this->*_GetSurfaceLevel_pre_ptr._value)(Level, ppSurfaceLevel); }
    HRESULT ret = GetRealObj()->GetSurfaceLevel(Level, ppSurfaceLevel);
    if ( ppSurfaceLevel && *ppSurfaceLevel) { *ppSurfaceLevel = RealToHooked9( *ppSurfaceLevel ); }
    if (_GetSurfaceLevel_post_ptr._value) { (this->*_GetSurfaceLevel_post_ptr._value)(ret, Level, ppSurfaceLevel); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DTexture9Hook::LockRect(
    UINT Level,
    D3DLOCKED_RECT* pLockedRect,
    CONST RECT* pRect,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DTexture9Hook::LockRect");
    if (_LockRect_pre_ptr._value) { (this->*_LockRect_pre_ptr._value)(Level, pLockedRect, pRect, Flags); }
    HRESULT ret = GetRealObj()->LockRect(Level, pLockedRect, pRect, Flags);
    if (_LockRect_post_ptr._value) { (this->*_LockRect_post_ptr._value)(ret, Level, pLockedRect, pRect, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DTexture9Hook::UnlockRect(
    UINT Level)
{
    calltrace::AutoTrace trace(L"Direct3DTexture9Hook::UnlockRect");
    if (_UnlockRect_pre_ptr._value) { (this->*_UnlockRect_pre_ptr._value)(Level); }
    HRESULT ret = GetRealObj()->UnlockRect(Level);
    if (_UnlockRect_post_ptr._value) { (this->*_UnlockRect_post_ptr._value)(ret, Level); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DTexture9Hook::AddDirtyRect(
    CONST RECT* pDirtyRect)
{
    calltrace::AutoTrace trace(L"Direct3DTexture9Hook::AddDirtyRect");
    if (_AddDirtyRect_pre_ptr._value) { (this->*_AddDirtyRect_pre_ptr._value)(pDirtyRect); }
    HRESULT ret = GetRealObj()->AddDirtyRect(pDirtyRect);
    if (_AddDirtyRect_post_ptr._value) { (this->*_AddDirtyRect_post_ptr._value)(ret, pDirtyRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolumeTexture9Hook::GetLevelDesc(
    UINT Level,
    D3DVOLUME_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"Direct3DVolumeTexture9Hook::GetLevelDesc");
    if (_GetLevelDesc_pre_ptr._value) { (this->*_GetLevelDesc_pre_ptr._value)(Level, pDesc); }
    HRESULT ret = GetRealObj()->GetLevelDesc(Level, pDesc);
    if (_GetLevelDesc_post_ptr._value) { (this->*_GetLevelDesc_post_ptr._value)(ret, Level, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolumeTexture9Hook::GetVolumeLevel(
    UINT Level,
    IDirect3DVolume9** ppVolumeLevel)
{
    calltrace::AutoTrace trace(L"Direct3DVolumeTexture9Hook::GetVolumeLevel");
    if (_GetVolumeLevel_pre_ptr._value) { (this->*_GetVolumeLevel_pre_ptr._value)(Level, ppVolumeLevel); }
    HRESULT ret = GetRealObj()->GetVolumeLevel(Level, ppVolumeLevel);
    if ( ppVolumeLevel && *ppVolumeLevel) { *ppVolumeLevel = RealToHooked9( *ppVolumeLevel ); }
    if (_GetVolumeLevel_post_ptr._value) { (this->*_GetVolumeLevel_post_ptr._value)(ret, Level, ppVolumeLevel); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolumeTexture9Hook::LockBox(
    UINT Level,
    D3DLOCKED_BOX* pLockedVolume,
    CONST D3DBOX* pBox,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DVolumeTexture9Hook::LockBox");
    if (_LockBox_pre_ptr._value) { (this->*_LockBox_pre_ptr._value)(Level, pLockedVolume, pBox, Flags); }
    HRESULT ret = GetRealObj()->LockBox(Level, pLockedVolume, pBox, Flags);
    if (_LockBox_post_ptr._value) { (this->*_LockBox_post_ptr._value)(ret, Level, pLockedVolume, pBox, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolumeTexture9Hook::UnlockBox(
    UINT Level)
{
    calltrace::AutoTrace trace(L"Direct3DVolumeTexture9Hook::UnlockBox");
    if (_UnlockBox_pre_ptr._value) { (this->*_UnlockBox_pre_ptr._value)(Level); }
    HRESULT ret = GetRealObj()->UnlockBox(Level);
    if (_UnlockBox_post_ptr._value) { (this->*_UnlockBox_post_ptr._value)(ret, Level); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolumeTexture9Hook::AddDirtyBox(
    CONST D3DBOX* pDirtyBox)
{
    calltrace::AutoTrace trace(L"Direct3DVolumeTexture9Hook::AddDirtyBox");
    if (_AddDirtyBox_pre_ptr._value) { (this->*_AddDirtyBox_pre_ptr._value)(pDirtyBox); }
    HRESULT ret = GetRealObj()->AddDirtyBox(pDirtyBox);
    if (_AddDirtyBox_post_ptr._value) { (this->*_AddDirtyBox_post_ptr._value)(ret, pDirtyBox); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCubeTexture9Hook::GetLevelDesc(
    UINT Level,
    D3DSURFACE_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"Direct3DCubeTexture9Hook::GetLevelDesc");
    if (_GetLevelDesc_pre_ptr._value) { (this->*_GetLevelDesc_pre_ptr._value)(Level, pDesc); }
    HRESULT ret = GetRealObj()->GetLevelDesc(Level, pDesc);
    if (_GetLevelDesc_post_ptr._value) { (this->*_GetLevelDesc_post_ptr._value)(ret, Level, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCubeTexture9Hook::GetCubeMapSurface(
    D3DCUBEMAP_FACES FaceType,
    UINT Level,
    IDirect3DSurface9** ppCubeMapSurface)
{
    calltrace::AutoTrace trace(L"Direct3DCubeTexture9Hook::GetCubeMapSurface");
    if (_GetCubeMapSurface_pre_ptr._value) { (this->*_GetCubeMapSurface_pre_ptr._value)(FaceType, Level, ppCubeMapSurface); }
    HRESULT ret = GetRealObj()->GetCubeMapSurface(FaceType, Level, ppCubeMapSurface);
    if ( ppCubeMapSurface && *ppCubeMapSurface) { *ppCubeMapSurface = RealToHooked9( *ppCubeMapSurface ); }
    if (_GetCubeMapSurface_post_ptr._value) { (this->*_GetCubeMapSurface_post_ptr._value)(ret, FaceType, Level, ppCubeMapSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCubeTexture9Hook::LockRect(
    D3DCUBEMAP_FACES FaceType,
    UINT Level,
    D3DLOCKED_RECT* pLockedRect,
    CONST RECT* pRect,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DCubeTexture9Hook::LockRect");
    if (_LockRect_pre_ptr._value) { (this->*_LockRect_pre_ptr._value)(FaceType, Level, pLockedRect, pRect, Flags); }
    HRESULT ret = GetRealObj()->LockRect(FaceType, Level, pLockedRect, pRect, Flags);
    if (_LockRect_post_ptr._value) { (this->*_LockRect_post_ptr._value)(ret, FaceType, Level, pLockedRect, pRect, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCubeTexture9Hook::UnlockRect(
    D3DCUBEMAP_FACES FaceType,
    UINT Level)
{
    calltrace::AutoTrace trace(L"Direct3DCubeTexture9Hook::UnlockRect");
    if (_UnlockRect_pre_ptr._value) { (this->*_UnlockRect_pre_ptr._value)(FaceType, Level); }
    HRESULT ret = GetRealObj()->UnlockRect(FaceType, Level);
    if (_UnlockRect_post_ptr._value) { (this->*_UnlockRect_post_ptr._value)(ret, FaceType, Level); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCubeTexture9Hook::AddDirtyRect(
    D3DCUBEMAP_FACES FaceType,
    CONST RECT* pDirtyRect)
{
    calltrace::AutoTrace trace(L"Direct3DCubeTexture9Hook::AddDirtyRect");
    if (_AddDirtyRect_pre_ptr._value) { (this->*_AddDirtyRect_pre_ptr._value)(FaceType, pDirtyRect); }
    HRESULT ret = GetRealObj()->AddDirtyRect(FaceType, pDirtyRect);
    if (_AddDirtyRect_post_ptr._value) { (this->*_AddDirtyRect_post_ptr._value)(ret, FaceType, pDirtyRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVertexBuffer9Hook::Lock(
    UINT OffsetToLock,
    UINT SizeToLock,
    void** ppbData,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DVertexBuffer9Hook::Lock");
    if (_Lock_pre_ptr._value) { (this->*_Lock_pre_ptr._value)(OffsetToLock, SizeToLock, ppbData, Flags); }
    HRESULT ret = GetRealObj()->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
    if (_Lock_post_ptr._value) { (this->*_Lock_post_ptr._value)(ret, OffsetToLock, SizeToLock, ppbData, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVertexBuffer9Hook::Unlock()
{
    calltrace::AutoTrace trace(L"Direct3DVertexBuffer9Hook::Unlock");
    if (_Unlock_pre_ptr._value) { (this->*_Unlock_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Unlock();
    if (_Unlock_post_ptr._value) { (this->*_Unlock_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVertexBuffer9Hook::GetDesc(
    D3DVERTEXBUFFER_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"Direct3DVertexBuffer9Hook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DIndexBuffer9Hook::Lock(
    UINT OffsetToLock,
    UINT SizeToLock,
    void** ppbData,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DIndexBuffer9Hook::Lock");
    if (_Lock_pre_ptr._value) { (this->*_Lock_pre_ptr._value)(OffsetToLock, SizeToLock, ppbData, Flags); }
    HRESULT ret = GetRealObj()->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
    if (_Lock_post_ptr._value) { (this->*_Lock_post_ptr._value)(ret, OffsetToLock, SizeToLock, ppbData, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DIndexBuffer9Hook::Unlock()
{
    calltrace::AutoTrace trace(L"Direct3DIndexBuffer9Hook::Unlock");
    if (_Unlock_pre_ptr._value) { (this->*_Unlock_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Unlock();
    if (_Unlock_post_ptr._value) { (this->*_Unlock_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DIndexBuffer9Hook::GetDesc(
    D3DINDEXBUFFER_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"Direct3DIndexBuffer9Hook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSurface9Hook::GetContainer(
    REFIID riid,
    void** ppContainer)
{
    calltrace::AutoTrace trace(L"Direct3DSurface9Hook::GetContainer");
    if (_GetContainer_pre_ptr._value) { (this->*_GetContainer_pre_ptr._value)(riid, ppContainer); }
    HRESULT ret = GetRealObj()->GetContainer(riid, ppContainer);
    if (_GetContainer_post_ptr._value) { (this->*_GetContainer_post_ptr._value)(ret, riid, ppContainer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSurface9Hook::GetDesc(
    D3DSURFACE_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"Direct3DSurface9Hook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSurface9Hook::LockRect(
    D3DLOCKED_RECT* pLockedRect,
    CONST RECT* pRect,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DSurface9Hook::LockRect");
    if (_LockRect_pre_ptr._value) { (this->*_LockRect_pre_ptr._value)(pLockedRect, pRect, Flags); }
    HRESULT ret = GetRealObj()->LockRect(pLockedRect, pRect, Flags);
    if (_LockRect_post_ptr._value) { (this->*_LockRect_post_ptr._value)(ret, pLockedRect, pRect, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSurface9Hook::UnlockRect()
{
    calltrace::AutoTrace trace(L"Direct3DSurface9Hook::UnlockRect");
    if (_UnlockRect_pre_ptr._value) { (this->*_UnlockRect_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->UnlockRect();
    if (_UnlockRect_post_ptr._value) { (this->*_UnlockRect_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSurface9Hook::GetDC(
    HDC * phdc)
{
    calltrace::AutoTrace trace(L"Direct3DSurface9Hook::GetDC");
    if (_GetDC_pre_ptr._value) { (this->*_GetDC_pre_ptr._value)(phdc); }
    HRESULT ret = GetRealObj()->GetDC(phdc);
    if (_GetDC_post_ptr._value) { (this->*_GetDC_post_ptr._value)(ret, phdc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSurface9Hook::ReleaseDC(
    HDC hdc)
{
    calltrace::AutoTrace trace(L"Direct3DSurface9Hook::ReleaseDC");
    if (_ReleaseDC_pre_ptr._value) { (this->*_ReleaseDC_pre_ptr._value)(hdc); }
    HRESULT ret = GetRealObj()->ReleaseDC(hdc);
    if (_ReleaseDC_post_ptr._value) { (this->*_ReleaseDC_post_ptr._value)(ret, hdc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolume9Hook::GetDevice(
    IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace(L"Direct3DVolume9Hook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked9( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolume9Hook::SetPrivateData(
    REFGUID refguid,
    CONST void* pData,
    DWORD SizeOfData,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DVolume9Hook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(refguid, pData, SizeOfData, Flags); }
    HRESULT ret = GetRealObj()->SetPrivateData(refguid, pData, SizeOfData, Flags);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, refguid, pData, SizeOfData, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolume9Hook::GetPrivateData(
    REFGUID refguid,
    void* pData,
    DWORD* pSizeOfData)
{
    calltrace::AutoTrace trace(L"Direct3DVolume9Hook::GetPrivateData");
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(refguid, pData, pSizeOfData); }
    HRESULT ret = GetRealObj()->GetPrivateData(refguid, pData, pSizeOfData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, refguid, pData, pSizeOfData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolume9Hook::FreePrivateData(
    REFGUID refguid)
{
    calltrace::AutoTrace trace(L"Direct3DVolume9Hook::FreePrivateData");
    if (_FreePrivateData_pre_ptr._value) { (this->*_FreePrivateData_pre_ptr._value)(refguid); }
    HRESULT ret = GetRealObj()->FreePrivateData(refguid);
    if (_FreePrivateData_post_ptr._value) { (this->*_FreePrivateData_post_ptr._value)(ret, refguid); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolume9Hook::GetContainer(
    REFIID riid,
    void** ppContainer)
{
    calltrace::AutoTrace trace(L"Direct3DVolume9Hook::GetContainer");
    if (_GetContainer_pre_ptr._value) { (this->*_GetContainer_pre_ptr._value)(riid, ppContainer); }
    HRESULT ret = GetRealObj()->GetContainer(riid, ppContainer);
    if (_GetContainer_post_ptr._value) { (this->*_GetContainer_post_ptr._value)(ret, riid, ppContainer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolume9Hook::GetDesc(
    D3DVOLUME_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"Direct3DVolume9Hook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolume9Hook::LockBox(
    D3DLOCKED_BOX * pLockedVolume,
    CONST D3DBOX* pBox,
    DWORD Flags)
{
    calltrace::AutoTrace trace(L"Direct3DVolume9Hook::LockBox");
    if (_LockBox_pre_ptr._value) { (this->*_LockBox_pre_ptr._value)(pLockedVolume, pBox, Flags); }
    HRESULT ret = GetRealObj()->LockBox(pLockedVolume, pBox, Flags);
    if (_LockBox_post_ptr._value) { (this->*_LockBox_post_ptr._value)(ret, pLockedVolume, pBox, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DVolume9Hook::UnlockBox()
{
    calltrace::AutoTrace trace(L"Direct3DVolume9Hook::UnlockBox");
    if (_UnlockBox_pre_ptr._value) { (this->*_UnlockBox_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->UnlockBox();
    if (_UnlockBox_post_ptr._value) { (this->*_UnlockBox_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DQuery9Hook::GetDevice(
    IDirect3DDevice9** ppDevice)
{
    calltrace::AutoTrace trace(L"Direct3DQuery9Hook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked9( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
D3DQUERYTYPE STDMETHODCALLTYPE Direct3DQuery9Hook::GetType()
{
    calltrace::AutoTrace trace(L"Direct3DQuery9Hook::GetType");
    if (_GetType_pre_ptr._value) { (this->*_GetType_pre_ptr._value)(); }
    D3DQUERYTYPE ret = GetRealObj()->GetType();
    if (_GetType_post_ptr._value) { (this->*_GetType_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWORD STDMETHODCALLTYPE Direct3DQuery9Hook::GetDataSize()
{
    calltrace::AutoTrace trace(L"Direct3DQuery9Hook::GetDataSize");
    if (_GetDataSize_pre_ptr._value) { (this->*_GetDataSize_pre_ptr._value)(); }
    DWORD ret = GetRealObj()->GetDataSize();
    if (_GetDataSize_post_ptr._value) { (this->*_GetDataSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DQuery9Hook::Issue(
    DWORD dwIssueFlags)
{
    calltrace::AutoTrace trace(L"Direct3DQuery9Hook::Issue");
    if (_Issue_pre_ptr._value) { (this->*_Issue_pre_ptr._value)(dwIssueFlags); }
    HRESULT ret = GetRealObj()->Issue(dwIssueFlags);
    if (_Issue_post_ptr._value) { (this->*_Issue_post_ptr._value)(ret, dwIssueFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DQuery9Hook::GetData(
    void* pData,
    DWORD dwSize,
    DWORD dwGetDataFlags)
{
    calltrace::AutoTrace trace(L"Direct3DQuery9Hook::GetData");
    if (_GetData_pre_ptr._value) { (this->*_GetData_pre_ptr._value)(pData, dwSize, dwGetDataFlags); }
    HRESULT ret = GetRealObj()->GetData(pData, dwSize, dwGetDataFlags);
    if (_GetData_post_ptr._value) { (this->*_GetData_post_ptr._value)(ret, pData, dwSize, dwGetDataFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE Direct3D9ExHook::GetAdapterModeCountEx(
    UINT Adapter,
    CONST D3DDISPLAYMODEFILTER* pFilter)
{
    calltrace::AutoTrace trace(L"Direct3D9ExHook::GetAdapterModeCountEx");
    if (_GetAdapterModeCountEx_pre_ptr._value) { (this->*_GetAdapterModeCountEx_pre_ptr._value)(Adapter, pFilter); }
    UINT ret = GetRealObj()->GetAdapterModeCountEx(Adapter, pFilter);
    if (_GetAdapterModeCountEx_post_ptr._value) { (this->*_GetAdapterModeCountEx_post_ptr._value)(ret, Adapter, pFilter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9ExHook::EnumAdapterModesEx(
    UINT Adapter,
    CONST D3DDISPLAYMODEFILTER* pFilter,
    UINT Mode,
    D3DDISPLAYMODEEX* pMode)
{
    calltrace::AutoTrace trace(L"Direct3D9ExHook::EnumAdapterModesEx");
    if (_EnumAdapterModesEx_pre_ptr._value) { (this->*_EnumAdapterModesEx_pre_ptr._value)(Adapter, pFilter, Mode, pMode); }
    HRESULT ret = GetRealObj()->EnumAdapterModesEx(Adapter, pFilter, Mode, pMode);
    if (_EnumAdapterModesEx_post_ptr._value) { (this->*_EnumAdapterModesEx_post_ptr._value)(ret, Adapter, pFilter, Mode, pMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9ExHook::GetAdapterDisplayModeEx(
    UINT Adapter,
    D3DDISPLAYMODEEX* pMode,
    D3DDISPLAYROTATION* pRotation)
{
    calltrace::AutoTrace trace(L"Direct3D9ExHook::GetAdapterDisplayModeEx");
    if (_GetAdapterDisplayModeEx_pre_ptr._value) { (this->*_GetAdapterDisplayModeEx_pre_ptr._value)(Adapter, pMode, pRotation); }
    HRESULT ret = GetRealObj()->GetAdapterDisplayModeEx(Adapter, pMode, pRotation);
    if (_GetAdapterDisplayModeEx_post_ptr._value) { (this->*_GetAdapterDisplayModeEx_post_ptr._value)(ret, Adapter, pMode, pRotation); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9ExHook::CreateDeviceEx(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    D3DDISPLAYMODEEX* pFullscreenDisplayMode,
    IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
    calltrace::AutoTrace trace(L"Direct3D9ExHook::CreateDeviceEx");
    if (_CreateDeviceEx_pre_ptr._value) { (this->*_CreateDeviceEx_pre_ptr._value)(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface); }
    HRESULT ret = GetRealObj()->CreateDeviceEx(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
    if ( ppReturnedDeviceInterface && *ppReturnedDeviceInterface) { *ppReturnedDeviceInterface = RealToHooked9( *ppReturnedDeviceInterface ); }
    if (_CreateDeviceEx_post_ptr._value) { (this->*_CreateDeviceEx_post_ptr._value)(ret, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9ExHook::GetAdapterLUID(
    UINT Adapter,
    LUID * pLUID)
{
    calltrace::AutoTrace trace(L"Direct3D9ExHook::GetAdapterLUID");
    if (_GetAdapterLUID_pre_ptr._value) { (this->*_GetAdapterLUID_pre_ptr._value)(Adapter, pLUID); }
    HRESULT ret = GetRealObj()->GetAdapterLUID(Adapter, pLUID);
    if (_GetAdapterLUID_post_ptr._value) { (this->*_GetAdapterLUID_post_ptr._value)(ret, Adapter, pLUID); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::SetConvolutionMonoKernel(
    UINT width,
    UINT height,
    float* rows,
    float* columns)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::SetConvolutionMonoKernel");
    if (_SetConvolutionMonoKernel_pre_ptr._value) { (this->*_SetConvolutionMonoKernel_pre_ptr._value)(width, height, rows, columns); }
    HRESULT ret = GetRealObj()->SetConvolutionMonoKernel(width, height, rows, columns);
    if (_SetConvolutionMonoKernel_post_ptr._value) { (this->*_SetConvolutionMonoKernel_post_ptr._value)(ret, width, height, rows, columns); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::ComposeRects(
    IDirect3DSurface9* pSrc,
    IDirect3DSurface9* pDst,
    IDirect3DVertexBuffer9* pSrcRectDescs,
    UINT NumRects,
    IDirect3DVertexBuffer9* pDstRectDescs,
    D3DCOMPOSERECTSOP Operation,
    int Xoffset,
    int Yoffset)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::ComposeRects");
    if (_ComposeRects_pre_ptr._value) { (this->*_ComposeRects_pre_ptr._value)(pSrc, pDst, pSrcRectDescs, NumRects, pDstRectDescs, Operation, Xoffset, Yoffset); }
    HRESULT ret = GetRealObj()->ComposeRects(HookedToReal(pSrc), HookedToReal(pDst), HookedToReal(pSrcRectDescs), NumRects, HookedToReal(pDstRectDescs), Operation, Xoffset, Yoffset);
    if (_ComposeRects_post_ptr._value) { (this->*_ComposeRects_post_ptr._value)(ret, pSrc, pDst, pSrcRectDescs, NumRects, pDstRectDescs, Operation, Xoffset, Yoffset); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::PresentEx(
    CONST RECT* pSourceRect,
    CONST RECT* pDestRect,
    HWND hDestWindowOverride,
    CONST RGNDATA* pDirtyRegion,
    DWORD dwFlags)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::PresentEx");
    if (_PresentEx_pre_ptr._value) { (this->*_PresentEx_pre_ptr._value)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags); }
    HRESULT ret = GetRealObj()->PresentEx(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
    if (_PresentEx_post_ptr._value) { (this->*_PresentEx_post_ptr._value)(ret, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::GetGPUThreadPriority(
    INT* pPriority)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::GetGPUThreadPriority");
    if (_GetGPUThreadPriority_pre_ptr._value) { (this->*_GetGPUThreadPriority_pre_ptr._value)(pPriority); }
    HRESULT ret = GetRealObj()->GetGPUThreadPriority(pPriority);
    if (_GetGPUThreadPriority_post_ptr._value) { (this->*_GetGPUThreadPriority_post_ptr._value)(ret, pPriority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::SetGPUThreadPriority(
    INT Priority)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::SetGPUThreadPriority");
    if (_SetGPUThreadPriority_pre_ptr._value) { (this->*_SetGPUThreadPriority_pre_ptr._value)(Priority); }
    HRESULT ret = GetRealObj()->SetGPUThreadPriority(Priority);
    if (_SetGPUThreadPriority_post_ptr._value) { (this->*_SetGPUThreadPriority_post_ptr._value)(ret, Priority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::WaitForVBlank(
    UINT iSwapChain)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::WaitForVBlank");
    if (_WaitForVBlank_pre_ptr._value) { (this->*_WaitForVBlank_pre_ptr._value)(iSwapChain); }
    HRESULT ret = GetRealObj()->WaitForVBlank(iSwapChain);
    if (_WaitForVBlank_post_ptr._value) { (this->*_WaitForVBlank_post_ptr._value)(ret, iSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::CheckResourceResidency(
    IDirect3DResource9** pResourceArray,
    UINT32 NumResources)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::CheckResourceResidency");
    if (_CheckResourceResidency_pre_ptr._value) { (this->*_CheckResourceResidency_pre_ptr._value)(pResourceArray, NumResources); }
    HRESULT ret = GetRealObj()->CheckResourceResidency(pResourceArray, NumResources);
    if ( pResourceArray && *pResourceArray) { *pResourceArray = RealToHooked9( *pResourceArray ); }
    if (_CheckResourceResidency_post_ptr._value) { (this->*_CheckResourceResidency_post_ptr._value)(ret, pResourceArray, NumResources); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::SetMaximumFrameLatency(
    UINT MaxLatency)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::SetMaximumFrameLatency");
    if (_SetMaximumFrameLatency_pre_ptr._value) { (this->*_SetMaximumFrameLatency_pre_ptr._value)(MaxLatency); }
    HRESULT ret = GetRealObj()->SetMaximumFrameLatency(MaxLatency);
    if (_SetMaximumFrameLatency_post_ptr._value) { (this->*_SetMaximumFrameLatency_post_ptr._value)(ret, MaxLatency); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::GetMaximumFrameLatency(
    UINT* pMaxLatency)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::GetMaximumFrameLatency");
    if (_GetMaximumFrameLatency_pre_ptr._value) { (this->*_GetMaximumFrameLatency_pre_ptr._value)(pMaxLatency); }
    HRESULT ret = GetRealObj()->GetMaximumFrameLatency(pMaxLatency);
    if (_GetMaximumFrameLatency_post_ptr._value) { (this->*_GetMaximumFrameLatency_post_ptr._value)(ret, pMaxLatency); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::CheckDeviceState(
    HWND hDestinationWindow)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::CheckDeviceState");
    if (_CheckDeviceState_pre_ptr._value) { (this->*_CheckDeviceState_pre_ptr._value)(hDestinationWindow); }
    HRESULT ret = GetRealObj()->CheckDeviceState(hDestinationWindow);
    if (_CheckDeviceState_post_ptr._value) { (this->*_CheckDeviceState_post_ptr._value)(ret, hDestinationWindow); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::CreateRenderTargetEx(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DMULTISAMPLE_TYPE MultiSample,
    DWORD MultisampleQuality,
    BOOL Lockable,
    IDirect3DSurface9** ppSurface,
    HANDLE* pSharedHandle,
    DWORD Usage)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::CreateRenderTargetEx");
    if (_CreateRenderTargetEx_pre_ptr._value) { (this->*_CreateRenderTargetEx_pre_ptr._value)(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, Usage); }
    HRESULT ret = GetRealObj()->CreateRenderTargetEx(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, Usage);
    if ( ppSurface && *ppSurface) { *ppSurface = RealToHooked9( *ppSurface ); }
    if (_CreateRenderTargetEx_post_ptr._value) { (this->*_CreateRenderTargetEx_post_ptr._value)(ret, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, Usage); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::CreateOffscreenPlainSurfaceEx(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DSurface9** ppSurface,
    HANDLE* pSharedHandle,
    DWORD Usage)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::CreateOffscreenPlainSurfaceEx");
    if (_CreateOffscreenPlainSurfaceEx_pre_ptr._value) { (this->*_CreateOffscreenPlainSurfaceEx_pre_ptr._value)(Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage); }
    HRESULT ret = GetRealObj()->CreateOffscreenPlainSurfaceEx(Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage);
    if ( ppSurface && *ppSurface) { *ppSurface = RealToHooked9( *ppSurface ); }
    if (_CreateOffscreenPlainSurfaceEx_post_ptr._value) { (this->*_CreateOffscreenPlainSurfaceEx_post_ptr._value)(ret, Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::CreateDepthStencilSurfaceEx(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DMULTISAMPLE_TYPE MultiSample,
    DWORD MultisampleQuality,
    BOOL Discard,
    IDirect3DSurface9** ppSurface,
    HANDLE* pSharedHandle,
    DWORD Usage)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::CreateDepthStencilSurfaceEx");
    if (_CreateDepthStencilSurfaceEx_pre_ptr._value) { (this->*_CreateDepthStencilSurfaceEx_pre_ptr._value)(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage); }
    HRESULT ret = GetRealObj()->CreateDepthStencilSurfaceEx(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage);
    if ( ppSurface && *ppSurface) { *ppSurface = RealToHooked9( *ppSurface ); }
    if (_CreateDepthStencilSurfaceEx_post_ptr._value) { (this->*_CreateDepthStencilSurfaceEx_post_ptr._value)(ret, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::ResetEx(
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    D3DDISPLAYMODEEX * pFullscreenDisplayMode)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::ResetEx");
    if (_ResetEx_pre_ptr._value) { (this->*_ResetEx_pre_ptr._value)(pPresentationParameters, pFullscreenDisplayMode); }
    HRESULT ret = GetRealObj()->ResetEx(pPresentationParameters, pFullscreenDisplayMode);
    if (_ResetEx_post_ptr._value) { (this->*_ResetEx_post_ptr._value)(ret, pPresentationParameters, pFullscreenDisplayMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9ExHook::GetDisplayModeEx(
    UINT iSwapChain,
    D3DDISPLAYMODEEX* pMode,
    D3DDISPLAYROTATION* pRotation)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9ExHook::GetDisplayModeEx");
    if (_GetDisplayModeEx_pre_ptr._value) { (this->*_GetDisplayModeEx_pre_ptr._value)(iSwapChain, pMode, pRotation); }
    HRESULT ret = GetRealObj()->GetDisplayModeEx(iSwapChain, pMode, pRotation);
    if (_GetDisplayModeEx_post_ptr._value) { (this->*_GetDisplayModeEx_post_ptr._value)(ret, iSwapChain, pMode, pRotation); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9ExHook::GetLastPresentCount(
    UINT* pLastPresentCount)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9ExHook::GetLastPresentCount");
    if (_GetLastPresentCount_pre_ptr._value) { (this->*_GetLastPresentCount_pre_ptr._value)(pLastPresentCount); }
    HRESULT ret = GetRealObj()->GetLastPresentCount(pLastPresentCount);
    if (_GetLastPresentCount_post_ptr._value) { (this->*_GetLastPresentCount_post_ptr._value)(ret, pLastPresentCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9ExHook::GetPresentStats(
    D3DPRESENTSTATS* pPresentationStatistics)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9ExHook::GetPresentStats");
    if (_GetPresentStats_pre_ptr._value) { (this->*_GetPresentStats_pre_ptr._value)(pPresentationStatistics); }
    HRESULT ret = GetRealObj()->GetPresentStats(pPresentationStatistics);
    if (_GetPresentStats_post_ptr._value) { (this->*_GetPresentStats_post_ptr._value)(ret, pPresentationStatistics); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DSwapChain9ExHook::GetDisplayModeEx(
    D3DDISPLAYMODEEX* pMode,
    D3DDISPLAYROTATION* pRotation)
{
    calltrace::AutoTrace trace(L"Direct3DSwapChain9ExHook::GetDisplayModeEx");
    if (_GetDisplayModeEx_pre_ptr._value) { (this->*_GetDisplayModeEx_pre_ptr._value)(pMode, pRotation); }
    HRESULT ret = GetRealObj()->GetDisplayModeEx(pMode, pRotation);
    if (_GetDisplayModeEx_post_ptr._value) { (this->*_GetDisplayModeEx_post_ptr._value)(ret, pMode, pRotation); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3D9ExOverlayExtensionHook::CheckDeviceOverlayType(
    UINT Adapter,
    D3DDEVTYPE DevType,
    UINT OverlayWidth,
    UINT OverlayHeight,
    D3DFORMAT OverlayFormat,
    D3DDISPLAYMODEEX* pDisplayMode,
    D3DDISPLAYROTATION DisplayRotation,
    D3DOVERLAYCAPS* pOverlayCaps)
{
    calltrace::AutoTrace trace(L"Direct3D9ExOverlayExtensionHook::CheckDeviceOverlayType");
    if (_CheckDeviceOverlayType_pre_ptr._value) { (this->*_CheckDeviceOverlayType_pre_ptr._value)(Adapter, DevType, OverlayWidth, OverlayHeight, OverlayFormat, pDisplayMode, DisplayRotation, pOverlayCaps); }
    HRESULT ret = GetRealObj()->CheckDeviceOverlayType(Adapter, DevType, OverlayWidth, OverlayHeight, OverlayFormat, pDisplayMode, DisplayRotation, pOverlayCaps);
    if (_CheckDeviceOverlayType_post_ptr._value) { (this->*_CheckDeviceOverlayType_post_ptr._value)(ret, Adapter, DevType, OverlayWidth, OverlayHeight, OverlayFormat, pDisplayMode, DisplayRotation, pOverlayCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9VideoHook::GetContentProtectionCaps(
    CONST GUID* pCryptoType,
    CONST GUID* pDecodeProfile,
    D3DCONTENTPROTECTIONCAPS* pCaps)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9VideoHook::GetContentProtectionCaps");
    if (_GetContentProtectionCaps_pre_ptr._value) { (this->*_GetContentProtectionCaps_pre_ptr._value)(pCryptoType, pDecodeProfile, pCaps); }
    HRESULT ret = GetRealObj()->GetContentProtectionCaps(pCryptoType, pDecodeProfile, pCaps);
    if (_GetContentProtectionCaps_post_ptr._value) { (this->*_GetContentProtectionCaps_post_ptr._value)(ret, pCryptoType, pDecodeProfile, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9VideoHook::CreateAuthenticatedChannel(
    D3DAUTHENTICATEDCHANNELTYPE ChannelType,
    IDirect3DAuthenticatedChannel9** ppAuthenticatedChannel,
    HANDLE* pChannelHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9VideoHook::CreateAuthenticatedChannel");
    if (_CreateAuthenticatedChannel_pre_ptr._value) { (this->*_CreateAuthenticatedChannel_pre_ptr._value)(ChannelType, ppAuthenticatedChannel, pChannelHandle); }
    HRESULT ret = GetRealObj()->CreateAuthenticatedChannel(ChannelType, ppAuthenticatedChannel, pChannelHandle);
    if ( ppAuthenticatedChannel && *ppAuthenticatedChannel) { *ppAuthenticatedChannel = RealToHooked9( *ppAuthenticatedChannel ); }
    if (_CreateAuthenticatedChannel_post_ptr._value) { (this->*_CreateAuthenticatedChannel_post_ptr._value)(ret, ChannelType, ppAuthenticatedChannel, pChannelHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DDevice9VideoHook::CreateCryptoSession(
    CONST GUID* pCryptoType,
    CONST GUID* pDecodeProfile,
    IDirect3DCryptoSession9** ppCryptoSession,
    HANDLE* pCryptoHandle)
{
    calltrace::AutoTrace trace(L"Direct3DDevice9VideoHook::CreateCryptoSession");
    if (_CreateCryptoSession_pre_ptr._value) { (this->*_CreateCryptoSession_pre_ptr._value)(pCryptoType, pDecodeProfile, ppCryptoSession, pCryptoHandle); }
    HRESULT ret = GetRealObj()->CreateCryptoSession(pCryptoType, pDecodeProfile, ppCryptoSession, pCryptoHandle);
    if ( ppCryptoSession && *ppCryptoSession) { *ppCryptoSession = RealToHooked9( *ppCryptoSession ); }
    if (_CreateCryptoSession_post_ptr._value) { (this->*_CreateCryptoSession_post_ptr._value)(ret, pCryptoType, pDecodeProfile, ppCryptoSession, pCryptoHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DAuthenticatedChannel9Hook::GetCertificateSize(
    UINT* pCertificateSize)
{
    calltrace::AutoTrace trace(L"Direct3DAuthenticatedChannel9Hook::GetCertificateSize");
    if (_GetCertificateSize_pre_ptr._value) { (this->*_GetCertificateSize_pre_ptr._value)(pCertificateSize); }
    HRESULT ret = GetRealObj()->GetCertificateSize(pCertificateSize);
    if (_GetCertificateSize_post_ptr._value) { (this->*_GetCertificateSize_post_ptr._value)(ret, pCertificateSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DAuthenticatedChannel9Hook::GetCertificate(
    UINT CertifacteSize,
    BYTE* ppCertificate)
{
    calltrace::AutoTrace trace(L"Direct3DAuthenticatedChannel9Hook::GetCertificate");
    if (_GetCertificate_pre_ptr._value) { (this->*_GetCertificate_pre_ptr._value)(CertifacteSize, ppCertificate); }
    HRESULT ret = GetRealObj()->GetCertificate(CertifacteSize, ppCertificate);
    if (_GetCertificate_post_ptr._value) { (this->*_GetCertificate_post_ptr._value)(ret, CertifacteSize, ppCertificate); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DAuthenticatedChannel9Hook::NegotiateKeyExchange(
    UINT DataSize,
    VOID* pData)
{
    calltrace::AutoTrace trace(L"Direct3DAuthenticatedChannel9Hook::NegotiateKeyExchange");
    if (_NegotiateKeyExchange_pre_ptr._value) { (this->*_NegotiateKeyExchange_pre_ptr._value)(DataSize, pData); }
    HRESULT ret = GetRealObj()->NegotiateKeyExchange(DataSize, pData);
    if (_NegotiateKeyExchange_post_ptr._value) { (this->*_NegotiateKeyExchange_post_ptr._value)(ret, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DAuthenticatedChannel9Hook::Query(
    UINT InputSize,
    CONST VOID* pInput,
    UINT OutputSize,
    VOID* pOutput)
{
    calltrace::AutoTrace trace(L"Direct3DAuthenticatedChannel9Hook::Query");
    if (_Query_pre_ptr._value) { (this->*_Query_pre_ptr._value)(InputSize, pInput, OutputSize, pOutput); }
    HRESULT ret = GetRealObj()->Query(InputSize, pInput, OutputSize, pOutput);
    if (_Query_post_ptr._value) { (this->*_Query_post_ptr._value)(ret, InputSize, pInput, OutputSize, pOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DAuthenticatedChannel9Hook::Configure(
    UINT InputSize,
    CONST VOID* pInput,
    D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT* pOutput)
{
    calltrace::AutoTrace trace(L"Direct3DAuthenticatedChannel9Hook::Configure");
    if (_Configure_pre_ptr._value) { (this->*_Configure_pre_ptr._value)(InputSize, pInput, pOutput); }
    HRESULT ret = GetRealObj()->Configure(InputSize, pInput, pOutput);
    if (_Configure_post_ptr._value) { (this->*_Configure_post_ptr._value)(ret, InputSize, pInput, pOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::GetCertificateSize(
    UINT* pCertificateSize)
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::GetCertificateSize");
    if (_GetCertificateSize_pre_ptr._value) { (this->*_GetCertificateSize_pre_ptr._value)(pCertificateSize); }
    HRESULT ret = GetRealObj()->GetCertificateSize(pCertificateSize);
    if (_GetCertificateSize_post_ptr._value) { (this->*_GetCertificateSize_post_ptr._value)(ret, pCertificateSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::GetCertificate(
    UINT CertifacteSize,
    BYTE* ppCertificate)
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::GetCertificate");
    if (_GetCertificate_pre_ptr._value) { (this->*_GetCertificate_pre_ptr._value)(CertifacteSize, ppCertificate); }
    HRESULT ret = GetRealObj()->GetCertificate(CertifacteSize, ppCertificate);
    if (_GetCertificate_post_ptr._value) { (this->*_GetCertificate_post_ptr._value)(ret, CertifacteSize, ppCertificate); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::NegotiateKeyExchange(
    UINT DataSize,
    VOID* pData)
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::NegotiateKeyExchange");
    if (_NegotiateKeyExchange_pre_ptr._value) { (this->*_NegotiateKeyExchange_pre_ptr._value)(DataSize, pData); }
    HRESULT ret = GetRealObj()->NegotiateKeyExchange(DataSize, pData);
    if (_NegotiateKeyExchange_post_ptr._value) { (this->*_NegotiateKeyExchange_post_ptr._value)(ret, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::EncryptionBlt(
    IDirect3DSurface9* pSrcSurface,
    IDirect3DSurface9* pDstSurface,
    UINT DstSurfaceSize,
    VOID* pIV)
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::EncryptionBlt");
    if (_EncryptionBlt_pre_ptr._value) { (this->*_EncryptionBlt_pre_ptr._value)(pSrcSurface, pDstSurface, DstSurfaceSize, pIV); }
    HRESULT ret = GetRealObj()->EncryptionBlt(HookedToReal(pSrcSurface), HookedToReal(pDstSurface), DstSurfaceSize, pIV);
    if (_EncryptionBlt_post_ptr._value) { (this->*_EncryptionBlt_post_ptr._value)(ret, pSrcSurface, pDstSurface, DstSurfaceSize, pIV); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::DecryptionBlt(
    IDirect3DSurface9* pSrcSurface,
    IDirect3DSurface9* pDstSurface,
    UINT SrcSurfaceSize,
    D3DENCRYPTED_BLOCK_INFO* pEncryptedBlockInfo,
    VOID* pContentKey,
    VOID* pIV)
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::DecryptionBlt");
    if (_DecryptionBlt_pre_ptr._value) { (this->*_DecryptionBlt_pre_ptr._value)(pSrcSurface, pDstSurface, SrcSurfaceSize, pEncryptedBlockInfo, pContentKey, pIV); }
    HRESULT ret = GetRealObj()->DecryptionBlt(HookedToReal(pSrcSurface), HookedToReal(pDstSurface), SrcSurfaceSize, pEncryptedBlockInfo, pContentKey, pIV);
    if (_DecryptionBlt_post_ptr._value) { (this->*_DecryptionBlt_post_ptr._value)(ret, pSrcSurface, pDstSurface, SrcSurfaceSize, pEncryptedBlockInfo, pContentKey, pIV); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::GetSurfacePitch(
    IDirect3DSurface9* pSrcSurface,
    UINT* pSurfacePitch)
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::GetSurfacePitch");
    if (_GetSurfacePitch_pre_ptr._value) { (this->*_GetSurfacePitch_pre_ptr._value)(pSrcSurface, pSurfacePitch); }
    HRESULT ret = GetRealObj()->GetSurfacePitch(HookedToReal(pSrcSurface), pSurfacePitch);
    if (_GetSurfacePitch_post_ptr._value) { (this->*_GetSurfacePitch_post_ptr._value)(ret, pSrcSurface, pSurfacePitch); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::StartSessionKeyRefresh(
    VOID* pRandomNumber,
    UINT RandomNumberSize)
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::StartSessionKeyRefresh");
    if (_StartSessionKeyRefresh_pre_ptr._value) { (this->*_StartSessionKeyRefresh_pre_ptr._value)(pRandomNumber, RandomNumberSize); }
    HRESULT ret = GetRealObj()->StartSessionKeyRefresh(pRandomNumber, RandomNumberSize);
    if (_StartSessionKeyRefresh_post_ptr._value) { (this->*_StartSessionKeyRefresh_post_ptr._value)(ret, pRandomNumber, RandomNumberSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::FinishSessionKeyRefresh()
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::FinishSessionKeyRefresh");
    if (_FinishSessionKeyRefresh_pre_ptr._value) { (this->*_FinishSessionKeyRefresh_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->FinishSessionKeyRefresh();
    if (_FinishSessionKeyRefresh_post_ptr._value) { (this->*_FinishSessionKeyRefresh_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Direct3DCryptoSession9Hook::GetEncryptionBltKey(
    VOID* pReadbackKey,
    UINT KeySize)
{
    calltrace::AutoTrace trace(L"Direct3DCryptoSession9Hook::GetEncryptionBltKey");
    if (_GetEncryptionBltKey_pre_ptr._value) { (this->*_GetEncryptionBltKey_pre_ptr._value)(pReadbackKey, KeySize); }
    HRESULT ret = GetRealObj()->GetEncryptionBltKey(pReadbackKey, KeySize);
    if (_GetEncryptionBltKey_post_ptr._value) { (this->*_GetEncryptionBltKey_post_ptr._value)(ret, pReadbackKey, KeySize); }
    return ret;
}

