// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetAdapter, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  IDXGIAdapter **, pAdapter)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSurface, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_SURFACE_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, NumSurfaces),
    DEFINE_METHOD_PARAMETER(/* [in] */ DXGI_USAGE, Usage),
    DEFINE_METHOD_PARAMETER(_In_opt_  const DXGI_SHARED_RESOURCE *, pSharedResource),
    DEFINE_METHOD_PARAMETER(_Out_  IDXGISurface **, ppSurface)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, QueryResourceResidency, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_reads_(NumResources)  IUnknown *const *, ppResources),
    DEFINE_METHOD_PARAMETER(_Out_writes_(NumResources)  DXGI_RESIDENCY *, pResidencyStatus),
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, NumResources)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetGPUThreadPriority, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(/* [in] */ INT, Priority)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetGPUThreadPriority, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  INT *, pPriority)))
