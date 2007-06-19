#ifndef __GN_GFX2_D3D9BACKBUFFER_H__
#define __GN_GFX2_D3D9BACKBUFFER_H__
// *****************************************************************************
//! \file    d3d9/d3d9BackBuffer.h
//! \brief   D3D9 back buffer class
//! \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// backbuffer surface
    ///
    class D3D9BackBuffer : public D3D9Surface
    {
        SurfaceDesc         mDesc;
        IDirect3DSurface9 * mSurface;

        D3D9BackBuffer( const SurfaceDesc & desc ) : mDesc(desc), mSurface(0) {}

    public:

        ///
        /// get depth surface
        ///
        IDirect3DSurface9 * getSurface() const { GN_ASSERT(mSurface); return mSurface; }

        ///
        /// create new instance
        ///
        static D3D9BackBuffer * sNewInstance(
            const SurfaceLayoutTemplate & templ,
            int                           access,
            const SurfaceCreationHints  & hints );

        // from effect
        virtual const SurfaceDesc & getDesc() const { return mDesc; }
    };
}}

// *****************************************************************************
//                           End of d3d9BackBuffer.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9BACKBUFFER_H__
