#ifndef __GN_GFXOGL_OGLVTXBUF_H__
#define __GN_GFXOGL_OGLVTXBUF_H__
// *****************************************************************************
/// \file
/// \brief   OGL vertex buffer classes
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "oglResource.h"
#include "../common/basicSurface.h"

namespace GN {
namespace gfx {
///
/// Vertex buffer that use GL_ARB_vertex_buffer_object.
///
class OGLVtxBufVBO : public BasicVtxBuf, public OGLResource, public StdClass {
    GN_DECLARE_STDCLASS(OGLVtxBufVBO, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    OGLVtxBufVBO(OGLGpu & r): OGLResource(r) { clear(); }
    virtual ~OGLVtxBufVBO() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(const VtxBufDesc & desc);
    void quit();

private:
    void clear() {
        mOGLVertexBufferObject = 0;
        mOGLUsage              = 0;
    }
    //@}

    // ********************************
    // from VtxBuf
    // ********************************
public:
    virtual void update(uint32 offset, uint32 length, const void * data, SurfaceUpdateFlag flag);
    virtual void readback(DynaArray<uint8> & data);

    // ********************************
    // public OGLBasicVtxBuf
    // ********************************
public:
    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, mOGLVertexBufferObject); }

    // ********************************
    // private variables
    // ********************************
private:
    GLuint mOGLVertexBufferObject;
    GLenum mOGLUsage;

    // ********************************
    // private functions
    // ********************************
private:
    bool createVBO();
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLVTXBUF_H__
