// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDisplayModeList1, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(/* [in] */ DXGI_FORMAT, EnumFormat),
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, Flags),
    DEFINE_METHOD_PARAMETER(_Inout_  UINT *, pNumModes),
    DEFINE_METHOD_PARAMETER(_Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 *, pDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, FindClosestMatchingMode1, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_MODE_DESC1 *, pModeToMatch),
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_MODE_DESC1 *, pClosestMatch),
    DEFINE_METHOD_PARAMETER(_In_opt_  IUnknown *, pConcernedDevice)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDisplaySurfaceData1, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  IDXGIResource *, pDestination)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, DuplicateOutput, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  IUnknown *, pDevice),
    DEFINE_METHOD_PARAMETER(_Out_  IDXGIOutputDuplication **, ppOutputDuplication)))
