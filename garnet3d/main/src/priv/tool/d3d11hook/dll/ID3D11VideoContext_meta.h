// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDecoderBuffer, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoDecoder *, pDecoder),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_DECODER_BUFFER_TYPE, Type),
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pBufferSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_opt_(*pBufferSize)  void **, ppBuffer)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, ReleaseDecoderBuffer, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoDecoder *, pDecoder),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_DECODER_BUFFER_TYPE, Type)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, DecoderBeginFrame, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoDecoder *, pDecoder),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoDecoderOutputView *, pView),
    DEFINE_METHOD_PARAMETER(_In_  UINT, ContentKeySize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_opt_(ContentKeySize)  const void *, pContentKey)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, DecoderEndFrame, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoDecoder *, pDecoder)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SubmitDecoderBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoDecoder *, pDecoder),
    DEFINE_METHOD_PARAMETER(_In_  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC *, pBufferDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, DecoderExtension, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoDecoder *, pDecoder),
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_DECODER_EXTENSION *, pExtensionData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetOutputTargetRect, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_opt_  const RECT *, pRect)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetOutputBackgroundColor, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, YCbCr),
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_COLOR *, pColor)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetOutputColorSpace, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE *, pColorSpace)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetOutputAlphaFillMode, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE, AlphaFillMode),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetOutputConstriction, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_  SIZE, Size)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetOutputStereoMode, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, VideoProcessorSetOutputExtension, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  const GUID *, pExtensionGuid),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_In_  void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetOutputTargetRect, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, Enabled),
    DEFINE_METHOD_PARAMETER(_Out_  RECT *, pRect)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetOutputBackgroundColor, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pYCbCr),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_COLOR *, pColor)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetOutputColorSpace, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE *, pColorSpace)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetOutputAlphaFillMode, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE *, pAlphaFillMode),
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pStreamIndex)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetOutputConstriction, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled),
    DEFINE_METHOD_PARAMETER(_Out_  SIZE *, pSize)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetOutputStereoMode, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, VideoProcessorGetOutputExtension, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  const GUID *, pExtensionGuid),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_Out_  void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamFrameFormat, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_FRAME_FORMAT, FrameFormat)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamColorSpace, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE *, pColorSpace)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamOutputRate, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE, OutputRate),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, RepeatFrame),
    DEFINE_METHOD_PARAMETER(_In_opt_  const DXGI_RATIONAL *, pCustomRate)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamSourceRect, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_opt_  const RECT *, pRect)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamDestRect, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_opt_  const RECT *, pRect)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamAlpha, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_  FLOAT, Alpha)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamPalette, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  UINT, Count),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(Count)  const UINT *, pEntries)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamPixelAspectRatio, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_opt_  const DXGI_RATIONAL *, pSourceAspectRatio),
    DEFINE_METHOD_PARAMETER(_In_opt_  const DXGI_RATIONAL *, pDestinationAspectRatio)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamLumaKey, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_  FLOAT, Lower),
    DEFINE_METHOD_PARAMETER(_In_  FLOAT, Upper)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamStereoFormat, PARAMETER_LIST_8(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT, Format),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, LeftViewFrame0),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, BaseViewFrame0),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE, FlipMode),
    DEFINE_METHOD_PARAMETER(_In_  int, MonoOffset)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamAutoProcessingMode, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamFilter, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_PROCESSOR_FILTER, Filter),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_  int, Level)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, VideoProcessorSetStreamExtension, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  const GUID *, pExtensionGuid),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_In_  void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamFrameFormat, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_FRAME_FORMAT *, pFrameFormat)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamColorSpace, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE *, pColorSpace)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamOutputRate, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE *, pOutputRate),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pRepeatFrame),
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_RATIONAL *, pCustomRate)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamSourceRect, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled),
    DEFINE_METHOD_PARAMETER(_Out_  RECT *, pRect)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamDestRect, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled),
    DEFINE_METHOD_PARAMETER(_Out_  RECT *, pRect)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamAlpha, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled),
    DEFINE_METHOD_PARAMETER(_Out_  FLOAT *, pAlpha)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamPalette, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  UINT, Count),
    DEFINE_METHOD_PARAMETER(_Out_writes_(Count)  UINT *, pEntries)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamPixelAspectRatio, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled),
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_RATIONAL *, pSourceAspectRatio),
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_RATIONAL *, pDestinationAspectRatio)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamLumaKey, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled),
    DEFINE_METHOD_PARAMETER(_Out_  FLOAT *, pLower),
    DEFINE_METHOD_PARAMETER(_Out_  FLOAT *, pUpper)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamStereoFormat, PARAMETER_LIST_8(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnable),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT *, pFormat),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pLeftViewFrame0),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pBaseViewFrame0),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE *, pFlipMode),
    DEFINE_METHOD_PARAMETER(_Out_  int *, MonoOffset)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamAutoProcessingMode, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamFilter, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_PROCESSOR_FILTER, Filter),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnabled),
    DEFINE_METHOD_PARAMETER(_Out_  int *, pLevel)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, VideoProcessorGetStreamExtension, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  const GUID *, pExtensionGuid),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_Out_  void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, VideoProcessorBlt, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessorOutputView *, pView),
    DEFINE_METHOD_PARAMETER(_In_  UINT, OutputFrame),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamCount),
    DEFINE_METHOD_PARAMETER(_In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM *, pStreams)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, NegotiateCryptoSessionKeyExchange, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11CryptoSession *, pCryptoSession),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_Inout_updates_bytes_(DataSize)  void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, EncryptionBlt, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11CryptoSession *, pCryptoSession),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Texture2D *, pSrcSurface),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Texture2D *, pDstSurface),
    DEFINE_METHOD_PARAMETER(_In_  UINT, IVSize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_opt_(IVSize)  void *, pIV)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DecryptionBlt, PARAMETER_LIST_8(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11CryptoSession *, pCryptoSession),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Texture2D *, pSrcSurface),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Texture2D *, pDstSurface),
    DEFINE_METHOD_PARAMETER(_In_opt_  D3D11_ENCRYPTED_BLOCK_INFO *, pEncryptedBlockInfo),
    DEFINE_METHOD_PARAMETER(_In_  UINT, ContentKeySize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_opt_(ContentKeySize)  const void *, pContentKey),
    DEFINE_METHOD_PARAMETER(_In_  UINT, IVSize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_opt_(IVSize)  void *, pIV)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, StartSessionKeyRefresh, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11CryptoSession *, pCryptoSession),
    DEFINE_METHOD_PARAMETER(_In_  UINT, RandomNumberSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_(RandomNumberSize)  void *, pRandomNumber)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, FinishSessionKeyRefresh, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11CryptoSession *, pCryptoSession)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetEncryptionBltKey, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11CryptoSession *, pCryptoSession),
    DEFINE_METHOD_PARAMETER(_In_  UINT, KeySize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_(KeySize)  void *, pReadbackKey)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, NegotiateAuthenticatedChannelKeyExchange, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11AuthenticatedChannel *, pChannel),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_Inout_updates_bytes_(DataSize)  void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, QueryAuthenticatedChannel, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11AuthenticatedChannel *, pChannel),
    DEFINE_METHOD_PARAMETER(_In_  UINT, InputSize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_(InputSize)  const void *, pInput),
    DEFINE_METHOD_PARAMETER(_In_  UINT, OutputSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_(OutputSize)  void *, pOutput)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, ConfigureAuthenticatedChannel, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11AuthenticatedChannel *, pChannel),
    DEFINE_METHOD_PARAMETER(_In_  UINT, InputSize),
    DEFINE_METHOD_PARAMETER(_In_reads_bytes_(InputSize)  const void *, pInput),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT *, pOutput)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorSetStreamRotation, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, Enable),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_PROCESSOR_ROTATION, Rotation)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VideoProcessorGetStreamRotation, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11VideoProcessor *, pVideoProcessor),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StreamIndex),
    DEFINE_METHOD_PARAMETER(_Out_  BOOL *, pEnable),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_ROTATION *, pRotation)))
