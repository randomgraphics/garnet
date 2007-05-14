#ifndef __GN_GFX_SURFACE_H__
#define __GN_GFX_SURFACE_H__
// *****************************************************************************
//! \file    gfx/surface.h
//! \brief   Graphics surface classes, include texture, vtxbuf and idxbuf
//! \author  chen@@CHENLI-HOMEPC (2007.2.23)
// *****************************************************************************

namespace GN { namespace gfx
{
    enum
    {
        MAX_TEXTURE_SIZE          = 16384, ///< max texture size (2^14)
        MAX_TEXTURE_MIPMAP_LEVELS = 15,
    };

    ///
    /// 贴图类型
    ///
    enum TexDim
    {
        TEXDIM_1D,    ///< 1D texture
        TEXDIM_2D,    ///< 2D texture
        TEXDIM_3D,    ///< 3D texture
        TEXDIM_CUBE,  ///< Cube texture
        TEXDIM_STACK, ///< Stack texture
        NUM_TEXDIMS   ///< Number of avaliable texture types.
    };

    ///
    /// Texture usage.
    ///
    enum TextureUsage
    {
        TEXUSAGE_DYNAMIC        = 1<<0, ///< dynamic texture
        TEXUSAGE_AUTOGEN_MIPMAP = 1<<1, ///< texture that can auto-generate mipmaps
        TEXUSAGE_RENDER_TARGET  = 1<<2, ///< render target texture
        TEXUSAGE_DEPTH          = 1<<3, ///< depth texture
        TEXUSAGE_READBACK       = 1<<4, ///< Normally, read data from texture is extremly slow.
                                        ///< Use this flag to increase reading speed, in exchange for rendering speed.
                                        ///< Use this flag when you really need it.
        TEXUSAGE_TILED          = 1<<5, ///< tile texture (ignored on platform other then Xenon)
    };

    ///
    /// 贴图过滤器
    ///
    enum TexFilter
    {
        TEXFILTER_NEAREST,      ///< nearest
        TEXFILTER_LINEAR,       ///< linear
        TEXFILTER_N_MIP_N,      ///< NEAREST_MIPMAP_NEAREST
        TEXFILTER_N_MIP_L,      ///< NEAREST_MIPMAP_LINEAR
        TEXFILTER_L_MIP_N,      ///< LINEAR_MIPMAP_NEAREST
        TEXFILTER_L_MIP_L,      ///< LINEAR_MIPMAP_LINEAR
        NUM_TEXFILTERS
    };

    ///
    /// 贴图Wrap Mode
    ///
    enum TexWrap
    {
        TEXWRAP_REPEAT,
        TEXWRAP_CLAMP,
        TEXWRAP_CLAMP_TO_EDGE,
        NUM_TEXWRAPS
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
        TexDim    dim;       ///< texture dimension
        UInt32    width;     ///< basemap width
        UInt32    height;    ///< basemap height
        UInt32    depth;     ///< basemap depth
        UInt32    faces;     ///< face count. When used as parameter of Renderer::createTexture(),
                             ///< you may set it to 0 to use default face count: 6 for cubemap, 1 for others.
        UInt32    levels;    ///< mipmap level count. When used as parameter of Renderer::createTexture(),
                             ///< you may set it to 0 to create full mipmap tower (down to 1x1).
        ClrFmt    format;    ///< pixel format. When used as parameter of Renderer::createTexture(),
                             ///< you may set it to FMT_DEFAULT. To use default texture format.

        ///
        /// get basemap size
        ///
        const Vector3<UInt32> & size() const { return *(Vector3<UInt32>*)&width; }

        union
        {
            UInt32 u32;     ///< usage bits as unsigned integer
            struct
            {
                unsigned int dynamic      :  1; ///< See TEXUSAGE_DYNAMIC
                unsigned int automip      :  1; ///< See TEXUSAGE_AUTOGEN_MIPMAP
                unsigned int rendertarget :  1; ///< See TEXUSAGE_RENDER_TARGET
                unsigned int depthstencil :  1; ///< See TEXUSAGE_DEPTH
                unsigned int readback     :  1; ///< See TEXUSAGE_READBACK
                unsigned int tiled        :  1; ///< See TEXUSAGE_TILED
                unsigned int _            : 26; ///< reserved
            };
        } usage; ///< texture usage

        //
        // validate texture descriptor
        //
        bool validate()
        {
            static Logger * sLogger = getLogger("GN.gfx.TextureDesc");

            // check dim
            if( dim < 0 || dim >= NUM_TEXDIMS )
            {
                GN_ERROR(sLogger)( "invalid texture dimension!" );
                return false;
            }

            // check texture size
            switch( dim )
            {
                case TEXDIM_1D :
                {
                    height = 1;
                    depth = 1;
                    break;
                }

                case TEXDIM_CUBE :
                {
                    height = width;
                    depth = 1;
                    break;
                }

                case TEXDIM_2D :
                case TEXDIM_STACK :
                {
                    depth = 1;
                    break;
                }

                case TEXDIM_3D :
                {
                    // do nothing
                    break;
                }

                default : GN_UNEXPECTED();
            }

            // check face count
            if( TEXDIM_CUBE == dim )
            {
                if( 0 != faces && 6 != faces )
                {
                    GN_WARN(sLogger)( "Cubemap must have 6 faces." );
                }
                faces = 6;
            }
            else if( TEXDIM_STACK == dim )
            {
                if( 0 == faces ) faces = 1;
            }
            else
            {
                if( 0 != faces && 1 != faces )
                {
                    GN_WARN(sLogger)( "Texture other then cube/stack texture can have only 1 face." );
                }
                faces = 1;
            }

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
            if( ( format < 0 || format >= NUM_CLRFMTS ) &&
                FMT_DEFAULT != format )
            {
                GN_ERROR(sLogger)( "invalid texture format: %s", clrFmt2Str(format) );
                return false;
            }

            // success
            return true;
        }
    };

    ///
    /// vertex buffer descriptor
    ///
    struct VtxBufDesc
    {
        UInt32 bytes;    ///< number of bytes in the buffer
        bool   dynamic;  ///< dynamic buffer.
        bool   readback; ///< can read data back from buffer.
    };
    GN_CASSERT( sizeof(VtxBufDesc) == 8 );

    ///
    /// vertex buffer descriptor
    ///
    struct IdxBufDesc
    {
        UInt32 numidx;   ///< number of indices in index buffer
        bool   dynamic;  ///< dynamic buffer.
        bool   readback; ///< can read data back from buffer.
    };
    GN_CASSERT( sizeof(IdxBufDesc) == 8 );

    ///
    /// texture lock area
    ///
    typedef Box<size_t> TexLockArea;

    ///
    /// 贴图锁定的返回结果
    ///
    struct TexLockedResult
    {
        size_t rowBytes;   ///< bytes per scanline (for DXT texture, it is 1/4 of one texel row)
        size_t sliceBytes; ///< bytes per slice
        void * data;       ///< 指向被锁定图象的第一个字节
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
        /// set texture filters
        ///
        /// \note default is [linear, linear]
        ///
        virtual void setFilter( TexFilter min, TexFilter mag ) const = 0;

        ///
        /// set texture address mode
        ///
        /// \note default is TEXWRAP_REPEAT
        ///
        virtual void setWrap( TexWrap s,
                              TexWrap t = TEXWRAP_REPEAT,
                              TexWrap r = TEXWRAP_REPEAT ) const = 0;

        /// \name lock/unlock functions
        //@{

        ///
        /// Lock specific level of the texture. Can be used to all kind of texture.
        ///
        /// \param result   return locking result
        /// \param face     Specify face you want to lock.
        /// \param level    Specify mipmap level you want to lock.
        /// \param area     Specify locking area in the mipmap. Null means whole level.
        /// \param flag     Locking flag. See LockFlag for details.
        ///
        /// \return         Return false, if locking failed.
        ///
        virtual bool lock(
            TexLockedResult & result,
            size_t face,
            size_t level,
            const TexLockArea * area,
            LockFlag flag ) = 0;

        ///
        /// lock a 1D texture.
        ///
        void * lock1D( size_t level, size_t offset, size_t length, LockFlag flag )
        {
            GN_ASSERT( TEXDIM_1D == getDesc().dim );
            TexLockedResult result;
            TexLockArea area;
            area.x = (int)offset;
            area.y = 0;
            area.z = 0;
            area.w = (int)length;
            area.h = 1;
            area.d = 1;
            if( !lock( result, 0, level, &area, flag ) ) return 0;
            return result.data;
        }

        ///
        /// unlock previous lock
        ///
        virtual void unlock() = 0;

        //@}

        /// \name misc. functions
        //@{

        ///
        /// update contents of sub-mipmap-levels based on level 0
        ///
        virtual void updateMipmap() = 0;

        ///
        /// Get low-level device handle of the texture. LPDIRECT3DBASETEXTURE9 for
        /// DirectX; name of texture object(GLuint) for OpenGL.
        ///
        virtual void * getAPIDependentData() const = 0;

        //@}

        /// \name get reference to shader name.
        ///
        /// Name field is for debug purpose only, it is not used by garnet library. Set it to any value you want.
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

    /// \name convert between texture tags and string
    //@{

    //
    //
    // -------------------------------------------------------------------------
    inline bool
    texDim2Str( StrA & str, TexDim textype )
    {
        static const char * sTable [] =
        { "1D", "2D", "3D", "CUBE", "STACK" };

        if( 0 <= textype && textype < NUM_TEXDIMS )
        {
            str = sTable[textype];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    texDim2Str( TexDim textype )
    {
        static const char * sTable [] =
        { "1D", "2D", "3D", "CUBE", "STACK" };

        if( 0 <= textype && textype < NUM_TEXDIMS )
        {
            return sTable[textype];
        }
        else return "BAD_TEXDIM";
    }
    //
    inline bool
    str2TexDim( TexDim & value, const char * name )
    {
        static const char * sTable[] =
        { "1D", "2D", "3D", "CUBE", "STACK" };

        if( name )
        {
            for( int i = 0; i < NUM_TEXDIMS; ++i )
            {
                if( 0 == ::strcmp(sTable[i],name) )
                {
                    value = static_cast<TexDim>(i);
                    return true;
                }
            }
        }
        return false;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline bool
    texFilter2Str( StrA & str, TexFilter filter )
    {
        static const char * sTable [] =
        {
            "NEAREST", "LINEAR",
            "N_MIP_N", "N_MIP_L",
            "L_MIP_N", "L_MIP_L",
        };

        if( 0 <= filter && filter < NUM_TEXFILTERS )
        {
            str = sTable[filter];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    texFilter2Str( TexFilter filter )
    {
        static const char * sTable [] =
        {
            "NEAREST", "LINEAR",
            "N_MIP_N", "N_MIP_L",
            "L_MIP_N", "L_MIP_L",
        };

        if( 0 <= filter && filter < NUM_TEXFILTERS )
        {
            return sTable[filter];
        }
        else return "BAD_TEXTURE_FILTER";
    }
    //
    inline bool
    str2TexFilter( TexFilter & value, const char * name )
    {
        static const char * sTable [] =
        {
            "NEAREST", "LINEAR",
            "N_MIP_N", "N_MIP_L",
            "L_MIP_N", "L_MIP_L",
        };

        if( name )
        {
            for( size_t i = 0; i < NUM_TEXFILTERS; ++i )
            {
                if( 0 ==::strcmp(sTable[i],name) )
                {
                    value = static_cast<TexFilter>(i);
                    return true;
                }
            }
        }
        return false;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline bool
    texWrap2Str( StrA & str, TexWrap texwrap )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( 0 <= texwrap && texwrap < NUM_TEXWRAPS )
        {
            str = sTable[texwrap];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    texWrap2Str( TexWrap texwrap )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( 0 <= texwrap && texwrap < NUM_TEXWRAPS )
        {
            return sTable[texwrap];
        }
        else return "BAD_TEXTURE_WRAP";
    }
    //
    inline bool
    str2TexWrap( TexWrap & value, const char * name )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( name )
        {
            for( size_t i = 0; i < NUM_TEXWRAPS; ++i )
            {
                if( 0 ==::strcmp(sTable[i],name) )
                {
                    value = static_cast<TexWrap>(i);
                    return true;
                }
            }
        }
        return false;
    }

    //@}

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
        /// Lock vertex buffer
        ///
        /// \param offset
        ///     offset in bytes of lock range.
        /// \param bytes
        ///     bytes of lock range. '0' means to the end of the buffer.
        /// \param flag
        ///     Locking flags, see LockFlag.
        ///     Note that LOCK_RW and LOCK_RO can only be used for buffers with system copy
        /// \return
        ///     Return locked buffer pointer. NULL means failed.
        ///
        virtual void * lock( size_t offset, size_t bytes, LockFlag flag ) = 0;

        ///
        /// Unlock specific stream
        ///
        virtual void unlock() = 0;

    protected:

        ///
        /// Set buffer properties
        ///
        void setDesc( const VtxBufDesc & desc )
        {
            GN_ASSERT( desc.bytes > 0 );
            mDesc = desc;
        }

    private:

        VtxBufDesc mDesc; ///< Buffer descriptor
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
        /// lock the buffer
        ///
        /// \param startidx
        ///     first index of this locking
        /// \param numidx
        ///     index count of this locking, '0' means to the end of the buffer.
        /// \param flag
        ///     Locking flags, see LockFlag.
        ///     Note that LOCK_RW and LOCK_RO can only be used for buffers with system copy
        /// \return
        ///     Return locked buffer pointer. NULL means failed.
        ///
        virtual UInt16 * lock( size_t startidx, size_t numidx, LockFlag flag ) = 0;

        ///
        /// unlock the buffer
        ///
        virtual void unlock() = 0;

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

    ///
    /// Unlock the surface automatically, before going out of life scope.
    ///
    template<class SURFACE>
    class AutoSurfaceUnlocker
    {
        SURFACE * mSurface;

    public:

        ///
        /// Ctor
        ///
        AutoSurfaceUnlocker( SURFACE * surf ) : mSurface(surf) {}

        ///
        /// Dtor
        ///
        ~AutoSurfaceUnlocker() { if(mSurface) mSurface->unlock(); }
    };
}}


// *****************************************************************************
//                           End of surface.h
// *****************************************************************************
#endif // __GN_GFX_SURFACE_H__
