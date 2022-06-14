#ifndef __GN_GFX_TEXTURERESOURCE_H__
#define __GN_GFX_TEXTURERESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Texture resource implementation header.
/// \author  chenli@@REDMOND (2009.8.31)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN {
namespace gfx {
bool registerTextureResourceFactory(GpuResourceDatabase & db);
}
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_TEXTURERESOURCE_H__
