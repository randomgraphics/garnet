#include "pch.h"
#include "oglTexture.h"
#include "oglRenderer.h"

// *****************************************************************************
// local var/types/functions
// *****************************************************************************

//!
//! Automatically pop OGL attributes while out of scope.
//!
struct AutoAttributeStack
{
    //!
    //! Ctor
    //!
    AutoAttributeStack( GLuint bit )
    {
        GN_OGL_CHECK( glPushAttrib( bit ) );
    }

    //!
    //! Dtor
    //!
    ~AutoAttributeStack()
    {
        GN_OGL_CHECK( glPopAttrib() );
    }
};

//!
//! Automatically delete OGL textures while out of scope.
//!
class AutoDeleteTexture
{
    GLuint mTex;

public:

    //!
    //! Ctor
    //!
    AutoDeleteTexture( GLuint tex ) : mTex(tex) {}

    //!
    //! Dtor
    //!
    ~AutoDeleteTexture()
    {
        if( mTex ) glDeleteTextures( 1, &mTex );
    }

    //!
    //! Dismiss the auto-deletetion
    //!
    void dismiss() { mTex = 0; }
};

//!
//! map filter type to opengl constant
// -----------------------------------------------------------------------------
static GN_INLINE GLenum sTexFilter2OGL( GN::gfx::TexFilter f )
{
    switch(f)
    {
    case GN::gfx::TEXFILTER_NEAREST : return GL_NEAREST;
    case GN::gfx::TEXFILTER_LINEAR  : return GL_LINEAR;
    case GN::gfx::TEXFILTER_N_MIP_N : return GL_NEAREST_MIPMAP_NEAREST;
    case GN::gfx::TEXFILTER_N_MIP_L : return GL_NEAREST_MIPMAP_LINEAR;
    case GN::gfx::TEXFILTER_L_MIP_N : return GL_LINEAR_MIPMAP_NEAREST;
    case GN::gfx::TEXFILTER_L_MIP_L : return GL_LINEAR_MIPMAP_LINEAR;
    default : GN_ASSERT_EX( 0, "invaid filter type!" );
        return GL_LINEAR;
    }
}

//!
//! map wrap mode to opengl constant
// -----------------------------------------------------------------------------
static GN_INLINE GLenum sTexWrap2OGL( GN::gfx::TexWrap w )
{
    if( GN::gfx::TEXWRAP_REPEAT == w ) return GL_REPEAT;
    else if( GN::gfx::TEXWRAP_CLAMP == w ) return GL_CLAMP;
    else if( GN::gfx::TEXWRAP_CLAMP_TO_EDGE == w )
    {
        if( GLEW_EXT_texture_edge_clamp )
            return GL_CLAMP_TO_EDGE_EXT;
        else if (GLEW_SGIS_texture_edge_clamp )
            return GL_CLAMP_TO_EDGE_SGIS;
        else
        {
            GN_ERROR( "do not support clamp to edge!" );
            return GL_CLAMP;
        }
    }
    else
    {
        GN_ASSERT_EX( 0, "invaid wrap type!" );
        return GL_REPEAT;
    }
}

//!
//! convert garnet color format to OpenGL format
// -----------------------------------------------------------------------------
static GN_INLINE bool sColorFormat2OGL(
    GLint & gl_internalformat,
    GLuint & gl_format,
    GLuint & gl_type,
    bool & gl_compressed,
    GN::gfx::ClrFmt clrfmt )
{
    gl_compressed = false;
    switch( clrfmt )
    {
        case GN::gfx::FMT_RGBA_16_16_16_16_UNORM:
            gl_internalformat  = GL_RGBA16;
            gl_format          = GL_RGBA;
            gl_type            = GL_UNSIGNED_SHORT;
            return true;

        case GN::gfx::FMT_RGBX_16_16_16_16_UNORM:
            gl_internalformat  = GL_RGB16;
            gl_format          = GL_RGBA;
            gl_type            = GL_UNSIGNED_SHORT;
            return true;

        case GN::gfx::FMT_RGBA_8_8_8_8_UNORM :
            gl_internalformat  = GL_RGBA8;
            gl_format          = GL_RGBA;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_BGRA_8_8_8_8_UNORM :
            gl_internalformat  = GL_RGBA8;
            gl_format          = GL_BGRA_EXT;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_BGRX_8_8_8_8_UNORM :
            gl_internalformat  = GL_RGB8;
            gl_format          = GL_BGRA_EXT;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_RGB_8_8_8_UNORM :
            gl_internalformat  = GL_RGB8;
            gl_format          = GL_RGB;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_BGR_8_8_8_UNORM :
            gl_internalformat  = GL_RGB8;
            gl_format          = GL_BGR_EXT;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_BGRA_5_5_5_1_UNORM :
            gl_internalformat  = GL_RGB5_A1;
            gl_format          = GL_BGRA_EXT;
            gl_type            = GL_UNSIGNED_SHORT_5_5_5_1;
            return true;

        case GN::gfx::FMT_BGR_5_6_5_UNORM :
            gl_internalformat  = GL_RGB5;
            gl_format          = GL_BGR_EXT;
            gl_type            = GL_UNSIGNED_SHORT_5_6_5_REV;
            return true;

        case GN::gfx::FMT_RG_8_8_SNORM :
            if( GLEW_ATI_envmap_bumpmap )
            {
                gl_internalformat  = GL_DU8DV8_ATI;
                gl_format          = GL_DUDV_ATI;
                gl_type            = GL_BYTE;
            }
            else
            {
                GN_WARN( "current hardware do not support "
                         "UV texture format!" );
                gl_internalformat  = 2;
                gl_format          = GL_RGBA;
                gl_type            = GL_BYTE;
            }
            return true;

        case GN::gfx::FMT_LA_16_16_UNORM :
            gl_internalformat  = GL_LUMINANCE16_ALPHA16;
            gl_format          = GL_LUMINANCE_ALPHA;
            gl_type            = GL_UNSIGNED_SHORT;
            return true;

        case GN::gfx::FMT_LA_8_8_UNORM :
            gl_internalformat  = GL_LUMINANCE8_ALPHA8;
            gl_format          = GL_LUMINANCE_ALPHA;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_L_16_UNORM :
            gl_internalformat  = GL_LUMINANCE16;
            gl_format          = GL_LUMINANCE;
            gl_type            = GL_UNSIGNED_SHORT;
            return true;

        case GN::gfx::FMT_L_8_UNORM :
            gl_internalformat  = GL_LUMINANCE8;
            gl_format          = GL_LUMINANCE;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_A_8_UNORM :
            gl_internalformat  = GL_ALPHA8;
            gl_format          = GL_ALPHA;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_D_16 :
            gl_internalformat  = GL_DEPTH_COMPONENT16_ARB;
            gl_format          = GL_DEPTH_COMPONENT;
            gl_type            = GL_FLOAT;
            return true;

        case GN::gfx::FMT_D_24 :
            gl_internalformat  = GL_DEPTH_COMPONENT24_ARB;
            gl_format          = GL_DEPTH_COMPONENT;
            gl_type            = GL_FLOAT;
            return true;

        case GN::gfx::FMT_D_32 :
            gl_internalformat  = GL_DEPTH_COMPONENT32_ARB;
            gl_format          = GL_DEPTH_COMPONENT;
            gl_type            = GL_FLOAT;
            return true;

        case GN::gfx::FMT_DXT1 :
            if( GLEW_ARB_texture_compression &&
                GLEW_EXT_texture_compression_s3tc )
            {
                gl_internalformat  = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                gl_format          = GL_RGBA;
                gl_type            = GL_UNSIGNED_BYTE;
                gl_compressed      = true;
                return true;
            }
            break;

        case GN::gfx::FMT_DXT3 :
            if( GLEW_ARB_texture_compression &&
                GLEW_EXT_texture_compression_s3tc )
            {
                gl_internalformat  = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                gl_format          = GL_RGBA;
                gl_type            = GL_UNSIGNED_BYTE;
                gl_compressed      = true;
                return true;
            }
            break;

        case GN::gfx::FMT_DXT5 :
            if( GLEW_ARB_texture_compression &&
                GLEW_EXT_texture_compression_s3tc )
            {
                gl_internalformat  = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                gl_format          = GL_RGBA;
                gl_type            = GL_UNSIGNED_BYTE;
                gl_compressed      = true;
                return true;
            }
            break;

        default : break;
    }

    // failed
    GN_ERROR( "invalid or unsupported format '%s'!", GN::gfx::clrFmt2Str(clrfmt) );
    return false;
}

//!
//! generate 2D mipmaps
// ------------------------------------------------------------------------
static bool sGen2DMipmap( GLenum target,
                          GLsizei width, GLsizei height,
                          GLint levels,
                          GLint internalformat,
                          GLenum format, GLenum type )
{
    GN_GUARD;

    GN_ASSERT( width > 0 && height > 0 );

    for( GLint i = 0; i < levels; ++i )
    {
        GN_OGL_CHECK_RV(
            glTexImage2D( target, i, internalformat, width, height, 0, format, type, 0 ),
            false );
        if( width > 1 ) width >>= 1;
        if( height > 1 ) height >>= 1;
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
GLuint sNew2DTexture(
    GLint   internalformat,
    GLsizei size_x,
    GLsizei size_y,
    GLint   levels,
    GLenum  format,
    GLenum  type )
{
    GN_GUARD;

    // declare an auto-opengl-property-stack
    AutoAttributeStack aas( GL_CURRENT_BIT );

    // generate new texture
    GLuint result;
    GN_OGL_CHECK_RV( glGenTextures(1, &result), 0 );
    AutoDeleteTexture autoDel( result );

    GN_OGL_CHECK( glBindTexture( GL_TEXTURE_2D, result ) );
    if( !sGen2DMipmap(
            GL_TEXTURE_2D, size_x, size_y, levels,
            internalformat, format, type ) )
        return 0;

    // success
    autoDel.dismiss();
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static GLuint sNew3DTexture(
    GLint   /*internalformat*/,
    GLsizei /*size_x*/,
    GLsizei /*size_y*/,
    GLsizei /*size_z*/,
    GLint   /*levels*/,
    GLenum  /*format*/,
    GLenum  /*type*/ )
{
    GN_GUARD;

    GN_ERROR( "no implementation" );
    return 0;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static GLuint sNewCubeTexture(
    GLint   internalformat,
    GLsizei size_x,
    GLint   levels,
    GLenum  format,
    GLenum  type )
{
    GN_GUARD;

    // declare an auto-opengl-property-stack
    AutoAttributeStack aas( GL_CURRENT_BIT );

    // generate new texture
    GLuint result;
    GN_OGL_CHECK_RV( glGenTextures(1, &result), 0 );
    AutoDeleteTexture autoDel( result );

    if( !GLEW_ARB_texture_cube_map )
    {
        GN_ERROR( "do not support cube map!" );
        return 0;
    }

    GN_OGL_CHECK( glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, result ) );
    for( GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB;
         face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB;
         ++face )
    {
        if( !sGen2DMipmap(
                face, size_x, size_x, levels,
                internalformat, format, type ) )
            return 0;
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
bool GN::gfx::OGLTexture::init( TextureDesc desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( OGLTexture, () );

    // determine pixelformat
    if( FMT_DEFAULT == desc.format )
    {
        if( TEXUSAGE_DEPTH == desc.usage )
        {
            desc.format = FMT_D_32; // default depth format
        }
        else
        {
            desc.format = FMT_BGRA_8_8_8_8_UNORM; // default color format
        }
    }

    // store texture properties
    if( !setDesc( desc ) ) { quit(); return selfOK(); }

    // determine gl texture type
    switch( getDesc().type )
    {
        case TEXTYPE_1D   :
        case TEXTYPE_2D   :
            mOGLTarget = GL_TEXTURE_2D;
            break;
        case TEXTYPE_3D   :
            if ( !GLEW_EXT_texture3D )
            {
                GN_ERROR( "do not support 3D texture!" );
                quit(); return selfOK();
            }
            mOGLTarget = GL_TEXTURE_3D;
            break;
        case TEXTYPE_CUBE :
            if ( !GLEW_ARB_texture_cube_map )
            {
                GN_ERROR( "do not support CUBE texture!" );
                quit(); return selfOK();
            }
            mOGLTarget = GL_TEXTURE_CUBE_MAP_ARB;
            break;
        default :
            GN_ERROR( "invalid texture type!" );
            quit(); return selfOK();
    }

    // convert format to opengl paramaters
    if( !sColorFormat2OGL( mOGLInternalFormat,
                           mOGLFormat,
                           mOGLType,
                           mOGLCompressed,
                           getDesc().format ) )
    { quit(); return selfOK(); }

    // create new opengl texture object
    const TextureDesc & desc = getDesc();
    switch( getDesc().type )
    {
        case TEXTYPE_1D :
        case TEXTYPE_2D :
            mOGLTexture = sNew2DTexture(
                mOGLInternalFormat, desc.width, desc.height, desc.levels,
                mOGLFormat, mOGLType );
            break;

        case TEXTYPE_3D :
            mOGLTexture = sNew3DTexture(
                mOGLInternalFormat, desc.width, desc.height, desc.depth, desc.levels,
                mOGLFormat, mOGLType );
            break;

        case TEXTYPE_CUBE :
            mOGLTexture = sNewCubeTexture(
                mOGLInternalFormat, desc.width, desc.levels,
                mOGLFormat, mOGLType );
            break;

        case TEXTYPE_STACK :
            GN_ERROR( "OpenGL does not support STACK texture." );
            mOGLTexture = 0;
            break;

        default:
            GN_UNEXPECTED();
            mOGLTexture = 0;
    }
    if( 0 == mOGLTexture ) { quit(); return selfOK(); }

    // enable/disable mipmap autogeneration
    if( TEXTYPE_CUBE != getDesc().type && GLEW_SGIS_generate_mipmap )
    {
        if( TEXUSAGE_AUTOGEN_MIPMAP & desc.usage )
        {
            GN_OGL_CHECK( glTexParameteri( mOGLTarget,GL_GENERATE_MIPMAP_SGIS, GL_TRUE) );
        }
        else
        {
            GN_OGL_CHECK( glTexParameteri( mOGLTarget,GL_GENERATE_MIPMAP_SGIS, GL_FALSE) );
        }
    }

    // setup default filters and wrap modes
    GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
    GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
    GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_WRAP_S, GL_REPEAT ) );
    GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_WRAP_T, GL_REPEAT ) );
    if( GLEW_EXT_texture3D )
    {
        GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_WRAP_R, GL_REPEAT ) );
    }

    // call user-defined content loader
    if( !getLoader().empty() )
    {
        if( !getLoader()( *this ) ) { quit(); return selfOK(); };
    }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexture::quit()
{
    GN_GUARD;

    // check if locked
    if( isLocked() )
    {
        GN_WARN( "call You are destroying a locked texture!" );
        unlock();
    }

    // delete opengl texture
    if (mOGLTexture) glDeleteTextures( 1, &mOGLTexture ), mOGLTexture = 0;


    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::Vector3<uint32_t> GN::gfx::OGLTexture::getMipSize( size_t level ) const
{
    GN_GUARD_SLOW;

    bind();

    GLint sx, sy, sz;

    GN_OGL_CHECK( glGetTexLevelParameteriv(
        GL_TEXTURE_2D, (GLint)level, GL_TEXTURE_WIDTH, &sx ) );
    GN_OGL_CHECK( glGetTexLevelParameteriv(
        GL_TEXTURE_2D, (GLint)level, GL_TEXTURE_HEIGHT, &sy ) );
    if( TEXTYPE_3D == getDesc().type )
    {
        GN_OGL_CHECK( glGetTexLevelParameteriv(
            GL_TEXTURE_3D_EXT, (GLint)level, GL_TEXTURE_DEPTH_EXT, &sz ) );
    }
    else sz = 1;

    // success
    return Vector3<uint32_t>(
        (uint32_t)sx,
        (uint32_t)sy,
        (uint32_t)sz );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexture::setFilter( TexFilter min, TexFilter mag ) const
{
    GN_ASSERT( selfOK() );

    bind();

    if( mFilters[0] != min )
    {
        mFilters[0] = min;
        GN_OGL_CHECK( glTexParameteri(
            mOGLTarget,
            GL_TEXTURE_MIN_FILTER,
            sTexFilter2OGL( min) ) );
    }

    if( mFilters[1] != mag )
    {
        mFilters[1] = mag;
        GN_OGL_CHECK( glTexParameteri( mOGLTarget,
            GL_TEXTURE_MAG_FILTER,
            sTexFilter2OGL( mag ) ) );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexture::setWrap( TexWrap s, TexWrap t, TexWrap r ) const
{
    GN_GUARD_SLOW;

    bind();

    if( mWraps[0] != s )
    {
        mWraps[0] = s;

        GN_OGL_CHECK( glTexParameteri(
            mOGLTarget,
            GL_TEXTURE_WRAP_S,
            sTexWrap2OGL( s ) ) );
    }

    if( mWraps[1] != t )
    {
        mWraps[1] = t;

        GN_OGL_CHECK( glTexParameteri(
            mOGLTarget,
            GL_TEXTURE_WRAP_T,
            sTexWrap2OGL( t ) ) );
    }

    if( TEXTYPE_3D == getDesc().type && mWraps[2] != r )
    {
        mWraps[2] = r;

        GN_OGL_CHECK( glTexParameteri(
            mOGLTarget,
            GL_TEXTURE_WRAP_R,
            sTexWrap2OGL( r ) ) );
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLTexture::lock(
    TexLockedResult & result,
    size_t face,
    size_t level,
    const Boxi * area,
    LockFlag flag )
{
    GN_GUARD_SLOW;

    // call basic lock
    if( !basicLock( face, level, area, flag, mLockedArea ) ) return false;
    AutoScope< Functor0<bool> > basicUnlocker( makeFunctor(*this,&OGLTexture::basicUnlock) );

    // 计算pitch
    if( mOGLCompressed )
    {
        switch ( getDesc().format )
        {
            case FMT_DXT1:
                result.rowBytes = ((mLockedArea.w + 3) >> 2) * 8;
                result.sliceBytes = result.rowBytes * ((mLockedArea.h + 3) >> 2);
                mLockedBytes = result.sliceBytes * mLockedArea.d;
                break;

		    case FMT_DXT3:
		    case FMT_DXT5:
                result.rowBytes = ((mLockedArea.w + 3) >> 2) * 16;
                result.sliceBytes = result.rowBytes * ((mLockedArea.h + 3) >> 2);
                mLockedBytes = result.sliceBytes * mLockedArea.d;
                break;

            default:
                GN_ERROR( "unsupport compress format '%s'!", clrFmt2Str(getDesc().format) );
                return false;
        }
    }
    else
    {
        GLint alignment;
        GN_OGL_CHECK( glGetIntegerv( GL_PACK_ALIGNMENT, &alignment ) );
        GN_ASSERT( isPowerOf2(alignment) ); // alignment必定是2^n
        size_t bpp = getClrFmtDesc(getDesc().format).bits / 8;
        // 将宽度值按照alignment的大小对齐
#define _GN_ALIGN(X,A) X = ( (X & -A) + (X & (A - 1) ? A : 0) )
        _GN_ALIGN(result.rowBytes,alignment);
#undef _GN_ALIGN
        result.rowBytes = mLockedArea.w * bpp;
        result.sliceBytes = result.rowBytes * mLockedArea.h;
        mLockedBytes = result.sliceBytes * mLockedArea.d;
    }

    // 分配缓冲区
    mLockedBuffer = new uint8_t[mLockedBytes];
    GN_ASSERT( mLockedBuffer );

    // 如果不是只写锁定，则读取当前的贴图内容到缓冲区中
    if( LOCK_RO == flag || LOCK_RW == flag )
    {
        GN_WARN( "目前不支持从贴图中读取数据!" );
    }

    // success
    mLockedTarget  = TEXTYPE_CUBE == getDesc().type ? OGLTexture::sCubeface2OGL(face) : mOGLTarget;
    mLockedLevel   = level;
    mLockedFlag    = flag;
    result.data    = mLockedBuffer;
    basicUnlocker.dismiss();
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexture::unlock()
{
    GN_GUARD_SLOW;

    // call basic unlock
    if( !basicUnlock() ) return;

    // do nothing for read-only lock
    if( LOCK_RO == mLockedFlag ) return;

    // bind myself as current texture
    bind();

    GN_ASSERT( mLockedBuffer );

    if( TEXTYPE_3D == getDesc().type )
    {
        GN_UNIMPL_WARNING();
    }
    else
    {
        // 将缓冲区中的内容写入贴图
        if( mOGLCompressed )
        {
            if( GLEW_ARB_texture_compression )
            {
                GN_OGL_CHECK( glCompressedTexSubImage2DARB(
                    mLockedTarget, (GLint)mLockedLevel,
                    mLockedArea.x, mLockedArea.y,
                    mLockedArea.w, mLockedArea.h,
                    mOGLInternalFormat,
                    (GLsizei)mLockedBytes, mLockedBuffer ) );
            }
            else
            {
                GN_WARN( "do not support texture compression!" );
            }
        }
        else
        {
            GN_OGL_CHECK( glTexSubImage2D(
                mLockedTarget, (GLint)mLockedLevel,
                mLockedArea.x, mLockedArea.y,
                mLockedArea.w, mLockedArea.h,
                mOGLFormat, mOGLType, mLockedBuffer ) );
        }
    }

    // release mLockedBuffer
    delete [] mLockedBuffer;
    mLockedBuffer = 0;

    GN_UNGUARD_SLOW;
}
