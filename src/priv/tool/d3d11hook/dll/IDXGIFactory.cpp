// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::EnumAdapters(
    /* [in] */ UINT Adapter,
    _Out_  IDXGIAdapter ** ppAdapter)
{
    if (_EnumAdapters_pre_ptr._value) { (this->*_EnumAdapters_pre_ptr._value)(Adapter, ppAdapter); }
    HRESULT ret = GetRealObj()->EnumAdapters(Adapter, ppAdapter);
    if (ppAdapter) *ppAdapter = RealToHooked( *ppAdapter );
    if (_EnumAdapters_post_ptr._value) { (this->*_EnumAdapters_post_ptr._value)(ret, Adapter, ppAdapter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::MakeWindowAssociation(
    HWND WindowHandle,
    UINT Flags)
{
    if (_MakeWindowAssociation_pre_ptr._value) { (this->*_MakeWindowAssociation_pre_ptr._value)(WindowHandle, Flags); }
    HRESULT ret = GetRealObj()->MakeWindowAssociation(WindowHandle, Flags);
    if (_MakeWindowAssociation_post_ptr._value) { (this->*_MakeWindowAssociation_post_ptr._value)(ret, WindowHandle, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::GetWindowAssociation(
    _Out_  HWND * pWindowHandle)
{
    if (_GetWindowAssociation_pre_ptr._value) { (this->*_GetWindowAssociation_pre_ptr._value)(pWindowHandle); }
    HRESULT ret = GetRealObj()->GetWindowAssociation(pWindowHandle);
    if (_GetWindowAssociation_post_ptr._value) { (this->*_GetWindowAssociation_post_ptr._value)(ret, pWindowHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::CreateSwapChain(
    _In_  IUnknown * pDevice,
    _In_  DXGI_SWAP_CHAIN_DESC * pDesc,
    _Out_  IDXGISwapChain ** ppSwapChain)
{
    if (_CreateSwapChain_pre_ptr._value) { (this->*_CreateSwapChain_pre_ptr._value)(pDevice, pDesc, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateSwapChain(pDevice, pDesc, ppSwapChain);
    if (ppSwapChain) *ppSwapChain = RealToHooked( *ppSwapChain );
    if (_CreateSwapChain_post_ptr._value) { (this->*_CreateSwapChain_post_ptr._value)(ret, pDevice, pDesc, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::CreateSoftwareAdapter(
    /* [in] */ HMODULE Module,
    _Out_  IDXGIAdapter ** ppAdapter)
{
    if (_CreateSoftwareAdapter_pre_ptr._value) { (this->*_CreateSoftwareAdapter_pre_ptr._value)(Module, ppAdapter); }
    HRESULT ret = GetRealObj()->CreateSoftwareAdapter(Module, ppAdapter);
    if (ppAdapter) *ppAdapter = RealToHooked( *ppAdapter );
    if (_CreateSoftwareAdapter_post_ptr._value) { (this->*_CreateSoftwareAdapter_post_ptr._value)(ret, Module, ppAdapter); }
    return ret;
}

