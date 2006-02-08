#ifndef __GN_GFXD3D_D3DUTILS_H__
#define __GN_GFXD3D_D3DUTILS_H__
// *****************************************************************************
//! \file    d3d/d3dUtils.h
//! \brief   D3D util functions
//! \author  chenlee (2005.12.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! print shader compile error in meaning full way.
    //!
    void printShaderCompileError( HRESULT hr, const char * code, LPD3DXBUFFER err );
}}

// *****************************************************************************
//                           End of d3dUtils.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DUTILS_H__
