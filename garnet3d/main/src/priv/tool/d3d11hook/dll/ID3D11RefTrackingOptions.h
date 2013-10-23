// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:


protected:

D3D11RefTrackingOptionsHook(UnknownBase & unknown, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
{
    Construct(); 
}

~D3D11RefTrackingOptionsHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11RefTrackingOptionsHook(unknown, realobj);
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
virtual HRESULT STDMETHODCALLTYPE SetTrackingOptions(
    UINT uOptions);
NullPtr<void (D3D11RefTrackingOptionsHook::*)(UINT &)> _SetTrackingOptions_pre_ptr;
NullPtr<void (D3D11RefTrackingOptionsHook::*)(HRESULT, UINT)> _SetTrackingOptions_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: