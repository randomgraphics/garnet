// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:


protected:

D3D11VideoDeviceHook(UnknownBase & unknown, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
{
    Construct(); 
}

~D3D11VideoDeviceHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11VideoDeviceHook(unknown, realobj);
        result->AddRef();
        return result;
    }
    catch(std::bad_alloc&)
    {
        GN_ERROR(GN::getLogger("GN.d3d11hook"))("Out of memory.");
        return nullptr;
    }
}

// ==============================================================================
// Calling to base interfaces
// ==============================================================================
public:

// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateVideoDecoder(
    _In_  const D3D11_VIDEO_DECODER_DESC * pVideoDesc,
    _In_  const D3D11_VIDEO_DECODER_CONFIG * pConfig,
    _Out_  ID3D11VideoDecoder ** ppDecoder);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const D3D11_VIDEO_DECODER_DESC * &, _In_  const D3D11_VIDEO_DECODER_CONFIG * &, _Out_  ID3D11VideoDecoder ** &)> _CreateVideoDecoder_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const D3D11_VIDEO_DECODER_DESC *, _In_  const D3D11_VIDEO_DECODER_CONFIG *, _Out_  ID3D11VideoDecoder **)> _CreateVideoDecoder_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessor(
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  UINT RateConversionIndex,
    _Out_  ID3D11VideoProcessor ** ppVideoProcessor);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  ID3D11VideoProcessorEnumerator * &, _In_  UINT &, _Out_  ID3D11VideoProcessor ** &)> _CreateVideoProcessor_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  ID3D11VideoProcessorEnumerator *, _In_  UINT, _Out_  ID3D11VideoProcessor **)> _CreateVideoProcessor_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateAuthenticatedChannel(
    _In_  D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType,
    _Out_  ID3D11AuthenticatedChannel ** ppAuthenticatedChannel);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  D3D11_AUTHENTICATED_CHANNEL_TYPE &, _Out_  ID3D11AuthenticatedChannel ** &)> _CreateAuthenticatedChannel_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  D3D11_AUTHENTICATED_CHANNEL_TYPE, _Out_  ID3D11AuthenticatedChannel **)> _CreateAuthenticatedChannel_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateCryptoSession(
    _In_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _In_  const GUID * pKeyExchangeType,
    _Outptr_  ID3D11CryptoSession ** ppCryptoSession);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const GUID * &, _In_opt_  const GUID * &, _In_  const GUID * &, _Outptr_  ID3D11CryptoSession ** &)> _CreateCryptoSession_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const GUID *, _In_opt_  const GUID *, _In_  const GUID *, _Outptr_  ID3D11CryptoSession **)> _CreateCryptoSession_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateVideoDecoderOutputView(
    _In_  ID3D11Resource * pResource,
    _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoDecoderOutputView ** ppVDOVView);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  ID3D11Resource * &, _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * &, _Out_opt_  ID3D11VideoDecoderOutputView ** &)> _CreateVideoDecoderOutputView_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC *, _Out_opt_  ID3D11VideoDecoderOutputView **)> _CreateVideoDecoderOutputView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorInputView(
    _In_  ID3D11Resource * pResource,
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoProcessorInputView ** ppVPIView);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  ID3D11Resource * &, _In_  ID3D11VideoProcessorEnumerator * &, _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * &, _Out_opt_  ID3D11VideoProcessorInputView ** &)> _CreateVideoProcessorInputView_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_  ID3D11VideoProcessorEnumerator *, _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC *, _Out_opt_  ID3D11VideoProcessorInputView **)> _CreateVideoProcessorInputView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorOutputView(
    _In_  ID3D11Resource * pResource,
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoProcessorOutputView ** ppVPOView);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  ID3D11Resource * &, _In_  ID3D11VideoProcessorEnumerator * &, _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * &, _Out_opt_  ID3D11VideoProcessorOutputView ** &)> _CreateVideoProcessorOutputView_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_  ID3D11VideoProcessorEnumerator *, _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC *, _Out_opt_  ID3D11VideoProcessorOutputView **)> _CreateVideoProcessorOutputView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorEnumerator(
    _In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc,
    _Out_  ID3D11VideoProcessorEnumerator ** ppEnum);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * &, _Out_  ID3D11VideoProcessorEnumerator ** &)> _CreateVideoProcessorEnumerator_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC *, _Out_  ID3D11VideoProcessorEnumerator **)> _CreateVideoProcessorEnumerator_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetVideoDecoderProfileCount();
NullPtr<void (D3D11VideoDeviceHook::*)()> _GetVideoDecoderProfileCount_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(UINT)> _GetVideoDecoderProfileCount_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderProfile(
    _In_  UINT Index,
    _Out_  GUID * pDecoderProfile);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  UINT &, _Out_  GUID * &)> _GetVideoDecoderProfile_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  UINT, _Out_  GUID *)> _GetVideoDecoderProfile_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckVideoDecoderFormat(
    _In_  const GUID * pDecoderProfile,
    _In_  DXGI_FORMAT Format,
    _Out_  BOOL * pSupported);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const GUID * &, _In_  DXGI_FORMAT &, _Out_  BOOL * &)> _CheckVideoDecoderFormat_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const GUID *, _In_  DXGI_FORMAT, _Out_  BOOL *)> _CheckVideoDecoderFormat_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderConfigCount(
    _In_  const D3D11_VIDEO_DECODER_DESC * pDesc,
    _Out_  UINT * pCount);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const D3D11_VIDEO_DECODER_DESC * &, _Out_  UINT * &)> _GetVideoDecoderConfigCount_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const D3D11_VIDEO_DECODER_DESC *, _Out_  UINT *)> _GetVideoDecoderConfigCount_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderConfig(
    _In_  const D3D11_VIDEO_DECODER_DESC * pDesc,
    _In_  UINT Index,
    _Out_  D3D11_VIDEO_DECODER_CONFIG * pConfig);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const D3D11_VIDEO_DECODER_DESC * &, _In_  UINT &, _Out_  D3D11_VIDEO_DECODER_CONFIG * &)> _GetVideoDecoderConfig_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const D3D11_VIDEO_DECODER_DESC *, _In_  UINT, _Out_  D3D11_VIDEO_DECODER_CONFIG *)> _GetVideoDecoderConfig_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetContentProtectionCaps(
    _In_opt_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_opt_  const GUID * &, _In_opt_  const GUID * &, _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS * &)> _GetContentProtectionCaps_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_opt_  const GUID *, _In_opt_  const GUID *, _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS *)> _GetContentProtectionCaps_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckCryptoKeyExchange(
    _In_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _In_  UINT Index,
    _Out_  GUID * pKeyExchangeType);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const GUID * &, _In_opt_  const GUID * &, _In_  UINT &, _Out_  GUID * &)> _CheckCryptoKeyExchange_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const GUID *, _In_opt_  const GUID *, _In_  UINT, _Out_  GUID *)> _CheckCryptoKeyExchange_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_(DataSize)  const void * pData);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  REFGUID, _In_  UINT &, _In_reads_bytes_opt_(DataSize)  const void * &)> _SetPrivateData_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  REFGUID, _In_  UINT, _In_reads_bytes_opt_(DataSize)  const void *)> _SetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData);
NullPtr<void (D3D11VideoDeviceHook::*)(_In_  REFGUID, _In_opt_  const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  REFGUID, _In_opt_  const IUnknown *)> _SetPrivateDataInterface_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: