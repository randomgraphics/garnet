#ifndef __GN_GFX_IMAGE_H__
#define __GN_GFX_IMAGE_H__

#if !GN_BUILD_IS_STATIC
    #define RAPID_IMAGE_SHARED_LIB 1
    #ifdef GN_CORE_INTERNAL
        #define RAPID_IMAGE_EXPORTS 1
    #endif
#endif

/// Define namespace for image module.
namespace GN::gfx::img {}

#define RAPID_IMAGE_NAMESPACE GN::gfx::img

// rapid-image library is still using printf syntax for logging, so we need to use true here.
#define RAPID_IMAGE_LOGE GN_ERROR(GN::getLogger("GN.gfx.image", true))
#define RAPID_IMAGE_LOGW GN_WARN(GN::getLogger("GN.gfx.image", true))
#define RAPID_IMAGE_LOGI GN_INFO(GN::getLogger("GN.gfx.image", true))

#include <rapid-image/rapid-image.h>

#endif