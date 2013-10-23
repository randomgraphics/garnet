// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateVideoDecoder, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_DECODER_DESC *, pVideoDesc),
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_DECODER_CONFIG *, pConfig),
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11VideoDecoder **, ppDecoder)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateVideoProcessor, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessorEnumerator *, pEnum),
    DEFINE_METHOD_PARAMETER(_In_  UINT, RateConversionIndex),
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11VideoProcessor **, ppVideoProcessor)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateAuthenticatedChannel, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  D3D11_AUTHENTICATED_CHANNEL_TYPE, ChannelType),
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11AuthenticatedChannel **, ppAuthenticatedChannel)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateCryptoSession, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  const GUID *, pCryptoType),
    DEFINE_METHOD_PARAMETER(_In_opt_  const GUID *, pDecoderProfile),
    DEFINE_METHOD_PARAMETER(_In_  const GUID *, pKeyExchangeType),
    DEFINE_METHOD_PARAMETER(_Outptr_  ID3D11CryptoSession **, ppCryptoSession)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateVideoDecoderOutputView, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11VideoDecoderOutputView **, ppVDOVView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateVideoProcessorInputView, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessorEnumerator *, pEnum),
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11VideoProcessorInputView **, ppVPIView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateVideoProcessorOutputView, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessorEnumerator *, pEnum),
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11VideoProcessorOutputView **, ppVPOView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateVideoProcessorEnumerator, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11VideoProcessorEnumerator **, ppEnum)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT, STDMETHODCALLTYPE, GetVideoDecoderProfileCount, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetVideoDecoderProfile, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  UINT, Index),
    DEFINE_METHOD_PARAMETER(_Out_  GUID *, pDecoderProfile)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CheckVideoDecoderFormat, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  const GUID *, pDecoderProfile),
    DEFINE_METHOD_PARAMETER(_In_  DXGI_FORMAT, Format),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pSupported)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetVideoDecoderConfigCount, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_DECODER_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pCount)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetVideoDecoderConfig, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_DECODER_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(_In_  UINT, Index),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_DECODER_CONFIG *, pConfig)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetContentProtectionCaps, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  const GUID *, pCryptoType),
    DEFINE_METHOD_PARAMETER(_In_opt_  const GUID *, pDecoderProfile),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS *, pCaps)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CheckCryptoKeyExchange, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  const GUID *, pCryptoType),
    DEFINE_METHOD_PARAMETER(_In_opt_  const GUID *, pDecoderProfile),
    DEFINE_METHOD_PARAMETER(_In_  UINT, Index),
    DEFINE_METHOD_PARAMETER(_Out_  GUID *, pKeyExchangeType)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  REFGUID, guid),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_opt_(DataSize)  const void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateDataInterface, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  REFGUID, guid),
    DEFINE_METHOD_PARAMETER(_In_opt_  const IUnknown *, pData)))
