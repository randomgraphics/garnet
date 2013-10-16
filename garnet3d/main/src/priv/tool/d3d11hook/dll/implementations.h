// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"
#pragma warning(disable: 4355) // 'this' : used in base member initializer list

// -----------------------------------------------------------------------------
class DXGIObjectImpl
    : public UnknownBase
    , public DXGIObjectHook
{
public:
    DXGIObjectImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class D3D11DeviceContextImpl
    : public UnknownBase
    , public D3D11DeviceChildHook
    , public D3D11DeviceContextHook
{
public:
    D3D11DeviceContextImpl(IUnknown * realobj)
        : D3D11DeviceChildHook(*(UnknownBase*)this, realobj)
        , D3D11DeviceContextHook(*(UnknownBase*)this, *(D3D11DeviceChildHook*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class DXGIDeviceSubObjectImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceSubObjectHook
{
public:
    DXGIDeviceSubObjectImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceSubObjectHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class DXGISurfaceImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceSubObjectHook
    , public DXGISurfaceHook
{
public:
    DXGISurfaceImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceSubObjectHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , DXGISurfaceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, *(DXGIDeviceSubObjectHook*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class DXGIOutputImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIOutputHook
{
public:
    DXGIOutputImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIOutputHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
    {
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
public:
    DXGIDeviceImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11DebugHook(*(UnknownBase*)this, realobj)
        , D3D11InfoQueueHook(*(UnknownBase*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class DXGIAdapterImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIAdapterHook
{
public:
    DXGIAdapterImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIAdapterHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class DXGIAdapter1Impl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIAdapterHook
    , public DXGIAdapter1Hook
{
public:
    DXGIAdapter1Impl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIAdapterHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , DXGIAdapter1Hook(*(UnknownBase*)this, *(DXGIObjectHook*)this, *(DXGIAdapterHook*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class D3D11DeviceChildImpl
    : public UnknownBase
    , public D3D11DeviceChildHook
{
public:
    D3D11DeviceChildImpl(IUnknown * realobj)
        : D3D11DeviceChildHook(*(UnknownBase*)this, realobj)
    {
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
public:
    D3D11DeviceImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11DebugHook(*(UnknownBase*)this, realobj)
        , D3D11InfoQueueHook(*(UnknownBase*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class DXGIFactory1Impl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIFactoryHook
    , public DXGIFactory1Hook
{
public:
    DXGIFactory1Impl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIFactoryHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , DXGIFactory1Hook(*(UnknownBase*)this, *(DXGIObjectHook*)this, *(DXGIFactoryHook*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class DXGISwapChainImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIDeviceSubObjectHook
    , public DXGISwapChainHook
{
public:
    DXGISwapChainImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceSubObjectHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , DXGISwapChainHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, *(DXGIDeviceSubObjectHook*)this, realobj)
    {
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
public:
    D3D11DebugImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11DebugHook(*(UnknownBase*)this, realobj)
        , D3D11InfoQueueHook(*(UnknownBase*)this, realobj)
    {
    }
};

// -----------------------------------------------------------------------------
class DXGIFactoryImpl
    : public UnknownBase
    , public DXGIObjectHook
    , public DXGIFactoryHook
{
public:
    DXGIFactoryImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIFactoryHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
    {
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
public:
    D3D11InfoQueueImpl(IUnknown * realobj)
        : DXGIObjectHook(*(UnknownBase*)this, realobj)
        , DXGIDeviceHook(*(UnknownBase*)this, *(DXGIObjectHook*)this, realobj)
        , D3D11DeviceHook(*(UnknownBase*)this, realobj)
        , D3D11DebugHook(*(UnknownBase*)this, realobj)
        , D3D11InfoQueueHook(*(UnknownBase*)this, realobj)
    {
    }
};

