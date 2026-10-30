// define shared data structure between cuda host and device.
#pragma once

#include <stdint.h>

struct LaunchParameters {
    uchar4 * image;
    uint32_t width;
    uint32_t height;
};
