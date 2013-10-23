// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
static GN::Logger * sLogger = GN::getLogger("GN.d3d11hook");

// -----------------------------------------------------------------------------
class DXGIObjectImpl
    : public UnknownBase
    , public DXGIObjectHook
{
    DXGIObjectImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
    {
    }
    ~DXGIObjectImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGIObjectImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class D3D11DeviceContextImpl
    : public UnknownBase
    , public D3D11DeviceChildHook
    , public D3D11DeviceContextHook
{
    D3D11DeviceContextImpl(IUnknown * realobj)
        : D3D11DeviceChildHook(*(UnknownBase*)this, realobj)
        , D3D11DeviceContextHook(*(UnknownBase*)this, *(D3D11DeviceChildHook*)this, realobj)
    {
    }
    ~D3D11DeviceContextImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new D3D11DeviceContextImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGIDeviceSubObjectImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceSubObjectHook
{
    DXGIDeviceSubObjectImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceSubObjectHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
    {
    }
    ~DXGIDeviceSubObjectImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGIDeviceSubObjectImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class D3D11Device1Impl
    : public UnknownBase
    , public D3D11DeviceHook
    , public D3D11Device1Hook
{
    D3D11Device1Impl(IUnknown * realobj)
        : D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11Device1Hook(*(UnknownBase*)this, *(D3D11DeviceHook*)this, realobj)
    {
    }
    ~D3D11Device1Impl()
    {
    }
public:

    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new D3D11Device1Impl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGIOutputImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIOutputHook
{
    DXGIOutputImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIOutputHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
    {
    }
    ~DXGIOutputImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGIOutputImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGIDeviceImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceHook
    , public D3D11DeviceHook
    , public D3D11DebugHook
    , public D3D11InfoQueueHook
{
    DXGIDeviceImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11DebugHook(*(UnknownBase*)this, realobj)
        , D3D11InfoQueueHook(*(UnknownBase*)this, realobj)
    {
    }
    ~DXGIDeviceImpl()
    {
    }
public:

    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGIDeviceImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGISurfaceImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceSubObjectHook
    , public DXGISurfaceHook
{
    DXGISurfaceImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceSubObjectHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , DXGISurfaceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, *(DXGIDeviceSubObjectHook*)this, realobj)
    {
    }
    ~DXGISurfaceImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGISurfaceImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGIAdapterImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIAdapterHook
{
    DXGIAdapterImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIAdapterHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
    {
    }
    ~DXGIAdapterImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGIAdapterImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGIAdapter1Impl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIAdapterHook
    , public DXGIAdapter1Hook
{
    DXGIAdapter1Impl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIAdapterHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , DXGIAdapter1Hook(*(UnknownBase*)this, *(DXGIObjectHook*)this, *(DXGIAdapterHook*)this, realobj)
    {
    }
    ~DXGIAdapter1Impl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGIAdapter1Impl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class D3D11DeviceChildImpl
    : public UnknownBase
    , public D3D11DeviceChildHook
{
    D3D11DeviceChildImpl(IUnknown * realobj)
        : D3D11DeviceChildHook(*(UnknownBase*)this, realobj)
    {
    }
    ~D3D11DeviceChildImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new D3D11DeviceChildImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class D3D11DeviceImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceHook
    , public D3D11DeviceHook
    , public D3D11DebugHook
    , public D3D11InfoQueueHook
{
    D3D11DeviceImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11DebugHook(*(UnknownBase*)this, realobj)
        , D3D11InfoQueueHook(*(UnknownBase*)this, realobj)
    {
    }
    ~D3D11DeviceImpl()
    {
    }
public:

    virtual ULONG STDMETHODCALLTYPE AddRef()
        {
            return UnknownBase::AddRef();
        }

    virtual ULONG STDMETHODCALLTYPE Release()
        {
            return UnknownBase::Release();
        }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID & iid, void **ppvObject)
        {
            return UnknownBase::QueryInterface(iid, ppvObject);
        }

    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new D3D11DeviceImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGIFactory1Impl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIFactoryHook
    , public DXGIFactory1Hook
{
    DXGIFactory1Impl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIFactoryHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , DXGIFactory1Hook(*(UnknownBase*)this, *(DXGIObjectHook*)this, *(DXGIFactoryHook*)this, realobj)
    {
    }
    ~DXGIFactory1Impl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGIFactory1Impl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGISwapChainImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceSubObjectHook
    , public DXGISwapChainHook
{
    DXGISwapChainImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceSubObjectHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , DXGISwapChainHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, *(DXGIDeviceSubObjectHook*)this, realobj)
    {
    }
    ~DXGISwapChainImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGISwapChainImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class D3D11DebugImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceHook
    , public D3D11DeviceHook
    , public D3D11DebugHook
    , public D3D11InfoQueueHook
{
    D3D11DebugImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11DebugHook(*(UnknownBase*)this, realobj)
        , D3D11InfoQueueHook(*(UnknownBase*)this, realobj)
    {
    }
    ~D3D11DebugImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new D3D11DebugImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class DXGIFactoryImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIFactoryHook
{
    DXGIFactoryImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIFactoryHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
    {
    }
    ~DXGIFactoryImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new DXGIFactoryImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

// -----------------------------------------------------------------------------
class D3D11InfoQueueImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceHook
    , public D3D11DeviceHook
    , public D3D11DebugHook
    , public D3D11InfoQueueHook
{
    D3D11InfoQueueImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11DebugHook(*(UnknownBase*)this, realobj)
        , D3D11InfoQueueHook(*(UnknownBase*)this, realobj)
    {
    }
    ~D3D11InfoQueueImpl()
    {
    }
public:
    static IUnknown * NewInstance(void * context, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            IUnknown * result = (UnknownBase*)new D3D11InfoQueueImpl(realobj);
            UINT count = result->AddRef();
            count;
            GN_ASSERT(1 == count);
            return result;
        }
        catch(std::bad_alloc&)
        {
            GN_ERROR(sLogger)("Out of memory.");
            return nullptr;
        }
    }
};

