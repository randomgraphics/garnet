// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE EnumOutputs(
    /* [in] */ UINT Output,
    IDXGIOutput ** ppOutput);
NullPtr<void (DXGIAdapterHook::*)(/* [in] */ UINT, IDXGIOutput **)> _EnumOutputs_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, /* [in] */ UINT, IDXGIOutput **)> _EnumOutputs_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc(
    DXGI_ADAPTER_DESC * pDesc);
NullPtr<void (DXGIAdapterHook::*)(DXGI_ADAPTER_DESC *)> _GetDesc_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, DXGI_ADAPTER_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(
    REFGUID InterfaceName,
    LARGE_INTEGER * pUMDVersion);
NullPtr<void (DXGIAdapterHook::*)(REFGUID, LARGE_INTEGER *)> _CheckInterfaceSupport_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, REFGUID, LARGE_INTEGER *)> _CheckInterfaceSupport_post_ptr;
