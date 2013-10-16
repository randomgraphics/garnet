// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;

protected:

DXGIDeviceHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
{
    unknown.AddInterface<IDXGIDevice>(this, realobj);
    Construct(); 
}

~DXGIDeviceHook() {}

// ==============================================================================
// Calling to base interfaces
// ==============================================================================
public:

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID Name,
    /* [in] */ UINT DataSize,
    _In_reads_bytes_(DataSize)  const void * pData)
{
    _DXGIObject.SetPrivateData(Name, DataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID Name,
    _In_  const IUnknown * pUnknown)
{
    _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID Name,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_(*pDataSize)  void * pData)
{
    _DXGIObject.GetPrivateData(Name, pDataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetParent(
    _In_  REFIID riid,
    _Out_  void ** ppParent)
{
    _DXGIObject.GetParent(riid, ppParent);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

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

// ==============================================================================
// The End
// ==============================================================================
private: