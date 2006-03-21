#ifndef __GN_GFX_D3DVERTEXDECL_H__
#define __GN_GFX_D3DVERTEXDECL_H__
// *****************************************************************************
//! \file    d3d/d3dVertexDecl.h
//! \brief   D3D vertex declaration class
//! \author  chenlee (2006.3.14)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! create D3D decl from vertex format structure
    //!
    LPDIRECT3DVERTEXDECLARATION9
    createD3DVertexDecl( LPDIRECT3DDEVICE9 dev, const GN::gfx::VtxFmtDesc & format );
}}

// *****************************************************************************
//                           End of d3dVertexDecl.h
// *****************************************************************************
#endif // __GN_GFX_D3DVERTEXDECL_H__
