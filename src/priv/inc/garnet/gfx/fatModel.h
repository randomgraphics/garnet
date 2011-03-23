#ifndef __GN_GFX_FATMODEL_H__
#define __GN_GFX_FATMODEL_H__
// *****************************************************************************
/// \file
/// \brief   A universal model class
/// \author  chen@@chenli-homepc (2011.3.22)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace gfx
{
    class FatVertexBuffer
    {
        void * mVertices[NUM_SEMANTICS];
        size_t mCount;
        uint32 mFormat;

    public:

        enum VertexSemantic
        {
            POSITION = 0,
            NORMAL,
            TANGENT,
            BINORMAL,
            TEXCOORD0,
            TEXCOORD1,
            TEXCOORD2,
            TEXCOORD3,
            TEXCOORD4,
            TEXCOORD5,
            TEXCOORD6,
            TEXCOORD7,
            ALBEDO,
            BONE_ID,
            BONE_WEIGHT,
            CUSTOM0,
            CUSTOM1,
            CUSTOM2,
            CUSTOM3,
            CUSTOM4,
            CUSTOM5,
            CUSTOM6,
            CUSTOM7,
            NUM_SEMANTICS,
        };

        FatVertexBuffer()
            : mCount(0)
            , mFormat(0)
        {
            memset( &mVertices, 0, sizeof(mVertices) );

        }

        ~FatVertexBuffer()
        {
            clear();
        }

        void clear()
        {
            for( int i = 0; i < NUM_SEMANTICS; ++i )
            {
                if( mVertices[i] )
                {
                    HeapMemory::dealloc( mVertices[i] );
                    mVertices[i] = NULL;
                }
            }
            mCount = 0;
        }

        // If resizing fails, the function returns false. And the mesh content remains unchanged.
        bool resize( uint32 format, size_t count );

        uint32 getVertexFormat() const { return mFormat; }

        // each semantic is an array with each element takes 128bit memory (could be float4, uint4 or int4)
        void * getVertexSementic( int semantic ) const
        {
            if( 0 <= semantic && semantic < NUM_SEMANTICS )
            {
                return mVertices[i];
            }
            else
            {
                return NULL;
            }
        }

        void * getPosition() const { return mVertices[POSITION]; }
        void * getNormal() const { return mVertices[NORMAL]; }
        void * getTexcoord0() const { return mVertices[TEXCOORD0]; }
    };

    struct FatMeshSubset
    {
        StrA   material; // name of the material.
        uint32 basevtx;
        uint32 numvtx;
        size_t startidx;
        size_t numidx;
    };

    struct FatBone
    {
        // Transformation (from parent space to bone local space)
        //  matrix = translation * rotation;
        Quaternionf rotation;
        Vector3f    translation;
    };

    struct FatMaterial
    {
        StrA     name; //< Material name
        StrA     albedoTexture;
        StrA     normalTexture;
        Vector4f albedoColor;
    };

    struct FatBonePose
    {
        FatBone pose;
        float   time;
    };

    struct FatAnimation
    {
        StrA                               name;
        DynaArray<DynaArray<FatBonePose> > keyframes; /// keyframes[bone_id][frame_id]
    };

    struct FatModel
    {
        StrA                         name;
        FatVertexBuffer              vertices;
        DynaArray<uint32>            indices;
        DynaArray<FatMeshSubset>     subsets;
        DynaArray<FatBone>           skeleton;
        StringMap<char,FatMaterial>  materials;
        StringMap<char,FatAnimation> animations;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_FATMODEL_H__
