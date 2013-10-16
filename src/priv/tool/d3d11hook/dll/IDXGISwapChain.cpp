// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::Present(
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT Flags)
{
    if (_Present_pre_ptr._value) { (this->*_Present_pre_ptr._value)(SyncInterval, Flags); }
    HRESULT ret = GetRealObj()->Present(SyncInterval, Flags);
    if (_Present_post_ptr._value) { (this->*_Present_post_ptr._value)(ret, SyncInterval, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetBuffer(
    /* [in] */ UINT Buffer,
    _In_  REFIID riid,
    _Out_  void ** ppSurface)
{
    if (_GetBuffer_pre_ptr._value) { (this->*_GetBuffer_pre_ptr._value)(Buffer, riid, ppSurface); }
    HRESULT ret = GetRealObj()->GetBuffer(Buffer, riid, ppSurface);
    if (_GetBuffer_post_ptr._value) { (this->*_GetBuffer_post_ptr._value)(ret, Buffer, riid, ppSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::SetFullscreenState(
    /* [in] */ BOOL Fullscreen,
    _In_opt_  IDXGIOutput * pTarget)
{
    if (_SetFullscreenState_pre_ptr._value) { (this->*_SetFullscreenState_pre_ptr._value)(Fullscreen, pTarget); }
    HRESULT ret = GetRealObj()->SetFullscreenState(Fullscreen, HookedToReal(pTarget));
    if (_SetFullscreenState_post_ptr._value) { (this->*_SetFullscreenState_post_ptr._value)(ret, Fullscreen, pTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetFullscreenState(
    _Out_opt_  BOOL * pFullscreen,
    _Out_opt_  IDXGIOutput ** ppTarget)
{
    if (_GetFullscreenState_pre_ptr._value) { (this->*_GetFullscreenState_pre_ptr._value)(pFullscreen, ppTarget); }
    HRESULT ret = GetRealObj()->GetFullscreenState(pFullscreen, ppTarget);
    if (ppTarget) *ppTarget = RealToHooked( *ppTarget );
    if (_GetFullscreenState_post_ptr._value) { (this->*_GetFullscreenState_post_ptr._value)(ret, pFullscreen, ppTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetDesc(
    _Out_  DXGI_SWAP_CHAIN_DESC * pDesc)
{
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::ResizeBuffers(
    /* [in] */ UINT BufferCount,
    /* [in] */ UINT Width,
    /* [in] */ UINT Height,
    /* [in] */ DXGI_FORMAT NewFormat,
    /* [in] */ UINT SwapChainFlags)
{
    if (_ResizeBuffers_pre_ptr._value) { (this->*_ResizeBuffers_pre_ptr._value)(BufferCount, Width, Height, NewFormat, SwapChainFlags); }
    HRESULT ret = GetRealObj()->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
    if (_ResizeBuffers_post_ptr._value) { (this->*_ResizeBuffers_post_ptr._value)(ret, BufferCount, Width, Height, NewFormat, SwapChainFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::ResizeTarget(
    _In_  const DXGI_MODE_DESC * pNewTargetParameters)
{
    if (_ResizeTarget_pre_ptr._value) { (this->*_ResizeTarget_pre_ptr._value)(pNewTargetParameters); }
    HRESULT ret = GetRealObj()->ResizeTarget(pNewTargetParameters);
    if (_ResizeTarget_post_ptr._value) { (this->*_ResizeTarget_post_ptr._value)(ret, pNewTargetParameters); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetContainingOutput(
    _Out_  IDXGIOutput ** ppOutput)
{
    if (_GetContainingOutput_pre_ptr._value) { (this->*_GetContainingOutput_pre_ptr._value)(ppOutput); }
    HRESULT ret = GetRealObj()->GetContainingOutput(ppOutput);
    if (ppOutput) *ppOutput = RealToHooked( *ppOutput );
    if (_GetContainingOutput_post_ptr._value) { (this->*_GetContainingOutput_post_ptr._value)(ret, ppOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats)
{
    if (_GetFrameStatistics_pre_ptr._value) { (this->*_GetFrameStatistics_pre_ptr._value)(pStats); }
    HRESULT ret = GetRealObj()->GetFrameStatistics(pStats);
    if (_GetFrameStatistics_post_ptr._value) { (this->*_GetFrameStatistics_post_ptr._value)(ret, pStats); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetLastPresentCount(
    _Out_  UINT * pLastPresentCount)
{
    if (_GetLastPresentCount_pre_ptr._value) { (this->*_GetLastPresentCount_pre_ptr._value)(pLastPresentCount); }
    HRESULT ret = GetRealObj()->GetLastPresentCount(pLastPresentCount);
    if (_GetLastPresentCount_post_ptr._value) { (this->*_GetLastPresentCount_post_ptr._value)(ret, pLastPresentCount); }
    return ret;
}

