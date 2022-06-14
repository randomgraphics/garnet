#include "pch.h"
#include "d3d11Gpu.h"
#include "d3d11Resource.h"
#include "d3d11Shader.h"
#include "d3d11Texture.h"
#include "d3d11Buffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::resourceInit() {
    GN_GUARD;

    GN_ASSERT(mResourceList.empty());

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::resourceQuit() {
    GN_GUARD;

    if (!mResourceList.empty()) {
        GN_ERROR(sLogger)("All graphics resouces MUST be released, before destroying GPU!");

        std::list<D3D11Resource *>::iterator i = mResourceList.begin();
        while (i != mResourceList.end()) {
            D3D11Resource * r = *i;
            GN_ERROR(sLogger)("0x%p", r);
            ++i;
        }
    }

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgram * GN::gfx::D3D11Gpu::createGpuProgram(const GpuProgramDesc & desc) {
    if (0 == (desc.shaderModels & mCaps.shaderModels)) {
        GN_ERROR(sLogger)("Unsupported GPU shader model: %s", ShaderModel::sToString(desc.shaderModels).rawptr());
        return NULL;
    }

    if (GpuProgramLanguage::HLSL10 == desc.lang || GpuProgramLanguage::HLSL9 == desc.lang) {
        AutoRef<D3D11GpuProgramHLSL> prog = referenceTo(new D3D11GpuProgramHLSL(*this));
        if (!prog->init(desc)) return NULL;
        return prog.detach();
    } else {
        GN_ERROR(sLogger)("Unsupported or invalid GPU program language: %d", desc.lang.toRawEnum());
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Uniform * GN::gfx::D3D11Gpu::createUniform(uint32 size) {
    Uniform * result = new SysMemUniform(size);
    result->incref();
    return result;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture * GN::gfx::D3D11Gpu::createTexture(const TextureDesc & desc) {
    AutoRef<D3D11Texture> p = referenceTo(new D3D11Texture(*this));
    if (!p->init(desc)) return 0;
    return p.detach();
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::D3D11Gpu::createVtxBuf(const VtxBufDesc & desc) {
    AutoRef<D3D11VtxBuf> buf = referenceTo(new D3D11VtxBuf(*this));

    if (!buf->init(desc)) return 0;

    return buf.detach();
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::D3D11Gpu::createIdxBuf(const IdxBufDesc & desc) {
    AutoRef<D3D11IdxBuf> buf = referenceTo(new D3D11IdxBuf(*this));

    if (!buf->init(desc)) return 0;

    return buf.detach();
}
