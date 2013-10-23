// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;
DXGISurfaceHook & _DXGISurface;

protected:

DXGISurface1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, DXGISurfaceHook & DXGISurface, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIDeviceSubObject(DXGIDeviceSubObject)
    , _DXGISurface(DXGISurface)
{
    Construct(); 
}

~DXGISurface1Hook() {}

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

    DXGISurfaceHook * DXGISurface = (DXGISurfaceHook *)unknown.GetHookedObj(__uuidof(IDXGISurface));
    if (nullptr == DXGISurface) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGISurface1Hook(unknown, *DXGIObject, *DXGIDeviceSubObject, *DXGISurface, realobj);
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
HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_SURFACE_DESC * pDesc)
{
    return _DXGISurface.GetDesc(pDesc);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Map(
    _Out_  DXGI_MAPPED_RECT * pLockedRect,
    /* [in] */ UINT MapFlags)
{
    return _DXGISurface.Map(pLockedRect, MapFlags);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Unmap()
{
    return _DXGISurface.Unmap();
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDC(
    /* [in] */ BOOL Discard,
    _Out_  HDC * phdc);
NullPtr<void (DXGISurface1Hook::*)(/* [in] */ BOOL &, _Out_  HDC * &)> _GetDC_pre_ptr;
NullPtr<void (DXGISurface1Hook::*)(HRESULT, /* [in] */ BOOL, _Out_  HDC *)> _GetDC_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ReleaseDC(
    _In_opt_  RECT * pDirtyRect);
NullPtr<void (DXGISurface1Hook::*)(_In_opt_  RECT * &)> _ReleaseDC_pre_ptr;
NullPtr<void (DXGISurface1Hook::*)(HRESULT, _In_opt_  RECT *)> _ReleaseDC_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: