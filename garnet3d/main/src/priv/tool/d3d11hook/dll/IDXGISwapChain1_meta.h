// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDesc1, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_SWAP_CHAIN_DESC1 *, pDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetFullscreenDesc, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC *, pDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetHwnd, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  HWND *, pHwnd)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetCoreWindow, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  REFIID, refiid),
    DEFINE_METHOD_PARAMETER(_Out_  void **, ppUnk)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, Present1, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, SyncInterval),
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, PresentFlags),
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_PRESENT_PARAMETERS *, pPresentParameters)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, BOOL, STDMETHODCALLTYPE, IsTemporaryMonoSupported, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetRestrictToOutput, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  IDXGIOutput **, ppRestrictToOutput)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetBackgroundColor, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_RGBA *, pColor)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetBackgroundColor, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_RGBA *, pColor)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetRotation, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  DXGI_MODE_ROTATION, Rotation)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetRotation, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_MODE_ROTATION *, pRotation)))
