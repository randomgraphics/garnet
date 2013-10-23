// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:


protected:

D3D11SwitchToRefHook(UnknownBase & unknown, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
{
    Construct(); 
}

~D3D11SwitchToRefHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11SwitchToRefHook(unknown, realobj);
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
virtual BOOL STDMETHODCALLTYPE SetUseRef(
    BOOL UseRef);
NullPtr<void (D3D11SwitchToRefHook::*)(BOOL &)> _SetUseRef_pre_ptr;
NullPtr<void (D3D11SwitchToRefHook::*)(BOOL, BOOL)> _SetUseRef_post_ptr;
// -----------------------------------------------------------------------------
virtual BOOL STDMETHODCALLTYPE GetUseRef();
NullPtr<void (D3D11SwitchToRefHook::*)()> _GetUseRef_pre_ptr;
NullPtr<void (D3D11SwitchToRefHook::*)(BOOL)> _GetUseRef_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: