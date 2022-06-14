#define GN_CORE_INTERNAL
#include <garnet/GNgfx2.h>

using namespace GN;
using namespace GN::gfx;

static auto * sLogger = GN::getLogger("GN.gfx.gpu2");

#if GN_BUILD_HAS_D3D12
    #include <d3dcompiler.h>
//
//
// -------------------------------------------------------------------------
GN_API DynaArray<uint8_t> GN::gfx::compileHLSL(const ShaderCompileParameters & p) {
    AutoComPtr<ID3DBlob> bin, err;

    auto hr = D3DCompile(p.source, strlen(p.source), nullptr, nullptr, nullptr, p.entry, p.profile, 0, 0, &bin, &err);

    if (FAILED(hr)) {
        GN_ERROR(sLogger)("failed to compile shader:\n%s", (const char *) err->GetBufferPointer());
        return {};
    }

    return {(const uint8_t *) bin->GetBufferPointer(), bin->GetBufferSize()};
}
#else
GN_API DynaArray<uint8_t> GN::gfx::compileHLSL(const ShaderCompileParameters &) {
    GN_ERROR(sLogger)("compiling HLSL is not supported on current build.");
    return {};
}
#endif