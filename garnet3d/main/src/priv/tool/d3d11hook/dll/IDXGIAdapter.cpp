// script generated file. DO NOT edit.
#include "pch.h"
#include "d3d11hook.h"
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::EnumOutputs(
    /* [in] */ UINT Output,
    IDXGIOutput ** ppOutput)
{
    if (_EnumOutputs_pre_ptr._value) { (this->*_EnumOutputs_pre_ptr._value)(Output, ppOutput); }
    HRESULT ret = GetRealObj()->EnumOutputs(Output, ppOutput);
    if (_EnumOutputs_post_ptr._value) { (this->*_EnumOutputs_post_ptr._value)(ret, Output, ppOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::GetDesc(
    DXGI_ADAPTER_DESC * pDesc)
{
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::CheckInterfaceSupport(
    REFGUID InterfaceName,
    LARGE_INTEGER * pUMDVersion)
{
    if (_CheckInterfaceSupport_pre_ptr._value) { (this->*_CheckInterfaceSupport_pre_ptr._value)(InterfaceName, pUMDVersion); }
    HRESULT ret = GetRealObj()->CheckInterfaceSupport(InterfaceName, pUMDVersion);
    if (_CheckInterfaceSupport_post_ptr._value) { (this->*_CheckInterfaceSupport_post_ptr._value)(ret, InterfaceName, pUMDVersion); }
    return ret;
}

