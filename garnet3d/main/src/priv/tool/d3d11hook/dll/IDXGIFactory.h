// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE EnumAdapters(
    /* [in] */ UINT Adapter,
    IDXGIAdapter ** ppAdapter);
NullPtr<void (DXGIFactoryHook::*)(/* [in] */ UINT, IDXGIAdapter **)> _EnumAdapters_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, /* [in] */ UINT, IDXGIAdapter **)> _EnumAdapters_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE MakeWindowAssociation(
    HWND WindowHandle,
    UINT Flags);
NullPtr<void (DXGIFactoryHook::*)(HWND, UINT)> _MakeWindowAssociation_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, HWND, UINT)> _MakeWindowAssociation_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetWindowAssociation(
    HWND * pWindowHandle);
NullPtr<void (DXGIFactoryHook::*)(HWND *)> _GetWindowAssociation_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, HWND *)> _GetWindowAssociation_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSwapChain(
    IUnknown * pDevice,
    DXGI_SWAP_CHAIN_DESC * pDesc,
    IDXGISwapChain ** ppSwapChain);
NullPtr<void (DXGIFactoryHook::*)(IUnknown *, DXGI_SWAP_CHAIN_DESC *, IDXGISwapChain **)> _CreateSwapChain_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, IUnknown *, DXGI_SWAP_CHAIN_DESC *, IDXGISwapChain **)> _CreateSwapChain_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSoftwareAdapter(
    /* [in] */ HMODULE Module,
    IDXGIAdapter ** ppAdapter);
NullPtr<void (DXGIFactoryHook::*)(/* [in] */ HMODULE, IDXGIAdapter **)> _CreateSoftwareAdapter_pre_ptr;
NullPtr<void (DXGIFactoryHook::*)(HRESULT, /* [in] */ HMODULE, IDXGIAdapter **)> _CreateSoftwareAdapter_post_ptr;
