// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIOutputDuplicationHook::GetDesc(
    _Out_  DXGI_OUTDUPL_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::AcquireNextFrame(
    _In_  UINT TimeoutInMilliseconds,
    _Out_  DXGI_OUTDUPL_FRAME_INFO * pFrameInfo,
    _Out_  IDXGIResource ** ppDesktopResource)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::AcquireNextFrame");
    if (_AcquireNextFrame_pre_ptr._value) { (this->*_AcquireNextFrame_pre_ptr._value)(TimeoutInMilliseconds, pFrameInfo, ppDesktopResource); }
    HRESULT ret = GetRealObj()->AcquireNextFrame(TimeoutInMilliseconds, pFrameInfo, ppDesktopResource);
    if (ppDesktopResource) *ppDesktopResource = RealToHooked( *ppDesktopResource );
    if (_AcquireNextFrame_post_ptr._value) { (this->*_AcquireNextFrame_post_ptr._value)(ret, TimeoutInMilliseconds, pFrameInfo, ppDesktopResource); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::GetFrameDirtyRects(
    _In_  UINT DirtyRectsBufferSize,
    _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT * pDirtyRectsBuffer,
    _Out_  UINT * pDirtyRectsBufferSizeRequired)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::GetFrameDirtyRects");
    if (_GetFrameDirtyRects_pre_ptr._value) { (this->*_GetFrameDirtyRects_pre_ptr._value)(DirtyRectsBufferSize, pDirtyRectsBuffer, pDirtyRectsBufferSizeRequired); }
    HRESULT ret = GetRealObj()->GetFrameDirtyRects(DirtyRectsBufferSize, pDirtyRectsBuffer, pDirtyRectsBufferSizeRequired);
    if (_GetFrameDirtyRects_post_ptr._value) { (this->*_GetFrameDirtyRects_post_ptr._value)(ret, DirtyRectsBufferSize, pDirtyRectsBuffer, pDirtyRectsBufferSizeRequired); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::GetFrameMoveRects(
    _In_  UINT MoveRectsBufferSize,
    _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT * pMoveRectBuffer,
    _Out_  UINT * pMoveRectsBufferSizeRequired)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::GetFrameMoveRects");
    if (_GetFrameMoveRects_pre_ptr._value) { (this->*_GetFrameMoveRects_pre_ptr._value)(MoveRectsBufferSize, pMoveRectBuffer, pMoveRectsBufferSizeRequired); }
    HRESULT ret = GetRealObj()->GetFrameMoveRects(MoveRectsBufferSize, pMoveRectBuffer, pMoveRectsBufferSizeRequired);
    if (_GetFrameMoveRects_post_ptr._value) { (this->*_GetFrameMoveRects_post_ptr._value)(ret, MoveRectsBufferSize, pMoveRectBuffer, pMoveRectsBufferSizeRequired); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::GetFramePointerShape(
    _In_  UINT PointerShapeBufferSize,
    _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void * pPointerShapeBuffer,
    _Out_  UINT * pPointerShapeBufferSizeRequired,
    _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO * pPointerShapeInfo)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::GetFramePointerShape");
    if (_GetFramePointerShape_pre_ptr._value) { (this->*_GetFramePointerShape_pre_ptr._value)(PointerShapeBufferSize, pPointerShapeBuffer, pPointerShapeBufferSizeRequired, pPointerShapeInfo); }
    HRESULT ret = GetRealObj()->GetFramePointerShape(PointerShapeBufferSize, pPointerShapeBuffer, pPointerShapeBufferSizeRequired, pPointerShapeInfo);
    if (_GetFramePointerShape_post_ptr._value) { (this->*_GetFramePointerShape_post_ptr._value)(ret, PointerShapeBufferSize, pPointerShapeBuffer, pPointerShapeBufferSizeRequired, pPointerShapeInfo); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::MapDesktopSurface(
    _Out_  DXGI_MAPPED_RECT * pLockedRect)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::MapDesktopSurface");
    if (_MapDesktopSurface_pre_ptr._value) { (this->*_MapDesktopSurface_pre_ptr._value)(pLockedRect); }
    HRESULT ret = GetRealObj()->MapDesktopSurface(pLockedRect);
    if (_MapDesktopSurface_post_ptr._value) { (this->*_MapDesktopSurface_post_ptr._value)(ret, pLockedRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::UnMapDesktopSurface()
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::UnMapDesktopSurface");
    if (_UnMapDesktopSurface_pre_ptr._value) { (this->*_UnMapDesktopSurface_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->UnMapDesktopSurface();
    if (_UnMapDesktopSurface_post_ptr._value) { (this->*_UnMapDesktopSurface_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::ReleaseFrame()
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::ReleaseFrame");
    if (_ReleaseFrame_pre_ptr._value) { (this->*_ReleaseFrame_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->ReleaseFrame();
    if (_ReleaseFrame_post_ptr._value) { (this->*_ReleaseFrame_post_ptr._value)(ret); }
    return ret;
}

