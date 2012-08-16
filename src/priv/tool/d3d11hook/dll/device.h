#ifndef __GN_D3D11HOOK_DEVICE_H__
#define __GN_D3D11HOOK_DEVICE_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 device wrappers
/// \author  chenlee (2012.8.16)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <d3d11.h>
#include "d3d11cid.h"
#include "interfacebase.h"
#include "utils.h"

///
/// ID3D11Device wrapper
///
class D3D11Device : InterfaceBase<D3D11Device, ID3D11Device, CID_ID3D11DEVICE_COUNT>
{
    typedef InterfaceBase<D3D11Device, ID3D11Device, CID_ID3D11DEVICE_COUNT> BASE_CLASS;

private:

    D3D11Device(ID3D11Device * realobj)
        : BASE_CLASS(realobj)
    {
        //AddKnownInterface<DXGIAdapter, IDXGIAdapter>();
        //AddKnownInterface<D3D11Debug, ID3D11Debug>();
        //AddKnownInterface<D3D11InfoQueue, ID3D11InfoQueue>();
        _preFuncPtr[CID_ID3D11Device_CreateBuffer] = (METHOD_FUNC_PTR)&D3D11Device::CreateBuffer_PRE;
    }

    ~D3D11Device()
    {
    }

    void STDMETHODCALLTYPE CreateBuffer_PRE(
        const D3D11_BUFFER_DESC * &,
        const D3D11_SUBRESOURCE_DATA * &)
    {
    }

public:

    NullPtr<void (D3D11Device::*)(const D3D11_BUFFER_DESC *, const D3D11_SUBRESOURCE_DATA *, ID3D11Buffer **)> _customPreFuncPtr_CreateBuffer;

    #include "ID3D11Device.h"
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D11HOOK_DEVICE_H__
