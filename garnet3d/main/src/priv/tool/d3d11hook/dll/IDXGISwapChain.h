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
    _In_  REFIID riid,
    _Out_  void ** ppSurface);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, _In_  REFIID, _Out_  void ** &)> _GetBuffer_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, _In_  REFIID, _Out_  void **)> _GetBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetFullscreenState(
    /* [in] */ BOOL Fullscreen,
    _In_opt_  IDXGIOutput * pTarget);
NullPtr<void (DXGISwapChainHook::*)(/* [in] */ BOOL &, _In_opt_  IDXGIOutput * &)> _SetFullscreenState_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ BOOL, _In_opt_  IDXGIOutput *)> _SetFullscreenState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(
    _Out_opt_  BOOL * pFullscreen,
    _Out_opt_  IDXGIOutput ** ppTarget);
NullPtr<void (DXGISwapChainHook::*)(_Out_opt_  BOOL * &, _Out_opt_  IDXGIOutput ** &)> _GetFullscreenState_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_opt_  BOOL *, _Out_opt_  IDXGIOutput **)> _GetFullscreenState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDesc(
    _Out_  DXGI_SWAP_CHAIN_DESC * pDesc);
NullPtr<void (DXGISwapChainHook::*)(_Out_  DXGI_SWAP_CHAIN_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  DXGI_SWAP_CHAIN_DESC *)> _GetDesc_post_ptr;
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
    _In_  const DXGI_MODE_DESC * pNewTargetParameters);
NullPtr<void (DXGISwapChainHook::*)(_In_  const DXGI_MODE_DESC * &)> _ResizeTarget_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _In_  const DXGI_MODE_DESC *)> _ResizeTarget_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(
    _Out_  IDXGIOutput ** ppOutput);
NullPtr<void (DXGISwapChainHook::*)(_Out_  IDXGIOutput ** &)> _GetContainingOutput_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  IDXGIOutput **)> _GetContainingOutput_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats);
NullPtr<void (DXGISwapChainHook::*)(_Out_  DXGI_FRAME_STATISTICS * &)> _GetFrameStatistics_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  DXGI_FRAME_STATISTICS *)> _GetFrameStatistics_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(
    _Out_  UINT * pLastPresentCount);
NullPtr<void (DXGISwapChainHook::*)(_Out_  UINT * &)> _GetLastPresentCount_pre_ptr;
NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  UINT *)> _GetLastPresentCount_post_ptr;
