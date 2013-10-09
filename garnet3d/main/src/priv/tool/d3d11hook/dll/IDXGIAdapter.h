// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE EnumOutputs(
    /* [in] */ UINT Output,
    _Out_  IDXGIOutput ** ppOutput);
NullPtr<void (DXGIAdapterHook::*)(/* [in] */ UINT &, _Out_  IDXGIOutput ** &)> _EnumOutputs_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, /* [in] */ UINT, _Out_  IDXGIOutput **)> _EnumOutputs_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_ADAPTER_DESC * pDesc);
NullPtr<void (DXGIAdapterHook::*)(_Out_  DXGI_ADAPTER_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, _Out_  DXGI_ADAPTER_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(
    _In_  REFGUID InterfaceName,
    _Out_  LARGE_INTEGER * pUMDVersion);
NullPtr<void (DXGIAdapterHook::*)(_In_  REFGUID, _Out_  LARGE_INTEGER * &)> _CheckInterfaceSupport_pre_ptr;
NullPtr<void (DXGIAdapterHook::*)(HRESULT, _In_  REFGUID, _Out_  LARGE_INTEGER *)> _CheckInterfaceSupport_post_ptr;
