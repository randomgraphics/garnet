// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DXGIDisplayControlHook::IsStereoEnabled()
{
    calltrace::AutoTrace trace(L"DXGIDisplayControlHook::IsStereoEnabled");
    if (_IsStereoEnabled_pre_ptr._value) { (this->*_IsStereoEnabled_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsStereoEnabled();
    if (_IsStereoEnabled_post_ptr._value) { (this->*_IsStereoEnabled_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIDisplayControlHook::SetStereoEnabled(
    BOOL enabled)
{
    calltrace::AutoTrace trace(L"DXGIDisplayControlHook::SetStereoEnabled");
    if (_SetStereoEnabled_pre_ptr._value) { (this->*_SetStereoEnabled_pre_ptr._value)(enabled); }
    GetRealObj()->SetStereoEnabled(enabled);
    if (_SetStereoEnabled_post_ptr._value) { (this->*_SetStereoEnabled_post_ptr._value)(enabled); }
}

