// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DXGIFactory2Hook::IsWindowedStereoEnabled()
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::IsWindowedStereoEnabled");
    if (_IsWindowedStereoEnabled_pre_ptr._value) { (this->*_IsWindowedStereoEnabled_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsWindowedStereoEnabled();
    if (_IsWindowedStereoEnabled_post_ptr._value) { (this->*_IsWindowedStereoEnabled_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::CreateSwapChainForHwnd(
    _In_  IUnknown * pDevice,
    _In_  HWND hWnd,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pFullscreenDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Out_  IDXGISwapChain1 ** ppSwapChain)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::CreateSwapChainForHwnd");
    if (_CreateSwapChainForHwnd_pre_ptr._value) { (this->*_CreateSwapChainForHwnd_pre_ptr._value)(pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateSwapChainForHwnd(pDevice, hWnd, pDesc, pFullscreenDesc, HookedToReal(pRestrictToOutput), ppSwapChain);
    if (ppSwapChain) *ppSwapChain = RealToHooked( *ppSwapChain );
    if (_CreateSwapChainForHwnd_post_ptr._value) { (this->*_CreateSwapChainForHwnd_post_ptr._value)(ret, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::CreateSwapChainForCoreWindow(
    _In_  IUnknown * pDevice,
    _In_  IUnknown * pWindow,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Out_  IDXGISwapChain1 ** ppSwapChain)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::CreateSwapChainForCoreWindow");
    if (_CreateSwapChainForCoreWindow_pre_ptr._value) { (this->*_CreateSwapChainForCoreWindow_pre_ptr._value)(pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateSwapChainForCoreWindow(pDevice, pWindow, pDesc, HookedToReal(pRestrictToOutput), ppSwapChain);
    if (ppSwapChain) *ppSwapChain = RealToHooked( *ppSwapChain );
    if (_CreateSwapChainForCoreWindow_post_ptr._value) { (this->*_CreateSwapChainForCoreWindow_post_ptr._value)(ret, pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::GetSharedResourceAdapterLuid(
    _In_  HANDLE hResource,
    _Out_  LUID * pLuid)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::GetSharedResourceAdapterLuid");
    if (_GetSharedResourceAdapterLuid_pre_ptr._value) { (this->*_GetSharedResourceAdapterLuid_pre_ptr._value)(hResource, pLuid); }
    HRESULT ret = GetRealObj()->GetSharedResourceAdapterLuid(hResource, pLuid);
    if (_GetSharedResourceAdapterLuid_post_ptr._value) { (this->*_GetSharedResourceAdapterLuid_post_ptr._value)(ret, hResource, pLuid); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::RegisterStereoStatusWindow(
    _In_  HWND WindowHandle,
    _In_  UINT wMsg,
    _Out_  DWORD * pdwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::RegisterStereoStatusWindow");
    if (_RegisterStereoStatusWindow_pre_ptr._value) { (this->*_RegisterStereoStatusWindow_pre_ptr._value)(WindowHandle, wMsg, pdwCookie); }
    HRESULT ret = GetRealObj()->RegisterStereoStatusWindow(WindowHandle, wMsg, pdwCookie);
    if (_RegisterStereoStatusWindow_post_ptr._value) { (this->*_RegisterStereoStatusWindow_post_ptr._value)(ret, WindowHandle, wMsg, pdwCookie); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::RegisterStereoStatusEvent(
    _In_  HANDLE hEvent,
    _Out_  DWORD * pdwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::RegisterStereoStatusEvent");
    if (_RegisterStereoStatusEvent_pre_ptr._value) { (this->*_RegisterStereoStatusEvent_pre_ptr._value)(hEvent, pdwCookie); }
    HRESULT ret = GetRealObj()->RegisterStereoStatusEvent(hEvent, pdwCookie);
    if (_RegisterStereoStatusEvent_post_ptr._value) { (this->*_RegisterStereoStatusEvent_post_ptr._value)(ret, hEvent, pdwCookie); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIFactory2Hook::UnregisterStereoStatus(
    _In_  DWORD dwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::UnregisterStereoStatus");
    if (_UnregisterStereoStatus_pre_ptr._value) { (this->*_UnregisterStereoStatus_pre_ptr._value)(dwCookie); }
    GetRealObj()->UnregisterStereoStatus(dwCookie);
    if (_UnregisterStereoStatus_post_ptr._value) { (this->*_UnregisterStereoStatus_post_ptr._value)(dwCookie); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::RegisterOcclusionStatusWindow(
    _In_  HWND WindowHandle,
    _In_  UINT wMsg,
    _Out_  DWORD * pdwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::RegisterOcclusionStatusWindow");
    if (_RegisterOcclusionStatusWindow_pre_ptr._value) { (this->*_RegisterOcclusionStatusWindow_pre_ptr._value)(WindowHandle, wMsg, pdwCookie); }
    HRESULT ret = GetRealObj()->RegisterOcclusionStatusWindow(WindowHandle, wMsg, pdwCookie);
    if (_RegisterOcclusionStatusWindow_post_ptr._value) { (this->*_RegisterOcclusionStatusWindow_post_ptr._value)(ret, WindowHandle, wMsg, pdwCookie); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::RegisterOcclusionStatusEvent(
    _In_  HANDLE hEvent,
    _Out_  DWORD * pdwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::RegisterOcclusionStatusEvent");
    if (_RegisterOcclusionStatusEvent_pre_ptr._value) { (this->*_RegisterOcclusionStatusEvent_pre_ptr._value)(hEvent, pdwCookie); }
    HRESULT ret = GetRealObj()->RegisterOcclusionStatusEvent(hEvent, pdwCookie);
    if (_RegisterOcclusionStatusEvent_post_ptr._value) { (this->*_RegisterOcclusionStatusEvent_post_ptr._value)(ret, hEvent, pdwCookie); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIFactory2Hook::UnregisterOcclusionStatus(
    _In_  DWORD dwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::UnregisterOcclusionStatus");
    if (_UnregisterOcclusionStatus_pre_ptr._value) { (this->*_UnregisterOcclusionStatus_pre_ptr._value)(dwCookie); }
    GetRealObj()->UnregisterOcclusionStatus(dwCookie);
    if (_UnregisterOcclusionStatus_post_ptr._value) { (this->*_UnregisterOcclusionStatus_post_ptr._value)(dwCookie); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::CreateSwapChainForComposition(
    _In_  IUnknown * pDevice,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Outptr_  IDXGISwapChain1 ** ppSwapChain)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::CreateSwapChainForComposition");
    if (_CreateSwapChainForComposition_pre_ptr._value) { (this->*_CreateSwapChainForComposition_pre_ptr._value)(pDevice, pDesc, pRestrictToOutput, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateSwapChainForComposition(pDevice, pDesc, HookedToReal(pRestrictToOutput), HookedToReal(ppSwapChain));
    if (_CreateSwapChainForComposition_post_ptr._value) { (this->*_CreateSwapChainForComposition_post_ptr._value)(ret, pDevice, pDesc, pRestrictToOutput, ppSwapChain); }
    return ret;
}

