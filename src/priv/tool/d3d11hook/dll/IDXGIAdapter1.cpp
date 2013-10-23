// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapter1Hook::GetDesc1(
    _Out_  DXGI_ADAPTER_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIAdapter1Hook::GetDesc1");
    if (_GetDesc1_pre_ptr._value) { (this->*_GetDesc1_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc1(pDesc);
    if (_GetDesc1_post_ptr._value) { (this->*_GetDesc1_post_ptr._value)(ret, pDesc); }
    return ret;
}

