#ifndef __GN_GFXD3D11_D3D11RESOURCE_H__
#define __GN_GFXD3D11_D3D11RESOURCE_H__
// *****************************************************************************
/// \file
/// \brief
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "d3d11Renderer.h"

namespace GN { namespace gfx
{
    class D3D11Renderer;

    ///
    /// General D3D11 resource class
    ///
    class D3D11Resource
    {
    public:

        ///
        /// Get the renderer that the resource belongs to.
        ///
        D3D11Renderer & getRenderer() const { return mRenderer; }

        ///
        /// Get reference to D3D11Device
        ///
        ID3D11Device & getDeviceRef() const { return mRenderer.getDeviceRefInlined(); }

    protected :

        D3D11Resource( D3D11Renderer & ); ///< ctor
        virtual ~D3D11Resource();         ///< dtor

    private:

        D3D11Renderer & mRenderer;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11RESOURCE_H__
