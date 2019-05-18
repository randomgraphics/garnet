#ifndef __GN_RT_H
#define __GN_RT_H
// *****************************************************************************
/// \file
/// \brief   Ray tracing system
/// \author  chenlee(2019.05.17)
// *****************************************************************************

#include <GNgfx2.h>

namespace GN { namespace rt
{
    using namespace gfx;

    union Vec4
    {
        Vector4f v4;
        struct
        {
            Vector3f v3;
            union
            {
                float    f;
                int32_t  i;
                uint32_t u;
            } w;
        };
    };
    static_assert(sizeof(Vec4) == 16);

    struct AABB
    {
        Vec4 min;
        Vec4 max;
    };

    class AABBTree
    {

    };

    // currently, rect light only
    struct Light
    {
        Vec4 position;
        Vec4 edges[2];
    };

    struct  Scene
    {
        std::vector<Vec4>  positions; // w channel is primitive ID
        std::vector<Vec4>  normals;   // w channel is material ID
        std::vector<AABB>  bvh;
        std::vector<Light> lights;
    };
}}

// *****************************************************************************
//                         END of GNrt.h
// *****************************************************************************
#endif
