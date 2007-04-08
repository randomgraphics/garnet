#ifndef __GN_D3D9WRAPPER_BASERESOURCE_H__
#define __GN_D3D9WRAPPER_BASERESOURCE_H__
// *****************************************************************************
//! \file    d3d9Wrapper/baseResource.h
//! \brief   Basic D3D9 resource wrapper
//! \author  chen@@CHENLI-HOMEPC (2007.3.17)
// *****************************************************************************

#include "baseInterface.h"

class MyDevice9;

///
/// Wrapper of IDirect3DBaseResource9
///
template<class T>
class BasicResource : public BasicInterface<T>
{

protected:

    //@{

    MyDevice9 * mDevice;

    BasicResource( MyDevice9 * dev) : mDevice(dev)
    {
        GN_ASSERT( dev );
        dev->AddRef();
        addRuntimeType( IID_IDirect3DResource9, this );
    }

    //@}

public:

    /// \name IDirect3DResource9 methods
    //@{
    HRESULT         __stdcall FreePrivateData(REFGUID refguid) { return realobj()->FreePrivateData( refguid ); }
    HRESULT         __stdcall GetDevice( IDirect3DDevice9 ** ppDevice );
    DWORD           __stdcall GetPriority() { return realobj()->GetPriority(); }
    HRESULT         __stdcall GetPrivateData( REFGUID refguid, void * pData, DWORD * pSizeOfData ) { return realobj()->GetPrivateData( refguid, pData, pSizeOfData ); }
    D3DRESOURCETYPE __stdcall GetType() { return realobj()->GetType(); }
    void            __stdcall PreLoad() { return realobj()->PreLoad(); }
    DWORD           __stdcall SetPriority( DWORD PriorityNew ) { return realobj()->SetPriority( PriorityNew ); }
    HRESULT         __stdcall SetPrivateData( REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags ) { return SetPrivateData( refguid, pData, SizeOfData, Flags ); }
    //@}
};

template<class T>
HRESULT BasicResource<T>::GetDevice( IDirect3DDevice9 ** ppDevice )
{
    if( !ppDevice ) return E_INVALIDARG;

    GN_ASSERT( mDevice );

    mDevice->AddRef();

    *ppDevice = mDevice;

    return S_OK;
}

// *****************************************************************************
//                           End of baseResource.h
// *****************************************************************************
#endif // __GN_D3D9WRAPPER_BASERESOURCE_H__
