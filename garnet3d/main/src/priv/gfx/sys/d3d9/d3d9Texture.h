#ifndef __GN_GFX2_D3D9TEXTURE_H__
#define __GN_GFX2_D3D9TEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 texture surface
/// \author  chen@@CHENLI-HOMEPC (2007.6.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// texture surface
    ///
    class D3D9Texture : public D3D9Surface
    {
    public:

        ///
        /// create new instance
        ///
        static D3D9Texture * sNewInstance(
            D3D9GraphicsSystem          & gs,
            D3D9SurfaceType               surftype,
            const SurfaceLayout         & layout,
            int                           access,
            const SurfaceCreationHints  & hints );

        ///
        /// dtor
        ///
        ~D3D9Texture() { safeRelease( mSurface ); }

        ///
        /// get D3D surface
        ///
        IDirect3DBaseTexture9 * getSurface() const { GN_ASSERT(mSurface); return mSurface; }

        // from parent class
        //@{
        virtual const SubSurfaceLayout * getSubSurfaceLayout( size_t subsurface ) const;
        virtual void download(
            size_t                 subsurface,
            const Box<size_t>    * area,
            const void           * source,
            size_t                 srcRowBytes,
            size_t                 srcSliceBytes );
        virtual void upload(
            size_t              subsurface,
            const Box<size_t> * area,
            void              * destination,
            size_t              destRowBytes,
            size_t              destSliceBytes );
        virtual void save( NativeSurfaceData & ) const;
        virtual void load( const NativeSurfaceData & );
        //@}

    private:

        D3D9GraphicsSystem          & mGraphicsSystem;
        IDirect3DBaseTexture9       * mSurface;
        SurfaceCreationHints          mHints;
        DynaArray<SubSurfaceLayout>   mSubsurfaces;
        bool                          mIsRGBA;

    private:

        D3D9Texture( D3D9GraphicsSystem & gs, const D3D9SurfaceDesc & desc, const SurfaceCreationHints & hints );

        bool init();

        bool create2DTexture();
        bool create3DTexture();
        bool createCubeTexture();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9TEXTURE_H__
