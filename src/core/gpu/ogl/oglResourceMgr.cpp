#include "pch.h"
#include "oglGpu.h"
#include "oglShader.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"
#include "oglTexture.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.ResourceMgr");

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpu::resourceInit() {
    GN_GUARD;

    if (!mResourceList.empty()) {
        GN_ERROR(sLogger)("Not _ALL_ graphics resouces are released!");
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpu::resourceQuit() {
    GN_GUARD;

    // check for non-released resources
    if (!mResourceList.empty()) {
        GN_UNEXPECTED_EX("All graphics resouces have to be released, before renderer is destroied!");
        for (std::list<OGLResource *>::iterator i = mResourceList.begin(); i != mResourceList.end(); ++i) {
            const OGLResource * r = *i;
            GN_ERROR(sLogger)("0x%p", r);
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
GN::gfx::GpuProgram * GN::gfx::OGLGpu::createGpuProgram(const GpuProgramDesc & desc) {
    GN_GUARD;

    if (0 == (desc.shaderModels & mCaps.shaderModels)) {
        GN_ERROR(sLogger)("Unsupported GPU shader model: %s", ShaderModel::sToString(desc.shaderModels).rawptr());
        return NULL;
    }

    switch (desc.lang) {
    case GpuProgramLanguage::GLSL: {
        AutoRef<OGLGpuProgram> prog = referenceTo(new OGLGpuProgram(*this));
        if (!prog->init(desc)) return NULL;
        return prog.detach();
    }

    default:
        GN_ERROR(sLogger)("invalid or unsupported GPU program language: %s", desc.lang.toString());
        return NULL;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Uniform * GN::gfx::OGLGpu::createUniform(uint32_t size) {
    Uniform * u = new SysMemUniform(size);
    u->incref();
    return u;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture * GN::gfx::OGLGpu::createTexture(const TextureDesc & desc) {
    GN_GUARD;

    AutoRef<OGLTexture> p = referenceTo(new OGLTexture(*this));
    if (!p->init(desc)) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::OGLGpu::createVtxBuf(const VtxBufDesc & desc) {
    GN_GUARD;

    AutoRef<OGLVtxBufVBO> p = referenceTo(new OGLVtxBufVBO(*this));
    if (!p->init(desc)) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::OGLGpu::createIdxBuf(const IdxBufDesc & desc) {
    GN_GUARD;

    AutoRef<OGLIdxBuf> p = referenceTo(new OGLIdxBuf);
    if (!p->init(desc)) return 0;

    return p.detach();

    GN_UNGUARD;
}
