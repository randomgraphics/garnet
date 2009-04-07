#ifndef __GN_GFXD3D10_D3D10RESOURCE_H__
#define __GN_GFXD3D10_D3D10RESOURCE_H__
// *****************************************************************************
/// \file
/// \brief
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "d3d10Renderer.h"

namespace GN { namespace gfx
{
    class D3D10Renderer;

    ///
    /// General D3D10 resource class
    ///
    class D3D10Resource
    {
    public:

        ///
        /// Get the renderer that the resource belongs to.
        ///
        D3D10Renderer & getRenderer() const { return mRenderer; }

        ///
        /// Get reference to D3D10Device
        ///
        ID3D10Device & getDeviceRef() const { return mRenderer.getDeviceRefInlined(); }

    protected :

        D3D10Resource( D3D10Renderer & ); ///< ctor
        virtual ~D3D10Resource();         ///< dtor

    private:

        D3D10Renderer & mRenderer;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10RESOURCE_H__
