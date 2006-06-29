#ifndef __GN_GFX_FATMESH_H__
#define __GN_GFX_FATMESH_H__
// *****************************************************************************
//! \file    gfx/fatMesh.h
//! \brief   Universal mesh class
//! \author  chenlee (2006.6.29)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! Universal vertex that can hold almost any kind of vertex data
    //!
    struct FatVertex
    {
        //!
        //! vertex format flag
        //!
        union FormatFlag
        {
            uint32_t u32; //!< all format flags as 32-bit integer.
            struct
            {
                // byte 0
                unsigned int pos      :  3; //!< number of position coordinates, 0-4
                unsigned int weight   :  3; //!< number of weights, 0-4
                unsigned int normal   :  1; //!< has normal
                unsigned int diffuse  :  1; //!< has diffuse
                // byte 1
                unsigned int fog      :  1; //!< has fog
                unsigned int specular :  1; //!< has specular
                unsigned int tangent  :  1; //!< has tangent
                unsigned int binormal :  1; //!< has binormal
                unsigned int texcoord :  4; //!< number of texture coordinates, 0-8
                // byte 2,3
                unsigned int texMasks : 16; //!< channel masks, 2 bits for one texture.
                                            //!< 00 - 1 channel, 01 - 2 channels, 02 - 3 channels, 03 - 4 channels
            };
        };
        FormatFlag format;      //!< vertex format
        Vector4f   pos;         //!< position
        Vector4f   weight;      //!< weights, for vertex blending or skinning
        Vector3f   normal;      //!< normal
        Vector4f   diffuse;     //!< diffuse color
        Vector4f   specular;    //!< specular
        float      fog;         //!< fog cooridnate
        Vector3f   tangent;     //!< tangent
        Vector3f   binormal;    //!< binormal
        Vector4f   texcoord[8]; //!< texture coordinates
    };

    //!
    //! Universal mesh
    //!
    class FatMesh
    {
    public:

        //! \name ctor and dtor
        //@{
        FatMesh() { mTmpVertex.format.u32 = 0; }
        //@}

        //!
        //! clear to empty
        //!
        void clear() { mVertices.clear(); mFaces.clear(); }

        //! \name new vertex
        //@{
        void newVertex( const FatVertex & v ) { mVertices.append( v ); }
        /*void pos( float x, float y = .0f, float z = .0f, float w = 1.0f )
        {
            mTmpVertex.format.pos = 1;
            mTmpVertex.pos.set( x, y, z, w );
            newVertex( mTmpVertex );
            mTmpVertex.format.u32 = 0;
        }
        void weight2( float w1, float w2 )
        {
            mTmpVertex.weight.set( w1, w2, 0, 0 );
            mTmpVertex.format.weight = 2;
        }
        void weight3( float w1, float w2, float w3 )
        {
            mTmpVertex.weight.set( w1, w2, w3, 0 );
            mTmpVertex.format.weight = 3;
        }
        void weight4( float w1, float w2, float w3, float w4 )
        {
            mTmpVertex.weight.set( w1, w2, w3, w4 );
            mTmpVertex.format.weight = 4;
        }
        void normal( float x, float y, float z )
        {
            mTmpVertex.normal.set( x, y, z );
        }*/
        //@}

    private:

        //!
        //! (Triangle) face
        //!
        struct Face
        {
            //!
            //! material ID.
            //!
            //! There's no pre-defined meaning for material ID. You can use it in any way you like the most.
            //!
            int material;

            //!
            //! vertice indices
            //!
            size_t i1, i2, i3;

            //!
            //! face normal
            //!
            Vector3f normal;
        };

        DynaArray<FatVertex> mVertices;
        DynaArray<Face>      mFaces;

        FatVertex  mTmpVertex;
    };
}}

// *****************************************************************************
//                           End of fatMesh.h
// *****************************************************************************
#endif // __GN_GFX_FATMESH_H__
