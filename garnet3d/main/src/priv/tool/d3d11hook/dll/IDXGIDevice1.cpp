// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice1Hook::SetMaximumFrameLatency(
    /* [in] */ UINT MaxLatency)
{
    calltrace::AutoTrace trace(L"DXGIDevice1Hook::SetMaximumFrameLatency");
    if (_SetMaximumFrameLatency_pre_ptr._value) { (this->*_SetMaximumFrameLatency_pre_ptr._value)(MaxLatency); }
    HRESULT ret = GetRealObj()->SetMaximumFrameLatency(MaxLatency);
    if (_SetMaximumFrameLatency_post_ptr._value) { (this->*_SetMaximumFrameLatency_post_ptr._value)(ret, MaxLatency); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice1Hook::GetMaximumFrameLatency(
    _Out_  UINT * pMaxLatency)
{
    calltrace::AutoTrace trace(L"DXGIDevice1Hook::GetMaximumFrameLatency");
    if (_GetMaximumFrameLatency_pre_ptr._value) { (this->*_GetMaximumFrameLatency_pre_ptr._value)(pMaxLatency); }
    HRESULT ret = GetRealObj()->GetMaximumFrameLatency(pMaxLatency);
    if (_GetMaximumFrameLatency_post_ptr._value) { (this->*_GetMaximumFrameLatency_post_ptr._value)(ret, pMaxLatency); }
    return ret;
}

