// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory1Hook::EnumAdapters1(
    /* [in] */ UINT Adapter,
    _Out_  IDXGIAdapter1 ** ppAdapter)
{
    if (_EnumAdapters1_pre_ptr._value) { (this->*_EnumAdapters1_pre_ptr._value)(Adapter, ppAdapter); }
    HRESULT ret = GetRealObj()->EnumAdapters1(Adapter, ppAdapter);
    if (ppAdapter) *ppAdapter = RealToHooked( *ppAdapter );
    if (_EnumAdapters1_post_ptr._value) { (this->*_EnumAdapters1_post_ptr._value)(ret, Adapter, ppAdapter); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DXGIFactory1Hook::IsCurrent()
{
    if (_IsCurrent_pre_ptr._value) { (this->*_IsCurrent_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsCurrent();
    if (_IsCurrent_post_ptr._value) { (this->*_IsCurrent_post_ptr._value)(ret); }
    return ret;
}

