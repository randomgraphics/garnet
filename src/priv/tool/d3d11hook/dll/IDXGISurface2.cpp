// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurface2Hook::GetResource(
    _In_  REFIID riid,
    _Out_  void ** ppParentResource,
    _Out_  UINT * pSubresourceIndex)
{
    calltrace::AutoTrace trace(L"DXGISurface2Hook::GetResource");
    if (_GetResource_pre_ptr._value) { (this->*_GetResource_pre_ptr._value)(riid, ppParentResource, pSubresourceIndex); }
    HRESULT ret = GetRealObj()->GetResource(riid, ppParentResource, pSubresourceIndex);
    if (_GetResource_post_ptr._value) { (this->*_GetResource_post_ptr._value)(ret, riid, ppParentResource, pSubresourceIndex); }
    return ret;
}

