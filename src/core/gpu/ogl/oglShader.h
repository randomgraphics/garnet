#ifndef __GN_GFXOGL_OGLSHADER_H__
#define __GN_GFXOGL_OGLSHADER_H__
// *****************************************************************************
/// \file
/// \brief   OGL shader classes
/// \author  chenlee (2005.12.6)
// *****************************************************************************

#include "oglResource.h"
#include "../common/basicShader.h"

namespace GN {
namespace gfx {
class OglGpuProgramParameterDesc : public GpuProgramParameterDesc {
public:
    void setUniformArray(const GpuProgramUniformParameterDesc * array, uint32_t count, uint32_t stride) {
        mUniformArray       = array;
        mUniformCount       = count;
        mUniformArrayStride = stride;
    }

    void setTextureArray(const GpuProgramTextureParameterDesc * array, uint32_t count, uint32_t stride) {
        mTextureArray       = array;
        mTextureCount       = count;
        mTextureArrayStride = stride;
    }

    void setAttributeArray(const GpuProgramAttributeParameterDesc * array, uint32_t count, uint32_t stride) {
        mAttributeArray       = array;
        mAttributeCount       = count;
        mAttributeArrayStride = stride;
    }
};

// *************************************************************************
// Basic program object
// *************************************************************************

enum OGLVertexSemantic {
    VERTEX_SEMANTIC_VERTEX,
    VERTEX_SEMANTIC_NORMAL,
    VERTEX_SEMANTIC_COLOR,
    VERTEX_SEMANTIC_FOG,
    VERTEX_SEMANTIC_TEXCOORD,
    VERTEX_SEMANTIC_ATTRIBUTE,
};

struct OGLVertexBindingDesc {
    OGLVertexSemantic semantic;
    uint8_t           index;
};

// *************************************************************************
// GLSL program
// *************************************************************************

///
/// GLSL program class
///
class OGLGpuProgram : public GpuProgram, public OGLResource, public StdClass {
    GN_DECLARE_STDCLASS(OGLGpuProgram, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    OGLGpuProgram(OGLGpu & r): OGLResource(r) {
        static uint64_t counter = 1;
        mID                     = counter++;
        clear();
    }
    virtual ~OGLGpuProgram() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(const GpuProgramDesc & desc);
    void quit();

private:
    void clear() { mProgram = 0; }
    //@}

    // ********************************
    // from GpuProgram
    // ********************************

public:
    virtual const GpuProgramParameterDesc & getParameterDesc() const { return mParamDesc; }

    // ********************************
    // from OGLGpuProgram
    // ********************************
public:
    ///
    /// return non-zero unique shader ID.
    ///
    uint64_t uniqueID() const { return mID; }

    bool getBindingDesc(OGLVertexBindingDesc & result, uint32_t attributeIndex) const;

    void enable() const {
        GN_GUARD_SLOW;
        glUseProgram(mProgram);
        GN_UNGUARD_SLOW;
    }

    void applyUniforms(const Uniform * const * uniforms, uint32_t count) const;

    void applyTextures(const TextureBinding * textures, uint32_t count) const;

    // ********************************
    // private variables
    // ********************************
private:
    uint64_t mID;
    StrA     mName; // for debugging and logging only.

    ///
    /// GLSL uniform parameter description
    ///
    struct GLSLUniformOrTextureDesc {
        GpuProgramUniformParameterDesc                   uniformDesc;  ///< uniform parameter description
        GpuProgramTextureParameterDesc                   textureDesc;  ///< textureparameter description
        GLenum                                           type;         ///< uniform type
        GLsizei                                          count;        ///< uniform count
        GLint                                            location;     ///< uniform location
        uint32_t                                         size;         ///< uniform size in bytes
        StrA                                             name;         ///< uniform name
        mutable WeakRef<const Uniform>                   lastUniform;  ///< pointer to last uniform parameter
        mutable int32_t                                  lastStamp;    ///< update time stamp of the last uniform parameter
        mutable AutoInitializer<uint32_t, (uint32_t) -1> lastTexStage; ///< last texture stage associated to this parameter
    };

    ///
    /// GLSL vertex attribute description
    ///
    struct GLSLAttributeDesc {
        GpuProgramAttributeParameterDesc desc {}; ///< attribute parameter description
        StrA                             name {}; ///< attribute name
        OGLVertexSemantic                semanticName {};
        uint8_t                          semanticIndex {};
    };

    // GLSL program
    GLuint mProgram = 0;

    // uniforms
    DynaArray<GLSLUniformOrTextureDesc, uint32_t> mUniforms;

    // textures
    DynaArray<GLSLUniformOrTextureDesc, uint32_t> mTextures;

    // attributes
    DynaArray<GLSLAttributeDesc, uint32_t> mAttributes;

    // parameter descriptor
    OglGpuProgramParameterDesc mParamDesc;

    // ********************************
    // private functions
    // ********************************
private:
    bool enumParameters();
    bool enumAttributes();
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLSHADER_H__
