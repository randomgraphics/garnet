#ifndef __GN_DXGIHOOK_H__
#define __GN_DXGIHOOK_H__
// *****************************************************************************
/// \file
/// \brief   DXGI interface wrappers
/// \author  chenlee (2012.8.16)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <dxgi.h>
#include "d3d11cid.h"
#include "interfacebase.h"
#include "utils.h"

class DXGIFactoryHook : public HookBase<DXGIFactoryHook, IDXGIFactory>
{
public:

    DXGIFactoryHook(IDXGIFactory * realobj) : BASE_CLASS(realobj)
    {
    }

    ~DXGIFactoryHook()
    {
    }

public:

    #include "IDXGIFactory.h"
};

class DXGIFactory1Hook : public HookBase<DXGIFactory1Hook, IDXGIFactory1>
{
public:

    DXGIFactory1Hook(IDXGIFactory1 * realobj) : BASE_CLASS(realobj)
    {
    }

    ~DXGIFactory1Hook()
    {
    }

public:

    #include "IDXGIFactory.h"
};

template<class WRAP_CLASS, class REAL_INTERFACE>
interface DXGIObjectHook : public HookBase<WRAP_CLASS, REAL_INTERFACE>
{

protected:

    DXGIObjectHook(REAL_INTERFACE * realobj) : BASE_CLASS(realobj)
    {
    }

    virtual ~DXGIObjectHook()
    {
    }

public:

    typedef DXGIObjectHook<WRAP_CLASS, REAL_INTERFACE> DXGIOBJECT_HOOK_CLASS;

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        /* [annotation][in] */
        __in  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        __in_bcount(DataSize)  const void *pData)
    {
        return GetRealObj()->SetPrivateData(Name, DataSize, pData);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        /* [annotation][in] */
        __in  REFGUID Name,
        /* [annotation][in] */
        __in  const IUnknown *pUnknown)
    {
        return GetRealObj()->SetPrivateDataInterface(Name, pUnknown);
    }

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        /* [annotation][in] */
        __in  REFGUID Name,
        /* [annotation][out][in] */
        __inout  UINT *pDataSize,
        /* [annotation][out] */
        __out_bcount(*pDataSize)  void *pData)
    {
        return GetRealObj()->GetPrivateData(Name, pDataSize, pData);
    }

    virtual HRESULT STDMETHODCALLTYPE GetParent(
        /* [annotation][in] */
        __in  REFIID riid,
        /* [annotation][retval][out] */
        __out  void **ppParent)
    {
        // TODO: Need to return the hooked parent object.
        GN_UNUSED_PARAM(riid);
        GN_UNUSED_PARAM(ppParent);
        GN_UNIMPL();
    }
};

class DXGIDeviceHook : public DXGIObjectHook<DXGIDeviceHook, IDXGIDevice>
{
public:

    DXGIDeviceHook(IDXGIAdapter * realobj)
        : DXGIOBJECT_HOOK_CLASS(realobj)
    {
    }

    ~DXGIDeviceHook()
    {
    }

public:

    #include "IDXGIDevice.h"
};

class DXGIAdapterHook : public DXGIObjectHook<DXGIAdapterHook, IDXGIAdapter>
{
public:

    DXGIAdapterHook(IDXGIAdapter * realobj)
        : DXGIOBJECT_HOOK_CLASS(realobj)
    {
    }

    ~DXGIAdapterHook()
    {
    }

public:

    #include "IDXGIAdapter.h"
};

///
/// IDXGISwapChain wrapper
///
class DXGISwapChainHook : public DXGIObjectHook<DXGISwapChainHook, IDXGISwapChain>
{
public:

    DXGISwapChainHook(IDXGISwapChain * realobj) : DXGIOBJECT_HOOK_CLASS(realobj)
    {
    }

    ~DXGISwapChainHook()
    {
    }

public:

    #include "IDXGISwapChain.h"
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_DXGIHOOK_H__
