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

        //!
        //! default ctor
        //!
        FatVertex() { format.u32 = 0; }

        //!
        //! clear to empty vertex
        //!
        void clear() { format.u32 = 0; }

        //! \name set position
        //@{
        void setPos1( float x ) { pos.set(x,0,0,1.0f); format.pos = 1; }
        void setPos2( float x, float y ) { pos.set(x,y,0,1.0f); format.pos = 2; }
        void setPos3( float x, float y, float z ) { pos.set(x,y,z,1.0f); format.pos = 3; }
        void setPos4( float x, float y, float z, float w ) { pos.set(x,y,z,w); format.pos = 4; }
        void setPos2( const Vector2f & v ) { setPos2( v.x, v.y ); }
        void setPos3( const Vector3f & v ) { setPos3( v.x, v.y, v.z ); }
        void setPos4( const Vector4f & v ) { setPos4( v.x, v.y, v.z, v.z ); }
        //@}

        //! \name set weight
        //@{
        void setWeight1( float x ) { weight.set(x,0,0,1.0f); format.weight = 1; }
        void setWeight2( float x, float y ) { weight.set(x,y,0,1.0f); format.weight = 2; }
        void setWeight3( float x, float y, float z ) { weight.set(x,y,z,1.0f); format.weight = 3; }
        void setWeight4( float x, float y, float z, float w ) { weight.set(x,y,z,w); format.weight = 4; }
        void setWeight2( const Vector2f & v ) { setWeight2( v.x, v.y ); }
        void setWeight3( const Vector3f & v ) { setWeight3( v.x, v.y, v.z ); }
        void setWeight4( const Vector4f & v ) { setWeight4( v.x, v.y, v.z, v.z ); }
        //@}

        //! \name set normal
        //@{
        void setNormal( float x, float y, float z ) { normal.set( x, y, z ); format.normal = 1; }
        void setNormal( const Vector3f & n ) { normal = n; format.normal = 1; }
        //@}

        //!
        //! set fog
        //!
        void setFog( float f ) { fog = f; format.fog = 1; }

        //! \name set diffuse
        //!@{
        void setDiffuse( float r, float g, float b, float a = 1.0f ) { diffuse.set(r,g,b,a); format.diffuse = 1; }
        void setDiffuse( const Vector3f & rgb, float a = 1.0f ) { diffuse.set(rgb,a); format.diffuse = 1; }
        void setDiffuse( const Vector4f & rgba ) { diffuse = rgba; format.diffuse = 1; } 
        //@}

        //! \name set specular
        //!@{
        void setSpecular( float r, float g, float b, float a = 1.0f ) { specular.set(r,g,b,a); format.specular = 1; }
        void setSpecular( const Vector3f & rgb, float a = 1.0f ) { specular.set(rgb,a); format.specular = 1; }
        void setSpecular( const Vector4f & rgba ) { specular = rgba; format.specular = 1; } 
        //@}

        //! \name set tangent
        //@{
        void setTangent( float x, float y, float z ) { tangent.set( x, y, z ); format.tangent = 1; }
        void setTangent( const Vector3f & t ) { tangent = t; format.tangent = 1; }
        //@}

        //! \name set binormal
        //@{
        void setBinormal( float x, float y, float z ) { binormal.set( x, y, z ); format.binormal = 1; }
        void setBinormal( const Vector3f & b ) { binormal = b; format.binormal = 1; }
        //@}

        //! \name set texture coordinates
        //@{
        void setTexcoord1( size_t stage, float u )
        {
            GN_ASSERT( stage < 8 );
            if( format.texcoord <= stage ) format.texcoord = (unsigned int)stage + 1;
            size_t shift = stage << 1;
            format.texMasks &= ~(3<<shift);
            texcoord[stage].set( u, 0, 0, 0 );
        }
        void setTexcoord2( size_t stage, float u, float v )
        {
            GN_ASSERT( stage < 8 );
            if( format.texcoord <= stage ) format.texcoord = (unsigned int)stage + 1;
            size_t shift = stage << 1;
            format.texMasks &= ~(3<<shift);
            format.texMasks |= 1<<shift;
            texcoord[stage].set( u, v, 0, 0 );
        }
        void setTexcoord3( size_t stage, float u, float v, float w )
        {
            GN_ASSERT( stage < 8 );
            if( format.texcoord <= stage ) format.texcoord = (unsigned int)stage + 1;
            size_t shift = stage << 1;
            format.texMasks &= ~(3<<shift);
            format.texMasks |= 2<<shift;
            texcoord[stage].set( u, v, w, 0 );
        }
        void setTexcoord4( size_t stage, float u, float v, float w, float q )
        {
            GN_ASSERT( stage < 8 );
            if( format.texcoord <= stage ) format.texcoord = (unsigned int)stage + 1;
            size_t shift = stage << 1;
            format.texMasks |= 3<<shift;
            texcoord[stage].set( u, v, w, q );
        }
        void setTexcoord2( size_t stage, const Vector2f & uv ) { setTexcoord2( stage, uv.x, uv.y ); }
        void setTexcoord3( size_t stage, const Vector3f & uvw ) { setTexcoord3( stage, uvw.x, uvw.y, uvw.z ); }
        void setTexcoord4( size_t stage, const Vector4f & uvwq ) { setTexcoord4( stage, uvwq.x, uvwq.y, uvwq.z, uvwq.w ); }
        //@}
    };

    //!
    //! Universal mesh
    //!
    class FatMesh
    {
    public:

        //! \name ctor and dtor
        //@{
        FatMesh() { mTmpVtx.format.u32 = 0; mHasFaceNormal = true; }
        //@}

        //!
        //! clear to empty
        //!
        void clear() { mVertices.clear(); mFaces.clear(); clearOptimizationData(); }

        //! \name new vertex
        //@{
        void newVertices( const FatVertex * verts, size_t count )
        {
            mVertices.append( verts, count );
            clearOptimizationData();
        }
        void newVertex( const FatVertex & v ) { mVertices.append( v ); }
        void pos1( float x ) { mTmpVtx.setPos1( x ); newVertex( mTmpVtx ); mTmpVtx.clear(); }
        void pos2( float x, float y ) { mTmpVtx.setPos2( x, y ); newVertex( mTmpVtx ); mTmpVtx.clear(); }
        void pos3( float x, float y, float z ) { mTmpVtx.setPos3( x, y, z ); newVertex( mTmpVtx ); mTmpVtx.clear(); }
        void pos4( float x, float y, float z, float w ) { mTmpVtx.setPos4( x, y, z, w ); newVertex( mTmpVtx ); mTmpVtx.clear(); }
        void pos2( const Vector2f & v ) { pos2( v ); }
        void pos3( const Vector3f & v ) { pos3( v ); }
        void pos4( const Vector4f & v ) { pos4( v ); }
        void weight1( float x ) { mTmpVtx.setWeight1( x ); }
        void weight2( float x, float y ) { mTmpVtx.setWeight2( x, y ); }
        void weight3( float x, float y, float z ) { mTmpVtx.setWeight3( x, y, z ); }
        void weight4( float x, float y, float z, float w ) { mTmpVtx.setWeight4( x, y, z, w ); }
        void weight2( const Vector2f & v ) { mTmpVtx.setWeight2( v ); }
        void weight3( const Vector3f & v ) { mTmpVtx.setWeight3( v ); }
        void weight4( const Vector4f & v ) { mTmpVtx.setWeight4( v ); }
        void normal( float x, float y, float z ) { mTmpVtx.setNormal( x, y, z ); }
        void normal( const Vector3f & n ) { mTmpVtx.setNormal( n.x, n.y, n.z ); }
        void diffuse( float r, float g, float b, float a = 1.0f ) { mTmpVtx.setDiffuse( r, g, b, a ); }
        void diffuse( const Vector3f & rgb, float a = 1.0f ) { mTmpVtx.setDiffuse( rgb, a ); }
        void diffuse( const Vector4f & rgba ) { mTmpVtx.setDiffuse( rgba ); }
        void specular( float r, float g, float b, float a = 1.0f ) { mTmpVtx.setSpecular( r, g, b, a ); }
        void specular( const Vector3f & rgb, float a = 1.0f ) { mTmpVtx.setSpecular( rgb, a ); }
        void specular( const Vector4f & rgba ) { mTmpVtx.setSpecular( rgba ); }
        void tangent( float x, float y, float z ) { mTmpVtx.setTangent( x, y, z ); }
        void tangent( const Vector3f & t ) { mTmpVtx.setTangent( t ); }
        void binormal( float x, float y, float z ) { mTmpVtx.setBinormal( x, y, z ); }
        void binormal( const Vector3f & b ) { mTmpVtx.setBinormal( b ); }
        void texcoord( size_t stage, float u ) { mTmpVtx.setTexcoord1( stage, u ); }
        void texcoord( size_t stage, float u, float v ) { mTmpVtx.setTexcoord2( stage, u, v ); }
        void texcoord( size_t stage, float u, float v, float w ) { mTmpVtx.setTexcoord3( stage, u, v, w ); }
        void texcoord( size_t stage, float u, float v, float w, float q ) { mTmpVtx.setTexcoord4( stage, u, v, w, q ); }
        void texcoord( size_t stage, const Vector2f & uv ) { mTmpVtx.setTexcoord2( stage, uv ); }
        void texcoord( size_t stage, const Vector3f & uvw ) { mTmpVtx.setTexcoord3( stage, uvw ); }
        void texcoord( size_t stage, const Vector4f & uvwq ) { mTmpVtx.setTexcoord4( stage, uvwq ); }
        //@}

        //! \name new face
        //@{
        void newFace( size_t i0, size_t i1, size_t i2, int material = 0 )
        {
            Face f;
            f.i0 = i0; f.i1 = i1; f.i2 = i2;
            f.material = material;
            mFaces.append( f );
            mHasFaceNormal = false;
            clearOptimizationData();
        }
        void newFace( size_t i0, size_t i1, size_t i2, float nx, float ny, float nz, int material = 0 )
        {
            Face f;
            f.i0 = i0; f.i1 = i1; f.i2 = i2;
            f.normal.set( nx, ny, nz );
            f.material = material;
            mFaces.append( f );
            clearOptimizationData();
        }
        void newFace( size_t i0, size_t i1, size_t i2, const Vector3f & normal, int material = 0 )
        {
            Face f;
            f.i0 = i0; f.i1 = i1; f.i2 = i2;
            f.normal = normal;
            f.material = material;
            mFaces.append( f );
            clearOptimizationData();
        }
        template< typename INDEX_TYPE >
        void newFaces( const INDEX_TYPE * indices, size_t faceCount, int material = 0 )
        {
            Face f;
            f.material = material;
            if( 0 == indices ) faceCount = 0;
            for( size_t i = 0; i < faceCount; ++i, indices += 3 )
            {
                // make sure indices are positive.
                GN_ASSERT( indices[0] >= 0 && indices[1] >= 0 && indices[2] >= 0 );

                f.i0 = (size_t)indices[0];
                f.i1 = (size_t)indices[1];
                f.i2 = (size_t)indices[2];

                mFaces.append( f );
            }
            mHasFaceNormal = false;
            clearOptimizationData();
        }
        //@}

        //! \name optimization
        //@{

        //!
        //! optimization options
        //!
        struct OptimizeOptions
        {
            //! \name required fields
            //@{
            size_t maxPrimitivesInSingleDraw; //!< as is
            bool vcache; //!< optimize for vcache.
            bool strip;  //!< generate triangle strips.
            bool useResetIndex; //!< use reset index to connect triangle strips (for Xenon)
            bool use32BitIndex; //!< use 32-bit indices.
            //@}

            //! \name optional fields
            //!@{
            size_t   vcacheSize; //!< size of vcache. Ignored, if vcache is false.
            uint32_t resetIndex; //!< reset index. Ignored, if useResetIndex is false.
            //!@}
        };

        //!
        //! In-place optimization.
        //!
        void optimize( const OptimizeOptions & );

        //@}

        //! \name rendering
        //@{
        void draw( int material ); //!< note, this function is very inefficient. Do not use this in performance critical code.
        //@}

        //! \name serialize
        //@{
        bool readFrom( File & );
        bool writeTo( File &, char mode ); //!< mode must be 'B' (binary) or 'T' (text)
        //@}

    private:

        // Triangle face
        struct Face
        {
            size_t i0, i1, i2; // vertice indices
            Vector3f normal;   // face normal
            int material;      // material ID.
        };

        struct FaceSegment
        {
            int                 material;  // material ID
            size_t              vtxSegIdx; // index of vertex segment that this face segment belongs to.
            DynaArray<uint32_t> indices32; // indices are based on start vertex of owner vertex segment.
            DynaArray<uint16_t> indices16;
        };

        struct VtxSegment
        {
            size_t start; // index of first vertex in mVertices.
            size_t count; // count of vertices in this segments.
        };

        // raw mesh data
        DynaArray<FatVertex> mVertices;
        DynaArray<Face>      mFaces;
        bool                 mHasFaceNormal; //!< True means all faces have normal.

        // optimized mesh data
        DynaArray<VtxSegment>  mVtxSegments; // optimized vertex segments.
        DynaArray<FaceSegment> mFaceSegments; // optimized face segments.
        bool                   mUse32BitIndex;
        bool                   mUseTriStrip;

        // misc.
        FatVertex  mTmpVtx;

    private:

        void clearOptimizationData()
        {
            mVtxSegments.clear();
            mFaceSegments.clear();
        }

        // Sort faces in specific vertex segment, by material ID. Store result into face segment array.
        void sortByMaterial( size_t vtxSegIdx, const DynaArray<size_t> & faces );

        inline void drawFaceSegment( size_t idx );
    };
}}

// *****************************************************************************
//                           End of fatMesh.h
// *****************************************************************************
#endif // __GN_GFX_FATMESH_H__
