// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoProcessorHook::GetContentDesc(
    _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorHook::GetContentDesc");
    if (_GetContentDesc_pre_ptr._value) { (this->*_GetContentDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetContentDesc(pDesc);
    if (_GetContentDesc_post_ptr._value) { (this->*_GetContentDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoProcessorHook::GetRateConversionCaps(
    _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorHook::GetRateConversionCaps");
    if (_GetRateConversionCaps_pre_ptr._value) { (this->*_GetRateConversionCaps_pre_ptr._value)(pCaps); }
    GetRealObj()->GetRateConversionCaps(pCaps);
    if (_GetRateConversionCaps_post_ptr._value) { (this->*_GetRateConversionCaps_post_ptr._value)(pCaps); }
}

