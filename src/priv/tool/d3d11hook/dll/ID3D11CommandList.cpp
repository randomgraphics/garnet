// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11CommandListHook::GetContextFlags()
{
    calltrace::AutoTrace trace(L"D3D11CommandListHook::GetContextFlags");
    if (_GetContextFlags_pre_ptr._value) { (this->*_GetContextFlags_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetContextFlags();
    if (_GetContextFlags_post_ptr._value) { (this->*_GetContextFlags_post_ptr._value)(ret); }
    return ret;
}

