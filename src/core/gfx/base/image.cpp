#include "pch.h"

#ifdef _MSC_VER
    #define __STDC_LIB_EXT1__ // this is to tell stb image header to use secure version of the C library.
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wmissing-field-initializers" // this is to suppress warning from stb image header.
#pragma clang diagnostic ignored "-Wdeprecated-declarations" // this is to suppress warning from stb image header.
#endif
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#define RAPID_IMAGE_IMPLEMENTATION
#include <rapid-image/rapid-image.h>