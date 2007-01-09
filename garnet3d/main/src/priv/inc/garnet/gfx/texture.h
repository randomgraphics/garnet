#ifndef __GN_GFX_TEXTURE_H__
#define __GN_GFX_TEXTURE_H__
// *****************************************************************************
//! \file    texture.h
//! \brief   Texture interface
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! 贴图类型
    //!
    enum TexType
    {
        TEXTYPE_1D,    //!< 1D texture
        TEXTYPE_2D,    //!< 2D texture
        TEXTYPE_3D,    //!< 3D texture
        TEXTYPE_CUBE,  //!< Cube texture
        TEXTYPE_STACK, //!< Stack texture
        NUM_TEXTYPES   //!< Number of avaliable texture types.
    };

    //!
    //! Texture usage.
    //!
    enum TextureUsage
    {
        TEXUSAGE_DYNAMIC        = 1<<0, //!< dynamic texture
        TEXUSAGE_AUTOGEN_MIPMAP = 1<<1, //!< texture that can auto-generate mipmaps
        TEXUSAGE_RENDER_TARGET  = 1<<2, //!< render target texture
        TEXUSAGE_DEPTH          = 1<<3, //!< depth texture
        TEXUSAGE_READBACK       = 1<<4, //!< Normally, read data from texture is extremly slow.
                                        //!< Use this flag to increase reading speed, in exchange for rendering speed.
                                        //!< Use this flag when you really need it.
    };

    //!
    //! 贴图过滤器
    //!
    enum TexFilter
    {
        TEXFILTER_NEAREST,      //!< nearest
        TEXFILTER_LINEAR,       //!< linear
        TEXFILTER_N_MIP_N,      //!< NEAREST_MIPMAP_NEAREST
        TEXFILTER_N_MIP_L,      //!< NEAREST_MIPMAP_LINEAR
        TEXFILTER_L_MIP_N,      //!< LINEAR_MIPMAP_NEAREST
        TEXFILTER_L_MIP_L,      //!< LINEAR_MIPMAP_LINEAR
        NUM_TEXFILTERS
    };

    //!
    //! 贴图Wrap Mode
    //!
    enum TexWrap
    {
        TEXWRAP_REPEAT,
        TEXWRAP_CLAMP,
        TEXWRAP_CLAMP_TO_EDGE,
        NUM_TEXWRAPS
    };

    //!
    //! cube texture face
    //!
    enum CubeFace
    {
        CUBEFACE_PX = 0, //!< positive X
        CUBEFACE_NX, //!< negative X
        CUBEFACE_PY, //!< positive Y
        CUBEFACE_NY, //!< negative Y
        CUBEFACE_PZ, //!< positive Z
        CUBEFACE_NZ, //!< negative Z
        NUM_CUBEFACES
    };

    struct Texture;

    //!
    //! Texture content loader
    //!
    typedef Delegate1<bool,Texture&> TextureLoader;

    //!
    //! Texture descriptor
    //!
    struct TextureDesc
    {
        TexType   type;      //!< texture type
        UInt32    width;     //!< basemap width
        UInt32    height;    //!< basemap height
        UInt32    depth;     //!< basemap depth
        UInt32    faces;     //!< face count. When used as parameter of Renderer::createTexture(),
                             //!< you may set it to 0 to use default face count: 6 for cubemap, 1 for others.
        UInt32    levels;    //!< mipmap level count. When used as parameter of Renderer::createTexture(),
                             //!< you may set it to 0 to create full mipmap tower (down to 1x1).
        ClrFmt    format;    //!< pixel format. When used as parameter of Renderer::createTexture(),
                             //!< you may set it to FMT_DEFAULT. To use default texture format.
        BitFields usage;     //!< usage
        bool      tiled;     //!< tiled format. Ignored on platform other then Xenon. 
    };

    //!
    //! 贴图锁定的返回结果
    //!
    struct TexLockedResult
    {
        size_t rowBytes;   //!< bytes per line
        size_t sliceBytes; //!< bytes per slice
        void * data;       //!< 指向被锁定图象的第一个字节
    };

    //!
    //! 贴图对象
    //!
    struct Texture : public RefCounter
    {
        //!
        //! Get texture descriptor
        //!
        const TextureDesc & getDesc() const { return mDesc; }

        //!
        //! get size of base map
        //!
        const Vector3<UInt32> & getBaseSize() const { return *(const Vector3<UInt32>*)&mDesc.width; }

        //!
        //! get size of base map
        //!
        template<typename T>
        void getBaseSize( T * sx, T * sy = 0, T * sz = 0 ) const
        {
            const Vector3<UInt32> & baseSize = getBaseSize();
            if( sx ) *sx = (T)baseSize.x;
            if( sy ) *sy = (T)baseSize.y;
            if( sz ) *sz = (T)baseSize.z;
        }

        //!
        //! get size of specific mip level
        //!
        const Vector3<UInt32> & getMipSize( size_t level ) const { GN_ASSERT( level < mDesc.levels ); return mMipSize[level]; }

        //!
        //! get size of specific mip level
        //!
        template<typename T>
        void getMipSize( size_t level, T * sx, T * sy = 0, T * sz = 0 ) const
        {
            const Vector3<UInt32> & mipSize = getMipSize( level );
            if( sx ) *sx = (T)mipSize.x;
            if( sy ) *sy = (T)mipSize.y;
            if( sz ) *sz = (T)mipSize.z;
        }

        //!
        //! set texture filters
        //!
        //! \note default is [linear, linear]
        //!
        virtual void setFilter( TexFilter min, TexFilter mag ) const = 0;

        //!
        //! set texture address mode
        //!
        //! \note default is TEXWRAP_REPEAT
        //!
        virtual void setWrap( TexWrap s,
                              TexWrap t = TEXWRAP_REPEAT,
                              TexWrap r = TEXWRAP_REPEAT ) const = 0;

        //!
        //! Set content loader
        //!
        void setLoader( const TextureLoader & loader ) { mLoader = loader; }

        //! \name lock/unlock functions
        //@{

        //!
        //! Lock specific level of the texture. Can be used to all kind of texture.
        //!
        //! \param result   return locking result
        //! \param face     Specify face you want to lock.
        //! \param level    Specify mipmap level you want to lock.
        //! \param area     Specify locking area in the mipmap. Null means whole level.
        //! \param flag     Locking flag. See LockFlag for details.
        //!
        //! \return         Return false, if locking failed.
        //!
        virtual bool lock(
            TexLockedResult & result,
            size_t face,
            size_t level,
            const Boxi * area,
            LockFlag flag ) = 0;

        //!
        //! lock a 1D texture.
        //!
        void * lock1D( size_t level, size_t offset, size_t length, LockFlag flag )
        {
            GN_ASSERT( TEXTYPE_1D == getDesc().type );
            TexLockedResult result;
            Boxi area;
            area.x = (int)offset;
            area.y = 0;
            area.z = 0;
            area.w = (int)length;
            area.h = 0;
            area.d = 0;
            if( !lock( result, 0, level, &area, flag ) ) return 0;
            return result.data;
        }

        //!
        //! unlock previous lock
        //!
        virtual void unlock() = 0;

        //@}

        //! \name misc. functions
        //@{

        //!
        //! update contents of sub-mipmap-levels based on level 0
        //!
        virtual void updateMipmap() = 0;

        //!
        //! Get low-level device handle of the texture. LPDIRECT3DBASETEXTURE9 for
        //! DirectX; name of texture object(GLuint) for OpenGL.
        //!
        virtual void * getAPIDependentData() const = 0;

        //@}

        //! \name get reference to shader name.
        //!
        //! Name field is for debug purpose only, it is not used by garnet library. Set it to any value you want.
        //@{
        const StrA & name() const { return mName; }
        StrA & name() { return mName; }
        //@}

    protected :

        //!
        //! Set texture descriptor. Subclass must call this function to set 
        //! all texture properities to valid value.
        //!
        bool setDesc( const TextureDesc & desc )
        {
            static Logger * sLogger = getLogger("GN.gfx.Texture");
            
            mDesc = desc;

            // check type
            if( mDesc.type < 0 || mDesc.type >= NUM_TEXTYPES )
            {
                GN_ERROR(sLogger)( "invalid texture type!" );
                return false;
            }

            // check texture size
            switch( mDesc.type )
            {
                case TEXTYPE_1D :
                {
                    mDesc.height = 1;
                    mDesc.depth = 1;
                    break;
                }

                case TEXTYPE_CUBE :
                {
                    mDesc.height = mDesc.width;
                    mDesc.depth = 1;
                    break;
                }

                case TEXTYPE_2D :
                case TEXTYPE_STACK :
                {
                    mDesc.depth = 1;
                    break;
                }

                case TEXTYPE_3D :
                {
                    // do nothing
                    break;
                }

                default : GN_UNEXPECTED();
            }

            // check face count
            if( TEXTYPE_CUBE == mDesc.type )
            {
                if( 0 != mDesc.faces && 6 != mDesc.faces )
                {
                    GN_WARN(sLogger)( "Cubemap must have 6 mDesc.faces." );
                }
                mDesc.faces = 6;
            }
            else if( TEXTYPE_STACK == mDesc.type )
            {
                if( 0 == mDesc.faces ) mDesc.faces = 1;
            }
            else
            {
                if( 0 != mDesc.faces && 1 != mDesc.faces )
                {
                    GN_WARN(sLogger)( "Texture other then cube/stack texture can have only 1 face." );
                }
                mDesc.faces = 1;
            }

            // calculate maximum mipmap levels
            UInt32 nx = 0, ny = 0, nz = 0;
            UInt32 maxLevels;

            maxLevels = mDesc.width;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++nx; }

            maxLevels = mDesc.height;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++ny; }

            maxLevels = mDesc.depth;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++nz; }

            maxLevels = max( max(nx, ny), nz );

            mDesc.levels = ( 0 == mDesc.levels ) ? maxLevels : min( maxLevels, mDesc.levels );

            // allocate mipmap size array
            mMipSize.resize( mDesc.levels );

            // check format
            if( ( mDesc.format < 0 || mDesc.format >= NUM_CLRFMTS ) &&
                FMT_DEFAULT != mDesc.format )
            {
                GN_ERROR(sLogger)( "invalid texture format: %s", clrFmt2Str(mDesc.format) );
                return false;
            }

            // success
            return true;
        }

        //!
        //! setup mip size
        //!
        void setMipSize( size_t level, const Vector3<UInt32> & s )
        {
            GN_ASSERT( level < mDesc.levels );
            mMipSize[level] = s;
        }

        //!
        //! setup mip size
        //!
        template<typename T>
        void setMipSize( size_t level, T sx, T sy, T sz )
        {
            GN_ASSERT( level < mDesc.levels );
            mMipSize[level].set( (UInt32)sx, (UInt32)sy, (UInt32)sz );
        }

        //!
        //! Get content loader
        //!
        const TextureLoader & getLoader() const { return mLoader; }

    private :
        TextureDesc   mDesc;   //!< descriptor
        TextureLoader mLoader; //!< content loader
        std::vector< Vector3<UInt32> > mMipSize; //!< mipmap size of each level
        StrA          mName; //!< texture name. Only for debug purpose.
    };

    //! \name convert between texture tags and string
    //@{

    //
    //
    // -------------------------------------------------------------------------
    inline bool
    texType2Str( StrA & str, TexType textype )
    {
        static const char * sTable [] =
        { "1D", "2D", "3D", "CUBE", "STACK" };

        if( 0 <= textype && textype < NUM_TEXTYPES )
        {
            str = sTable[textype];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    texType2Str( TexType textype )
    {
        static const char * sTable [] =
        { "1D", "2D", "3D", "CUBE", "STACK" };

        if( 0 <= textype && textype < NUM_TEXTYPES )
        {
            return sTable[textype];
        }
        else return "BAD_TEXTURE_TYPE";
    }
    //
    inline bool
    str2TexType( TexType & value, const char * name )
    {
        static const char * sTable[] =
        { "1D", "2D", "3D", "CUBE", "STACK" };

        if( name )
        {
            for( int i = 0; i < NUM_TEXTYPES; ++i )
            {
                if( 0 == ::strcmp(sTable[i],name) )
                {
                    value = static_cast<TexType>(i);
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
}}

// *****************************************************************************
//                           End of texture.h
// *****************************************************************************
#endif // __GN_GFX_TEXTURE_H__
