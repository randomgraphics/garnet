// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;
DXGIResourceHook & _DXGIResource;

protected:

DXGIResource1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, DXGIResourceHook & DXGIResource, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIDeviceSubObject(DXGIDeviceSubObject)
    , _DXGIResource(DXGIResource)
{
    Construct(); 
}

~DXGIResource1Hook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
    if (nullptr == DXGIObject) return nullptr;

    DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
    if (nullptr == DXGIDeviceSubObject) return nullptr;

    DXGIResourceHook * DXGIResource = (DXGIResourceHook *)unknown.GetHookedObj(__uuidof(IDXGIResource));
    if (nullptr == DXGIResource) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIResource1Hook(unknown, *DXGIObject, *DXGIDeviceSubObject, *DXGIResource, realobj);
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
HRESULT STDMETHODCALLTYPE GetDevice(
    _In_  REFIID riid,
    _Out_  void ** ppDevice)
{
    return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetSharedHandle(
    _Out_  HANDLE * pSharedHandle)
{
    return _DXGIResource.GetSharedHandle(pSharedHandle);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetUsage(
    _Out_  DXGI_USAGE * pUsage)
{
    return _DXGIResource.GetUsage(pUsage);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetEvictionPriority(
    /* [in] */ UINT EvictionPriority)
{
    return _DXGIResource.SetEvictionPriority(EvictionPriority);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetEvictionPriority(
    _Out_  UINT * pEvictionPriority)
{
    return _DXGIResource.GetEvictionPriority(pEvictionPriority);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSubresourceSurface(
    UINT index,
    _Out_  IDXGISurface2 ** ppSurface);
NullPtr<void (DXGIResource1Hook::*)(UINT &, _Out_  IDXGISurface2 ** &)> _CreateSubresourceSurface_pre_ptr;
NullPtr<void (DXGIResource1Hook::*)(HRESULT, UINT, _Out_  IDXGISurface2 **)> _CreateSubresourceSurface_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSharedHandle(
    _In_opt_  const SECURITY_ATTRIBUTES * pAttributes,
    _In_  DWORD dwAccess,
    _In_opt_  LPCWSTR lpName,
    _Out_  HANDLE * pHandle);
NullPtr<void (DXGIResource1Hook::*)(_In_opt_  const SECURITY_ATTRIBUTES * &, _In_  DWORD &, _In_opt_  LPCWSTR &, _Out_  HANDLE * &)> _CreateSharedHandle_pre_ptr;
NullPtr<void (DXGIResource1Hook::*)(HRESULT, _In_opt_  const SECURITY_ATTRIBUTES *, _In_  DWORD, _In_opt_  LPCWSTR, _Out_  HANDLE *)> _CreateSharedHandle_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: