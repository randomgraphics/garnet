#include "pch.h"
#include "oglRenderer.h"
//#include "oglShader.h"
//#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"
#include "oglTexture.h"
//#include "oglSampler.h"

// *****************************************************************************
// local function
// *****************************************************************************

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::resourceInit()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "Not _ALL_ graphics resouces are released!" );
        return false;
    }

#ifdef HAS_CG_OGL
    if( !mCgContext.init() ) return false;
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::resourceQuit()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // check for non-released resources
    if( !mResourceList.empty() )
    {
        GN_ERROR(sLogger)( "All graphics resouces have to be released, before renderer is destroied!" );
        GN_UNEXPECTED();
        for( std::list<OGLResource*>::iterator i = mResourceList.begin(); i != mResourceList.end(); ++i )
        {
            const OGLResource * r = *i;
            GN_ERROR(sLogger)( "0x%p", r );
        }
    }

#ifdef HAS_CG_OGL
    mCgContext.quit();
#endif

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::CompiledShaderBlob *
GN::gfx::OGLRenderer::compileShader( const ShaderDesc & )
{
    GN_GUARD;

    return NULL;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Shader *
GN::gfx::OGLRenderer::createShader( const CompiledShaderBlob & )
{
    GN_GUARD;

    return NULL;

    /*
    switch( type )
    {
        case SHADER_VS :
            switch( lang )
            {
                case LANG_OGL_ARB:
                {
                    AutoRef<OGLVtxShaderARB> p( new OGLVtxShaderARB(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                case LANG_OGL_GLSL:
                {
                    AutoRef<OGLVtxShaderGLSL> p( new OGLVtxShaderGLSL(*this) );
                    if( !p->init( code, hints ) ) return 0;
                    return p.detach();
                }
#ifdef HAS_CG_OGL
                case LANG_CG:
                {
                    AutoRef<OGLVtxShaderCg> p( new OGLVtxShaderCg(*this) );
                    if( !p->init( code, hints ) ) return 0;
                    return p.detach();
                }
#endif
                default:
                    GN_ERROR(sLogger)( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

        case SHADER_PS :
            switch( lang )
            {
                case LANG_OGL_ARB:
                {
                    AutoRef<OGLPxlShaderARB> p( new OGLPxlShaderARB(*this) );
                    if( !p->init( code ) ) return 0;
                    return p.detach();
                }

                case LANG_OGL_GLSL:
                {
                    AutoRef<OGLPxlShaderGLSL> p( new OGLPxlShaderGLSL(*this) );
                    if( !p->init( code, hints ) ) return 0;
                    return p.detach();
                }
#ifdef HAS_CG_OGL
                case LANG_CG:
                {
                    AutoRef<OGLPxlShaderCg> p( new OGLPxlShaderCg(*this) );
                    if( !p->init( code, hints ) ) return 0;
                    return p.detach();
                }
#endif
                default:
                    GN_ERROR(sLogger)( "unsupport shading language : %s", shadingLanguage2Str(lang) );
                    return 0;
            }

        case SHADER_GS:
            GN_ERROR(sLogger)( "OpenGL does not support GeometryShader." );
            return 0;

        default:
            GN_UNEXPECTED(); // program should not reach here
            GN_ERROR(sLogger)( "invalid shader type: %d", type );
            return 0;
    }
    */

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::OGLRenderer::createTexture( const TextureDesc & desc )
{
    GN_GUARD;

    AutoRef<OGLTexture> p( new OGLTexture(*this) );
    if( !p->init( desc ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::VtxBuf * GN::gfx::OGLRenderer::createVtxBuf( const VtxBufDesc & desc )
{
    GN_GUARD;

    if( GLEW_ARB_vertex_buffer_object )
    {
        AutoRef<OGLVtxBufVBO> p( new OGLVtxBufVBO(*this) );
        if( !p->init( desc ) ) return 0;
        return p.detach();
    }
    else
    {
        AutoRef<OGLVtxBufNormal> p( new OGLVtxBufNormal(*this) );
        if( !p->init( desc ) ) return 0;
        return p.detach();
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::IdxBuf * GN::gfx::OGLRenderer::createIdxBuf( const IdxBufDesc & desc )
{
    GN_GUARD;

    AutoRef<OGLIdxBuf> p( new OGLIdxBuf );
    if( !p->init( desc ) ) return 0;

    return p.detach();

    GN_UNGUARD;
}
