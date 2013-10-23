// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResource1Hook::CreateSubresourceSurface(
    UINT index,
    _Out_  IDXGISurface2 ** ppSurface)
{
    calltrace::AutoTrace trace(L"DXGIResource1Hook::CreateSubresourceSurface");
    if (_CreateSubresourceSurface_pre_ptr._value) { (this->*_CreateSubresourceSurface_pre_ptr._value)(index, ppSurface); }
    HRESULT ret = GetRealObj()->CreateSubresourceSurface(index, ppSurface);
    if (ppSurface) *ppSurface = RealToHooked( *ppSurface );
    if (_CreateSubresourceSurface_post_ptr._value) { (this->*_CreateSubresourceSurface_post_ptr._value)(ret, index, ppSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResource1Hook::CreateSharedHandle(
    _In_opt_  const SECURITY_ATTRIBUTES * pAttributes,
    _In_  DWORD dwAccess,
    _In_opt_  LPCWSTR lpName,
    _Out_  HANDLE * pHandle)
{
    calltrace::AutoTrace trace(L"DXGIResource1Hook::CreateSharedHandle");
    if (_CreateSharedHandle_pre_ptr._value) { (this->*_CreateSharedHandle_pre_ptr._value)(pAttributes, dwAccess, lpName, pHandle); }
    HRESULT ret = GetRealObj()->CreateSharedHandle(pAttributes, dwAccess, lpName, pHandle);
    if (_CreateSharedHandle_post_ptr._value) { (this->*_CreateSharedHandle_post_ptr._value)(ret, pAttributes, dwAccess, lpName, pHandle); }
    return ret;
}

