// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetFeatureMask, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(UINT, Mask)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT, STDMETHODCALLTYPE, GetFeatureMask, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPresentPerRenderOpDelay, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(UINT, Milliseconds)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT, STDMETHODCALLTYPE, GetPresentPerRenderOpDelay, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetSwapChain, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_opt_  IDXGISwapChain *, pSwapChain)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetSwapChain, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  IDXGISwapChain **, ppSwapChain)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, ValidateContext, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11DeviceContext *, pContext)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, ReportLiveDeviceObjects, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_RLDO_FLAGS, Flags)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, ValidateContextForDispatch, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11DeviceContext *, pContext)))
