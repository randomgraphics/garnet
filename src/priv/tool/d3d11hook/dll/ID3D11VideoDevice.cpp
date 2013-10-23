// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoDecoder(
    _In_  const D3D11_VIDEO_DECODER_DESC * pVideoDesc,
    _In_  const D3D11_VIDEO_DECODER_CONFIG * pConfig,
    _Out_  ID3D11VideoDecoder ** ppDecoder)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoDecoder");
    if (_CreateVideoDecoder_pre_ptr._value) { (this->*_CreateVideoDecoder_pre_ptr._value)(pVideoDesc, pConfig, ppDecoder); }
    HRESULT ret = GetRealObj()->CreateVideoDecoder(pVideoDesc, pConfig, ppDecoder);
    if (ppDecoder) *ppDecoder = RealToHooked( *ppDecoder );
    if (_CreateVideoDecoder_post_ptr._value) { (this->*_CreateVideoDecoder_post_ptr._value)(ret, pVideoDesc, pConfig, ppDecoder); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoProcessor(
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  UINT RateConversionIndex,
    _Out_  ID3D11VideoProcessor ** ppVideoProcessor)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoProcessor");
    if (_CreateVideoProcessor_pre_ptr._value) { (this->*_CreateVideoProcessor_pre_ptr._value)(pEnum, RateConversionIndex, ppVideoProcessor); }
    HRESULT ret = GetRealObj()->CreateVideoProcessor(HookedToReal(pEnum), RateConversionIndex, ppVideoProcessor);
    if (ppVideoProcessor) *ppVideoProcessor = RealToHooked( *ppVideoProcessor );
    if (_CreateVideoProcessor_post_ptr._value) { (this->*_CreateVideoProcessor_post_ptr._value)(ret, pEnum, RateConversionIndex, ppVideoProcessor); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateAuthenticatedChannel(
    _In_  D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType,
    _Out_  ID3D11AuthenticatedChannel ** ppAuthenticatedChannel)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateAuthenticatedChannel");
    if (_CreateAuthenticatedChannel_pre_ptr._value) { (this->*_CreateAuthenticatedChannel_pre_ptr._value)(ChannelType, ppAuthenticatedChannel); }
    HRESULT ret = GetRealObj()->CreateAuthenticatedChannel(ChannelType, ppAuthenticatedChannel);
    if (ppAuthenticatedChannel) *ppAuthenticatedChannel = RealToHooked( *ppAuthenticatedChannel );
    if (_CreateAuthenticatedChannel_post_ptr._value) { (this->*_CreateAuthenticatedChannel_post_ptr._value)(ret, ChannelType, ppAuthenticatedChannel); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateCryptoSession(
    _In_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _In_  const GUID * pKeyExchangeType,
    _Outptr_  ID3D11CryptoSession ** ppCryptoSession)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateCryptoSession");
    if (_CreateCryptoSession_pre_ptr._value) { (this->*_CreateCryptoSession_pre_ptr._value)(pCryptoType, pDecoderProfile, pKeyExchangeType, ppCryptoSession); }
    HRESULT ret = GetRealObj()->CreateCryptoSession(pCryptoType, pDecoderProfile, pKeyExchangeType, HookedToReal(ppCryptoSession));
    if (_CreateCryptoSession_post_ptr._value) { (this->*_CreateCryptoSession_post_ptr._value)(ret, pCryptoType, pDecoderProfile, pKeyExchangeType, ppCryptoSession); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoDecoderOutputView(
    _In_  ID3D11Resource * pResource,
    _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoDecoderOutputView ** ppVDOVView)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoDecoderOutputView");
    if (_CreateVideoDecoderOutputView_pre_ptr._value) { (this->*_CreateVideoDecoderOutputView_pre_ptr._value)(pResource, pDesc, ppVDOVView); }
    HRESULT ret = GetRealObj()->CreateVideoDecoderOutputView(HookedToReal(pResource), pDesc, ppVDOVView);
    if (ppVDOVView) *ppVDOVView = RealToHooked( *ppVDOVView );
    if (_CreateVideoDecoderOutputView_post_ptr._value) { (this->*_CreateVideoDecoderOutputView_post_ptr._value)(ret, pResource, pDesc, ppVDOVView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoProcessorInputView(
    _In_  ID3D11Resource * pResource,
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoProcessorInputView ** ppVPIView)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoProcessorInputView");
    if (_CreateVideoProcessorInputView_pre_ptr._value) { (this->*_CreateVideoProcessorInputView_pre_ptr._value)(pResource, pEnum, pDesc, ppVPIView); }
    HRESULT ret = GetRealObj()->CreateVideoProcessorInputView(HookedToReal(pResource), HookedToReal(pEnum), pDesc, ppVPIView);
    if (ppVPIView) *ppVPIView = RealToHooked( *ppVPIView );
    if (_CreateVideoProcessorInputView_post_ptr._value) { (this->*_CreateVideoProcessorInputView_post_ptr._value)(ret, pResource, pEnum, pDesc, ppVPIView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoProcessorOutputView(
    _In_  ID3D11Resource * pResource,
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoProcessorOutputView ** ppVPOView)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoProcessorOutputView");
    if (_CreateVideoProcessorOutputView_pre_ptr._value) { (this->*_CreateVideoProcessorOutputView_pre_ptr._value)(pResource, pEnum, pDesc, ppVPOView); }
    HRESULT ret = GetRealObj()->CreateVideoProcessorOutputView(HookedToReal(pResource), HookedToReal(pEnum), pDesc, ppVPOView);
    if (ppVPOView) *ppVPOView = RealToHooked( *ppVPOView );
    if (_CreateVideoProcessorOutputView_post_ptr._value) { (this->*_CreateVideoProcessorOutputView_post_ptr._value)(ret, pResource, pEnum, pDesc, ppVPOView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoProcessorEnumerator(
    _In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc,
    _Out_  ID3D11VideoProcessorEnumerator ** ppEnum)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoProcessorEnumerator");
    if (_CreateVideoProcessorEnumerator_pre_ptr._value) { (this->*_CreateVideoProcessorEnumerator_pre_ptr._value)(pDesc, ppEnum); }
    HRESULT ret = GetRealObj()->CreateVideoProcessorEnumerator(pDesc, ppEnum);
    if (ppEnum) *ppEnum = RealToHooked( *ppEnum );
    if (_CreateVideoProcessorEnumerator_post_ptr._value) { (this->*_CreateVideoProcessorEnumerator_post_ptr._value)(ret, pDesc, ppEnum); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetVideoDecoderProfileCount()
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetVideoDecoderProfileCount");
    if (_GetVideoDecoderProfileCount_pre_ptr._value) { (this->*_GetVideoDecoderProfileCount_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetVideoDecoderProfileCount();
    if (_GetVideoDecoderProfileCount_post_ptr._value) { (this->*_GetVideoDecoderProfileCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetVideoDecoderProfile(
    _In_  UINT Index,
    _Out_  GUID * pDecoderProfile)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetVideoDecoderProfile");
    if (_GetVideoDecoderProfile_pre_ptr._value) { (this->*_GetVideoDecoderProfile_pre_ptr._value)(Index, pDecoderProfile); }
    HRESULT ret = GetRealObj()->GetVideoDecoderProfile(Index, pDecoderProfile);
    if (_GetVideoDecoderProfile_post_ptr._value) { (this->*_GetVideoDecoderProfile_post_ptr._value)(ret, Index, pDecoderProfile); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CheckVideoDecoderFormat(
    _In_  const GUID * pDecoderProfile,
    _In_  DXGI_FORMAT Format,
    _Out_  BOOL * pSupported)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CheckVideoDecoderFormat");
    if (_CheckVideoDecoderFormat_pre_ptr._value) { (this->*_CheckVideoDecoderFormat_pre_ptr._value)(pDecoderProfile, Format, pSupported); }
    HRESULT ret = GetRealObj()->CheckVideoDecoderFormat(pDecoderProfile, Format, pSupported);
    if (_CheckVideoDecoderFormat_post_ptr._value) { (this->*_CheckVideoDecoderFormat_post_ptr._value)(ret, pDecoderProfile, Format, pSupported); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetVideoDecoderConfigCount(
    _In_  const D3D11_VIDEO_DECODER_DESC * pDesc,
    _Out_  UINT * pCount)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetVideoDecoderConfigCount");
    if (_GetVideoDecoderConfigCount_pre_ptr._value) { (this->*_GetVideoDecoderConfigCount_pre_ptr._value)(pDesc, pCount); }
    HRESULT ret = GetRealObj()->GetVideoDecoderConfigCount(pDesc, pCount);
    if (_GetVideoDecoderConfigCount_post_ptr._value) { (this->*_GetVideoDecoderConfigCount_post_ptr._value)(ret, pDesc, pCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetVideoDecoderConfig(
    _In_  const D3D11_VIDEO_DECODER_DESC * pDesc,
    _In_  UINT Index,
    _Out_  D3D11_VIDEO_DECODER_CONFIG * pConfig)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetVideoDecoderConfig");
    if (_GetVideoDecoderConfig_pre_ptr._value) { (this->*_GetVideoDecoderConfig_pre_ptr._value)(pDesc, Index, pConfig); }
    HRESULT ret = GetRealObj()->GetVideoDecoderConfig(pDesc, Index, pConfig);
    if (_GetVideoDecoderConfig_post_ptr._value) { (this->*_GetVideoDecoderConfig_post_ptr._value)(ret, pDesc, Index, pConfig); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetContentProtectionCaps(
    _In_opt_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetContentProtectionCaps");
    if (_GetContentProtectionCaps_pre_ptr._value) { (this->*_GetContentProtectionCaps_pre_ptr._value)(pCryptoType, pDecoderProfile, pCaps); }
    HRESULT ret = GetRealObj()->GetContentProtectionCaps(pCryptoType, pDecoderProfile, pCaps);
    if (_GetContentProtectionCaps_post_ptr._value) { (this->*_GetContentProtectionCaps_post_ptr._value)(ret, pCryptoType, pDecoderProfile, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CheckCryptoKeyExchange(
    _In_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _In_  UINT Index,
    _Out_  GUID * pKeyExchangeType)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CheckCryptoKeyExchange");
    if (_CheckCryptoKeyExchange_pre_ptr._value) { (this->*_CheckCryptoKeyExchange_pre_ptr._value)(pCryptoType, pDecoderProfile, Index, pKeyExchangeType); }
    HRESULT ret = GetRealObj()->CheckCryptoKeyExchange(pCryptoType, pDecoderProfile, Index, pKeyExchangeType);
    if (_CheckCryptoKeyExchange_post_ptr._value) { (this->*_CheckCryptoKeyExchange_post_ptr._value)(ret, pCryptoType, pDecoderProfile, Index, pKeyExchangeType); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_(DataSize)  const void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(guid, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(guid, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, guid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::SetPrivateDataInterface");
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(guid, pData); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(guid, pData);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, guid, pData); }
    return ret;
}

