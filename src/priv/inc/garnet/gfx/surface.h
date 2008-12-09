#ifndef __GN_GFX_SURFACE_H__
#define __GN_GFX_SURFACE_H__
// *****************************************************************************
/// \file
/// \brief   Graphics surface classes, include texture, vtxbuf and idxbuf
/// \author  chen@@CHENLI-HOMEPC (2007.2.23)
// *****************************************************************************

namespace GN { namespace gfx
{
    enum
    {
        MAX_TEXTURE_SIZE          = 16384, ///< max texture size (2^14)
        MAX_TEXTURE_MIPMAP_LEVELS = 15,
        MAX_VERTEX_ELEMENTS       = 32,    ///< max number of vertex elements per vertex buffer
    };

    ///
    /// Texture usage.
    ///
    enum TextureUsage
    {
        TEXUSAGE_DYNAMIC        = 1<<0, ///< dynamic texture
        TEXUSAGE_READBACK       = 1<<1, ///< Normally, read data from texture is extremly slow.
                                        ///< Use this flag to increase reading speed, in exchange for rendering speed.
                                        ///< Use this flag when you really need it.
        TEXUSAGE_RENDER_TARGET  = 1<<2, ///< render target texture
        TEXUSAGE_DEPTH          = 1<<3, ///< depth texture
        TEXUSAGE_CUBE_MAP       = 1<<4, ///< texture will be used as cube map
    };

    ///
    /// cube texture face
    ///
    enum CubeFace
    {
        CUBEFACE_PX = 0, ///< positive X
        CUBEFACE_NX, ///< negative X
        CUBEFACE_PY, ///< positive Y
        CUBEFACE_NY, ///< negative Y
        CUBEFACE_PZ, ///< positive Z
        CUBEFACE_NZ, ///< negative Z
        NUM_CUBEFACES
    };

    ///
    /// 锁定标志
    ///
    enum LockFlag
    {
        LOCK_RW,           ///< read-write lock, the slowest one.
        LOCK_RO,           ///< read-only lock.
        LOCK_WO,           ///< write-only lock
        LOCK_DISCARD,      ///< write-only lock, discard old value.
        LOCK_NO_OVERWRITE, ///< write-only lock, promise not to modify any section of the buffer being used.
        NUM_LOCK_FLAGS     ///< number of lock flags.
    };

    ///
    /// Texture descriptor
    ///
    struct TextureDesc
    {
        UInt32      width;     ///< basemap width
        UInt32      height;    ///< basemap height
        UInt32      depth;     ///< basemap depth
        UInt32      faces;     ///< face count. When used as parameter of Renderer::createTexture(),
                               ///< you may set it to 0 to use default face count: 6 for cubemap, 1 for others.
        UInt32      levels;    ///< mipmap level count. When used as parameter of Renderer::createTexture(),
                               ///< you may set it to 0 to create full mipmap tower (down to 1x1).
        ColorFormat format;    ///< pixel format.

        ///
        /// get basemap size
        ///
        const Vector3<UInt32> & size() const { return *(Vector3<UInt32>*)&width; }

        union
        {
            UInt32 u32;     ///< usage bits as unsigned integer
            struct
            {
                unsigned int rendertarget :  1; ///< See TEXUSAGE_RENDER_TARGET
                unsigned int depthstencil :  1; ///< See TEXUSAGE_DEPTH
                unsigned int dynamic      :  1; ///< See TEXUSAGE_DYNAMIC
                unsigned int readback     :  1; ///< See TEXUSAGE_READBACK
                unsigned int _            : 27; ///< reserved
            };
        } usage; ///< texture usage

        ///
        /// compose texture descriptor from image descriptor
        ///
        bool fromImageDesc( const ImageDesc & id )
        {
            // get image size
            UInt32 w = id.mipmaps[0].width;
            UInt32 h = id.mipmaps[0].height;
            UInt32 d = id.mipmaps[0].depth;

            width     = w;
            height    = h;
            depth     = d;
            faces     = id.numFaces;
            levels    = id.numLevels;
            format    = id.format;
            usage.u32 = 0;

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

            maxLevels = max( max(nx, ny), nz );

            levels = ( 0 == levels ) ? maxLevels : min( maxLevels, levels );

            // check format
            if( format < 0 || format >= NUM_CLRFMTS )
            {
                static Logger * sLogger = getLogger("GN.gfx.TextureDesc");
                GN_ERROR(sLogger)( "invalid texture format: %s", clrFmt2Str(format) );
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
    /// 贴图对象
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
        /// update texture content
        ///
        /// \param face, level      Subsurface location
        /// \param area             Subsurface area that is going to be updated. Set to NULL to update whole subsurface.
        /// \param rowPitch         Row pitch in bytes of the data
        /// \param slicePitch       Slice pitch in bytes of the data
        /// \param data             The data buffer that holds data that are going to be copied to texture.
        ///
        virtual void update(
            size_t              face,
            size_t              level,
            const TexLockArea * area,
            size_t              rowPitch,
            size_t              slicePitch,
            const void        * data ) = 0;

        ///
        /// read texture content. Texture must have TEXUSAGE_READBACK usage.
        ///
        virtual void readback( size_t face, size_t level, MipmapData & data ) = 0;

        ///
        /// generate content of all mipmap levels based on content in base level
        ///
        virtual void generateMipmap() = 0;

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
        void setMipSize( size_t level, const Vector3<UInt32> & s )
        {
            setMipSize( level, s.x, s.y, s.z );
        }

        ///
        /// setup mip size
        ///
        template<typename T>
        void setMipSize( size_t level, T sx, T sy, T sz )
        {
            GN_ASSERT( level < mDesc.levels );
            GN_ASSERT( level > 0 ||
                sx == (T)mDesc.width &&
                sy == (T)mDesc.height &&
                sz == (T)mDesc.depth );
            mMipSize[level].set( (UInt32)sx, (UInt32)sy, (UInt32)sz );
        }

    private :
        TextureDesc                    mDesc;    ///< descriptor
        std::vector< Vector3<UInt32> > mMipSize; ///< mipmap size of each level
        StrA                           mName;    ///< texture name. Only for debug purpose.
    };

    ///
    /// vertex format descriptor
    ///
    struct VertexFormat
    {
        ColorFormat elementFormats[MAX_VERTEX_ELEMENTS];
        UInt16      elementOffsets[MAX_VERTEX_ELEMENTS];
        UInt16      numElements;
    };

    ///
    /// vertex buffer descriptor
    ///
    struct VtxBufDesc
    {
        VertexFormat format;   ///< vertex format
        UInt32       stride;   ///< vertex stride
        UInt32       numvtx;   ///< number of vertices in the buffer
        bool         dynamic;  ///< dynamic buffer.
        bool         readback; ///< can read data back from buffer.
    };
    GN_CASSERT( sizeof(VtxBufDesc) == 8 );

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
        virtual void update( size_t startvtx, size_t numvtx, const void * data ) = 0;

        ///
        /// Read buffer content. The buffer must have readback flag.
        ///
        virtual void readback( std::vector<UInt8> & data ) = 0;

    protected:

        ///
        /// Set buffer properties
        ///
        void setDesc( const VtxBufDesc & desc )
        {
            GN_ASSERT( desc.numvtx > 0 );
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
        UInt32 numidx;   ///< number of indices in index buffer
        bool   bits32;   ///< is 32-bit index buffer or not.
        bool   dynamic;  ///< dynamic buffer.
        bool   readback; ///< can read data back from buffer.
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
        virtual void update( size_t startidx, size_t numidx, const void * data ) = 0;

        ///
        /// Read buffer content. The buffer must have readback flag.
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
