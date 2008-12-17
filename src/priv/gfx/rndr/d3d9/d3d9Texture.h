#ifndef __GN_GFXD3D9_D3D9TEXTURE_H__
#define __GN_GFXD3D9_D3D9TEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   D3D texture class
/// \author  chenlee (2005.11.5)
// *****************************************************************************

#include "../common/basicSurface.h"
#include "d3d9Resource.h"

namespace GN { namespace gfx
{
    class D3D9Renderer;

    ///
    /// Convert texture type to D3DRESOURCETYPE
    ///
    D3DRESOURCETYPE texType2D3DResourceType( TexDim );

    ///
    /// Convert texture usage to D3DUSAGE(s)
    ///
    DWORD texUsage2D3DUsage( BitFields );

    ///
    /// D3D texture
    ///
    class D3D9Texture : public BasicTexture, public StdClass, public D3D9Resource
    {
         GN_DECLARE_STDCLASS( D3D9Texture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9Texture( D3D9Renderer & r ) : D3D9Resource(r) { clear(); }
        virtual ~D3D9Texture() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const TextureDesc & desc );
        void quit();
    private:
        void clear()
        {
            mD3DTexture = 0;
            mShadowCopy = 0;
        }
        //@}

        // ********************************
        // from D3D9Resource
        // ********************************

    public:
        bool deviceRestore();
        void deviceDispose();

        // ********************************
        // From Texture
        // ********************************
    public:

        virtual bool lock( TexLockedResult & result, size_t face, size_t level, const TexLockArea * area, LockFlag flag );
        virtual void unlock();
        virtual void updateMipmap();
        virtual void * getAPIDependentData() const { return mD3DTexture; }

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Bind myself to specific stage
        ///
        void bind( UINT stage ) const
        {
            D3D9Renderer & r = getRenderer();
            r.getDevice()->SetTexture( stage, mD3DTexture );
/*
            r.setD3DSamplerState( stage, D3DSAMP_MINFILTER, mD3DFilters[0] );
            r.setD3DSamplerState( stage, D3DSAMP_MAGFILTER, mD3DFilters[1] );
            r.setD3DSamplerState( stage, D3DSAMP_MIPFILTER, mD3DFilters[2] );

            r.setD3DSamplerState( stage, D3DSAMP_ADDRESSU, mD3DWraps[0] );
            r.setD3DSamplerState( stage, D3DSAMP_ADDRESSV, mD3DWraps[1] );
            r.setD3DSamplerState( stage, D3DSAMP_ADDRESSW, mD3DWraps[2] );*/
        }

        ///
        /// get D3D surface. Can _NOT_ be used on 3D texture.
        ///
        /// \note Do not foget to release the returned surface after using it.
        ///
        LPDIRECT3DSURFACE9 getSurface( size_t face, size_t level ) const
        {
            GN_GUARD_SLOW;

            GN_ASSERT( mD3DTexture );

            LPDIRECT3DSURFACE9 surf;

            switch( getDesc().dim )
            {
                case TEXDIM_1D:
                case TEXDIM_2D:
                    {
                        LPDIRECT3DTEXTURE9 tex2D = static_cast<LPDIRECT3DTEXTURE9>( mD3DTexture );
                        GN_DX9_CHECK_RV( tex2D->GetSurfaceLevel( (DWORD)level, &surf ), 0 );
                    }
                    return surf;

                case TEXDIM_CUBE:
                    {
                        LPDIRECT3DCUBETEXTURE9 texCube = static_cast<LPDIRECT3DCUBETEXTURE9>( mD3DTexture );
                        GN_DX9_CHECK_RV( texCube->GetCubeMapSurface( sCubeFace2D3D(face), (DWORD)level, &surf ), 0 );
                    }
                    return surf;

                case TEXDIM_3D:
                    GN_ERROR(sLogger)( "Can't get surface from 3D texture." );
                    return 0;

                default:
                    // program should not reach here.
                    GN_ERROR(sLogger)( "Invalid texture type!" );
                    GN_UNEXPECTED();
                    return 0;
            }

            GN_UNGUARD_SLOW;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        ///
        /// D3D texture instance pointer
        ///
        IDirect3DBaseTexture9 * mD3DTexture;

        ///
        /// shadow copy in system memory
        ///
        IDirect3DBaseTexture9 * mShadowCopy;

        ///
        /// D3D texture parameters
        ///
        //@{
        D3DFORMAT mD3DFormat;
        DWORD     mD3DUsage;
        //@}

        bool mWritable;  ///< Is the D3D texture writable?
        bool mIsRGBA;    ///< R-G-B-A is special

        ///
        /// D3D filters( min, mag, mip )
        ///
        //mutable D3DTEXTUREFILTERTYPE mD3DFilters[3];

        ///
        /// texture wrapping modes
        ///
        //mutable D3DTEXTUREADDRESS mD3DWraps[3];

        ///
        /// \name locking related variables
        ///
        //@{
        LockFlag mLockedFlag;
        size_t   mLockedFace;
        size_t   mLockedLevel;
        bool     mLockedNeedRebind; // for Xenon only, to unbind/rebind texture to device before/after lock
        UInt32   mLockedRebindStage;
        TexLockArea     mLockedArea;
        TexLockedResult mLockedResult;
        //@}

        static Logger * sLogger;

        // ********************************
        // private functions
        // ********************************
    private:

        ///
        /// convert garnet cube face to D3D tag
        ///
        static D3DCUBEMAP_FACES sCubeFace2D3D( size_t face )
        {
            static D3DCUBEMAP_FACES sTable[ NUM_CUBEFACES ] =
            {
                D3DCUBEMAP_FACE_POSITIVE_X,
                D3DCUBEMAP_FACE_NEGATIVE_X,
                D3DCUBEMAP_FACE_POSITIVE_Y,
                D3DCUBEMAP_FACE_NEGATIVE_Y,
                D3DCUBEMAP_FACE_POSITIVE_Z,
                D3DCUBEMAP_FACE_NEGATIVE_Z,
            };
            GN_ASSERT( face < 6 );
            return sTable[face];
        }

        ///
        /// create instance of D3D texture
        ///
        LPDIRECT3DBASETEXTURE9
        newD3DTexture( TexDim   type,
                       size_t    width,
                       size_t    height,
                       size_t    depth,
                       size_t    levels,
                       DWORD     d3dusage,
                       D3DFORMAT d3dformat,
                       D3DPOOL   d3dpool );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9TEXTURE_H__
