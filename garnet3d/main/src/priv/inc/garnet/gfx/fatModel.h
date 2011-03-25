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
    public:

        enum Semantic
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
            TEXCOORD_LAST = TEXCOORD7,
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
            INVALID = 0xFFFFFFFF,
        };

        static const char * const SEMANTIC_NAMES[];

        static Semantic sString2Semantic( const char * s );

        FatVertexBuffer()
            : mCount(0)
            , mLayout(0)
        {
            memset( &mElements, 0, sizeof(mElements) );
            memset( &mFormats, 0, sizeof(mFormats) );
        }

        ~FatVertexBuffer()
        {
            clear();
        }

        void clear()
        {
            for( int i = 0; i < NUM_SEMANTICS; ++i )
            {
                if( mElements[i] )
                {
                    HeapMemory::dealloc( mElements[i] );
                    mElements[i] = NULL;
                }
            }
            mCount = 0;
        }

        /// If resizing fails, the function returns false. And the mesh content remains unchanged.
        bool resize( uint32 layout, uint32 count );

        uint32 getLayout() const { return mLayout; }

        uint32 getVertexCount() const { return mCount; }

        /// No matter what the format is, element array is always in unit of 16 bytes (like float4 or int4).
        void * getElementData( int semantic ) const
        {
            if( 0 <= semantic && semantic < NUM_SEMANTICS )
            {
                return mElements[semantic];
            }
            else
            {
                return NULL;
            }
        }

        /// Get format of the element.
        ColorFormat getElementFormat( int semantic ) const
        {
            if( 0 <= semantic && semantic < NUM_SEMANTICS )
            {
                return mFormats[semantic];
            }
            else
            {
                return ColorFormat::UNKNOWN;
            }
        }

        /// Set element format
        void setElementFormat( int semantic, ColorFormat format )
        {
            if( 0 <= semantic && semantic < NUM_SEMANTICS )
            {
                mFormats[semantic] = format;
            }
        }

        void * getPosition() const { return mElements[POSITION]; }
        void * getNormal() const { return mElements[NORMAL]; }
        void * getTexcoord( size_t stage ) const
        {
            size_t semantic = TEXCOORD0+stage;
            if( 0 <= semantic && semantic <= TEXCOORD_LAST )
            {
                return mElements[semantic];
            }
            else
            {
                return NULL;
            }
        }

        void GenerateMeshVertexFormat( MeshVertexFormat & mvf ) const;

        bool GenerateVertexStream(
                const MeshVertexFormat & mvf,
                size_t                   stream,
                size_t                   stride,
                void                   * buffer,
                size_t                   bufferSize ) const;

    private:

        void      * mElements[NUM_SEMANTICS];
        ColorFormat mFormats[NUM_SEMANTICS];
        uint32      mCount;
        uint32      mLayout;
    };

    struct FatMeshSubset
    {
        StrA   material; // name of the material.
        uint32 basevtx;
        uint32 numvtx;
        uint32 startidx;
        uint32 numidx;
    };

    struct FatMesh
    {
        FatVertexBuffer          vertices;
        DynaArray<uint32,uint32> indices;
        DynaArray<FatMeshSubset> subsets;
        StrA                     skeleton;
        Boxf                     bbox;
    };

    struct FatBone
    {
        // Transformation (from parent space to bone local space)
        //  matrix = translation * rotation;
        Quaternionf rotation;
        Vector3f    translation;
    };

    struct FatSkeleton
    {
        StrA               name;
        DynaArray<FatBone> bones;
    };

    struct FatMaterial
    {
        StrA     name; //< Material name
        StrA     albedoTexture;
        StrA     normalTexture;
        Vector4f albedoColor;

        void clear()
        {
            name.clear();
            albedoTexture.clear();
            normalTexture.clear();
            albedoColor.set( 0, 0, 0, 1 );
        }
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
        StrA                         name; // name of the model. Usually the filename which the model is loaded from.
        DynaArray<FatMesh>           meshes;
        StringMap<char,FatSkeleton>  skeletons;
        StringMap<char,FatMaterial>  materials;
        StringMap<char,FatAnimation> animations;
        Boxf                         bbox;

        void clear()
        {
            meshes.clear();
            skeletons.clear();
            materials.clear();
            animations.clear();
            bbox.clear();
        }

        bool loadFromFile( const StrA & filename );
        bool saveToFile( const StrA & filename ) const;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_FATMODEL_H__
