// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, BOOL, STDMETHODCALLTYPE, IsWindowedStereoEnabled, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSwapChainForHwnd, PARAMETER_LIST_6(
    DEFINE_METHOD_PARAMETER(_In_  IUnknown *, pDevice),
    DEFINE_METHOD_PARAMETER(_In_  HWND, hWnd),
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_SWAP_CHAIN_DESC1 *, pDesc),
    DEFINE_METHOD_PARAMETER(_In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *, pFullscreenDesc),
    DEFINE_METHOD_PARAMETER(_In_opt_  IDXGIOutput *, pRestrictToOutput),
    DEFINE_METHOD_PARAMETER(_Out_  IDXGISwapChain1 **, ppSwapChain)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSwapChainForCoreWindow, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  IUnknown *, pDevice),
    DEFINE_METHOD_PARAMETER(_In_  IUnknown *, pWindow),
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_SWAP_CHAIN_DESC1 *, pDesc),
    DEFINE_METHOD_PARAMETER(_In_opt_  IDXGIOutput *, pRestrictToOutput),
    DEFINE_METHOD_PARAMETER(_Out_  IDXGISwapChain1 **, ppSwapChain)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetSharedResourceAdapterLuid, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  HANDLE, hResource),
    DEFINE_METHOD_PARAMETER(_Out_  LUID *, pLuid)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, RegisterStereoStatusWindow, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  HWND, WindowHandle),
    DEFINE_METHOD_PARAMETER(_In_  UINT, wMsg),
    DEFINE_METHOD_PARAMETER(_Out_  DWORD *, pdwCookie)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, RegisterStereoStatusEvent, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  HANDLE, hEvent),
    DEFINE_METHOD_PARAMETER(_Out_  DWORD *, pdwCookie)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, UnregisterStereoStatus, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  DWORD, dwCookie)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, RegisterOcclusionStatusWindow, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  HWND, WindowHandle),
    DEFINE_METHOD_PARAMETER(_In_  UINT, wMsg),
    DEFINE_METHOD_PARAMETER(_Out_  DWORD *, pdwCookie)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, RegisterOcclusionStatusEvent, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  HANDLE, hEvent),
    DEFINE_METHOD_PARAMETER(_Out_  DWORD *, pdwCookie)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, UnregisterOcclusionStatus, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  DWORD, dwCookie)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSwapChainForComposition, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  IUnknown *, pDevice),
    DEFINE_METHOD_PARAMETER(_In_  const DXGI_SWAP_CHAIN_DESC1 *, pDesc),
    DEFINE_METHOD_PARAMETER(_In_opt_  IDXGIOutput *, pRestrictToOutput),
    DEFINE_METHOD_PARAMETER(_Outptr_  IDXGISwapChain1 **, ppSwapChain)))
