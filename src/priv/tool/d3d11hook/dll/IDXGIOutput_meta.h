// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDesc, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_OUTPUT_DESC *, pDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDisplayModeList, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(/* [in] */ DXGI_FORMAT, EnumFormat),
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, Flags),
    DEFINE_METHOD_PARAMETER(_Inout_  UINT *, pNumModes),
    DEFINE_METHOD_PARAMETER(_Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC *, pDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, FindClosestMatchingMode, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_MODE_DESC *, pModeToMatch),
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_MODE_DESC *, pClosestMatch),
    DEFINE_METHOD_PARAMETER(_In_opt_  IUnknown *, pConcernedDevice)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, WaitForVBlank, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, TakeOwnership, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  IUnknown *, pDevice),
    DEFINE_METHOD_PARAMETER(BOOL, Exclusive)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ReleaseOwnership, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetGammaControlCapabilities, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_GAMMA_CONTROL_CAPABILITIES *, pGammaCaps)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetGammaControl, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_GAMMA_CONTROL *, pArray)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetGammaControl, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_GAMMA_CONTROL *, pArray)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetDisplaySurface, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  IDXGISurface *, pScanoutSurface)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDisplaySurfaceData, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  IDXGISurface *, pDestination)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetFrameStatistics, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_FRAME_STATISTICS *, pStats)))
