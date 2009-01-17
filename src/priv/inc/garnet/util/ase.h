#ifndef __GN_UTIL_ASE_H__
#define __GN_UTIL_ASE_H__
// *****************************************************************************
/// \file
/// \brief   ASE loader
/// \author  chenli@@REDMOND (2009.1.16)
// *****************************************************************************

namespace GN { namespace util
{
    ///
    /// ASE map
    ///
    struct AseMap
    {
        StrA     name;
        StrA     class_;
        int      subno;
        float    amount;
        StrA     bitmap;
        StrA     type;
        Vector2f offset;
        Vector2f tiling;
        float    angle;
        float    blur;
        float    blur_offset;
        float    noise_amt;
        float    noise_size;
        int      noise_level;
        float    noise_phase;
        StrA     filter;
    };

    ///
    /// ASE material
    ///
    struct AseMaterial
    {
        StrA          name;
        StrA          class_;
        Vector3f      ambient, diffuse, specular;
        float         shine;
        float         shinestrength;
        float         transparency;
        float         wiresize;
        StrA          shading;
        float         xp_falloff;
        float         selfillum;
        StrA          falloff;
        StrA          xp_type;
        AseMap        mapdiff;
        AseMap        mapbump;
        UInt32        numsub;
        AseMaterial * submaterials;

        AseMaterial() : numsub(0), submaterials(0) {}

        ~AseMaterial() { safeDeleteArray(submaterials); }

        void allocSubMaterials( UInt32 count )
        {
            GN_ASSERT( 0 == numsub && 0 == submaterials );
            if( 0 == count ) return;
            submaterials = new AseMaterial[count];
            numsub = count;
        }
    };

    ///
    /// ASE vertex
    ///
    struct AseVertex
    {
        Vector3f            p;
        DynaArray<Vector3f> t;
        DynaArray<Vector3f> n;

        UInt32 addTexcoord( const Vector3f & v )
        {
            for( UInt32 i = 0; i < t.size(); ++i )
            {
                if( t[i] == v ) return i;
            }
            t.append( v );
            return (UInt32)( t.size() - 1 );
        }

        UInt32 addNormal( const Vector3f & v )
        {
            for( UInt32 i = 0; i < n.size(); ++i )
            {
                if( n[i] == v ) return i;
            }
            n.append( v );
            return (UInt32)( n.size() - 1 );
        }
    };

    ///
    /// ASE triangle face
    ///
    struct AseFace
    {
        UInt32   v[3];   ///< vertices (index into AseMesh.vertices)
        UInt32   t[3];   ///< texcoords (index into AseVertex.t)
        UInt32   vn[3];  ///< normal (index into AseVertex.n)
        Vector3f fn;     ///< face normal
        UInt32   smooth; ///< smooth group ID
        UInt32   submat; ///< sub material ID
    };

    ///
    /// ASE face chunks (faces with same sub-material)
    ///
    struct AseFaceChunk
    {
        UInt32            submat; ///< submaterial ID
        DynaArray<UInt32> faces;  ///< indices into AseMesh.faces
    };

    ///
    /// ASE mesh object
    ///
    struct AseMesh
    {
        ///
        /// this group is loaded directly from ASE file.
        //@{
        UInt32                  timevalue;
        DynaArray<AseVertex>    vertices;  ///< vertex array
        DynaArray<AseFace>      faces;     ///< face array
        //@}

        //@{
        DynaArray<AseFaceChunk> chunks; ///< faces sorted by material
        Boxf                    bbox;   ///< bounding box of the mesh itself
        //@}
    };

    ///
    /// ASE node (elemnet for mesh hierachy)
    ///
    struct AseNode
    {
        StrA      parent;
        StrA      name;
        Matrix44f transform;
        Vector3f  pos;
        Vector3f  rotaxis;
        float     rotangle; // rotation angle in radian
        Vector3f  scale;
        Boxf      bbox;   ///< bounding box of the node itself and its descendants.
    };

    ///
    /// An complete ASE geometry object that includes a mesh, a node and a meterial
    ///
    struct AseGeoObject : public GN::TreeNode<AseGeoObject>
    {
        AseNode node;
        AseMesh mesh;
        UInt32  matid; ///< material ID into global material array
    };

    ///
    /// An static ASE scene, including multiple geometry object and materials.
    ///
    struct AseScene
    {
        DynaArray<AseMaterial>  materials;
        DynaArray<AseGeoObject> objects;
        AseGeoObject            root; ///< root object

        AseGeoObject * findObj( const StrA & name )
        {
            if( name.empty() ) return &root;
            for( AseGeoObject * o = objects.begin(); o != objects.end(); ++o )
            {
                if( name == o->node.name ) return o;
            }
            return 0;
        }

        const AseMaterial & getChunkMaterial( const AseGeoObject & o, UInt32 cid ) const
        {
            GN_ASSERT( cid < o.mesh.chunks.size() );

            if( "Multi/Sub-Object" == materials[o.matid].class_ )
            {
                const AseFaceChunk & c = o.mesh.chunks[cid];

                return materials[o.matid].submaterials[c.submat];
            }
            else
            {
                GN_ASSERT( 0 == cid );
                GN_ASSERT( 1 == o.mesh.chunks.size() );
                return materials[o.matid];
            }
        }
    };

    ///
    /// load ASE scene from file
    ///
    bool loadAseSceneFromFile( AseScene & scene, File & file );

    // TODO: convert AseScene to list of mesh descriptors
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_ASE_H__
