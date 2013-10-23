// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11AsynchronousHook::GetDataSize()
{
    calltrace::AutoTrace trace(L"D3D11AsynchronousHook::GetDataSize");
    if (_GetDataSize_pre_ptr._value) { (this->*_GetDataSize_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetDataSize();
    if (_GetDataSize_post_ptr._value) { (this->*_GetDataSize_post_ptr._value)(ret); }
    return ret;
}

