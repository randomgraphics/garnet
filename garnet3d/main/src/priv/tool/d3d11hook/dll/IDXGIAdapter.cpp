// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::EnumOutputs(
    /* [in] */ UINT Output,
    _Out_  IDXGIOutput ** ppOutput)
{
    if (_EnumOutputs_pre_ptr._value) { (this->*_EnumOutputs_pre_ptr._value)(Output, ppOutput); }
    HRESULT ret = GetRealObj()->EnumOutputs(Output, ppOutput);
    if (ppOutput) *ppOutput = RealToHooked( *ppOutput );
    if (_EnumOutputs_post_ptr._value) { (this->*_EnumOutputs_post_ptr._value)(ret, Output, ppOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::GetDesc(
    _Out_  DXGI_ADAPTER_DESC * pDesc)
{
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::CheckInterfaceSupport(
    _In_  REFGUID InterfaceName,
    _Out_  LARGE_INTEGER * pUMDVersion)
{
    if (_CheckInterfaceSupport_pre_ptr._value) { (this->*_CheckInterfaceSupport_pre_ptr._value)(InterfaceName, pUMDVersion); }
    HRESULT ret = GetRealObj()->CheckInterfaceSupport(InterfaceName, pUMDVersion);
    if (_CheckInterfaceSupport_post_ptr._value) { (this->*_CheckInterfaceSupport_post_ptr._value)(ret, InterfaceName, pUMDVersion); }
    return ret;
}

