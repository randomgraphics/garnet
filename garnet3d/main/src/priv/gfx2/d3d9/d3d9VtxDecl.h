#ifndef __GN_GFX2_D3D9VTXDECL_H__
#define __GN_GFX2_D3D9VTXDECL_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 vertex declration utils
/// \author  chenlee (2006.3.14)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// create D3D9 decl from vertex format structure
    ///
    LPDIRECT3DVERTEXDECLARATION9
    createD3D9VtxDecl(
        LPDIRECT3DDEVICE9                    dev,
        const SurfaceElementFormat * const * streams,
        size_t                               count );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9VTXDECL_H__
