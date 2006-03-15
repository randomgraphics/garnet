#ifndef __GN_GFX_D3DRENDERSTATEBLOCK_H__
#define __GN_GFX_D3DRENDERSTATEBLOCK_H__
// *****************************************************************************
//! \file    d3d/d3dRenderStateBlock.h
//! \brief   D3D render state block class
//! \author  chenlee (2006.3.14)
// *****************************************************************************

namespace GN { namespace gfx
{
    class D3DRenderer;

    //!
    //! Apply render state block to device
    //!
    void applyRenderStateBlock( D3DRenderer &, const RenderStateBlockDesc & );
}}

// *****************************************************************************
//                           End of d3dRenderStateBlock.h
// *****************************************************************************
#endif // __GN_GFX_D3DRENDERSTATEBLOCK_H__
