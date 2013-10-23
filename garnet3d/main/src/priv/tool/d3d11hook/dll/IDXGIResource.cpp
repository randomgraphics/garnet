// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResourceHook::GetSharedHandle(
    _Out_  HANDLE * pSharedHandle)
{
    calltrace::AutoTrace trace(L"DXGIResourceHook::GetSharedHandle");
    if (_GetSharedHandle_pre_ptr._value) { (this->*_GetSharedHandle_pre_ptr._value)(pSharedHandle); }
    HRESULT ret = GetRealObj()->GetSharedHandle(pSharedHandle);
    if (_GetSharedHandle_post_ptr._value) { (this->*_GetSharedHandle_post_ptr._value)(ret, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResourceHook::GetUsage(
    _Out_  DXGI_USAGE * pUsage)
{
    calltrace::AutoTrace trace(L"DXGIResourceHook::GetUsage");
    if (_GetUsage_pre_ptr._value) { (this->*_GetUsage_pre_ptr._value)(pUsage); }
    HRESULT ret = GetRealObj()->GetUsage(pUsage);
    if (_GetUsage_post_ptr._value) { (this->*_GetUsage_post_ptr._value)(ret, pUsage); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResourceHook::SetEvictionPriority(
    /* [in] */ UINT EvictionPriority)
{
    calltrace::AutoTrace trace(L"DXGIResourceHook::SetEvictionPriority");
    if (_SetEvictionPriority_pre_ptr._value) { (this->*_SetEvictionPriority_pre_ptr._value)(EvictionPriority); }
    HRESULT ret = GetRealObj()->SetEvictionPriority(EvictionPriority);
    if (_SetEvictionPriority_post_ptr._value) { (this->*_SetEvictionPriority_post_ptr._value)(ret, EvictionPriority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResourceHook::GetEvictionPriority(
    _Out_  UINT * pEvictionPriority)
{
    calltrace::AutoTrace trace(L"DXGIResourceHook::GetEvictionPriority");
    if (_GetEvictionPriority_pre_ptr._value) { (this->*_GetEvictionPriority_pre_ptr._value)(pEvictionPriority); }
    HRESULT ret = GetRealObj()->GetEvictionPriority(pEvictionPriority);
    if (_GetEvictionPriority_post_ptr._value) { (this->*_GetEvictionPriority_post_ptr._value)(ret, pEvictionPriority); }
    return ret;
}

