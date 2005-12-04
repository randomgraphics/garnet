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
    //! Ã˘Õº¿‡–Õ
    //!
    enum TexType
    {
        TEXTYPE_1D,    //!< 1D texture
        TEXTYPE_2D,    //!< 2D texture
        TEXTYPE_3D,    //!< 3D texture
        TEXTYPE_CUBE,  //!< Cube texture
        NUM_TEXTYPES   //!< Number of avaliable texture types.
    };

    //!
    //! Texture usage.
    //!
    enum TextureUsage
    {
        TEXUSAGE_DYNAMIC        = 1<<0, //!< dynamic texture
        TEXUSAGE_AUTOGEN_MIPMAP = 1<<1, //!< texture that can auto-generate mipmaps
        TEXUSAGE_RENDERTARGET   = 1<<2, //!< render target texture
        TEXUSAGE_DEPTH          = 1<<3, //!< depth texture
    };

    //!
    //! Ã˘Õºπ˝¬À∆˜
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
    //! Ã˘ÕºWrap Mode
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
    //! Ã˘Õº∂‘œÛ
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
        //! get texture size
        //!
        template<typename T>
        void getSize( T * sx, T * sy = 0, T * sz = 0 ) const
        {
            if( sx ) *sx = mSize.x;
            if( sy ) *sy = mSize.y;
            if( sz ) *sz = mSize.z;
        }

        //!
        //! get texture size
        //!
        const Vector3<uint32_t> & getSize() const { return mSize; }

        //!
        //! get number of mipmap levels
        //!
        uint32_t getLevels() const { return mLevels; }

        //!
        //! get texture format
        //!
        ClrFmt getFormat() const { return mFormat; }

        //!
        //! get texture usage
        //!
        uint32_t getUsage() const { return mUsage; }

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

        //! \name lock/unlock functions
        //@{

        //!
        //! lock a 1D texture, only can be called on 1D texture
        //!
        //! \param level    specify the miplevel you want to lock, starting from 0
        //! \param offset   offset from start of texture, in pixel unit.
        //! \param length   length of the lock, in pixel unit. 0 means "to the end
        //!                 of the texture".
        //! \param flag     locking flags
        //!
        //! \return         return false, if lock failed.
        //!
        virtual void * lock1D( uint32_t level, uint32_t offset, uint32_t length, uint32_t flag ) = 0;

        //!
        //! lock a 2D texture, only can be called on 2D texture
        //!
        //! \param result   return the locking result.
        //! \param level    specify the miplevel you want to lock, starting from 0
        //! \param area     specify locking area, 0 means whole texture.
        //! \param flag     locking flags
        //!
        //! \return         return false, if lock failed.
        //!
        virtual bool lock2D( LockedRect &  result, uint32_t level, const Recti * area, uint32_t flag ) = 0;

        //!
        //! lock a 3D texture, only can be called on 3D texture
        //!
        //! \param result   return the locking result.
        //! \param level    specify the miplevel you want to lock, starting from 0
        //! \param box      specify locking area, 0 means whole texture.
        //! \param flag     locking flags
        //!
        //! \return         return false, if lock failed.
        //!
        virtual bool lock3D( LockedBox &  result, uint32_t level, const Boxi * box, uint32_t flag ) = 0;

        //!
        //! lock a cube texture, only can be called on cube texture
        //!
        //! \param result   return the locking result.
        //! \param face     specify the face you want t lock
        //! \param level    specify the miplevel you want to lock, starting from 0
        //! \param area     specify locking area, 0 means whole texture.
        //! \param flag     locking flags
        //!
        //! \return         return false, if lock failed.
        //!
        virtual bool lockCube( LockedRect &  result,
                               TexFace       face,
                               uint32_t      level,
                               const Recti * area,
                               uint32_t      flag ) = 0;

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
        //! Set texture properties
        //!
        //!  - for 1D texture, mSize.y and size_z is always 1
        //!  - for non-3D texture, mSize.z is always 1
        //!  - for cube texture, mSize.y is always equal to mSize.x
        //!
        bool setProperties( TexType  type,
                            uint32_t sx, uint32_t sy, uint32_t sz,
                            uint32_t levels,
                            ClrFmt   format,
                            uint32_t usage )
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
            if( TEXTYPE_1D == type ) mSize.y = 1;
            else if( TEXTYPE_CUBE == type ) mSize.y = sx;
            else mSize.y = sy;
            if( TEXTYPE_3D != type ) mSize.z = 1;
            else mSize.z = sz;

            // store format & creation flags
            mFormat = format;
            mUsage = usage;

            // calculate mipmap levels
            uint32_t nx = 0, ny = 0, nz = 0;
            uint32_t k;

            k = mSize.x;
            while( k > 0 ) { k >>= 1; ++nx; }

            k = mSize.y;
            while( k > 0 ) { k >>= 1; ++ny; }

            k = mSize.z;
            while( k > 0 ) { k >>= 1; ++nz; }

            k = max( max(nx, ny), nz );

            mLevels = 0 == levels ? k : min( k, levels );

            // success
            return true;
        }

    private :
        TexType             mType;    //!< texture type
        Vector3<uint32_t>   mSize;    //!< texture size
        uint32_t            mLevels;  //!< number of mipmap levels
        ClrFmt              mFormat;  //!< pixel format
        uint32_t            mUsage;   //!< creation flags
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
        { "1D", "2D", "3D", "CUBE" };

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
        { "1D", "2D", "3D", "CUBE" };

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
        static const char * sTable [] =
        { "1D", "2D", "3D", "CUBE" };

        if( name )
        {
            for( int i = 0; i < 4; ++i )
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
