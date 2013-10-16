// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, EnumAdapters, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, Adapter),
    DEFINE_METHOD_PARAMETER(_Out_  IDXGIAdapter **, ppAdapter)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, MakeWindowAssociation, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(HWND, WindowHandle),
    DEFINE_METHOD_PARAMETER(UINT, Flags)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetWindowAssociation, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  HWND *, pWindowHandle)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSwapChain, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  IUnknown *, pDevice),
    DEFINE_METHOD_PARAMETER(_In_  DXGI_SWAP_CHAIN_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(_Out_  IDXGISwapChain **, ppSwapChain)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSoftwareAdapter, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(/* [in] */ HMODULE, Module),
    DEFINE_METHOD_PARAMETER(_Out_  IDXGIAdapter **, ppAdapter)))
