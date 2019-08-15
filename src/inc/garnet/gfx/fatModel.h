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
    class GN_API FatVertexBuffer
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
            JOINT_ID,
            JOINT_WEIGHT,
            CUSTOM0,
            CUSTOM1,
            CUSTOM2,
            CUSTOM3,
            CUSTOM4,
            CUSTOM5,
            CUSTOM6,
            CUSTOM7,
            NUM_SEMANTICS,
            INVALID = -1,
        };

        static const uint32 MAX_TEXCOORDS;// = (uint32)(TEXCOORD_LAST - TEXCOORD0);

        static const uint32 POS_NORMAL = (1<<POSITION) | (1<<NORMAL);

        static const uint32 POS_NORMAL_TEX;// = (1<<POSITION) | (1<<NORMAL) | (1<<TEXCOORD0);

        static const uint32 POS_NORMAL_TEX_SKINNING;// = (1<<POSITION) | (1<<NORMAL) | (1<<TEXCOORD0) | (1<<JOINT_ID) | (1<<JOINT_WEIGHT);

        static const char * const SEMANTIC_NAMES[];

        static Semantic sString2Semantic( const char * s );

        union VertexElement
        {
            uint8_t u8[16];
            uint32_t u32[4];
            float f32[4];
            uint64_t u64[2];
            Vector4f & v4() { return *(Vector4f*)f32; };
            Vector3f & v3() { return *(Vector3f*)f32; };
            Vector2f & v2() { return *(Vector2f*)f32; };
        };
        // Each element is 16 bytes;
        static_assert(sizeof(VertexElement) == 16);

        FatVertexBuffer()
            : mCount(0)
            , mLayout(0)
        {
            memset( &mElements, 0, sizeof(mElements) );
            memset( &mFormats, 0, sizeof(mFormats) );
        }

        GN_NO_COPY(FatVertexBuffer);

        FatVertexBuffer( FatVertexBuffer && fvb ) { moveFrom(fvb); }

        ~FatVertexBuffer()
        {
            clear();
        }

        FatVertexBuffer & operator=( FatVertexBuffer && rhs )
        {
            clear();
            moveFrom( rhs );
            return *this;
        }

        void clear()
        {
            for(auto & e : mElements) e.clear();
            mCount = 0;
        }

        /// If resizing fails, the function returns false. And the mesh content remains unchanged.
        bool resize( uint32 layout, uint32 count );

        uint32 getLayout() const { return mLayout; }

        uint32 getVertexCount() const { return mCount; }

        VertexElement * getElementData( int semantic )
        {
            if( 0 <= semantic && semantic < (int)NUM_SEMANTICS )
            {
                return mElements[semantic].rawptr();
            }
            else
            {
                return NULL;
            }
        }

        /// Get format of the element.
        ColorFormat getElementFormat( int semantic ) const
        {
            if( 0 <= semantic && semantic < (int)NUM_SEMANTICS )
            {
                return mFormats[semantic];
            }
            else
            {
                return ColorFormat::UNKNOWN;
            }
        }

        /// getXXX() helpers
        //@{
        VertexElement * getPosition() { return mElements[POSITION].rawptr(); }
        VertexElement * getNormal() { return mElements[NORMAL].rawptr(); }
        VertexElement * getJoints() { return mElements[JOINT_ID].rawptr(); }
        VertexElement * getTexcoord( size_t stage )
        {
            size_t semantic = TEXCOORD0+stage;
            if( semantic <= TEXCOORD_LAST )
            {
                return mElements[semantic].rawptr();
            }
            else
            {
                return NULL;
            }
        }
        //@}

        /// Set element format
        void setElementFormat( int semantic, ColorFormat format )
        {
            if( 0 <= semantic && semantic < (int)NUM_SEMANTICS )
            {
                mFormats[semantic] = format;
            }
        }

        bool beginVertices(uint32_t layout, uint32_t estimatedCount = 0);
        void addVertexElement(int semantic, const VertexElement & value);
        void endVertices();

        void GenerateMeshVertexFormat( MeshVertexFormat & mvf ) const;

        bool GenerateVertexStream(
                const MeshVertexFormat & mvf,
                size_t                   stream,
                size_t                   stride,
                void                   * buffer,
                size_t                   bufferSize ) const;

    private:

        DynaArray<VertexElement> mElements[NUM_SEMANTICS];
        ColorFormat              mFormats[NUM_SEMANTICS];
        uint32                   mCount;
        uint32                   mLayout;
        DynaArray<VertexElement> mFatVertex; // temporary storage used to store the lastest vertex data between beginVertices() and endVerttices(),

        void moveFrom(FatVertexBuffer & other)
        {
            if (this == &other) return;
            for(size_t i = 0; i < NUM_SEMANTICS; ++i) {
                mElements[i] = std::move(other.mElements[i]); GN_ASSERT(other.mElements[i].empty());
                mFormats[i] = other.mFormats[i]; other.mFormats[i] = 0;
            }
            mCount = other.mCount; other.mCount = 0;
            mLayout = other.mLayout; other.mLayout = 0;
        }
    };

    struct FatMeshSubset
    {
        uint32 material; //< index into FatModel.materials array.

        // Vertex range.
        uint32 basevtx;
        uint32 numvtx;

        // Index range. If the mesh has no index buffer, index range will be ignored.
        uint32 startidx;
        uint32 numidx;

        /// Joints used by the subset. This is also a joint ID remapping table for
        /// vertices used by this subset:
        ///
        ///     joint ID = joints[vertex.jointID].
        ///
        /// If a vertex is used by more than one subsets, the FatModel loading function
        /// guarantees that each subset have exactly same mapping for that paticular
        /// vertex:
        ///
        ///    subset1.joints[TheVertex.jointID] == subset2.joints[TheVertex.jointID] == subset3....
        DynaArray<uint32,uint32> joints;
    };

    struct FatMesh
    {
        /// Constant value indicating that the mesh has no skeleton.
        static const uint32 NO_SKELETON = (uint32)-1;

        FatVertexBuffer          vertices;
        DynaArray<uint32,uint32> indices;
        PrimitiveType            primitive;
        DynaArray<FatMeshSubset> subsets;
        uint32                   skeleton; ///< index into FatModel::skeletons, or NO_SKELETON if the mesh has no skeleton.
        Boxf                     bbox;

        GN_NO_COPY(FatMesh);
        GN_DEFAULT_MOVE(FatMesh);
        FatMesh() {};
    };

    struct FatMaterial
    {
        StrA     name; //< Material name
        StrA     albedoTexture;
        StrA     normalTexture;
        Vector3f albedo;
        Vector3f emmisive;
        float    roughness = .0f;
        float    refIndex = .0f;
        bool     metal = false;

        void clear()
        {
            name.clear();
            albedoTexture.clear();
            normalTexture.clear();
            albedo.set(0, 0, 0);
            emmisive.set(0, 0, 0);
            roughness = 0.f;
            refIndex = 0.f;
            metal = false;
        }
    };

    struct FatJointBindPose
    {
        // Local transformation (relative to parent joint)
        Vector3f           position;    //< Position in parent space
        Quaternionf        rotation;    //< Rotation in local space
        Vector3f           scaling;     //< Scaling in local space.

        // Global transformation (in model space)
        Matrix44f          model2joint; //< bind pose in model space -> joint space transformation.
    };

    struct FatJoint
    {
        static const uint32 NO_JOINT = (uint32)-1;

        StrA               name;     //< Joint name (unique in a skeleton).

        // Hierarchy
        uint32             parent;   //< parent joint index. NO_JOINT, if the joint has no parent.
        uint32             child;    //< first child joint index. NO_JOINT, if the joint has no child.
        uint32             sibling;  //< next sibling joing index. NO_JOINT, if the joint has no next sibling.

        FatJointBindPose   bindPose; //< Bind pose transformation.
    };

    struct GN_API FatSkeleton
    {
        StrA                       name;   //< name of the skeleton.
        DynaArray<FatJoint,uint32> joints; //< Joint array.
        uint32                     root;   //< Index of the root joint.

        /// Print joint hierarchy to a string.
        void printJointHierarchy( StrA & ) const;
    };

    template<typename T>
    struct FatKeyFrame
    {
        float time;  //< Time stamp in seconds.
        T     value; //< The value at the specified time.
    };

    struct FatRigidAnimation
    {
        // local space -> parent space = T * R * S;
        DynaArray<FatKeyFrame<Vector3f>>    positions;
        DynaArray<FatKeyFrame<Quaternionf>> rotations;
        DynaArray<FatKeyFrame<Vector3f>>    scalings;
    };

    struct FatAnimation
    {
        StrA                                    name;               //< Name of the animation.
        double                                  duration;           //< Duration of the animation.
        DynaArray<DynaArray<FatRigidAnimation>> skeletonAnimations; //< 2D array that stores animations of each joint indexed by [skeletonIndex][jointIndex]
    };

    struct GN_API FatModel
    {
        StrA                           name;       //< name of the model. Usually the filename which the model is loaded from.
        DynaArray<FatMesh,uint32>      meshes;     //< Mesh array. Use FatMesh* to avoid expensive copy opertaion when the array is resized.
        DynaArray<FatMaterial,uint32>  materials;
        DynaArray<FatSkeleton,uint32>  skeletons;
        DynaArray<FatAnimation,uint32> skinAnimations;
        Boxf                           bbox;

        GN_NO_COPY(FatModel);
        GN_DEFAULT_MOVE(FatModel);

        FatModel()
        {
        }

        /// destructor
        ~FatModel()
        {
            clear();
        }

        /// clear the model
        void clear()
        {
            meshes.clear();
            materials.clear();
            skeletons.clear();
            skinAnimations.clear();
            bbox.clear();
        }

        // Calculate FatModel.bbox from mesh data.
        void calcBoundingBox();

        /// Split skinned mesh to meet "maximum joints per subset" criteria.
        /// If the function fails (and returns false), the FatModel will be
        /// left in undefine state. User must call clear to reset the FatModel
        /// content and start over.
        bool splitSkinnedMesh( uint32 maxJointsPerSubset );

        /// load fatmodel from file
        bool loadFromFile( const StrA & filename );

        /// save fatmodel to file.
        bool saveToFile( const StrA & filename ) const;
    };

    // Misc. ulitities
    //@{

    ///
    /// Print module file node hierarchy
    ///w
    GN_API void printModelFileNodeHierarchy( StrA & hierarchy, const StrA & filename );

    //@}
 }}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_FATMODEL_H__
