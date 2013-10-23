// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;

protected:

DXGIOutputHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
{
    unknown.AddInterface<IDXGIOutput>(this, realobj);
    Construct(); 
}

~DXGIOutputHook() {}

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
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_OUTPUT_DESC * pDesc);
NullPtr<void (DXGIOutputHook::*)(_Out_  DXGI_OUTPUT_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _Out_  DXGI_OUTPUT_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDisplayModeList(
    /* [in] */ DXGI_FORMAT EnumFormat,
    /* [in] */ UINT Flags,
    _Inout_  UINT * pNumModes,
    _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC * pDesc);
NullPtr<void (DXGIOutputHook::*)(/* [in] */ DXGI_FORMAT &, /* [in] */ UINT &, _Inout_  UINT * &, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC * &)> _GetDisplayModeList_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, /* [in] */ DXGI_FORMAT, /* [in] */ UINT, _Inout_  UINT *, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC *)> _GetDisplayModeList_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE FindClosestMatchingMode(
    _In_  const DXGI_MODE_DESC * pModeToMatch,
    _Out_  DXGI_MODE_DESC * pClosestMatch,
    _In_opt_  IUnknown * pConcernedDevice);
NullPtr<void (DXGIOutputHook::*)(_In_  const DXGI_MODE_DESC * &, _Out_  DXGI_MODE_DESC * &, _In_opt_  IUnknown * &)> _FindClosestMatchingMode_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  const DXGI_MODE_DESC *, _Out_  DXGI_MODE_DESC *, _In_opt_  IUnknown *)> _FindClosestMatchingMode_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE WaitForVBlank();
NullPtr<void (DXGIOutputHook::*)()> _WaitForVBlank_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT)> _WaitForVBlank_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE TakeOwnership(
    _In_  IUnknown * pDevice,
    BOOL Exclusive);
NullPtr<void (DXGIOutputHook::*)(_In_  IUnknown * &, BOOL &)> _TakeOwnership_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  IUnknown *, BOOL)> _TakeOwnership_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ReleaseOwnership();
NullPtr<void (DXGIOutputHook::*)()> _ReleaseOwnership_pre_ptr;
NullPtr<void (DXGIOutputHook::*)()> _ReleaseOwnership_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetGammaControlCapabilities(
    _Out_  DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps);
NullPtr<void (DXGIOutputHook::*)(_Out_  DXGI_GAMMA_CONTROL_CAPABILITIES * &)> _GetGammaControlCapabilities_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _Out_  DXGI_GAMMA_CONTROL_CAPABILITIES *)> _GetGammaControlCapabilities_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetGammaControl(
    _In_  const DXGI_GAMMA_CONTROL * pArray);
NullPtr<void (DXGIOutputHook::*)(_In_  const DXGI_GAMMA_CONTROL * &)> _SetGammaControl_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  const DXGI_GAMMA_CONTROL *)> _SetGammaControl_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetGammaControl(
    _Out_  DXGI_GAMMA_CONTROL * pArray);
NullPtr<void (DXGIOutputHook::*)(_Out_  DXGI_GAMMA_CONTROL * &)> _GetGammaControl_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _Out_  DXGI_GAMMA_CONTROL *)> _GetGammaControl_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetDisplaySurface(
    _In_  IDXGISurface * pScanoutSurface);
NullPtr<void (DXGIOutputHook::*)(_In_  IDXGISurface * &)> _SetDisplaySurface_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  IDXGISurface *)> _SetDisplaySurface_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDisplaySurfaceData(
    _In_  IDXGISurface * pDestination);
NullPtr<void (DXGIOutputHook::*)(_In_  IDXGISurface * &)> _GetDisplaySurfaceData_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  IDXGISurface *)> _GetDisplaySurfaceData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats);
NullPtr<void (DXGIOutputHook::*)(_Out_  DXGI_FRAME_STATISTICS * &)> _GetFrameStatistics_pre_ptr;
NullPtr<void (DXGIOutputHook::*)(HRESULT, _Out_  DXGI_FRAME_STATISTICS *)> _GetFrameStatistics_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: