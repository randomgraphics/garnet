#include "pch.h"
#include "oglTexture.h"
#include "oglGpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.Texture");

// *****************************************************************************
// local var/types/functions
// *****************************************************************************

///
/// Automatically delete OGL textures while out of scope.
///
class AutoDeleteTexture {
    GLuint mTex;

public:
    ///
    /// Ctor
    ///
    AutoDeleteTexture(GLuint tex): mTex(tex) {}

    ///
    /// Dtor
    ///
    ~AutoDeleteTexture() {
        if (mTex) glDeleteTextures(1, &mTex);
    }

    ///
    /// Dismiss the auto-deletetion
    ///
    void dismiss() { mTex = 0; }
};

static const GLenum INVALID_DIMENSION = 0xFFFFFFFF;

///
/// determine texture dimension based on texture size, return 0xFFFFFFFF if failed.
// -----------------------------------------------------------------------------
static inline GLenum sDetermineTextureDimension(uint32 faces, uint32 width, uint32 height, uint32 depth) {
    if (depth > 1) {
        // 3D texture
        if (faces > 1) {
            GN_ERROR(sLogger)("OpenGL does not support 3D texture array.");
            return INVALID_DIMENSION;
        }

        if (!GLEW_EXT_texture3D) {
            GN_ERROR(sLogger)("Current hardware does not 3D texture. (EXT_texture3D)");
            return INVALID_DIMENSION;
        }

        return GL_TEXTURE_3D_EXT;
    } else if (faces > 1) {
        // cube or array texture
        if (6 == faces && width == height) {
            if (!GLEW_ARB_texture_cube_map) {
                GN_ERROR(sLogger)("Current hardware does not support cube texture. (ARB_texture_cube_map)");
                return INVALID_DIMENSION;
            } else {
                return GL_TEXTURE_CUBE_MAP_ARB;
            }
        } else {
            GN_ERROR(sLogger)("Array texture is not implemented yet.");
            return INVALID_DIMENSION;
        }
    } else if (height > 1) {
        // 2D texture
        return GL_TEXTURE_2D;
    } else {
        // 1D texture
        return GL_TEXTURE_1D;
    }
}

///
/// convert garnet color format to OpenGL format
// -----------------------------------------------------------------------------
static inline bool sColorFormat2OGL(GLint & gl_internalformat, GLuint & gl_format, GLuint & gl_type, bool & gl_compressed, GN::gfx::PixelFormat clrfmt,
                                    GN::gfx::TextureUsage usage) {
    using namespace GN::gfx;

    gl_compressed = false;

    if (PixelFormat::RGBA_32_32_32_32_FLOAT() == clrfmt) {
        if (!GLEW_ARB_texture_float) {
            GN_WARN(sLogger)("current hardware do not support floating point texture format!");
            return false;
        }
        gl_internalformat = GL_RGBA32F_ARB;
        gl_format         = GL_RGBA;
        gl_type           = GL_FLOAT;
        return true;
    } else if (PixelFormat::RG_32_32_FLOAT() == clrfmt) {
        if (!GLEW_ARB_texture_float) {
            GN_WARN(sLogger)("current hardware do not support floating point texture format!");
            return false;
        }
        gl_internalformat = GL_LUMINANCE_ALPHA32F_ARB;
        gl_format         = GL_RGBA;
        gl_type           = GL_FLOAT;
        return true;
    } else if (PixelFormat::RGBA_16_16_16_16_FLOAT() == clrfmt) {
        if (!GLEW_ARB_texture_float) {
            GN_WARN(sLogger)("current hardware do not support floating point texture format!");
            return false;
        }
        gl_internalformat = GL_RGBA16F_ARB;
        gl_format         = GL_RGBA;
        gl_type           = GL_FLOAT;
        return true;
    } else if (PixelFormat::RG_16_16_FLOAT() == clrfmt) {
        if (!GLEW_ARB_texture_float) {
            GN_WARN(sLogger)("current hardware do not support floating point texture format!");
            return false;
        }
        gl_internalformat = GL_LUMINANCE_ALPHA16F_ARB;
        gl_format         = GL_RGBA;
        gl_type           = GL_FLOAT;
        return true;
    } else if (PixelFormat::RGBA_16_16_16_16_UNORM() == clrfmt) {
        gl_internalformat = GL_RGBA16;
        gl_format         = GL_RGBA;
        gl_type           = GL_UNSIGNED_SHORT;
        return true;
    } else if (PixelFormat::RGBX_16_16_16_16_UNORM() == clrfmt) {
        gl_internalformat = GL_RGB16;
        gl_format         = GL_RGBA;
        gl_type           = GL_UNSIGNED_SHORT;
        return true;
    } else if (PixelFormat::RG_16_16_UNORM() == clrfmt) {
        gl_internalformat = GL_LUMINANCE_ALPHA;
        gl_format         = GL_RGBA;
        gl_type           = GL_UNSIGNED_SHORT;
        return true;
    } else if (PixelFormat::RGBA_8_8_8_8_UNORM() == clrfmt) {
        gl_internalformat = GL_RGBA8;
        gl_format         = GL_RGBA;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::BGRA_8_8_8_8_UNORM() == clrfmt) {
        gl_internalformat = GL_RGBA8;
        gl_format         = GL_BGRA_EXT;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::RGBX_8_8_8_8_UNORM() == clrfmt) {
        gl_internalformat = GL_RGB8;
        gl_format         = GL_RGBA;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::BGRX_8_8_8_8_UNORM() == clrfmt) {
        gl_internalformat = GL_RGB8;
        gl_format         = GL_BGRA_EXT;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::RGB_8_8_8_UNORM() == clrfmt) {
        gl_internalformat = GL_RGB8;
        gl_format         = GL_RGB;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::BGR_8_8_8_UNORM() == clrfmt) {
        gl_internalformat = GL_RGB8;
        gl_format         = GL_BGR_EXT;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::BGRA_5_5_5_1_UNORM() == clrfmt) {
        gl_internalformat = GL_RGB5_A1;
        gl_format         = GL_BGRA_EXT;
        gl_type           = GL_UNSIGNED_SHORT_5_5_5_1;
        return true;
    } else if (PixelFormat::BGR_5_6_5_UNORM() == clrfmt) {
        gl_internalformat = GL_RGB5;
        gl_format         = GL_BGR_EXT;
        gl_type           = GL_UNSIGNED_SHORT_5_6_5_REV;
        return true;
    } else if (PixelFormat::RG_8_8_SNORM() == clrfmt) {
        if (GLEW_ATI_envmap_bumpmap) {
            gl_internalformat = GL_DU8DV8_ATI;
            gl_format         = GL_DUDV_ATI;
            gl_type           = GL_BYTE;
        } else {
            GN_WARN(sLogger)("current hardware do not support UV texture format!");
            gl_internalformat = 2;
            gl_format         = GL_RGBA;
            gl_type           = GL_BYTE;
        }
        return true;
    } else if (PixelFormat::LA_16_16_UNORM() == clrfmt) {
        gl_internalformat = GL_LUMINANCE16_ALPHA16;
        gl_format         = GL_LUMINANCE_ALPHA;
        gl_type           = GL_UNSIGNED_SHORT;
        return true;
    } else if (PixelFormat::LA_8_8_UNORM() == clrfmt) {
        gl_internalformat = GL_LUMINANCE8_ALPHA8;
        gl_format         = GL_LUMINANCE_ALPHA;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::L_16_UNORM() == clrfmt) {
        gl_internalformat = GL_LUMINANCE16;
        gl_format         = GL_LUMINANCE;
        gl_type           = GL_UNSIGNED_SHORT;
        return true;
    } else if (PixelFormat::R_8_UNORM() == clrfmt) {
        gl_internalformat = 1;
        gl_format         = GL_RED;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;

    } else if (PixelFormat::L_8_UNORM() == clrfmt) {
        gl_internalformat = GL_LUMINANCE8;
        gl_format         = GL_LUMINANCE;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::A_8_UNORM() == clrfmt) {
        gl_internalformat = GL_ALPHA8;
        gl_format         = GL_ALPHA;
        gl_type           = GL_UNSIGNED_BYTE;
        return true;
    } else if (PixelFormat::R_16_UINT() == clrfmt) {
        if (TextureUsage::DEPTH_RENDER_TARGET == usage) {
            if (!GLEW_ARB_depth_texture) {
                GN_ERROR(sLogger)("does not support GL_ARB_depth_texture.");
                return false;
            }
            gl_internalformat = GL_DEPTH_COMPONENT;
            gl_format         = GL_DEPTH_COMPONENT;
            gl_type           = GL_UNSIGNED_SHORT;
            return true;
        } else {
            GN_ERROR(sLogger)("integer texture is not supported yet.");
            return false;
        }
    } else if (PixelFormat::R_32_UINT() == clrfmt) {
        if (TextureUsage::DEPTH_RENDER_TARGET == usage) {
            if (!GLEW_ARB_depth_texture) {
                GN_ERROR(sLogger)("does not support GL_ARB_depth_texture.");
                return false;
            }
            gl_internalformat = GL_DEPTH_COMPONENT;
            gl_format         = GL_DEPTH_COMPONENT;
            gl_type           = GL_UNSIGNED_INT;
            return true;
        } else {
            GN_ERROR(sLogger)("integer texture is not supported yet.");
            return false;
        }
    } else if (PixelFormat::DXT1_UNORM() == clrfmt) {
        if (GLEW_ARB_texture_compression && GLEW_EXT_texture_compression_s3tc) {
            gl_internalformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            gl_format         = GL_RGBA;
            gl_type           = GL_UNSIGNED_BYTE;
            gl_compressed     = true;
            return true;
        }
    } else if (PixelFormat::DXT3_UNORM() == clrfmt) {
        if (GLEW_ARB_texture_compression && GLEW_EXT_texture_compression_s3tc) {
            gl_internalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            gl_format         = GL_RGBA;
            gl_type           = GL_UNSIGNED_BYTE;
            gl_compressed     = true;
            return true;
        }
    } else if (PixelFormat::DXT5_UNORM() == clrfmt) {
        if (GLEW_ARB_texture_compression && GLEW_EXT_texture_compression_s3tc) {
            gl_internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            gl_format         = GL_RGBA;
            gl_type           = GL_UNSIGNED_BYTE;
            gl_compressed     = true;
            return true;
        }
    }

    // failed
    GN_ERROR(sLogger)("invalid or unsupported format '%s'!", clrfmt.toString().c_str());
    return false;
}

///
/// map wrap mode to opengl constant
// -----------------------------------------------------------------------------
static inline GLint sTexWrap2OGL(uint32 wrap) {
    switch (wrap) {
    case GN::gfx::SamplerDesc::ADDRESS_REPEAT:
        return GL_REPEAT;

    case GN::gfx::SamplerDesc::ADDRESS_CLAMP:
        return GL_CLAMP;

    case GN::gfx::SamplerDesc::ADDRESS_CLAMP_BORDER:
        if (GLEW_EXT_texture_edge_clamp) {
            return GL_CLAMP_TO_EDGE_EXT;
        } else if (GLEW_SGIS_texture_edge_clamp) {
            return GL_CLAMP_TO_EDGE_SGIS;
        } else {
            GN_WARN(sLogger)("Current hardware does not support clamp to edge feature. (EXT_texture_edge_clamp or SGIS_texture_edge_clamp)");
            return GL_CLAMP;
        }

    case GN::gfx::SamplerDesc::ADDRESS_MIRROR:
        if (GLEW_ARB_texture_mirrored_repeat) {
            return GL_MIRRORED_REPEAT_ARB;
        } else {
            GN_WARN(sLogger)("Current hardware does not support texture mirror (ARB_texture_mirrored_repeat)!");
            return GL_REPEAT;
        }

    default: {
        GN_ERROR(sLogger)("invaid texture address mode : %d!", wrap);
        return GL_REPEAT;
    }
    }
}

///
/// generate 2D mipmaps
// ------------------------------------------------------------------------
static bool sGen2DMipmap(GLenum target, GLsizei width, GLsizei height, GLint levels, GLint internalformat, GLenum format, GLenum type) {
    GN_GUARD;

    GN_ASSERT(width > 0 && height > 0);

    for (GLint i = 0; i < levels; ++i) {
        GN_OGL_CHECK_R(glTexImage2D(target, i, internalformat, width, height, 0, format, type, 0), false);
        if (width > 1) width >>= 1;
        if (height > 1) height >>= 1;
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// OGL texture creation routines
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GLuint sNew1DTexture(GLint internalformat, GLsizei size_x, GLint levels, GLenum format, GLenum type) {
    GN_GUARD;

    // generate new texture
    GLuint result;
    GN_OGL_CHECK_R(glGenTextures(1, &result), 0);
    AutoDeleteTexture autoDel(result);

    // generate mipmaps
    GN_OGL_CHECK(glBindTexture(GL_TEXTURE_1D, result));
    for (GLint i = 0; i < levels; ++i) {
        GN_ASSERT(size_x > 0);
        GN_OGL_CHECK_R(glTexImage1D(GL_TEXTURE_1D, i, internalformat, size_x, 0, format, type, 0), false);
        size_x >>= 1;
    }

    // success
    autoDel.dismiss();
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static GLuint sNew2DTexture(GLint internalformat, GLsizei size_x, GLsizei size_y, GLint levels, GLenum format, GLenum type) {
    GN_GUARD;

    // generate new texture
    GLuint result;
    GN_OGL_CHECK_R(glGenTextures(1, &result), 0);
    AutoDeleteTexture autoDel(result);

    GN_OGL_CHECK(glBindTexture(GL_TEXTURE_2D, result));
    if (!sGen2DMipmap(GL_TEXTURE_2D, size_x, size_y, levels, internalformat, format, type)) return 0;

    // success
    autoDel.dismiss();
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static GLuint sNew3DTexture(GLint /*internalformat*/, GLsizei /*size_x*/, GLsizei /*size_y*/, GLsizei /*size_z*/, GLint /*levels*/, GLenum /*format*/,
                            GLenum /*type*/) {
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
static GLuint sNewCubeTexture(GLint internalformat, GLsizei size_x, GLint levels, GLenum format, GLenum type) {
    GN_GUARD;

    // generate new texture
    GLuint result;
    GN_OGL_CHECK_R(glGenTextures(1, &result), 0);
    AutoDeleteTexture autoDel(result);

    if (!GLEW_ARB_texture_cube_map) {
        GN_ERROR(sLogger)("do not support cube map!");
        return 0;
    }

    GN_OGL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, result));
    for (GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB; face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB; ++face) {
        if (!sGen2DMipmap(face, size_x, size_x, levels, internalformat, format, type)) return 0;
    }

    // success
    autoDel.dismiss();
    return result;

    GN_UNGUARD;
}

// *****************************************************************************
// OGLTexture implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLTexture::init(const TextureDesc & inputDesc) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    OGLAutoAttribStack autoAttribStack; // auto-restore OGL states

    // store texture properties
    if (!setDesc(inputDesc)) return failure();

    // Note: this descriptor may differ with the input one.
    const TextureDesc & texdesc = getDesc();

    // determine texture dimension
    mTarget = sDetermineTextureDimension(texdesc.faces, texdesc.width, texdesc.height, texdesc.depth);
    if (INVALID_DIMENSION == mTarget) return failure();

    // convert format to opengl paramaters
    if (!sColorFormat2OGL(mOGLInternalFormat, mOGLFormat, mOGLType, mOGLCompressed, texdesc.format, texdesc.usage)) return failure();

    // create new opengl texture object
    switch (mTarget) {
    case GL_TEXTURE_1D:
        mOGLTexture = sNew1DTexture(mOGLInternalFormat, texdesc.width, texdesc.levels, mOGLFormat, mOGLType);
        break;

    case GL_TEXTURE_2D:
        mOGLTexture = sNew2DTexture(mOGLInternalFormat, texdesc.width, texdesc.height, texdesc.levels, mOGLFormat, mOGLType);
        break;

    case GL_TEXTURE_3D:
        mOGLTexture = sNew3DTexture(mOGLInternalFormat, texdesc.width, texdesc.height, texdesc.depth, texdesc.levels, mOGLFormat, mOGLType);
        break;

    case GL_TEXTURE_CUBE_MAP_ARB:
        mOGLTexture = sNewCubeTexture(mOGLInternalFormat, texdesc.width, texdesc.levels, mOGLFormat, mOGLType);
        break;

    default:
        GN_UNEXPECTED();
        mOGLTexture = 0;
    }
    if (0 == mOGLTexture) return failure();

    // setup mipmap size array
    for (uint32 i = 0; i < texdesc.levels; ++i) {
        GLint sx, sy, sz;
        switch (mTarget) {
        case GL_TEXTURE_1D:
            GN_OGL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_1D, (GLint) i, GL_TEXTURE_WIDTH, &sx));
            sy = 1;
            sz = 1;
            break;

        case GL_TEXTURE_2D:
            GN_OGL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, (GLint) i, GL_TEXTURE_WIDTH, &sx));
            GN_OGL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, (GLint) i, GL_TEXTURE_HEIGHT, &sy));
            sz = 1;
            break;

        case GL_TEXTURE_3D:
            GN_OGL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_3D_EXT, (GLint) i, GL_TEXTURE_WIDTH, &sx));
            GN_OGL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_3D_EXT, (GLint) i, GL_TEXTURE_HEIGHT, &sy));
            GN_OGL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_3D_EXT, (GLint) i, GL_TEXTURE_DEPTH_EXT, &sz));
            break;

        case GL_TEXTURE_CUBE_MAP_ARB:
            GN_OGL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, (GLint) i, GL_TEXTURE_WIDTH, &sx));
            GN_OGL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, (GLint) i, GL_TEXTURE_HEIGHT, &sy));
            sz = 1;
            break;

        default:
            GN_UNEXPECTED();
            return failure();
        }

        setMipSize(i, sx, sy, sz);
    }

    // setup default filters and wrap modes
    mSampler.clear();
    setSampler(mSampler, true);

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexture::quit() {
    GN_GUARD;

    // delete opengl texture
    if (mOGLTexture) glDeleteTextures(1, &mOGLTexture), mOGLTexture = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexture::updateMipmap(uint32 face, uint32 level, const Box<uint32> * area, uint32 rowPitch, uint32 slicePitch, const void * inputData,
                                       SurfaceUpdateFlag flag) {
    // check update parameters,
    Box<uint32> clippedArea;
    if (!validateUpdateParameters(face, level, area, flag, clippedArea)) return;

    // Auto-restore texture binding when exiting this function.
    OGLAutoAttribStack autoAttribStack(GL_TEXTURE_BIT);

    // bind the texture as current texture
    bind();

    // setup pixel store parameters

    // Note: GL_PACK_ROW_LENGTH defines number of pixels in a row.
    size_t bpp = getDesc().format.bitsPerPixel();
    glPixelStorei(GL_UNPACK_ROW_LENGTH, (GLint) (rowPitch * 8 / bpp));

    GLint alignment;
    if (rowPitch & 1)
        alignment = 1;
    else if (rowPitch & 2)
        alignment = 2;
    else if (rowPitch & 4)
        alignment = 4;
    else
        alignment = 8;
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    // TODO: setup slice pitch parameter

    if (mOGLCompressed) {
        if (GLEW_ARB_texture_compression) {
            switch (mTarget) {
            case GL_TEXTURE_1D:
                GN_OGL_CHECK(glCompressedTexSubImage1DARB(GL_TEXTURE_1D, (GLint) level, (GLsizei) clippedArea.x, (GLsizei) clippedArea.w, mOGLInternalFormat,
                                                          (GLsizei) slicePitch, inputData));
                break;

            case GL_TEXTURE_2D:
                GN_OGL_CHECK(glCompressedTexSubImage2DARB(GL_TEXTURE_2D, (GLint) level, (GLsizei) clippedArea.x, (GLsizei) clippedArea.y,
                                                          (GLsizei) clippedArea.w, (GLsizei) clippedArea.h, mOGLInternalFormat, (GLsizei) slicePitch,
                                                          inputData));
                break;

            case GL_TEXTURE_3D_EXT:
                GN_OGL_CHECK(glCompressedTexSubImage3DARB(GL_TEXTURE_3D, (GLint) level, (GLsizei) clippedArea.x, (GLsizei) clippedArea.y,
                                                          (GLsizei) clippedArea.z, (GLsizei) clippedArea.w, (GLsizei) clippedArea.h, (GLsizei) clippedArea.d,
                                                          mOGLInternalFormat, (GLsizei) (slicePitch * clippedArea.d), inputData));
                break;

            case GL_TEXTURE_CUBE_MAP_ARB:
                GN_OGL_CHECK(glCompressedTexSubImage2DARB(OGLTexture::sCubeface2OGL(face), (GLint) level, (GLsizei) clippedArea.x, (GLsizei) clippedArea.y,
                                                          (GLsizei) clippedArea.w, (GLsizei) clippedArea.h, mOGLInternalFormat, (GLsizei) slicePitch,
                                                          inputData));
                break;

            default:
                GN_ERROR(sLogger)("Unsupported dimension.");
            };
        } else {
            GN_ERROR(sLogger)("Current hardware does not support ARB_texture_compression extension.");
        }
    } else {
        switch (mTarget) {
        case GL_TEXTURE_1D:
            GN_OGL_CHECK(glTexSubImage1D(GL_TEXTURE_1D, (GLint) level, (GLsizei) clippedArea.x, (GLsizei) clippedArea.w, mOGLFormat, mOGLType, inputData));
            break;

        case GL_TEXTURE_2D: {
            GN_OGL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, (GLint) level, (GLsizei) clippedArea.x, (GLsizei) clippedArea.y, (GLsizei) clippedArea.w,
                                         (GLsizei) clippedArea.h, mOGLFormat, mOGLType, inputData));
            break;
        }

        case GL_TEXTURE_3D_EXT:
            GN_OGL_CHECK(glTexSubImage3DEXT(GL_TEXTURE_3D_EXT, (GLint) level, (GLsizei) clippedArea.x, (GLsizei) clippedArea.y, (GLsizei) clippedArea.z,
                                            (GLsizei) clippedArea.w, (GLsizei) clippedArea.h, (GLsizei) clippedArea.d, mOGLFormat, mOGLType, inputData));
            break;

        case GL_TEXTURE_CUBE_MAP_ARB:
            GN_OGL_CHECK(glTexSubImage2D(OGLTexture::sCubeface2OGL(face), (GLint) level, (GLsizei) clippedArea.x, (GLsizei) clippedArea.y,
                                         (GLsizei) clippedArea.w, (GLsizei) clippedArea.h, mOGLFormat, mOGLType, inputData));
            break;

        default:
            GN_ERROR(sLogger)("Unsupported dimension.");
        };
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexture::readMipmap(uint32, uint32, MipmapData &) { GN_UNIMPL(); }

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexture::setSampler(const SamplerDesc & samp, bool forceUpdate) const {
    GN_GUARD_SLOW;

    if (!forceUpdate && samp == mSampler) return;

    mSamplerDirty = true;

    // MAG filter
    mOGLFilters[1] = GL_NEAREST + samp.filterMag;

    // min and mip filter
    if (1 == getDesc().levels) {
        // the texture has no mipmap, ignore mipmap filter
        mOGLFilters[0] = GL_NEAREST + samp.filterMag;
    } else {
        // Filter Combination                          Bilinear Filtering (Near)   Bilinear Filtering (Far)    Mipmapping
        // (MAG_FILTER/MIN_FILTER)
        // GL_NEAREST / GL_NEAREST_MIPMAP_NEAREST      Off                         Off                         Standard
        // GL_NEAREST / GL_LINEAR_MIPMAP_NEAREST       Off                         On                          Standard
        // GL_NEAREST / GL_NEAREST_MIPMAP_LINEAR       Off                         Off                         Use trilinear filtering
        // GL_NEAREST / GL_LINEAR_MIPMAP_LINEAR        Off                         On                          Use trilinear filtering
        // GL_NEAREST / GL_NEAREST                     Off                         Off                         None
        // GL_NEAREST / GL_LINEAR                      Off                         On                          None
        // GL_LINEAR  / GL_NEAREST_MIPMAP_NEAREST      On                          Off                         Standard
        // GL_LINEAR  / GL_LINEAR_MIPMAP_NEAREST       On                          On                          Standard
        // GL_LINEAR  / GL_NEAREST_MIPMAP_LINEAR       On                          Off                         Use trilinear filtering
        // GL_LINEAR  / GL_LINEAR_MIPMAP_LINEAR        On                          On                          Use trilinear filtering
        // GL_LINEAR  / GL_NEAREST                     On                          Off                         None
        // GL_LINEAR  / GL_LINEAR                      On                          On                          None

        static const GLint sConvertTable[4] = {
            GL_NEAREST_MIPMAP_NEAREST, // min = POINT , mip = POINT
            GL_NEAREST_MIPMAP_LINEAR,  // min = POINT , mip = LINEAR
            GL_LINEAR_MIPMAP_NEAREST,  // min = LINEAR, mip = POINT
            GL_LINEAR_MIPMAP_LINEAR,   // min = LINEAR, mip = LINEAR
        };

        mOGLFilters[0] = sConvertTable[samp.filterMin * 2 + samp.filterMag];
    }

    // wraps
    mOGLWraps[0] = sTexWrap2OGL(samp.addressU);
    mOGLWraps[1] = sTexWrap2OGL(samp.addressV);
    mOGLWraps[2] = sTexWrap2OGL(samp.addressW);

    GN_UNGUARD_SLOW;
}
