// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:


protected:

D3D11TracingDeviceHook(UnknownBase & unknown, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
{
    Construct(); 
}

~D3D11TracingDeviceHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11TracingDeviceHook(unknown, realobj);
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
virtual HRESULT STDMETHODCALLTYPE SetShaderTrackingOptionsByType(
    _In_  UINT ResourceTypeFlags,
    _In_  UINT Options);
NullPtr<void (D3D11TracingDeviceHook::*)(_In_  UINT &, _In_  UINT &)> _SetShaderTrackingOptionsByType_pre_ptr;
NullPtr<void (D3D11TracingDeviceHook::*)(HRESULT, _In_  UINT, _In_  UINT)> _SetShaderTrackingOptionsByType_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetShaderTrackingOptions(
    _In_  IUnknown * pShader,
    _In_  UINT Options);
NullPtr<void (D3D11TracingDeviceHook::*)(_In_  IUnknown * &, _In_  UINT &)> _SetShaderTrackingOptions_pre_ptr;
NullPtr<void (D3D11TracingDeviceHook::*)(HRESULT, _In_  IUnknown *, _In_  UINT)> _SetShaderTrackingOptions_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: