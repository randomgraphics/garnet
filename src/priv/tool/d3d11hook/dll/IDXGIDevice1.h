// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIDeviceHook & _DXGIDevice;

protected:

DXGIDevice1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceHook & DXGIDevice, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIDevice(DXGIDevice)
{
    Construct(); 
}

~DXGIDevice1Hook() {}

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

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIDevice1Hook(unknown, *DXGIObject, *DXGIDevice, realobj);
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
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetMaximumFrameLatency(
    /* [in] */ UINT MaxLatency);
NullPtr<void (DXGIDevice1Hook::*)(/* [in] */ UINT &)> _SetMaximumFrameLatency_pre_ptr;
NullPtr<void (DXGIDevice1Hook::*)(HRESULT, /* [in] */ UINT)> _SetMaximumFrameLatency_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetMaximumFrameLatency(
    _Out_  UINT * pMaxLatency);
NullPtr<void (DXGIDevice1Hook::*)(_Out_  UINT * &)> _GetMaximumFrameLatency_pre_ptr;
NullPtr<void (DXGIDevice1Hook::*)(HRESULT, _Out_  UINT *)> _GetMaximumFrameLatency_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: