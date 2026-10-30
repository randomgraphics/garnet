#ifndef __GN_GFXD3D11_D3D11RESOURCE_H__
#define __GN_GFXD3D11_D3D11RESOURCE_H__
// *****************************************************************************
/// \file
/// \brief
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "d3d11Gpu.h"

namespace GN {
namespace gfx {
class D3D11Gpu;

///
/// General D3D11 resource class
///
class D3D11Resource {
public:
    ///
    /// Get the GPU that the resource belongs to.
    ///
    D3D11Gpu & getGpu() const { return mGpu; }

    ///
    /// Get reference to D3D11Device
    ///
    ID3D11Device & getDeviceRef() const { return mGpu.getDeviceRefInlined(); }

    ///
    /// Get reference to D3D11Device
    ///
    ID3D11DeviceContext & getDeviceContextRef() const { return mGpu.getDeviceContextRefInlined(); }

protected:
    D3D11Resource(D3D11Gpu &); ///< ctor
    virtual ~D3D11Resource();  ///< dtor

private:
    D3D11Gpu & mGpu;
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11RESOURCE_H__
