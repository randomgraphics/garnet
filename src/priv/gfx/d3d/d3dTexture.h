#ifndef __GN_GFXD3D_D3DTEXTURE_H__
#define __GN_GFXD3D_D3DTEXTURE_H__
// *****************************************************************************
//! \file    d3dTexture.h
//! \brief   D3D texture class
//! \author  chenlee (2005.11.5)
// *****************************************************************************

#include "../common/basicTexture.h"
#include "d3dResource.h"

namespace GN { namespace gfx
{
    class D3DRenderer;

    //!
    //! D3D texture
    //!
    class D3DTexture : public BasicTexture, public StdClass, public D3DResource
    {
         GN_DECLARE_STDCLASS( D3DTexture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3DTexture( D3DRenderer & r ) : D3DResource(r), mRenderer(r) { clear(); }
        virtual ~D3DTexture() { quit(); }
        //@}

        // ********************************
        // standard init/quit
        // ********************************

        //@{
    public:
        bool init( TexType type,
                   uint32_t w, uint32_t h, uint32_t d,
                   uint32_t levels,
                   ClrFmt format,
                   uint32_t usage );
        bool initFromFile( File & );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mD3DTexture = 0;
            mD3DFilters[0] =
            mD3DFilters[1] = D3DTEXF_LINEAR;
            mD3DFilters[2] = D3DTEXF_NONE;
            mD3DWraps[0] =
            mD3DWraps[1] =
            mD3DWraps[2] = D3DTADDRESS_WRAP;
        }
        //@}

        // ********************************
        // from D3DResource
        // ********************************

    public:
        bool deviceCreate() { return true; }
        bool deviceRestore();
        void deviceDispose();
        void deviceDestroy() {}

        // ********************************
        // From Texture
        // ********************************
    public:

        virtual void setFilter( TexFilter min, TexFilter mag ) const;
        virtual void setWrap( TexWrap s, TexWrap t, TexWrap r ) const;
        virtual void * lock1D( uint32_t level,
                               uint32_t offset,
                               uint32_t length,
                               uint32_t flag );
        virtual bool lock2D( LockedRect &  result,
                             uint32_t      level,
                             const Recti * area,
                             uint32_t      flag );
        virtual bool lock3D( LockedBox &  result,
                             uint32_t     level,
                             const Boxi * box,
                             uint32_t     flag );
        virtual bool lockCube( LockedRect &  result,
                               TexFace       face,
                               uint32_t      level,
                               const Recti * area,
                               uint32_t      flag );
        virtual void unlock();
        virtual void updateMipmap();
        virtual void * getAPIDependentData() const { return getD3DTexture(); }

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! get D3D texture
        //!
        LPDIRECT3DBASETEXTURE9 getD3DTexture() const { return mD3DTexture; }

        //!
        //! get D3D surface. Can _NOT_ be used on 3D texture.
        //!
        //! \note Do not foget to release the returned surface after using it.
        //!
        LPDIRECT3DSURFACE9 getSurface( TexFace face, uint32_t level ) const
        {
            GN_GUARD_SLOW;

            GN_ASSERT( mD3DTexture );

            LPDIRECT3DSURFACE9 surf;

            switch( getType() )
            {
                case TEXTYPE_1D:
                case TEXTYPE_2D:
                    {
                        LPDIRECT3DTEXTURE9 tex2D = static_cast<LPDIRECT3DTEXTURE9>( mD3DTexture );
                        GN_DX_CHECK_RV( tex2D->GetSurfaceLevel( level, &surf ), 0 );
                    }
                    return surf;

                case TEXTYPE_CUBE:
                    {
                        LPDIRECT3DCUBETEXTURE9 texCube = static_cast<LPDIRECT3DCUBETEXTURE9>( mD3DTexture );
                        GN_DX_CHECK_RV( texCube->GetCubeMapSurface( sTexFace2D3D(face), level, &surf ), 0 );
                    }
                    return surf;

                case TEXTYPE_3D:
                    GNGFX_ERROR( "Can't get surface from 3D texture." );
                    return 0;

                default:
                    // program should not reach here.
                    GNGFX_ERROR( "Invalid texture type!" );
                    GN_UNEXPECTED();
                    return 0;
            }

            GN_UNGUARD_SLOW;
        }

        //!
        //! get filters
        //!
        const D3DTEXTUREFILTERTYPE * getD3DFilters() const
        { return mD3DFilters; }

        //!
        //! get address wrapping modes
        //!
        const D3DTEXTUREADDRESS * getD3DWraps() const
        { return mD3DWraps; }

        // ********************************
        // private variables
        // ********************************
    private:

        D3DRenderer & mRenderer;

        //!
        //! \name texture initialization parameters
        //!
        //@{
        TexType  mInitType;
        uint32_t mInitSize[3];
        uint32_t mInitLevels;
        ClrFmt   mInitFormat;
        uint32_t mInitUsage;
        //@}

        //!
        //! D3D texture instance pointer
        //!
        IDirect3DBaseTexture9 * mD3DTexture;

        //!
        //! D3D texture parameters
        //!
        uint32_t mD3DUsage;

        //!
        //! D3D filters( min, mag, mip )
        //!
        mutable D3DTEXTUREFILTERTYPE mD3DFilters[3];

        //!
        //! texture wrapping modes
        //!
        mutable D3DTEXTUREADDRESS mD3DWraps[3];

        //!
        //! \name locking related variables
        //!
        //@{
        D3DCUBEMAP_FACES mLockedFace;
        uint32_t         mLockedLevel;
        //@}

        // ********************************
        // private functions
        // ********************************
    private:

        //!
        //! convert garnet cube face to D3D tag
        //!
        static D3DCUBEMAP_FACES sTexFace2D3D( TexFace face )
        {
            static D3DCUBEMAP_FACES sTable[ NUM_TEXFACES ] =
            {
                D3DCUBEMAP_FACE_POSITIVE_X,
                D3DCUBEMAP_FACE_NEGATIVE_X,
                D3DCUBEMAP_FACE_POSITIVE_Y,
                D3DCUBEMAP_FACE_NEGATIVE_Y,
                D3DCUBEMAP_FACE_POSITIVE_Z,
                D3DCUBEMAP_FACE_NEGATIVE_Z,
            };
            GN_ASSERT( 0 <= face && face < 6 );
            return sTable[face];
        }

        //!
        //! create instance of D3D texture
        //!
        LPDIRECT3DBASETEXTURE9
        newD3DTexture( TexType   type,
                       uint32_t  width,
                       uint32_t  height,
                       uint32_t  depth,
                       uint32_t  levels,
                       uint32_t  d3dusage,
                       D3DFORMAT d3dformat,
                       D3DPOOL   d3dpool );
    };
}}

// *****************************************************************************
//                           End of d3dTexture.h
// *****************************************************************************
#endif // __GN_GFXD3D_D3DTEXTURE_H__
