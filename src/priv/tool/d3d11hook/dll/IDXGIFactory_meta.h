// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, EnumAdapters, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(/* [in] */ UINT, Adapter),
    DEFINE_METHOD_PARAMETER(IDXGIAdapter **, ppAdapter)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, MakeWindowAssociation, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(HWND, WindowHandle),
    DEFINE_METHOD_PARAMETER(UINT, Flags)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetWindowAssociation, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(HWND *, pWindowHandle)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSwapChain, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(IUnknown *, pDevice),
    DEFINE_METHOD_PARAMETER(DXGI_SWAP_CHAIN_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(IDXGISwapChain **, ppSwapChain)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSoftwareAdapter, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(/* [in] */ HMODULE, Module),
    DEFINE_METHOD_PARAMETER(IDXGIAdapter **, ppAdapter)))
