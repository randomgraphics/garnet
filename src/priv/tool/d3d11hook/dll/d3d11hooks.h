// script generated file. Do _NOT_ edit.

#include "hooks.h"
#include "d3d/d3d11_1.h"

// ==============================================================================
// ID3D11DeviceChild
// ==============================================================================
class D3D11DeviceChildHook : public HookBase<ID3D11DeviceChild>
{

protected:

    D3D11DeviceChildHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11DeviceChildHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11DeviceChildHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * typedPtr = (D3D11DeviceChildHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice);
    NullPtr<void (D3D11DeviceChildHook::*)(_Out_  ID3D11Device ** &)> _GetDevice_pre_ptr;
    NullPtr<void (D3D11DeviceChildHook::*)(_Out_  ID3D11Device **)> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData);
    NullPtr<void (D3D11DeviceChildHook::*)(_In_  REFGUID, _Inout_  UINT * &, _Out_writes_bytes_opt_( *pDataSize )  void * &)> _GetPrivateData_pre_ptr;
    NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, _In_  REFGUID, _Inout_  UINT *, _Out_writes_bytes_opt_( *pDataSize )  void *)> _GetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData);
    NullPtr<void (D3D11DeviceChildHook::*)(_In_  REFGUID, _In_  UINT &, _In_reads_bytes_opt_( DataSize )  const void * &)> _SetPrivateData_pre_ptr;
    NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, _In_  REFGUID, _In_  UINT, _In_reads_bytes_opt_( DataSize )  const void *)> _SetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData);
    NullPtr<void (D3D11DeviceChildHook::*)(_In_  REFGUID, _In_opt_  const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
    NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, _In_  REFGUID, _In_opt_  const IUnknown *)> _SetPrivateDataInterface_post_ptr;

};

// ==============================================================================
// ID3D11DepthStencilState
// ==============================================================================
class D3D11DepthStencilStateHook : public HookBase<ID3D11DepthStencilState>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11DepthStencilStateHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11DepthStencilStateHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11DepthStencilStateHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DepthStencilStateHook * typedPtr = (D3D11DepthStencilStateHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_DEPTH_STENCIL_DESC * pDesc);
    NullPtr<void (D3D11DepthStencilStateHook::*)(_Out_  D3D11_DEPTH_STENCIL_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11DepthStencilStateHook::*)(_Out_  D3D11_DEPTH_STENCIL_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11BlendState
// ==============================================================================
class D3D11BlendStateHook : public HookBase<ID3D11BlendState>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11BlendStateHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11BlendStateHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11BlendStateHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11BlendStateHook * typedPtr = (D3D11BlendStateHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_BLEND_DESC * pDesc);
    NullPtr<void (D3D11BlendStateHook::*)(_Out_  D3D11_BLEND_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11BlendStateHook::*)(_Out_  D3D11_BLEND_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11RasterizerState
// ==============================================================================
class D3D11RasterizerStateHook : public HookBase<ID3D11RasterizerState>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11RasterizerStateHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11RasterizerStateHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11RasterizerStateHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11RasterizerStateHook * typedPtr = (D3D11RasterizerStateHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_RASTERIZER_DESC * pDesc);
    NullPtr<void (D3D11RasterizerStateHook::*)(_Out_  D3D11_RASTERIZER_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11RasterizerStateHook::*)(_Out_  D3D11_RASTERIZER_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11Resource
// ==============================================================================
class D3D11ResourceHook : public HookBase<ID3D11Resource>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11ResourceHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11ResourceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11ResourceHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11ResourceHook * typedPtr = (D3D11ResourceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetType(_Out_  D3D11_RESOURCE_DIMENSION * pResourceDimension);
    NullPtr<void (D3D11ResourceHook::*)(_Out_  D3D11_RESOURCE_DIMENSION * &)> _GetType_pre_ptr;
    NullPtr<void (D3D11ResourceHook::*)(_Out_  D3D11_RESOURCE_DIMENSION *)> _GetType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetEvictionPriority(_In_  UINT EvictionPriority);
    NullPtr<void (D3D11ResourceHook::*)(_In_  UINT &)> _SetEvictionPriority_pre_ptr;
    NullPtr<void (D3D11ResourceHook::*)(_In_  UINT)> _SetEvictionPriority_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetEvictionPriority();
    NullPtr<void (D3D11ResourceHook::*)()> _GetEvictionPriority_pre_ptr;
    NullPtr<void (D3D11ResourceHook::*)(UINT)> _GetEvictionPriority_post_ptr;

};

// ==============================================================================
// ID3D11Buffer
// ==============================================================================
class D3D11BufferHook : public HookBase<ID3D11Buffer>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ResourceHook & _D3D11Resource;

protected:

    D3D11BufferHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ResourceHook & D3D11Resource,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11Resource(D3D11Resource)
    {
    }

    ~D3D11BufferHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ResourceHook * D3D11Resource = (D3D11ResourceHook *)unknown.GetHookedObj(__uuidof(ID3D11Resource));
        if (nullptr == D3D11Resource) return nullptr;

        try
        {
            return new D3D11BufferHook(unknown, *D3D11DeviceChild, *D3D11Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11BufferHook * typedPtr = (D3D11BufferHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetType(_Out_  D3D11_RESOURCE_DIMENSION * pResourceDimension)
    {
        return _D3D11Resource.GetType(pResourceDimension);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetEvictionPriority(_In_  UINT EvictionPriority)
    {
        return _D3D11Resource.SetEvictionPriority(EvictionPriority);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetEvictionPriority()
    {
        return _D3D11Resource.GetEvictionPriority();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_BUFFER_DESC * pDesc);
    NullPtr<void (D3D11BufferHook::*)(_Out_  D3D11_BUFFER_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11BufferHook::*)(_Out_  D3D11_BUFFER_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11Texture1D
// ==============================================================================
class D3D11Texture1DHook : public HookBase<ID3D11Texture1D>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ResourceHook & _D3D11Resource;

protected:

    D3D11Texture1DHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ResourceHook & D3D11Resource,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11Resource(D3D11Resource)
    {
    }

    ~D3D11Texture1DHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ResourceHook * D3D11Resource = (D3D11ResourceHook *)unknown.GetHookedObj(__uuidof(ID3D11Resource));
        if (nullptr == D3D11Resource) return nullptr;

        try
        {
            return new D3D11Texture1DHook(unknown, *D3D11DeviceChild, *D3D11Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11Texture1DHook * typedPtr = (D3D11Texture1DHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetType(_Out_  D3D11_RESOURCE_DIMENSION * pResourceDimension)
    {
        return _D3D11Resource.GetType(pResourceDimension);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetEvictionPriority(_In_  UINT EvictionPriority)
    {
        return _D3D11Resource.SetEvictionPriority(EvictionPriority);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetEvictionPriority()
    {
        return _D3D11Resource.GetEvictionPriority();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_TEXTURE1D_DESC * pDesc);
    NullPtr<void (D3D11Texture1DHook::*)(_Out_  D3D11_TEXTURE1D_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11Texture1DHook::*)(_Out_  D3D11_TEXTURE1D_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11Texture2D
// ==============================================================================
class D3D11Texture2DHook : public HookBase<ID3D11Texture2D>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ResourceHook & _D3D11Resource;

protected:

    D3D11Texture2DHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ResourceHook & D3D11Resource,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11Resource(D3D11Resource)
    {
    }

    ~D3D11Texture2DHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ResourceHook * D3D11Resource = (D3D11ResourceHook *)unknown.GetHookedObj(__uuidof(ID3D11Resource));
        if (nullptr == D3D11Resource) return nullptr;

        try
        {
            return new D3D11Texture2DHook(unknown, *D3D11DeviceChild, *D3D11Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11Texture2DHook * typedPtr = (D3D11Texture2DHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetType(_Out_  D3D11_RESOURCE_DIMENSION * pResourceDimension)
    {
        return _D3D11Resource.GetType(pResourceDimension);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetEvictionPriority(_In_  UINT EvictionPriority)
    {
        return _D3D11Resource.SetEvictionPriority(EvictionPriority);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetEvictionPriority()
    {
        return _D3D11Resource.GetEvictionPriority();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_TEXTURE2D_DESC * pDesc);
    NullPtr<void (D3D11Texture2DHook::*)(_Out_  D3D11_TEXTURE2D_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11Texture2DHook::*)(_Out_  D3D11_TEXTURE2D_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11Texture3D
// ==============================================================================
class D3D11Texture3DHook : public HookBase<ID3D11Texture3D>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ResourceHook & _D3D11Resource;

protected:

    D3D11Texture3DHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ResourceHook & D3D11Resource,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11Resource(D3D11Resource)
    {
    }

    ~D3D11Texture3DHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ResourceHook * D3D11Resource = (D3D11ResourceHook *)unknown.GetHookedObj(__uuidof(ID3D11Resource));
        if (nullptr == D3D11Resource) return nullptr;

        try
        {
            return new D3D11Texture3DHook(unknown, *D3D11DeviceChild, *D3D11Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11Texture3DHook * typedPtr = (D3D11Texture3DHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetType(_Out_  D3D11_RESOURCE_DIMENSION * pResourceDimension)
    {
        return _D3D11Resource.GetType(pResourceDimension);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetEvictionPriority(_In_  UINT EvictionPriority)
    {
        return _D3D11Resource.SetEvictionPriority(EvictionPriority);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetEvictionPriority()
    {
        return _D3D11Resource.GetEvictionPriority();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_TEXTURE3D_DESC * pDesc);
    NullPtr<void (D3D11Texture3DHook::*)(_Out_  D3D11_TEXTURE3D_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11Texture3DHook::*)(_Out_  D3D11_TEXTURE3D_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11View
// ==============================================================================
class D3D11ViewHook : public HookBase<ID3D11View>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11ViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11ViewHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11ViewHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11ViewHook * typedPtr = (D3D11ViewHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetResource(_Out_  ID3D11Resource ** ppResource);
    NullPtr<void (D3D11ViewHook::*)(_Out_  ID3D11Resource ** &)> _GetResource_pre_ptr;
    NullPtr<void (D3D11ViewHook::*)(_Out_  ID3D11Resource **)> _GetResource_post_ptr;

};

// ==============================================================================
// ID3D11ShaderResourceView
// ==============================================================================
class D3D11ShaderResourceViewHook : public HookBase<ID3D11ShaderResourceView>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ViewHook & _D3D11View;

protected:

    D3D11ShaderResourceViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ViewHook & D3D11View,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11View(D3D11View)
    {
    }

    ~D3D11ShaderResourceViewHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ViewHook * D3D11View = (D3D11ViewHook *)unknown.GetHookedObj(__uuidof(ID3D11View));
        if (nullptr == D3D11View) return nullptr;

        try
        {
            return new D3D11ShaderResourceViewHook(unknown, *D3D11DeviceChild, *D3D11View, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11ShaderResourceViewHook * typedPtr = (D3D11ShaderResourceViewHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetResource(_Out_  ID3D11Resource ** ppResource)
    {
        return _D3D11View.GetResource(ppResource);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc);
    NullPtr<void (D3D11ShaderResourceViewHook::*)(_Out_  D3D11_SHADER_RESOURCE_VIEW_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11ShaderResourceViewHook::*)(_Out_  D3D11_SHADER_RESOURCE_VIEW_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11RenderTargetView
// ==============================================================================
class D3D11RenderTargetViewHook : public HookBase<ID3D11RenderTargetView>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ViewHook & _D3D11View;

protected:

    D3D11RenderTargetViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ViewHook & D3D11View,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11View(D3D11View)
    {
    }

    ~D3D11RenderTargetViewHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ViewHook * D3D11View = (D3D11ViewHook *)unknown.GetHookedObj(__uuidof(ID3D11View));
        if (nullptr == D3D11View) return nullptr;

        try
        {
            return new D3D11RenderTargetViewHook(unknown, *D3D11DeviceChild, *D3D11View, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11RenderTargetViewHook * typedPtr = (D3D11RenderTargetViewHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetResource(_Out_  ID3D11Resource ** ppResource)
    {
        return _D3D11View.GetResource(ppResource);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_RENDER_TARGET_VIEW_DESC * pDesc);
    NullPtr<void (D3D11RenderTargetViewHook::*)(_Out_  D3D11_RENDER_TARGET_VIEW_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11RenderTargetViewHook::*)(_Out_  D3D11_RENDER_TARGET_VIEW_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11DepthStencilView
// ==============================================================================
class D3D11DepthStencilViewHook : public HookBase<ID3D11DepthStencilView>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ViewHook & _D3D11View;

protected:

    D3D11DepthStencilViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ViewHook & D3D11View,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11View(D3D11View)
    {
    }

    ~D3D11DepthStencilViewHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ViewHook * D3D11View = (D3D11ViewHook *)unknown.GetHookedObj(__uuidof(ID3D11View));
        if (nullptr == D3D11View) return nullptr;

        try
        {
            return new D3D11DepthStencilViewHook(unknown, *D3D11DeviceChild, *D3D11View, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DepthStencilViewHook * typedPtr = (D3D11DepthStencilViewHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetResource(_Out_  ID3D11Resource ** ppResource)
    {
        return _D3D11View.GetResource(ppResource);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc);
    NullPtr<void (D3D11DepthStencilViewHook::*)(_Out_  D3D11_DEPTH_STENCIL_VIEW_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11DepthStencilViewHook::*)(_Out_  D3D11_DEPTH_STENCIL_VIEW_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11UnorderedAccessView
// ==============================================================================
class D3D11UnorderedAccessViewHook : public HookBase<ID3D11UnorderedAccessView>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ViewHook & _D3D11View;

protected:

    D3D11UnorderedAccessViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ViewHook & D3D11View,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11View(D3D11View)
    {
    }

    ~D3D11UnorderedAccessViewHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ViewHook * D3D11View = (D3D11ViewHook *)unknown.GetHookedObj(__uuidof(ID3D11View));
        if (nullptr == D3D11View) return nullptr;

        try
        {
            return new D3D11UnorderedAccessViewHook(unknown, *D3D11DeviceChild, *D3D11View, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11UnorderedAccessViewHook * typedPtr = (D3D11UnorderedAccessViewHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetResource(_Out_  ID3D11Resource ** ppResource)
    {
        return _D3D11View.GetResource(ppResource);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc);
    NullPtr<void (D3D11UnorderedAccessViewHook::*)(_Out_  D3D11_UNORDERED_ACCESS_VIEW_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11UnorderedAccessViewHook::*)(_Out_  D3D11_UNORDERED_ACCESS_VIEW_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11VertexShader
// ==============================================================================
class D3D11VertexShaderHook : public HookBase<ID3D11VertexShader>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11VertexShaderHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11VertexShaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11VertexShaderHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VertexShaderHook * typedPtr = (D3D11VertexShaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

};

// ==============================================================================
// ID3D11HullShader
// ==============================================================================
class D3D11HullShaderHook : public HookBase<ID3D11HullShader>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11HullShaderHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11HullShaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11HullShaderHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11HullShaderHook * typedPtr = (D3D11HullShaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

};

// ==============================================================================
// ID3D11DomainShader
// ==============================================================================
class D3D11DomainShaderHook : public HookBase<ID3D11DomainShader>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11DomainShaderHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11DomainShaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11DomainShaderHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DomainShaderHook * typedPtr = (D3D11DomainShaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

};

// ==============================================================================
// ID3D11GeometryShader
// ==============================================================================
class D3D11GeometryShaderHook : public HookBase<ID3D11GeometryShader>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11GeometryShaderHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11GeometryShaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11GeometryShaderHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11GeometryShaderHook * typedPtr = (D3D11GeometryShaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

};

// ==============================================================================
// ID3D11PixelShader
// ==============================================================================
class D3D11PixelShaderHook : public HookBase<ID3D11PixelShader>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11PixelShaderHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11PixelShaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11PixelShaderHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11PixelShaderHook * typedPtr = (D3D11PixelShaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

};

// ==============================================================================
// ID3D11ComputeShader
// ==============================================================================
class D3D11ComputeShaderHook : public HookBase<ID3D11ComputeShader>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11ComputeShaderHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11ComputeShaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11ComputeShaderHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11ComputeShaderHook * typedPtr = (D3D11ComputeShaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

};

// ==============================================================================
// ID3D11InputLayout
// ==============================================================================
class D3D11InputLayoutHook : public HookBase<ID3D11InputLayout>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11InputLayoutHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11InputLayoutHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11InputLayoutHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11InputLayoutHook * typedPtr = (D3D11InputLayoutHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

};

// ==============================================================================
// ID3D11SamplerState
// ==============================================================================
class D3D11SamplerStateHook : public HookBase<ID3D11SamplerState>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11SamplerStateHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11SamplerStateHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11SamplerStateHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11SamplerStateHook * typedPtr = (D3D11SamplerStateHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_SAMPLER_DESC * pDesc);
    NullPtr<void (D3D11SamplerStateHook::*)(_Out_  D3D11_SAMPLER_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11SamplerStateHook::*)(_Out_  D3D11_SAMPLER_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11Asynchronous
// ==============================================================================
class D3D11AsynchronousHook : public HookBase<ID3D11Asynchronous>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11AsynchronousHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11AsynchronousHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11AsynchronousHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11AsynchronousHook * typedPtr = (D3D11AsynchronousHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetDataSize();
    NullPtr<void (D3D11AsynchronousHook::*)()> _GetDataSize_pre_ptr;
    NullPtr<void (D3D11AsynchronousHook::*)(UINT)> _GetDataSize_post_ptr;

};

// ==============================================================================
// ID3D11Query
// ==============================================================================
class D3D11QueryHook : public HookBase<ID3D11Query>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11AsynchronousHook & _D3D11Asynchronous;

protected:

    D3D11QueryHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11AsynchronousHook & D3D11Asynchronous,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11Asynchronous(D3D11Asynchronous)
    {
    }

    ~D3D11QueryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11AsynchronousHook * D3D11Asynchronous = (D3D11AsynchronousHook *)unknown.GetHookedObj(__uuidof(ID3D11Asynchronous));
        if (nullptr == D3D11Asynchronous) return nullptr;

        try
        {
            return new D3D11QueryHook(unknown, *D3D11DeviceChild, *D3D11Asynchronous, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11QueryHook * typedPtr = (D3D11QueryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetDataSize()
    {
        return _D3D11Asynchronous.GetDataSize();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_QUERY_DESC * pDesc);
    NullPtr<void (D3D11QueryHook::*)(_Out_  D3D11_QUERY_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11QueryHook::*)(_Out_  D3D11_QUERY_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11Predicate
// ==============================================================================
class D3D11PredicateHook : public HookBase<ID3D11Predicate>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11AsynchronousHook & _D3D11Asynchronous;
    D3D11QueryHook & _D3D11Query;

protected:

    D3D11PredicateHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11AsynchronousHook & D3D11Asynchronous, D3D11QueryHook & D3D11Query,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11Asynchronous(D3D11Asynchronous)
        , _D3D11Query(D3D11Query)
    {
    }

    ~D3D11PredicateHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11AsynchronousHook * D3D11Asynchronous = (D3D11AsynchronousHook *)unknown.GetHookedObj(__uuidof(ID3D11Asynchronous));
        if (nullptr == D3D11Asynchronous) return nullptr;

        D3D11QueryHook * D3D11Query = (D3D11QueryHook *)unknown.GetHookedObj(__uuidof(ID3D11Query));
        if (nullptr == D3D11Query) return nullptr;

        try
        {
            return new D3D11PredicateHook(unknown, *D3D11DeviceChild, *D3D11Asynchronous, *D3D11Query, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11PredicateHook * typedPtr = (D3D11PredicateHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetDataSize()
    {
        return _D3D11Asynchronous.GetDataSize();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_QUERY_DESC * pDesc)
    {
        return _D3D11Query.GetDesc(pDesc);
    }

};

// ==============================================================================
// ID3D11Counter
// ==============================================================================
class D3D11CounterHook : public HookBase<ID3D11Counter>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11AsynchronousHook & _D3D11Asynchronous;

protected:

    D3D11CounterHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11AsynchronousHook & D3D11Asynchronous,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11Asynchronous(D3D11Asynchronous)
    {
    }

    ~D3D11CounterHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11AsynchronousHook * D3D11Asynchronous = (D3D11AsynchronousHook *)unknown.GetHookedObj(__uuidof(ID3D11Asynchronous));
        if (nullptr == D3D11Asynchronous) return nullptr;

        try
        {
            return new D3D11CounterHook(unknown, *D3D11DeviceChild, *D3D11Asynchronous, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11CounterHook * typedPtr = (D3D11CounterHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetDataSize()
    {
        return _D3D11Asynchronous.GetDataSize();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_COUNTER_DESC * pDesc);
    NullPtr<void (D3D11CounterHook::*)(_Out_  D3D11_COUNTER_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11CounterHook::*)(_Out_  D3D11_COUNTER_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11ClassInstance
// ==============================================================================
class D3D11ClassInstanceHook : public HookBase<ID3D11ClassInstance>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11ClassInstanceHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11ClassInstanceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11ClassInstanceHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11ClassInstanceHook * typedPtr = (D3D11ClassInstanceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetClassLinkage(_Out_  ID3D11ClassLinkage ** ppLinkage);
    NullPtr<void (D3D11ClassInstanceHook::*)(_Out_  ID3D11ClassLinkage ** &)> _GetClassLinkage_pre_ptr;
    NullPtr<void (D3D11ClassInstanceHook::*)(_Out_  ID3D11ClassLinkage **)> _GetClassLinkage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_CLASS_INSTANCE_DESC * pDesc);
    NullPtr<void (D3D11ClassInstanceHook::*)(_Out_  D3D11_CLASS_INSTANCE_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11ClassInstanceHook::*)(_Out_  D3D11_CLASS_INSTANCE_DESC *)> _GetDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetInstanceName(_Out_writes_opt_(*pBufferLength)  LPSTR pInstanceName, _Inout_  SIZE_T * pBufferLength);
    NullPtr<void (D3D11ClassInstanceHook::*)(_Out_writes_opt_(*pBufferLength)  LPSTR &, _Inout_  SIZE_T * &)> _GetInstanceName_pre_ptr;
    NullPtr<void (D3D11ClassInstanceHook::*)(_Out_writes_opt_(*pBufferLength)  LPSTR, _Inout_  SIZE_T *)> _GetInstanceName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetTypeName(_Out_writes_opt_(*pBufferLength)  LPSTR pTypeName, _Inout_  SIZE_T * pBufferLength);
    NullPtr<void (D3D11ClassInstanceHook::*)(_Out_writes_opt_(*pBufferLength)  LPSTR &, _Inout_  SIZE_T * &)> _GetTypeName_pre_ptr;
    NullPtr<void (D3D11ClassInstanceHook::*)(_Out_writes_opt_(*pBufferLength)  LPSTR, _Inout_  SIZE_T *)> _GetTypeName_post_ptr;

};

// ==============================================================================
// ID3D11ClassLinkage
// ==============================================================================
class D3D11ClassLinkageHook : public HookBase<ID3D11ClassLinkage>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11ClassLinkageHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11ClassLinkageHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11ClassLinkageHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11ClassLinkageHook * typedPtr = (D3D11ClassLinkageHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetClassInstance(_In_  LPCSTR pClassInstanceName, _In_  UINT InstanceIndex, _Out_  ID3D11ClassInstance ** ppInstance);
    NullPtr<void (D3D11ClassLinkageHook::*)(_In_  LPCSTR &, _In_  UINT &, _Out_  ID3D11ClassInstance ** &)> _GetClassInstance_pre_ptr;
    NullPtr<void (D3D11ClassLinkageHook::*)(HRESULT, _In_  LPCSTR, _In_  UINT, _Out_  ID3D11ClassInstance **)> _GetClassInstance_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateClassInstance(_In_  LPCSTR pClassTypeName, _In_  UINT ConstantBufferOffset, _In_  UINT ConstantVectorOffset, _In_  UINT TextureOffset, _In_  UINT SamplerOffset, _Out_  ID3D11ClassInstance ** ppInstance);
    NullPtr<void (D3D11ClassLinkageHook::*)(_In_  LPCSTR &, _In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  UINT &, _Out_  ID3D11ClassInstance ** &)> _CreateClassInstance_pre_ptr;
    NullPtr<void (D3D11ClassLinkageHook::*)(HRESULT, _In_  LPCSTR, _In_  UINT, _In_  UINT, _In_  UINT, _In_  UINT, _Out_  ID3D11ClassInstance **)> _CreateClassInstance_post_ptr;

};

// ==============================================================================
// ID3D11CommandList
// ==============================================================================
class D3D11CommandListHook : public HookBase<ID3D11CommandList>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11CommandListHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11CommandListHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11CommandListHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11CommandListHook * typedPtr = (D3D11CommandListHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetContextFlags();
    NullPtr<void (D3D11CommandListHook::*)()> _GetContextFlags_pre_ptr;
    NullPtr<void (D3D11CommandListHook::*)(UINT)> _GetContextFlags_post_ptr;

};

// ==============================================================================
// ID3D11DeviceContext
// ==============================================================================
class D3D11DeviceContextHook : public HookBase<ID3D11DeviceContext>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11DeviceContextHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11DeviceContextHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11DeviceContextHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceContextHook * typedPtr = (D3D11DeviceContextHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _VSSetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _VSSetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _PSSetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _PSSetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSSetShader(_In_opt_  ID3D11PixelShader * pPixelShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11PixelShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _PSSetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11PixelShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _PSSetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _PSSetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _PSSetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSSetShader(_In_opt_  ID3D11VertexShader * pVertexShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11VertexShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _VSSetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11VertexShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _VSSetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawIndexed(_In_  UINT IndexCount, _In_  UINT StartIndexLocation, _In_  INT BaseVertexLocation);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &, _In_  INT &)> _DrawIndexed_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT, _In_  INT)> _DrawIndexed_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE Draw(_In_  UINT VertexCount, _In_  UINT StartVertexLocation);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &)> _Draw_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT)> _Draw_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Map(_In_  ID3D11Resource * pResource, _In_  UINT Subresource, _In_  D3D11_MAP MapType, _In_  UINT MapFlags, _Out_  D3D11_MAPPED_SUBRESOURCE * pMappedResource);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_  D3D11_MAP &, _In_  UINT &, _Out_  D3D11_MAPPED_SUBRESOURCE * &)> _Map_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, _In_  ID3D11Resource *, _In_  UINT, _In_  D3D11_MAP, _In_  UINT, _Out_  D3D11_MAPPED_SUBRESOURCE *)> _Map_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE Unmap(_In_  ID3D11Resource * pResource, _In_  UINT Subresource);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &)> _Unmap_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  UINT)> _Unmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _PSSetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _PSSetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE IASetInputLayout(_In_opt_  ID3D11InputLayout * pInputLayout);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11InputLayout * &)> _IASetInputLayout_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11InputLayout *)> _IASetInputLayout_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE IASetVertexBuffers(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppVertexBuffers, _In_reads_opt_(NumBuffers)  const UINT * pStrides, _In_reads_opt_(NumBuffers)  const UINT * pOffsets);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _IASetVertexBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *, _In_reads_opt_(NumBuffers)  const UINT *)> _IASetVertexBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE IASetIndexBuffer(_In_opt_  ID3D11Buffer * pIndexBuffer, _In_  DXGI_FORMAT Format, _In_  UINT Offset);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11Buffer * &, _In_  DXGI_FORMAT &, _In_  UINT &)> _IASetIndexBuffer_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11Buffer *, _In_  DXGI_FORMAT, _In_  UINT)> _IASetIndexBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawIndexedInstanced(_In_  UINT IndexCountPerInstance, _In_  UINT InstanceCount, _In_  UINT StartIndexLocation, _In_  INT BaseVertexLocation, _In_  UINT StartInstanceLocation);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  INT &, _In_  UINT &)> _DrawIndexedInstanced_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT, _In_  UINT, _In_  INT, _In_  UINT)> _DrawIndexedInstanced_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawInstanced(_In_  UINT VertexCountPerInstance, _In_  UINT InstanceCount, _In_  UINT StartVertexLocation, _In_  UINT StartInstanceLocation);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  UINT &)> _DrawInstanced_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT, _In_  UINT, _In_  UINT)> _DrawInstanced_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _GSSetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _GSSetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSSetShader(_In_opt_  ID3D11GeometryShader * pShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11GeometryShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _GSSetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11GeometryShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _GSSetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE IASetPrimitiveTopology(_In_  D3D11_PRIMITIVE_TOPOLOGY Topology);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  D3D11_PRIMITIVE_TOPOLOGY &)> _IASetPrimitiveTopology_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  D3D11_PRIMITIVE_TOPOLOGY)> _IASetPrimitiveTopology_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _VSSetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _VSSetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _VSSetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _VSSetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE Begin(_In_  ID3D11Asynchronous * pAsync);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous * &)> _Begin_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous *)> _Begin_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE End(_In_  ID3D11Asynchronous * pAsync);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous * &)> _End_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous *)> _End_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetData(_In_  ID3D11Asynchronous * pAsync, _Out_writes_bytes_opt_( DataSize )  void * pData, _In_  UINT DataSize, _In_  UINT GetDataFlags);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous * &, _Out_writes_bytes_opt_( DataSize )  void * &, _In_  UINT &, _In_  UINT &)> _GetData_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, _In_  ID3D11Asynchronous *, _Out_writes_bytes_opt_( DataSize )  void *, _In_  UINT, _In_  UINT)> _GetData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetPredication(_In_opt_  ID3D11Predicate * pPredicate, _In_  BOOL PredicateValue);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11Predicate * &, _In_  BOOL &)> _SetPredication_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11Predicate *, _In_  BOOL)> _SetPredication_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _GSSetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _GSSetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _GSSetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _GSSetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE OMSetRenderTargets(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const * ppRenderTargetViews, _In_opt_  ID3D11DepthStencilView * pDepthStencilView);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT &, _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const * &, _In_opt_  ID3D11DepthStencilView * &)> _OMSetRenderTargets_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const *, _In_opt_  ID3D11DepthStencilView *)> _OMSetRenderTargets_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(_In_  UINT NumRTVs, _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const * ppRenderTargetViews, _In_opt_  ID3D11DepthStencilView * pDepthStencilView, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT UAVStartSlot, _In_  UINT NumUAVs, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews, _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const * &, _In_opt_  ID3D11DepthStencilView * &, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT &, _In_  UINT &, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * &, _In_reads_opt_(NumUAVs)  const UINT * &)> _OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const *, _In_opt_  ID3D11DepthStencilView *, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT, _In_  UINT, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *, _In_reads_opt_(NumUAVs)  const UINT *)> _OMSetRenderTargetsAndUnorderedAccessViews_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE OMSetBlendState(_In_opt_  ID3D11BlendState * pBlendState, _In_opt_  const FLOAT BlendFactor [4], _In_  UINT SampleMask);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11BlendState * &, _In_opt_  const FLOAT *, _In_  UINT &)> _OMSetBlendState_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11BlendState *, _In_opt_  const FLOAT [4], _In_  UINT)> _OMSetBlendState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE OMSetDepthStencilState(_In_opt_  ID3D11DepthStencilState * pDepthStencilState, _In_  UINT StencilRef);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11DepthStencilState * &, _In_  UINT &)> _OMSetDepthStencilState_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11DepthStencilState *, _In_  UINT)> _OMSetDepthStencilState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SOSetTargets(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppSOTargets, _In_reads_opt_(NumBuffers)  const UINT * pOffsets);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _SOSetTargets_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *)> _SOSetTargets_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawAuto();
    NullPtr<void (D3D11DeviceContextHook::*)()> _DrawAuto_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)()> _DrawAuto_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(_In_  ID3D11Buffer * pBufferForArgs, _In_  UINT AlignedByteOffsetForArgs);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer * &, _In_  UINT &)> _DrawIndexedInstancedIndirect_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer *, _In_  UINT)> _DrawIndexedInstancedIndirect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawInstancedIndirect(_In_  ID3D11Buffer * pBufferForArgs, _In_  UINT AlignedByteOffsetForArgs);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer * &, _In_  UINT &)> _DrawInstancedIndirect_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer *, _In_  UINT)> _DrawInstancedIndirect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE Dispatch(_In_  UINT ThreadGroupCountX, _In_  UINT ThreadGroupCountY, _In_  UINT ThreadGroupCountZ);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &, _In_  UINT &)> _Dispatch_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT, _In_  UINT)> _Dispatch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DispatchIndirect(_In_  ID3D11Buffer * pBufferForArgs, _In_  UINT AlignedByteOffsetForArgs);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer * &, _In_  UINT &)> _DispatchIndirect_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer *, _In_  UINT)> _DispatchIndirect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE RSSetState(_In_opt_  ID3D11RasterizerState * pRasterizerState);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11RasterizerState * &)> _RSSetState_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11RasterizerState *)> _RSSetState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE RSSetViewports(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports, _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT * pViewports);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT &, _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT * &)> _RSSetViewports_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT, _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT *)> _RSSetViewports_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE RSSetScissorRects(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects, _In_reads_opt_(NumRects)  const D3D11_RECT * pRects);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT &, _In_reads_opt_(NumRects)  const D3D11_RECT * &)> _RSSetScissorRects_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT, _In_reads_opt_(NumRects)  const D3D11_RECT *)> _RSSetScissorRects_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CopySubresourceRegion(_In_  ID3D11Resource * pDstResource, _In_  UINT DstSubresource, _In_  UINT DstX, _In_  UINT DstY, _In_  UINT DstZ, _In_  ID3D11Resource * pSrcResource, _In_  UINT SrcSubresource, _In_opt_  const D3D11_BOX * pSrcBox);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  ID3D11Resource * &, _In_  UINT &, _In_opt_  const D3D11_BOX * &)> _CopySubresourceRegion_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  UINT, _In_  UINT, _In_  UINT, _In_  UINT, _In_  ID3D11Resource *, _In_  UINT, _In_opt_  const D3D11_BOX *)> _CopySubresourceRegion_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CopyResource(_In_  ID3D11Resource * pDstResource, _In_  ID3D11Resource * pSrcResource);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  ID3D11Resource * &)> _CopyResource_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  ID3D11Resource *)> _CopyResource_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE UpdateSubresource(_In_  ID3D11Resource * pDstResource, _In_  UINT DstSubresource, _In_opt_  const D3D11_BOX * pDstBox, _In_  const void * pSrcData, _In_  UINT SrcRowPitch, _In_  UINT SrcDepthPitch);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_opt_  const D3D11_BOX * &, _In_  const void * &, _In_  UINT &, _In_  UINT &)> _UpdateSubresource_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  UINT, _In_opt_  const D3D11_BOX *, _In_  const void *, _In_  UINT, _In_  UINT)> _UpdateSubresource_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CopyStructureCount(_In_  ID3D11Buffer * pDstBuffer, _In_  UINT DstAlignedByteOffset, _In_  ID3D11UnorderedAccessView * pSrcView);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer * &, _In_  UINT &, _In_  ID3D11UnorderedAccessView * &)> _CopyStructureCount_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer *, _In_  UINT, _In_  ID3D11UnorderedAccessView *)> _CopyStructureCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearRenderTargetView(_In_  ID3D11RenderTargetView * pRenderTargetView, _In_  const FLOAT ColorRGBA [4]);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11RenderTargetView * &, _In_  const FLOAT *)> _ClearRenderTargetView_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11RenderTargetView *, _In_  const FLOAT [4])> _ClearRenderTargetView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(_In_  ID3D11UnorderedAccessView * pUnorderedAccessView, _In_  const UINT Values [4]);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11UnorderedAccessView * &, _In_  const UINT *)> _ClearUnorderedAccessViewUint_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11UnorderedAccessView *, _In_  const UINT [4])> _ClearUnorderedAccessViewUint_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(_In_  ID3D11UnorderedAccessView * pUnorderedAccessView, _In_  const FLOAT Values [4]);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11UnorderedAccessView * &, _In_  const FLOAT *)> _ClearUnorderedAccessViewFloat_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11UnorderedAccessView *, _In_  const FLOAT [4])> _ClearUnorderedAccessViewFloat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearDepthStencilView(_In_  ID3D11DepthStencilView * pDepthStencilView, _In_  UINT ClearFlags, _In_  FLOAT Depth, _In_  UINT8 Stencil);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11DepthStencilView * &, _In_  UINT &, _In_  FLOAT &, _In_  UINT8 &)> _ClearDepthStencilView_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11DepthStencilView *, _In_  UINT, _In_  FLOAT, _In_  UINT8)> _ClearDepthStencilView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GenerateMips(_In_  ID3D11ShaderResourceView * pShaderResourceView);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11ShaderResourceView * &)> _GenerateMips_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11ShaderResourceView *)> _GenerateMips_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetResourceMinLOD(_In_  ID3D11Resource * pResource, FLOAT MinLOD);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, FLOAT &)> _SetResourceMinLOD_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, FLOAT)> _SetResourceMinLOD_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetResourceMinLOD(_In_  ID3D11Resource * pResource);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &)> _GetResourceMinLOD_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(FLOAT, _In_  ID3D11Resource *)> _GetResourceMinLOD_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ResolveSubresource(_In_  ID3D11Resource * pDstResource, _In_  UINT DstSubresource, _In_  ID3D11Resource * pSrcResource, _In_  UINT SrcSubresource, _In_  DXGI_FORMAT Format);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_  ID3D11Resource * &, _In_  UINT &, _In_  DXGI_FORMAT &)> _ResolveSubresource_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  UINT, _In_  ID3D11Resource *, _In_  UINT, _In_  DXGI_FORMAT)> _ResolveSubresource_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ExecuteCommandList(_In_  ID3D11CommandList * pCommandList, BOOL RestoreContextState);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11CommandList * &, BOOL &)> _ExecuteCommandList_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11CommandList *, BOOL)> _ExecuteCommandList_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _HSSetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _HSSetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSSetShader(_In_opt_  ID3D11HullShader * pHullShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11HullShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _HSSetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11HullShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _HSSetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _HSSetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _HSSetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _HSSetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _HSSetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _DSSetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _DSSetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSSetShader(_In_opt_  ID3D11DomainShader * pDomainShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11DomainShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _DSSetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11DomainShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _DSSetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _DSSetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _DSSetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _DSSetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _DSSetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _CSSetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _CSSetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSSetUnorderedAccessViews(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT NumUAVs, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews, _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * &, _In_reads_opt_(NumUAVs)  const UINT * &)> _CSSetUnorderedAccessViews_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *, _In_reads_opt_(NumUAVs)  const UINT *)> _CSSetUnorderedAccessViews_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSSetShader(_In_opt_  ID3D11ComputeShader * pComputeShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11ComputeShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _CSSetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11ComputeShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _CSSetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _CSSetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _CSSetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _CSSetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _CSSetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _VSGetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _VSGetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _PSGetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _PSGetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSGetShader(_Out_  ID3D11PixelShader ** ppPixelShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11PixelShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _PSGetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11PixelShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _PSGetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _PSGetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _PSGetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSGetShader(_Out_  ID3D11VertexShader ** ppVertexShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11VertexShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _VSGetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11VertexShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _VSGetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _PSGetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _PSGetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE IAGetInputLayout(_Out_  ID3D11InputLayout ** ppInputLayout);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11InputLayout ** &)> _IAGetInputLayout_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11InputLayout **)> _IAGetInputLayout_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE IAGetVertexBuffers(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppVertexBuffers, _Out_writes_opt_(NumBuffers)  UINT * pStrides, _Out_writes_opt_(NumBuffers)  UINT * pOffsets);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &, _Out_writes_opt_(NumBuffers)  UINT * &, _Out_writes_opt_(NumBuffers)  UINT * &)> _IAGetVertexBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **, _Out_writes_opt_(NumBuffers)  UINT *, _Out_writes_opt_(NumBuffers)  UINT *)> _IAGetVertexBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE IAGetIndexBuffer(_Out_opt_  ID3D11Buffer ** pIndexBuffer, _Out_opt_  DXGI_FORMAT * Format, _Out_opt_  UINT * Offset);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11Buffer ** &, _Out_opt_  DXGI_FORMAT * &, _Out_opt_  UINT * &)> _IAGetIndexBuffer_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11Buffer **, _Out_opt_  DXGI_FORMAT *, _Out_opt_  UINT *)> _IAGetIndexBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _GSGetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _GSGetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSGetShader(_Out_  ID3D11GeometryShader ** ppGeometryShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11GeometryShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _GSGetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11GeometryShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _GSGetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology(_Out_  D3D11_PRIMITIVE_TOPOLOGY * pTopology);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  D3D11_PRIMITIVE_TOPOLOGY * &)> _IAGetPrimitiveTopology_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  D3D11_PRIMITIVE_TOPOLOGY *)> _IAGetPrimitiveTopology_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _VSGetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _VSGetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _VSGetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _VSGetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetPredication(_Out_opt_  ID3D11Predicate ** ppPredicate, _Out_opt_  BOOL * pPredicateValue);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11Predicate ** &, _Out_opt_  BOOL * &)> _GetPredication_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11Predicate **, _Out_opt_  BOOL *)> _GetPredication_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _GSGetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _GSGetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _GSGetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _GSGetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE OMGetRenderTargets(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11RenderTargetView ** ppRenderTargetViews, _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT &, _Out_writes_opt_(NumViews)  ID3D11RenderTargetView ** &, _Out_opt_  ID3D11DepthStencilView ** &)> _OMGetRenderTargets_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, _Out_writes_opt_(NumViews)  ID3D11RenderTargetView **, _Out_opt_  ID3D11DepthStencilView **)> _OMGetRenderTargets_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumRTVs, _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView ** ppRenderTargetViews, _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT UAVStartSlot, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT NumUAVs, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT &, _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView ** &, _Out_opt_  ID3D11DepthStencilView ** &, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT &, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** &)> _OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView **, _Out_opt_  ID3D11DepthStencilView **, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **)> _OMGetRenderTargetsAndUnorderedAccessViews_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE OMGetBlendState(_Out_opt_  ID3D11BlendState ** ppBlendState, _Out_opt_  FLOAT BlendFactor [4], _Out_opt_  UINT * pSampleMask);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11BlendState ** &, _Out_opt_  FLOAT *, _Out_opt_  UINT * &)> _OMGetBlendState_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11BlendState **, _Out_opt_  FLOAT [4], _Out_opt_  UINT *)> _OMGetBlendState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE OMGetDepthStencilState(_Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState, _Out_opt_  UINT * pStencilRef);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11DepthStencilState ** &, _Out_opt_  UINT * &)> _OMGetDepthStencilState_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11DepthStencilState **, _Out_opt_  UINT *)> _OMGetDepthStencilState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SOGetTargets(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppSOTargets);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _SOGetTargets_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _SOGetTargets_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE RSGetState(_Out_  ID3D11RasterizerState ** ppRasterizerState);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11RasterizerState ** &)> _RSGetState_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11RasterizerState **)> _RSGetState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE RSGetViewports(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumViewports, _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT * pViewports);
    NullPtr<void (D3D11DeviceContextHook::*)(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * &, _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT * &)> _RSGetViewports_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *, _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT *)> _RSGetViewports_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE RSGetScissorRects(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumRects, _Out_writes_opt_(*pNumRects)  D3D11_RECT * pRects);
    NullPtr<void (D3D11DeviceContextHook::*)(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * &, _Out_writes_opt_(*pNumRects)  D3D11_RECT * &)> _RSGetScissorRects_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *, _Out_writes_opt_(*pNumRects)  D3D11_RECT *)> _RSGetScissorRects_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _HSGetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _HSGetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSGetShader(_Out_  ID3D11HullShader ** ppHullShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11HullShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _HSGetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11HullShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _HSGetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _HSGetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _HSGetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _HSGetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _HSGetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _DSGetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _DSGetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSGetShader(_Out_  ID3D11DomainShader ** ppDomainShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11DomainShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _DSGetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11DomainShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _DSGetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _DSGetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _DSGetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _DSGetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _DSGetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _CSGetShaderResources_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _CSGetShaderResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSGetUnorderedAccessViews(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT NumUAVs, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** &)> _CSGetUnorderedAccessViews_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **)> _CSGetUnorderedAccessViews_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSGetShader(_Out_  ID3D11ComputeShader ** ppComputeShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances);
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11ComputeShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _CSGetShader_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11ComputeShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _CSGetShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _CSGetSamplers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _CSGetSamplers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _CSGetConstantBuffers_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _CSGetConstantBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearState();
    NullPtr<void (D3D11DeviceContextHook::*)()> _ClearState_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)()> _ClearState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE Flush();
    NullPtr<void (D3D11DeviceContextHook::*)()> _Flush_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)()> _Flush_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE GetType();
    NullPtr<void (D3D11DeviceContextHook::*)()> _GetType_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(D3D11_DEVICE_CONTEXT_TYPE)> _GetType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetContextFlags();
    NullPtr<void (D3D11DeviceContextHook::*)()> _GetContextFlags_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(UINT)> _GetContextFlags_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FinishCommandList(BOOL RestoreDeferredContextState, _Out_opt_  ID3D11CommandList ** ppCommandList);
    NullPtr<void (D3D11DeviceContextHook::*)(BOOL &, _Out_opt_  ID3D11CommandList ** &)> _FinishCommandList_pre_ptr;
    NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, BOOL, _Out_opt_  ID3D11CommandList **)> _FinishCommandList_post_ptr;

};

// ==============================================================================
// ID3D11VideoDecoder
// ==============================================================================
class D3D11VideoDecoderHook : public HookBase<ID3D11VideoDecoder>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11VideoDecoderHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11VideoDecoderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11VideoDecoderHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VideoDecoderHook * typedPtr = (D3D11VideoDecoderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCreationParameters(_Out_  D3D11_VIDEO_DECODER_DESC * pVideoDesc, _Out_  D3D11_VIDEO_DECODER_CONFIG * pConfig);
    NullPtr<void (D3D11VideoDecoderHook::*)(_Out_  D3D11_VIDEO_DECODER_DESC * &, _Out_  D3D11_VIDEO_DECODER_CONFIG * &)> _GetCreationParameters_pre_ptr;
    NullPtr<void (D3D11VideoDecoderHook::*)(HRESULT, _Out_  D3D11_VIDEO_DECODER_DESC *, _Out_  D3D11_VIDEO_DECODER_CONFIG *)> _GetCreationParameters_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDriverHandle(_Out_  HANDLE * pDriverHandle);
    NullPtr<void (D3D11VideoDecoderHook::*)(_Out_  HANDLE * &)> _GetDriverHandle_pre_ptr;
    NullPtr<void (D3D11VideoDecoderHook::*)(HRESULT, _Out_  HANDLE *)> _GetDriverHandle_post_ptr;

};

// ==============================================================================
// ID3D11VideoProcessorEnumerator
// ==============================================================================
class D3D11VideoProcessorEnumeratorHook : public HookBase<ID3D11VideoProcessorEnumerator>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11VideoProcessorEnumeratorHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11VideoProcessorEnumeratorHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11VideoProcessorEnumeratorHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VideoProcessorEnumeratorHook * typedPtr = (D3D11VideoProcessorEnumeratorHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorContentDesc(_Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc);
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * &)> _GetVideoProcessorContentDesc_pre_ptr;
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC *)> _GetVideoProcessorContentDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckVideoProcessorFormat(_In_  DXGI_FORMAT Format, _Out_  UINT * pFlags);
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_In_  DXGI_FORMAT &, _Out_  UINT * &)> _CheckVideoProcessorFormat_pre_ptr;
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _In_  DXGI_FORMAT, _Out_  UINT *)> _CheckVideoProcessorFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorCaps(_Out_  D3D11_VIDEO_PROCESSOR_CAPS * pCaps);
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_CAPS * &)> _GetVideoProcessorCaps_pre_ptr;
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _Out_  D3D11_VIDEO_PROCESSOR_CAPS *)> _GetVideoProcessorCaps_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorRateConversionCaps(_In_  UINT TypeIndex, _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps);
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_In_  UINT &, _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * &)> _GetVideoProcessorRateConversionCaps_pre_ptr;
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _In_  UINT, _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS *)> _GetVideoProcessorRateConversionCaps_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorCustomRate(_In_  UINT TypeIndex, _In_  UINT CustomRateIndex, _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate);
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_In_  UINT &, _In_  UINT &, _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * &)> _GetVideoProcessorCustomRate_pre_ptr;
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _In_  UINT, _In_  UINT, _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE *)> _GetVideoProcessorCustomRate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorFilterRange(_In_  D3D11_VIDEO_PROCESSOR_FILTER Filter, _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange);
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(_In_  D3D11_VIDEO_PROCESSOR_FILTER &, _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE * &)> _GetVideoProcessorFilterRange_pre_ptr;
    NullPtr<void (D3D11VideoProcessorEnumeratorHook::*)(HRESULT, _In_  D3D11_VIDEO_PROCESSOR_FILTER, _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE *)> _GetVideoProcessorFilterRange_post_ptr;

};

// ==============================================================================
// ID3D11VideoProcessor
// ==============================================================================
class D3D11VideoProcessorHook : public HookBase<ID3D11VideoProcessor>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11VideoProcessorHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11VideoProcessorHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11VideoProcessorHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VideoProcessorHook * typedPtr = (D3D11VideoProcessorHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetContentDesc(_Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc);
    NullPtr<void (D3D11VideoProcessorHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * &)> _GetContentDesc_pre_ptr;
    NullPtr<void (D3D11VideoProcessorHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC *)> _GetContentDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetRateConversionCaps(_Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps);
    NullPtr<void (D3D11VideoProcessorHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * &)> _GetRateConversionCaps_pre_ptr;
    NullPtr<void (D3D11VideoProcessorHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS *)> _GetRateConversionCaps_post_ptr;

};

// ==============================================================================
// ID3D11AuthenticatedChannel
// ==============================================================================
class D3D11AuthenticatedChannelHook : public HookBase<ID3D11AuthenticatedChannel>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11AuthenticatedChannelHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11AuthenticatedChannelHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11AuthenticatedChannelHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11AuthenticatedChannelHook * typedPtr = (D3D11AuthenticatedChannelHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCertificateSize(_Out_  UINT * pCertificateSize);
    NullPtr<void (D3D11AuthenticatedChannelHook::*)(_Out_  UINT * &)> _GetCertificateSize_pre_ptr;
    NullPtr<void (D3D11AuthenticatedChannelHook::*)(HRESULT, _Out_  UINT *)> _GetCertificateSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCertificate(_In_  UINT CertificateSize, _Out_writes_bytes_(CertificateSize)  BYTE * pCertificate);
    NullPtr<void (D3D11AuthenticatedChannelHook::*)(_In_  UINT &, _Out_writes_bytes_(CertificateSize)  BYTE * &)> _GetCertificate_pre_ptr;
    NullPtr<void (D3D11AuthenticatedChannelHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_(CertificateSize)  BYTE *)> _GetCertificate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetChannelHandle(_Out_  HANDLE * pChannelHandle);
    NullPtr<void (D3D11AuthenticatedChannelHook::*)(_Out_  HANDLE * &)> _GetChannelHandle_pre_ptr;
    NullPtr<void (D3D11AuthenticatedChannelHook::*)(_Out_  HANDLE *)> _GetChannelHandle_post_ptr;

};

// ==============================================================================
// ID3D11CryptoSession
// ==============================================================================
class D3D11CryptoSessionHook : public HookBase<ID3D11CryptoSession>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11CryptoSessionHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11CryptoSessionHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11CryptoSessionHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11CryptoSessionHook * typedPtr = (D3D11CryptoSessionHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetCryptoType(_Out_  GUID * pCryptoType);
    NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  GUID * &)> _GetCryptoType_pre_ptr;
    NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  GUID *)> _GetCryptoType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDecoderProfile(_Out_  GUID * pDecoderProfile);
    NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  GUID * &)> _GetDecoderProfile_pre_ptr;
    NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  GUID *)> _GetDecoderProfile_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCertificateSize(_Out_  UINT * pCertificateSize);
    NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  UINT * &)> _GetCertificateSize_pre_ptr;
    NullPtr<void (D3D11CryptoSessionHook::*)(HRESULT, _Out_  UINT *)> _GetCertificateSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCertificate(_In_  UINT CertificateSize, _Out_writes_bytes_(CertificateSize)  BYTE * pCertificate);
    NullPtr<void (D3D11CryptoSessionHook::*)(_In_  UINT &, _Out_writes_bytes_(CertificateSize)  BYTE * &)> _GetCertificate_pre_ptr;
    NullPtr<void (D3D11CryptoSessionHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_(CertificateSize)  BYTE *)> _GetCertificate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetCryptoSessionHandle(_Out_  HANDLE * pCryptoSessionHandle);
    NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  HANDLE * &)> _GetCryptoSessionHandle_pre_ptr;
    NullPtr<void (D3D11CryptoSessionHook::*)(_Out_  HANDLE *)> _GetCryptoSessionHandle_post_ptr;

};

// ==============================================================================
// ID3D11VideoDecoderOutputView
// ==============================================================================
class D3D11VideoDecoderOutputViewHook : public HookBase<ID3D11VideoDecoderOutputView>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ViewHook & _D3D11View;

protected:

    D3D11VideoDecoderOutputViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ViewHook & D3D11View,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11View(D3D11View)
    {
    }

    ~D3D11VideoDecoderOutputViewHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ViewHook * D3D11View = (D3D11ViewHook *)unknown.GetHookedObj(__uuidof(ID3D11View));
        if (nullptr == D3D11View) return nullptr;

        try
        {
            return new D3D11VideoDecoderOutputViewHook(unknown, *D3D11DeviceChild, *D3D11View, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VideoDecoderOutputViewHook * typedPtr = (D3D11VideoDecoderOutputViewHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetResource(_Out_  ID3D11Resource ** ppResource)
    {
        return _D3D11View.GetResource(ppResource);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc);
    NullPtr<void (D3D11VideoDecoderOutputViewHook::*)(_Out_  D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11VideoDecoderOutputViewHook::*)(_Out_  D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11VideoProcessorInputView
// ==============================================================================
class D3D11VideoProcessorInputViewHook : public HookBase<ID3D11VideoProcessorInputView>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ViewHook & _D3D11View;

protected:

    D3D11VideoProcessorInputViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ViewHook & D3D11View,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11View(D3D11View)
    {
    }

    ~D3D11VideoProcessorInputViewHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ViewHook * D3D11View = (D3D11ViewHook *)unknown.GetHookedObj(__uuidof(ID3D11View));
        if (nullptr == D3D11View) return nullptr;

        try
        {
            return new D3D11VideoProcessorInputViewHook(unknown, *D3D11DeviceChild, *D3D11View, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VideoProcessorInputViewHook * typedPtr = (D3D11VideoProcessorInputViewHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetResource(_Out_  ID3D11Resource ** ppResource)
    {
        return _D3D11View.GetResource(ppResource);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc);
    NullPtr<void (D3D11VideoProcessorInputViewHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11VideoProcessorInputViewHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11VideoProcessorOutputView
// ==============================================================================
class D3D11VideoProcessorOutputViewHook : public HookBase<ID3D11VideoProcessorOutputView>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11ViewHook & _D3D11View;

protected:

    D3D11VideoProcessorOutputViewHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11ViewHook & D3D11View,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11View(D3D11View)
    {
    }

    ~D3D11VideoProcessorOutputViewHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11ViewHook * D3D11View = (D3D11ViewHook *)unknown.GetHookedObj(__uuidof(ID3D11View));
        if (nullptr == D3D11View) return nullptr;

        try
        {
            return new D3D11VideoProcessorOutputViewHook(unknown, *D3D11DeviceChild, *D3D11View, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VideoProcessorOutputViewHook * typedPtr = (D3D11VideoProcessorOutputViewHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetResource(_Out_  ID3D11Resource ** ppResource)
    {
        return _D3D11View.GetResource(ppResource);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc);
    NullPtr<void (D3D11VideoProcessorOutputViewHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (D3D11VideoProcessorOutputViewHook::*)(_Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC *)> _GetDesc_post_ptr;

};

// ==============================================================================
// ID3D11VideoContext
// ==============================================================================
class D3D11VideoContextHook : public HookBase<ID3D11VideoContext>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3D11VideoContextHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3D11VideoContextHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3D11VideoContextHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VideoContextHook * typedPtr = (D3D11VideoContextHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDecoderBuffer(_In_  ID3D11VideoDecoder * pDecoder, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type, _Out_  UINT * pBufferSize, _Out_writes_bytes_opt_(*pBufferSize)  void ** ppBuffer);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE &, _Out_  UINT * &, _Out_writes_bytes_opt_(*pBufferSize)  void ** &)> _GetDecoderBuffer_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE, _Out_  UINT *, _Out_writes_bytes_opt_(*pBufferSize)  void **)> _GetDecoderBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReleaseDecoderBuffer(_In_  ID3D11VideoDecoder * pDecoder, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE &)> _ReleaseDecoderBuffer_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE)> _ReleaseDecoderBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DecoderBeginFrame(_In_  ID3D11VideoDecoder * pDecoder, _In_  ID3D11VideoDecoderOutputView * pView, _In_  UINT ContentKeySize, _In_reads_bytes_opt_(ContentKeySize)  const void * pContentKey);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  ID3D11VideoDecoderOutputView * &, _In_  UINT &, _In_reads_bytes_opt_(ContentKeySize)  const void * &)> _DecoderBeginFrame_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  ID3D11VideoDecoderOutputView *, _In_  UINT, _In_reads_bytes_opt_(ContentKeySize)  const void *)> _DecoderBeginFrame_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DecoderEndFrame(_In_  ID3D11VideoDecoder * pDecoder);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &)> _DecoderEndFrame_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *)> _DecoderEndFrame_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SubmitDecoderBuffers(_In_  ID3D11VideoDecoder * pDecoder, _In_  UINT NumBuffers, _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  UINT &, _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC * &)> _SubmitDecoderBuffers_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  UINT, _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC *)> _SubmitDecoderBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DecoderExtension(_In_  ID3D11VideoDecoder * pDecoder, _In_  const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoDecoder * &, _In_  const D3D11_VIDEO_DECODER_EXTENSION * &)> _DecoderExtension_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoDecoder *, _In_  const D3D11_VIDEO_DECODER_EXTENSION *)> _DecoderExtension_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputTargetRect(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  BOOL Enable, _In_opt_  const RECT * pRect);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  BOOL &, _In_opt_  const RECT * &)> _VideoProcessorSetOutputTargetRect_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  BOOL, _In_opt_  const RECT *)> _VideoProcessorSetOutputTargetRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputBackgroundColor(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  BOOL YCbCr, _In_  const D3D11_VIDEO_COLOR * pColor);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  BOOL &, _In_  const D3D11_VIDEO_COLOR * &)> _VideoProcessorSetOutputBackgroundColor_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  BOOL, _In_  const D3D11_VIDEO_COLOR *)> _VideoProcessorSetOutputBackgroundColor_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputColorSpace(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * &)> _VideoProcessorSetOutputColorSpace_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE *)> _VideoProcessorSetOutputColorSpace_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputAlphaFillMode(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode, _In_  UINT StreamIndex);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE &, _In_  UINT &)> _VideoProcessorSetOutputAlphaFillMode_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE, _In_  UINT)> _VideoProcessorSetOutputAlphaFillMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputConstriction(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  BOOL Enable, _In_  SIZE Size);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  BOOL &, _In_  SIZE &)> _VideoProcessorSetOutputConstriction_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  BOOL, _In_  SIZE)> _VideoProcessorSetOutputConstriction_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputStereoMode(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  BOOL Enable);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  BOOL &)> _VideoProcessorSetOutputStereoMode_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  BOOL)> _VideoProcessorSetOutputStereoMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE VideoProcessorSetOutputExtension(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  const GUID * pExtensionGuid, _In_  UINT DataSize, _In_  void * pData);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  const GUID * &, _In_  UINT &, _In_  void * &)> _VideoProcessorSetOutputExtension_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  const GUID *, _In_  UINT, _In_  void *)> _VideoProcessorSetOutputExtension_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputTargetRect(_In_  ID3D11VideoProcessor * pVideoProcessor, _Out_  BOOL * Enabled, _Out_  RECT * pRect);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  BOOL * &, _Out_  RECT * &)> _VideoProcessorGetOutputTargetRect_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  BOOL *, _Out_  RECT *)> _VideoProcessorGetOutputTargetRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputBackgroundColor(_In_  ID3D11VideoProcessor * pVideoProcessor, _Out_  BOOL * pYCbCr, _Out_  D3D11_VIDEO_COLOR * pColor);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  BOOL * &, _Out_  D3D11_VIDEO_COLOR * &)> _VideoProcessorGetOutputBackgroundColor_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  BOOL *, _Out_  D3D11_VIDEO_COLOR *)> _VideoProcessorGetOutputBackgroundColor_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputColorSpace(_In_  ID3D11VideoProcessor * pVideoProcessor, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * &)> _VideoProcessorGetOutputColorSpace_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE *)> _VideoProcessorGetOutputColorSpace_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputAlphaFillMode(_In_  ID3D11VideoProcessor * pVideoProcessor, _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode, _Out_  UINT * pStreamIndex);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * &, _Out_  UINT * &)> _VideoProcessorGetOutputAlphaFillMode_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE *, _Out_  UINT *)> _VideoProcessorGetOutputAlphaFillMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputConstriction(_In_  ID3D11VideoProcessor * pVideoProcessor, _Out_  BOOL * pEnabled, _Out_  SIZE * pSize);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  BOOL * &, _Out_  SIZE * &)> _VideoProcessorGetOutputConstriction_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  BOOL *, _Out_  SIZE *)> _VideoProcessorGetOutputConstriction_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputStereoMode(_In_  ID3D11VideoProcessor * pVideoProcessor, _Out_  BOOL * pEnabled);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _Out_  BOOL * &)> _VideoProcessorGetOutputStereoMode_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _Out_  BOOL *)> _VideoProcessorGetOutputStereoMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE VideoProcessorGetOutputExtension(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  const GUID * pExtensionGuid, _In_  UINT DataSize, _Out_  void * pData);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  const GUID * &, _In_  UINT &, _Out_  void * &)> _VideoProcessorGetOutputExtension_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  const GUID *, _In_  UINT, _Out_  void *)> _VideoProcessorGetOutputExtension_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamFrameFormat(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  D3D11_VIDEO_FRAME_FORMAT FrameFormat);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  D3D11_VIDEO_FRAME_FORMAT &)> _VideoProcessorSetStreamFrameFormat_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  D3D11_VIDEO_FRAME_FORMAT)> _VideoProcessorSetStreamFrameFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamColorSpace(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * &)> _VideoProcessorSetStreamColorSpace_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE *)> _VideoProcessorSetStreamColorSpace_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamOutputRate(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate, _In_  BOOL RepeatFrame, _In_opt_  const DXGI_RATIONAL * pCustomRate);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE &, _In_  BOOL &, _In_opt_  const DXGI_RATIONAL * &)> _VideoProcessorSetStreamOutputRate_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE, _In_  BOOL, _In_opt_  const DXGI_RATIONAL *)> _VideoProcessorSetStreamOutputRate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamSourceRect(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  BOOL Enable, _In_opt_  const RECT * pRect);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_opt_  const RECT * &)> _VideoProcessorSetStreamSourceRect_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_opt_  const RECT *)> _VideoProcessorSetStreamSourceRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamDestRect(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  BOOL Enable, _In_opt_  const RECT * pRect);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_opt_  const RECT * &)> _VideoProcessorSetStreamDestRect_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_opt_  const RECT *)> _VideoProcessorSetStreamDestRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamAlpha(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  BOOL Enable, _In_  FLOAT Alpha);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_  FLOAT &)> _VideoProcessorSetStreamAlpha_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_  FLOAT)> _VideoProcessorSetStreamAlpha_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamPalette(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  UINT Count, _In_reads_opt_(Count)  const UINT * pEntries);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  UINT &, _In_reads_opt_(Count)  const UINT * &)> _VideoProcessorSetStreamPalette_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  UINT, _In_reads_opt_(Count)  const UINT *)> _VideoProcessorSetStreamPalette_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamPixelAspectRatio(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  BOOL Enable, _In_opt_  const DXGI_RATIONAL * pSourceAspectRatio, _In_opt_  const DXGI_RATIONAL * pDestinationAspectRatio);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_opt_  const DXGI_RATIONAL * &, _In_opt_  const DXGI_RATIONAL * &)> _VideoProcessorSetStreamPixelAspectRatio_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_opt_  const DXGI_RATIONAL *, _In_opt_  const DXGI_RATIONAL *)> _VideoProcessorSetStreamPixelAspectRatio_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamLumaKey(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  BOOL Enable, _In_  FLOAT Lower, _In_  FLOAT Upper);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_  FLOAT &, _In_  FLOAT &)> _VideoProcessorSetStreamLumaKey_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_  FLOAT, _In_  FLOAT)> _VideoProcessorSetStreamLumaKey_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamStereoFormat(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  BOOL Enable, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format, _In_  BOOL LeftViewFrame0, _In_  BOOL BaseViewFrame0, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode, _In_  int MonoOffset);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT &, _In_  BOOL &, _In_  BOOL &, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE &, _In_  int &)> _VideoProcessorSetStreamStereoFormat_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT, _In_  BOOL, _In_  BOOL, _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE, _In_  int)> _VideoProcessorSetStreamStereoFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamAutoProcessingMode(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  BOOL Enable);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &)> _VideoProcessorSetStreamAutoProcessingMode_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL)> _VideoProcessorSetStreamAutoProcessingMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamFilter(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter, _In_  BOOL Enable, _In_  int Level);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  D3D11_VIDEO_PROCESSOR_FILTER &, _In_  BOOL &, _In_  int &)> _VideoProcessorSetStreamFilter_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  D3D11_VIDEO_PROCESSOR_FILTER, _In_  BOOL, _In_  int)> _VideoProcessorSetStreamFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE VideoProcessorSetStreamExtension(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  const GUID * pExtensionGuid, _In_  UINT DataSize, _In_  void * pData);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  const GUID * &, _In_  UINT &, _In_  void * &)> _VideoProcessorSetStreamExtension_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  UINT, _In_  const GUID *, _In_  UINT, _In_  void *)> _VideoProcessorSetStreamExtension_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamFrameFormat(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  D3D11_VIDEO_FRAME_FORMAT * pFrameFormat);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  D3D11_VIDEO_FRAME_FORMAT * &)> _VideoProcessorGetStreamFrameFormat_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  D3D11_VIDEO_FRAME_FORMAT *)> _VideoProcessorGetStreamFrameFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamColorSpace(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * &)> _VideoProcessorGetStreamColorSpace_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE *)> _VideoProcessorGetStreamColorSpace_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamOutputRate(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate, _Out_  BOOL * pRepeatFrame, _Out_  DXGI_RATIONAL * pCustomRate);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * &, _Out_  BOOL * &, _Out_  DXGI_RATIONAL * &)> _VideoProcessorGetStreamOutputRate_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE *, _Out_  BOOL *, _Out_  DXGI_RATIONAL *)> _VideoProcessorGetStreamOutputRate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamSourceRect(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  BOOL * pEnabled, _Out_  RECT * pRect);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  RECT * &)> _VideoProcessorGetStreamSourceRect_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  RECT *)> _VideoProcessorGetStreamSourceRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamDestRect(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  BOOL * pEnabled, _Out_  RECT * pRect);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  RECT * &)> _VideoProcessorGetStreamDestRect_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  RECT *)> _VideoProcessorGetStreamDestRect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamAlpha(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  BOOL * pEnabled, _Out_  FLOAT * pAlpha);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  FLOAT * &)> _VideoProcessorGetStreamAlpha_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  FLOAT *)> _VideoProcessorGetStreamAlpha_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamPalette(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  UINT Count, _Out_writes_(Count)  UINT * pEntries);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  UINT &, _Out_writes_(Count)  UINT * &)> _VideoProcessorGetStreamPalette_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  UINT, _Out_writes_(Count)  UINT *)> _VideoProcessorGetStreamPalette_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamPixelAspectRatio(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  BOOL * pEnabled, _Out_  DXGI_RATIONAL * pSourceAspectRatio, _Out_  DXGI_RATIONAL * pDestinationAspectRatio);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  DXGI_RATIONAL * &, _Out_  DXGI_RATIONAL * &)> _VideoProcessorGetStreamPixelAspectRatio_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  DXGI_RATIONAL *, _Out_  DXGI_RATIONAL *)> _VideoProcessorGetStreamPixelAspectRatio_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamLumaKey(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  BOOL * pEnabled, _Out_  FLOAT * pLower, _Out_  FLOAT * pUpper);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  FLOAT * &, _Out_  FLOAT * &)> _VideoProcessorGetStreamLumaKey_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  FLOAT *, _Out_  FLOAT *)> _VideoProcessorGetStreamLumaKey_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamStereoFormat(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  BOOL * pEnable, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat, _Out_  BOOL * pLeftViewFrame0, _Out_  BOOL * pBaseViewFrame0, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode, _Out_  int * MonoOffset);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * &, _Out_  BOOL * &, _Out_  BOOL * &, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * &, _Out_  int * &)> _VideoProcessorGetStreamStereoFormat_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT *, _Out_  BOOL *, _Out_  BOOL *, _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE *, _Out_  int *)> _VideoProcessorGetStreamStereoFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamAutoProcessingMode(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  BOOL * pEnabled);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &)> _VideoProcessorGetStreamAutoProcessingMode_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *)> _VideoProcessorGetStreamAutoProcessingMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamFilter(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter, _Out_  BOOL * pEnabled, _Out_  int * pLevel);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  D3D11_VIDEO_PROCESSOR_FILTER &, _Out_  BOOL * &, _Out_  int * &)> _VideoProcessorGetStreamFilter_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  D3D11_VIDEO_PROCESSOR_FILTER, _Out_  BOOL *, _Out_  int *)> _VideoProcessorGetStreamFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE VideoProcessorGetStreamExtension(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  const GUID * pExtensionGuid, _In_  UINT DataSize, _Out_  void * pData);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  const GUID * &, _In_  UINT &, _Out_  void * &)> _VideoProcessorGetStreamExtension_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  UINT, _In_  const GUID *, _In_  UINT, _Out_  void *)> _VideoProcessorGetStreamExtension_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE VideoProcessorBlt(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  ID3D11VideoProcessorOutputView * pView, _In_  UINT OutputFrame, _In_  UINT StreamCount, _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM * pStreams);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  ID3D11VideoProcessorOutputView * &, _In_  UINT &, _In_  UINT &, _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM * &)> _VideoProcessorBlt_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11VideoProcessor *, _In_  ID3D11VideoProcessorOutputView *, _In_  UINT, _In_  UINT, _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM *)> _VideoProcessorBlt_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE NegotiateCryptoSessionKeyExchange(_In_  ID3D11CryptoSession * pCryptoSession, _In_  UINT DataSize, _Inout_updates_bytes_(DataSize)  void * pData);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  UINT &, _Inout_updates_bytes_(DataSize)  void * &)> _NegotiateCryptoSessionKeyExchange_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11CryptoSession *, _In_  UINT, _Inout_updates_bytes_(DataSize)  void *)> _NegotiateCryptoSessionKeyExchange_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE EncryptionBlt(_In_  ID3D11CryptoSession * pCryptoSession, _In_  ID3D11Texture2D * pSrcSurface, _In_  ID3D11Texture2D * pDstSurface, _In_  UINT IVSize, _In_reads_bytes_opt_(IVSize)  void * pIV);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  ID3D11Texture2D * &, _In_  ID3D11Texture2D * &, _In_  UINT &, _In_reads_bytes_opt_(IVSize)  void * &)> _EncryptionBlt_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession *, _In_  ID3D11Texture2D *, _In_  ID3D11Texture2D *, _In_  UINT, _In_reads_bytes_opt_(IVSize)  void *)> _EncryptionBlt_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DecryptionBlt(_In_  ID3D11CryptoSession * pCryptoSession, _In_  ID3D11Texture2D * pSrcSurface, _In_  ID3D11Texture2D * pDstSurface, _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo, _In_  UINT ContentKeySize, _In_reads_bytes_opt_(ContentKeySize)  const void * pContentKey, _In_  UINT IVSize, _In_reads_bytes_opt_(IVSize)  void * pIV);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  ID3D11Texture2D * &, _In_  ID3D11Texture2D * &, _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO * &, _In_  UINT &, _In_reads_bytes_opt_(ContentKeySize)  const void * &, _In_  UINT &, _In_reads_bytes_opt_(IVSize)  void * &)> _DecryptionBlt_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession *, _In_  ID3D11Texture2D *, _In_  ID3D11Texture2D *, _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO *, _In_  UINT, _In_reads_bytes_opt_(ContentKeySize)  const void *, _In_  UINT, _In_reads_bytes_opt_(IVSize)  void *)> _DecryptionBlt_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE StartSessionKeyRefresh(_In_  ID3D11CryptoSession * pCryptoSession, _In_  UINT RandomNumberSize, _Out_writes_bytes_(RandomNumberSize)  void * pRandomNumber);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  UINT &, _Out_writes_bytes_(RandomNumberSize)  void * &)> _StartSessionKeyRefresh_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession *, _In_  UINT, _Out_writes_bytes_(RandomNumberSize)  void *)> _StartSessionKeyRefresh_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE FinishSessionKeyRefresh(_In_  ID3D11CryptoSession * pCryptoSession);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &)> _FinishSessionKeyRefresh_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession *)> _FinishSessionKeyRefresh_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetEncryptionBltKey(_In_  ID3D11CryptoSession * pCryptoSession, _In_  UINT KeySize, _Out_writes_bytes_(KeySize)  void * pReadbackKey);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11CryptoSession * &, _In_  UINT &, _Out_writes_bytes_(KeySize)  void * &)> _GetEncryptionBltKey_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11CryptoSession *, _In_  UINT, _Out_writes_bytes_(KeySize)  void *)> _GetEncryptionBltKey_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE NegotiateAuthenticatedChannelKeyExchange(_In_  ID3D11AuthenticatedChannel * pChannel, _In_  UINT DataSize, _Inout_updates_bytes_(DataSize)  void * pData);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11AuthenticatedChannel * &, _In_  UINT &, _Inout_updates_bytes_(DataSize)  void * &)> _NegotiateAuthenticatedChannelKeyExchange_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11AuthenticatedChannel *, _In_  UINT, _Inout_updates_bytes_(DataSize)  void *)> _NegotiateAuthenticatedChannelKeyExchange_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE QueryAuthenticatedChannel(_In_  ID3D11AuthenticatedChannel * pChannel, _In_  UINT InputSize, _In_reads_bytes_(InputSize)  const void * pInput, _In_  UINT OutputSize, _Out_writes_bytes_(OutputSize)  void * pOutput);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11AuthenticatedChannel * &, _In_  UINT &, _In_reads_bytes_(InputSize)  const void * &, _In_  UINT &, _Out_writes_bytes_(OutputSize)  void * &)> _QueryAuthenticatedChannel_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11AuthenticatedChannel *, _In_  UINT, _In_reads_bytes_(InputSize)  const void *, _In_  UINT, _Out_writes_bytes_(OutputSize)  void *)> _QueryAuthenticatedChannel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ConfigureAuthenticatedChannel(_In_  ID3D11AuthenticatedChannel * pChannel, _In_  UINT InputSize, _In_reads_bytes_(InputSize)  const void * pInput, _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11AuthenticatedChannel * &, _In_  UINT &, _In_reads_bytes_(InputSize)  const void * &, _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * &)> _ConfigureAuthenticatedChannel_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(HRESULT, _In_  ID3D11AuthenticatedChannel *, _In_  UINT, _In_reads_bytes_(InputSize)  const void *, _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT *)> _ConfigureAuthenticatedChannel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamRotation(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _In_  BOOL Enable, _In_  D3D11_VIDEO_PROCESSOR_ROTATION Rotation);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _In_  BOOL &, _In_  D3D11_VIDEO_PROCESSOR_ROTATION &)> _VideoProcessorSetStreamRotation_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _In_  BOOL, _In_  D3D11_VIDEO_PROCESSOR_ROTATION)> _VideoProcessorSetStreamRotation_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamRotation(_In_  ID3D11VideoProcessor * pVideoProcessor, _In_  UINT StreamIndex, _Out_  BOOL * pEnable, _Out_  D3D11_VIDEO_PROCESSOR_ROTATION * pRotation);
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor * &, _In_  UINT &, _Out_  BOOL * &, _Out_  D3D11_VIDEO_PROCESSOR_ROTATION * &)> _VideoProcessorGetStreamRotation_pre_ptr;
    NullPtr<void (D3D11VideoContextHook::*)(_In_  ID3D11VideoProcessor *, _In_  UINT, _Out_  BOOL *, _Out_  D3D11_VIDEO_PROCESSOR_ROTATION *)> _VideoProcessorGetStreamRotation_post_ptr;

};

// ==============================================================================
// ID3D11VideoDevice
// ==============================================================================
class D3D11VideoDeviceHook : public HookBase<ID3D11VideoDevice>
{

protected:

    D3D11VideoDeviceHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11VideoDeviceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11VideoDeviceHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11VideoDeviceHook * typedPtr = (D3D11VideoDeviceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVideoDecoder(_In_  const D3D11_VIDEO_DECODER_DESC * pVideoDesc, _In_  const D3D11_VIDEO_DECODER_CONFIG * pConfig, _Out_  ID3D11VideoDecoder ** ppDecoder);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const D3D11_VIDEO_DECODER_DESC * &, _In_  const D3D11_VIDEO_DECODER_CONFIG * &, _Out_  ID3D11VideoDecoder ** &)> _CreateVideoDecoder_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const D3D11_VIDEO_DECODER_DESC *, _In_  const D3D11_VIDEO_DECODER_CONFIG *, _Out_  ID3D11VideoDecoder **)> _CreateVideoDecoder_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessor(_In_  ID3D11VideoProcessorEnumerator * pEnum, _In_  UINT RateConversionIndex, _Out_  ID3D11VideoProcessor ** ppVideoProcessor);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  ID3D11VideoProcessorEnumerator * &, _In_  UINT &, _Out_  ID3D11VideoProcessor ** &)> _CreateVideoProcessor_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  ID3D11VideoProcessorEnumerator *, _In_  UINT, _Out_  ID3D11VideoProcessor **)> _CreateVideoProcessor_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateAuthenticatedChannel(_In_  D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType, _Out_  ID3D11AuthenticatedChannel ** ppAuthenticatedChannel);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  D3D11_AUTHENTICATED_CHANNEL_TYPE &, _Out_  ID3D11AuthenticatedChannel ** &)> _CreateAuthenticatedChannel_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  D3D11_AUTHENTICATED_CHANNEL_TYPE, _Out_  ID3D11AuthenticatedChannel **)> _CreateAuthenticatedChannel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCryptoSession(_In_  const GUID * pCryptoType, _In_opt_  const GUID * pDecoderProfile, _In_  const GUID * pKeyExchangeType, _Outptr_  ID3D11CryptoSession ** ppCryptoSession);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const GUID * &, _In_opt_  const GUID * &, _In_  const GUID * &, _Outptr_  ID3D11CryptoSession ** &)> _CreateCryptoSession_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const GUID *, _In_opt_  const GUID *, _In_  const GUID *, _Outptr_  ID3D11CryptoSession **)> _CreateCryptoSession_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVideoDecoderOutputView(_In_  ID3D11Resource * pResource, _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc, _Out_opt_  ID3D11VideoDecoderOutputView ** ppVDOVView);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  ID3D11Resource * &, _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * &, _Out_opt_  ID3D11VideoDecoderOutputView ** &)> _CreateVideoDecoderOutputView_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC *, _Out_opt_  ID3D11VideoDecoderOutputView **)> _CreateVideoDecoderOutputView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorInputView(_In_  ID3D11Resource * pResource, _In_  ID3D11VideoProcessorEnumerator * pEnum, _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc, _Out_opt_  ID3D11VideoProcessorInputView ** ppVPIView);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  ID3D11Resource * &, _In_  ID3D11VideoProcessorEnumerator * &, _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * &, _Out_opt_  ID3D11VideoProcessorInputView ** &)> _CreateVideoProcessorInputView_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_  ID3D11VideoProcessorEnumerator *, _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC *, _Out_opt_  ID3D11VideoProcessorInputView **)> _CreateVideoProcessorInputView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorOutputView(_In_  ID3D11Resource * pResource, _In_  ID3D11VideoProcessorEnumerator * pEnum, _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc, _Out_opt_  ID3D11VideoProcessorOutputView ** ppVPOView);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  ID3D11Resource * &, _In_  ID3D11VideoProcessorEnumerator * &, _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * &, _Out_opt_  ID3D11VideoProcessorOutputView ** &)> _CreateVideoProcessorOutputView_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_  ID3D11VideoProcessorEnumerator *, _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC *, _Out_opt_  ID3D11VideoProcessorOutputView **)> _CreateVideoProcessorOutputView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorEnumerator(_In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc, _Out_  ID3D11VideoProcessorEnumerator ** ppEnum);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * &, _Out_  ID3D11VideoProcessorEnumerator ** &)> _CreateVideoProcessorEnumerator_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC *, _Out_  ID3D11VideoProcessorEnumerator **)> _CreateVideoProcessorEnumerator_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetVideoDecoderProfileCount();
    NullPtr<void (D3D11VideoDeviceHook::*)()> _GetVideoDecoderProfileCount_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(UINT)> _GetVideoDecoderProfileCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderProfile(_In_  UINT Index, _Out_  GUID * pDecoderProfile);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  UINT &, _Out_  GUID * &)> _GetVideoDecoderProfile_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  UINT, _Out_  GUID *)> _GetVideoDecoderProfile_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckVideoDecoderFormat(_In_  const GUID * pDecoderProfile, _In_  DXGI_FORMAT Format, _Out_  BOOL * pSupported);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const GUID * &, _In_  DXGI_FORMAT &, _Out_  BOOL * &)> _CheckVideoDecoderFormat_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const GUID *, _In_  DXGI_FORMAT, _Out_  BOOL *)> _CheckVideoDecoderFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderConfigCount(_In_  const D3D11_VIDEO_DECODER_DESC * pDesc, _Out_  UINT * pCount);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const D3D11_VIDEO_DECODER_DESC * &, _Out_  UINT * &)> _GetVideoDecoderConfigCount_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const D3D11_VIDEO_DECODER_DESC *, _Out_  UINT *)> _GetVideoDecoderConfigCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderConfig(_In_  const D3D11_VIDEO_DECODER_DESC * pDesc, _In_  UINT Index, _Out_  D3D11_VIDEO_DECODER_CONFIG * pConfig);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const D3D11_VIDEO_DECODER_DESC * &, _In_  UINT &, _Out_  D3D11_VIDEO_DECODER_CONFIG * &)> _GetVideoDecoderConfig_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const D3D11_VIDEO_DECODER_DESC *, _In_  UINT, _Out_  D3D11_VIDEO_DECODER_CONFIG *)> _GetVideoDecoderConfig_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetContentProtectionCaps(_In_opt_  const GUID * pCryptoType, _In_opt_  const GUID * pDecoderProfile, _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_opt_  const GUID * &, _In_opt_  const GUID * &, _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS * &)> _GetContentProtectionCaps_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_opt_  const GUID *, _In_opt_  const GUID *, _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS *)> _GetContentProtectionCaps_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckCryptoKeyExchange(_In_  const GUID * pCryptoType, _In_opt_  const GUID * pDecoderProfile, _In_  UINT Index, _Out_  GUID * pKeyExchangeType);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  const GUID * &, _In_opt_  const GUID * &, _In_  UINT &, _Out_  GUID * &)> _CheckCryptoKeyExchange_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  const GUID *, _In_opt_  const GUID *, _In_  UINT, _Out_  GUID *)> _CheckCryptoKeyExchange_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_(DataSize)  const void * pData);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  REFGUID, _In_  UINT &, _In_reads_bytes_opt_(DataSize)  const void * &)> _SetPrivateData_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  REFGUID, _In_  UINT, _In_reads_bytes_opt_(DataSize)  const void *)> _SetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData);
    NullPtr<void (D3D11VideoDeviceHook::*)(_In_  REFGUID, _In_opt_  const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
    NullPtr<void (D3D11VideoDeviceHook::*)(HRESULT, _In_  REFGUID, _In_opt_  const IUnknown *)> _SetPrivateDataInterface_post_ptr;

};

// ==============================================================================
// ID3D11Device
// ==============================================================================
class D3D11DeviceHook : public HookBase<ID3D11Device>
{

protected:

    D3D11DeviceHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11DeviceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11DeviceHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceHook * typedPtr = (D3D11DeviceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBuffer(_In_  const D3D11_BUFFER_DESC * pDesc, _In_opt_  const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_  ID3D11Buffer ** ppBuffer);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_BUFFER_DESC * &, _In_opt_  const D3D11_SUBRESOURCE_DATA * &, _Out_opt_  ID3D11Buffer ** &)> _CreateBuffer_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_BUFFER_DESC *, _In_opt_  const D3D11_SUBRESOURCE_DATA *, _Out_opt_  ID3D11Buffer **)> _CreateBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTexture1D(_In_  const D3D11_TEXTURE1D_DESC * pDesc, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_  ID3D11Texture1D ** ppTexture1D);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_TEXTURE1D_DESC * &, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * &, _Out_opt_  ID3D11Texture1D ** &)> _CreateTexture1D_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_TEXTURE1D_DESC *, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *, _Out_opt_  ID3D11Texture1D **)> _CreateTexture1D_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(_In_  const D3D11_TEXTURE2D_DESC * pDesc, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_  ID3D11Texture2D ** ppTexture2D);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_TEXTURE2D_DESC * &, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * &, _Out_opt_  ID3D11Texture2D ** &)> _CreateTexture2D_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_TEXTURE2D_DESC *, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *, _Out_opt_  ID3D11Texture2D **)> _CreateTexture2D_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTexture3D(_In_  const D3D11_TEXTURE3D_DESC * pDesc, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_  ID3D11Texture3D ** ppTexture3D);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_TEXTURE3D_DESC * &, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA * &, _Out_opt_  ID3D11Texture3D ** &)> _CreateTexture3D_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_TEXTURE3D_DESC *, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA *, _Out_opt_  ID3D11Texture3D **)> _CreateTexture3D_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView(_In_  ID3D11Resource * pResource, _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc, _Out_opt_  ID3D11ShaderResourceView ** ppSRView);
    NullPtr<void (D3D11DeviceHook::*)(_In_  ID3D11Resource * &, _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC * &, _Out_opt_  ID3D11ShaderResourceView ** &)> _CreateShaderResourceView_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC *, _Out_opt_  ID3D11ShaderResourceView **)> _CreateShaderResourceView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(_In_  ID3D11Resource * pResource, _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc, _Out_opt_  ID3D11UnorderedAccessView ** ppUAView);
    NullPtr<void (D3D11DeviceHook::*)(_In_  ID3D11Resource * &, _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC * &, _Out_opt_  ID3D11UnorderedAccessView ** &)> _CreateUnorderedAccessView_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC *, _Out_opt_  ID3D11UnorderedAccessView **)> _CreateUnorderedAccessView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(_In_  ID3D11Resource * pResource, _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC * pDesc, _Out_opt_  ID3D11RenderTargetView ** ppRTView);
    NullPtr<void (D3D11DeviceHook::*)(_In_  ID3D11Resource * &, _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC * &, _Out_opt_  ID3D11RenderTargetView ** &)> _CreateRenderTargetView_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC *, _Out_opt_  ID3D11RenderTargetView **)> _CreateRenderTargetView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(_In_  ID3D11Resource * pResource, _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc, _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView);
    NullPtr<void (D3D11DeviceHook::*)(_In_  ID3D11Resource * &, _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC * &, _Out_opt_  ID3D11DepthStencilView ** &)> _CreateDepthStencilView_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC *, _Out_opt_  ID3D11DepthStencilView **)> _CreateDepthStencilView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateInputLayout(_In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs, _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT NumElements, _In_  const void * pShaderBytecodeWithInputSignature, _In_  SIZE_T BytecodeLength, _Out_opt_  ID3D11InputLayout ** ppInputLayout);
    NullPtr<void (D3D11DeviceHook::*)(_In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC * &, _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT &, _In_  const void * &, _In_  SIZE_T &, _Out_opt_  ID3D11InputLayout ** &)> _CreateInputLayout_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC *, _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT, _In_  const void *, _In_  SIZE_T, _Out_opt_  ID3D11InputLayout **)> _CreateInputLayout_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11VertexShader ** ppVertexShader);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11VertexShader ** &)> _CreateVertexShader_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11VertexShader **)> _CreateVertexShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11GeometryShader ** ppGeometryShader);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11GeometryShader ** &)> _CreateGeometryShader_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11GeometryShader **)> _CreateGeometryShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY * pSODeclaration, _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT NumEntries, _In_reads_opt_(NumStrides)  const UINT * pBufferStrides, _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumStrides, _In_  UINT RasterizedStream, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11GeometryShader ** ppGeometryShader);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY * &, _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT &, _In_reads_opt_(NumStrides)  const UINT * &, _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT &, _In_  UINT &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11GeometryShader ** &)> _CreateGeometryShaderWithStreamOutput_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY *, _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT, _In_reads_opt_(NumStrides)  const UINT *, _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT, _In_  UINT, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11GeometryShader **)> _CreateGeometryShaderWithStreamOutput_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11PixelShader ** ppPixelShader);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11PixelShader ** &)> _CreatePixelShader_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11PixelShader **)> _CreatePixelShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateHullShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11HullShader ** ppHullShader);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11HullShader ** &)> _CreateHullShader_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11HullShader **)> _CreateHullShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDomainShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11DomainShader ** ppDomainShader);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11DomainShader ** &)> _CreateDomainShader_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11DomainShader **)> _CreateDomainShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateComputeShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11ComputeShader ** ppComputeShader);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11ComputeShader ** &)> _CreateComputeShader_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11ComputeShader **)> _CreateComputeShader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateClassLinkage(_Out_  ID3D11ClassLinkage ** ppLinkage);
    NullPtr<void (D3D11DeviceHook::*)(_Out_  ID3D11ClassLinkage ** &)> _CreateClassLinkage_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _Out_  ID3D11ClassLinkage **)> _CreateClassLinkage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBlendState(_In_  const D3D11_BLEND_DESC * pBlendStateDesc, _Out_opt_  ID3D11BlendState ** ppBlendState);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_BLEND_DESC * &, _Out_opt_  ID3D11BlendState ** &)> _CreateBlendState_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_BLEND_DESC *, _Out_opt_  ID3D11BlendState **)> _CreateBlendState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState(_In_  const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc, _Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_DEPTH_STENCIL_DESC * &, _Out_opt_  ID3D11DepthStencilState ** &)> _CreateDepthStencilState_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_DEPTH_STENCIL_DESC *, _Out_opt_  ID3D11DepthStencilState **)> _CreateDepthStencilState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(_In_  const D3D11_RASTERIZER_DESC * pRasterizerDesc, _Out_opt_  ID3D11RasterizerState ** ppRasterizerState);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_RASTERIZER_DESC * &, _Out_opt_  ID3D11RasterizerState ** &)> _CreateRasterizerState_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_RASTERIZER_DESC *, _Out_opt_  ID3D11RasterizerState **)> _CreateRasterizerState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(_In_  const D3D11_SAMPLER_DESC * pSamplerDesc, _Out_opt_  ID3D11SamplerState ** ppSamplerState);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_SAMPLER_DESC * &, _Out_opt_  ID3D11SamplerState ** &)> _CreateSamplerState_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_SAMPLER_DESC *, _Out_opt_  ID3D11SamplerState **)> _CreateSamplerState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateQuery(_In_  const D3D11_QUERY_DESC * pQueryDesc, _Out_opt_  ID3D11Query ** ppQuery);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_QUERY_DESC * &, _Out_opt_  ID3D11Query ** &)> _CreateQuery_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_QUERY_DESC *, _Out_opt_  ID3D11Query **)> _CreateQuery_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreatePredicate(_In_  const D3D11_QUERY_DESC * pPredicateDesc, _Out_opt_  ID3D11Predicate ** ppPredicate);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_QUERY_DESC * &, _Out_opt_  ID3D11Predicate ** &)> _CreatePredicate_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_QUERY_DESC *, _Out_opt_  ID3D11Predicate **)> _CreatePredicate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCounter(_In_  const D3D11_COUNTER_DESC * pCounterDesc, _Out_opt_  ID3D11Counter ** ppCounter);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_COUNTER_DESC * &, _Out_opt_  ID3D11Counter ** &)> _CreateCounter_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_COUNTER_DESC *, _Out_opt_  ID3D11Counter **)> _CreateCounter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(UINT ContextFlags, _Out_opt_  ID3D11DeviceContext ** ppDeferredContext);
    NullPtr<void (D3D11DeviceHook::*)(UINT &, _Out_opt_  ID3D11DeviceContext ** &)> _CreateDeferredContext_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, UINT, _Out_opt_  ID3D11DeviceContext **)> _CreateDeferredContext_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(_In_  HANDLE hResource, _In_  REFIID ReturnedInterface, _Out_opt_  void ** ppResource);
    NullPtr<void (D3D11DeviceHook::*)(_In_  HANDLE &, _In_  REFIID, _Out_opt_  void ** &)> _OpenSharedResource_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  HANDLE, _In_  REFIID, _Out_opt_  void **)> _OpenSharedResource_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(_In_  DXGI_FORMAT Format, _Out_  UINT * pFormatSupport);
    NullPtr<void (D3D11DeviceHook::*)(_In_  DXGI_FORMAT &, _Out_  UINT * &)> _CheckFormatSupport_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  DXGI_FORMAT, _Out_  UINT *)> _CheckFormatSupport_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(_In_  DXGI_FORMAT Format, _In_  UINT SampleCount, _Out_  UINT * pNumQualityLevels);
    NullPtr<void (D3D11DeviceHook::*)(_In_  DXGI_FORMAT &, _In_  UINT &, _Out_  UINT * &)> _CheckMultisampleQualityLevels_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  DXGI_FORMAT, _In_  UINT, _Out_  UINT *)> _CheckMultisampleQualityLevels_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CheckCounterInfo(_Out_  D3D11_COUNTER_INFO * pCounterInfo);
    NullPtr<void (D3D11DeviceHook::*)(_Out_  D3D11_COUNTER_INFO * &)> _CheckCounterInfo_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(_Out_  D3D11_COUNTER_INFO *)> _CheckCounterInfo_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckCounter(_In_  const D3D11_COUNTER_DESC * pDesc, _Out_  D3D11_COUNTER_TYPE * pType, _Out_  UINT * pActiveCounters, _Out_writes_opt_(*pNameLength)  LPSTR szName, _Inout_opt_  UINT * pNameLength, _Out_writes_opt_(*pUnitsLength)  LPSTR szUnits, _Inout_opt_  UINT * pUnitsLength, _Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription, _Inout_opt_  UINT * pDescriptionLength);
    NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_COUNTER_DESC * &, _Out_  D3D11_COUNTER_TYPE * &, _Out_  UINT * &, _Out_writes_opt_(*pNameLength)  LPSTR &, _Inout_opt_  UINT * &, _Out_writes_opt_(*pUnitsLength)  LPSTR &, _Inout_opt_  UINT * &, _Out_writes_opt_(*pDescriptionLength)  LPSTR &, _Inout_opt_  UINT * &)> _CheckCounter_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_COUNTER_DESC *, _Out_  D3D11_COUNTER_TYPE *, _Out_  UINT *, _Out_writes_opt_(*pNameLength)  LPSTR, _Inout_opt_  UINT *, _Out_writes_opt_(*pUnitsLength)  LPSTR, _Inout_opt_  UINT *, _Out_writes_opt_(*pDescriptionLength)  LPSTR, _Inout_opt_  UINT *)> _CheckCounter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(D3D11_FEATURE Feature, _Out_writes_bytes_(FeatureSupportDataSize)  void * pFeatureSupportData, UINT FeatureSupportDataSize);
    NullPtr<void (D3D11DeviceHook::*)(D3D11_FEATURE &, _Out_writes_bytes_(FeatureSupportDataSize)  void * &, UINT &)> _CheckFeatureSupport_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, D3D11_FEATURE, _Out_writes_bytes_(FeatureSupportDataSize)  void *, UINT)> _CheckFeatureSupport_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_(*pDataSize)  void * pData);
    NullPtr<void (D3D11DeviceHook::*)(_In_  REFGUID, _Inout_  UINT * &, _Out_writes_bytes_opt_(*pDataSize)  void * &)> _GetPrivateData_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  REFGUID, _Inout_  UINT *, _Out_writes_bytes_opt_(*pDataSize)  void *)> _GetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_(DataSize)  const void * pData);
    NullPtr<void (D3D11DeviceHook::*)(_In_  REFGUID, _In_  UINT &, _In_reads_bytes_opt_(DataSize)  const void * &)> _SetPrivateData_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  REFGUID, _In_  UINT, _In_reads_bytes_opt_(DataSize)  const void *)> _SetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData);
    NullPtr<void (D3D11DeviceHook::*)(_In_  REFGUID, _In_opt_  const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  REFGUID, _In_opt_  const IUnknown *)> _SetPrivateDataInterface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel();
    NullPtr<void (D3D11DeviceHook::*)()> _GetFeatureLevel_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(D3D_FEATURE_LEVEL)> _GetFeatureLevel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetCreationFlags();
    NullPtr<void (D3D11DeviceHook::*)()> _GetCreationFlags_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(UINT)> _GetCreationFlags_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason();
    NullPtr<void (D3D11DeviceHook::*)()> _GetDeviceRemovedReason_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT)> _GetDeviceRemovedReason_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetImmediateContext(_Out_  ID3D11DeviceContext ** ppImmediateContext);
    NullPtr<void (D3D11DeviceHook::*)(_Out_  ID3D11DeviceContext ** &)> _GetImmediateContext_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(_Out_  ID3D11DeviceContext **)> _GetImmediateContext_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(UINT RaiseFlags);
    NullPtr<void (D3D11DeviceHook::*)(UINT &)> _SetExceptionMode_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(HRESULT, UINT)> _SetExceptionMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetExceptionMode();
    NullPtr<void (D3D11DeviceHook::*)()> _GetExceptionMode_pre_ptr;
    NullPtr<void (D3D11DeviceHook::*)(UINT)> _GetExceptionMode_post_ptr;

};

// ==============================================================================
// ID3D11BlendState1
// ==============================================================================
class D3D11BlendState1Hook : public HookBase<ID3D11BlendState1>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11BlendStateHook & _D3D11BlendState;

protected:

    D3D11BlendState1Hook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11BlendStateHook & D3D11BlendState,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11BlendState(D3D11BlendState)
    {
    }

    ~D3D11BlendState1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11BlendStateHook * D3D11BlendState = (D3D11BlendStateHook *)unknown.GetHookedObj(__uuidof(ID3D11BlendState));
        if (nullptr == D3D11BlendState) return nullptr;

        try
        {
            return new D3D11BlendState1Hook(unknown, *D3D11DeviceChild, *D3D11BlendState, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11BlendState1Hook * typedPtr = (D3D11BlendState1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_BLEND_DESC * pDesc)
    {
        return _D3D11BlendState.GetDesc(pDesc);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc1(_Out_  D3D11_BLEND_DESC1 * pDesc);
    NullPtr<void (D3D11BlendState1Hook::*)(_Out_  D3D11_BLEND_DESC1 * &)> _GetDesc1_pre_ptr;
    NullPtr<void (D3D11BlendState1Hook::*)(_Out_  D3D11_BLEND_DESC1 *)> _GetDesc1_post_ptr;

};

// ==============================================================================
// ID3D11RasterizerState1
// ==============================================================================
class D3D11RasterizerState1Hook : public HookBase<ID3D11RasterizerState1>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11RasterizerStateHook & _D3D11RasterizerState;

protected:

    D3D11RasterizerState1Hook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11RasterizerStateHook & D3D11RasterizerState,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11RasterizerState(D3D11RasterizerState)
    {
    }

    ~D3D11RasterizerState1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11RasterizerStateHook * D3D11RasterizerState = (D3D11RasterizerStateHook *)unknown.GetHookedObj(__uuidof(ID3D11RasterizerState));
        if (nullptr == D3D11RasterizerState) return nullptr;

        try
        {
            return new D3D11RasterizerState1Hook(unknown, *D3D11DeviceChild, *D3D11RasterizerState, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11RasterizerState1Hook * typedPtr = (D3D11RasterizerState1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDesc(_Out_  D3D11_RASTERIZER_DESC * pDesc)
    {
        return _D3D11RasterizerState.GetDesc(pDesc);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc1(_Out_  D3D11_RASTERIZER_DESC1 * pDesc);
    NullPtr<void (D3D11RasterizerState1Hook::*)(_Out_  D3D11_RASTERIZER_DESC1 * &)> _GetDesc1_pre_ptr;
    NullPtr<void (D3D11RasterizerState1Hook::*)(_Out_  D3D11_RASTERIZER_DESC1 *)> _GetDesc1_post_ptr;

};

// ==============================================================================
// ID3DDeviceContextState
// ==============================================================================
class D3DDeviceContextStateHook : public HookBase<ID3DDeviceContextState>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

    D3DDeviceContextStateHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
    {
    }

    ~D3DDeviceContextStateHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        try
        {
            return new D3DDeviceContextStateHook(unknown, *D3D11DeviceChild, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3DDeviceContextStateHook * typedPtr = (D3DDeviceContextStateHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

};

// ==============================================================================
// ID3D11DeviceContext1
// ==============================================================================
class D3D11DeviceContext1Hook : public HookBase<ID3D11DeviceContext1>
{
    D3D11DeviceChildHook & _D3D11DeviceChild;
    D3D11DeviceContextHook & _D3D11DeviceContext;

protected:

    D3D11DeviceContext1Hook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11DeviceContextHook & D3D11DeviceContext,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11DeviceChild(D3D11DeviceChild)
        , _D3D11DeviceContext(D3D11DeviceContext)
    {
    }

    ~D3D11DeviceContext1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
        if (nullptr == D3D11DeviceChild) return nullptr;

        D3D11DeviceContextHook * D3D11DeviceContext = (D3D11DeviceContextHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceContext));
        if (nullptr == D3D11DeviceContext) return nullptr;

        try
        {
            return new D3D11DeviceContext1Hook(unknown, *D3D11DeviceChild, *D3D11DeviceContext, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceContext1Hook * typedPtr = (D3D11DeviceContext1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDevice(_Out_  ID3D11Device ** ppDevice)
    {
        return _D3D11DeviceChild.GetDevice(ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_( *pDataSize )  void * pData)
    {
        return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_( DataSize )  const void * pData)
    {
        return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE VSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
    {
        return _D3D11DeviceContext.VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
    {
        return _D3D11DeviceContext.PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PSSetShader(_In_opt_  ID3D11PixelShader * pPixelShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances)
    {
        return _D3D11DeviceContext.PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
    {
        return _D3D11DeviceContext.PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE VSSetShader(_In_opt_  ID3D11VertexShader * pVertexShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances)
    {
        return _D3D11DeviceContext.VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawIndexed(_In_  UINT IndexCount, _In_  UINT StartIndexLocation, _In_  INT BaseVertexLocation)
    {
        return _D3D11DeviceContext.DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Draw(_In_  UINT VertexCount, _In_  UINT StartVertexLocation)
    {
        return _D3D11DeviceContext.Draw(VertexCount, StartVertexLocation);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Map(_In_  ID3D11Resource * pResource, _In_  UINT Subresource, _In_  D3D11_MAP MapType, _In_  UINT MapFlags, _Out_  D3D11_MAPPED_SUBRESOURCE * pMappedResource)
    {
        return _D3D11DeviceContext.Map(pResource, Subresource, MapType, MapFlags, pMappedResource);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Unmap(_In_  ID3D11Resource * pResource, _In_  UINT Subresource)
    {
        return _D3D11DeviceContext.Unmap(pResource, Subresource);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
    {
        return _D3D11DeviceContext.PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE IASetInputLayout(_In_opt_  ID3D11InputLayout * pInputLayout)
    {
        return _D3D11DeviceContext.IASetInputLayout(pInputLayout);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE IASetVertexBuffers(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppVertexBuffers, _In_reads_opt_(NumBuffers)  const UINT * pStrides, _In_reads_opt_(NumBuffers)  const UINT * pOffsets)
    {
        return _D3D11DeviceContext.IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE IASetIndexBuffer(_In_opt_  ID3D11Buffer * pIndexBuffer, _In_  DXGI_FORMAT Format, _In_  UINT Offset)
    {
        return _D3D11DeviceContext.IASetIndexBuffer(pIndexBuffer, Format, Offset);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawIndexedInstanced(_In_  UINT IndexCountPerInstance, _In_  UINT InstanceCount, _In_  UINT StartIndexLocation, _In_  INT BaseVertexLocation, _In_  UINT StartInstanceLocation)
    {
        return _D3D11DeviceContext.DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawInstanced(_In_  UINT VertexCountPerInstance, _In_  UINT InstanceCount, _In_  UINT StartVertexLocation, _In_  UINT StartInstanceLocation)
    {
        return _D3D11DeviceContext.DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
    {
        return _D3D11DeviceContext.GSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GSSetShader(_In_opt_  ID3D11GeometryShader * pShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances)
    {
        return _D3D11DeviceContext.GSSetShader(pShader, ppClassInstances, NumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE IASetPrimitiveTopology(_In_  D3D11_PRIMITIVE_TOPOLOGY Topology)
    {
        return _D3D11DeviceContext.IASetPrimitiveTopology(Topology);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE VSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
    {
        return _D3D11DeviceContext.VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE VSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
    {
        return _D3D11DeviceContext.VSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Begin(_In_  ID3D11Asynchronous * pAsync)
    {
        return _D3D11DeviceContext.Begin(pAsync);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE End(_In_  ID3D11Asynchronous * pAsync)
    {
        return _D3D11DeviceContext.End(pAsync);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetData(_In_  ID3D11Asynchronous * pAsync, _Out_writes_bytes_opt_( DataSize )  void * pData, _In_  UINT DataSize, _In_  UINT GetDataFlags)
    {
        return _D3D11DeviceContext.GetData(pAsync, pData, DataSize, GetDataFlags);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetPredication(_In_opt_  ID3D11Predicate * pPredicate, _In_  BOOL PredicateValue)
    {
        return _D3D11DeviceContext.SetPredication(pPredicate, PredicateValue);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
    {
        return _D3D11DeviceContext.GSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
    {
        return _D3D11DeviceContext.GSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE OMSetRenderTargets(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const * ppRenderTargetViews, _In_opt_  ID3D11DepthStencilView * pDepthStencilView)
    {
        return _D3D11DeviceContext.OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(_In_  UINT NumRTVs, _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const * ppRenderTargetViews, _In_opt_  ID3D11DepthStencilView * pDepthStencilView, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT UAVStartSlot, _In_  UINT NumUAVs, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews, _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts)
    {
        return _D3D11DeviceContext.OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE OMSetBlendState(_In_opt_  ID3D11BlendState * pBlendState, _In_opt_  const FLOAT BlendFactor [4], _In_  UINT SampleMask)
    {
        return _D3D11DeviceContext.OMSetBlendState(pBlendState, BlendFactor, SampleMask);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE OMSetDepthStencilState(_In_opt_  ID3D11DepthStencilState * pDepthStencilState, _In_  UINT StencilRef)
    {
        return _D3D11DeviceContext.OMSetDepthStencilState(pDepthStencilState, StencilRef);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SOSetTargets(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppSOTargets, _In_reads_opt_(NumBuffers)  const UINT * pOffsets)
    {
        return _D3D11DeviceContext.SOSetTargets(NumBuffers, ppSOTargets, pOffsets);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawAuto()
    {
        return _D3D11DeviceContext.DrawAuto();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(_In_  ID3D11Buffer * pBufferForArgs, _In_  UINT AlignedByteOffsetForArgs)
    {
        return _D3D11DeviceContext.DrawIndexedInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawInstancedIndirect(_In_  ID3D11Buffer * pBufferForArgs, _In_  UINT AlignedByteOffsetForArgs)
    {
        return _D3D11DeviceContext.DrawInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Dispatch(_In_  UINT ThreadGroupCountX, _In_  UINT ThreadGroupCountY, _In_  UINT ThreadGroupCountZ)
    {
        return _D3D11DeviceContext.Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DispatchIndirect(_In_  ID3D11Buffer * pBufferForArgs, _In_  UINT AlignedByteOffsetForArgs)
    {
        return _D3D11DeviceContext.DispatchIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RSSetState(_In_opt_  ID3D11RasterizerState * pRasterizerState)
    {
        return _D3D11DeviceContext.RSSetState(pRasterizerState);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RSSetViewports(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports, _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT * pViewports)
    {
        return _D3D11DeviceContext.RSSetViewports(NumViewports, pViewports);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RSSetScissorRects(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects, _In_reads_opt_(NumRects)  const D3D11_RECT * pRects)
    {
        return _D3D11DeviceContext.RSSetScissorRects(NumRects, pRects);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CopySubresourceRegion(_In_  ID3D11Resource * pDstResource, _In_  UINT DstSubresource, _In_  UINT DstX, _In_  UINT DstY, _In_  UINT DstZ, _In_  ID3D11Resource * pSrcResource, _In_  UINT SrcSubresource, _In_opt_  const D3D11_BOX * pSrcBox)
    {
        return _D3D11DeviceContext.CopySubresourceRegion(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CopyResource(_In_  ID3D11Resource * pDstResource, _In_  ID3D11Resource * pSrcResource)
    {
        return _D3D11DeviceContext.CopyResource(pDstResource, pSrcResource);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE UpdateSubresource(_In_  ID3D11Resource * pDstResource, _In_  UINT DstSubresource, _In_opt_  const D3D11_BOX * pDstBox, _In_  const void * pSrcData, _In_  UINT SrcRowPitch, _In_  UINT SrcDepthPitch)
    {
        return _D3D11DeviceContext.UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CopyStructureCount(_In_  ID3D11Buffer * pDstBuffer, _In_  UINT DstAlignedByteOffset, _In_  ID3D11UnorderedAccessView * pSrcView)
    {
        return _D3D11DeviceContext.CopyStructureCount(pDstBuffer, DstAlignedByteOffset, pSrcView);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE ClearRenderTargetView(_In_  ID3D11RenderTargetView * pRenderTargetView, _In_  const FLOAT ColorRGBA [4])
    {
        return _D3D11DeviceContext.ClearRenderTargetView(pRenderTargetView, ColorRGBA);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(_In_  ID3D11UnorderedAccessView * pUnorderedAccessView, _In_  const UINT Values [4])
    {
        return _D3D11DeviceContext.ClearUnorderedAccessViewUint(pUnorderedAccessView, Values);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(_In_  ID3D11UnorderedAccessView * pUnorderedAccessView, _In_  const FLOAT Values [4])
    {
        return _D3D11DeviceContext.ClearUnorderedAccessViewFloat(pUnorderedAccessView, Values);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE ClearDepthStencilView(_In_  ID3D11DepthStencilView * pDepthStencilView, _In_  UINT ClearFlags, _In_  FLOAT Depth, _In_  UINT8 Stencil)
    {
        return _D3D11DeviceContext.ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GenerateMips(_In_  ID3D11ShaderResourceView * pShaderResourceView)
    {
        return _D3D11DeviceContext.GenerateMips(pShaderResourceView);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetResourceMinLOD(_In_  ID3D11Resource * pResource, FLOAT MinLOD)
    {
        return _D3D11DeviceContext.SetResourceMinLOD(pResource, MinLOD);
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetResourceMinLOD(_In_  ID3D11Resource * pResource)
    {
        return _D3D11DeviceContext.GetResourceMinLOD(pResource);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE ResolveSubresource(_In_  ID3D11Resource * pDstResource, _In_  UINT DstSubresource, _In_  ID3D11Resource * pSrcResource, _In_  UINT SrcSubresource, _In_  DXGI_FORMAT Format)
    {
        return _D3D11DeviceContext.ResolveSubresource(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE ExecuteCommandList(_In_  ID3D11CommandList * pCommandList, BOOL RestoreContextState)
    {
        return _D3D11DeviceContext.ExecuteCommandList(pCommandList, RestoreContextState);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE HSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
    {
        return _D3D11DeviceContext.HSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE HSSetShader(_In_opt_  ID3D11HullShader * pHullShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances)
    {
        return _D3D11DeviceContext.HSSetShader(pHullShader, ppClassInstances, NumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE HSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
    {
        return _D3D11DeviceContext.HSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE HSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
    {
        return _D3D11DeviceContext.HSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
    {
        return _D3D11DeviceContext.DSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DSSetShader(_In_opt_  ID3D11DomainShader * pDomainShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances)
    {
        return _D3D11DeviceContext.DSSetShader(pDomainShader, ppClassInstances, NumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
    {
        return _D3D11DeviceContext.DSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
    {
        return _D3D11DeviceContext.DSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSSetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
    {
        return _D3D11DeviceContext.CSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSSetUnorderedAccessViews(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT NumUAVs, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews, _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts)
    {
        return _D3D11DeviceContext.CSSetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSSetShader(_In_opt_  ID3D11ComputeShader * pComputeShader, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances, UINT NumClassInstances)
    {
        return _D3D11DeviceContext.CSSetShader(pComputeShader, ppClassInstances, NumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSSetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
    {
        return _D3D11DeviceContext.CSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSSetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
    {
        return _D3D11DeviceContext.CSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE VSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
    {
        return _D3D11DeviceContext.VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
    {
        return _D3D11DeviceContext.PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PSGetShader(_Out_  ID3D11PixelShader ** ppPixelShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances)
    {
        return _D3D11DeviceContext.PSGetShader(ppPixelShader, ppClassInstances, pNumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
    {
        return _D3D11DeviceContext.PSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE VSGetShader(_Out_  ID3D11VertexShader ** ppVertexShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances)
    {
        return _D3D11DeviceContext.VSGetShader(ppVertexShader, ppClassInstances, pNumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
    {
        return _D3D11DeviceContext.PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE IAGetInputLayout(_Out_  ID3D11InputLayout ** ppInputLayout)
    {
        return _D3D11DeviceContext.IAGetInputLayout(ppInputLayout);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE IAGetVertexBuffers(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppVertexBuffers, _Out_writes_opt_(NumBuffers)  UINT * pStrides, _Out_writes_opt_(NumBuffers)  UINT * pOffsets)
    {
        return _D3D11DeviceContext.IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE IAGetIndexBuffer(_Out_opt_  ID3D11Buffer ** pIndexBuffer, _Out_opt_  DXGI_FORMAT * Format, _Out_opt_  UINT * Offset)
    {
        return _D3D11DeviceContext.IAGetIndexBuffer(pIndexBuffer, Format, Offset);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
    {
        return _D3D11DeviceContext.GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GSGetShader(_Out_  ID3D11GeometryShader ** ppGeometryShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances)
    {
        return _D3D11DeviceContext.GSGetShader(ppGeometryShader, ppClassInstances, pNumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE IAGetPrimitiveTopology(_Out_  D3D11_PRIMITIVE_TOPOLOGY * pTopology)
    {
        return _D3D11DeviceContext.IAGetPrimitiveTopology(pTopology);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE VSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
    {
        return _D3D11DeviceContext.VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE VSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
    {
        return _D3D11DeviceContext.VSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetPredication(_Out_opt_  ID3D11Predicate ** ppPredicate, _Out_opt_  BOOL * pPredicateValue)
    {
        return _D3D11DeviceContext.GetPredication(ppPredicate, pPredicateValue);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
    {
        return _D3D11DeviceContext.GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
    {
        return _D3D11DeviceContext.GSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE OMGetRenderTargets(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11RenderTargetView ** ppRenderTargetViews, _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView)
    {
        return _D3D11DeviceContext.OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumRTVs, _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView ** ppRenderTargetViews, _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT UAVStartSlot, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT NumUAVs, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews)
    {
        return _D3D11DeviceContext.OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE OMGetBlendState(_Out_opt_  ID3D11BlendState ** ppBlendState, _Out_opt_  FLOAT BlendFactor [4], _Out_opt_  UINT * pSampleMask)
    {
        return _D3D11DeviceContext.OMGetBlendState(ppBlendState, BlendFactor, pSampleMask);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE OMGetDepthStencilState(_Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState, _Out_opt_  UINT * pStencilRef)
    {
        return _D3D11DeviceContext.OMGetDepthStencilState(ppDepthStencilState, pStencilRef);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SOGetTargets(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppSOTargets)
    {
        return _D3D11DeviceContext.SOGetTargets(NumBuffers, ppSOTargets);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RSGetState(_Out_  ID3D11RasterizerState ** ppRasterizerState)
    {
        return _D3D11DeviceContext.RSGetState(ppRasterizerState);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RSGetViewports(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumViewports, _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT * pViewports)
    {
        return _D3D11DeviceContext.RSGetViewports(pNumViewports, pViewports);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RSGetScissorRects(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumRects, _Out_writes_opt_(*pNumRects)  D3D11_RECT * pRects)
    {
        return _D3D11DeviceContext.RSGetScissorRects(pNumRects, pRects);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE HSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
    {
        return _D3D11DeviceContext.HSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE HSGetShader(_Out_  ID3D11HullShader ** ppHullShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances)
    {
        return _D3D11DeviceContext.HSGetShader(ppHullShader, ppClassInstances, pNumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE HSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
    {
        return _D3D11DeviceContext.HSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE HSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
    {
        return _D3D11DeviceContext.HSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
    {
        return _D3D11DeviceContext.DSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DSGetShader(_Out_  ID3D11DomainShader ** ppDomainShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances)
    {
        return _D3D11DeviceContext.DSGetShader(ppDomainShader, ppClassInstances, pNumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
    {
        return _D3D11DeviceContext.DSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
    {
        return _D3D11DeviceContext.DSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSGetShaderResources(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
    {
        return _D3D11DeviceContext.CSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSGetUnorderedAccessViews(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT NumUAVs, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews)
    {
        return _D3D11DeviceContext.CSGetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSGetShader(_Out_  ID3D11ComputeShader ** ppComputeShader, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances, _Inout_opt_  UINT * pNumClassInstances)
    {
        return _D3D11DeviceContext.CSGetShader(ppComputeShader, ppClassInstances, pNumClassInstances);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSGetSamplers(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
    {
        return _D3D11DeviceContext.CSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CSGetConstantBuffers(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
    {
        return _D3D11DeviceContext.CSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE ClearState()
    {
        return _D3D11DeviceContext.ClearState();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Flush()
    {
        return _D3D11DeviceContext.Flush();
    }

    // -----------------------------------------------------------------------------
    D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE GetType()
    {
        return _D3D11DeviceContext.GetType();
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetContextFlags()
    {
        return _D3D11DeviceContext.GetContextFlags();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FinishCommandList(BOOL RestoreDeferredContextState, _Out_opt_  ID3D11CommandList ** ppCommandList)
    {
        return _D3D11DeviceContext.FinishCommandList(RestoreDeferredContextState, ppCommandList);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CopySubresourceRegion1(_In_  ID3D11Resource * pDstResource, _In_  UINT DstSubresource, _In_  UINT DstX, _In_  UINT DstY, _In_  UINT DstZ, _In_  ID3D11Resource * pSrcResource, _In_  UINT SrcSubresource, _In_opt_  const D3D11_BOX * pSrcBox, _In_  UINT CopyFlags);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  ID3D11Resource * &, _In_  UINT &, _In_opt_  const D3D11_BOX * &, _In_  UINT &)> _CopySubresourceRegion1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11Resource *, _In_  UINT, _In_  UINT, _In_  UINT, _In_  UINT, _In_  ID3D11Resource *, _In_  UINT, _In_opt_  const D3D11_BOX *, _In_  UINT)> _CopySubresourceRegion1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE UpdateSubresource1(_In_  ID3D11Resource * pDstResource, _In_  UINT DstSubresource, _In_opt_  const D3D11_BOX * pDstBox, _In_  const void * pSrcData, _In_  UINT SrcRowPitch, _In_  UINT SrcDepthPitch, _In_  UINT CopyFlags);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_opt_  const D3D11_BOX * &, _In_  const void * &, _In_  UINT &, _In_  UINT &, _In_  UINT &)> _UpdateSubresource1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11Resource *, _In_  UINT, _In_opt_  const D3D11_BOX *, _In_  const void *, _In_  UINT, _In_  UINT, _In_  UINT)> _UpdateSubresource1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DiscardResource(_In_  ID3D11Resource * pResource);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11Resource * &)> _DiscardResource_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11Resource *)> _DiscardResource_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DiscardView(_In_  ID3D11View * pResourceView);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11View * &)> _DiscardView_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11View *)> _DiscardView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSSetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers, _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant, _In_reads_opt_(NumBuffers)  const UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _VSSetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *, _In_reads_opt_(NumBuffers)  const UINT *)> _VSSetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSSetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers, _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant, _In_reads_opt_(NumBuffers)  const UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _HSSetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *, _In_reads_opt_(NumBuffers)  const UINT *)> _HSSetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSSetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers, _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant, _In_reads_opt_(NumBuffers)  const UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _DSSetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *, _In_reads_opt_(NumBuffers)  const UINT *)> _DSSetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSSetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers, _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant, _In_reads_opt_(NumBuffers)  const UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _GSSetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *, _In_reads_opt_(NumBuffers)  const UINT *)> _GSSetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSSetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers, _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant, _In_reads_opt_(NumBuffers)  const UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _PSSetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *, _In_reads_opt_(NumBuffers)  const UINT *)> _PSSetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSSetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers, _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant, _In_reads_opt_(NumBuffers)  const UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _CSSetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *, _In_reads_opt_(NumBuffers)  const UINT *)> _CSSetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE VSGetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant, _Out_writes_opt_(NumBuffers)  UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &, _Out_writes_opt_(NumBuffers)  UINT * &, _Out_writes_opt_(NumBuffers)  UINT * &)> _VSGetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **, _Out_writes_opt_(NumBuffers)  UINT *, _Out_writes_opt_(NumBuffers)  UINT *)> _VSGetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE HSGetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant, _Out_writes_opt_(NumBuffers)  UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &, _Out_writes_opt_(NumBuffers)  UINT * &, _Out_writes_opt_(NumBuffers)  UINT * &)> _HSGetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **, _Out_writes_opt_(NumBuffers)  UINT *, _Out_writes_opt_(NumBuffers)  UINT *)> _HSGetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DSGetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant, _Out_writes_opt_(NumBuffers)  UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &, _Out_writes_opt_(NumBuffers)  UINT * &, _Out_writes_opt_(NumBuffers)  UINT * &)> _DSGetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **, _Out_writes_opt_(NumBuffers)  UINT *, _Out_writes_opt_(NumBuffers)  UINT *)> _DSGetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GSGetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant, _Out_writes_opt_(NumBuffers)  UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &, _Out_writes_opt_(NumBuffers)  UINT * &, _Out_writes_opt_(NumBuffers)  UINT * &)> _GSGetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **, _Out_writes_opt_(NumBuffers)  UINT *, _Out_writes_opt_(NumBuffers)  UINT *)> _GSGetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PSGetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant, _Out_writes_opt_(NumBuffers)  UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &, _Out_writes_opt_(NumBuffers)  UINT * &, _Out_writes_opt_(NumBuffers)  UINT * &)> _PSGetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **, _Out_writes_opt_(NumBuffers)  UINT *, _Out_writes_opt_(NumBuffers)  UINT *)> _PSGetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE CSGetConstantBuffers1(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant, _Out_writes_opt_(NumBuffers)  UINT * pNumConstants);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &, _Out_writes_opt_(NumBuffers)  UINT * &, _Out_writes_opt_(NumBuffers)  UINT * &)> _CSGetConstantBuffers1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **, _Out_writes_opt_(NumBuffers)  UINT *, _Out_writes_opt_(NumBuffers)  UINT *)> _CSGetConstantBuffers1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SwapDeviceContextState(_In_  ID3DDeviceContextState * pState, _Out_opt_  ID3DDeviceContextState ** ppPreviousState);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3DDeviceContextState * &, _Out_opt_  ID3DDeviceContextState ** &)> _SwapDeviceContextState_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3DDeviceContextState *, _Out_opt_  ID3DDeviceContextState **)> _SwapDeviceContextState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearView(_In_  ID3D11View * pView, _In_  const FLOAT Color [4], _In_reads_opt_(NumRects)  const D3D11_RECT * pRect, UINT NumRects);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11View * &, _In_  const FLOAT *, _In_reads_opt_(NumRects)  const D3D11_RECT * &, UINT &)> _ClearView_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11View *, _In_  const FLOAT [4], _In_reads_opt_(NumRects)  const D3D11_RECT *, UINT)> _ClearView_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DiscardView1(_In_  ID3D11View * pResourceView, _In_reads_opt_(NumRects)  const D3D11_RECT * pRects, UINT NumRects);
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11View * &, _In_reads_opt_(NumRects)  const D3D11_RECT * &, UINT &)> _DiscardView1_pre_ptr;
    NullPtr<void (D3D11DeviceContext1Hook::*)(_In_  ID3D11View *, _In_reads_opt_(NumRects)  const D3D11_RECT *, UINT)> _DiscardView1_post_ptr;

};

// ==============================================================================
// ID3D11Device1
// ==============================================================================
class D3D11Device1Hook : public HookBase<ID3D11Device1>
{
    D3D11DeviceHook & _D3D11Device;

protected:

    D3D11Device1Hook(UnknownBase & unknown, D3D11DeviceHook & D3D11Device,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D3D11Device(D3D11Device)
    {
    }

    ~D3D11Device1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DeviceHook * D3D11Device = (D3D11DeviceHook *)unknown.GetHookedObj(__uuidof(ID3D11Device));
        if (nullptr == D3D11Device) return nullptr;

        try
        {
            return new D3D11Device1Hook(unknown, *D3D11Device, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11Device1Hook * typedPtr = (D3D11Device1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBuffer(_In_  const D3D11_BUFFER_DESC * pDesc, _In_opt_  const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_  ID3D11Buffer ** ppBuffer)
    {
        return _D3D11Device.CreateBuffer(pDesc, pInitialData, ppBuffer);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateTexture1D(_In_  const D3D11_TEXTURE1D_DESC * pDesc, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_  ID3D11Texture1D ** ppTexture1D)
    {
        return _D3D11Device.CreateTexture1D(pDesc, pInitialData, ppTexture1D);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateTexture2D(_In_  const D3D11_TEXTURE2D_DESC * pDesc, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_  ID3D11Texture2D ** ppTexture2D)
    {
        return _D3D11Device.CreateTexture2D(pDesc, pInitialData, ppTexture2D);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateTexture3D(_In_  const D3D11_TEXTURE3D_DESC * pDesc, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_  ID3D11Texture3D ** ppTexture3D)
    {
        return _D3D11Device.CreateTexture3D(pDesc, pInitialData, ppTexture3D);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateShaderResourceView(_In_  ID3D11Resource * pResource, _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc, _Out_opt_  ID3D11ShaderResourceView ** ppSRView)
    {
        return _D3D11Device.CreateShaderResourceView(pResource, pDesc, ppSRView);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(_In_  ID3D11Resource * pResource, _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc, _Out_opt_  ID3D11UnorderedAccessView ** ppUAView)
    {
        return _D3D11Device.CreateUnorderedAccessView(pResource, pDesc, ppUAView);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRenderTargetView(_In_  ID3D11Resource * pResource, _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC * pDesc, _Out_opt_  ID3D11RenderTargetView ** ppRTView)
    {
        return _D3D11Device.CreateRenderTargetView(pResource, pDesc, ppRTView);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDepthStencilView(_In_  ID3D11Resource * pResource, _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc, _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView)
    {
        return _D3D11Device.CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateInputLayout(_In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs, _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT NumElements, _In_  const void * pShaderBytecodeWithInputSignature, _In_  SIZE_T BytecodeLength, _Out_opt_  ID3D11InputLayout ** ppInputLayout)
    {
        return _D3D11Device.CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateVertexShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11VertexShader ** ppVertexShader)
    {
        return _D3D11Device.CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateGeometryShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11GeometryShader ** ppGeometryShader)
    {
        return _D3D11Device.CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY * pSODeclaration, _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT NumEntries, _In_reads_opt_(NumStrides)  const UINT * pBufferStrides, _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumStrides, _In_  UINT RasterizedStream, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11GeometryShader ** ppGeometryShader)
    {
        return _D3D11Device.CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreatePixelShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11PixelShader ** ppPixelShader)
    {
        return _D3D11Device.CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateHullShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11HullShader ** ppHullShader)
    {
        return _D3D11Device.CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDomainShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11DomainShader ** ppDomainShader)
    {
        return _D3D11Device.CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateComputeShader(_In_  const void * pShaderBytecode, _In_  SIZE_T BytecodeLength, _In_opt_  ID3D11ClassLinkage * pClassLinkage, _Out_opt_  ID3D11ComputeShader ** ppComputeShader)
    {
        return _D3D11Device.CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateClassLinkage(_Out_  ID3D11ClassLinkage ** ppLinkage)
    {
        return _D3D11Device.CreateClassLinkage(ppLinkage);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBlendState(_In_  const D3D11_BLEND_DESC * pBlendStateDesc, _Out_opt_  ID3D11BlendState ** ppBlendState)
    {
        return _D3D11Device.CreateBlendState(pBlendStateDesc, ppBlendState);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDepthStencilState(_In_  const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc, _Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState)
    {
        return _D3D11Device.CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRasterizerState(_In_  const D3D11_RASTERIZER_DESC * pRasterizerDesc, _Out_opt_  ID3D11RasterizerState ** ppRasterizerState)
    {
        return _D3D11Device.CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSamplerState(_In_  const D3D11_SAMPLER_DESC * pSamplerDesc, _Out_opt_  ID3D11SamplerState ** ppSamplerState)
    {
        return _D3D11Device.CreateSamplerState(pSamplerDesc, ppSamplerState);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateQuery(_In_  const D3D11_QUERY_DESC * pQueryDesc, _Out_opt_  ID3D11Query ** ppQuery)
    {
        return _D3D11Device.CreateQuery(pQueryDesc, ppQuery);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreatePredicate(_In_  const D3D11_QUERY_DESC * pPredicateDesc, _Out_opt_  ID3D11Predicate ** ppPredicate)
    {
        return _D3D11Device.CreatePredicate(pPredicateDesc, ppPredicate);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateCounter(_In_  const D3D11_COUNTER_DESC * pCounterDesc, _Out_opt_  ID3D11Counter ** ppCounter)
    {
        return _D3D11Device.CreateCounter(pCounterDesc, ppCounter);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDeferredContext(UINT ContextFlags, _Out_opt_  ID3D11DeviceContext ** ppDeferredContext)
    {
        return _D3D11Device.CreateDeferredContext(ContextFlags, ppDeferredContext);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE OpenSharedResource(_In_  HANDLE hResource, _In_  REFIID ReturnedInterface, _Out_opt_  void ** ppResource)
    {
        return _D3D11Device.OpenSharedResource(hResource, ReturnedInterface, ppResource);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckFormatSupport(_In_  DXGI_FORMAT Format, _Out_  UINT * pFormatSupport)
    {
        return _D3D11Device.CheckFormatSupport(Format, pFormatSupport);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(_In_  DXGI_FORMAT Format, _In_  UINT SampleCount, _Out_  UINT * pNumQualityLevels)
    {
        return _D3D11Device.CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE CheckCounterInfo(_Out_  D3D11_COUNTER_INFO * pCounterInfo)
    {
        return _D3D11Device.CheckCounterInfo(pCounterInfo);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckCounter(_In_  const D3D11_COUNTER_DESC * pDesc, _Out_  D3D11_COUNTER_TYPE * pType, _Out_  UINT * pActiveCounters, _Out_writes_opt_(*pNameLength)  LPSTR szName, _Inout_opt_  UINT * pNameLength, _Out_writes_opt_(*pUnitsLength)  LPSTR szUnits, _Inout_opt_  UINT * pUnitsLength, _Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription, _Inout_opt_  UINT * pDescriptionLength)
    {
        return _D3D11Device.CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckFeatureSupport(D3D11_FEATURE Feature, _Out_writes_bytes_(FeatureSupportDataSize)  void * pFeatureSupportData, UINT FeatureSupportDataSize)
    {
        return _D3D11Device.CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID guid, _Inout_  UINT * pDataSize, _Out_writes_bytes_opt_(*pDataSize)  void * pData)
    {
        return _D3D11Device.GetPrivateData(guid, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID guid, _In_  UINT DataSize, _In_reads_bytes_opt_(DataSize)  const void * pData)
    {
        return _D3D11Device.SetPrivateData(guid, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID guid, _In_opt_  const IUnknown * pData)
    {
        return _D3D11Device.SetPrivateDataInterface(guid, pData);
    }

    // -----------------------------------------------------------------------------
    D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel()
    {
        return _D3D11Device.GetFeatureLevel();
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetCreationFlags()
    {
        return _D3D11Device.GetCreationFlags();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason()
    {
        return _D3D11Device.GetDeviceRemovedReason();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetImmediateContext(_Out_  ID3D11DeviceContext ** ppImmediateContext)
    {
        return _D3D11Device.GetImmediateContext(ppImmediateContext);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetExceptionMode(UINT RaiseFlags)
    {
        return _D3D11Device.SetExceptionMode(RaiseFlags);
    }

    // -----------------------------------------------------------------------------
    UINT STDMETHODCALLTYPE GetExceptionMode()
    {
        return _D3D11Device.GetExceptionMode();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetImmediateContext1(_Out_  ID3D11DeviceContext1 ** ppImmediateContext);
    NullPtr<void (D3D11Device1Hook::*)(_Out_  ID3D11DeviceContext1 ** &)> _GetImmediateContext1_pre_ptr;
    NullPtr<void (D3D11Device1Hook::*)(_Out_  ID3D11DeviceContext1 **)> _GetImmediateContext1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext1(UINT ContextFlags, _Out_opt_  ID3D11DeviceContext1 ** ppDeferredContext);
    NullPtr<void (D3D11Device1Hook::*)(UINT &, _Out_opt_  ID3D11DeviceContext1 ** &)> _CreateDeferredContext1_pre_ptr;
    NullPtr<void (D3D11Device1Hook::*)(HRESULT, UINT, _Out_opt_  ID3D11DeviceContext1 **)> _CreateDeferredContext1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBlendState1(_In_  const D3D11_BLEND_DESC1 * pBlendStateDesc, _Out_opt_  ID3D11BlendState1 ** ppBlendState);
    NullPtr<void (D3D11Device1Hook::*)(_In_  const D3D11_BLEND_DESC1 * &, _Out_opt_  ID3D11BlendState1 ** &)> _CreateBlendState1_pre_ptr;
    NullPtr<void (D3D11Device1Hook::*)(HRESULT, _In_  const D3D11_BLEND_DESC1 *, _Out_opt_  ID3D11BlendState1 **)> _CreateBlendState1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState1(_In_  const D3D11_RASTERIZER_DESC1 * pRasterizerDesc, _Out_opt_  ID3D11RasterizerState1 ** ppRasterizerState);
    NullPtr<void (D3D11Device1Hook::*)(_In_  const D3D11_RASTERIZER_DESC1 * &, _Out_opt_  ID3D11RasterizerState1 ** &)> _CreateRasterizerState1_pre_ptr;
    NullPtr<void (D3D11Device1Hook::*)(HRESULT, _In_  const D3D11_RASTERIZER_DESC1 *, _Out_opt_  ID3D11RasterizerState1 **)> _CreateRasterizerState1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDeviceContextState(UINT Flags, _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL * pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, REFIID EmulatedInterface, _Out_opt_  D3D_FEATURE_LEVEL * pChosenFeatureLevel, _Out_opt_  ID3DDeviceContextState ** ppContextState);
    NullPtr<void (D3D11Device1Hook::*)(UINT &, _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL * &, UINT &, UINT &, REFIID, _Out_opt_  D3D_FEATURE_LEVEL * &, _Out_opt_  ID3DDeviceContextState ** &)> _CreateDeviceContextState_pre_ptr;
    NullPtr<void (D3D11Device1Hook::*)(HRESULT, UINT, _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL *, UINT, UINT, REFIID, _Out_opt_  D3D_FEATURE_LEVEL *, _Out_opt_  ID3DDeviceContextState **)> _CreateDeviceContextState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE OpenSharedResource1(_In_  HANDLE hResource, _In_  REFIID returnedInterface, _Out_  void ** ppResource);
    NullPtr<void (D3D11Device1Hook::*)(_In_  HANDLE &, _In_  REFIID, _Out_  void ** &)> _OpenSharedResource1_pre_ptr;
    NullPtr<void (D3D11Device1Hook::*)(HRESULT, _In_  HANDLE, _In_  REFIID, _Out_  void **)> _OpenSharedResource1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE OpenSharedResourceByName(_In_  LPCWSTR lpName, _In_  DWORD dwDesiredAccess, _In_  REFIID returnedInterface, _Out_  void ** ppResource);
    NullPtr<void (D3D11Device1Hook::*)(_In_  LPCWSTR &, _In_  DWORD &, _In_  REFIID, _Out_  void ** &)> _OpenSharedResourceByName_pre_ptr;
    NullPtr<void (D3D11Device1Hook::*)(HRESULT, _In_  LPCWSTR, _In_  DWORD, _In_  REFIID, _Out_  void **)> _OpenSharedResourceByName_post_ptr;

};

// ==============================================================================
// ID3DUserDefinedAnnotation
// ==============================================================================
class D3DUserDefinedAnnotationHook : public HookBase<ID3DUserDefinedAnnotation>
{

protected:

    D3DUserDefinedAnnotationHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3DUserDefinedAnnotationHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3DUserDefinedAnnotationHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3DUserDefinedAnnotationHook * typedPtr = (D3DUserDefinedAnnotationHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual INT STDMETHODCALLTYPE BeginEvent(_In_  LPCWSTR Name);
    NullPtr<void (D3DUserDefinedAnnotationHook::*)(_In_  LPCWSTR &)> _BeginEvent_pre_ptr;
    NullPtr<void (D3DUserDefinedAnnotationHook::*)(INT, _In_  LPCWSTR)> _BeginEvent_post_ptr;

    // -----------------------------------------------------------------------------
    virtual INT STDMETHODCALLTYPE EndEvent();
    NullPtr<void (D3DUserDefinedAnnotationHook::*)()> _EndEvent_pre_ptr;
    NullPtr<void (D3DUserDefinedAnnotationHook::*)(INT)> _EndEvent_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetMarker(_In_  LPCWSTR Name);
    NullPtr<void (D3DUserDefinedAnnotationHook::*)(_In_  LPCWSTR &)> _SetMarker_pre_ptr;
    NullPtr<void (D3DUserDefinedAnnotationHook::*)(_In_  LPCWSTR)> _SetMarker_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE GetStatus();
    NullPtr<void (D3DUserDefinedAnnotationHook::*)()> _GetStatus_pre_ptr;
    NullPtr<void (D3DUserDefinedAnnotationHook::*)(BOOL)> _GetStatus_post_ptr;

};

// ==============================================================================
// ID3D11Debug
// ==============================================================================
class D3D11DebugHook : public HookBase<ID3D11Debug>
{

protected:

    D3D11DebugHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11DebugHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11DebugHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11DebugHook * typedPtr = (D3D11DebugHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFeatureMask(UINT Mask);
    NullPtr<void (D3D11DebugHook::*)(UINT &)> _SetFeatureMask_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(HRESULT, UINT)> _SetFeatureMask_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetFeatureMask();
    NullPtr<void (D3D11DebugHook::*)()> _GetFeatureMask_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(UINT)> _GetFeatureMask_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPresentPerRenderOpDelay(UINT Milliseconds);
    NullPtr<void (D3D11DebugHook::*)(UINT &)> _SetPresentPerRenderOpDelay_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(HRESULT, UINT)> _SetPresentPerRenderOpDelay_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetPresentPerRenderOpDelay();
    NullPtr<void (D3D11DebugHook::*)()> _GetPresentPerRenderOpDelay_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(UINT)> _GetPresentPerRenderOpDelay_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetSwapChain(_In_opt_  IDXGISwapChain * pSwapChain);
    NullPtr<void (D3D11DebugHook::*)(_In_opt_  IDXGISwapChain * &)> _SetSwapChain_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(HRESULT, _In_opt_  IDXGISwapChain *)> _SetSwapChain_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSwapChain(_Out_  IDXGISwapChain ** ppSwapChain);
    NullPtr<void (D3D11DebugHook::*)(_Out_  IDXGISwapChain ** &)> _GetSwapChain_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(HRESULT, _Out_  IDXGISwapChain **)> _GetSwapChain_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ValidateContext(_In_  ID3D11DeviceContext * pContext);
    NullPtr<void (D3D11DebugHook::*)(_In_  ID3D11DeviceContext * &)> _ValidateContext_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(HRESULT, _In_  ID3D11DeviceContext *)> _ValidateContext_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReportLiveDeviceObjects(D3D11_RLDO_FLAGS Flags);
    NullPtr<void (D3D11DebugHook::*)(D3D11_RLDO_FLAGS &)> _ReportLiveDeviceObjects_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(HRESULT, D3D11_RLDO_FLAGS)> _ReportLiveDeviceObjects_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ValidateContextForDispatch(_In_  ID3D11DeviceContext * pContext);
    NullPtr<void (D3D11DebugHook::*)(_In_  ID3D11DeviceContext * &)> _ValidateContextForDispatch_pre_ptr;
    NullPtr<void (D3D11DebugHook::*)(HRESULT, _In_  ID3D11DeviceContext *)> _ValidateContextForDispatch_post_ptr;

};

// ==============================================================================
// ID3D11SwitchToRef
// ==============================================================================
class D3D11SwitchToRefHook : public HookBase<ID3D11SwitchToRef>
{

protected:

    D3D11SwitchToRefHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11SwitchToRefHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11SwitchToRefHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11SwitchToRefHook * typedPtr = (D3D11SwitchToRefHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE SetUseRef(BOOL UseRef);
    NullPtr<void (D3D11SwitchToRefHook::*)(BOOL &)> _SetUseRef_pre_ptr;
    NullPtr<void (D3D11SwitchToRefHook::*)(BOOL, BOOL)> _SetUseRef_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE GetUseRef();
    NullPtr<void (D3D11SwitchToRefHook::*)()> _GetUseRef_pre_ptr;
    NullPtr<void (D3D11SwitchToRefHook::*)(BOOL)> _GetUseRef_post_ptr;

};

// ==============================================================================
// ID3D11TracingDevice
// ==============================================================================
class D3D11TracingDeviceHook : public HookBase<ID3D11TracingDevice>
{

protected:

    D3D11TracingDeviceHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11TracingDeviceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11TracingDeviceHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11TracingDeviceHook * typedPtr = (D3D11TracingDeviceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetShaderTrackingOptionsByType(_In_  UINT ResourceTypeFlags, _In_  UINT Options);
    NullPtr<void (D3D11TracingDeviceHook::*)(_In_  UINT &, _In_  UINT &)> _SetShaderTrackingOptionsByType_pre_ptr;
    NullPtr<void (D3D11TracingDeviceHook::*)(HRESULT, _In_  UINT, _In_  UINT)> _SetShaderTrackingOptionsByType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetShaderTrackingOptions(_In_  IUnknown * pShader, _In_  UINT Options);
    NullPtr<void (D3D11TracingDeviceHook::*)(_In_  IUnknown * &, _In_  UINT &)> _SetShaderTrackingOptions_pre_ptr;
    NullPtr<void (D3D11TracingDeviceHook::*)(HRESULT, _In_  IUnknown *, _In_  UINT)> _SetShaderTrackingOptions_post_ptr;

};

// ==============================================================================
// ID3D11RefTrackingOptions
// ==============================================================================
class D3D11RefTrackingOptionsHook : public HookBase<ID3D11RefTrackingOptions>
{

protected:

    D3D11RefTrackingOptionsHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11RefTrackingOptionsHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11RefTrackingOptionsHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11RefTrackingOptionsHook * typedPtr = (D3D11RefTrackingOptionsHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTrackingOptions(UINT uOptions);
    NullPtr<void (D3D11RefTrackingOptionsHook::*)(UINT &)> _SetTrackingOptions_pre_ptr;
    NullPtr<void (D3D11RefTrackingOptionsHook::*)(HRESULT, UINT)> _SetTrackingOptions_post_ptr;

};

// ==============================================================================
// ID3D11RefDefaultTrackingOptions
// ==============================================================================
class D3D11RefDefaultTrackingOptionsHook : public HookBase<ID3D11RefDefaultTrackingOptions>
{

protected:

    D3D11RefDefaultTrackingOptionsHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11RefDefaultTrackingOptionsHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11RefDefaultTrackingOptionsHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11RefDefaultTrackingOptionsHook * typedPtr = (D3D11RefDefaultTrackingOptionsHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTrackingOptions(UINT ResourceTypeFlags, UINT Options);
    NullPtr<void (D3D11RefDefaultTrackingOptionsHook::*)(UINT &, UINT &)> _SetTrackingOptions_pre_ptr;
    NullPtr<void (D3D11RefDefaultTrackingOptionsHook::*)(HRESULT, UINT, UINT)> _SetTrackingOptions_post_ptr;

};

// ==============================================================================
// ID3D11InfoQueue
// ==============================================================================
class D3D11InfoQueueHook : public HookBase<ID3D11InfoQueue>
{

protected:

    D3D11InfoQueueHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D3D11InfoQueueHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D3D11InfoQueueHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D3D11InfoQueueHook * typedPtr = (D3D11InfoQueueHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetMessageCountLimit(_In_  UINT64 MessageCountLimit);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  UINT64 &)> _SetMessageCountLimit_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  UINT64)> _SetMessageCountLimit_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearStoredMessages();
    NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStoredMessages_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStoredMessages_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetMessage(_In_  UINT64 MessageIndex, _Out_writes_bytes_opt_(*pMessageByteLength)  D3D11_MESSAGE * pMessage, _Inout_  SIZE_T * pMessageByteLength);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  UINT64 &, _Out_writes_bytes_opt_(*pMessageByteLength)  D3D11_MESSAGE * &, _Inout_  SIZE_T * &)> _GetMessage_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  UINT64, _Out_writes_bytes_opt_(*pMessageByteLength)  D3D11_MESSAGE *, _Inout_  SIZE_T *)> _GetMessage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT64 STDMETHODCALLTYPE GetNumMessagesAllowedByStorageFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumMessagesAllowedByStorageFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumMessagesAllowedByStorageFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT64 STDMETHODCALLTYPE GetNumMessagesDeniedByStorageFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumMessagesDeniedByStorageFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumMessagesDeniedByStorageFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT64 STDMETHODCALLTYPE GetNumStoredMessages();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumStoredMessages_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumStoredMessages_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT64 STDMETHODCALLTYPE GetNumStoredMessagesAllowedByRetrievalFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumStoredMessagesAllowedByRetrievalFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumStoredMessagesAllowedByRetrievalFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT64 STDMETHODCALLTYPE GetNumMessagesDiscardedByMessageCountLimit();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetNumMessagesDiscardedByMessageCountLimit_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetNumMessagesDiscardedByMessageCountLimit_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT64 STDMETHODCALLTYPE GetMessageCountLimit();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetMessageCountLimit_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(UINT64)> _GetMessageCountLimit_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddStorageFilterEntries(_In_  D3D11_INFO_QUEUE_FILTER * pFilter);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_INFO_QUEUE_FILTER * &)> _AddStorageFilterEntries_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_INFO_QUEUE_FILTER *)> _AddStorageFilterEntries_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetStorageFilter(_Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * pFilter, _Inout_  SIZE_T * pFilterByteLength);
    NullPtr<void (D3D11InfoQueueHook::*)(_Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * &, _Inout_  SIZE_T * &)> _GetStorageFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER *, _Inout_  SIZE_T *)> _GetStorageFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearStorageFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStorageFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)()> _ClearStorageFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PushEmptyStorageFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _PushEmptyStorageFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT)> _PushEmptyStorageFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PushCopyOfStorageFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _PushCopyOfStorageFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT)> _PushCopyOfStorageFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PushStorageFilter(_In_  D3D11_INFO_QUEUE_FILTER * pFilter);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_INFO_QUEUE_FILTER * &)> _PushStorageFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_INFO_QUEUE_FILTER *)> _PushStorageFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PopStorageFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _PopStorageFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)()> _PopStorageFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetStorageFilterStackSize();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetStorageFilterStackSize_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(UINT)> _GetStorageFilterStackSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddRetrievalFilterEntries(_In_  D3D11_INFO_QUEUE_FILTER * pFilter);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_INFO_QUEUE_FILTER * &)> _AddRetrievalFilterEntries_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_INFO_QUEUE_FILTER *)> _AddRetrievalFilterEntries_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRetrievalFilter(_Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * pFilter, _Inout_  SIZE_T * pFilterByteLength);
    NullPtr<void (D3D11InfoQueueHook::*)(_Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * &, _Inout_  SIZE_T * &)> _GetRetrievalFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER *, _Inout_  SIZE_T *)> _GetRetrievalFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearRetrievalFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _ClearRetrievalFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)()> _ClearRetrievalFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PushEmptyRetrievalFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _PushEmptyRetrievalFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT)> _PushEmptyRetrievalFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PushCopyOfRetrievalFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _PushCopyOfRetrievalFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT)> _PushCopyOfRetrievalFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PushRetrievalFilter(_In_  D3D11_INFO_QUEUE_FILTER * pFilter);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_INFO_QUEUE_FILTER * &)> _PushRetrievalFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_INFO_QUEUE_FILTER *)> _PushRetrievalFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PopRetrievalFilter();
    NullPtr<void (D3D11InfoQueueHook::*)()> _PopRetrievalFilter_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)()> _PopRetrievalFilter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT STDMETHODCALLTYPE GetRetrievalFilterStackSize();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetRetrievalFilterStackSize_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(UINT)> _GetRetrievalFilterStackSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddMessage(_In_  D3D11_MESSAGE_CATEGORY Category, _In_  D3D11_MESSAGE_SEVERITY Severity, _In_  D3D11_MESSAGE_ID ID, _In_  LPCSTR pDescription);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_CATEGORY &, _In_  D3D11_MESSAGE_SEVERITY &, _In_  D3D11_MESSAGE_ID &, _In_  LPCSTR &)> _AddMessage_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_CATEGORY, _In_  D3D11_MESSAGE_SEVERITY, _In_  D3D11_MESSAGE_ID, _In_  LPCSTR)> _AddMessage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddApplicationMessage(_In_  D3D11_MESSAGE_SEVERITY Severity, _In_  LPCSTR pDescription);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_SEVERITY &, _In_  LPCSTR &)> _AddApplicationMessage_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_SEVERITY, _In_  LPCSTR)> _AddApplicationMessage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetBreakOnCategory(_In_  D3D11_MESSAGE_CATEGORY Category, _In_  BOOL bEnable);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_CATEGORY &, _In_  BOOL &)> _SetBreakOnCategory_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_CATEGORY, _In_  BOOL)> _SetBreakOnCategory_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetBreakOnSeverity(_In_  D3D11_MESSAGE_SEVERITY Severity, _In_  BOOL bEnable);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_SEVERITY &, _In_  BOOL &)> _SetBreakOnSeverity_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_SEVERITY, _In_  BOOL)> _SetBreakOnSeverity_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetBreakOnID(_In_  D3D11_MESSAGE_ID ID, _In_  BOOL bEnable);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_ID &, _In_  BOOL &)> _SetBreakOnID_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(HRESULT, _In_  D3D11_MESSAGE_ID, _In_  BOOL)> _SetBreakOnID_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE GetBreakOnCategory(_In_  D3D11_MESSAGE_CATEGORY Category);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_CATEGORY &)> _GetBreakOnCategory_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(BOOL, _In_  D3D11_MESSAGE_CATEGORY)> _GetBreakOnCategory_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE GetBreakOnSeverity(_In_  D3D11_MESSAGE_SEVERITY Severity);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_SEVERITY &)> _GetBreakOnSeverity_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(BOOL, _In_  D3D11_MESSAGE_SEVERITY)> _GetBreakOnSeverity_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE GetBreakOnID(_In_  D3D11_MESSAGE_ID ID);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  D3D11_MESSAGE_ID &)> _GetBreakOnID_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(BOOL, _In_  D3D11_MESSAGE_ID)> _GetBreakOnID_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetMuteDebugOutput(_In_  BOOL bMute);
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  BOOL &)> _SetMuteDebugOutput_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(_In_  BOOL)> _SetMuteDebugOutput_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE GetMuteDebugOutput();
    NullPtr<void (D3D11InfoQueueHook::*)()> _GetMuteDebugOutput_pre_ptr;
    NullPtr<void (D3D11InfoQueueHook::*)(BOOL)> _GetMuteDebugOutput_post_ptr;

};

// ==============================================================================
// IDXGIObject
// ==============================================================================
class DXGIObjectHook : public HookBase<IDXGIObject>
{

protected:

    DXGIObjectHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DXGIObjectHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DXGIObjectHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * typedPtr = (DXGIObjectHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData);
    NullPtr<void (DXGIObjectHook::*)(_In_  REFGUID, /* [in] */ UINT &, _In_reads_bytes_(DataSize)  const void * &)> _SetPrivateData_pre_ptr;
    NullPtr<void (DXGIObjectHook::*)(HRESULT, _In_  REFGUID, /* [in] */ UINT, _In_reads_bytes_(DataSize)  const void *)> _SetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown);
    NullPtr<void (DXGIObjectHook::*)(_In_  REFGUID, _In_  const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
    NullPtr<void (DXGIObjectHook::*)(HRESULT, _In_  REFGUID, _In_  const IUnknown *)> _SetPrivateDataInterface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData);
    NullPtr<void (DXGIObjectHook::*)(_In_  REFGUID, _Inout_  UINT * &, _Out_writes_bytes_(*pDataSize)  void * &)> _GetPrivateData_pre_ptr;
    NullPtr<void (DXGIObjectHook::*)(HRESULT, _In_  REFGUID, _Inout_  UINT *, _Out_writes_bytes_(*pDataSize)  void *)> _GetPrivateData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent);
    NullPtr<void (DXGIObjectHook::*)(_In_  REFIID, _Out_  void ** &)> _GetParent_pre_ptr;
    NullPtr<void (DXGIObjectHook::*)(HRESULT, _In_  REFIID, _Out_  void **)> _GetParent_post_ptr;

};

// ==============================================================================
// IDXGIDeviceSubObject
// ==============================================================================
class DXGIDeviceSubObjectHook : public HookBase<IDXGIDeviceSubObject>
{
    DXGIObjectHook & _DXGIObject;

protected:

    DXGIDeviceSubObjectHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
    {
    }

    ~DXGIDeviceSubObjectHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        try
        {
            return new DXGIDeviceSubObjectHook(unknown, *DXGIObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIDeviceSubObjectHook * typedPtr = (DXGIDeviceSubObjectHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice);
    NullPtr<void (DXGIDeviceSubObjectHook::*)(_In_  REFIID, _Out_  void ** &)> _GetDevice_pre_ptr;
    NullPtr<void (DXGIDeviceSubObjectHook::*)(HRESULT, _In_  REFIID, _Out_  void **)> _GetDevice_post_ptr;

};

// ==============================================================================
// IDXGIResource
// ==============================================================================
class DXGIResourceHook : public HookBase<IDXGIResource>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;

protected:

    DXGIResourceHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDeviceSubObject(DXGIDeviceSubObject)
    {
    }

    ~DXGIResourceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
        if (nullptr == DXGIDeviceSubObject) return nullptr;

        try
        {
            return new DXGIResourceHook(unknown, *DXGIObject, *DXGIDeviceSubObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIResourceHook * typedPtr = (DXGIResourceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice)
    {
        return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSharedHandle(_Out_  HANDLE * pSharedHandle);
    NullPtr<void (DXGIResourceHook::*)(_Out_  HANDLE * &)> _GetSharedHandle_pre_ptr;
    NullPtr<void (DXGIResourceHook::*)(HRESULT, _Out_  HANDLE *)> _GetSharedHandle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetUsage(_Out_  DXGI_USAGE * pUsage);
    NullPtr<void (DXGIResourceHook::*)(_Out_  DXGI_USAGE * &)> _GetUsage_pre_ptr;
    NullPtr<void (DXGIResourceHook::*)(HRESULT, _Out_  DXGI_USAGE *)> _GetUsage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetEvictionPriority(/* [in] */ UINT EvictionPriority);
    NullPtr<void (DXGIResourceHook::*)(/* [in] */ UINT &)> _SetEvictionPriority_pre_ptr;
    NullPtr<void (DXGIResourceHook::*)(HRESULT, /* [in] */ UINT)> _SetEvictionPriority_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetEvictionPriority(_Out_  UINT * pEvictionPriority);
    NullPtr<void (DXGIResourceHook::*)(_Out_  UINT * &)> _GetEvictionPriority_pre_ptr;
    NullPtr<void (DXGIResourceHook::*)(HRESULT, _Out_  UINT *)> _GetEvictionPriority_post_ptr;

};

// ==============================================================================
// IDXGIKeyedMutex
// ==============================================================================
class DXGIKeyedMutexHook : public HookBase<IDXGIKeyedMutex>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;

protected:

    DXGIKeyedMutexHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDeviceSubObject(DXGIDeviceSubObject)
    {
    }

    ~DXGIKeyedMutexHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
        if (nullptr == DXGIDeviceSubObject) return nullptr;

        try
        {
            return new DXGIKeyedMutexHook(unknown, *DXGIObject, *DXGIDeviceSubObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIKeyedMutexHook * typedPtr = (DXGIKeyedMutexHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice)
    {
        return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AcquireSync(/* [in] */ UINT64 Key, /* [in] */ DWORD dwMilliseconds);
    NullPtr<void (DXGIKeyedMutexHook::*)(/* [in] */ UINT64 &, /* [in] */ DWORD &)> _AcquireSync_pre_ptr;
    NullPtr<void (DXGIKeyedMutexHook::*)(HRESULT, /* [in] */ UINT64, /* [in] */ DWORD)> _AcquireSync_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReleaseSync(/* [in] */ UINT64 Key);
    NullPtr<void (DXGIKeyedMutexHook::*)(/* [in] */ UINT64 &)> _ReleaseSync_pre_ptr;
    NullPtr<void (DXGIKeyedMutexHook::*)(HRESULT, /* [in] */ UINT64)> _ReleaseSync_post_ptr;

};

// ==============================================================================
// IDXGISurface
// ==============================================================================
class DXGISurfaceHook : public HookBase<IDXGISurface>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;

protected:

    DXGISurfaceHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDeviceSubObject(DXGIDeviceSubObject)
    {
    }

    ~DXGISurfaceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
        if (nullptr == DXGIDeviceSubObject) return nullptr;

        try
        {
            return new DXGISurfaceHook(unknown, *DXGIObject, *DXGIDeviceSubObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGISurfaceHook * typedPtr = (DXGISurfaceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice)
    {
        return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_SURFACE_DESC * pDesc);
    NullPtr<void (DXGISurfaceHook::*)(_Out_  DXGI_SURFACE_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (DXGISurfaceHook::*)(HRESULT, _Out_  DXGI_SURFACE_DESC *)> _GetDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Map(_Out_  DXGI_MAPPED_RECT * pLockedRect, /* [in] */ UINT MapFlags);
    NullPtr<void (DXGISurfaceHook::*)(_Out_  DXGI_MAPPED_RECT * &, /* [in] */ UINT &)> _Map_pre_ptr;
    NullPtr<void (DXGISurfaceHook::*)(HRESULT, _Out_  DXGI_MAPPED_RECT *, /* [in] */ UINT)> _Map_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Unmap();
    NullPtr<void (DXGISurfaceHook::*)()> _Unmap_pre_ptr;
    NullPtr<void (DXGISurfaceHook::*)(HRESULT)> _Unmap_post_ptr;

};

// ==============================================================================
// IDXGISurface1
// ==============================================================================
class DXGISurface1Hook : public HookBase<IDXGISurface1>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;
    DXGISurfaceHook & _DXGISurface;

protected:

    DXGISurface1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, DXGISurfaceHook & DXGISurface,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDeviceSubObject(DXGIDeviceSubObject)
        , _DXGISurface(DXGISurface)
    {
    }

    ~DXGISurface1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
        if (nullptr == DXGIDeviceSubObject) return nullptr;

        DXGISurfaceHook * DXGISurface = (DXGISurfaceHook *)unknown.GetHookedObj(__uuidof(IDXGISurface));
        if (nullptr == DXGISurface) return nullptr;

        try
        {
            return new DXGISurface1Hook(unknown, *DXGIObject, *DXGIDeviceSubObject, *DXGISurface, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGISurface1Hook * typedPtr = (DXGISurface1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice)
    {
        return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_SURFACE_DESC * pDesc)
    {
        return _DXGISurface.GetDesc(pDesc);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Map(_Out_  DXGI_MAPPED_RECT * pLockedRect, /* [in] */ UINT MapFlags)
    {
        return _DXGISurface.Map(pLockedRect, MapFlags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Unmap()
    {
        return _DXGISurface.Unmap();
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDC(/* [in] */ BOOL Discard, _Out_  HDC * phdc);
    NullPtr<void (DXGISurface1Hook::*)(/* [in] */ BOOL &, _Out_  HDC * &)> _GetDC_pre_ptr;
    NullPtr<void (DXGISurface1Hook::*)(HRESULT, /* [in] */ BOOL, _Out_  HDC *)> _GetDC_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReleaseDC(_In_opt_  RECT * pDirtyRect);
    NullPtr<void (DXGISurface1Hook::*)(_In_opt_  RECT * &)> _ReleaseDC_pre_ptr;
    NullPtr<void (DXGISurface1Hook::*)(HRESULT, _In_opt_  RECT *)> _ReleaseDC_post_ptr;

};

// ==============================================================================
// IDXGIAdapter
// ==============================================================================
class DXGIAdapterHook : public HookBase<IDXGIAdapter>
{
    DXGIObjectHook & _DXGIObject;

protected:

    DXGIAdapterHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
    {
    }

    ~DXGIAdapterHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        try
        {
            return new DXGIAdapterHook(unknown, *DXGIObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIAdapterHook * typedPtr = (DXGIAdapterHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EnumOutputs(/* [in] */ UINT Output, _Out_  IDXGIOutput ** ppOutput);
    NullPtr<void (DXGIAdapterHook::*)(/* [in] */ UINT &, _Out_  IDXGIOutput ** &)> _EnumOutputs_pre_ptr;
    NullPtr<void (DXGIAdapterHook::*)(HRESULT, /* [in] */ UINT, _Out_  IDXGIOutput **)> _EnumOutputs_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_ADAPTER_DESC * pDesc);
    NullPtr<void (DXGIAdapterHook::*)(_Out_  DXGI_ADAPTER_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (DXGIAdapterHook::*)(HRESULT, _Out_  DXGI_ADAPTER_DESC *)> _GetDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(_In_  REFGUID InterfaceName, _Out_  LARGE_INTEGER * pUMDVersion);
    NullPtr<void (DXGIAdapterHook::*)(_In_  REFGUID, _Out_  LARGE_INTEGER * &)> _CheckInterfaceSupport_pre_ptr;
    NullPtr<void (DXGIAdapterHook::*)(HRESULT, _In_  REFGUID, _Out_  LARGE_INTEGER *)> _CheckInterfaceSupport_post_ptr;

};

// ==============================================================================
// IDXGIOutput
// ==============================================================================
class DXGIOutputHook : public HookBase<IDXGIOutput>
{
    DXGIObjectHook & _DXGIObject;

protected:

    DXGIOutputHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
    {
    }

    ~DXGIOutputHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        try
        {
            return new DXGIOutputHook(unknown, *DXGIObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIOutputHook * typedPtr = (DXGIOutputHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_OUTPUT_DESC * pDesc);
    NullPtr<void (DXGIOutputHook::*)(_Out_  DXGI_OUTPUT_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _Out_  DXGI_OUTPUT_DESC *)> _GetDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDisplayModeList(/* [in] */ DXGI_FORMAT EnumFormat, /* [in] */ UINT Flags, _Inout_  UINT * pNumModes, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC * pDesc);
    NullPtr<void (DXGIOutputHook::*)(/* [in] */ DXGI_FORMAT &, /* [in] */ UINT &, _Inout_  UINT * &, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC * &)> _GetDisplayModeList_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, /* [in] */ DXGI_FORMAT, /* [in] */ UINT, _Inout_  UINT *, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC *)> _GetDisplayModeList_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FindClosestMatchingMode(_In_  const DXGI_MODE_DESC * pModeToMatch, _Out_  DXGI_MODE_DESC * pClosestMatch, _In_opt_  IUnknown * pConcernedDevice);
    NullPtr<void (DXGIOutputHook::*)(_In_  const DXGI_MODE_DESC * &, _Out_  DXGI_MODE_DESC * &, _In_opt_  IUnknown * &)> _FindClosestMatchingMode_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  const DXGI_MODE_DESC *, _Out_  DXGI_MODE_DESC *, _In_opt_  IUnknown *)> _FindClosestMatchingMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE WaitForVBlank();
    NullPtr<void (DXGIOutputHook::*)()> _WaitForVBlank_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT)> _WaitForVBlank_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE TakeOwnership(_In_  IUnknown * pDevice, BOOL Exclusive);
    NullPtr<void (DXGIOutputHook::*)(_In_  IUnknown * &, BOOL &)> _TakeOwnership_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  IUnknown *, BOOL)> _TakeOwnership_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ReleaseOwnership();
    NullPtr<void (DXGIOutputHook::*)()> _ReleaseOwnership_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)()> _ReleaseOwnership_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGammaControlCapabilities(_Out_  DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps);
    NullPtr<void (DXGIOutputHook::*)(_Out_  DXGI_GAMMA_CONTROL_CAPABILITIES * &)> _GetGammaControlCapabilities_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _Out_  DXGI_GAMMA_CONTROL_CAPABILITIES *)> _GetGammaControlCapabilities_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetGammaControl(_In_  const DXGI_GAMMA_CONTROL * pArray);
    NullPtr<void (DXGIOutputHook::*)(_In_  const DXGI_GAMMA_CONTROL * &)> _SetGammaControl_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  const DXGI_GAMMA_CONTROL *)> _SetGammaControl_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGammaControl(_Out_  DXGI_GAMMA_CONTROL * pArray);
    NullPtr<void (DXGIOutputHook::*)(_Out_  DXGI_GAMMA_CONTROL * &)> _GetGammaControl_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _Out_  DXGI_GAMMA_CONTROL *)> _GetGammaControl_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetDisplaySurface(_In_  IDXGISurface * pScanoutSurface);
    NullPtr<void (DXGIOutputHook::*)(_In_  IDXGISurface * &)> _SetDisplaySurface_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  IDXGISurface *)> _SetDisplaySurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDisplaySurfaceData(_In_  IDXGISurface * pDestination);
    NullPtr<void (DXGIOutputHook::*)(_In_  IDXGISurface * &)> _GetDisplaySurfaceData_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _In_  IDXGISurface *)> _GetDisplaySurfaceData_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(_Out_  DXGI_FRAME_STATISTICS * pStats);
    NullPtr<void (DXGIOutputHook::*)(_Out_  DXGI_FRAME_STATISTICS * &)> _GetFrameStatistics_pre_ptr;
    NullPtr<void (DXGIOutputHook::*)(HRESULT, _Out_  DXGI_FRAME_STATISTICS *)> _GetFrameStatistics_post_ptr;

};

// ==============================================================================
// IDXGISwapChain
// ==============================================================================
class DXGISwapChainHook : public HookBase<IDXGISwapChain>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;

protected:

    DXGISwapChainHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDeviceSubObject(DXGIDeviceSubObject)
    {
    }

    ~DXGISwapChainHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
        if (nullptr == DXGIDeviceSubObject) return nullptr;

        try
        {
            return new DXGISwapChainHook(unknown, *DXGIObject, *DXGIDeviceSubObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGISwapChainHook * typedPtr = (DXGISwapChainHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice)
    {
        return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Present(/* [in] */ UINT SyncInterval, /* [in] */ UINT Flags);
    NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, /* [in] */ UINT &)> _Present_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, /* [in] */ UINT)> _Present_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetBuffer(/* [in] */ UINT Buffer, _In_  REFIID riid, _Out_  void ** ppSurface);
    NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, _In_  REFIID, _Out_  void ** &)> _GetBuffer_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, _In_  REFIID, _Out_  void **)> _GetBuffer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFullscreenState(/* [in] */ BOOL Fullscreen, _In_opt_  IDXGIOutput * pTarget);
    NullPtr<void (DXGISwapChainHook::*)(/* [in] */ BOOL &, _In_opt_  IDXGIOutput * &)> _SetFullscreenState_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ BOOL, _In_opt_  IDXGIOutput *)> _SetFullscreenState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(_Out_opt_  BOOL * pFullscreen, _Out_opt_  IDXGIOutput ** ppTarget);
    NullPtr<void (DXGISwapChainHook::*)(_Out_opt_  BOOL * &, _Out_opt_  IDXGIOutput ** &)> _GetFullscreenState_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_opt_  BOOL *, _Out_opt_  IDXGIOutput **)> _GetFullscreenState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_SWAP_CHAIN_DESC * pDesc);
    NullPtr<void (DXGISwapChainHook::*)(_Out_  DXGI_SWAP_CHAIN_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  DXGI_SWAP_CHAIN_DESC *)> _GetDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ResizeBuffers(/* [in] */ UINT BufferCount, /* [in] */ UINT Width, /* [in] */ UINT Height, /* [in] */ DXGI_FORMAT NewFormat, /* [in] */ UINT SwapChainFlags);
    NullPtr<void (DXGISwapChainHook::*)(/* [in] */ UINT &, /* [in] */ UINT &, /* [in] */ UINT &, /* [in] */ DXGI_FORMAT &, /* [in] */ UINT &)> _ResizeBuffers_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, /* [in] */ UINT, /* [in] */ UINT, /* [in] */ UINT, /* [in] */ DXGI_FORMAT, /* [in] */ UINT)> _ResizeBuffers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ResizeTarget(_In_  const DXGI_MODE_DESC * pNewTargetParameters);
    NullPtr<void (DXGISwapChainHook::*)(_In_  const DXGI_MODE_DESC * &)> _ResizeTarget_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, _In_  const DXGI_MODE_DESC *)> _ResizeTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(_Out_  IDXGIOutput ** ppOutput);
    NullPtr<void (DXGISwapChainHook::*)(_Out_  IDXGIOutput ** &)> _GetContainingOutput_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  IDXGIOutput **)> _GetContainingOutput_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(_Out_  DXGI_FRAME_STATISTICS * pStats);
    NullPtr<void (DXGISwapChainHook::*)(_Out_  DXGI_FRAME_STATISTICS * &)> _GetFrameStatistics_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  DXGI_FRAME_STATISTICS *)> _GetFrameStatistics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(_Out_  UINT * pLastPresentCount);
    NullPtr<void (DXGISwapChainHook::*)(_Out_  UINT * &)> _GetLastPresentCount_pre_ptr;
    NullPtr<void (DXGISwapChainHook::*)(HRESULT, _Out_  UINT *)> _GetLastPresentCount_post_ptr;

};

// ==============================================================================
// IDXGIFactory
// ==============================================================================
class DXGIFactoryHook : public HookBase<IDXGIFactory>
{
    DXGIObjectHook & _DXGIObject;

protected:

    DXGIFactoryHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
    {
    }

    ~DXGIFactoryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        try
        {
            return new DXGIFactoryHook(unknown, *DXGIObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIFactoryHook * typedPtr = (DXGIFactoryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EnumAdapters(/* [in] */ UINT Adapter, _Out_  IDXGIAdapter ** ppAdapter);
    NullPtr<void (DXGIFactoryHook::*)(/* [in] */ UINT &, _Out_  IDXGIAdapter ** &)> _EnumAdapters_pre_ptr;
    NullPtr<void (DXGIFactoryHook::*)(HRESULT, /* [in] */ UINT, _Out_  IDXGIAdapter **)> _EnumAdapters_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE MakeWindowAssociation(HWND WindowHandle, UINT Flags);
    NullPtr<void (DXGIFactoryHook::*)(HWND &, UINT &)> _MakeWindowAssociation_pre_ptr;
    NullPtr<void (DXGIFactoryHook::*)(HRESULT, HWND, UINT)> _MakeWindowAssociation_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetWindowAssociation(_Out_  HWND * pWindowHandle);
    NullPtr<void (DXGIFactoryHook::*)(_Out_  HWND * &)> _GetWindowAssociation_pre_ptr;
    NullPtr<void (DXGIFactoryHook::*)(HRESULT, _Out_  HWND *)> _GetWindowAssociation_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSwapChain(_In_  IUnknown * pDevice, _In_  DXGI_SWAP_CHAIN_DESC * pDesc, _Out_  IDXGISwapChain ** ppSwapChain);
    NullPtr<void (DXGIFactoryHook::*)(_In_  IUnknown * &, _In_  DXGI_SWAP_CHAIN_DESC * &, _Out_  IDXGISwapChain ** &)> _CreateSwapChain_pre_ptr;
    NullPtr<void (DXGIFactoryHook::*)(HRESULT, _In_  IUnknown *, _In_  DXGI_SWAP_CHAIN_DESC *, _Out_  IDXGISwapChain **)> _CreateSwapChain_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSoftwareAdapter(/* [in] */ HMODULE Module, _Out_  IDXGIAdapter ** ppAdapter);
    NullPtr<void (DXGIFactoryHook::*)(/* [in] */ HMODULE &, _Out_  IDXGIAdapter ** &)> _CreateSoftwareAdapter_pre_ptr;
    NullPtr<void (DXGIFactoryHook::*)(HRESULT, /* [in] */ HMODULE, _Out_  IDXGIAdapter **)> _CreateSoftwareAdapter_post_ptr;

};

// ==============================================================================
// IDXGIDevice
// ==============================================================================
class DXGIDeviceHook : public HookBase<IDXGIDevice>
{
    DXGIObjectHook & _DXGIObject;

protected:

    DXGIDeviceHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
    {
    }

    ~DXGIDeviceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        try
        {
            return new DXGIDeviceHook(unknown, *DXGIObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIDeviceHook * typedPtr = (DXGIDeviceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetAdapter(_Out_  IDXGIAdapter ** pAdapter);
    NullPtr<void (DXGIDeviceHook::*)(_Out_  IDXGIAdapter ** &)> _GetAdapter_pre_ptr;
    NullPtr<void (DXGIDeviceHook::*)(HRESULT, _Out_  IDXGIAdapter **)> _GetAdapter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSurface(_In_  const DXGI_SURFACE_DESC * pDesc, /* [in] */ UINT NumSurfaces, /* [in] */ DXGI_USAGE Usage, _In_opt_  const DXGI_SHARED_RESOURCE * pSharedResource, _Out_  IDXGISurface ** ppSurface);
    NullPtr<void (DXGIDeviceHook::*)(_In_  const DXGI_SURFACE_DESC * &, /* [in] */ UINT &, /* [in] */ DXGI_USAGE &, _In_opt_  const DXGI_SHARED_RESOURCE * &, _Out_  IDXGISurface ** &)> _CreateSurface_pre_ptr;
    NullPtr<void (DXGIDeviceHook::*)(HRESULT, _In_  const DXGI_SURFACE_DESC *, /* [in] */ UINT, /* [in] */ DXGI_USAGE, _In_opt_  const DXGI_SHARED_RESOURCE *, _Out_  IDXGISurface **)> _CreateSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE QueryResourceResidency(_In_reads_(NumResources)  IUnknown *const * ppResources, _Out_writes_(NumResources)  DXGI_RESIDENCY * pResidencyStatus, /* [in] */ UINT NumResources);
    NullPtr<void (DXGIDeviceHook::*)(_In_reads_(NumResources)  IUnknown *const * &, _Out_writes_(NumResources)  DXGI_RESIDENCY * &, /* [in] */ UINT &)> _QueryResourceResidency_pre_ptr;
    NullPtr<void (DXGIDeviceHook::*)(HRESULT, _In_reads_(NumResources)  IUnknown *const *, _Out_writes_(NumResources)  DXGI_RESIDENCY *, /* [in] */ UINT)> _QueryResourceResidency_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetGPUThreadPriority(/* [in] */ INT Priority);
    NullPtr<void (DXGIDeviceHook::*)(/* [in] */ INT &)> _SetGPUThreadPriority_pre_ptr;
    NullPtr<void (DXGIDeviceHook::*)(HRESULT, /* [in] */ INT)> _SetGPUThreadPriority_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGPUThreadPriority(_Out_  INT * pPriority);
    NullPtr<void (DXGIDeviceHook::*)(_Out_  INT * &)> _GetGPUThreadPriority_pre_ptr;
    NullPtr<void (DXGIDeviceHook::*)(HRESULT, _Out_  INT *)> _GetGPUThreadPriority_post_ptr;

};

// ==============================================================================
// IDXGIFactory1
// ==============================================================================
class DXGIFactory1Hook : public HookBase<IDXGIFactory1>
{
    DXGIObjectHook & _DXGIObject;
    DXGIFactoryHook & _DXGIFactory;

protected:

    DXGIFactory1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIFactoryHook & DXGIFactory,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIFactory(DXGIFactory)
    {
    }

    ~DXGIFactory1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIFactoryHook * DXGIFactory = (DXGIFactoryHook *)unknown.GetHookedObj(__uuidof(IDXGIFactory));
        if (nullptr == DXGIFactory) return nullptr;

        try
        {
            return new DXGIFactory1Hook(unknown, *DXGIObject, *DXGIFactory, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIFactory1Hook * typedPtr = (DXGIFactory1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EnumAdapters(/* [in] */ UINT Adapter, _Out_  IDXGIAdapter ** ppAdapter)
    {
        return _DXGIFactory.EnumAdapters(Adapter, ppAdapter);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE MakeWindowAssociation(HWND WindowHandle, UINT Flags)
    {
        return _DXGIFactory.MakeWindowAssociation(WindowHandle, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWindowAssociation(_Out_  HWND * pWindowHandle)
    {
        return _DXGIFactory.GetWindowAssociation(pWindowHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSwapChain(_In_  IUnknown * pDevice, _In_  DXGI_SWAP_CHAIN_DESC * pDesc, _Out_  IDXGISwapChain ** ppSwapChain)
    {
        return _DXGIFactory.CreateSwapChain(pDevice, pDesc, ppSwapChain);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSoftwareAdapter(/* [in] */ HMODULE Module, _Out_  IDXGIAdapter ** ppAdapter)
    {
        return _DXGIFactory.CreateSoftwareAdapter(Module, ppAdapter);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EnumAdapters1(/* [in] */ UINT Adapter, _Out_  IDXGIAdapter1 ** ppAdapter);
    NullPtr<void (DXGIFactory1Hook::*)(/* [in] */ UINT &, _Out_  IDXGIAdapter1 ** &)> _EnumAdapters1_pre_ptr;
    NullPtr<void (DXGIFactory1Hook::*)(HRESULT, /* [in] */ UINT, _Out_  IDXGIAdapter1 **)> _EnumAdapters1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsCurrent();
    NullPtr<void (DXGIFactory1Hook::*)()> _IsCurrent_pre_ptr;
    NullPtr<void (DXGIFactory1Hook::*)(BOOL)> _IsCurrent_post_ptr;

};

// ==============================================================================
// IDXGIAdapter1
// ==============================================================================
class DXGIAdapter1Hook : public HookBase<IDXGIAdapter1>
{
    DXGIObjectHook & _DXGIObject;
    DXGIAdapterHook & _DXGIAdapter;

protected:

    DXGIAdapter1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIAdapterHook & DXGIAdapter,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIAdapter(DXGIAdapter)
    {
    }

    ~DXGIAdapter1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIAdapterHook * DXGIAdapter = (DXGIAdapterHook *)unknown.GetHookedObj(__uuidof(IDXGIAdapter));
        if (nullptr == DXGIAdapter) return nullptr;

        try
        {
            return new DXGIAdapter1Hook(unknown, *DXGIObject, *DXGIAdapter, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIAdapter1Hook * typedPtr = (DXGIAdapter1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EnumOutputs(/* [in] */ UINT Output, _Out_  IDXGIOutput ** ppOutput)
    {
        return _DXGIAdapter.EnumOutputs(Output, ppOutput);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_ADAPTER_DESC * pDesc)
    {
        return _DXGIAdapter.GetDesc(pDesc);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(_In_  REFGUID InterfaceName, _Out_  LARGE_INTEGER * pUMDVersion)
    {
        return _DXGIAdapter.CheckInterfaceSupport(InterfaceName, pUMDVersion);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc1(_Out_  DXGI_ADAPTER_DESC1 * pDesc);
    NullPtr<void (DXGIAdapter1Hook::*)(_Out_  DXGI_ADAPTER_DESC1 * &)> _GetDesc1_pre_ptr;
    NullPtr<void (DXGIAdapter1Hook::*)(HRESULT, _Out_  DXGI_ADAPTER_DESC1 *)> _GetDesc1_post_ptr;

};

// ==============================================================================
// IDXGIDevice1
// ==============================================================================
class DXGIDevice1Hook : public HookBase<IDXGIDevice1>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceHook & _DXGIDevice;

protected:

    DXGIDevice1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceHook & DXGIDevice,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDevice(DXGIDevice)
    {
    }

    ~DXGIDevice1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceHook * DXGIDevice = (DXGIDeviceHook *)unknown.GetHookedObj(__uuidof(IDXGIDevice));
        if (nullptr == DXGIDevice) return nullptr;

        try
        {
            return new DXGIDevice1Hook(unknown, *DXGIObject, *DXGIDevice, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIDevice1Hook * typedPtr = (DXGIDevice1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetAdapter(_Out_  IDXGIAdapter ** pAdapter)
    {
        return _DXGIDevice.GetAdapter(pAdapter);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSurface(_In_  const DXGI_SURFACE_DESC * pDesc, /* [in] */ UINT NumSurfaces, /* [in] */ DXGI_USAGE Usage, _In_opt_  const DXGI_SHARED_RESOURCE * pSharedResource, _Out_  IDXGISurface ** ppSurface)
    {
        return _DXGIDevice.CreateSurface(pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE QueryResourceResidency(_In_reads_(NumResources)  IUnknown *const * ppResources, _Out_writes_(NumResources)  DXGI_RESIDENCY * pResidencyStatus, /* [in] */ UINT NumResources)
    {
        return _DXGIDevice.QueryResourceResidency(ppResources, pResidencyStatus, NumResources);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetGPUThreadPriority(/* [in] */ INT Priority)
    {
        return _DXGIDevice.SetGPUThreadPriority(Priority);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetGPUThreadPriority(_Out_  INT * pPriority)
    {
        return _DXGIDevice.GetGPUThreadPriority(pPriority);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetMaximumFrameLatency(/* [in] */ UINT MaxLatency);
    NullPtr<void (DXGIDevice1Hook::*)(/* [in] */ UINT &)> _SetMaximumFrameLatency_pre_ptr;
    NullPtr<void (DXGIDevice1Hook::*)(HRESULT, /* [in] */ UINT)> _SetMaximumFrameLatency_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetMaximumFrameLatency(_Out_  UINT * pMaxLatency);
    NullPtr<void (DXGIDevice1Hook::*)(_Out_  UINT * &)> _GetMaximumFrameLatency_pre_ptr;
    NullPtr<void (DXGIDevice1Hook::*)(HRESULT, _Out_  UINT *)> _GetMaximumFrameLatency_post_ptr;

};

// ==============================================================================
// IDXGIDisplayControl
// ==============================================================================
class DXGIDisplayControlHook : public HookBase<IDXGIDisplayControl>
{

protected:

    DXGIDisplayControlHook(UnknownBase & unknown,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DXGIDisplayControlHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DXGIDisplayControlHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIDisplayControlHook * typedPtr = (DXGIDisplayControlHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsStereoEnabled();
    NullPtr<void (DXGIDisplayControlHook::*)()> _IsStereoEnabled_pre_ptr;
    NullPtr<void (DXGIDisplayControlHook::*)(BOOL)> _IsStereoEnabled_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetStereoEnabled(BOOL enabled);
    NullPtr<void (DXGIDisplayControlHook::*)(BOOL &)> _SetStereoEnabled_pre_ptr;
    NullPtr<void (DXGIDisplayControlHook::*)(BOOL)> _SetStereoEnabled_post_ptr;

};

// ==============================================================================
// IDXGIOutputDuplication
// ==============================================================================
class DXGIOutputDuplicationHook : public HookBase<IDXGIOutputDuplication>
{
    DXGIObjectHook & _DXGIObject;

protected:

    DXGIOutputDuplicationHook(UnknownBase & unknown, DXGIObjectHook & DXGIObject,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
    {
    }

    ~DXGIOutputDuplicationHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        try
        {
            return new DXGIOutputDuplicationHook(unknown, *DXGIObject, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIOutputDuplicationHook * typedPtr = (DXGIOutputDuplicationHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesc(_Out_  DXGI_OUTDUPL_DESC * pDesc);
    NullPtr<void (DXGIOutputDuplicationHook::*)(_Out_  DXGI_OUTDUPL_DESC * &)> _GetDesc_pre_ptr;
    NullPtr<void (DXGIOutputDuplicationHook::*)(_Out_  DXGI_OUTDUPL_DESC *)> _GetDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AcquireNextFrame(_In_  UINT TimeoutInMilliseconds, _Out_  DXGI_OUTDUPL_FRAME_INFO * pFrameInfo, _Out_  IDXGIResource ** ppDesktopResource);
    NullPtr<void (DXGIOutputDuplicationHook::*)(_In_  UINT &, _Out_  DXGI_OUTDUPL_FRAME_INFO * &, _Out_  IDXGIResource ** &)> _AcquireNextFrame_pre_ptr;
    NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _In_  UINT, _Out_  DXGI_OUTDUPL_FRAME_INFO *, _Out_  IDXGIResource **)> _AcquireNextFrame_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFrameDirtyRects(_In_  UINT DirtyRectsBufferSize, _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT * pDirtyRectsBuffer, _Out_  UINT * pDirtyRectsBufferSizeRequired);
    NullPtr<void (DXGIOutputDuplicationHook::*)(_In_  UINT &, _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT * &, _Out_  UINT * &)> _GetFrameDirtyRects_pre_ptr;
    NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT *, _Out_  UINT *)> _GetFrameDirtyRects_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFrameMoveRects(_In_  UINT MoveRectsBufferSize, _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT * pMoveRectBuffer, _Out_  UINT * pMoveRectsBufferSizeRequired);
    NullPtr<void (DXGIOutputDuplicationHook::*)(_In_  UINT &, _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT * &, _Out_  UINT * &)> _GetFrameMoveRects_pre_ptr;
    NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT *, _Out_  UINT *)> _GetFrameMoveRects_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFramePointerShape(_In_  UINT PointerShapeBufferSize, _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void * pPointerShapeBuffer, _Out_  UINT * pPointerShapeBufferSizeRequired, _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO * pPointerShapeInfo);
    NullPtr<void (DXGIOutputDuplicationHook::*)(_In_  UINT &, _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void * &, _Out_  UINT * &, _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO * &)> _GetFramePointerShape_pre_ptr;
    NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _In_  UINT, _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void *, _Out_  UINT *, _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO *)> _GetFramePointerShape_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE MapDesktopSurface(_Out_  DXGI_MAPPED_RECT * pLockedRect);
    NullPtr<void (DXGIOutputDuplicationHook::*)(_Out_  DXGI_MAPPED_RECT * &)> _MapDesktopSurface_pre_ptr;
    NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT, _Out_  DXGI_MAPPED_RECT *)> _MapDesktopSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnMapDesktopSurface();
    NullPtr<void (DXGIOutputDuplicationHook::*)()> _UnMapDesktopSurface_pre_ptr;
    NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT)> _UnMapDesktopSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReleaseFrame();
    NullPtr<void (DXGIOutputDuplicationHook::*)()> _ReleaseFrame_pre_ptr;
    NullPtr<void (DXGIOutputDuplicationHook::*)(HRESULT)> _ReleaseFrame_post_ptr;

};

// ==============================================================================
// IDXGISurface2
// ==============================================================================
class DXGISurface2Hook : public HookBase<IDXGISurface2>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;
    DXGISurfaceHook & _DXGISurface;
    DXGISurface1Hook & _DXGISurface1;

protected:

    DXGISurface2Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, DXGISurfaceHook & DXGISurface, DXGISurface1Hook & DXGISurface1,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDeviceSubObject(DXGIDeviceSubObject)
        , _DXGISurface(DXGISurface)
        , _DXGISurface1(DXGISurface1)
    {
    }

    ~DXGISurface2Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
        if (nullptr == DXGIDeviceSubObject) return nullptr;

        DXGISurfaceHook * DXGISurface = (DXGISurfaceHook *)unknown.GetHookedObj(__uuidof(IDXGISurface));
        if (nullptr == DXGISurface) return nullptr;

        DXGISurface1Hook * DXGISurface1 = (DXGISurface1Hook *)unknown.GetHookedObj(__uuidof(IDXGISurface1));
        if (nullptr == DXGISurface1) return nullptr;

        try
        {
            return new DXGISurface2Hook(unknown, *DXGIObject, *DXGIDeviceSubObject, *DXGISurface, *DXGISurface1, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGISurface2Hook * typedPtr = (DXGISurface2Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice)
    {
        return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_SURFACE_DESC * pDesc)
    {
        return _DXGISurface.GetDesc(pDesc);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Map(_Out_  DXGI_MAPPED_RECT * pLockedRect, /* [in] */ UINT MapFlags)
    {
        return _DXGISurface.Map(pLockedRect, MapFlags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Unmap()
    {
        return _DXGISurface.Unmap();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDC(/* [in] */ BOOL Discard, _Out_  HDC * phdc)
    {
        return _DXGISurface1.GetDC(Discard, phdc);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ReleaseDC(_In_opt_  RECT * pDirtyRect)
    {
        return _DXGISurface1.ReleaseDC(pDirtyRect);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetResource(_In_  REFIID riid, _Out_  void ** ppParentResource, _Out_  UINT * pSubresourceIndex);
    NullPtr<void (DXGISurface2Hook::*)(_In_  REFIID, _Out_  void ** &, _Out_  UINT * &)> _GetResource_pre_ptr;
    NullPtr<void (DXGISurface2Hook::*)(HRESULT, _In_  REFIID, _Out_  void **, _Out_  UINT *)> _GetResource_post_ptr;

};

// ==============================================================================
// IDXGIResource1
// ==============================================================================
class DXGIResource1Hook : public HookBase<IDXGIResource1>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;
    DXGIResourceHook & _DXGIResource;

protected:

    DXGIResource1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, DXGIResourceHook & DXGIResource,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDeviceSubObject(DXGIDeviceSubObject)
        , _DXGIResource(DXGIResource)
    {
    }

    ~DXGIResource1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
        if (nullptr == DXGIDeviceSubObject) return nullptr;

        DXGIResourceHook * DXGIResource = (DXGIResourceHook *)unknown.GetHookedObj(__uuidof(IDXGIResource));
        if (nullptr == DXGIResource) return nullptr;

        try
        {
            return new DXGIResource1Hook(unknown, *DXGIObject, *DXGIDeviceSubObject, *DXGIResource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIResource1Hook * typedPtr = (DXGIResource1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice)
    {
        return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetSharedHandle(_Out_  HANDLE * pSharedHandle)
    {
        return _DXGIResource.GetSharedHandle(pSharedHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetUsage(_Out_  DXGI_USAGE * pUsage)
    {
        return _DXGIResource.GetUsage(pUsage);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetEvictionPriority(/* [in] */ UINT EvictionPriority)
    {
        return _DXGIResource.SetEvictionPriority(EvictionPriority);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetEvictionPriority(_Out_  UINT * pEvictionPriority)
    {
        return _DXGIResource.GetEvictionPriority(pEvictionPriority);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSubresourceSurface(UINT index, _Out_  IDXGISurface2 ** ppSurface);
    NullPtr<void (DXGIResource1Hook::*)(UINT &, _Out_  IDXGISurface2 ** &)> _CreateSubresourceSurface_pre_ptr;
    NullPtr<void (DXGIResource1Hook::*)(HRESULT, UINT, _Out_  IDXGISurface2 **)> _CreateSubresourceSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSharedHandle(_In_opt_  const SECURITY_ATTRIBUTES * pAttributes, _In_  DWORD dwAccess, _In_opt_  LPCWSTR lpName, _Out_  HANDLE * pHandle);
    NullPtr<void (DXGIResource1Hook::*)(_In_opt_  const SECURITY_ATTRIBUTES * &, _In_  DWORD &, _In_opt_  LPCWSTR &, _Out_  HANDLE * &)> _CreateSharedHandle_pre_ptr;
    NullPtr<void (DXGIResource1Hook::*)(HRESULT, _In_opt_  const SECURITY_ATTRIBUTES *, _In_  DWORD, _In_opt_  LPCWSTR, _Out_  HANDLE *)> _CreateSharedHandle_post_ptr;

};

// ==============================================================================
// IDXGIDevice2
// ==============================================================================
class DXGIDevice2Hook : public HookBase<IDXGIDevice2>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceHook & _DXGIDevice;
    DXGIDevice1Hook & _DXGIDevice1;

protected:

    DXGIDevice2Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceHook & DXGIDevice, DXGIDevice1Hook & DXGIDevice1,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDevice(DXGIDevice)
        , _DXGIDevice1(DXGIDevice1)
    {
    }

    ~DXGIDevice2Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceHook * DXGIDevice = (DXGIDeviceHook *)unknown.GetHookedObj(__uuidof(IDXGIDevice));
        if (nullptr == DXGIDevice) return nullptr;

        DXGIDevice1Hook * DXGIDevice1 = (DXGIDevice1Hook *)unknown.GetHookedObj(__uuidof(IDXGIDevice1));
        if (nullptr == DXGIDevice1) return nullptr;

        try
        {
            return new DXGIDevice2Hook(unknown, *DXGIObject, *DXGIDevice, *DXGIDevice1, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIDevice2Hook * typedPtr = (DXGIDevice2Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetAdapter(_Out_  IDXGIAdapter ** pAdapter)
    {
        return _DXGIDevice.GetAdapter(pAdapter);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSurface(_In_  const DXGI_SURFACE_DESC * pDesc, /* [in] */ UINT NumSurfaces, /* [in] */ DXGI_USAGE Usage, _In_opt_  const DXGI_SHARED_RESOURCE * pSharedResource, _Out_  IDXGISurface ** ppSurface)
    {
        return _DXGIDevice.CreateSurface(pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE QueryResourceResidency(_In_reads_(NumResources)  IUnknown *const * ppResources, _Out_writes_(NumResources)  DXGI_RESIDENCY * pResidencyStatus, /* [in] */ UINT NumResources)
    {
        return _DXGIDevice.QueryResourceResidency(ppResources, pResidencyStatus, NumResources);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetGPUThreadPriority(/* [in] */ INT Priority)
    {
        return _DXGIDevice.SetGPUThreadPriority(Priority);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetGPUThreadPriority(_Out_  INT * pPriority)
    {
        return _DXGIDevice.GetGPUThreadPriority(pPriority);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetMaximumFrameLatency(/* [in] */ UINT MaxLatency)
    {
        return _DXGIDevice1.SetMaximumFrameLatency(MaxLatency);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetMaximumFrameLatency(_Out_  UINT * pMaxLatency)
    {
        return _DXGIDevice1.GetMaximumFrameLatency(pMaxLatency);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE OfferResources(_In_  UINT NumResources, _In_reads_(NumResources)  IDXGIResource *const * ppResources, _In_  DXGI_OFFER_RESOURCE_PRIORITY Priority);
    NullPtr<void (DXGIDevice2Hook::*)(_In_  UINT &, _In_reads_(NumResources)  IDXGIResource *const * &, _In_  DXGI_OFFER_RESOURCE_PRIORITY &)> _OfferResources_pre_ptr;
    NullPtr<void (DXGIDevice2Hook::*)(HRESULT, _In_  UINT, _In_reads_(NumResources)  IDXGIResource *const *, _In_  DXGI_OFFER_RESOURCE_PRIORITY)> _OfferResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReclaimResources(_In_  UINT NumResources, _In_reads_(NumResources)  IDXGIResource *const * ppResources, _Out_writes_all_opt_(NumResources)  BOOL * pDiscarded);
    NullPtr<void (DXGIDevice2Hook::*)(_In_  UINT &, _In_reads_(NumResources)  IDXGIResource *const * &, _Out_writes_all_opt_(NumResources)  BOOL * &)> _ReclaimResources_pre_ptr;
    NullPtr<void (DXGIDevice2Hook::*)(HRESULT, _In_  UINT, _In_reads_(NumResources)  IDXGIResource *const *, _Out_writes_all_opt_(NumResources)  BOOL *)> _ReclaimResources_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EnqueueSetEvent(_In_  HANDLE hEvent);
    NullPtr<void (DXGIDevice2Hook::*)(_In_  HANDLE &)> _EnqueueSetEvent_pre_ptr;
    NullPtr<void (DXGIDevice2Hook::*)(HRESULT, _In_  HANDLE)> _EnqueueSetEvent_post_ptr;

};

// ==============================================================================
// IDXGISwapChain1
// ==============================================================================
class DXGISwapChain1Hook : public HookBase<IDXGISwapChain1>
{
    DXGIObjectHook & _DXGIObject;
    DXGIDeviceSubObjectHook & _DXGIDeviceSubObject;
    DXGISwapChainHook & _DXGISwapChain;

protected:

    DXGISwapChain1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIDeviceSubObjectHook & DXGIDeviceSubObject, DXGISwapChainHook & DXGISwapChain,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIDeviceSubObject(DXGIDeviceSubObject)
        , _DXGISwapChain(DXGISwapChain)
    {
    }

    ~DXGISwapChain1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIDeviceSubObjectHook * DXGIDeviceSubObject = (DXGIDeviceSubObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIDeviceSubObject));
        if (nullptr == DXGIDeviceSubObject) return nullptr;

        DXGISwapChainHook * DXGISwapChain = (DXGISwapChainHook *)unknown.GetHookedObj(__uuidof(IDXGISwapChain));
        if (nullptr == DXGISwapChain) return nullptr;

        try
        {
            return new DXGISwapChain1Hook(unknown, *DXGIObject, *DXGIDeviceSubObject, *DXGISwapChain, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGISwapChain1Hook * typedPtr = (DXGISwapChain1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _Out_  void ** ppDevice)
    {
        return _DXGIDeviceSubObject.GetDevice(riid, ppDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Present(/* [in] */ UINT SyncInterval, /* [in] */ UINT Flags)
    {
        return _DXGISwapChain.Present(SyncInterval, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBuffer(/* [in] */ UINT Buffer, _In_  REFIID riid, _Out_  void ** ppSurface)
    {
        return _DXGISwapChain.GetBuffer(Buffer, riid, ppSurface);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetFullscreenState(/* [in] */ BOOL Fullscreen, _In_opt_  IDXGIOutput * pTarget)
    {
        return _DXGISwapChain.SetFullscreenState(Fullscreen, pTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFullscreenState(_Out_opt_  BOOL * pFullscreen, _Out_opt_  IDXGIOutput ** ppTarget)
    {
        return _DXGISwapChain.GetFullscreenState(pFullscreen, ppTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_SWAP_CHAIN_DESC * pDesc)
    {
        return _DXGISwapChain.GetDesc(pDesc);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ResizeBuffers(/* [in] */ UINT BufferCount, /* [in] */ UINT Width, /* [in] */ UINT Height, /* [in] */ DXGI_FORMAT NewFormat, /* [in] */ UINT SwapChainFlags)
    {
        return _DXGISwapChain.ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ResizeTarget(_In_  const DXGI_MODE_DESC * pNewTargetParameters)
    {
        return _DXGISwapChain.ResizeTarget(pNewTargetParameters);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetContainingOutput(_Out_  IDXGIOutput ** ppOutput)
    {
        return _DXGISwapChain.GetContainingOutput(ppOutput);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFrameStatistics(_Out_  DXGI_FRAME_STATISTICS * pStats)
    {
        return _DXGISwapChain.GetFrameStatistics(pStats);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetLastPresentCount(_Out_  UINT * pLastPresentCount)
    {
        return _DXGISwapChain.GetLastPresentCount(pLastPresentCount);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc1(_Out_  DXGI_SWAP_CHAIN_DESC1 * pDesc);
    NullPtr<void (DXGISwapChain1Hook::*)(_Out_  DXGI_SWAP_CHAIN_DESC1 * &)> _GetDesc1_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  DXGI_SWAP_CHAIN_DESC1 *)> _GetDesc1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFullscreenDesc(_Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pDesc);
    NullPtr<void (DXGISwapChain1Hook::*)(_Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC * &)> _GetFullscreenDesc_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC *)> _GetFullscreenDesc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetHwnd(_Out_  HWND * pHwnd);
    NullPtr<void (DXGISwapChain1Hook::*)(_Out_  HWND * &)> _GetHwnd_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  HWND *)> _GetHwnd_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCoreWindow(_In_  REFIID refiid, _Out_  void ** ppUnk);
    NullPtr<void (DXGISwapChain1Hook::*)(_In_  REFIID, _Out_  void ** &)> _GetCoreWindow_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _In_  REFIID, _Out_  void **)> _GetCoreWindow_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Present1(/* [in] */ UINT SyncInterval, /* [in] */ UINT PresentFlags, _In_  const DXGI_PRESENT_PARAMETERS * pPresentParameters);
    NullPtr<void (DXGISwapChain1Hook::*)(/* [in] */ UINT &, /* [in] */ UINT &, _In_  const DXGI_PRESENT_PARAMETERS * &)> _Present1_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, /* [in] */ UINT, /* [in] */ UINT, _In_  const DXGI_PRESENT_PARAMETERS *)> _Present1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsTemporaryMonoSupported();
    NullPtr<void (DXGISwapChain1Hook::*)()> _IsTemporaryMonoSupported_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(BOOL)> _IsTemporaryMonoSupported_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRestrictToOutput(_Out_  IDXGIOutput ** ppRestrictToOutput);
    NullPtr<void (DXGISwapChain1Hook::*)(_Out_  IDXGIOutput ** &)> _GetRestrictToOutput_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  IDXGIOutput **)> _GetRestrictToOutput_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetBackgroundColor(_In_  const DXGI_RGBA * pColor);
    NullPtr<void (DXGISwapChain1Hook::*)(_In_  const DXGI_RGBA * &)> _SetBackgroundColor_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _In_  const DXGI_RGBA *)> _SetBackgroundColor_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetBackgroundColor(_Out_  DXGI_RGBA * pColor);
    NullPtr<void (DXGISwapChain1Hook::*)(_Out_  DXGI_RGBA * &)> _GetBackgroundColor_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  DXGI_RGBA *)> _GetBackgroundColor_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetRotation(_In_  DXGI_MODE_ROTATION Rotation);
    NullPtr<void (DXGISwapChain1Hook::*)(_In_  DXGI_MODE_ROTATION &)> _SetRotation_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _In_  DXGI_MODE_ROTATION)> _SetRotation_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRotation(_Out_  DXGI_MODE_ROTATION * pRotation);
    NullPtr<void (DXGISwapChain1Hook::*)(_Out_  DXGI_MODE_ROTATION * &)> _GetRotation_pre_ptr;
    NullPtr<void (DXGISwapChain1Hook::*)(HRESULT, _Out_  DXGI_MODE_ROTATION *)> _GetRotation_post_ptr;

};

// ==============================================================================
// IDXGIFactory2
// ==============================================================================
class DXGIFactory2Hook : public HookBase<IDXGIFactory2>
{
    DXGIObjectHook & _DXGIObject;
    DXGIFactoryHook & _DXGIFactory;
    DXGIFactory1Hook & _DXGIFactory1;

protected:

    DXGIFactory2Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIFactoryHook & DXGIFactory, DXGIFactory1Hook & DXGIFactory1,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIFactory(DXGIFactory)
        , _DXGIFactory1(DXGIFactory1)
    {
    }

    ~DXGIFactory2Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIFactoryHook * DXGIFactory = (DXGIFactoryHook *)unknown.GetHookedObj(__uuidof(IDXGIFactory));
        if (nullptr == DXGIFactory) return nullptr;

        DXGIFactory1Hook * DXGIFactory1 = (DXGIFactory1Hook *)unknown.GetHookedObj(__uuidof(IDXGIFactory1));
        if (nullptr == DXGIFactory1) return nullptr;

        try
        {
            return new DXGIFactory2Hook(unknown, *DXGIObject, *DXGIFactory, *DXGIFactory1, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIFactory2Hook * typedPtr = (DXGIFactory2Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EnumAdapters(/* [in] */ UINT Adapter, _Out_  IDXGIAdapter ** ppAdapter)
    {
        return _DXGIFactory.EnumAdapters(Adapter, ppAdapter);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE MakeWindowAssociation(HWND WindowHandle, UINT Flags)
    {
        return _DXGIFactory.MakeWindowAssociation(WindowHandle, Flags);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWindowAssociation(_Out_  HWND * pWindowHandle)
    {
        return _DXGIFactory.GetWindowAssociation(pWindowHandle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSwapChain(_In_  IUnknown * pDevice, _In_  DXGI_SWAP_CHAIN_DESC * pDesc, _Out_  IDXGISwapChain ** ppSwapChain)
    {
        return _DXGIFactory.CreateSwapChain(pDevice, pDesc, ppSwapChain);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSoftwareAdapter(/* [in] */ HMODULE Module, _Out_  IDXGIAdapter ** ppAdapter)
    {
        return _DXGIFactory.CreateSoftwareAdapter(Module, ppAdapter);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EnumAdapters1(/* [in] */ UINT Adapter, _Out_  IDXGIAdapter1 ** ppAdapter)
    {
        return _DXGIFactory1.EnumAdapters1(Adapter, ppAdapter);
    }

    // -----------------------------------------------------------------------------
    BOOL STDMETHODCALLTYPE IsCurrent()
    {
        return _DXGIFactory1.IsCurrent();
    }

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsWindowedStereoEnabled();
    NullPtr<void (DXGIFactory2Hook::*)()> _IsWindowedStereoEnabled_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(BOOL)> _IsWindowedStereoEnabled_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSwapChainForHwnd(_In_  IUnknown * pDevice, _In_  HWND hWnd, _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc, _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pFullscreenDesc, _In_opt_  IDXGIOutput * pRestrictToOutput, _Out_  IDXGISwapChain1 ** ppSwapChain);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  IUnknown * &, _In_  HWND &, _In_  const DXGI_SWAP_CHAIN_DESC1 * &, _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * &, _In_opt_  IDXGIOutput * &, _Out_  IDXGISwapChain1 ** &)> _CreateSwapChainForHwnd_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  IUnknown *, _In_  HWND, _In_  const DXGI_SWAP_CHAIN_DESC1 *, _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *, _In_opt_  IDXGIOutput *, _Out_  IDXGISwapChain1 **)> _CreateSwapChainForHwnd_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSwapChainForCoreWindow(_In_  IUnknown * pDevice, _In_  IUnknown * pWindow, _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc, _In_opt_  IDXGIOutput * pRestrictToOutput, _Out_  IDXGISwapChain1 ** ppSwapChain);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  IUnknown * &, _In_  IUnknown * &, _In_  const DXGI_SWAP_CHAIN_DESC1 * &, _In_opt_  IDXGIOutput * &, _Out_  IDXGISwapChain1 ** &)> _CreateSwapChainForCoreWindow_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  IUnknown *, _In_  IUnknown *, _In_  const DXGI_SWAP_CHAIN_DESC1 *, _In_opt_  IDXGIOutput *, _Out_  IDXGISwapChain1 **)> _CreateSwapChainForCoreWindow_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSharedResourceAdapterLuid(_In_  HANDLE hResource, _Out_  LUID * pLuid);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  HANDLE &, _Out_  LUID * &)> _GetSharedResourceAdapterLuid_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HANDLE, _Out_  LUID *)> _GetSharedResourceAdapterLuid_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterStereoStatusWindow(_In_  HWND WindowHandle, _In_  UINT wMsg, _Out_  DWORD * pdwCookie);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  HWND &, _In_  UINT &, _Out_  DWORD * &)> _RegisterStereoStatusWindow_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HWND, _In_  UINT, _Out_  DWORD *)> _RegisterStereoStatusWindow_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterStereoStatusEvent(_In_  HANDLE hEvent, _Out_  DWORD * pdwCookie);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  HANDLE &, _Out_  DWORD * &)> _RegisterStereoStatusEvent_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HANDLE, _Out_  DWORD *)> _RegisterStereoStatusEvent_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE UnregisterStereoStatus(_In_  DWORD dwCookie);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  DWORD &)> _UnregisterStereoStatus_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(_In_  DWORD)> _UnregisterStereoStatus_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterOcclusionStatusWindow(_In_  HWND WindowHandle, _In_  UINT wMsg, _Out_  DWORD * pdwCookie);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  HWND &, _In_  UINT &, _Out_  DWORD * &)> _RegisterOcclusionStatusWindow_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HWND, _In_  UINT, _Out_  DWORD *)> _RegisterOcclusionStatusWindow_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterOcclusionStatusEvent(_In_  HANDLE hEvent, _Out_  DWORD * pdwCookie);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  HANDLE &, _Out_  DWORD * &)> _RegisterOcclusionStatusEvent_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  HANDLE, _Out_  DWORD *)> _RegisterOcclusionStatusEvent_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE UnregisterOcclusionStatus(_In_  DWORD dwCookie);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  DWORD &)> _UnregisterOcclusionStatus_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(_In_  DWORD)> _UnregisterOcclusionStatus_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSwapChainForComposition(_In_  IUnknown * pDevice, _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc, _In_opt_  IDXGIOutput * pRestrictToOutput, _Outptr_  IDXGISwapChain1 ** ppSwapChain);
    NullPtr<void (DXGIFactory2Hook::*)(_In_  IUnknown * &, _In_  const DXGI_SWAP_CHAIN_DESC1 * &, _In_opt_  IDXGIOutput * &, _Outptr_  IDXGISwapChain1 ** &)> _CreateSwapChainForComposition_pre_ptr;
    NullPtr<void (DXGIFactory2Hook::*)(HRESULT, _In_  IUnknown *, _In_  const DXGI_SWAP_CHAIN_DESC1 *, _In_opt_  IDXGIOutput *, _Outptr_  IDXGISwapChain1 **)> _CreateSwapChainForComposition_post_ptr;

};

// ==============================================================================
// IDXGIAdapter2
// ==============================================================================
class DXGIAdapter2Hook : public HookBase<IDXGIAdapter2>
{
    DXGIObjectHook & _DXGIObject;
    DXGIAdapterHook & _DXGIAdapter;
    DXGIAdapter1Hook & _DXGIAdapter1;

protected:

    DXGIAdapter2Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIAdapterHook & DXGIAdapter, DXGIAdapter1Hook & DXGIAdapter1,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIAdapter(DXGIAdapter)
        , _DXGIAdapter1(DXGIAdapter1)
    {
    }

    ~DXGIAdapter2Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIAdapterHook * DXGIAdapter = (DXGIAdapterHook *)unknown.GetHookedObj(__uuidof(IDXGIAdapter));
        if (nullptr == DXGIAdapter) return nullptr;

        DXGIAdapter1Hook * DXGIAdapter1 = (DXGIAdapter1Hook *)unknown.GetHookedObj(__uuidof(IDXGIAdapter1));
        if (nullptr == DXGIAdapter1) return nullptr;

        try
        {
            return new DXGIAdapter2Hook(unknown, *DXGIObject, *DXGIAdapter, *DXGIAdapter1, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIAdapter2Hook * typedPtr = (DXGIAdapter2Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EnumOutputs(/* [in] */ UINT Output, _Out_  IDXGIOutput ** ppOutput)
    {
        return _DXGIAdapter.EnumOutputs(Output, ppOutput);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_ADAPTER_DESC * pDesc)
    {
        return _DXGIAdapter.GetDesc(pDesc);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(_In_  REFGUID InterfaceName, _Out_  LARGE_INTEGER * pUMDVersion)
    {
        return _DXGIAdapter.CheckInterfaceSupport(InterfaceName, pUMDVersion);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDesc1(_Out_  DXGI_ADAPTER_DESC1 * pDesc)
    {
        return _DXGIAdapter1.GetDesc1(pDesc);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesc2(_Out_  DXGI_ADAPTER_DESC2 * pDesc);
    NullPtr<void (DXGIAdapter2Hook::*)(_Out_  DXGI_ADAPTER_DESC2 * &)> _GetDesc2_pre_ptr;
    NullPtr<void (DXGIAdapter2Hook::*)(HRESULT, _Out_  DXGI_ADAPTER_DESC2 *)> _GetDesc2_post_ptr;

};

// ==============================================================================
// IDXGIOutput1
// ==============================================================================
class DXGIOutput1Hook : public HookBase<IDXGIOutput1>
{
    DXGIObjectHook & _DXGIObject;
    DXGIOutputHook & _DXGIOutput;

protected:

    DXGIOutput1Hook(UnknownBase & unknown, DXGIObjectHook & DXGIObject, DXGIOutputHook & DXGIOutput,     IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DXGIObject(DXGIObject)
        , _DXGIOutput(DXGIOutput)
    {
    }

    ~DXGIOutput1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIObjectHook * DXGIObject = (DXGIObjectHook *)unknown.GetHookedObj(__uuidof(IDXGIObject));
        if (nullptr == DXGIObject) return nullptr;

        DXGIOutputHook * DXGIOutput = (DXGIOutputHook *)unknown.GetHookedObj(__uuidof(IDXGIOutput));
        if (nullptr == DXGIOutput) return nullptr;

        try
        {
            return new DXGIOutput1Hook(unknown, *DXGIObject, *DXGIOutput, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DXGIOutput1Hook * typedPtr = (DXGIOutput1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateData(_In_  REFGUID Name, /* [in] */ UINT DataSize, _In_reads_bytes_(DataSize)  const void * pData)
    {
        return _DXGIObject.SetPrivateData(Name, DataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(_In_  REFGUID Name, _In_  const IUnknown * pUnknown)
    {
        return _DXGIObject.SetPrivateDataInterface(Name, pUnknown);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPrivateData(_In_  REFGUID Name, _Inout_  UINT * pDataSize, _Out_writes_bytes_(*pDataSize)  void * pData)
    {
        return _DXGIObject.GetPrivateData(Name, pDataSize, pData);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetParent(_In_  REFIID riid, _Out_  void ** ppParent)
    {
        return _DXGIObject.GetParent(riid, ppParent);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_OUTPUT_DESC * pDesc)
    {
        return _DXGIOutput.GetDesc(pDesc);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDisplayModeList(/* [in] */ DXGI_FORMAT EnumFormat, /* [in] */ UINT Flags, _Inout_  UINT * pNumModes, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC * pDesc)
    {
        return _DXGIOutput.GetDisplayModeList(EnumFormat, Flags, pNumModes, pDesc);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FindClosestMatchingMode(_In_  const DXGI_MODE_DESC * pModeToMatch, _Out_  DXGI_MODE_DESC * pClosestMatch, _In_opt_  IUnknown * pConcernedDevice)
    {
        return _DXGIOutput.FindClosestMatchingMode(pModeToMatch, pClosestMatch, pConcernedDevice);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE WaitForVBlank()
    {
        return _DXGIOutput.WaitForVBlank();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE TakeOwnership(_In_  IUnknown * pDevice, BOOL Exclusive)
    {
        return _DXGIOutput.TakeOwnership(pDevice, Exclusive);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE ReleaseOwnership()
    {
        return _DXGIOutput.ReleaseOwnership();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetGammaControlCapabilities(_Out_  DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps)
    {
        return _DXGIOutput.GetGammaControlCapabilities(pGammaCaps);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetGammaControl(_In_  const DXGI_GAMMA_CONTROL * pArray)
    {
        return _DXGIOutput.SetGammaControl(pArray);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetGammaControl(_Out_  DXGI_GAMMA_CONTROL * pArray)
    {
        return _DXGIOutput.GetGammaControl(pArray);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetDisplaySurface(_In_  IDXGISurface * pScanoutSurface)
    {
        return _DXGIOutput.SetDisplaySurface(pScanoutSurface);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetDisplaySurfaceData(_In_  IDXGISurface * pDestination)
    {
        return _DXGIOutput.GetDisplaySurfaceData(pDestination);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFrameStatistics(_Out_  DXGI_FRAME_STATISTICS * pStats)
    {
        return _DXGIOutput.GetFrameStatistics(pStats);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDisplayModeList1(/* [in] */ DXGI_FORMAT EnumFormat, /* [in] */ UINT Flags, _Inout_  UINT * pNumModes, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 * pDesc);
    NullPtr<void (DXGIOutput1Hook::*)(/* [in] */ DXGI_FORMAT &, /* [in] */ UINT &, _Inout_  UINT * &, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 * &)> _GetDisplayModeList1_pre_ptr;
    NullPtr<void (DXGIOutput1Hook::*)(HRESULT, /* [in] */ DXGI_FORMAT, /* [in] */ UINT, _Inout_  UINT *, _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 *)> _GetDisplayModeList1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FindClosestMatchingMode1(_In_  const DXGI_MODE_DESC1 * pModeToMatch, _Out_  DXGI_MODE_DESC1 * pClosestMatch, _In_opt_  IUnknown * pConcernedDevice);
    NullPtr<void (DXGIOutput1Hook::*)(_In_  const DXGI_MODE_DESC1 * &, _Out_  DXGI_MODE_DESC1 * &, _In_opt_  IUnknown * &)> _FindClosestMatchingMode1_pre_ptr;
    NullPtr<void (DXGIOutput1Hook::*)(HRESULT, _In_  const DXGI_MODE_DESC1 *, _Out_  DXGI_MODE_DESC1 *, _In_opt_  IUnknown *)> _FindClosestMatchingMode1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDisplaySurfaceData1(_In_  IDXGIResource * pDestination);
    NullPtr<void (DXGIOutput1Hook::*)(_In_  IDXGIResource * &)> _GetDisplaySurfaceData1_pre_ptr;
    NullPtr<void (DXGIOutput1Hook::*)(HRESULT, _In_  IDXGIResource *)> _GetDisplaySurfaceData1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DuplicateOutput(_In_  IUnknown * pDevice, _Out_  IDXGIOutputDuplication ** ppOutputDuplication);
    NullPtr<void (DXGIOutput1Hook::*)(_In_  IUnknown * &, _Out_  IDXGIOutputDuplication ** &)> _DuplicateOutput_pre_ptr;
    NullPtr<void (DXGIOutput1Hook::*)(HRESULT, _In_  IUnknown *, _Out_  IDXGIOutputDuplication **)> _DuplicateOutput_post_ptr;

};

