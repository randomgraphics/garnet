// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetAdapter(
    _Out_  IDXGIAdapter ** pAdapter);
NullPtr<void (DXGIDeviceHook::*)(_Out_  IDXGIAdapter ** &)> _GetAdapter_pre_ptr;
NullPtr<void (DXGIDeviceHook::*)(HRESULT, _Out_  IDXGIAdapter **)> _GetAdapter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSurface(
    _In_  const DXGI_SURFACE_DESC * pDesc,
    /* [in] */ UINT NumSurfaces,
    /* [in] */ DXGI_USAGE Usage,
    _In_opt_  const DXGI_SHARED_RESOURCE * pSharedResource,
    _Out_  IDXGISurface ** ppSurface);
NullPtr<void (DXGIDeviceHook::*)(_In_  const DXGI_SURFACE_DESC * &, /* [in] */ UINT &, /* [in] */ DXGI_USAGE &, _In_opt_  const DXGI_SHARED_RESOURCE * &, _Out_  IDXGISurface ** &)> _CreateSurface_pre_ptr;
NullPtr<void (DXGIDeviceHook::*)(HRESULT, _In_  const DXGI_SURFACE_DESC *, /* [in] */ UINT, /* [in] */ DXGI_USAGE, _In_opt_  const DXGI_SHARED_RESOURCE *, _Out_  IDXGISurface **)> _CreateSurface_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE QueryResourceResidency(
    _In_reads_(NumResources)  IUnknown *const * ppResources,
    _Out_writes_(NumResources)  DXGI_RESIDENCY * pResidencyStatus,
    /* [in] */ UINT NumResources);
NullPtr<void (DXGIDeviceHook::*)(_In_reads_(NumResources)  IUnknown *const * &, _Out_writes_(NumResources)  DXGI_RESIDENCY * &, /* [in] */ UINT &)> _QueryResourceResidency_pre_ptr;
NullPtr<void (DXGIDeviceHook::*)(HRESULT, _In_reads_(NumResources)  IUnknown *const *, _Out_writes_(NumResources)  DXGI_RESIDENCY *, /* [in] */ UINT)> _QueryResourceResidency_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetGPUThreadPriority(
    /* [in] */ INT Priority);
NullPtr<void (DXGIDeviceHook::*)(/* [in] */ INT &)> _SetGPUThreadPriority_pre_ptr;
NullPtr<void (DXGIDeviceHook::*)(HRESULT, /* [in] */ INT)> _SetGPUThreadPriority_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetGPUThreadPriority(
    _Out_  INT * pPriority);
NullPtr<void (DXGIDeviceHook::*)(_Out_  INT * &)> _GetGPUThreadPriority_pre_ptr;
NullPtr<void (DXGIDeviceHook::*)(HRESULT, _Out_  INT *)> _GetGPUThreadPriority_post_ptr;
