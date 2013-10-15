// script generated file. DO NOT edit.
#include "pch.h"
#include "d3d11hook.h"
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::GetAdapter(
    _Out_  IDXGIAdapter ** pAdapter)
{
    if (_GetAdapter_pre_ptr._value) { (this->*_GetAdapter_pre_ptr._value)(pAdapter); }
    HRESULT ret = GetRealObj()->GetAdapter(pAdapter);
    if (_GetAdapter_post_ptr._value) { (this->*_GetAdapter_post_ptr._value)(ret, pAdapter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::CreateSurface(
    _In_  const DXGI_SURFACE_DESC * pDesc,
    /* [in] */ UINT NumSurfaces,
    /* [in] */ DXGI_USAGE Usage,
    _In_opt_  const DXGI_SHARED_RESOURCE * pSharedResource,
    _Out_  IDXGISurface ** ppSurface)
{
    if (_CreateSurface_pre_ptr._value) { (this->*_CreateSurface_pre_ptr._value)(pDesc, NumSurfaces, Usage, pSharedResource, ppSurface); }
    HRESULT ret = GetRealObj()->CreateSurface(pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
    if (_CreateSurface_post_ptr._value) { (this->*_CreateSurface_post_ptr._value)(ret, pDesc, NumSurfaces, Usage, pSharedResource, ppSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::QueryResourceResidency(
    _In_reads_(NumResources)  IUnknown *const * ppResources,
    _Out_writes_(NumResources)  DXGI_RESIDENCY * pResidencyStatus,
    /* [in] */ UINT NumResources)
{
    if (_QueryResourceResidency_pre_ptr._value) { (this->*_QueryResourceResidency_pre_ptr._value)(ppResources, pResidencyStatus, NumResources); }
    HRESULT ret = GetRealObj()->QueryResourceResidency(ppResources, pResidencyStatus, NumResources);
    if (_QueryResourceResidency_post_ptr._value) { (this->*_QueryResourceResidency_post_ptr._value)(ret, ppResources, pResidencyStatus, NumResources); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::SetGPUThreadPriority(
    /* [in] */ INT Priority)
{
    if (_SetGPUThreadPriority_pre_ptr._value) { (this->*_SetGPUThreadPriority_pre_ptr._value)(Priority); }
    HRESULT ret = GetRealObj()->SetGPUThreadPriority(Priority);
    if (_SetGPUThreadPriority_post_ptr._value) { (this->*_SetGPUThreadPriority_post_ptr._value)(ret, Priority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::GetGPUThreadPriority(
    _Out_  INT * pPriority)
{
    if (_GetGPUThreadPriority_pre_ptr._value) { (this->*_GetGPUThreadPriority_pre_ptr._value)(pPriority); }
    HRESULT ret = GetRealObj()->GetGPUThreadPriority(pPriority);
    if (_GetGPUThreadPriority_post_ptr._value) { (this->*_GetGPUThreadPriority_post_ptr._value)(ret, pPriority); }
    return ret;
}

