#include "shared.h"
#include <optix.h>
#include <stdio.h>

extern "C" {
    __constant__ LaunchParameters launchParameters;
}

extern "C"
__global__ void __raygen__test1() {
    auto launchIndex = optixGetLaunchIndex();
    launchParameters.image[launchIndex.y * launchParameters.width + launchIndex.x] = make_uchar4(
        (uint8_t)( launchIndex.x * 255 / ( launchParameters.width  - 1 ) ),
        (uint8_t)( launchIndex.y * 255 / ( launchParameters.height - 1 ) ),
        0,
        255
    );
}
