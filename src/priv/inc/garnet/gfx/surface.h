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
    /// texture usage flags
    ///
    union TextureUsages
    {
        UInt32 u32;     ///< usage bits as unsigned integer

        struct
        {
            unsigned int rendertarget :  1; ///< use as color render target
            unsigned int depth        :  1; ///< use as depth buffer
            unsigned int fastCpuWrite :  1; ///< Allow fast CPU write, in exchange of GPU rendering speed.
                                            ///< Note that only textures with this usage off, can be used as render target or depth buffer.
            unsigned int nouse        : 29; ///< no use. must be zero.
        };

        /// default usage
        static TextureUsages DEFAULT() { TextureUsages u; u.u32 = 0; return u; };
    };
    GN_CASSERT( sizeof(TextureUsages) == sizeof(UInt32) );

    ///
    /// Texture descriptor
    ///
    struct TextureDesc
    {
        UInt32        width;   ///< basemap width
        UInt32        height;  ///< basemap height
        UInt32        depth;   ///< basemap depth
        UInt32        faces;   ///< face count. When used as parameter of Renderer::createTexture(),
                               ///< you may set it to 0 to use default face count: 6 for cubemap, 1 for others.
        UInt32        levels;  ///< mipmap level count. When used as parameter of Renderer::createTexture(),
                               ///< you may set it to 0 to create full mipmap chain (down to 1x1).
        ColorFormat   format;  ///< pixel format.
        TextureUsages usages;  ///< texture usages

        ///
        /// get basemap size
        ///
        const Vector3<UInt32> & size() const { return *(Vector3<UInt32>*)&width; }

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
            usages.u32 = 0;
            return validate();
        }


        ///
        /// validate texture descriptor
        ///
        bool validate()
        {
            // calculate maximum mipmap levels
            UInt32 nx = 0, ny = 0, nz = 0;
            UInt32 maxLevels;

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
                static Logger * sLogger = getLogger("GN.gfx.TextureDesc");
                GN_ERROR(sLogger)( "invalid texture format: %s", format.toString().cptr() );
                return false;
            }

            // success
            return true;
        }
    };

    struct MipmapData
    {
        size_t             rowPitch;
        size_t             slicePitch;
        std::vector<UInt8> data;
    };

    ///
    /// cube texture face
    ///
    enum CubeFace
    {
        CUBEFACE_PX = 0, ///< positive X
        CUBEFACE_NX,     ///< negative X
        CUBEFACE_PY,     ///< positive Y
        CUBEFACE_NY,     ///< negative Y
        CUBEFACE_PZ,     ///< positive Z
        CUBEFACE_NZ,     ///< negative Z
        NUM_CUBEFACES
    };

    ///
    /// Update flags
    ///
    enum SurfaceUpdateFlag
    {
        SURFACE_UPDATE_DEFAULT,      ///< default update flag
        SURFACE_UPDATE_DISCARD,      ///< discard old content of whole surface.
        SURFACE_UPDATE_NO_OVERWRITE, ///< promise not to modify any section of the surface that is being used by GPU.
        NUM_SURFACE_UPDATE_FLAGS     ///< number of update flags.
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
        const Vector3<UInt32> & getBaseSize() const { return mDesc.size(); }

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
        const Vector3<UInt32> & getMipSize( size_t level ) const { GN_ASSERT( level < mDesc.levels ); return mMipSize[level]; }

        ///
        /// get size of specific mip level
        ///
        template<typename T>
        void getMipSize( size_t level, T * sx, T * sy = 0, T * sz = 0 ) const
        {
            const Vector3<UInt32> & mipSize = getMipSize( level );
            if( sx ) *sx = (T)mipSize.x;
            if( sy ) *sy = (T)mipSize.y;
            if( sz ) *sz = (T)mipSize.z;
        }

        ///
        /// update mipmap content
        ///
        /// \param face, level      Subsurface location
        /// \param area             Subsurface area that is going to be updated. Set to NULL to update whole subsurface.
        /// \param rowPitch         Row pitch in bytes of the data
        /// \param slicePitch       Slice pitch in bytes of the data
        /// \param data             The data buffer that holds data that are going to be copied to texture.
        ///                         The data must be the same format as the texture.
        ///
        virtual void updateMipmap(
            size_t              face,
            size_t              level,
            const Box<UInt32> * area,
            size_t              rowPitch,
            size_t              slicePitch,
            const void        * data,
            SurfaceUpdateFlag   flag = SURFACE_UPDATE_DEFAULT ) = 0;

        ///
        /// read mipmap content.
        ///
        virtual void readMipmap( size_t face, size_t level, MipmapData & data ) = 0;

        /// read/write the whole texture as a BLOB.
        //@{
        virtual void   blobWrite( const void * data, size_t length ) = 0;
        virtual size_t blobRead( void * data ) = 0;
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
        void setMipSize( size_t level, T sx, T sy, T sz )
        {
            GN_ASSERT( level < mDesc.levels );
            GN_ASSERT( level > 0 ||
                ( sx == (T)mDesc.width &&
                  sy == (T)mDesc.height &&
                  sz == (T)mDesc.depth ) );
            mMipSize[level].set( (UInt32)sx, (UInt32)sy, (UInt32)sz );
        }

        ///
        /// setup mip size
        ///
        void setMipSize( size_t level, const Vector3<UInt32> & s )
        {
            setMipSize( level, s.x, s.y, s.z );
        }

    private :
        TextureDesc                    mDesc;    ///< descriptor
        std::vector< Vector3<UInt32> > mMipSize; ///< mipmap size of each level
        StrA                           mName;    ///< texture name. Only for debug purpose.
    };

    ///
    /// vertex buffer descriptor
    ///
    struct VtxBufDesc
    {
        UInt32 length;        ///< length in bytes of the vertex buffer
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
        /// update vertex buffer content
        ///
        virtual void update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag = SURFACE_UPDATE_DEFAULT ) = 0;

        ///
        /// Read buffer content.
        ///
        virtual void readback( std::vector<UInt8> & data ) = 0;

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
        UInt32 numidx;        ///< number of indices in index buffer
        bool   bits32;        ///< is 32-bit index buffer or not.
        bool   fastCpuWrite;  ///< support fast CPU write (rendering speed compromised)
    };
    GN_CASSERT( sizeof(IdxBufDesc) == 8 );

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
        virtual void update( size_t startidx, size_t numidx, const void * data, SurfaceUpdateFlag flag = SURFACE_UPDATE_DEFAULT ) = 0;

        ///
        /// Read buffer content.
        ///
        virtual void readback( std::vector<UInt8> & data ) = 0;

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
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SURFACE_H__
