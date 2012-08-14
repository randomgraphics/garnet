#ifndef __GN_D3D9WRAPPER_MYDEVICE9_H__
#define __GN_D3D9WRAPPER_MYDEVICE9_H__
// *****************************************************************************
/// \file
/// \brief   IDirect3DDevice9 wrapper
/// \author  chenlee (2006.6.24)
// *****************************************************************************

#include "baseInterface.h"

class MyD3D9;
class MyVtxBuf9;

///
/// IDirect3DDevice9 wrapper
///
class D3DDevice11 : public BasicInterface<D3DDevice11>
{
    ~D3DDevice11();

public:

    ///
    /// ctor
    ///
    D3DDevice11()
    {
    }
#include "ID3D11Device.h"
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D9WRAPPER_MYDEVICE9_H__
