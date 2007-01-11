#ifndef __GN_GFXD3D9_D3D9VERTEXDECL_H__
#define __GN_GFXD3D9_D3D9VERTEXDECL_H__
// *****************************************************************************
/// \file    d3d9/d3d9VertexDecl.h
/// \brief   D3D vertex declaration class
/// \author  chenlee (2006.3.14)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// create D3D9 decl from vertex format structure
    ///
    LPDIRECT3DVERTEXDECLARATION9
    createD3D9VertexDecl( LPDIRECT3DDEVICE9 dev, const GN::gfx::VtxFmtDesc & format );
}}

// *****************************************************************************
//                           End of d3d9VertexDecl.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9VERTEXDECL_H__
