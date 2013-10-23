// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetDesc, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_OUTDUPL_DESC *, pDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, AcquireNextFrame, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  UINT, TimeoutInMilliseconds),
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_OUTDUPL_FRAME_INFO *, pFrameInfo),
    DEFINE_METHOD_PARAMETER(_Out_  IDXGIResource **, ppDesktopResource)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetFrameDirtyRects, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  UINT, DirtyRectsBufferSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT *, pDirtyRectsBuffer),
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pDirtyRectsBufferSizeRequired)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetFrameMoveRects, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  UINT, MoveRectsBufferSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT *, pMoveRectBuffer),
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pMoveRectsBufferSizeRequired)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetFramePointerShape, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  UINT, PointerShapeBufferSize),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void *, pPointerShapeBuffer),
    DEFINE_METHOD_PARAMETER(_Out_  UINT *, pPointerShapeBufferSizeRequired),
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO *, pPointerShapeInfo)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, MapDesktopSurface, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  DXGI_MAPPED_RECT *, pLockedRect)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, UnMapDesktopSurface, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, ReleaseFrame, PARAMETER_LIST_0())
