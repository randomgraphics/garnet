// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIKeyedMutexHook::AcquireSync(
    /* [in] */ UINT64 Key,
    /* [in] */ DWORD dwMilliseconds)
{
    calltrace::AutoTrace trace(L"DXGIKeyedMutexHook::AcquireSync");
    if (_AcquireSync_pre_ptr._value) { (this->*_AcquireSync_pre_ptr._value)(Key, dwMilliseconds); }
    HRESULT ret = GetRealObj()->AcquireSync(Key, dwMilliseconds);
    if (_AcquireSync_post_ptr._value) { (this->*_AcquireSync_post_ptr._value)(ret, Key, dwMilliseconds); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIKeyedMutexHook::ReleaseSync(
    /* [in] */ UINT64 Key)
{
    calltrace::AutoTrace trace(L"DXGIKeyedMutexHook::ReleaseSync");
    if (_ReleaseSync_pre_ptr._value) { (this->*_ReleaseSync_pre_ptr._value)(Key); }
    HRESULT ret = GetRealObj()->ReleaseSync(Key);
    if (_ReleaseSync_post_ptr._value) { (this->*_ReleaseSync_post_ptr._value)(ret, Key); }
    return ret;
}

