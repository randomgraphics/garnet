#include "shared.h"
#include <optix.h>

extern "C" {
    __constant__ LaunchParameters launchParameters;
}

extern "C"
__global__ void __raygen__test1() {
    auto launchIndex = optixGetLaunchIndex();
    launchParameters.image[launchIndex.y * launchParameters.width + launchIndex.x] = make_uchar4(255, 0, 0, 255); // clear to pure red.
}
