// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, OfferResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  UINT, NumResources),
    DEFINE_METHOD_PARAMETER(_In_reads_(NumResources)  IDXGIResource *const *, ppResources),
    DEFINE_METHOD_PARAMETER(_In_  DXGI_OFFER_RESOURCE_PRIORITY, Priority)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, ReclaimResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  UINT, NumResources),
    DEFINE_METHOD_PARAMETER(_In_reads_(NumResources)  IDXGIResource *const *, ppResources),
    DEFINE_METHOD_PARAMETER(_Out_writes_all_opt_(NumResources)  BOOL *, pDiscarded)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, EnqueueSetEvent, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  HANDLE, hEvent)))
