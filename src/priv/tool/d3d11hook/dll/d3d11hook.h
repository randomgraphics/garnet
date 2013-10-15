#ifndef __GN_D3D11HOOK_H__
#define __GN_D3D11HOOK_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 interface wrappers
/// \author  chenlee (2012.8.16)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <d3d11.h>
#include "d3d11cid.h"
#include "interfacebase.h"
#include "utils.h"
#include "dxgihook.h"

// *****************************************************************************
// D3D11 interfaces
// *****************************************************************************

class D3D11DeviceHook : public HookBase<D3D11DeviceHook, ID3D11Device>
{
public:

    D3D11DeviceHook(ID3D11Device * realobj)
        : BASE_CLASS(realobj)
    {
        // setup special pre/post function pointers
        _CreateBuffer_pre_ptr = (METHOD_FUNC_PTR)&D3D11DeviceHook::CustomCreateBuffer_PRE;
    }

    ~D3D11DeviceHook()
    {
    }

    void STDMETHODCALLTYPE CustomCreateBuffer_PRE(
        const D3D11_BUFFER_DESC * &,
        const D3D11_SUBRESOURCE_DATA * &,
        ID3D11Buffer ** &);

public:

    #include "ID3D11Device.h"
};

class D3D11DeviceChildHook : public HookBase<D3D11DeviceChildHook, ID3D11DeviceChild>
{

protected:

    D3D11DeviceChildHook(REAL_CLASS * realobj) : BASE_CLASS(realobj)
    {
    }

    virtual ~D3D11DeviceChildHook()
    {
    }

public:

    virtual void STDMETHODCALLTYPE GetDevice(
        /* [annotation] */
        __out  ID3D11Device **ppDevice)
    {
        if (nullptr == ppDevice) return;

        GN::AutoComPtr<ID3D11Device> pRealDevice;
        GetRealObj()->GetDevice(&pRealDevice);

        ID3D11Device * pHookedDevice = GetHookedInstanceFromReal<ID3D11Device, ID3D11Device>(pRealDevice);
        GN_ASSERT(nullptr != pHookedDevice);

        *ppDevice = pHookedDevice;
    }

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        /* [annotation] */
        __in  REFGUID guid,
        /* [annotation] */
        __inout  UINT *pDataSize,
        /* [annotation] */
        __out_bcount_opt( *pDataSize )  void *pData)
    {
        return GetRealObj()->GetPrivateData(guid, pDataSize, pData);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        /* [annotation] */
        __in  REFGUID guid,
        /* [annotation] */
        __in  UINT DataSize,
        /* [annotation] */
        __in_bcount_opt( DataSize )  const void *pData)
    {
        return GetRealObj()->SetPrivateData(guid, DataSize, pData);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        /* [annotation] */
        __in  REFGUID guid,
        /* [annotation] */
        __in_opt  const IUnknown *pData)
    {
        return GetRealObj()->SetPrivateDataInterface(guid, pData);
    }
};

class D3D11DeviceContextHook : public HookBase<D3D11DeviceContextHook, ID3D11DeviceContext>
{
public:

    D3D11DeviceContextHook(ID3D11DeviceContext * realobj): BASE_CLASS(realobj)
    {
    }

    ~D3D11DeviceContextHook()
    {
    }

    #include "ID3D11DeviceContext.h"
};

class D3D11DebugHook : public HookBase<D3D11DebugHook, ID3D11Debug>
{
public:

    D3D11DebugHook(ID3D11Debug * realobj) : BASE_CLASS(realobj)
    {
    }

    ~D3D11DebugHook()
    {
    }

    #include "ID3D11Debug.h"
};

class D3D11InfoQueueHook : public HookBase<D3D11InfoQueueHook, ID3D11InfoQueue>
{
public:

    D3D11InfoQueueHook(ID3D11InfoQueue * realobj) : BASE_CLASS(realobj)
    {
    }

    ~D3D11InfoQueueHook()
    {
    }

    #include "ID3D11InfoQueue.h"
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D11HOOK_H__
