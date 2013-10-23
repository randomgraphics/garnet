// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:


protected:

DXGIDisplayControlHook(UnknownBase & unknown, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
{
    Construct(); 
}

~DXGIDisplayControlHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    try
    {
        IUnknown * result = (UnknownBase*)new DXGIDisplayControlHook(unknown, realobj);
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
virtual BOOL STDMETHODCALLTYPE IsStereoEnabled();
NullPtr<void (DXGIDisplayControlHook::*)()> _IsStereoEnabled_pre_ptr;
NullPtr<void (DXGIDisplayControlHook::*)(BOOL)> _IsStereoEnabled_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SetStereoEnabled(
    BOOL enabled);
NullPtr<void (DXGIDisplayControlHook::*)(BOOL &)> _SetStereoEnabled_pre_ptr;
NullPtr<void (DXGIDisplayControlHook::*)(BOOL)> _SetStereoEnabled_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: