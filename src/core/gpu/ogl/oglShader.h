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
    void setUniformArray(const GpuProgramUniformParameterDesc * array, uint32 count, uint32 stride) {
        mUniformArray       = array;
        mUniformCount       = count;
        mUniformArrayStride = stride;
    }

    void setTextureArray(const GpuProgramTextureParameterDesc * array, uint32 count, uint32 stride) {
        mTextureArray       = array;
        mTextureCount       = count;
        mTextureArrayStride = stride;
    }

    void setAttributeArray(const GpuProgramAttributeParameterDesc * array, uint32 count, uint32 stride) {
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
    uint8             index;
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
        static uint64 counter = 1;
        mID                   = counter++;
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
    uint64 uniqueID() const { return mID; }

    bool getBindingDesc(OGLVertexBindingDesc & result, uint32 attributeIndex) const;

    void enable() const {
        GN_GUARD_SLOW;
        glUseProgram(mProgram);
        GN_UNGUARD_SLOW;
    }

    void applyUniforms(const Uniform * const * uniforms, uint32 count) const;

    void applyTextures(const TextureBinding * textures, uint32 count) const;

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
        GpuProgramUniformParameterDesc               uniformDesc;  ///< uniform parameter description
        GpuProgramTextureParameterDesc               textureDesc;  ///< textureparameter description
        GLenum                                       type;         ///< uniform type
        GLsizei                                      count;        ///< uniform count
        GLint                                        location;     ///< uniform location
        uint32                                       size;         ///< uniform size in bytes
        StrA                                         name;         ///< uniform name
        mutable WeakRef<const Uniform>               lastUniform;  ///< pointer to last uniform parameter
        mutable sint32                               lastStamp;    ///< update time stamp of the last uniform parameter
        mutable AutoInitializer<uint32, (uint32) -1> lastTexStage; ///< last texture stage associated to this parameter
    };

    ///
    /// GLSL vertex attribute description
    ///
    struct GLSLAttributeDesc {
        GpuProgramAttributeParameterDesc desc; ///< attribute parameter description
        StrA                             name; ///< attribute name
        OGLVertexSemantic                semanticName;
        uint8                            semanticIndex;
    };

    // GLSL program
    GLuint mProgram = 0;

    // uniforms
    DynaArray<GLSLUniformOrTextureDesc, uint32> mUniforms;

    // textures
    DynaArray<GLSLUniformOrTextureDesc, uint32> mTextures;

    // attributes
    DynaArray<GLSLAttributeDesc, uint32> mAttributes;

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
