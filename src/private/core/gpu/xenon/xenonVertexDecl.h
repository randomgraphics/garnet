#ifndef __GN_GFXD3D9_D3D9VERTEXDECL_H__
#define __GN_GFXD3D9_D3D9VERTEXDECL_H__
// *****************************************************************************
/// \file
/// \brief   D3D vertex declaration class
/// \author  chenlee (2006.3.14)
// *****************************************************************************

#include "XenonShader.h"

namespace GN { namespace gfx
{
    ///
    /// create D3D9 decl from vertex format structure
    ///
    IDirect3DVertexDeclaration9 *
    createXenonVertexDecl(
        IDirect3DDevice9             & dev,
        const XenonBasicGpuProgram   & gpuProgram,
        const VertexBinding          & vtxbind );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9VERTEXDECL_H__
