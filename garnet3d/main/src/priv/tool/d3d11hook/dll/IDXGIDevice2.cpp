// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice2Hook::OfferResources(
    _In_  UINT NumResources,
    _In_reads_(NumResources)  IDXGIResource *const * ppResources,
    _In_  DXGI_OFFER_RESOURCE_PRIORITY Priority)
{
    calltrace::AutoTrace trace(L"DXGIDevice2Hook::OfferResources");
    if (_OfferResources_pre_ptr._value) { (this->*_OfferResources_pre_ptr._value)(NumResources, ppResources, Priority); }
    HRESULT ret = GetRealObj()->OfferResources(NumResources, HookedToReal(NumResources, ppResources), Priority);
    if (_OfferResources_post_ptr._value) { (this->*_OfferResources_post_ptr._value)(ret, NumResources, ppResources, Priority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice2Hook::ReclaimResources(
    _In_  UINT NumResources,
    _In_reads_(NumResources)  IDXGIResource *const * ppResources,
    _Out_writes_all_opt_(NumResources)  BOOL * pDiscarded)
{
    calltrace::AutoTrace trace(L"DXGIDevice2Hook::ReclaimResources");
    if (_ReclaimResources_pre_ptr._value) { (this->*_ReclaimResources_pre_ptr._value)(NumResources, ppResources, pDiscarded); }
    HRESULT ret = GetRealObj()->ReclaimResources(NumResources, HookedToReal(NumResources, ppResources), pDiscarded);
    if (_ReclaimResources_post_ptr._value) { (this->*_ReclaimResources_post_ptr._value)(ret, NumResources, ppResources, pDiscarded); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice2Hook::EnqueueSetEvent(
    _In_  HANDLE hEvent)
{
    calltrace::AutoTrace trace(L"DXGIDevice2Hook::EnqueueSetEvent");
    if (_EnqueueSetEvent_pre_ptr._value) { (this->*_EnqueueSetEvent_pre_ptr._value)(hEvent); }
    HRESULT ret = GetRealObj()->EnqueueSetEvent(hEvent);
    if (_EnqueueSetEvent_post_ptr._value) { (this->*_EnqueueSetEvent_post_ptr._value)(ret, hEvent); }
    return ret;
}

