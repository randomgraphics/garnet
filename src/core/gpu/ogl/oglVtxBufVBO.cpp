#include "pch.h"
#include "oglVtxBuf.h"
#include "oglGpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufVBO::init(const VtxBufDesc & desc) {
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // standard init procedure
    GN_STDCLASS_INIT();

    if (0 == desc.length) {
        GN_ERROR(sLogger)("Vertex buffer size can't be zero!");
        return failure();
    }

    // store properties
    setDesc(desc);

    // determine buffer usage
    // TODO: try GL_STREAM_DRAW
    mOGLUsage = desc.fastCpuWrite ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // initialize device data
    if (!createVBO()) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::quit() {
    GN_GUARD;

    // release opengl vertex array
    if (mOGLVertexBufferObject) {
        GN_ASSERT(glIsBuffer(mOGLVertexBufferObject));
        GN_OGL_CHECK(glDeleteBuffers(1, &mOGLVertexBufferObject));
        mOGLVertexBufferObject = 0;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from VtxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::update(uint32 offset, uint32 length, const void * data, SurfaceUpdateFlag flag) {
    GN_GUARD_SLOW;

    GN_ASSERT(ok());

    if (!validateUpdateParameters(offset, &length, data, flag)) return;

    if (0 == length) return;

    OGLAutoAttribStack autoAttribStack(0, GL_CLIENT_VERTEX_ARRAY_BIT);

    // sanity check
    GN_ASSERT(offset < getDesc().length && 0 < length && (offset + length) <= getDesc().length);

    // bind as active buffer
    GN_OGL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mOGLVertexBufferObject));

    // update VBO
    GN_OGL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, offset, length, data));

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::readback(DynaArray<uint8> & data) {
    GN_UNIMPL_WARNING();
    data.clear();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufVBO::createVBO() {
    GN_GUARD;

    struct AutoDel {
        GLuint vbo;

        AutoDel(GLuint v): vbo(v) {}

        ~AutoDel() {
            if (vbo) glDeleteBuffers(1, &vbo);
        }

        void dismiss() { vbo = 0; }
    };

    // create VBO
    GN_OGL_CHECK_R(glGenBuffers(1, &mOGLVertexBufferObject), false);
    AutoDel ad(mOGLVertexBufferObject);

    // initialize VBO memory store
    GN_OGL_CHECK_R(glBindBuffer(GL_ARRAY_BUFFER, mOGLVertexBufferObject), false);
    GN_OGL_CHECK_R(glBufferData(GL_ARRAY_BUFFER, getDesc().length, NULL, mOGLUsage), false);

    // success
    ad.dismiss();
    return true;

    GN_UNGUARD;
}
