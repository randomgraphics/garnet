#ifndef __GN_GFX_GFXTEXTURE_H__
#define __GN_GFX_GFXTEXTURE_H__
// *****************************************************************************
//! \file    gfxTexture.h
//! \brief   Texture interface
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
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
        template<typename T1, typename T2, typename T3>
        void getSize( T1 * sx, T2 * sy = 0, T3 * sz = 0 ) const
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
        virtual void * lock( uint32_t level,
                             uint32_t offset,
                             uint32_t length,
                             LockFlag flag ) = 0;

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
        virtual bool lock( LockedRect &  result,
                           uint32_t      level,
                           const Recti * area,
                           LockFlag      flag ) = 0;

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
        virtual bool lock( LockedBox &  result,
                           uint32_t     level,
                           const Boxi * box,
                           LockFlag     flag ) = 0;

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
        virtual bool lock( LockedRect &  result,
                           TexFace       face,
                           uint32_t      level,
                           const Recti * area,
                           LockFlag      flag ) = 0;

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
        virtual void updateMips() = 0;

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

            k = std::max( std::max(nx, ny), nz );

            mLevels = 0 == levels ? k : std::min( k, levels );

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
}}

// *****************************************************************************
//                           End of gfxTexture.h
// *****************************************************************************
#endif // __GN_GFX_GFXTEXTURE_H__
