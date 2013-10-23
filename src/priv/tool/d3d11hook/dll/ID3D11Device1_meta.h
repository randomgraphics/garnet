// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetImmediateContext1, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11DeviceContext1 **, ppImmediateContext)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateDeferredContext1, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(UINT, ContextFlags),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11DeviceContext1 **, ppDeferredContext)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateBlendState1, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_BLEND_DESC1 *, pBlendStateDesc),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11BlendState1 **, ppBlendState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateRasterizerState1, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  const D3D11_RASTERIZER_DESC1 *, pRasterizerDesc),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11RasterizerState1 **, ppRasterizerState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateDeviceContextState, PARAMETER_LIST_7(
    DEFINE_METHOD_PARAMETER(UINT, Flags),
    DEFINE_METHOD_PARAMETER(_In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL *, pFeatureLevels),
    DEFINE_METHOD_PARAMETER(UINT, FeatureLevels),
    DEFINE_METHOD_PARAMETER(UINT, SDKVersion),
    DEFINE_METHOD_PARAMETER(REFIID, EmulatedInterface),
    DEFINE_METHOD_PARAMETER(_Out_opt_  D3D_FEATURE_LEVEL *, pChosenFeatureLevel),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3DDeviceContextState **, ppContextState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, OpenSharedResource1, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  HANDLE, hResource),
    DEFINE_METHOD_PARAMETER(_In_  REFIID, returnedInterface),
    DEFINE_METHOD_PARAMETER(_Out_  void **, ppResource)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, OpenSharedResourceByName, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  LPCWSTR, lpName),
    DEFINE_METHOD_PARAMETER(_In_  DWORD, dwDesiredAccess),
    DEFINE_METHOD_PARAMETER(_In_  REFIID, returnedInterface),
    DEFINE_METHOD_PARAMETER(_Out_  void **, ppResource)))
