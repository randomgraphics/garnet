// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapter2Hook::GetDesc2(
    _Out_  DXGI_ADAPTER_DESC2 * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIAdapter2Hook::GetDesc2");
    if (_GetDesc2_pre_ptr._value) { (this->*_GetDesc2_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc2(pDesc);
    if (_GetDesc2_post_ptr._value) { (this->*_GetDesc2_post_ptr._value)(ret, pDesc); }
    return ret;
}

