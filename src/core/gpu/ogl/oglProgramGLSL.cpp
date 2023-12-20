#include "pch.h"
#include "oglGpu.h"
#include "oglShader.h"
#include "oglTexture.h"
#include "garnet/GNogl.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.GpuProgramGLSL");

// *****************************************************************************
// Local function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline uint32 sGetUniformSize(GLenum type) {
    switch (type) {
    case GL_FLOAT:
        return sizeof(float);
    case GL_FLOAT_VEC2:
        return sizeof(float) * 2;
    case GL_FLOAT_VEC3:
        return sizeof(float) * 3;
    case GL_FLOAT_VEC4:
        return sizeof(float) * 4;
    case GL_INT:
    case GL_BOOL:
        return sizeof(int);
    case GL_INT_VEC2:
    case GL_BOOL_VEC2:
        return sizeof(int) * 2;
    case GL_INT_VEC3:
    case GL_BOOL_VEC3:
        return sizeof(int) * 3;
    case GL_INT_VEC4:
    case GL_BOOL_VEC4:
        return sizeof(int) * 4;
    case GL_FLOAT_MAT2:
        return sizeof(float) * 4;
    case GL_FLOAT_MAT3:
        return sizeof(float) * 9;
    case GL_FLOAT_MAT4:
        return sizeof(float) * 16;
    case GL_SAMPLER_1D:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_3D:
    case GL_SAMPLER_CUBE:
    case GL_SAMPLER_1D_SHADOW:
    case GL_SAMPLER_2D_SHADOW:
    case GL_SAMPLER_2D_RECT:
    case GL_SAMPLER_2D_RECT_SHADOW:
        GN_UNEXPECTED_EX("Texture parameter is not supposed to be handled by this function.");
        return sizeof(GLint);

    default:
        GN_UNEXPECTED_EX("invalid uniform type");
        return 0;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sIsTextureUniform(GLenum type) { return GL_SAMPLER_1D <= type && type <= GL_SAMPLER_2D_RECT_SHADOW; }

//
//
// -----------------------------------------------------------------------------
static bool sGetOglVertexSemantic(OGLVertexSemantic & semanticName, uint8 & semanticIndex, const char * attributeName, GLuint attributeLocation) {
    semanticIndex = 0;

    if (0 == str::compare("gl_Vertex", attributeName)) {
        semanticName = VERTEX_SEMANTIC_VERTEX;
    } else if (0 == str::compare("gl_Normal", attributeName)) {
        semanticName = VERTEX_SEMANTIC_NORMAL;
    } else if (0 == str::compare("gl_Color", attributeName)) {
        semanticName = VERTEX_SEMANTIC_COLOR;
    } else if (0 == str::compare("gl_SecondaryColor", attributeName)) {
        semanticName  = VERTEX_SEMANTIC_COLOR;
        semanticIndex = 1;
    } else if (0 == str::compare("gl_FogCoord", attributeName)) {
        semanticName = VERTEX_SEMANTIC_FOG;
    } else if (0 == str::compare("gl_MultiTexCoord0", attributeName)) {
        semanticName = VERTEX_SEMANTIC_TEXCOORD;
    } else if (0 == str::compare("gl_MultiTexCoord1", attributeName)) {
        semanticName  = VERTEX_SEMANTIC_TEXCOORD;
        semanticIndex = 1;
    } else if (0 == str::compare("gl_MultiTexCoord2", attributeName)) {
        semanticName  = VERTEX_SEMANTIC_TEXCOORD;
        semanticIndex = 2;
    } else if (0 == str::compare("gl_MultiTexCoord3", attributeName)) {
        semanticName  = VERTEX_SEMANTIC_TEXCOORD;
        semanticIndex = 3;
    } else if (0 == str::compare("gl_MultiTexCoord4", attributeName)) {
        semanticName  = VERTEX_SEMANTIC_TEXCOORD;
        semanticIndex = 4;
    } else if (0 == str::compare("gl_MultiTexCoord5", attributeName)) {
        semanticName  = VERTEX_SEMANTIC_TEXCOORD;
        semanticIndex = 5;
    } else if (0 == str::compare("gl_MultiTexCoord6", attributeName)) {
        semanticName  = VERTEX_SEMANTIC_TEXCOORD;
        semanticIndex = 6;
    } else if (0 == str::compare("gl_MultiTexCoord7", attributeName)) {
        semanticName  = VERTEX_SEMANTIC_TEXCOORD;
        semanticIndex = 7;
    } else if ((GLuint) -1 != attributeLocation) {
        // this is general vertex attribute
        semanticName  = VERTEX_SEMANTIC_ATTRIBUTE;
        semanticIndex = (uint8) attributeLocation;
    } else {
        // never reach here
        GN_UNEXPECTED();
        return false;
    }

    // success
    return true;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgram::init(const GpuProgramDesc & desc) {
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // standard init procedure
    GN_STDCLASS_INIT();

    GN_ASSERT(GpuProgramLanguage::GLSL == desc.lang);

    mName = desc.name ? StrA(desc.name) : str::format("program %llu", mID);

#define COMPILE_SHADER(x, type)                                                   \
    if (desc.x.source) {                                                          \
        auto s = ogl::loadShaderFromString(desc.x.source, 0, type, mName.data()); \
        if (0 == s) return failure();                                             \
        shaders.append(s);                                                        \
    }

    DynaArray<GLuint> shaders;
    COMPILE_SHADER(vs, GL_VERTEX_SHADER);
    COMPILE_SHADER(ps, GL_FRAGMENT_SHADER);
    mProgram = ogl::linkProgram(shaders.data(), shaders.size(), mName.data());
    ;
    if (0 == mProgram) return failure();

    // enumerate parameters (textures and uniforms)
    if (!enumParameters()) return failure();

    // enumerate attributes
    if (!enumAttributes()) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgram::quit() {
    GN_GUARD;

    mUniforms.clear();
    mTextures.clear();
    mAttributes.clear();

    if (mProgram) glDeleteProgram(mProgram), mProgram = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgram::getBindingDesc(OGLVertexBindingDesc & result, uint32 attributeIndex) const {
    if (attributeIndex >= mAttributes.size()) {
        GN_ERROR(sLogger)("Invalid attribute index.");
        result.index    = 255;
        result.semantic = (OGLVertexSemantic) -1;
        return false;
    }

    const GLSLAttributeDesc & desc = mAttributes[attributeIndex];

    result.semantic = desc.semanticName;
    result.index    = desc.semanticIndex;

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgram::applyUniforms(const Uniform * const * uniforms, uint32 count) const {
    GN_GUARD_SLOW;

    if (count != mUniforms.size()) { GN_ERROR(sLogger)("Current GPU program requires %d uniforms. But %d are provided.", mUniforms.size(), count); }

    if (count > mUniforms.size()) count = mUniforms.size();

    for (uint32 i = 0; i < count; ++i) {
        // get uniform pointer
        const SysMemUniform * uniform = (const SysMemUniform *) uniforms[i];
        if (NULL == uniform) {
            const GpuProgramUniformParameterDesc & ud = mParamDesc.uniforms[i];
            GN_ERROR(sLogger)("Null uniform pointer for GPU program uniform parameter #%d: %s.", i, ud.name);
            continue;
        }

        // get uniform descriptor
        const GLSLUniformOrTextureDesc & desc = mUniforms[i];

        // If the uniform is currently bind to OpenGL, skip it.
        if (uniform == desc.lastUniform && uniform->getTimeStamp() == desc.lastStamp) { continue; }

        // update time stamp
        desc.lastUniform.set(uniform);
        desc.lastStamp = uniform->getTimeStamp();

        // check parameter size
        if (getGpu().paramCheckEnabled()) {
            if (uniform->size() != desc.size) {
                GN_WARN(sLogger)("parameter %s: value size(%d) differs from size defined in shader code(%d).", desc.name.rawptr(), uniform->size(), desc.size);
            }
        }

        switch (desc.type) {
        case GL_FLOAT:
            glUniform1fv(desc.location, desc.count, (GLfloat *) uniform->getval());
            break;

        case GL_FLOAT_VEC2:
            glUniform2fv(desc.location, desc.count, (GLfloat *) uniform->getval());
            break;

        case GL_FLOAT_VEC3:
            glUniform3fv(desc.location, desc.count, (GLfloat *) uniform->getval());
            break;

        case GL_FLOAT_VEC4:
            glUniform4fv(desc.location, desc.count, (GLfloat *) uniform->getval());
            break;

        case GL_INT:
        case GL_BOOL:
            glUniform1iv(desc.location, desc.count, (GLint *) uniform->getval());
            break;

        case GL_INT_VEC2:
        case GL_BOOL_VEC2:
            glUniform2iv(desc.location, desc.count, (GLint *) uniform->getval());
            break;

        case GL_INT_VEC3:
        case GL_BOOL_VEC3:
            glUniform3iv(desc.location, desc.count, (GLint *) uniform->getval());
            break;

        case GL_INT_VEC4:
        case GL_BOOL_VEC4:
            glUniform4iv(desc.location, desc.count, (GLint *) uniform->getval());
            break;

        case GL_FLOAT_MAT2:
            glUniformMatrix2fv(desc.location, desc.count, true, (GLfloat *) uniform->getval());
            break;

        case GL_FLOAT_MAT3:
            glUniformMatrix3fv(desc.location, desc.count, true, (GLfloat *) uniform->getval());
            break;

        case GL_FLOAT_MAT4:
            glUniformMatrix4fv(desc.location, desc.count, true, (GLfloat *) uniform->getval());
            break;

        case GL_SAMPLER_1D:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
        case GL_SAMPLER_CUBE:
        case GL_SAMPLER_1D_SHADOW:
        case GL_SAMPLER_2D_SHADOW:
        case GL_SAMPLER_2D_RECT:
        case GL_SAMPLER_2D_RECT_SHADOW:
            GN_UNEXPECTED_EX("sampler/texture is not handled by this function.");
            break;
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgram::applyTextures(const TextureBinding * textures, uint32 count) const {
    GN_GUARD_SLOW;

    OGLGpu & r = getGpu();

    // determine effective texture count
    if (count > mTextures.size()) { count = (uint32) mTextures.size(); }
    GN_ASSERT(count <= r.caps().maxTextures);

    // apply textures to OpenGL, one by one
    uint32 i;
    for (i = 0; i < count; ++i) {
        const TextureBinding & b = textures[i];

        // get texture descriptor
        const GLSLUniformOrTextureDesc & desc = mTextures[i];

        if (b.texture) {
            r.chooseTextureStage(i);

            const OGLTexture * ogltexture = safeCastPtr<const OGLTexture>(b.texture.rawptr());

            // bind sampler. TODO: filter redundant sampler.
            ogltexture->setSampler(b.sampler);

            // bind texture to current stage
            ogltexture->bind();

            // bind current texture stage to the GPU program parameter
            if (desc.lastTexStage != i) {
                GLint s = (GLint) i;
                GN_OGL_CHECK(glUniform1iv(desc.location, desc.count, &s));
                desc.lastTexStage = i;
            }
        }
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgram::enumParameters() {
    GN_GUARD;

    // get parameter count
    GLint numParameters;
    GN_OGL_CHECK_R(glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &numParameters), false);

    // get maxinum length of parameter name;
    GLint maxLength;
    GN_OGL_CHECK_R(glGetProgramiv(mProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength), false);

    // enumerate all parameters
    char * nameptr = (char *) alloca(maxLength + 1);
    mUniforms.clear();
    for (GLint i = 0; i < numParameters; ++i) {
        GLSLUniformOrTextureDesc u;

        GN_OGL_CHECK_R(glGetActiveUniform(mProgram, i, maxLength, NULL, &u.count, &u.type, nameptr), false);
        nameptr[maxLength] = 0;

        // remove "[0]" suffix from array uniform
        size_t namelen = str::length(nameptr);
        if (namelen > 3 && 0 == str::compare("[0]", &nameptr[namelen - 3])) { nameptr[namelen - 3] = 0; }

        GN_VTRACE(sLogger)("%s: found GLSL uniform %s", mName.data(), nameptr);

        GN_OGL_CHECK_R(u.location = glGetUniformLocation(mProgram, nameptr), false);

        u.name = nameptr;

        if (sIsTextureUniform(u.type)) {
            mTextures.append(u);
        } else {
            u.size = sGetUniformSize(u.type) * u.count;
            mUniforms.append(u);
        }
    }

    // initialize name and size arrays
    for (uint32 i = 0; i < mUniforms.size(); ++i) {
        GLSLUniformOrTextureDesc & u = mUniforms[i];
        u.uniformDesc.name           = u.name.rawptr();
        u.uniformDesc.size           = u.size;
    }
    for (uint32 i = 0; i < mTextures.size(); ++i) {
        GLSLUniformOrTextureDesc & t = mTextures[i];
        t.textureDesc.name           = t.name.rawptr();
    }

    // update parameter descriptor
    if (mUniforms.size() > 0) { mParamDesc.setUniformArray(&mUniforms[0].uniformDesc, (uint32) mUniforms.size(), sizeof(GLSLUniformOrTextureDesc)); }
    if (mTextures.size() > 0) { mParamDesc.setTextureArray(&mTextures[0].textureDesc, (uint32) mTextures.size(), sizeof(GLSLUniformOrTextureDesc)); }

    // check for texture capability
    OGLGpu & r = getGpu();
    if (mTextures.size() > r.caps().maxTextures) {
        GN_ERROR(sLogger)("%s: the program requires more textures than current hardware supports.", mName.data());
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgram::enumAttributes() {
    // get attribute count
    GLint numAttributes;
    GN_OGL_CHECK_R(glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTES, &numAttributes), false);

    // get maxinum length of attribute name;
    GLint maxLength;
    GN_OGL_CHECK_R(glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength), false);

    // enumerate all attributes
    char * nameptr = (char *) alloca(maxLength + 1);
    mAttributes.clear();
    for (GLint i = 0; i < numAttributes; ++i) {
        GLsizei unusedCount;
        GLenum  unusedType;
        GN_OGL_CHECK_R(glGetActiveAttrib(mProgram, i, maxLength, NULL, &unusedCount, &unusedType, nameptr), false);
        nameptr[maxLength] = 0;
        if (0 == *nameptr) {
            // ignore non-named attributes.
            GN_WARN(sLogger)("Attribute %d has no name.", i);
            continue;
        }

        GLint location;
        GN_OGL_CHECK_R(location = glGetAttribLocation(mProgram, nameptr), false);

        GLSLAttributeDesc a;
        if (!sGetOglVertexSemantic(a.semanticName, a.semanticIndex, nameptr, location)) return false;
        if (a.semanticName != VERTEX_SEMANTIC_ATTRIBUTE) {
            GN_ERROR(sLogger)("%s: client side pointer is not supported anymore.", mName.data());
            return false;
        }

        a.name = nameptr;
        mAttributes.append(a);
    }

    // initialize name and format arrays
    for (uint32 i = 0; i < mAttributes.size(); ++i) {
        GLSLAttributeDesc & a = mAttributes[i];
        a.desc.name           = a.name.rawptr();
    }

    // update parameter descriptor
    if (mAttributes.size() > 0) { mParamDesc.setAttributeArray(&mAttributes[0].desc, (uint32) mAttributes.size(), sizeof(GLSLAttributeDesc)); }

    return true;
}
