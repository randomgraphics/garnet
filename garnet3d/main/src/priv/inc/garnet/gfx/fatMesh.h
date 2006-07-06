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
    //! Universal vertex format
    //!
    union FatVertexFormat
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
            unsigned int specular :  1; //!< has specular
            unsigned int fog      :  1; //!< has fog
            unsigned int tangent  :  1; //!< has tangent
            unsigned int binormal :  1; //!< has binormal
            unsigned int texcoord :  4; //!< number of texture coordinates, 0-8
            // byte 2,3
            unsigned int texMasks : 16; //!< channel masks, 2 bits for one texture.
                                        //!< 00 - 1 channel, 01 - 2 channels, 02 - 3 channels, 03 - 4 channels
        };

        //!
        //! make sure vertex format is valid.
        //!
        bool valid() const
        {
            return pos <= 4 && weight <= 4 && texcoord <= 8;
        }

        //!
        //! equality
        //!
        bool operator == ( const FatVertexFormat & rhs ) const { return u32 == rhs.u32; }

        //!
        //! equality
        //!
        bool operator != ( const FatVertexFormat & rhs ) const { return u32 != rhs.u32; }

        //!
        //! clear vertex format
        //!
        void clear() { GN_CASSERT( 4 == sizeof(FatVertexFormat) ); u32 = 0; }

        //!
        //! parse vertex format from string.
        //!
        bool fromStr( const char * str, size_t len = 0 );

        //!
        //! set texture channel mask
        //!
        void setTexChannelMask( size_t stage, size_t channels )
        {
            if( stage >= 8 ) { GN_ERROR( "stage is out of range [0-7]." ); return; }
            if( channels < 1 || channels > 4 ) { GN_ERROR( "channels is out of range [1-4]." ); return; }
            size_t shift = stage << 1;
            unsigned int bits = (unsigned int)channels - 1;
            texMasks &= ~(3<<shift);
            texMasks |= bits<<shift;
        }

        //!
        //! get channel count of specific texture stage. Return 0 if the stage is not available.
        //!
        unsigned int getTexChannelCount( size_t stage ) const
        {
            if( stage >= texcoord ) return 0;
            return ( ( texMasks >> (stage*2) ) & 3 ) + 1;
        }
    };

    //!
    //! Universal vertex that can hold almost any kind of vertex data
    //!
    struct FatVertex
    {
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
        //! \name set texture coordinates.
        //@{
        void setTexcoord( size_t stage, float s, float t = .0f, float r = .0f, float q = .0f )
        {
            if( stage >= 8 ) { GN_ERROR( "stage is out of range [0-7]." ); return; }
            texcoord[stage].set( s, t, r, q );
        }
        void setTexcoord( size_t stage, const Vector2f & v ) { setTexcoord( stage, v.x, v.y ); }
        void setTexcoord( size_t stage, const Vector3f & v ) { setTexcoord( stage, v.x, v.y, v.z ); }
        void setTexcoord( size_t stage, const Vector4f & v ) { setTexcoord( stage, v.x, v.x, v.y, v.z ); }
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
        FatMesh() { mHasFaceNormal = true; }
        //@}

        //!
        //! clear to empty
        //!
        void clear()
        {
            mVertices.clear();
            mFaces.clear();
            mHasFaceNormal = true;
            clearOptimizationData();
        }

        //! \name set vertices
        //@{
        void setVertexFormat( FatVertexFormat fmt )
        {
            if( !fmt.valid() ) { GN_WARN( "invalid vertex format" ); return; }
            mVertexFormat = fmt;
        }
        void newVertices( const FatVertex * verts, size_t count )
        {
            mVertices.append( verts, count );
            clearOptimizationData();
        }
        void newVertex( const FatVertex & v ) { newVertices( &v, 1 ); }
        void pos( float x, float y = .0f, float z = .0f, float w = 1.0f ) { mTmpVtx.pos.set(x,y,z,w); newVertex( mTmpVtx ); }
        void pos( const Vector2f & v ) { pos( v.x, v.y ); }
        void pos( const Vector3f & v ) { pos( v.x, v.y, v.z ); }
        void pos( const Vector4f & v ) { pos( v.x, v.y, v.z, v.w ); }
        void weight( float x, float y = .0f, float z = .0f, float w = .0f ) { mTmpVtx.weight.set( x, y, z, w ); }
        void weight( const Vector2f & v, float z = .0f, float w = .0f  ) { mTmpVtx.weight.set( v, z, w ); }
        void weight( const Vector3f & v, float w = .0f ) { mTmpVtx.weight.set( v, w ); }
        void weight( const Vector4f & v ) { mTmpVtx.weight = v; }
        void normal( float x, float y, float z ) { mTmpVtx.normal.set( x, y, z ); }
        void normal( const Vector3f & n ) { mTmpVtx.normal = n; }
        void diffuse( float r, float g, float b, float a = 1.0f ) { mTmpVtx.diffuse.set( r, g, b, a ); }
        void diffuse( const Vector3f & rgb, float a = 1.0f ) { mTmpVtx.diffuse.set( rgb, a ); }
        void diffuse( const Vector4f & rgba ) { mTmpVtx.diffuse = rgba; }
        void specular( float r, float g, float b, float a = 1.0f ) { mTmpVtx.specular.set( r, g, b, a ); }
        void specular( const Vector3f & rgb, float a = 1.0f ) { mTmpVtx.specular.set( rgb, a ); }
        void specular( const Vector4f & rgba ) { mTmpVtx.specular = rgba; }
        void tangent( float x, float y, float z ) { mTmpVtx.tangent.set( x, y, z ); }
        void tangent( const Vector3f & t ) { mTmpVtx.tangent = t; }
        void binormal( float x, float y, float z ) { mTmpVtx.binormal.set( x, y, z ); }
        void binormal( const Vector3f & b ) { mTmpVtx.binormal = b; }
        void texcoord( size_t stage, float s, float t = .0f, float r = .0f, float q = .0f ) { mTmpVtx.setTexcoord( stage, s, t, r, q ); }
        void texcoord( size_t stage, const Vector2f & v ) { mTmpVtx.setTexcoord( stage, v ); }
        void texcoord( size_t stage, const Vector3f & v ) { mTmpVtx.setTexcoord( stage, v ); }
        void texcoord( size_t stage, const Vector4f & v ) { mTmpVtx.setTexcoord( stage, v ); }
        //@}

        //! \name set faces
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
        bool readFrom( File & );//!< read from fatmesh file, support both binary and text format.
        bool readFromX( File & ); //!< read from D3D X mesh file.
        //!
        //! write mesh to file. See sample mesh files in tree details of mesh file format.
        //!
        //! \param mode
        //!     must be 'B' (binary) or 'T' (text)
        //!
        bool writeTo( File &, char mode ) const;
        //@}

    private:

        // Triangle face
        struct Face
        {
            size_t i0, i1, i2; // vertice indices
            Vector3f normal;     // face normal
            int material;        // material ID.
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
        FatVertexFormat      mVertexFormat;
        bool                 mHasFaceNormal; //!< True means all faces have normal.

        // optimized mesh data
        DynaArray<VtxSegment>  mVtxSegments; // optimized vertex segments.
        DynaArray<FaceSegment> mFaceSegments; // optimized face segments.
        bool                   mUse32BitIndex;
        bool                   mUseTriStrip;

        // misc.
        FatVertex       mTmpVtx;
        FatVertexFormat mTmpFmt;

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
