// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3D11SwitchToRefHook::SetUseRef(
    BOOL UseRef)
{
    calltrace::AutoTrace trace(L"D3D11SwitchToRefHook::SetUseRef");
    if (_SetUseRef_pre_ptr._value) { (this->*_SetUseRef_pre_ptr._value)(UseRef); }
    BOOL ret = GetRealObj()->SetUseRef(UseRef);
    if (_SetUseRef_post_ptr._value) { (this->*_SetUseRef_post_ptr._value)(ret, UseRef); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3D11SwitchToRefHook::GetUseRef()
{
    calltrace::AutoTrace trace(L"D3D11SwitchToRefHook::GetUseRef");
    if (_GetUseRef_pre_ptr._value) { (this->*_GetUseRef_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->GetUseRef();
    if (_GetUseRef_post_ptr._value) { (this->*_GetUseRef_post_ptr._value)(ret); }
    return ret;
}

