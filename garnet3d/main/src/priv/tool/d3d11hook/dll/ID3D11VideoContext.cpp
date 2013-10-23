// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::GetDecoderBuffer(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type,
    _Out_  UINT * pBufferSize,
    _Out_writes_bytes_opt_(*pBufferSize)  void ** ppBuffer)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::GetDecoderBuffer");
    if (_GetDecoderBuffer_pre_ptr._value) { (this->*_GetDecoderBuffer_pre_ptr._value)(pDecoder, Type, pBufferSize, ppBuffer); }
    HRESULT ret = GetRealObj()->GetDecoderBuffer(HookedToReal(pDecoder), Type, pBufferSize, ppBuffer);
    if (_GetDecoderBuffer_post_ptr._value) { (this->*_GetDecoderBuffer_post_ptr._value)(ret, pDecoder, Type, pBufferSize, ppBuffer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::ReleaseDecoderBuffer(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::ReleaseDecoderBuffer");
    if (_ReleaseDecoderBuffer_pre_ptr._value) { (this->*_ReleaseDecoderBuffer_pre_ptr._value)(pDecoder, Type); }
    HRESULT ret = GetRealObj()->ReleaseDecoderBuffer(HookedToReal(pDecoder), Type);
    if (_ReleaseDecoderBuffer_post_ptr._value) { (this->*_ReleaseDecoderBuffer_post_ptr._value)(ret, pDecoder, Type); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::DecoderBeginFrame(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  ID3D11VideoDecoderOutputView * pView,
    _In_  UINT ContentKeySize,
    _In_reads_bytes_opt_(ContentKeySize)  const void * pContentKey)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::DecoderBeginFrame");
    if (_DecoderBeginFrame_pre_ptr._value) { (this->*_DecoderBeginFrame_pre_ptr._value)(pDecoder, pView, ContentKeySize, pContentKey); }
    HRESULT ret = GetRealObj()->DecoderBeginFrame(HookedToReal(pDecoder), HookedToReal(pView), ContentKeySize, pContentKey);
    if (_DecoderBeginFrame_post_ptr._value) { (this->*_DecoderBeginFrame_post_ptr._value)(ret, pDecoder, pView, ContentKeySize, pContentKey); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::DecoderEndFrame(
    _In_  ID3D11VideoDecoder * pDecoder)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::DecoderEndFrame");
    if (_DecoderEndFrame_pre_ptr._value) { (this->*_DecoderEndFrame_pre_ptr._value)(pDecoder); }
    HRESULT ret = GetRealObj()->DecoderEndFrame(HookedToReal(pDecoder));
    if (_DecoderEndFrame_post_ptr._value) { (this->*_DecoderEndFrame_post_ptr._value)(ret, pDecoder); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::SubmitDecoderBuffers(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  UINT NumBuffers,
    _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::SubmitDecoderBuffers");
    if (_SubmitDecoderBuffers_pre_ptr._value) { (this->*_SubmitDecoderBuffers_pre_ptr._value)(pDecoder, NumBuffers, pBufferDesc); }
    HRESULT ret = GetRealObj()->SubmitDecoderBuffers(HookedToReal(pDecoder), NumBuffers, pBufferDesc);
    if (_SubmitDecoderBuffers_post_ptr._value) { (this->*_SubmitDecoderBuffers_post_ptr._value)(ret, pDecoder, NumBuffers, pBufferDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::DecoderExtension(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::DecoderExtension");
    if (_DecoderExtension_pre_ptr._value) { (this->*_DecoderExtension_pre_ptr._value)(pDecoder, pExtensionData); }
    HRESULT ret = GetRealObj()->DecoderExtension(HookedToReal(pDecoder), pExtensionData);
    if (_DecoderExtension_post_ptr._value) { (this->*_DecoderExtension_post_ptr._value)(ret, pDecoder, pExtensionData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputTargetRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputTargetRect");
    if (_VideoProcessorSetOutputTargetRect_pre_ptr._value) { (this->*_VideoProcessorSetOutputTargetRect_pre_ptr._value)(pVideoProcessor, Enable, pRect); }
    GetRealObj()->VideoProcessorSetOutputTargetRect(HookedToReal(pVideoProcessor), Enable, pRect);
    if (_VideoProcessorSetOutputTargetRect_post_ptr._value) { (this->*_VideoProcessorSetOutputTargetRect_post_ptr._value)(pVideoProcessor, Enable, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputBackgroundColor(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL YCbCr,
    _In_  const D3D11_VIDEO_COLOR * pColor)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputBackgroundColor");
    if (_VideoProcessorSetOutputBackgroundColor_pre_ptr._value) { (this->*_VideoProcessorSetOutputBackgroundColor_pre_ptr._value)(pVideoProcessor, YCbCr, pColor); }
    GetRealObj()->VideoProcessorSetOutputBackgroundColor(HookedToReal(pVideoProcessor), YCbCr, pColor);
    if (_VideoProcessorSetOutputBackgroundColor_post_ptr._value) { (this->*_VideoProcessorSetOutputBackgroundColor_post_ptr._value)(pVideoProcessor, YCbCr, pColor); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputColorSpace");
    if (_VideoProcessorSetOutputColorSpace_pre_ptr._value) { (this->*_VideoProcessorSetOutputColorSpace_pre_ptr._value)(pVideoProcessor, pColorSpace); }
    GetRealObj()->VideoProcessorSetOutputColorSpace(HookedToReal(pVideoProcessor), pColorSpace);
    if (_VideoProcessorSetOutputColorSpace_post_ptr._value) { (this->*_VideoProcessorSetOutputColorSpace_post_ptr._value)(pVideoProcessor, pColorSpace); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputAlphaFillMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode,
    _In_  UINT StreamIndex)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputAlphaFillMode");
    if (_VideoProcessorSetOutputAlphaFillMode_pre_ptr._value) { (this->*_VideoProcessorSetOutputAlphaFillMode_pre_ptr._value)(pVideoProcessor, AlphaFillMode, StreamIndex); }
    GetRealObj()->VideoProcessorSetOutputAlphaFillMode(HookedToReal(pVideoProcessor), AlphaFillMode, StreamIndex);
    if (_VideoProcessorSetOutputAlphaFillMode_post_ptr._value) { (this->*_VideoProcessorSetOutputAlphaFillMode_post_ptr._value)(pVideoProcessor, AlphaFillMode, StreamIndex); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputConstriction(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable,
    _In_  SIZE Size)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputConstriction");
    if (_VideoProcessorSetOutputConstriction_pre_ptr._value) { (this->*_VideoProcessorSetOutputConstriction_pre_ptr._value)(pVideoProcessor, Enable, Size); }
    GetRealObj()->VideoProcessorSetOutputConstriction(HookedToReal(pVideoProcessor), Enable, Size);
    if (_VideoProcessorSetOutputConstriction_post_ptr._value) { (this->*_VideoProcessorSetOutputConstriction_post_ptr._value)(pVideoProcessor, Enable, Size); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputStereoMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputStereoMode");
    if (_VideoProcessorSetOutputStereoMode_pre_ptr._value) { (this->*_VideoProcessorSetOutputStereoMode_pre_ptr._value)(pVideoProcessor, Enable); }
    GetRealObj()->VideoProcessorSetOutputStereoMode(HookedToReal(pVideoProcessor), Enable);
    if (_VideoProcessorSetOutputStereoMode_post_ptr._value) { (this->*_VideoProcessorSetOutputStereoMode_post_ptr._value)(pVideoProcessor, Enable); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _In_  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputExtension");
    if (_VideoProcessorSetOutputExtension_pre_ptr._value) { (this->*_VideoProcessorSetOutputExtension_pre_ptr._value)(pVideoProcessor, pExtensionGuid, DataSize, pData); }
    HRESULT ret = GetRealObj()->VideoProcessorSetOutputExtension(HookedToReal(pVideoProcessor), pExtensionGuid, DataSize, pData);
    if (_VideoProcessorSetOutputExtension_post_ptr._value) { (this->*_VideoProcessorSetOutputExtension_post_ptr._value)(ret, pVideoProcessor, pExtensionGuid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputTargetRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * Enabled,
    _Out_  RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputTargetRect");
    if (_VideoProcessorGetOutputTargetRect_pre_ptr._value) { (this->*_VideoProcessorGetOutputTargetRect_pre_ptr._value)(pVideoProcessor, Enabled, pRect); }
    GetRealObj()->VideoProcessorGetOutputTargetRect(HookedToReal(pVideoProcessor), Enabled, pRect);
    if (_VideoProcessorGetOutputTargetRect_post_ptr._value) { (this->*_VideoProcessorGetOutputTargetRect_post_ptr._value)(pVideoProcessor, Enabled, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputBackgroundColor(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pYCbCr,
    _Out_  D3D11_VIDEO_COLOR * pColor)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputBackgroundColor");
    if (_VideoProcessorGetOutputBackgroundColor_pre_ptr._value) { (this->*_VideoProcessorGetOutputBackgroundColor_pre_ptr._value)(pVideoProcessor, pYCbCr, pColor); }
    GetRealObj()->VideoProcessorGetOutputBackgroundColor(HookedToReal(pVideoProcessor), pYCbCr, pColor);
    if (_VideoProcessorGetOutputBackgroundColor_post_ptr._value) { (this->*_VideoProcessorGetOutputBackgroundColor_post_ptr._value)(pVideoProcessor, pYCbCr, pColor); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputColorSpace");
    if (_VideoProcessorGetOutputColorSpace_pre_ptr._value) { (this->*_VideoProcessorGetOutputColorSpace_pre_ptr._value)(pVideoProcessor, pColorSpace); }
    GetRealObj()->VideoProcessorGetOutputColorSpace(HookedToReal(pVideoProcessor), pColorSpace);
    if (_VideoProcessorGetOutputColorSpace_post_ptr._value) { (this->*_VideoProcessorGetOutputColorSpace_post_ptr._value)(pVideoProcessor, pColorSpace); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputAlphaFillMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode,
    _Out_  UINT * pStreamIndex)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputAlphaFillMode");
    if (_VideoProcessorGetOutputAlphaFillMode_pre_ptr._value) { (this->*_VideoProcessorGetOutputAlphaFillMode_pre_ptr._value)(pVideoProcessor, pAlphaFillMode, pStreamIndex); }
    GetRealObj()->VideoProcessorGetOutputAlphaFillMode(HookedToReal(pVideoProcessor), pAlphaFillMode, pStreamIndex);
    if (_VideoProcessorGetOutputAlphaFillMode_post_ptr._value) { (this->*_VideoProcessorGetOutputAlphaFillMode_post_ptr._value)(pVideoProcessor, pAlphaFillMode, pStreamIndex); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputConstriction(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pEnabled,
    _Out_  SIZE * pSize)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputConstriction");
    if (_VideoProcessorGetOutputConstriction_pre_ptr._value) { (this->*_VideoProcessorGetOutputConstriction_pre_ptr._value)(pVideoProcessor, pEnabled, pSize); }
    GetRealObj()->VideoProcessorGetOutputConstriction(HookedToReal(pVideoProcessor), pEnabled, pSize);
    if (_VideoProcessorGetOutputConstriction_post_ptr._value) { (this->*_VideoProcessorGetOutputConstriction_post_ptr._value)(pVideoProcessor, pEnabled, pSize); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputStereoMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pEnabled)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputStereoMode");
    if (_VideoProcessorGetOutputStereoMode_pre_ptr._value) { (this->*_VideoProcessorGetOutputStereoMode_pre_ptr._value)(pVideoProcessor, pEnabled); }
    GetRealObj()->VideoProcessorGetOutputStereoMode(HookedToReal(pVideoProcessor), pEnabled);
    if (_VideoProcessorGetOutputStereoMode_post_ptr._value) { (this->*_VideoProcessorGetOutputStereoMode_post_ptr._value)(pVideoProcessor, pEnabled); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _Out_  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputExtension");
    if (_VideoProcessorGetOutputExtension_pre_ptr._value) { (this->*_VideoProcessorGetOutputExtension_pre_ptr._value)(pVideoProcessor, pExtensionGuid, DataSize, pData); }
    HRESULT ret = GetRealObj()->VideoProcessorGetOutputExtension(HookedToReal(pVideoProcessor), pExtensionGuid, DataSize, pData);
    if (_VideoProcessorGetOutputExtension_post_ptr._value) { (this->*_VideoProcessorGetOutputExtension_post_ptr._value)(ret, pVideoProcessor, pExtensionGuid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamFrameFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_FRAME_FORMAT FrameFormat)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamFrameFormat");
    if (_VideoProcessorSetStreamFrameFormat_pre_ptr._value) { (this->*_VideoProcessorSetStreamFrameFormat_pre_ptr._value)(pVideoProcessor, StreamIndex, FrameFormat); }
    GetRealObj()->VideoProcessorSetStreamFrameFormat(HookedToReal(pVideoProcessor), StreamIndex, FrameFormat);
    if (_VideoProcessorSetStreamFrameFormat_post_ptr._value) { (this->*_VideoProcessorSetStreamFrameFormat_post_ptr._value)(pVideoProcessor, StreamIndex, FrameFormat); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamColorSpace");
    if (_VideoProcessorSetStreamColorSpace_pre_ptr._value) { (this->*_VideoProcessorSetStreamColorSpace_pre_ptr._value)(pVideoProcessor, StreamIndex, pColorSpace); }
    GetRealObj()->VideoProcessorSetStreamColorSpace(HookedToReal(pVideoProcessor), StreamIndex, pColorSpace);
    if (_VideoProcessorSetStreamColorSpace_post_ptr._value) { (this->*_VideoProcessorSetStreamColorSpace_post_ptr._value)(pVideoProcessor, StreamIndex, pColorSpace); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamOutputRate(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate,
    _In_  BOOL RepeatFrame,
    _In_opt_  const DXGI_RATIONAL * pCustomRate)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamOutputRate");
    if (_VideoProcessorSetStreamOutputRate_pre_ptr._value) { (this->*_VideoProcessorSetStreamOutputRate_pre_ptr._value)(pVideoProcessor, StreamIndex, OutputRate, RepeatFrame, pCustomRate); }
    GetRealObj()->VideoProcessorSetStreamOutputRate(HookedToReal(pVideoProcessor), StreamIndex, OutputRate, RepeatFrame, pCustomRate);
    if (_VideoProcessorSetStreamOutputRate_post_ptr._value) { (this->*_VideoProcessorSetStreamOutputRate_post_ptr._value)(pVideoProcessor, StreamIndex, OutputRate, RepeatFrame, pCustomRate); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamSourceRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamSourceRect");
    if (_VideoProcessorSetStreamSourceRect_pre_ptr._value) { (this->*_VideoProcessorSetStreamSourceRect_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, pRect); }
    GetRealObj()->VideoProcessorSetStreamSourceRect(HookedToReal(pVideoProcessor), StreamIndex, Enable, pRect);
    if (_VideoProcessorSetStreamSourceRect_post_ptr._value) { (this->*_VideoProcessorSetStreamSourceRect_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamDestRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamDestRect");
    if (_VideoProcessorSetStreamDestRect_pre_ptr._value) { (this->*_VideoProcessorSetStreamDestRect_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, pRect); }
    GetRealObj()->VideoProcessorSetStreamDestRect(HookedToReal(pVideoProcessor), StreamIndex, Enable, pRect);
    if (_VideoProcessorSetStreamDestRect_post_ptr._value) { (this->*_VideoProcessorSetStreamDestRect_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamAlpha(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  FLOAT Alpha)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamAlpha");
    if (_VideoProcessorSetStreamAlpha_pre_ptr._value) { (this->*_VideoProcessorSetStreamAlpha_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, Alpha); }
    GetRealObj()->VideoProcessorSetStreamAlpha(HookedToReal(pVideoProcessor), StreamIndex, Enable, Alpha);
    if (_VideoProcessorSetStreamAlpha_post_ptr._value) { (this->*_VideoProcessorSetStreamAlpha_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, Alpha); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamPalette(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  UINT Count,
    _In_reads_opt_(Count)  const UINT * pEntries)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamPalette");
    if (_VideoProcessorSetStreamPalette_pre_ptr._value) { (this->*_VideoProcessorSetStreamPalette_pre_ptr._value)(pVideoProcessor, StreamIndex, Count, pEntries); }
    GetRealObj()->VideoProcessorSetStreamPalette(HookedToReal(pVideoProcessor), StreamIndex, Count, pEntries);
    if (_VideoProcessorSetStreamPalette_post_ptr._value) { (this->*_VideoProcessorSetStreamPalette_post_ptr._value)(pVideoProcessor, StreamIndex, Count, pEntries); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamPixelAspectRatio(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const DXGI_RATIONAL * pSourceAspectRatio,
    _In_opt_  const DXGI_RATIONAL * pDestinationAspectRatio)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamPixelAspectRatio");
    if (_VideoProcessorSetStreamPixelAspectRatio_pre_ptr._value) { (this->*_VideoProcessorSetStreamPixelAspectRatio_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, pSourceAspectRatio, pDestinationAspectRatio); }
    GetRealObj()->VideoProcessorSetStreamPixelAspectRatio(HookedToReal(pVideoProcessor), StreamIndex, Enable, pSourceAspectRatio, pDestinationAspectRatio);
    if (_VideoProcessorSetStreamPixelAspectRatio_post_ptr._value) { (this->*_VideoProcessorSetStreamPixelAspectRatio_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, pSourceAspectRatio, pDestinationAspectRatio); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamLumaKey(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  FLOAT Lower,
    _In_  FLOAT Upper)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamLumaKey");
    if (_VideoProcessorSetStreamLumaKey_pre_ptr._value) { (this->*_VideoProcessorSetStreamLumaKey_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, Lower, Upper); }
    GetRealObj()->VideoProcessorSetStreamLumaKey(HookedToReal(pVideoProcessor), StreamIndex, Enable, Lower, Upper);
    if (_VideoProcessorSetStreamLumaKey_post_ptr._value) { (this->*_VideoProcessorSetStreamLumaKey_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, Lower, Upper); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamStereoFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format,
    _In_  BOOL LeftViewFrame0,
    _In_  BOOL BaseViewFrame0,
    _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode,
    _In_  int MonoOffset)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamStereoFormat");
    if (_VideoProcessorSetStreamStereoFormat_pre_ptr._value) { (this->*_VideoProcessorSetStreamStereoFormat_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, Format, LeftViewFrame0, BaseViewFrame0, FlipMode, MonoOffset); }
    GetRealObj()->VideoProcessorSetStreamStereoFormat(HookedToReal(pVideoProcessor), StreamIndex, Enable, Format, LeftViewFrame0, BaseViewFrame0, FlipMode, MonoOffset);
    if (_VideoProcessorSetStreamStereoFormat_post_ptr._value) { (this->*_VideoProcessorSetStreamStereoFormat_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, Format, LeftViewFrame0, BaseViewFrame0, FlipMode, MonoOffset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamAutoProcessingMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamAutoProcessingMode");
    if (_VideoProcessorSetStreamAutoProcessingMode_pre_ptr._value) { (this->*_VideoProcessorSetStreamAutoProcessingMode_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable); }
    GetRealObj()->VideoProcessorSetStreamAutoProcessingMode(HookedToReal(pVideoProcessor), StreamIndex, Enable);
    if (_VideoProcessorSetStreamAutoProcessingMode_post_ptr._value) { (this->*_VideoProcessorSetStreamAutoProcessingMode_post_ptr._value)(pVideoProcessor, StreamIndex, Enable); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamFilter(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _In_  BOOL Enable,
    _In_  int Level)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamFilter");
    if (_VideoProcessorSetStreamFilter_pre_ptr._value) { (this->*_VideoProcessorSetStreamFilter_pre_ptr._value)(pVideoProcessor, StreamIndex, Filter, Enable, Level); }
    GetRealObj()->VideoProcessorSetStreamFilter(HookedToReal(pVideoProcessor), StreamIndex, Filter, Enable, Level);
    if (_VideoProcessorSetStreamFilter_post_ptr._value) { (this->*_VideoProcessorSetStreamFilter_post_ptr._value)(pVideoProcessor, StreamIndex, Filter, Enable, Level); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _In_  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamExtension");
    if (_VideoProcessorSetStreamExtension_pre_ptr._value) { (this->*_VideoProcessorSetStreamExtension_pre_ptr._value)(pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData); }
    HRESULT ret = GetRealObj()->VideoProcessorSetStreamExtension(HookedToReal(pVideoProcessor), StreamIndex, pExtensionGuid, DataSize, pData);
    if (_VideoProcessorSetStreamExtension_post_ptr._value) { (this->*_VideoProcessorSetStreamExtension_post_ptr._value)(ret, pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamFrameFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_FRAME_FORMAT * pFrameFormat)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamFrameFormat");
    if (_VideoProcessorGetStreamFrameFormat_pre_ptr._value) { (this->*_VideoProcessorGetStreamFrameFormat_pre_ptr._value)(pVideoProcessor, StreamIndex, pFrameFormat); }
    GetRealObj()->VideoProcessorGetStreamFrameFormat(HookedToReal(pVideoProcessor), StreamIndex, pFrameFormat);
    if (_VideoProcessorGetStreamFrameFormat_post_ptr._value) { (this->*_VideoProcessorGetStreamFrameFormat_post_ptr._value)(pVideoProcessor, StreamIndex, pFrameFormat); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamColorSpace");
    if (_VideoProcessorGetStreamColorSpace_pre_ptr._value) { (this->*_VideoProcessorGetStreamColorSpace_pre_ptr._value)(pVideoProcessor, StreamIndex, pColorSpace); }
    GetRealObj()->VideoProcessorGetStreamColorSpace(HookedToReal(pVideoProcessor), StreamIndex, pColorSpace);
    if (_VideoProcessorGetStreamColorSpace_post_ptr._value) { (this->*_VideoProcessorGetStreamColorSpace_post_ptr._value)(pVideoProcessor, StreamIndex, pColorSpace); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamOutputRate(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate,
    _Out_  BOOL * pRepeatFrame,
    _Out_  DXGI_RATIONAL * pCustomRate)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamOutputRate");
    if (_VideoProcessorGetStreamOutputRate_pre_ptr._value) { (this->*_VideoProcessorGetStreamOutputRate_pre_ptr._value)(pVideoProcessor, StreamIndex, pOutputRate, pRepeatFrame, pCustomRate); }
    GetRealObj()->VideoProcessorGetStreamOutputRate(HookedToReal(pVideoProcessor), StreamIndex, pOutputRate, pRepeatFrame, pCustomRate);
    if (_VideoProcessorGetStreamOutputRate_post_ptr._value) { (this->*_VideoProcessorGetStreamOutputRate_post_ptr._value)(pVideoProcessor, StreamIndex, pOutputRate, pRepeatFrame, pCustomRate); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamSourceRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamSourceRect");
    if (_VideoProcessorGetStreamSourceRect_pre_ptr._value) { (this->*_VideoProcessorGetStreamSourceRect_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pRect); }
    GetRealObj()->VideoProcessorGetStreamSourceRect(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pRect);
    if (_VideoProcessorGetStreamSourceRect_post_ptr._value) { (this->*_VideoProcessorGetStreamSourceRect_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamDestRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamDestRect");
    if (_VideoProcessorGetStreamDestRect_pre_ptr._value) { (this->*_VideoProcessorGetStreamDestRect_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pRect); }
    GetRealObj()->VideoProcessorGetStreamDestRect(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pRect);
    if (_VideoProcessorGetStreamDestRect_post_ptr._value) { (this->*_VideoProcessorGetStreamDestRect_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamAlpha(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  FLOAT * pAlpha)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamAlpha");
    if (_VideoProcessorGetStreamAlpha_pre_ptr._value) { (this->*_VideoProcessorGetStreamAlpha_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pAlpha); }
    GetRealObj()->VideoProcessorGetStreamAlpha(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pAlpha);
    if (_VideoProcessorGetStreamAlpha_post_ptr._value) { (this->*_VideoProcessorGetStreamAlpha_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pAlpha); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamPalette(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  UINT Count,
    _Out_writes_(Count)  UINT * pEntries)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamPalette");
    if (_VideoProcessorGetStreamPalette_pre_ptr._value) { (this->*_VideoProcessorGetStreamPalette_pre_ptr._value)(pVideoProcessor, StreamIndex, Count, pEntries); }
    GetRealObj()->VideoProcessorGetStreamPalette(HookedToReal(pVideoProcessor), StreamIndex, Count, pEntries);
    if (_VideoProcessorGetStreamPalette_post_ptr._value) { (this->*_VideoProcessorGetStreamPalette_post_ptr._value)(pVideoProcessor, StreamIndex, Count, pEntries); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamPixelAspectRatio(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  DXGI_RATIONAL * pSourceAspectRatio,
    _Out_  DXGI_RATIONAL * pDestinationAspectRatio)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamPixelAspectRatio");
    if (_VideoProcessorGetStreamPixelAspectRatio_pre_ptr._value) { (this->*_VideoProcessorGetStreamPixelAspectRatio_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pSourceAspectRatio, pDestinationAspectRatio); }
    GetRealObj()->VideoProcessorGetStreamPixelAspectRatio(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pSourceAspectRatio, pDestinationAspectRatio);
    if (_VideoProcessorGetStreamPixelAspectRatio_post_ptr._value) { (this->*_VideoProcessorGetStreamPixelAspectRatio_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pSourceAspectRatio, pDestinationAspectRatio); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamLumaKey(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  FLOAT * pLower,
    _Out_  FLOAT * pUpper)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamLumaKey");
    if (_VideoProcessorGetStreamLumaKey_pre_ptr._value) { (this->*_VideoProcessorGetStreamLumaKey_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pLower, pUpper); }
    GetRealObj()->VideoProcessorGetStreamLumaKey(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pLower, pUpper);
    if (_VideoProcessorGetStreamLumaKey_post_ptr._value) { (this->*_VideoProcessorGetStreamLumaKey_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pLower, pUpper); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamStereoFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnable,
    _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat,
    _Out_  BOOL * pLeftViewFrame0,
    _Out_  BOOL * pBaseViewFrame0,
    _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode,
    _Out_  int * MonoOffset)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamStereoFormat");
    if (_VideoProcessorGetStreamStereoFormat_pre_ptr._value) { (this->*_VideoProcessorGetStreamStereoFormat_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnable, pFormat, pLeftViewFrame0, pBaseViewFrame0, pFlipMode, MonoOffset); }
    GetRealObj()->VideoProcessorGetStreamStereoFormat(HookedToReal(pVideoProcessor), StreamIndex, pEnable, pFormat, pLeftViewFrame0, pBaseViewFrame0, pFlipMode, MonoOffset);
    if (_VideoProcessorGetStreamStereoFormat_post_ptr._value) { (this->*_VideoProcessorGetStreamStereoFormat_post_ptr._value)(pVideoProcessor, StreamIndex, pEnable, pFormat, pLeftViewFrame0, pBaseViewFrame0, pFlipMode, MonoOffset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamAutoProcessingMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamAutoProcessingMode");
    if (_VideoProcessorGetStreamAutoProcessingMode_pre_ptr._value) { (this->*_VideoProcessorGetStreamAutoProcessingMode_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled); }
    GetRealObj()->VideoProcessorGetStreamAutoProcessingMode(HookedToReal(pVideoProcessor), StreamIndex, pEnabled);
    if (_VideoProcessorGetStreamAutoProcessingMode_post_ptr._value) { (this->*_VideoProcessorGetStreamAutoProcessingMode_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamFilter(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _Out_  BOOL * pEnabled,
    _Out_  int * pLevel)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamFilter");
    if (_VideoProcessorGetStreamFilter_pre_ptr._value) { (this->*_VideoProcessorGetStreamFilter_pre_ptr._value)(pVideoProcessor, StreamIndex, Filter, pEnabled, pLevel); }
    GetRealObj()->VideoProcessorGetStreamFilter(HookedToReal(pVideoProcessor), StreamIndex, Filter, pEnabled, pLevel);
    if (_VideoProcessorGetStreamFilter_post_ptr._value) { (this->*_VideoProcessorGetStreamFilter_post_ptr._value)(pVideoProcessor, StreamIndex, Filter, pEnabled, pLevel); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _Out_  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamExtension");
    if (_VideoProcessorGetStreamExtension_pre_ptr._value) { (this->*_VideoProcessorGetStreamExtension_pre_ptr._value)(pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData); }
    HRESULT ret = GetRealObj()->VideoProcessorGetStreamExtension(HookedToReal(pVideoProcessor), StreamIndex, pExtensionGuid, DataSize, pData);
    if (_VideoProcessorGetStreamExtension_post_ptr._value) { (this->*_VideoProcessorGetStreamExtension_post_ptr._value)(ret, pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorBlt(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  ID3D11VideoProcessorOutputView * pView,
    _In_  UINT OutputFrame,
    _In_  UINT StreamCount,
    _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM * pStreams)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorBlt");
    if (_VideoProcessorBlt_pre_ptr._value) { (this->*_VideoProcessorBlt_pre_ptr._value)(pVideoProcessor, pView, OutputFrame, StreamCount, pStreams); }
    HRESULT ret = GetRealObj()->VideoProcessorBlt(HookedToReal(pVideoProcessor), HookedToReal(pView), OutputFrame, StreamCount, pStreams);
    if (_VideoProcessorBlt_post_ptr._value) { (this->*_VideoProcessorBlt_post_ptr._value)(ret, pVideoProcessor, pView, OutputFrame, StreamCount, pStreams); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::NegotiateCryptoSessionKeyExchange(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT DataSize,
    _Inout_updates_bytes_(DataSize)  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::NegotiateCryptoSessionKeyExchange");
    if (_NegotiateCryptoSessionKeyExchange_pre_ptr._value) { (this->*_NegotiateCryptoSessionKeyExchange_pre_ptr._value)(pCryptoSession, DataSize, pData); }
    HRESULT ret = GetRealObj()->NegotiateCryptoSessionKeyExchange(HookedToReal(pCryptoSession), DataSize, pData);
    if (_NegotiateCryptoSessionKeyExchange_post_ptr._value) { (this->*_NegotiateCryptoSessionKeyExchange_post_ptr._value)(ret, pCryptoSession, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::EncryptionBlt(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  ID3D11Texture2D * pSrcSurface,
    _In_  ID3D11Texture2D * pDstSurface,
    _In_  UINT IVSize,
    _In_reads_bytes_opt_(IVSize)  void * pIV)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::EncryptionBlt");
    if (_EncryptionBlt_pre_ptr._value) { (this->*_EncryptionBlt_pre_ptr._value)(pCryptoSession, pSrcSurface, pDstSurface, IVSize, pIV); }
    GetRealObj()->EncryptionBlt(HookedToReal(pCryptoSession), HookedToReal(pSrcSurface), HookedToReal(pDstSurface), IVSize, pIV);
    if (_EncryptionBlt_post_ptr._value) { (this->*_EncryptionBlt_post_ptr._value)(pCryptoSession, pSrcSurface, pDstSurface, IVSize, pIV); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::DecryptionBlt(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  ID3D11Texture2D * pSrcSurface,
    _In_  ID3D11Texture2D * pDstSurface,
    _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo,
    _In_  UINT ContentKeySize,
    _In_reads_bytes_opt_(ContentKeySize)  const void * pContentKey,
    _In_  UINT IVSize,
    _In_reads_bytes_opt_(IVSize)  void * pIV)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::DecryptionBlt");
    if (_DecryptionBlt_pre_ptr._value) { (this->*_DecryptionBlt_pre_ptr._value)(pCryptoSession, pSrcSurface, pDstSurface, pEncryptedBlockInfo, ContentKeySize, pContentKey, IVSize, pIV); }
    GetRealObj()->DecryptionBlt(HookedToReal(pCryptoSession), HookedToReal(pSrcSurface), HookedToReal(pDstSurface), pEncryptedBlockInfo, ContentKeySize, pContentKey, IVSize, pIV);
    if (_DecryptionBlt_post_ptr._value) { (this->*_DecryptionBlt_post_ptr._value)(pCryptoSession, pSrcSurface, pDstSurface, pEncryptedBlockInfo, ContentKeySize, pContentKey, IVSize, pIV); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::StartSessionKeyRefresh(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT RandomNumberSize,
    _Out_writes_bytes_(RandomNumberSize)  void * pRandomNumber)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::StartSessionKeyRefresh");
    if (_StartSessionKeyRefresh_pre_ptr._value) { (this->*_StartSessionKeyRefresh_pre_ptr._value)(pCryptoSession, RandomNumberSize, pRandomNumber); }
    GetRealObj()->StartSessionKeyRefresh(HookedToReal(pCryptoSession), RandomNumberSize, pRandomNumber);
    if (_StartSessionKeyRefresh_post_ptr._value) { (this->*_StartSessionKeyRefresh_post_ptr._value)(pCryptoSession, RandomNumberSize, pRandomNumber); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::FinishSessionKeyRefresh(
    _In_  ID3D11CryptoSession * pCryptoSession)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::FinishSessionKeyRefresh");
    if (_FinishSessionKeyRefresh_pre_ptr._value) { (this->*_FinishSessionKeyRefresh_pre_ptr._value)(pCryptoSession); }
    GetRealObj()->FinishSessionKeyRefresh(HookedToReal(pCryptoSession));
    if (_FinishSessionKeyRefresh_post_ptr._value) { (this->*_FinishSessionKeyRefresh_post_ptr._value)(pCryptoSession); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::GetEncryptionBltKey(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT KeySize,
    _Out_writes_bytes_(KeySize)  void * pReadbackKey)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::GetEncryptionBltKey");
    if (_GetEncryptionBltKey_pre_ptr._value) { (this->*_GetEncryptionBltKey_pre_ptr._value)(pCryptoSession, KeySize, pReadbackKey); }
    HRESULT ret = GetRealObj()->GetEncryptionBltKey(HookedToReal(pCryptoSession), KeySize, pReadbackKey);
    if (_GetEncryptionBltKey_post_ptr._value) { (this->*_GetEncryptionBltKey_post_ptr._value)(ret, pCryptoSession, KeySize, pReadbackKey); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::NegotiateAuthenticatedChannelKeyExchange(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT DataSize,
    _Inout_updates_bytes_(DataSize)  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::NegotiateAuthenticatedChannelKeyExchange");
    if (_NegotiateAuthenticatedChannelKeyExchange_pre_ptr._value) { (this->*_NegotiateAuthenticatedChannelKeyExchange_pre_ptr._value)(pChannel, DataSize, pData); }
    HRESULT ret = GetRealObj()->NegotiateAuthenticatedChannelKeyExchange(HookedToReal(pChannel), DataSize, pData);
    if (_NegotiateAuthenticatedChannelKeyExchange_post_ptr._value) { (this->*_NegotiateAuthenticatedChannelKeyExchange_post_ptr._value)(ret, pChannel, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::QueryAuthenticatedChannel(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT InputSize,
    _In_reads_bytes_(InputSize)  const void * pInput,
    _In_  UINT OutputSize,
    _Out_writes_bytes_(OutputSize)  void * pOutput)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::QueryAuthenticatedChannel");
    if (_QueryAuthenticatedChannel_pre_ptr._value) { (this->*_QueryAuthenticatedChannel_pre_ptr._value)(pChannel, InputSize, pInput, OutputSize, pOutput); }
    HRESULT ret = GetRealObj()->QueryAuthenticatedChannel(HookedToReal(pChannel), InputSize, pInput, OutputSize, pOutput);
    if (_QueryAuthenticatedChannel_post_ptr._value) { (this->*_QueryAuthenticatedChannel_post_ptr._value)(ret, pChannel, InputSize, pInput, OutputSize, pOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::ConfigureAuthenticatedChannel(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT InputSize,
    _In_reads_bytes_(InputSize)  const void * pInput,
    _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::ConfigureAuthenticatedChannel");
    if (_ConfigureAuthenticatedChannel_pre_ptr._value) { (this->*_ConfigureAuthenticatedChannel_pre_ptr._value)(pChannel, InputSize, pInput, pOutput); }
    HRESULT ret = GetRealObj()->ConfigureAuthenticatedChannel(HookedToReal(pChannel), InputSize, pInput, pOutput);
    if (_ConfigureAuthenticatedChannel_post_ptr._value) { (this->*_ConfigureAuthenticatedChannel_post_ptr._value)(ret, pChannel, InputSize, pInput, pOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamRotation(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  D3D11_VIDEO_PROCESSOR_ROTATION Rotation)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamRotation");
    if (_VideoProcessorSetStreamRotation_pre_ptr._value) { (this->*_VideoProcessorSetStreamRotation_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, Rotation); }
    GetRealObj()->VideoProcessorSetStreamRotation(HookedToReal(pVideoProcessor), StreamIndex, Enable, Rotation);
    if (_VideoProcessorSetStreamRotation_post_ptr._value) { (this->*_VideoProcessorSetStreamRotation_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, Rotation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamRotation(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnable,
    _Out_  D3D11_VIDEO_PROCESSOR_ROTATION * pRotation)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamRotation");
    if (_VideoProcessorGetStreamRotation_pre_ptr._value) { (this->*_VideoProcessorGetStreamRotation_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnable, pRotation); }
    GetRealObj()->VideoProcessorGetStreamRotation(HookedToReal(pVideoProcessor), StreamIndex, pEnable, pRotation);
    if (_VideoProcessorGetStreamRotation_post_ptr._value) { (this->*_VideoProcessorGetStreamRotation_post_ptr._value)(pVideoProcessor, StreamIndex, pEnable, pRotation); }
}

