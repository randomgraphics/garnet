// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIDeviceHook & _DXGIDevice;
DXGIDevice1Hook & _DXGIDevice1;

protected:

DXGIDevice2Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceHook & DXGIDevice, DXGIDevice1Hook & DXGIDevice1, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIDevice(DXGIDevice)
    , _DXGIDevice1(DXGIDevice1)
{
    Construct(); 
}

~DXGIDevice2Hook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
    if (nullptr == DXGIObject) return nullptr;

    DXGIDeviceHook * DXGIDevice = (DXGIDeviceHook *)unknown.GetHookedObj(__uuidof(IDXGIDevice));
    if (nullptr == DXGIDevice) return nullptr;

    DXGIDevice1Hook * DXGIDevice1 = (DXGIDevice1Hook *)unknown.GetHookedObj(__uuidof(IDXGIDevice1));
    if (nullptr == DXGIDevice1) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIDevice2Hook(unknown, *DXGIObject, *DXGIDevice, *DXGIDevice1, realobj);
        result->AddRef();
        return result;
    }
    catch(std::bad_alloc&)
    {
        GN_ERROR(GN::getLogger("GN.d3d11hook"))("Out of memory.");
        return nullptr;
    }
}

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
    return _DXGIObject.SetPrivateData(Name, DataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID Name,
    _In_  const IUnknown * pUnknown)
{
    return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID Name,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_(*pDataSize)  void * pData)
{
    return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetParent(
    _In_  REFIID riid,
    _Out_  void ** ppParent)
{
    return _DXGIObject.GetParent(riid, ppParent);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetAdapter(
    _Out_  IDXGIAdapter ** pAdapter)
{
    return _DXGIDevice.GetAdapter(pAdapter);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateSurface(
    _In_  const DXGI_SURFACE_DESC * pDesc,
    /* [in] */ UINT NumSurfaces,
    /* [in] */ DXGI_USAGE Usage,
    _In_opt_  const DXGI_SHARED_RESOURCE * pSharedResource,
    _Out_  IDXGISurface ** ppSurface)
{
    return _DXGIDevice.CreateSurface(pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE QueryResourceResidency(
    _In_reads_(NumResources)  IUnknown *const * ppResources,
    _Out_writes_(NumResources)  DXGI_RESIDENCY * pResidencyStatus,
    /* [in] */ UINT NumResources)
{
    return _DXGIDevice.QueryResourceResidency(ppResources, pResidencyStatus, NumResources);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetGPUThreadPriority(
    /* [in] */ INT Priority)
{
    return _DXGIDevice.SetGPUThreadPriority(Priority);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetGPUThreadPriority(
    _Out_  INT * pPriority)
{
    return _DXGIDevice.GetGPUThreadPriority(pPriority);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetMaximumFrameLatency(
    /* [in] */ UINT MaxLatency)
{
    return _DXGIDevice1.SetMaximumFrameLatency(MaxLatency);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetMaximumFrameLatency(
    _Out_  UINT * pMaxLatency)
{
    return _DXGIDevice1.GetMaximumFrameLatency(pMaxLatency);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE OfferResources(
    _In_  UINT NumResources,
    _In_reads_(NumResources)  IDXGIResource *const * ppResources,
    _In_  DXGI_OFFER_RESOURCE_PRIORITY Priority);
NullPtr<void (DXGIDevice2Hook::*)(_In_  UINT &, _In_reads_(NumResources)  IDXGIResource *const * &, _In_  DXGI_OFFER_RESOURCE_PRIORITY &)> _OfferResources_pre_ptr;
NullPtr<void (DXGIDevice2Hook::*)(HRESULT, _In_  UINT, _In_reads_(NumResources)  IDXGIResource *const *, _In_  DXGI_OFFER_RESOURCE_PRIORITY)> _OfferResources_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ReclaimResources(
    _In_  UINT NumResources,
    _In_reads_(NumResources)  IDXGIResource *const * ppResources,
    _Out_writes_all_opt_(NumResources)  BOOL * pDiscarded);
NullPtr<void (DXGIDevice2Hook::*)(_In_  UINT &, _In_reads_(NumResources)  IDXGIResource *const * &, _Out_writes_all_opt_(NumResources)  BOOL * &)> _ReclaimResources_pre_ptr;
NullPtr<void (DXGIDevice2Hook::*)(HRESULT, _In_  UINT, _In_reads_(NumResources)  IDXGIResource *const *, _Out_writes_all_opt_(NumResources)  BOOL *)> _ReclaimResources_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE EnqueueSetEvent(
    _In_  HANDLE hEvent);
NullPtr<void (DXGIDevice2Hook::*)(_In_  HANDLE &)> _EnqueueSetEvent_pre_ptr;
NullPtr<void (DXGIDevice2Hook::*)(HRESULT, _In_  HANDLE)> _EnqueueSetEvent_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: