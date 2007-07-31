#ifndef __GN_GFX2_D3D9DEPTHBUFFER_H__
#define __GN_GFX2_D3D9DEPTHBUFFER_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 depth buffer class
/// \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// depthbuffer surface
    ///
    class D3D9DepthBuffer : public D3D9Surface
    {
        IDirect3DSurface9 * mSurface;

        D3D9DepthBuffer( const D3D9SurfaceDesc & desc ) : D3D9Surface(desc), mSurface(0) {}

    public:

        ///
        /// get depth surface
        ///
        IDirect3DSurface9 * getSurface() const { GN_ASSERT(mSurface); return mSurface; }

        ///
        /// create new instance
        ///
        static D3D9DepthBuffer * sNewInstance(
            const SurfaceLayout         & layout,
            int                           access,
            const SurfaceCreationHints  & hints );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9DEPTHBUFFER_H__
