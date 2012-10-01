// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE Present(
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT Flags);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, /* [in] */ UINT &)> _Present_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, /* [in] */ UINT)> _Present_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetBuffer(
    /* [in] */ UINT Buffer,
    REFIID riid,
    void ** ppSurface);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, REFIID, void ** &)> _GetBuffer_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, REFIID, void **)> _GetBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetFullscreenState(
    /* [in] */ BOOL Fullscreen,
    IDXGIOutput * pTarget);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ BOOL &, IDXGIOutput * &)> _SetFullscreenState_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ BOOL, IDXGIOutput *)> _SetFullscreenState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(
    BOOL * pFullscreen,
    IDXGIOutput ** ppTarget);
NullPtr<void (DXGISwapChainHook::*)(BOOL * &, IDXGIOutput ** &)> _GetFullscreenState_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, BOOL *, IDXGIOutput **)> _GetFullscreenState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc(
    DXGI_SWAP_CHAIN_DESC * pDesc);
NullPtr<void (DXGISwapChainHook::*)(DXGI_SWAP_CHAIN_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, DXGI_SWAP_CHAIN_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ResizeBuffers(
    /* [in] */ UINT BufferCount,
    /* [in] */ UINT Width,
    /* [in] */ UINT Height,
    /* [in] */ DXGI_FORMAT NewFormat,
    /* [in] */ UINT SwapChainFlags);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, /* [in] */ UINT &, /* [in] */ UINT &, /* [in] */ DXGI_FORMAT &, /* [in] */ UINT &)> _ResizeBuffers_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, /* [in] */ UINT, /* [in] */ UINT, /* [in] */ DXGI_FORMAT, /* [in] */ UINT)> _ResizeBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ResizeTarget(
    const DXGI_MODE_DESC * pNewTargetParameters);
NullPtr<void (DXGISwapChainHook::*)(const DXGI_MODE_DESC * &)> _ResizeTarget_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, const DXGI_MODE_DESC *)> _ResizeTarget_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(
    IDXGIOutput ** ppOutput);
NullPtr<void (DXGISwapChainHook::*)(IDXGIOutput ** &)> _GetContainingOutput_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, IDXGIOutput **)> _GetContainingOutput_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(
    DXGI_FRAME_STATISTICS * pStats);
NullPtr<void (DXGISwapChainHook::*)(DXGI_FRAME_STATISTICS * &)> _GetFrameStatistics_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, DXGI_FRAME_STATISTICS *)> _GetFrameStatistics_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(
    UINT * pLastPresentCount);
NullPtr<void (DXGISwapChainHook::*)(UINT * &)> _GetLastPresentCount_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, UINT *)> _GetLastPresentCount_post_ptr;
