// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetVideoProcessorContentDesc, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC *, pContentDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CheckVideoProcessorFormat, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  DXGI_FORMAT, Format),
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pFlags)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetVideoProcessorCaps, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_CAPS *, pCaps)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetVideoProcessorRateConversionCaps, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  UINT, TypeIndex),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS *, pCaps)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetVideoProcessorCustomRate, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  UINT, TypeIndex),
    DEFINE_METHOD_PARAMETER(_In_  UINT, CustomRateIndex),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE *, pRate)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetVideoProcessorFilterRange, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  D3D11_VIDEO_PROCESSOR_FILTER, Filter),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE *, pRange)))
