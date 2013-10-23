// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11RefTrackingOptionsHook::SetTrackingOptions(
    UINT uOptions)
{
    calltrace::AutoTrace trace(L"D3D11RefTrackingOptionsHook::SetTrackingOptions");
    if (_SetTrackingOptions_pre_ptr._value) { (this->*_SetTrackingOptions_pre_ptr._value)(uOptions); }
    HRESULT ret = GetRealObj()->SetTrackingOptions(uOptions);
    if (_SetTrackingOptions_post_ptr._value) { (this->*_SetTrackingOptions_post_ptr._value)(ret, uOptions); }
    return ret;
}

