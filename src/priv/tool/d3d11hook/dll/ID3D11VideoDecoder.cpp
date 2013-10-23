// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDecoderHook::GetCreationParameters(
    _Out_  D3D11_VIDEO_DECODER_DESC * pVideoDesc,
    _Out_  D3D11_VIDEO_DECODER_CONFIG * pConfig)
{
    calltrace::AutoTrace trace(L"D3D11VideoDecoderHook::GetCreationParameters");
    if (_GetCreationParameters_pre_ptr._value) { (this->*_GetCreationParameters_pre_ptr._value)(pVideoDesc, pConfig); }
    HRESULT ret = GetRealObj()->GetCreationParameters(pVideoDesc, pConfig);
    if (_GetCreationParameters_post_ptr._value) { (this->*_GetCreationParameters_post_ptr._value)(ret, pVideoDesc, pConfig); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDecoderHook::GetDriverHandle(
    _Out_  HANDLE * pDriverHandle)
{
    calltrace::AutoTrace trace(L"D3D11VideoDecoderHook::GetDriverHandle");
    if (_GetDriverHandle_pre_ptr._value) { (this->*_GetDriverHandle_pre_ptr._value)(pDriverHandle); }
    HRESULT ret = GetRealObj()->GetDriverHandle(pDriverHandle);
    if (_GetDriverHandle_post_ptr._value) { (this->*_GetDriverHandle_post_ptr._value)(ret, pDriverHandle); }
    return ret;
}

