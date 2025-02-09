#include "pch.h"
#include "oglVtxFmt.h"
#include "oglGpu.h"
#include "oglVtxBuf.h"

using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.VtxFmt");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::init(const VertexBinding & attributes, const OGLGpuProgram * program) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    mFormat = attributes;

    setupStateBindings(program);

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxFmt::quit() {
    GN_GUARD;

    if (mVAO) glDeleteVertexArrays(1, &mVAO), mVAO = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::bindBuffers(const VertexBufferBinding * bindings, size_t numbufs, size_t startvtx) const {
    GN_GUARD_SLOW;

    if (!mVAO) return false;

    for (size_t i = 0; i < mAttribBindings.size(); ++i) {
        auto & ab     = mAttribBindings[i];
        size_t stream = ab.stream;
        if (stream >= numbufs) {
            GN_ERROR(sLogger)("Current vertex format requires at least %u vertex buffers. But only %u are provided.", stream + 1, numbufs);
            return false;
        }
        const VertexBufferBinding & b = bindings[stream];
        safeCastPtr<const OGLVtxBufVBO>(b.vtxbuf.data())->bind();
        glVertexAttribPointer(ab.index, ab.components, ab.format, ab.normalization, (GLsizei) b.stride,
                              (const uint8_t *) (intptr_t) (startvtx * b.stride + b.offset + ab.offset));
    }

    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::bindRawMemoryBuffer(const void * data, size_t stride) const {
    GN_GUARD_SLOW;

    if (!mVAO) return false;

    for (size_t i = 0; i < mAttribBindings.size(); ++i) {
        auto & ab = mAttribBindings[i];

        if (ab.stream > 0) {
            GN_ERROR(sLogger)("Current vertex format requires at least %u vertex buffers. But only 1 are provided.", ab.stream + 1);
            return false;
        }

        GN_OGL_CHECK_R(glVertexAttribPointer(ab.index, ab.components, ab.format, ab.normalization, (GLsizei) stride, (const uint8_t *) data + ab.offset),
                       false);
    }

    return true;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxFmt::setupStateBindings(const OGLGpuProgram * gpuProgram) {
    GN_GUARD;

    uint32_t        maxAttributes = getGpu().getOGLCaps().maxVertexAttributes;
    DynaArray<bool> hasAttrib(maxAttributes, false);

    OGLVertexBindingDesc vbd;

    mAttribBindings.reserve(mFormat.size());

    for (size_t i = 0; i < mFormat.size(); ++i) {
        const VertexElement & e = mFormat[i];

        // get binding information
        if ((NULL == gpuProgram || !gpuProgram->getBindingDesc(vbd, e.attribute))) {
            GN_WARN(sLogger)
            ("Attribute #%d is ignored, since it is not used by "
             "current active GPU program.",
             i);
            continue;
        }

        AttribBindingInfo ab;
        ab.self   = this;
        ab.stream = (uint8_t) e.stream;
        ab.offset = (uint8_t) e.offset;

        switch (vbd.semantic) {
        case VERTEX_SEMANTIC_ATTRIBUTE:
            ab.index             = vbd.index;
            hasAttrib[vbd.index] = true;
            break;

        case VERTEX_SEMANTIC_VERTEX:
        case VERTEX_SEMANTIC_NORMAL:
        case VERTEX_SEMANTIC_TEXCOORD:
        case VERTEX_SEMANTIC_COLOR:
        case VERTEX_SEMANTIC_FOG:
            GN_ERROR(sLogger)("client side vertex data pointer is not supportd anymore.");
            return false;

        default:
            GN_UNEXPECTED();
            return false;
        }

        if (PixelFormat::FLOAT1() == e.format) {
            ab.format        = GL_FLOAT;
            ab.components    = 1;
            ab.normalization = false;
        } else if (PixelFormat::FLOAT2() == e.format) {
            ab.format        = GL_FLOAT;
            ab.components    = 2;
            ab.normalization = false;
        } else if (PixelFormat::FLOAT3() == e.format) {
            ab.format        = GL_FLOAT;
            ab.components    = 3;
            ab.normalization = false;
        } else if (PixelFormat::FLOAT4() == e.format) {
            ab.format        = GL_FLOAT;
            ab.components    = 4;
            ab.normalization = false;
        } else if (PixelFormat::RGBA8() == e.format) {
            ab.format        = GL_UNSIGNED_BYTE;
            ab.components    = 4;
            ab.normalization = true;
        } else if (PixelFormat::USHORT4() == e.format) {
            ab.format        = GL_UNSIGNED_SHORT;
            ab.components    = 4;
            ab.normalization = false;
        } else if (PixelFormat::SHORT4() == e.format) {
            ab.format        = GL_SHORT;
            ab.components    = 4;
            ab.normalization = false;
        } else if (PixelFormat::UINT4() == e.format) {
            ab.format        = GL_UNSIGNED_INT;
            ab.components    = 4;
            ab.normalization = false;
        } else if (PixelFormat::SINT4() == e.format) {
            ab.format        = GL_INT;
            ab.components    = 4;
            ab.normalization = false;
        } else {
            GN_ERROR(sLogger)("unsupport vertex format: %s", e.format.toString().c_str());
            return false;
        }

        mAttribBindings.append(ab);
    }

    // ===================
    // setup state binding
    // ===================
    GN_OGL_CHECK_R(glGenVertexArrays(1, &mVAO), false);
    glBindVertexArray(mVAO);
    for (uint32_t i = 0; i < maxAttributes; ++i) {
        if (hasAttrib[i]) {
            glEnableVertexAttribArrayARB(i);
        } else {
            glDisableVertexAttribArrayARB(i);
        }
    }
    glBindVertexArray(0);

    // success
    return true;

    GN_UNGUARD;
}
