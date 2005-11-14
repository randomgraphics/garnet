#include "pch.h"
#include "oglTexture.h"

// *****************************************************************************
// local var/types/functions
// *****************************************************************************

//!
//! Automatically pop OGL attributes while out of scope.
//!
struct AutoAttribPopper
{
    //!
    //! Dtor
    //!
    ~AutoAttribPopper()
    {
        glPopAttrib();
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
//!
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
//! convert garnet color format to OpenGL format
//!
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
        case GN::gfx::FMT_RGBA_16_16_16_16:
            gl_internalformat  = GL_RGBA16;
            gl_format          = GL_RGBA;
            gl_type            = GL_UNSIGNED_SHORT;
            return true;

        case GN::gfx::FMT_RGBX_16_16_16_16:
            gl_internalformat  = GL_RGB16;
            gl_format          = GL_RGBA;
            gl_type            = GL_UNSIGNED_SHORT;
            return true;

        case GN::gfx::FMT_RGBA_8_8_8_8 :
            gl_internalformat  = GL_RGBA8;
            gl_format          = GL_RGBA;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_BGRA_8_8_8_8 :
            gl_internalformat  = GL_RGBA8;
            gl_format          = GL_BGRA_EXT;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_BGRX_8_8_8_8 :
            gl_internalformat  = GL_RGB8;
            gl_format          = GL_BGRA_EXT;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_RGB_8_8_8 :
            gl_internalformat  = GL_RGB8;
            gl_format          = GL_RGB;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_BGR_8_8_8 :
            gl_internalformat  = GL_RGB8;
            gl_format          = GL_BGR_EXT;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_BGRA_5_5_5_1 :
            gl_internalformat  = GL_RGB5_A1;
            gl_format          = GL_BGRA_EXT;
            gl_type            = GL_UNSIGNED_SHORT_5_5_5_1;
            return true;

        case GN::gfx::FMT_BGR_5_6_5 :
            gl_internalformat  = GL_RGB5;
            gl_format          = GL_BGR_EXT;
            gl_type            = GL_UNSIGNED_SHORT_5_6_5_REV;
            return true;

        case GN::gfx::FMT_UV_8_8 :
            if( GLEW_ATI_envmap_bumpmap )
            {
                gl_internalformat  = GL_DU8DV8_ATI;
                gl_format          = GL_DUDV_ATI;
                gl_type            = GL_BYTE;
            }
            else
            {
                GNGFX_WARN( "current hardware do not support "
                         "UV texture format!" );
                gl_internalformat  = 2;
                gl_format          = GL_RGBA;
                gl_type            = GL_BYTE;
            }
            return true;

        case GN::gfx::FMT_LA_16_16 :
            gl_internalformat  = GL_LUMINANCE16_ALPHA16;
            gl_format          = GL_LUMINANCE_ALPHA;
            gl_type            = GL_UNSIGNED_SHORT;
            return true;

        case GN::gfx::FMT_LA_8_8 :
            gl_internalformat  = GL_LUMINANCE8_ALPHA8;
            gl_format          = GL_LUMINANCE_ALPHA;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_L_16 :
            gl_internalformat  = GL_LUMINANCE16;
            gl_format          = GL_LUMINANCE;
            gl_type            = GL_UNSIGNED_SHORT;
            return true;

        case GN::gfx::FMT_L_8 :
            gl_internalformat  = GL_LUMINANCE8;
            gl_format          = GL_LUMINANCE;
            gl_type            = GL_UNSIGNED_BYTE;
            return true;

        case GN::gfx::FMT_A_8 :
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
    GNGFX_ERROR( "invalid or unsupported format '%s'!", GN::gfx::getClrFmtDesc(clrfmt).name );
    return false;
}

//!
//! generate 1D mipmaps
// ------------------------------------------------------------------------
static bool sGen1DMipmap( GLenum target,
                          GLsizei width,
                          GLint levels,
                          GLint internalformat,
                          GLenum format,
                          GLenum type )
{
    GN_GUARD;

    for( GLint i = 0; i < levels; ++i )
    {
        GN_ASSERT( width >= 1 );
        GN_OGL_CHECK_RV(
            glTexImage1D( target, i, internalformat, width, 0, format, type, 0 ),
            false );
        width >>= 1;
    }

    // success
    return true;

    GN_UNGUARD;
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
// OGLBasicTexture implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicTexture::init(
    TexType type,
    uint32_t sx, uint32_t sy, uint32_t sz,
    uint32_t levels,
    ClrFmt format,
    uint32_t usage )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( OGLBasicTexture, () );

    // determine pixelformat
    if( FMT_DEFAULT == format )
    {
        if( USAGE_DEPTH == usage )
        {
            format = FMT_D_32; // default depth format
        }
        else
        {
            format = FMT_RGBA_8_8_8_8; // default color format
        }
    }

    // store texture properties
    if( !setProperties( type,sx,sy,sz,levels,format,usage ) )
    { quit(); return selfOK(); }

    // determine gl texture type
    switch( getType() )
    {
        case TEXTYPE_1D   :
            mOGLTarget = GL_TEXTURE_1D;
            break;
        case TEXTYPE_2D   :
            mOGLTarget = GL_TEXTURE_2D;
            break;
        case TEXTYPE_3D   :
            if ( !GLEW_EXT_texture3D )
            {
                GNGFX_ERROR( "do not support 3D texture!" );
                quit(); return selfOK();
            }
            mOGLTarget = GL_TEXTURE_3D;
            break;
        case TEXTYPE_CUBE :
            if ( !GLEW_ARB_texture_cube_map )
            {
                GNGFX_ERROR( "do not support CUBE texture!" );
                quit(); return selfOK();
            }
            mOGLTarget = GL_TEXTURE_CUBE_MAP_ARB;
            break;
        default :
            GNGFX_ERROR( "invalid texture type!" );
            quit(); return selfOK();
    }

    // convert format to opengl paramaters
    if( !sColorFormat2OGL( mOGLInternalFormat,
                           mOGLFormat,
                           mOGLType,
                           mOGLCompressed,
                           getFormat() ) )
    { quit(); return selfOK(); }

    if( !deviceCreate() ) { quit(); return selfOK(); }
    if( !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicTexture::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicTexture::deviceCreate()
{
    GN_GUARD;

    // create new opengl texture object
    uint32_t sx, sy, sz;
    getSize( &sx, &sy, &sz );
    GLint levels = (GLint)getLevels();
    mOGLTexture = newOGLTexture(
        mOGLInternalFormat, sx, sy, sz, levels,
        mOGLFormat, mOGLType );
    if( 0 == mOGLTexture ) return false;

    // enable/disable mipmap autogeneration
    if( TEXTYPE_CUBE != getType() && GLEW_SGIS_generate_mipmap )
    {
        if( USAGE_AUTOGEN_MIPMAP & getUsage() )
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

    // successful
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicTexture::deviceDestroy()
{
    GN_GUARD;

    // check if locked
    if( isLocked() )
    {
        GNGFX_WARN( "call You are destroying a locked texture!" );
        unlock();
    }

    // delete opengl texture
    if (mOGLTexture) glDeleteTextures( 1, &mOGLTexture ), mOGLTexture = 0;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicTexture::setFilter( TexFilter min, TexFilter mag ) const
{
    GN_ASSERT( selfOK() );

    // bind myself as current texture
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
bool GN::gfx::OGLBasicTexture::privateLock2D(
    LockedRect & result, GLenum target, uint32_t level, const Recti * area, uint32_t flag )
{
    GN_GUARD_SLOW;

    // call basic lock
    if( !basicLock() ) return 0;
    AutoScope< Functor0<bool> > baiscUnlocker( makeFunctor( this,&OGLBasicTexture::basicUnlock ) );

    // bind self as current texture
    bind();

    // calculate locked area
    if( area )
    {
#if GN_DEBUG
        // make sure target area is totaly inside the texture
        {
            GLint texw, texh;
            GN_OGL_CHECK( glGetTexLevelParameteriv(
                    target, level, GL_TEXTURE_WIDTH,  &texw ) );
            GN_OGL_CHECK( glGetTexLevelParameteriv(
                    target, level, GL_TEXTURE_HEIGHT, &texh ) );
            GN_ASSERT( 0 <= area->x && area->x < texw &&
                       0 <= area->y && area->y < texh &&
                       0 <  area->w && (area->x+area->w) <= texw &&
                       0 <  area->h && (area->y+area->h) <= texh );
        }
#endif
        mLockedArea = *area;
    }
    else
    {
        mLockedArea.x = mLockedArea.y = 0;
        GN_OGL_CHECK( glGetTexLevelParameteriv(
                target, level, GL_TEXTURE_WIDTH,  &mLockedArea.w ) );
        GN_OGL_CHECK( glGetTexLevelParameteriv(
                target, level, GL_TEXTURE_HEIGHT, &mLockedArea.h ) );
    }

    // 计算pitch
    if( mOGLCompressed )
    {
        switch ( getFormat() )
        {
            case FMT_DXT1:
                result.rowBytes = ((mLockedArea.w + 3) >> 2) * 8;
                mLockedBytes = result.rowBytes * ((mLockedArea.h + 3) >> 2);
                break;

		    case FMT_DXT3:
		    case FMT_DXT5:
                result.rowBytes = ((mLockedArea.w + 3) >> 2) * 16;
                mLockedBytes = result.rowBytes * ((mLockedArea.h + 3) >> 2);
                break;

            default:
                GNGFX_ERROR( "unsupport compress format '%s'!", getClrFmtDesc(getFormat()).name );
                return false;
        }
    }
    else
    {
        GLint alignment;
        GN_OGL_CHECK( glGetIntegerv( GL_PACK_ALIGNMENT, &alignment ) );
        GN_ASSERT( isPowerOf2(alignment) ); // alignment必定是2^n
        size_t bpp = getClrFmtDesc(getFormat()).bits / 8;
        result.rowBytes = mLockedArea.w * bpp;
        // 将宽度值按照alignment的大小对齐
#define _GN_ALIGN(X,A) X = ( (X & -A) + (X & (A - 1) ? A : 0) )
        _GN_ALIGN(result.rowBytes,alignment);
#undef _GN_ALIGN
        mLockedBytes = result.rowBytes * mLockedArea.h;
    }

    // 分配缓冲区
    mLockedBuffer = new uint8_t[mLockedBytes];
    GN_ASSERT( mLockedBuffer );

    // 如果不是只写锁定，则读取当前的贴图内容到缓冲区中
    if( LOCK_RO & mLockedFlag )
    {
        GN_ASSERT_EX( 0, "目前不支持从贴图中读取数据!" );
    }

    // success
    mLockedTarget  = target;
    mLockedLevel   = level;
    mLockedFlag    = flag;
    result.data    = mLockedBuffer;
    baiscUnlocker.dismiss();
    return 1;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicTexture::privateUnlock2D()
{
    GN_GUARD_SLOW;

    // call basic unlock
    if( !basicUnlock() ) return;

    // if not a write locking, return directly
    if( 0 == ( LOCK_WO & mLockedFlag ) ) return;

    // bind myself as current texture
    bind();

    GN_ASSERT( mLockedBuffer );

    // 将缓冲区中的内容写入贴图
    if( mOGLCompressed )
    {
        if( GLEW_ARB_texture_compression )
        {
            GN_OGL_CHECK( glCompressedTexSubImage2DARB(
                mLockedTarget, mLockedLevel,
                mLockedArea.x, mLockedArea.y,
                mLockedArea.w, mLockedArea.h,
                mOGLInternalFormat,
                mLockedBytes, mLockedBuffer ) );
        }
        else
        {
            GNGFX_WARN( "do not support texture compression!" );
        }
    }
    else
    {
        GN_OGL_CHECK( glTexSubImage2D(
            mLockedTarget, mLockedLevel,
            mLockedArea.x, mLockedArea.y,
            mLockedArea.w, mLockedArea.h,
            mOGLFormat, mOGLType, mLockedBuffer ) );
    }

    // release mLockedBuffer
    delete [] mLockedBuffer; mLockedBuffer = 0;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// GN::gfx::OGLTex1D implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTex1D::setWrap( TexWrap s, TexWrap, TexWrap ) const
{
    GN_GUARD_SLOW;

    GN_OGL_CHECK( glBindTexture( GL_TEXTURE_1D, getOGLTexture() ) );

    setWrapS( s );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::OGLTex1D::lock1D( uint32_t /*level*/,
                                  uint32_t /*offset*/,
                                  uint32_t /*length*/,
                                  uint32_t /*flag*/ )
{
    GNGFX_ERROR( "no implementation" );
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTex1D::unlock()
{
    GNGFX_ERROR( "no implementation" );
}

//
//
// -----------------------------------------------------------------------------
GLuint GN::gfx::OGLTex1D::newOGLTexture(
    GLint   internalformat,
    GLsizei size_x,
    GLsizei /*size_y*/,
    GLsizei /*size_z*/,
    GLint   levels,
    GLenum  format,
    GLenum  type )
{
    GN_GUARD;

    // declare an auto-opengl-property-stack
    glPushAttrib( GL_CURRENT_BIT );
    AutoAttribPopper attrPopper();

    // generate new texture
    GLuint result;
    GN_OGL_CHECK_RV( glGenTextures(1, &result), 0 );
    AutoDeleteTexture autoDel( result );

    GN_OGL_CHECK( glBindTexture( GL_TEXTURE_1D, result ) );
    if( !sGen1DMipmap(
            GL_TEXTURE_1D, size_x, levels,
            internalformat, format, type ) )
        return 0;

    // success
    autoDel.dismiss();
    return result;

    GN_UNGUARD;
}

// *****************************************************************************
// GN::gfx::OGLTex2D implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTex2D::setWrap( TexWrap s, TexWrap t, TexWrap ) const
{
    GN_GUARD_SLOW;
    GN_OGL_CHECK( glBindTexture( GL_TEXTURE_2D, getOGLTexture() ) );
    setWrapS( s );
    setWrapT( t );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLTex2D::lock2D(
    LockedRect & result, uint32_t level, const Recti * area, uint32_t flag )
{
    GN_GUARD_SLOW;
    return privateLock2D( result, GL_TEXTURE_2D, level, area, flag );
    GN_UNGUARD_SLOW
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTex2D::unlock()
{
    GN_GUARD_SLOW;
    return privateUnlock2D();
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GLuint GN::gfx::OGLTex2D::newOGLTexture(
    GLint   internalformat,
    GLsizei size_x,
    GLsizei size_y,
    GLsizei /*size_z*/,
    GLint   levels,
    GLenum  format,
    GLenum  type )
{
    GN_GUARD;

    // declare an auto-opengl-property-stack
    glPushAttrib( GL_CURRENT_BIT );
    AutoAttribPopper attribPopper();

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

// *****************************************************************************
// GN::gfx::OGLTex3D implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTex3D::setWrap( TexWrap s, TexWrap t, TexWrap r ) const
{
    GN_GUARD_SLOW;
    GN_OGL_CHECK( glBindTexture( GL_TEXTURE_3D_EXT, getOGLTexture() ) );
    setWrapS( s );
    setWrapT( t );
    setWrapR( r );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLTex3D::lock3D( LockedBox &  /*result*/,
                                uint32_t     /*level*/,
                                const Boxi * /*box*/,
                                uint32_t     /*flag*/ )
{
    GNGFX_ERROR( "no implementation" );
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTex3D::unlock()
{
    GNGFX_ERROR( "no implementation" );
}

//
//
// -----------------------------------------------------------------------------
GLuint GN::gfx::OGLTex3D::newOGLTexture(
    GLint   /*internalformat*/,
    GLsizei /*size_x*/,
    GLsizei /*size_y*/,
    GLsizei /*size_z*/,
    GLint   /*levels*/,
    GLenum  /*format*/,
    GLenum  /*type*/ )
{
    GN_GUARD;

    GNGFX_ERROR( "no implementation" );
    return 0;

    GN_UNGUARD;
}

// *****************************************************************************
// GN::gfx::OGLTexCube implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexCube::setWrap( TexWrap s, TexWrap t, TexWrap ) const
{
    GN_GUARD_SLOW;
    GN_OGL_CHECK( glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, getOGLTexture() ) );
    setWrapS( s );
    setWrapT( t );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLTexCube::lockCube( LockedRect &  result,
                                    TexFace       face,
                                    uint32_t      level,
                                    const Recti * area,
                                    uint32_t      flag )
{
    GN_GUARD_SLOW;
    return privateLock2D( result, cubeface2GL(face), level, area, flag );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLTexCube::unlock()
{
    GN_GUARD_SLOW;
    privateUnlock2D();
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GLuint GN::gfx::OGLTexCube::newOGLTexture(
    GLint   internalformat,
    GLsizei size_x,
    GLsizei /*size_y*/,
    GLsizei /*size_z*/,
    GLint   levels,
    GLenum  format,
    GLenum  type )
{
    GN_GUARD;

    // declare an auto-opengl-property-stack
    glPushAttrib( GL_CURRENT_BIT );
    AutoAttribPopper attribPopper();

    // generate new texture
    GLuint result;
    GN_OGL_CHECK_RV( glGenTextures(1, &result), 0 );
    AutoDeleteTexture autoDel( result );

    if( !GLEW_ARB_texture_cube_map )
    {
        GNGFX_ERROR( "do not support cube map!" );
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
