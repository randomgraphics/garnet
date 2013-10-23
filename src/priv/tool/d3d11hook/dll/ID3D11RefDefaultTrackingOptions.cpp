// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11RefDefaultTrackingOptionsHook::SetTrackingOptions(
    UINT ResourceTypeFlags,
    UINT Options)
{
    calltrace::AutoTrace trace(L"D3D11RefDefaultTrackingOptionsHook::SetTrackingOptions");
    if (_SetTrackingOptions_pre_ptr._value) { (this->*_SetTrackingOptions_pre_ptr._value)(ResourceTypeFlags, Options); }
    HRESULT ret = GetRealObj()->SetTrackingOptions(ResourceTypeFlags, Options);
    if (_SetTrackingOptions_post_ptr._value) { (this->*_SetTrackingOptions_post_ptr._value)(ret, ResourceTypeFlags, Options); }
    return ret;
}

