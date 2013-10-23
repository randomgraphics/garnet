// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;

protected:

DXGIResourceHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIDeviceSubObject(DXGIDeviceSubObject)
{
    Construct(); 
}

~DXGIResourceHook() {}

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

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIResourceHook(unknown, *DXGIObject, *DXGIDeviceSubObject, realobj);
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
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetSharedHandle(
    _Out_  HANDLE * pSharedHandle);
NullPtr<void (DXGIResourceHook::*)(_Out_  HANDLE * &)> _GetSharedHandle_pre_ptr;
NullPtr<void (DXGIResourceHook::*)(HRESULT, _Out_  HANDLE *)> _GetSharedHandle_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetUsage(
    _Out_  DXGI_USAGE * pUsage);
NullPtr<void (DXGIResourceHook::*)(_Out_  DXGI_USAGE * &)> _GetUsage_pre_ptr;
NullPtr<void (DXGIResourceHook::*)(HRESULT, _Out_  DXGI_USAGE *)> _GetUsage_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetEvictionPriority(
    /* [in] */ UINT EvictionPriority);
NullPtr<void (DXGIResourceHook::*)(/* [in] */ UINT &)> _SetEvictionPriority_pre_ptr;
NullPtr<void (DXGIResourceHook::*)(HRESULT, /* [in] */ UINT)> _SetEvictionPriority_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetEvictionPriority(
    _Out_  UINT * pEvictionPriority);
NullPtr<void (DXGIResourceHook::*)(_Out_  UINT * &)> _GetEvictionPriority_pre_ptr;
NullPtr<void (DXGIResourceHook::*)(HRESULT, _Out_  UINT *)> _GetEvictionPriority_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: