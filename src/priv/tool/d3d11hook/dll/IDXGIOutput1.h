// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;
DXGIOutputHook & _DXGIOutput;

protected:

DXGIOutput1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIOutputHook & DXGIOutput, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
    , _DXGIOutput(DXGIOutput)
{
    Construct(); 
}

~DXGIOutput1Hook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
    if (nullptr == DXGIObject) return nullptr;

    DXGIOutputHook * DXGIOutput = (DXGIOutputHook *)unknown.GetHookedObj(__uuidof(IDXGIOutput));
    if (nullptr == DXGIOutput) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIOutput1Hook(unknown, *DXGIObject, *DXGIOutput, realobj);
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
HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_OUTPUT_DESC * pDesc)
{
    return _DXGIOutput.GetDesc(pDesc);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetDisplayModeList(
    /* [in] */ DXGI_FORMAT EnumFormat,
    /* [in] */ UINT Flags,
    _Inout_  UINT * pNumModes,
    _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC * pDesc)
{
    return _DXGIOutput.GetDisplayModeList(EnumFormat, Flags, pNumModes, pDesc);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE FindClosestMatchingMode(
    _In_  const DXGI_MODE_DESC * pModeToMatch,
    _Out_  DXGI_MODE_DESC * pClosestMatch,
    _In_opt_  IUnknown * pConcernedDevice)
{
    return _DXGIOutput.FindClosestMatchingMode(pModeToMatch, pClosestMatch, pConcernedDevice);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE WaitForVBlank()
{
    return _DXGIOutput.WaitForVBlank();
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TakeOwnership(
    _In_  IUnknown * pDevice,
    BOOL Exclusive)
{
    return _DXGIOutput.TakeOwnership(pDevice, Exclusive);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE ReleaseOwnership()
{
    return _DXGIOutput.ReleaseOwnership();
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetGammaControlCapabilities(
    _Out_  DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps)
{
    return _DXGIOutput.GetGammaControlCapabilities(pGammaCaps);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetGammaControl(
    _In_  const DXGI_GAMMA_CONTROL * pArray)
{
    return _DXGIOutput.SetGammaControl(pArray);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetGammaControl(
    _Out_  DXGI_GAMMA_CONTROL * pArray)
{
    return _DXGIOutput.GetGammaControl(pArray);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetDisplaySurface(
    _In_  IDXGISurface * pScanoutSurface)
{
    return _DXGIOutput.SetDisplaySurface(pScanoutSurface);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetDisplaySurfaceData(
    _In_  IDXGISurface * pDestination)
{
    return _DXGIOutput.GetDisplaySurfaceData(pDestination);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats)
{
    return _DXGIOutput.GetFrameStatistics(pStats);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDisplayModeList1(
    /* [in] */ DXGI_FORMAT EnumFormat,
    /* [in] */ UINT Flags,
    _Inout_  UINT * pNumModes,
    _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 * pDesc);
NullPtr<void (DXGIOutput1Hook::*)(/* [in] */ DXGI_FORMAT &, /* [in] */ UINT &, _Inout_  UINT * &, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 * &)> _GetDisplayModeList1_pre_ptr;
NullPtr<void (DXGIOutput1Hook::*)(HRESULT, /* [in] */ DXGI_FORMAT, /* [in] */ UINT, _Inout_  UINT *, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 *)> _GetDisplayModeList1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE FindClosestMatchingMode1(
    _In_  const DXGI_MODE_DESC1 * pModeToMatch,
    _Out_  DXGI_MODE_DESC1 * pClosestMatch,
    _In_opt_  IUnknown * pConcernedDevice);
NullPtr<void (DXGIOutput1Hook::*)(_In_  const DXGI_MODE_DESC1 * &, _Out_  DXGI_MODE_DESC1 * &, _In_opt_  IUnknown * &)> _FindClosestMatchingMode1_pre_ptr;
NullPtr<void (DXGIOutput1Hook::*)(HRESULT, _In_  const DXGI_MODE_DESC1 *, _Out_  DXGI_MODE_DESC1 *, _In_opt_  IUnknown *)> _FindClosestMatchingMode1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDisplaySurfaceData1(
    _In_  IDXGIResource * pDestination);
NullPtr<void (DXGIOutput1Hook::*)(_In_  IDXGIResource * &)> _GetDisplaySurfaceData1_pre_ptr;
NullPtr<void (DXGIOutput1Hook::*)(HRESULT, _In_  IDXGIResource *)> _GetDisplaySurfaceData1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE DuplicateOutput(
    _In_  IUnknown * pDevice,
    _Out_  IDXGIOutputDuplication ** ppOutputDuplication);
NullPtr<void (DXGIOutput1Hook::*)(_In_  IUnknown * &, _Out_  IDXGIOutputDuplication ** &)> _DuplicateOutput_pre_ptr;
NullPtr<void (DXGIOutput1Hook::*)(HRESULT, _In_  IUnknown *, _Out_  IDXGIOutputDuplication **)> _DuplicateOutput_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: