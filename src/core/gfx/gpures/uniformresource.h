#ifndef __GN_GFX_UNIFORMRESOURCE_H__
#define __GN_GFX_UNIFORMRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Uniform resource implementation header
/// \author  chenli@@REDMOND (2009.8.31)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN {
namespace gfx {
bool registerUniformResourceFactory(GpuResourceDatabase & db);
}
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UNIFORMRESOURCE_H__
