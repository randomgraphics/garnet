// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11TracingDeviceHook::SetShaderTrackingOptionsByType(
    _In_  UINT ResourceTypeFlags,
    _In_  UINT Options)
{
    calltrace::AutoTrace trace(L"D3D11TracingDeviceHook::SetShaderTrackingOptionsByType");
    if (_SetShaderTrackingOptionsByType_pre_ptr._value) { (this->*_SetShaderTrackingOptionsByType_pre_ptr._value)(ResourceTypeFlags, Options); }
    HRESULT ret = GetRealObj()->SetShaderTrackingOptionsByType(ResourceTypeFlags, Options);
    if (_SetShaderTrackingOptionsByType_post_ptr._value) { (this->*_SetShaderTrackingOptionsByType_post_ptr._value)(ret, ResourceTypeFlags, Options); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11TracingDeviceHook::SetShaderTrackingOptions(
    _In_  IUnknown * pShader,
    _In_  UINT Options)
{
    calltrace::AutoTrace trace(L"D3D11TracingDeviceHook::SetShaderTrackingOptions");
    if (_SetShaderTrackingOptions_pre_ptr._value) { (this->*_SetShaderTrackingOptions_pre_ptr._value)(pShader, Options); }
    HRESULT ret = GetRealObj()->SetShaderTrackingOptions(pShader, Options);
    if (_SetShaderTrackingOptions_post_ptr._value) { (this->*_SetShaderTrackingOptions_post_ptr._value)(ret, pShader, Options); }
    return ret;
}

