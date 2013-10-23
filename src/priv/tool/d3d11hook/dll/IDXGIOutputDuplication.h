// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

DXGIObjectHook & _DXGIObject;

protected:

DXGIOutputDuplicationHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _DXGIObject(DXGIObject)
{
    Construct(); 
}

~DXGIOutputDuplicationHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
    if (nullptr == DXGIObject) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIOutputDuplicationHook(unknown, *DXGIObject, realobj);
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
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_OUTDUPL_DESC * pDesc);
NullPtr<void (DXGIOutputDuplicationHook::*)(_Out_  DXGI_OUTDUPL_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (DXGIOutputDuplicationHook::*)(_Out_  DXGI_OUTDUPL_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE AcquireNextFrame(
    _In_  UINT TimeoutInMilliseconds,
    _Out_  DXGI_OUTDUPL_FRAME_INFO * pFrameInfo,
    _Out_  IDXGIResource ** ppDesktopResource);
NullPtr<void (DXGIOutputDuplicationHook::*)(_In_  UINT &, _Out_  DXGI_OUTDUPL_FRAME_INFO * &, _Out_  IDXGIResource ** &)> _AcquireNextFrame_pre_ptr;
NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _In_  UINT, _Out_  DXGI_OUTDUPL_FRAME_INFO *, _Out_  IDXGIResource **)> _AcquireNextFrame_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFrameDirtyRects(
    _In_  UINT DirtyRectsBufferSize,
    _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT * pDirtyRectsBuffer,
    _Out_  UINT * pDirtyRectsBufferSizeRequired);
NullPtr<void (DXGIOutputDuplicationHook::*)(_In_  UINT &, _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT * &, _Out_  UINT * &)> _GetFrameDirtyRects_pre_ptr;
NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT *, _Out_  UINT *)> _GetFrameDirtyRects_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFrameMoveRects(
    _In_  UINT MoveRectsBufferSize,
    _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT * pMoveRectBuffer,
    _Out_  UINT * pMoveRectsBufferSizeRequired);
NullPtr<void (DXGIOutputDuplicationHook::*)(_In_  UINT &, _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT * &, _Out_  UINT * &)> _GetFrameMoveRects_pre_ptr;
NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT *, _Out_  UINT *)> _GetFrameMoveRects_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFramePointerShape(
    _In_  UINT PointerShapeBufferSize,
    _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void * pPointerShapeBuffer,
    _Out_  UINT * pPointerShapeBufferSizeRequired,
    _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO * pPointerShapeInfo);
NullPtr<void (DXGIOutputDuplicationHook::*)(_In_  UINT &, _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void * &, _Out_  UINT * &, _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO * &)> _GetFramePointerShape_pre_ptr;
NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void *, _Out_  UINT *, _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO *)> _GetFramePointerShape_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE MapDesktopSurface(
    _Out_  DXGI_MAPPED_RECT * pLockedRect);
NullPtr<void (DXGIOutputDuplicationHook::*)(_Out_  DXGI_MAPPED_RECT * &)> _MapDesktopSurface_pre_ptr;
NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _Out_  DXGI_MAPPED_RECT *)> _MapDesktopSurface_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE UnMapDesktopSurface();
NullPtr<void (DXGIOutputDuplicationHook::*)()> _UnMapDesktopSurface_pre_ptr;
NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT)> _UnMapDesktopSurface_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ReleaseFrame();
NullPtr<void (DXGIOutputDuplicationHook::*)()> _ReleaseFrame_pre_ptr;
NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT)> _ReleaseFrame_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: