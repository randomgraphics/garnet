#ifndef __GN_GFX_SURFACE_H__
#define __GN_GFX_SURFACE_H__
// *****************************************************************************
/// \file
/// \brief   Graphics surface classes, include texture, vtxbuf and idxbuf
/// \author  chen@@CHENLI-HOMEPC (2007.2.23)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// texture usage enumeration
    ///
    struct TextureUsage
    {
        enum Enum
        {
            DEFAULT = 0,         ///< Default usage.
            COLOR_RENDER_TARGET, ///< use as color render target
            DEPTH_RENDER_TARGET, ///< use as depth buffer
            FAST_CPU_WRITE,      ///< Allow fast CPU write, in exchange of GPU rendering speed
            NUM_USAGES,          ///< number of usage flags
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( TextureUsage, Enum );
    };
    GN_CASSERT( sizeof(TextureUsage) == sizeof(uint32) );

    ///
    /// Texture descriptor
    ///
    struct TextureDesc
    {
        uint32        width;   ///< basemap width
        uint32        height;  ///< basemap height
        uint32        depth;   ///< basemap depth
        uint32        faces;   ///< face count. When used as parameter of Gpu::createTexture(),
                               ///< you may set it to 0 to use default face count: 6 for cubemap, 1 for others.
        uint32        levels;  ///< mipmap level count. When used as parameter of Gpu::createTexture(),
                               ///< you may set it to 0 to create full mipmap chain (down to 1x1).
        ColorFormat   format;  ///< pixel format.
        TextureUsage  usage;   ///< texture usage

        ///
        /// get basemap size
        ///
        const Vector3<uint32> & size() const { return *(Vector3<uint32>*)&width; }

        ///
        /// compose texture descriptor from image descriptor
        ///
        bool fromImageDesc( const ImageDesc & id )
        {
            width      = id.mipmaps[0].width;
            height     = id.mipmaps[0].height;
            depth      = id.mipmaps[0].depth;
            faces      = id.numFaces;
            levels     = id.numLevels;
            format     = id.format;
            usage      = TextureUsage::DEFAULT;
            return validate();
        }

        ///
        /// validate texture descriptor
        ///
        bool validate()
        {
            // calculate maximum mipmap levels
            uint32 nx = 0, ny = 0, nz = 0;
            uint32 maxLevels;

            maxLevels = width;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++nx; }

            maxLevels = height;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++ny; }

            maxLevels = depth;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++nz; }

            maxLevels = math::getmax( nx, ny, nz );

            levels = ( 0 == levels ) ? maxLevels : math::getmin( maxLevels, levels );

            // check format
            if( !format.valid() )
            {
                static Logger * sLogger = GetLogger("GN.gfx.TextureDesc");
                GN_ERROR(sLogger)( "invalid texture format: %s", format.toString().rawptr() );
                return false;
            }

            // check usage
            if( usage < 0 && usage >= TextureUsage::NUM_USAGES )
            {
                static Logger * sLogger = GetLogger("GN.gfx.TextureDesc");
                GN_ERROR(sLogger)( "invalid texture usage: %d", usage.toRawEnum() );
                return false;
            }

            // success
            return true;
        }
    };

    struct MipmapData
    {
        uint32             rowPitch;
        uint32             slicePitch;
        DynaArray<uint8> data;
    };

    ///
    /// cube texture face
    ///
    struct CubeFace
    {
        enum Enum
        {
            PX = 0,///< positive X
            NX,    ///< negative X
            PY,    ///< positive Y
            NY,    ///< negative Y
            PZ,    ///< positive Z
            NZ,    ///< negative Z
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( CubeFace, Enum );
    };

    ///
    /// Update flags
    ///
    struct SurfaceUpdateFlag
    {
        enum Enum
        {
            DEFAULT,      ///< default update flag
            DISCARD,      ///< discard old content of whole surface.
            NO_OVERWRITE, ///< promise not to modify any section of the surface that is being used by GPU.
            NUM_FLAGS     ///< number of update flags.
        };

        /// convert enumeration to string
        const char * toString() const
        {
            static const char * TABLE[]=
            {
                "DEFAULT",
                "DISCARD",
                "NO_OVERWRITE",
            };

            const Enum & e = *this;
            if( 0 <= e && e <= NUM_FLAGS ) return TABLE[e];
            else return "INVALID_SURFACE_UPDATE_FLAG";
        }
        GN_DEFINE_ENUM_CLASS_HELPERS( SurfaceUpdateFlag, Enum );
    };

    ///
    /// ÌùÍ¼¶ÔÏó
    ///
    struct Texture : public RefCounter
    {
        ///
        /// Get texture descriptor
        ///
        const TextureDesc & getDesc() const { return mDesc; }

        ///
        /// get size of base map
        ///
        const Vector3<uint32> & getBaseSize() const { return mDesc.size(); }

        ///
        /// get size of base map
        ///
        template<typename T>
        void getBaseSize( T * sx, T * sy = 0, T * sz = 0 ) const
        {
            if( sx ) *sx = (T)mDesc.width;
            if( sy ) *sy = (T)mDesc.height;
            if( sz ) *sz = (T)mDesc.depth;
        }

        ///
        /// get size of specific mip level
        ///
        const Vector3<uint32> & getMipSize( uint32 level ) const { GN_ASSERT( level < mDesc.levels ); return mMipSize[level]; }

        ///
        /// get size of specific mip level
        ///
        template<typename T>
        void getMipSize( uint32 level, T * sx, T * sy = 0, T * sz = 0 ) const
        {
            const Vector3<uint32> & mipSize = getMipSize( level );
            if( sx ) *sx = (T)mipSize.x;
            if( sy ) *sy = (T)mipSize.y;
            if( sz ) *sz = (T)mipSize.z;
        }

        ///
        /// update mipmap content
        ///
        /// \param face, level      Subsurface location
        /// \param area             Subsurface area that is going to be updated. Set to NULL to update whole subsurface.
        /// \param rowPitch         Row pitch in bytes of the data (single texel row, not single texel block row)
        /// \param slicePitch       Slice pitch in bytes of the data
        /// \param data             The data buffer that holds data that are going to be copied to texture.
        ///                         The data must be the same format as the texture.
        /// \param flag             Updte flag. Must be one of SurfaceUpdateFlag.
        ///
        virtual void updateMipmap(
            uint32              face,
            uint32              level,
            const Box<uint32> * area,
            uint32              rowPitch,
            uint32              slicePitch,
            const void        * data,
            SurfaceUpdateFlag   flag = SurfaceUpdateFlag::DEFAULT ) = 0;

        ///
        /// read mipmap content.
        ///
        virtual void readMipmap( uint32 face, uint32 level, MipmapData & data ) = 0;

        /// read/write the whole texture as a BLOB.
        //@{
        virtual void   blobWrite( const void * data, uint32 length ) = 0;
        virtual uint32 blobRead( void * data ) = 0;
        //@}

        ///
        /// generate content of all mipmap levels based on content in base level
        ///
        virtual void generateMipmapPyramid() = 0;

        ///
        /// Get low-level device handle of the texture. LPDIRECT3DBASETEXTURE9 for
        /// DirectX; name of texture object(GLuint) for OpenGL.
        ///
        virtual void * getAPIDependentData() const = 0;

        /// \name get reference to texture name.
        ///
        /// Name field is for debug purpose only, it is not used by garnet library except logging.
        /// Set it to any value you want.
        //@{
        const StrA & name() const { return mName; }
        StrA & name() { return mName; }
        //@}

    protected :

        ///
        /// Set texture descriptor. Subclass must call this function to set
        /// all texture properities to valid value.
        ///
        bool setDesc( const TextureDesc & desc )
        {
            mDesc = desc;

            if( !mDesc.validate() ) return false;

            // allocate mipmap size array
            mMipSize.resize( mDesc.levels );

            return true;
        }

        ///
        /// setup mip size
        ///
        template<typename T>
        void setMipSize( uint32 level, T sx, T sy, T sz )
        {
            GN_ASSERT( level < mDesc.levels );
            GN_ASSERT( level > 0 ||
                ( sx == (T)mDesc.width &&
                  sy == (T)mDesc.height &&
                  sz == (T)mDesc.depth ) );
            mMipSize[level].set( (uint32)sx, (uint32)sy, (uint32)sz );
        }

        ///
        /// setup mip size
        ///
        void setMipSize( uint32 level, const Vector3<uint32> & s )
        {
            setMipSize( level, s.x, s.y, s.z );
        }

    private :
        TextureDesc                    mDesc;    ///< descriptor
        DynaArray< Vector3<uint32> > mMipSize; ///< mipmap size of each level
        StrA                           mName;    ///< texture name. Only for debug purpose.
    };

    ///
    /// vertex buffer descriptor
    ///
    struct VtxBufDesc
    {
        uint32 length;        ///< length in bytes of the vertex buffer
        bool   fastCpuWrite;  ///< support fast CPU write (rendering speed compromised)
    };

    ///
    /// Vertex buffer interface.
    ///
    struct VtxBuf : public RefCounter
    {
        ///
        /// get vertex buffer descriptor
        ///
        const VtxBufDesc & getDesc() const { return mDesc; }

        ///
        /// update vertex buffer content. offset and length are in number of bytes.
        ///
        virtual void update( uint32 offset, uint32 length, const void * data, SurfaceUpdateFlag flag = SurfaceUpdateFlag::DEFAULT ) = 0;

        ///
        /// Read buffer content.
        ///
        virtual void readback( DynaArray<uint8> & data ) = 0;

    protected:

        ///
        /// Set buffer properties
        ///
        void setDesc( const VtxBufDesc & desc )
        {
            GN_ASSERT( desc.length > 0 );
            mDesc = desc;
        }

    private:

        VtxBufDesc mDesc; ///< Buffer descriptor
    };

    ///
    /// index buffer descriptor
    ///
    struct IdxBufDesc
    {
        uint32 numidx;        ///< number of indices in index buffer
        bool   bits32;        ///< is 32-bit index buffer or not.
        bool   fastCpuWrite;  ///< support fast CPU write (rendering speed compromised)
    };

    ///
    /// Index Buffer
    ///
    struct IdxBuf : public RefCounter
    {
        ///
        /// Get descriptor
        ///
        const IdxBufDesc & getDesc() const { return mDesc; }

        ///
        /// update index buffer content
        ///
        virtual void update( uint32 startidx, uint32 numidx, const void * data, SurfaceUpdateFlag flag = SurfaceUpdateFlag::DEFAULT ) = 0;

        ///
        /// Read buffer content.
        ///
        virtual void readback( DynaArray<uint8> & data ) = 0;

    protected:

        ///
        /// Set buffer properties
        ///
        void setDesc( const IdxBufDesc & desc )
        {
            GN_ASSERT( desc.numidx > 0 );
            mDesc = desc;
        }

    private:

        IdxBufDesc mDesc;
    };

    struct BufferUsage
    {
        enum Enum
        {
            UNIFORM,        ///< uniform buffer
            VERTEX,         ///< vertex buffer
            INDEX,          ///< index buffer
            FAST_CPU_WRITE, ///< Allow fast CPU write, in exchange of GPU rendering speed
            NUM_USAGES,     ///< number of usage flags
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( BufferUsage, Enum );
    };
    GN_CASSERT( sizeof(TextureUsage) == sizeof(uint32) );

    struct BufferDesc
    {
        uint32 sizeInBytes;
        uint32 usages; // combination of BufferUsage flags.
    };

    struct Buffer : public RefCounter
    {
        ///
        /// Get descriptor
        ///
        const BufferDesc & getDesc() const { return mDesc; }

        ///
        /// update index buffer content
        ///
        virtual void update( uint32 startidx, uint32 numidx, const void * data) = 0;

        ///
        /// Read buffer content.
        ///
        virtual void readback( DynaArray<uint8> & data ) = 0;

    protected:

        ///
        /// Set buffer properties
        ///
        void setDesc( const BufferDesc & desc )
        {
            mDesc = desc;
        }

    private:

        BufferDesc mDesc;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SURFACE_H__
