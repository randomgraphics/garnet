// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;

protected:

DXGISurfaceHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIDeviceSubObject(DXGIDeviceSubObject)
{
    Construct(); 
}

~DXGISurfaceHook() {}

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
        IUnknown * result = (UnknownBase*)new DXGISurfaceHook(unknown, *DXGIObject, *DXGIDeviceSubObject, realobj);
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
virtual HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_SURFACE_DESC * pDesc);
NullPtr<void (DXGISurfaceHook::*)(_Out_  DXGI_SURFACE_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (DXGISurfaceHook::*)(HRESULT, _Out_  DXGI_SURFACE_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE Map(
    _Out_  DXGI_MAPPED_RECT * pLockedRect,
    /* [in] */ UINT MapFlags);
NullPtr<void (DXGISurfaceHook::*)(_Out_  DXGI_MAPPED_RECT * &, /* [in] */ UINT &)> _Map_pre_ptr;
NullPtr<void (DXGISurfaceHook::*)(HRESULT, _Out_  DXGI_MAPPED_RECT *, /* [in] */ UINT)> _Map_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE Unmap();
NullPtr<void (DXGISurfaceHook::*)()> _Unmap_pre_ptr;
NullPtr<void (DXGISurfaceHook::*)(HRESULT)> _Unmap_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: