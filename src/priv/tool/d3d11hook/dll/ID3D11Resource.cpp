// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ResourceHook::GetType(
    _Out_  D3D11_RESOURCE_DIMENSION * pResourceDimension)
{
    calltrace::AutoTrace trace(L"D3D11ResourceHook::GetType");
    if (_GetType_pre_ptr._value) { (this->*_GetType_pre_ptr._value)(pResourceDimension); }
    GetRealObj()->GetType(pResourceDimension);
    if (_GetType_post_ptr._value) { (this->*_GetType_post_ptr._value)(pResourceDimension); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ResourceHook::SetEvictionPriority(
    _In_  UINT EvictionPriority)
{
    calltrace::AutoTrace trace(L"D3D11ResourceHook::SetEvictionPriority");
    if (_SetEvictionPriority_pre_ptr._value) { (this->*_SetEvictionPriority_pre_ptr._value)(EvictionPriority); }
    GetRealObj()->SetEvictionPriority(EvictionPriority);
    if (_SetEvictionPriority_post_ptr._value) { (this->*_SetEvictionPriority_post_ptr._value)(EvictionPriority); }
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11ResourceHook::GetEvictionPriority()
{
    calltrace::AutoTrace trace(L"D3D11ResourceHook::GetEvictionPriority");
    if (_GetEvictionPriority_pre_ptr._value) { (this->*_GetEvictionPriority_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetEvictionPriority();
    if (_GetEvictionPriority_post_ptr._value) { (this->*_GetEvictionPriority_post_ptr._value)(ret); }
    return ret;
}

