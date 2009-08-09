#ifndef __GN_GFX_FATMESH_H__
#define __GN_GFX_FATMESH_H__
// *****************************************************************************
/// \file
/// \brief   Universal mesh class (pure CPU data)
/// \author  chen@@chenli-homepc (2009.8.8)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace gfx
{
    struct FatVertex
    {
        Vector3 position;
        Vector3 normal;
        Vector3 texcoord;
        Vector3 tangent;
        Vector4 albedo;
        Vector4 custom[4];
    };

    class FatMesh
    {
    public:

        // clear to empty mesh
        void clear();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_FATMESH_H__
