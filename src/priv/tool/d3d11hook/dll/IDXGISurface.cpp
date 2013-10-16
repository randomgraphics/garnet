// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurfaceHook::GetDesc(
    _Out_  DXGI_SURFACE_DESC * pDesc)
{
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurfaceHook::Map(
    _Out_  DXGI_MAPPED_RECT * pLockedRect,
    /* [in] */ UINT MapFlags)
{
    if (_Map_pre_ptr._value) { (this->*_Map_pre_ptr._value)(pLockedRect, MapFlags); }
    HRESULT ret = GetRealObj()->Map(pLockedRect, MapFlags);
    if (_Map_post_ptr._value) { (this->*_Map_post_ptr._value)(ret, pLockedRect, MapFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurfaceHook::Unmap()
{
    if (_Unmap_pre_ptr._value) { (this->*_Unmap_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Unmap();
    if (_Unmap_post_ptr._value) { (this->*_Unmap_post_ptr._value)(ret); }
    return ret;
}

