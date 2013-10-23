// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetDesc1(
    _Out_  DXGI_SWAP_CHAIN_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetDesc1");
    if (_GetDesc1_pre_ptr._value) { (this->*_GetDesc1_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc1(pDesc);
    if (_GetDesc1_post_ptr._value) { (this->*_GetDesc1_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetFullscreenDesc(
    _Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetFullscreenDesc");
    if (_GetFullscreenDesc_pre_ptr._value) { (this->*_GetFullscreenDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetFullscreenDesc(pDesc);
    if (_GetFullscreenDesc_post_ptr._value) { (this->*_GetFullscreenDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetHwnd(
    _Out_  HWND * pHwnd)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetHwnd");
    if (_GetHwnd_pre_ptr._value) { (this->*_GetHwnd_pre_ptr._value)(pHwnd); }
    HRESULT ret = GetRealObj()->GetHwnd(pHwnd);
    if (_GetHwnd_post_ptr._value) { (this->*_GetHwnd_post_ptr._value)(ret, pHwnd); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetCoreWindow(
    _In_  REFIID refiid,
    _Out_  void ** ppUnk)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetCoreWindow");
    if (_GetCoreWindow_pre_ptr._value) { (this->*_GetCoreWindow_pre_ptr._value)(refiid, ppUnk); }
    HRESULT ret = GetRealObj()->GetCoreWindow(refiid, ppUnk);
    if (_GetCoreWindow_post_ptr._value) { (this->*_GetCoreWindow_post_ptr._value)(ret, refiid, ppUnk); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::Present1(
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT PresentFlags,
    _In_  const DXGI_PRESENT_PARAMETERS * pPresentParameters)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::Present1");
    if (_Present1_pre_ptr._value) { (this->*_Present1_pre_ptr._value)(SyncInterval, PresentFlags, pPresentParameters); }
    HRESULT ret = GetRealObj()->Present1(SyncInterval, PresentFlags, pPresentParameters);
    if (_Present1_post_ptr._value) { (this->*_Present1_post_ptr._value)(ret, SyncInterval, PresentFlags, pPresentParameters); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DXGISwapChain1Hook::IsTemporaryMonoSupported()
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::IsTemporaryMonoSupported");
    if (_IsTemporaryMonoSupported_pre_ptr._value) { (this->*_IsTemporaryMonoSupported_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsTemporaryMonoSupported();
    if (_IsTemporaryMonoSupported_post_ptr._value) { (this->*_IsTemporaryMonoSupported_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetRestrictToOutput(
    _Out_  IDXGIOutput ** ppRestrictToOutput)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetRestrictToOutput");
    if (_GetRestrictToOutput_pre_ptr._value) { (this->*_GetRestrictToOutput_pre_ptr._value)(ppRestrictToOutput); }
    HRESULT ret = GetRealObj()->GetRestrictToOutput(ppRestrictToOutput);
    if (ppRestrictToOutput) *ppRestrictToOutput = RealToHooked( *ppRestrictToOutput );
    if (_GetRestrictToOutput_post_ptr._value) { (this->*_GetRestrictToOutput_post_ptr._value)(ret, ppRestrictToOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::SetBackgroundColor(
    _In_  const DXGI_RGBA * pColor)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::SetBackgroundColor");
    if (_SetBackgroundColor_pre_ptr._value) { (this->*_SetBackgroundColor_pre_ptr._value)(pColor); }
    HRESULT ret = GetRealObj()->SetBackgroundColor(pColor);
    if (_SetBackgroundColor_post_ptr._value) { (this->*_SetBackgroundColor_post_ptr._value)(ret, pColor); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetBackgroundColor(
    _Out_  DXGI_RGBA * pColor)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetBackgroundColor");
    if (_GetBackgroundColor_pre_ptr._value) { (this->*_GetBackgroundColor_pre_ptr._value)(pColor); }
    HRESULT ret = GetRealObj()->GetBackgroundColor(pColor);
    if (_GetBackgroundColor_post_ptr._value) { (this->*_GetBackgroundColor_post_ptr._value)(ret, pColor); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::SetRotation(
    _In_  DXGI_MODE_ROTATION Rotation)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::SetRotation");
    if (_SetRotation_pre_ptr._value) { (this->*_SetRotation_pre_ptr._value)(Rotation); }
    HRESULT ret = GetRealObj()->SetRotation(Rotation);
    if (_SetRotation_post_ptr._value) { (this->*_SetRotation_post_ptr._value)(ret, Rotation); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetRotation(
    _Out_  DXGI_MODE_ROTATION * pRotation)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetRotation");
    if (_GetRotation_pre_ptr._value) { (this->*_GetRotation_pre_ptr._value)(pRotation); }
    HRESULT ret = GetRealObj()->GetRotation(pRotation);
    if (_GetRotation_post_ptr._value) { (this->*_GetRotation_post_ptr._value)(ret, pRotation); }
    return ret;
}

