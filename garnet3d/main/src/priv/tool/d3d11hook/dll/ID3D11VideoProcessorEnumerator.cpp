// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorContentDesc(
    _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorContentDesc");
    if (_GetVideoProcessorContentDesc_pre_ptr._value) { (this->*_GetVideoProcessorContentDesc_pre_ptr._value)(pContentDesc); }
    HRESULT ret = GetRealObj()->GetVideoProcessorContentDesc(pContentDesc);
    if (_GetVideoProcessorContentDesc_post_ptr._value) { (this->*_GetVideoProcessorContentDesc_post_ptr._value)(ret, pContentDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::CheckVideoProcessorFormat(
    _In_  DXGI_FORMAT Format,
    _Out_  UINT * pFlags)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::CheckVideoProcessorFormat");
    if (_CheckVideoProcessorFormat_pre_ptr._value) { (this->*_CheckVideoProcessorFormat_pre_ptr._value)(Format, pFlags); }
    HRESULT ret = GetRealObj()->CheckVideoProcessorFormat(Format, pFlags);
    if (_CheckVideoProcessorFormat_post_ptr._value) { (this->*_CheckVideoProcessorFormat_post_ptr._value)(ret, Format, pFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorCaps(
    _Out_  D3D11_VIDEO_PROCESSOR_CAPS * pCaps)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorCaps");
    if (_GetVideoProcessorCaps_pre_ptr._value) { (this->*_GetVideoProcessorCaps_pre_ptr._value)(pCaps); }
    HRESULT ret = GetRealObj()->GetVideoProcessorCaps(pCaps);
    if (_GetVideoProcessorCaps_post_ptr._value) { (this->*_GetVideoProcessorCaps_post_ptr._value)(ret, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorRateConversionCaps(
    _In_  UINT TypeIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorRateConversionCaps");
    if (_GetVideoProcessorRateConversionCaps_pre_ptr._value) { (this->*_GetVideoProcessorRateConversionCaps_pre_ptr._value)(TypeIndex, pCaps); }
    HRESULT ret = GetRealObj()->GetVideoProcessorRateConversionCaps(TypeIndex, pCaps);
    if (_GetVideoProcessorRateConversionCaps_post_ptr._value) { (this->*_GetVideoProcessorRateConversionCaps_post_ptr._value)(ret, TypeIndex, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorCustomRate(
    _In_  UINT TypeIndex,
    _In_  UINT CustomRateIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorCustomRate");
    if (_GetVideoProcessorCustomRate_pre_ptr._value) { (this->*_GetVideoProcessorCustomRate_pre_ptr._value)(TypeIndex, CustomRateIndex, pRate); }
    HRESULT ret = GetRealObj()->GetVideoProcessorCustomRate(TypeIndex, CustomRateIndex, pRate);
    if (_GetVideoProcessorCustomRate_post_ptr._value) { (this->*_GetVideoProcessorCustomRate_post_ptr._value)(ret, TypeIndex, CustomRateIndex, pRate); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorFilterRange(
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorFilterRange");
    if (_GetVideoProcessorFilterRange_pre_ptr._value) { (this->*_GetVideoProcessorFilterRange_pre_ptr._value)(Filter, pRange); }
    HRESULT ret = GetRealObj()->GetVideoProcessorFilterRange(Filter, pRange);
    if (_GetVideoProcessorFilterRange_post_ptr._value) { (this->*_GetVideoProcessorFilterRange_post_ptr._value)(ret, Filter, pRange); }
    return ret;
}

