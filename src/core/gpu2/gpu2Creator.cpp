#include <garnet/GNgfx2.h>

using namespace GN;
using namespace GN::gfx;

static auto * sLogger = GN::getLogger("GN.gfx.gpu2");

//
//
// -------------------------------------------------------------------------
#if GN_BUILD_HAS_D3D12
    #include "d3d12/d3d12gpu2.h"
static AutoRef<Gpu2> createD3D12Gpu2(const GN::gfx::Gpu2::CreateParameters & cp) { return AutoRef<Gpu2>(new D3D12Gpu2(cp)); }
#else
static AutoRef<Gpu2> createD3D12Gpu2(const GN::gfx::Gpu2::CreateParameters &) {
    GN_ERROR(sLogger)(" D3D12 renderer is not available.");
    return {};
}
#endif

//
//
// -------------------------------------------------------------------------
GN_API AutoRef<Gpu2> GN::gfx::Gpu2::createGpu2(const CreateParameters & cp) {
    switch (cp.api) {
    case GraphicsAPI::AUTO:
    case GraphicsAPI::D3D12:
        return createD3D12Gpu2(cp);
    case GraphicsAPI::VULKAN:
    default:
        GN_UNIMPL();
        return {};
    }
}
