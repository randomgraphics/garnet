#pragma once
#ifndef __GN_GFX_IMAGE_H__
#define __GN_GFX_IMAGE_H__

#if !GN_BUILD_IS_STATIC
#define RAPID_IMAGE_SHARED_LIB 1
#ifdef GN_CORE_INTERNAL
#define RAPID_IMAGE_EXPORTS 1
#endif
#endif
#define RAPID_IMAGE_NAMESPACE GN::gfx
#include <rapid-image/rapid-image.h>

#endif