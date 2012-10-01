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

class D3D11DebugHook;
class D3D11InfoQueueHook;

class D3D11DeviceHook : public UnknownHook<D3D11DeviceHook, ID3D11Device, CID_ID3D11Device_COUNT>
{
public:

    D3D11DeviceHook(ID3D11Device * realobj)
        : IUNKNOWN_BASE_TYPE(realobj)
    {
        // Note: this will add reverse QI to other hook classes too.
        AddQIInterface<DXGIAdapterHook>();
        AddQIInterface<D3D11DebugHook>();
        AddQIInterface<D3D11InfoQueueHook>();

        // setup special pre/post function pointers
        _preFuncPtr[CID_ID3D11Device_CreateBuffer] = (METHOD_FUNC_PTR)&D3D11DeviceHook::CustomCreateBuffer_PRE;
    }

    ~D3D11DeviceHook()
    {
    }

    void STDMETHODCALLTYPE CustomCreateBuffer_PRE(
        const D3D11_BUFFER_DESC * &,
        const D3D11_SUBRESOURCE_DATA * &);

public:

    #include "ID3D11Device.h"
};

class D3D11DeviceContextHook : public D3D11DeviceChildHook<D3D11DeviceContextHook, ID3D11DeviceContext, CID_ID3D11DeviceContext_COUNT>
{
public:

    D3D11DeviceContextHook(ID3D11DeviceContext * realobj)
        : ID3D11DEVICECHILD_BASE_TYPE(realobj)
    {
    }

    ~D3D11DeviceContextHook()
    {
    }

    #include "ID3D11DeviceContext.h"
};

class D3D11DebugHook : public UnknownHook<D3D11DebugHook, ID3D11Debug, CID_ID3D11DeviceContext_COUNT>
{
public:

    D3D11DebugHook(ID3D11Debug * realobj)
        : IUNKNOWN_BASE_TYPE(realobj)
    {
    }

    ~D3D11DebugHook()
    {
    }

    #include "ID3D11Debug.h"
};

class D3D11InfoQueueHook : public UnknownHook<D3D11InfoQueueHook, ID3D11InfoQueue, CID_ID3D11DeviceContext_COUNT>
{
public:

    D3D11InfoQueueHook(ID3D11InfoQueue * realobj)
        : IUNKNOWN_BASE_TYPE(realobj)
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
