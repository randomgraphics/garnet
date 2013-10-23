// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

D3D11VideoContextHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11DeviceChild(D3D11DeviceChild)
{
    Construct(); 
}

~D3D11VideoContextHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
    if (nullptr == D3D11DeviceChild) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11VideoContextHook(unknown, *D3D11DeviceChild, realobj);
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

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE GetDevice(
    _Out_  ID3D11Device ** ppDevice)
{
    return _D3D11DeviceChild.GetDevice(ppDevice);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_( *pDataSize )  void * pData)
{
    return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_( DataSize )  const void * pData)
{
    return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDecoderBuffer(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type,
    _Out_  UINT * pBufferSize,
    _Out_writes_bytes_opt_(*pBufferSize)  void ** ppBuffer);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE &, _Out_  UINT * &, _Out_writes_bytes_opt_(*pBufferSize)  void ** &)> _GetDecoderBuffer_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE, _Out_  UINT *, _Out_writes_bytes_opt_(*pBufferSize)  void **)> _GetDecoderBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ReleaseDecoderBuffer(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE &)> _ReleaseDecoderBuffer_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE)> _ReleaseDecoderBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE DecoderBeginFrame(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  ID3D11VideoDecoderOutputView * pView,
    _In_  UINT ContentKeySize,
    _In_reads_bytes_opt_(ContentKeySize)  const void * pContentKey);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  ID3D11VideoDecoderOutputView * &, _In_  UINT &, _In_reads_bytes_opt_(ContentKeySize)  const void * &)> _DecoderBeginFrame_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  ID3D11VideoDecoderOutputView *, _In_  UINT, _In_reads_bytes_opt_(ContentKeySize)  const void *)> _DecoderBeginFrame_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE DecoderEndFrame(
    _In_  ID3D11VideoDecoder * pDecoder);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &)> _DecoderEndFrame_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *)> _DecoderEndFrame_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SubmitDecoderBuffers(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  UINT NumBuffers,
    _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  UINT &, _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC * &)> _SubmitDecoderBuffers_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  UINT, _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC *)> _SubmitDecoderBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE DecoderExtension(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  const D3D11_VIDEO_DECODER_EXTENSION * &)> _DecoderExtension_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  const D3D11_VIDEO_DECODER_EXTENSION *)> _DecoderExtension_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetOutputTargetRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  BOOL &, _In_opt_  const RECT * &)> _VideoProcessorSetOutputTargetRect_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  BOOL, _In_opt_  const RECT *)> _VideoProcessorSetOutputTargetRect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetOutputBackgroundColor(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL YCbCr,
    _In_  const D3D11_VIDEO_COLOR * pColor);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  BOOL &, _In_  const D3D11_VIDEO_COLOR * &)> _VideoProcessorSetOutputBackgroundColor_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  BOOL, _In_  const D3D11_VIDEO_COLOR *)> _VideoProcessorSetOutputBackgroundColor_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetOutputColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * &)> _VideoProcessorSetOutputColorSpace_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE *)> _VideoProcessorSetOutputColorSpace_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetOutputAlphaFillMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode,
    _In_  UINT StreamIndex);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE &, _In_  UINT &)> _VideoProcessorSetOutputAlphaFillMode_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE, _In_  UINT)> _VideoProcessorSetOutputAlphaFillMode_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetOutputConstriction(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable,
    _In_  SIZE Size);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  BOOL &, _In_  SIZE &)> _VideoProcessorSetOutputConstriction_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  BOOL, _In_  SIZE)> _VideoProcessorSetOutputConstriction_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetOutputStereoMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  BOOL &)> _VideoProcessorSetOutputStereoMode_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  BOOL)> _VideoProcessorSetOutputStereoMode_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE VideoProcessorSetOutputExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _In_  void * pData);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  const GUID * &, _In_  UINT &, _In_  void * &)> _VideoProcessorSetOutputExtension_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  const GUID *, _In_  UINT, _In_  void *)> _VideoProcessorSetOutputExtension_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetOutputTargetRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * Enabled,
    _Out_  RECT * pRect);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  BOOL * &, _Out_  RECT * &)> _VideoProcessorGetOutputTargetRect_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  BOOL *, _Out_  RECT *)> _VideoProcessorGetOutputTargetRect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetOutputBackgroundColor(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pYCbCr,
    _Out_  D3D11_VIDEO_COLOR * pColor);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  BOOL * &, _Out_  D3D11_VIDEO_COLOR * &)> _VideoProcessorGetOutputBackgroundColor_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  BOOL *, _Out_  D3D11_VIDEO_COLOR *)> _VideoProcessorGetOutputBackgroundColor_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetOutputColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * &)> _VideoProcessorGetOutputColorSpace_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE *)> _VideoProcessorGetOutputColorSpace_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetOutputAlphaFillMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode,
    _Out_  UINT * pStreamIndex);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * &, _Out_  UINT * &)> _VideoProcessorGetOutputAlphaFillMode_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE *, _Out_  UINT *)> _VideoProcessorGetOutputAlphaFillMode_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetOutputConstriction(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pEnabled,
    _Out_  SIZE * pSize);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  BOOL * &, _Out_  SIZE * &)> _VideoProcessorGetOutputConstriction_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  BOOL *, _Out_  SIZE *)> _VideoProcessorGetOutputConstriction_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetOutputStereoMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pEnabled);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  BOOL * &)> _VideoProcessorGetOutputStereoMode_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  BOOL *)> _VideoProcessorGetOutputStereoMode_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE VideoProcessorGetOutputExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _Out_  void * pData);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  const GUID * &, _In_  UINT &, _Out_  void * &)> _VideoProcessorGetOutputExtension_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  const GUID *, _In_  UINT, _Out_  void *)> _VideoProcessorGetOutputExtension_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamFrameFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_FRAME_FORMAT FrameFormat);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  D3D11_VIDEO_FRAME_FORMAT &)> _VideoProcessorSetStreamFrameFormat_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  D3D11_VIDEO_FRAME_FORMAT)> _VideoProcessorSetStreamFrameFormat_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * &)> _VideoProcessorSetStreamColorSpace_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE *)> _VideoProcessorSetStreamColorSpace_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamOutputRate(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate,
    _In_  BOOL RepeatFrame,
    _In_opt_  const DXGI_RATIONAL * pCustomRate);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE &, _In_  BOOL &, _In_opt_  const DXGI_RATIONAL * &)> _VideoProcessorSetStreamOutputRate_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE, _In_  BOOL, _In_opt_  const DXGI_RATIONAL *)> _VideoProcessorSetStreamOutputRate_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamSourceRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_opt_  const RECT * &)> _VideoProcessorSetStreamSourceRect_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_opt_  const RECT *)> _VideoProcessorSetStreamSourceRect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamDestRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_opt_  const RECT * &)> _VideoProcessorSetStreamDestRect_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_opt_  const RECT *)> _VideoProcessorSetStreamDestRect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamAlpha(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  FLOAT Alpha);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_  FLOAT &)> _VideoProcessorSetStreamAlpha_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_  FLOAT)> _VideoProcessorSetStreamAlpha_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamPalette(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  UINT Count,
    _In_reads_opt_(Count)  const UINT * pEntries);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  UINT &, _In_reads_opt_(Count)  const UINT * &)> _VideoProcessorSetStreamPalette_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  UINT, _In_reads_opt_(Count)  const UINT *)> _VideoProcessorSetStreamPalette_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamPixelAspectRatio(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const DXGI_RATIONAL * pSourceAspectRatio,
    _In_opt_  const DXGI_RATIONAL * pDestinationAspectRatio);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_opt_  const DXGI_RATIONAL * &, _In_opt_  const DXGI_RATIONAL * &)> _VideoProcessorSetStreamPixelAspectRatio_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_opt_  const DXGI_RATIONAL *, _In_opt_  const DXGI_RATIONAL *)> _VideoProcessorSetStreamPixelAspectRatio_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamLumaKey(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  FLOAT Lower,
    _In_  FLOAT Upper);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_  FLOAT &, _In_  FLOAT &)> _VideoProcessorSetStreamLumaKey_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_  FLOAT, _In_  FLOAT)> _VideoProcessorSetStreamLumaKey_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamStereoFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format,
    _In_  BOOL LeftViewFrame0,
    _In_  BOOL BaseViewFrame0,
    _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode,
    _In_  int MonoOffset);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT &, _In_  BOOL &, _In_  BOOL &, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE &, _In_  int &)> _VideoProcessorSetStreamStereoFormat_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT, _In_  BOOL, _In_  BOOL, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE, _In_  int)> _VideoProcessorSetStreamStereoFormat_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamAutoProcessingMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &)> _VideoProcessorSetStreamAutoProcessingMode_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL)> _VideoProcessorSetStreamAutoProcessingMode_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamFilter(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _In_  BOOL Enable,
    _In_  int Level);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  D3D11_VIDEO_PROCESSOR_FILTER &, _In_  BOOL &, _In_  int &)> _VideoProcessorSetStreamFilter_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  D3D11_VIDEO_PROCESSOR_FILTER, _In_  BOOL, _In_  int)> _VideoProcessorSetStreamFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE VideoProcessorSetStreamExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _In_  void * pData);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  const GUID * &, _In_  UINT &, _In_  void * &)> _VideoProcessorSetStreamExtension_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  UINT, _In_  const GUID *, _In_  UINT, _In_  void *)> _VideoProcessorSetStreamExtension_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamFrameFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_FRAME_FORMAT * pFrameFormat);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  D3D11_VIDEO_FRAME_FORMAT * &)> _VideoProcessorGetStreamFrameFormat_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  D3D11_VIDEO_FRAME_FORMAT *)> _VideoProcessorGetStreamFrameFormat_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * &)> _VideoProcessorGetStreamColorSpace_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE *)> _VideoProcessorGetStreamColorSpace_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamOutputRate(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate,
    _Out_  BOOL * pRepeatFrame,
    _Out_  DXGI_RATIONAL * pCustomRate);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * &, _Out_  BOOL * &, _Out_  DXGI_RATIONAL * &)> _VideoProcessorGetStreamOutputRate_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE *, _Out_  BOOL *, _Out_  DXGI_RATIONAL *)> _VideoProcessorGetStreamOutputRate_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamSourceRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  RECT * pRect);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  RECT * &)> _VideoProcessorGetStreamSourceRect_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  RECT *)> _VideoProcessorGetStreamSourceRect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamDestRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  RECT * pRect);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  RECT * &)> _VideoProcessorGetStreamDestRect_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  RECT *)> _VideoProcessorGetStreamDestRect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamAlpha(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  FLOAT * pAlpha);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  FLOAT * &)> _VideoProcessorGetStreamAlpha_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  FLOAT *)> _VideoProcessorGetStreamAlpha_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamPalette(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  UINT Count,
    _Out_writes_(Count)  UINT * pEntries);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  UINT &, _Out_writes_(Count)  UINT * &)> _VideoProcessorGetStreamPalette_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  UINT, _Out_writes_(Count)  UINT *)> _VideoProcessorGetStreamPalette_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamPixelAspectRatio(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  DXGI_RATIONAL * pSourceAspectRatio,
    _Out_  DXGI_RATIONAL * pDestinationAspectRatio);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  DXGI_RATIONAL * &, _Out_  DXGI_RATIONAL * &)> _VideoProcessorGetStreamPixelAspectRatio_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  DXGI_RATIONAL *, _Out_  DXGI_RATIONAL *)> _VideoProcessorGetStreamPixelAspectRatio_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamLumaKey(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  FLOAT * pLower,
    _Out_  FLOAT * pUpper);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  FLOAT * &, _Out_  FLOAT * &)> _VideoProcessorGetStreamLumaKey_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  FLOAT *, _Out_  FLOAT *)> _VideoProcessorGetStreamLumaKey_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamStereoFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnable,
    _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat,
    _Out_  BOOL * pLeftViewFrame0,
    _Out_  BOOL * pBaseViewFrame0,
    _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode,
    _Out_  int * MonoOffset);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * &, _Out_  BOOL * &, _Out_  BOOL * &, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * &, _Out_  int * &)> _VideoProcessorGetStreamStereoFormat_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT *, _Out_  BOOL *, _Out_  BOOL *, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE *, _Out_  int *)> _VideoProcessorGetStreamStereoFormat_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamAutoProcessingMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &)> _VideoProcessorGetStreamAutoProcessingMode_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *)> _VideoProcessorGetStreamAutoProcessingMode_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamFilter(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _Out_  BOOL * pEnabled,
    _Out_  int * pLevel);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  D3D11_VIDEO_PROCESSOR_FILTER &, _Out_  BOOL * &, _Out_  int * &)> _VideoProcessorGetStreamFilter_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  D3D11_VIDEO_PROCESSOR_FILTER, _Out_  BOOL *, _Out_  int *)> _VideoProcessorGetStreamFilter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE VideoProcessorGetStreamExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _Out_  void * pData);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  const GUID * &, _In_  UINT &, _Out_  void * &)> _VideoProcessorGetStreamExtension_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  UINT, _In_  const GUID *, _In_  UINT, _Out_  void *)> _VideoProcessorGetStreamExtension_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE VideoProcessorBlt(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  ID3D11VideoProcessorOutputView * pView,
    _In_  UINT OutputFrame,
    _In_  UINT StreamCount,
    _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM * pStreams);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  ID3D11VideoProcessorOutputView * &, _In_  UINT &, _In_  UINT &, _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM * &)> _VideoProcessorBlt_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  ID3D11VideoProcessorOutputView *, _In_  UINT, _In_  UINT, _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM *)> _VideoProcessorBlt_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE NegotiateCryptoSessionKeyExchange(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT DataSize,
    _Inout_updates_bytes_(DataSize)  void * pData);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  UINT &, _Inout_updates_bytes_(DataSize)  void * &)> _NegotiateCryptoSessionKeyExchange_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11CryptoSession *, _In_  UINT, _Inout_updates_bytes_(DataSize)  void *)> _NegotiateCryptoSessionKeyExchange_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE EncryptionBlt(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  ID3D11Texture2D * pSrcSurface,
    _In_  ID3D11Texture2D * pDstSurface,
    _In_  UINT IVSize,
    _In_reads_bytes_opt_(IVSize)  void * pIV);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  ID3D11Texture2D * &, _In_  ID3D11Texture2D * &, _In_  UINT &, _In_reads_bytes_opt_(IVSize)  void * &)> _EncryptionBlt_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession *, _In_  ID3D11Texture2D *, _In_  ID3D11Texture2D *, _In_  UINT, _In_reads_bytes_opt_(IVSize)  void *)> _EncryptionBlt_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DecryptionBlt(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  ID3D11Texture2D * pSrcSurface,
    _In_  ID3D11Texture2D * pDstSurface,
    _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo,
    _In_  UINT ContentKeySize,
    _In_reads_bytes_opt_(ContentKeySize)  const void * pContentKey,
    _In_  UINT IVSize,
    _In_reads_bytes_opt_(IVSize)  void * pIV);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  ID3D11Texture2D * &, _In_  ID3D11Texture2D * &, _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO * &, _In_  UINT &, _In_reads_bytes_opt_(ContentKeySize)  const void * &, _In_  UINT &, _In_reads_bytes_opt_(IVSize)  void * &)> _DecryptionBlt_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession *, _In_  ID3D11Texture2D *, _In_  ID3D11Texture2D *, _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO *, _In_  UINT, _In_reads_bytes_opt_(ContentKeySize)  const void *, _In_  UINT, _In_reads_bytes_opt_(IVSize)  void *)> _DecryptionBlt_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE StartSessionKeyRefresh(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT RandomNumberSize,
    _Out_writes_bytes_(RandomNumberSize)  void * pRandomNumber);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  UINT &, _Out_writes_bytes_(RandomNumberSize)  void * &)> _StartSessionKeyRefresh_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession *, _In_  UINT, _Out_writes_bytes_(RandomNumberSize)  void *)> _StartSessionKeyRefresh_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE FinishSessionKeyRefresh(
    _In_  ID3D11CryptoSession * pCryptoSession);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &)> _FinishSessionKeyRefresh_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession *)> _FinishSessionKeyRefresh_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetEncryptionBltKey(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT KeySize,
    _Out_writes_bytes_(KeySize)  void * pReadbackKey);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  UINT &, _Out_writes_bytes_(KeySize)  void * &)> _GetEncryptionBltKey_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11CryptoSession *, _In_  UINT, _Out_writes_bytes_(KeySize)  void *)> _GetEncryptionBltKey_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE NegotiateAuthenticatedChannelKeyExchange(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT DataSize,
    _Inout_updates_bytes_(DataSize)  void * pData);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11AuthenticatedChannel * &, _In_  UINT &, _Inout_updates_bytes_(DataSize)  void * &)> _NegotiateAuthenticatedChannelKeyExchange_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11AuthenticatedChannel *, _In_  UINT, _Inout_updates_bytes_(DataSize)  void *)> _NegotiateAuthenticatedChannelKeyExchange_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE QueryAuthenticatedChannel(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT InputSize,
    _In_reads_bytes_(InputSize)  const void * pInput,
    _In_  UINT OutputSize,
    _Out_writes_bytes_(OutputSize)  void * pOutput);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11AuthenticatedChannel * &, _In_  UINT &, _In_reads_bytes_(InputSize)  const void * &, _In_  UINT &, _Out_writes_bytes_(OutputSize)  void * &)> _QueryAuthenticatedChannel_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11AuthenticatedChannel *, _In_  UINT, _In_reads_bytes_(InputSize)  const void *, _In_  UINT, _Out_writes_bytes_(OutputSize)  void *)> _QueryAuthenticatedChannel_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ConfigureAuthenticatedChannel(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT InputSize,
    _In_reads_bytes_(InputSize)  const void * pInput,
    _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11AuthenticatedChannel * &, _In_  UINT &, _In_reads_bytes_(InputSize)  const void * &, _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * &)> _ConfigureAuthenticatedChannel_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11AuthenticatedChannel *, _In_  UINT, _In_reads_bytes_(InputSize)  const void *, _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT *)> _ConfigureAuthenticatedChannel_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorSetStreamRotation(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  D3D11_VIDEO_PROCESSOR_ROTATION Rotation);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_  D3D11_VIDEO_PROCESSOR_ROTATION &)> _VideoProcessorSetStreamRotation_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_  D3D11_VIDEO_PROCESSOR_ROTATION)> _VideoProcessorSetStreamRotation_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VideoProcessorGetStreamRotation(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnable,
    _Out_  D3D11_VIDEO_PROCESSOR_ROTATION * pRotation);
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  D3D11_VIDEO_PROCESSOR_ROTATION * &)> _VideoProcessorGetStreamRotation_pre_ptr;
NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  D3D11_VIDEO_PROCESSOR_ROTATION *)> _VideoProcessorGetStreamRotation_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: