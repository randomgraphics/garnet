#ifndef __GN_D3DWRAPPER_MYVTXBUF9_H__
#define __GN_D3DWRAPPER_MYVTXBUF9_H__
// *****************************************************************************
//! \file    d3d9Wrapper/myvtxbuf9.h
//! \brief   vertex buffer wrapper
//! \author  chen@@CHENLI-HOMEPC (2007.3.17)
// *****************************************************************************

#include "baseResource.h"

using namespace GN;

///
/// D3D9 vertex buffer wrapper
///
class MyVtxBuf9 : public BasicResource<IDirect3DVertexBuffer9>
{
    typedef BasicResource<IDirect3DVertexBuffer9> ParentType;

    DynaArray<UInt8> mSysCopy;

    UINT  mLockOffset;
    UINT  mLockBytes;
    DWORD mLockFlags;

public:

    //@{

    MyVtxBuf9( MyDevice9 * dev ) : ParentType( dev )
    {
        addRuntimeType( IID_IDirect3DVertexBuffer9, this );
    }

    HRESULT create(
        UINT Length,
        DWORD Usage,
        DWORD FVF,
        D3DPOOL Pool,
        HANDLE* pSharedHandle );

    //@}

    /// \name IDirect3DVertexBuffer9 methods
    //@{
    HRESULT __stdcall GetDesc( D3DVERTEXBUFFER_DESC * pDesc ) { return realobj()->GetDesc( pDesc ); }
    HRESULT __stdcall Lock( UINT OffsetToLock, UINT SizeToLock, VOID ** ppbData, DWORD Flags );
    HRESULT __stdcall Unlock();
    //@}
};

// *****************************************************************************
//                           End of myvtxbuf9.h
// *****************************************************************************
#endif // __GN_D3DWRAPPER_MYVTXBUF9_H__
