#ifndef __GN_GFX2_D3D9DEPTHBUFFER_H__
#define __GN_GFX2_D3D9DEPTHBUFFER_H__
// *****************************************************************************
//! \file    d3d9/d3d9DepthBuffer.h
//! \brief   D3D9 depth buffer class
//! \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// depthbuffer surface
    ///
    class D3D9DepthBuffer : public D3D9Surface
    {
        SurfaceDesc         mDesc;
        IDirect3DSurface9 * mSurface;

        D3D9DepthBuffer( const SurfaceDesc & desc ) : mDesc(desc), mSurface(0) {}

    public:

        ///
        /// get depth surface
        ///
        IDirect3DSurface9 * getSurface() const { GN_ASSERT(mSurface); return mSurface; }

        ///
        /// create new instance
        ///
        static D3D9DepthBuffer * sNewInstance(
            const SurfaceLayoutTemplate & templ,
            int                           access,
            const SurfaceCreationHints  & hints );

        // from effect
        virtual const SurfaceDesc & getDesc() const { return mDesc; }
    };
}}

// *****************************************************************************
//                           End of d3d9DepthBuffer.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9DEPTHBUFFER_H__
