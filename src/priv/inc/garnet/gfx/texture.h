#ifndef __GN_GFX_TEXTURE_H__
#define __GN_GFX_TEXTURE_H__
// *****************************************************************************
//! \file    texture.h
//! \brief   Texture interface
//! \author  chenlee (2005.9.30)
// *****************************************************************************

//!
//! global texture dictionary
//!
#define gTexDict (GN::gfx::TextureDictionary::sGetInstance())

//!
//! pointer to global texture dictionary
//!
#define gTexDictPtr (GN::gfx::TextureDictionary::sGetInstancePtr())

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
    enum TexFace
    {
        TEXFACE_PX, //!< positive X
        TEXFACE_NX, //!< negative X
        TEXFACE_PY, //!< positive Y
        TEXFACE_NY, //!< negative Y
        TEXFACE_PZ, //!< positive Z
        TEXFACE_NZ, //!< negative Z
        NUM_TEXFACES
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

    struct Texture;

    //!
    //! Texture content loader
    //!
    typedef Functor1<bool,Texture&> TextureLoader;

    //!
    //! 贴图对象
    //!
    struct Texture : public RefCounter
    {
        //! \name basic texture properties
        //@{

        //!
        //! get texture type
        //!
        TexType getType() const { return mType; }

        //!
        //! get size of base map
        //!
        template<typename T>
        void getBaseSize( T * sx, T * sy = 0, T * sz = 0 ) const
        {
            if( sx ) *sx = (T)mSize.x;
            if( sy ) *sy = (T)mSize.y;
            if( sz ) *sz = (T)mSize.z;
        }

        //!
        //! get size of base map
        //!
        const Vector3<size_t> & getBaseSize() const { return mSize; }

        //!
        //! get size of specific mip level
        //!
        virtual void getMipSize( size_t level, size_t * sx, size_t * sy = 0, size_t * sz = 0 ) const = 0;

        //!
        //! get size of specific mip level (templat version)
        //!
        template<typename T>
        void getMipSizeT( size_t level, T * sx, T * sy = 0, T * sz = 0 ) const
        {
            size_t x, y, z;
            getMipSize( level, &x, &y, &z );
            if( sx ) *sx = (T)x;
            if( sy ) *sy = (T)y;
            if( sz ) *sz = (T)z;
        }

        //!
        //! get size of specific mip level
        //!
        Vector3<size_t> getMipSize( size_t level ) const
        {
            Vector3<size_t> sz;
            getMipSize( level, &sz.x, &sz.y, &sz.z );
            return sz;
        }

        //!
        //! get number of faces
        //!
        size_t getFaces() const { return mFaces; }

        //!
        //! get number of mipmap levels
        //!
        size_t getLevels() const { return mLevels; }

        //!
        //! get texture format
        //!
        ClrFmt getFormat() const { return mFormat; }

        //!
        //! get texture usage
        //!
        BitField getUsage() const { return mUsage; }

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

        //@}

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
        //! \param flag     Locking flags. See LockFlag for details.
        //!
        //! \return         Return false, if locking failed.
        //!
        virtual bool lock(
            TexLockedResult & result,
            size_t face,
            size_t level,
            const Boxi * area,
            BitField flag ) = 0;

        //!
        //! lock a 1D texture, only can be called on 1D texture
        //!
        void * lock1D( size_t level, size_t offset, size_t length, BitField flag )
        {
            GN_ASSERT( TEXTYPE_1D == getType() );
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

    protected :

        //!
        //! Set texture properties. Subclass must call this function to set 
        //! all texture properities to valid value.
        //!
        //!  - for 1D texture, mSize.y and size_z is always 1
        //!  - for non-3D texture, mSize.z is always 1
        //!  - for cube texture, mSize.y is always equal to mSize.x
        //!
        bool setProperties( TexType  type,
                            size_t   sx, size_t sy, size_t sz,
                            size_t   faces,
                            size_t   levels,
                            ClrFmt   format,
                            BitField usage )
        {
            // check type
            if( type < 0 || type >= NUM_TEXTYPES )
            {
                GN_ERROR( "invalid texture type!" );
                return false;
            }
            mType = type;

            // initiate texture size
            mSize.x =sx;
            switch( type )
            {
                case TEXTYPE_1D :
                {
                    mSize.y = 1;
                    mSize.z = 1;
                    break;
                }

                case TEXTYPE_2D :
                case TEXTYPE_CUBE :
                case TEXTYPE_STACK :
                {
                    mSize.y = sy;
                    mSize.z = 1;
                    break;
                }

                case TEXTYPE_3D :
                {
                    mSize.y = sy;
                    mSize.z = sz;
                    break;
                }

                default : GN_UNEXPECTED();
            }

            // initialize face count
            if( TEXTYPE_CUBE == type )
            {
                if( 0 != faces && 6 != faces )
                {
                    GN_WARN( "Cubemap must have 6 faces." );
                }
                mFaces = 6;
            }
            else if( TEXTYPE_STACK == type )
            {
                mFaces = 0 == faces ? 1 : faces;
            }
            else
            {
                if( 0 != faces && 1 != faces )
                {
                    GN_WARN( "Texture other then cube/stack texture can have only 1 face." );
                }
                mFaces = 1;
            }

            // calculate maximum mipmap levels
            size_t nx = 0, ny = 0, nz = 0;
            size_t maxLevels;

            maxLevels = mSize.x;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++nx; }

            maxLevels = mSize.y;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++ny; }

            maxLevels = mSize.z;
            while( maxLevels > 0 ) { maxLevels >>= 1; ++nz; }

            maxLevels = max( max(nx, ny), nz );

            mLevels = 0 == levels ? maxLevels : min( maxLevels, levels );

            // store format
            if( ( format < 0 || format >= NUM_CLRFMTS ) &&
                FMT_DEFAULT != format )
            {
                GN_ERROR( "invalid texture format: %s", clrFmt2Str(format) );
                return false;
            }
            mFormat = format;

            // store usage flags
            mUsage = usage;

            // success
            return true;
        }

        //!
        //! Get content loader
        //!
        const TextureLoader & getLoader() const { return mLoader; }

    private :

        TexType           mType;    //!< texture type
        Vector3<size_t>   mSize;    //!< texture size
        size_t            mFaces;   //!< texture face count
        size_t            mLevels;  //!< number of mipmap levels
        ClrFmt            mFormat;  //!< pixel format
        BitField          mUsage;   //!< usage flags
        TextureLoader     mLoader;  //!< content loader
    };

    //!
    //! Texture manager class (singleton)
    //!
    typedef ResourceManager<Texture*,true> TextureDictionary;

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
