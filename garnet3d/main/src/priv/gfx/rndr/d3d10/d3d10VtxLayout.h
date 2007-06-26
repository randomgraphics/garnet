#ifndef __GN_GFXD3D10_D3D10VTXLAYOUT_H__
#define __GN_GFXD3D10_D3D10VTXLAYOUT_H__
// *****************************************************************************
/// \file
/// \brief   D3D vertex layout class
/// \author  chenlee (2006.3.14)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// create D3D10 decl from vertex format structure
    ///
    ID3D10InputLayout *
    createD3D10VtxLayout( ID3D10Device * dev, const GN::gfx::VtxFmtDesc & format );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10VTXLAYOUT_H__
