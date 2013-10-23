// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:


protected:

D3D11DebugHook(UnknownBase & unknown, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
{
    Construct(); 
}

~D3D11DebugHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11DebugHook(unknown, realobj);
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

// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetFeatureMask(
    UINT Mask);
NullPtr<void (D3D11DebugHook::*)(UINT &)> _SetFeatureMask_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(HRESULT, UINT)> _SetFeatureMask_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetFeatureMask();
NullPtr<void (D3D11DebugHook::*)()> _GetFeatureMask_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(UINT)> _GetFeatureMask_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPresentPerRenderOpDelay(
    UINT Milliseconds);
NullPtr<void (D3D11DebugHook::*)(UINT &)> _SetPresentPerRenderOpDelay_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(HRESULT, UINT)> _SetPresentPerRenderOpDelay_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetPresentPerRenderOpDelay();
NullPtr<void (D3D11DebugHook::*)()> _GetPresentPerRenderOpDelay_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(UINT)> _GetPresentPerRenderOpDelay_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetSwapChain(
    _In_opt_  IDXGISwapChain * pSwapChain);
NullPtr<void (D3D11DebugHook::*)(_In_opt_  IDXGISwapChain * &)> _SetSwapChain_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(HRESULT, _In_opt_  IDXGISwapChain *)> _SetSwapChain_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetSwapChain(
    _Out_  IDXGISwapChain ** ppSwapChain);
NullPtr<void (D3D11DebugHook::*)(_Out_  IDXGISwapChain ** &)> _GetSwapChain_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(HRESULT, _Out_  IDXGISwapChain **)> _GetSwapChain_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ValidateContext(
    _In_  ID3D11DeviceContext * pContext);
NullPtr<void (D3D11DebugHook::*)(_In_  ID3D11DeviceContext * &)> _ValidateContext_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(HRESULT, _In_  ID3D11DeviceContext *)> _ValidateContext_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ReportLiveDeviceObjects(
    D3D11_RLDO_FLAGS Flags);
NullPtr<void (D3D11DebugHook::*)(D3D11_RLDO_FLAGS &)> _ReportLiveDeviceObjects_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(HRESULT, D3D11_RLDO_FLAGS)> _ReportLiveDeviceObjects_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE ValidateContextForDispatch(
    _In_  ID3D11DeviceContext * pContext);
NullPtr<void (D3D11DebugHook::*)(_In_  ID3D11DeviceContext * &)> _ValidateContextForDispatch_pre_ptr;
NullPtr<void (D3D11DebugHook::*)(HRESULT, _In_  ID3D11DeviceContext *)> _ValidateContextForDispatch_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: