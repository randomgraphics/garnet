#ifndef __GN_GFXD3D10_D3D10TEXTURE_H__
#define __GN_GFXD3D10_D3D10TEXTURE_H__
// *****************************************************************************
/// \file    d3d10Texture.h
/// \brief   D3D 10 texture class
/// \author  chenlee (2006.09.20)
// *****************************************************************************

#include "../common/basicTexture.h"

namespace GN { namespace gfx
{
    class D3D10Renderer;

    ///
    /// D3D texture
    ///
    class D3D10Texture : public BasicTexture, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D10Texture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10Texture( D3D10Renderer & r ) : mRenderer(r) { clear(); }
        virtual ~D3D10Texture() { quit(); }
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
            mD3DTexture.res = 0;
            mSRView = 0;
            mRTView = 0;
        }
        //@}

        // ********************************
        // From Texture
        // ********************************
    public:

        virtual void setFilter( TexFilter min, TexFilter mag ) const;
        virtual void setWrap( TexWrap s, TexWrap t, TexWrap r ) const;
        virtual bool lock( TexLockedResult & result, size_t face, size_t level, const Boxi * area, LockFlag flag );
        virtual void unlock();
        virtual void updateMipmap();
        virtual void * getAPIDependentData() const { return mD3DTexture.res; }

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        union D3D10Tex
        {
            ID3D10Resource  * res;
            ID3D10Texture1D * tex1d;
            ID3D10Texture2D * tex2d;
            ID3D10Texture3D * tex3d;
        };

        D3D10Renderer & mRenderer;

        D3D10Tex mD3DTexture; ///< resource instance

        ID3D10ShaderResourceView * mSRView; ///< view as shader resource
        ID3D10ShaderResourceView * mRTView; ///< view as render target

        ///
        /// \name locking related variables
        ///
        //@{
        D3D10Tex mLockedTexture;
        LockFlag mLockedFlag;
        size_t mLockedFace;
        size_t mLockedLevel;
        //@}

        static Logger * sLogger;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createTexture();
        bool createViews();
    };
}}

// *****************************************************************************
//                           End of d3d10Texture.h
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10TEXTURE_H__
