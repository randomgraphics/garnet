#include "pch.h"
#include "oglRenderer.h"
#include "oglShader.h"

// *****************************************************************************
// device management
// *****************************************************************************


// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::supportShader( ShaderType type, ShadingLanguage lang )
{
    GN_GUARD;

    makeCurrent();

    // check parameter
    if( 0 > type || type >= NUM_SHADER_TYPES )
    {
        GNGFX_ERROR( "invalid shader usage!" );
        return false;
    }
    if( 0 > lang || lang >= NUM_SHADING_LANGUAGES )
    {
        GNGFX_ERROR( "invalid shading language!" );
        return false;
    }

    switch( lang )
    {
        // ARB shaders
        case LANG_OGL_ARB :
            if( VERTEX_SHADER == type )
            {
                return 0 != GLEW_ARB_vertex_program;
            }
            else
            {
                GN_ASSERT( PIXEL_SHADER == type );
                return 0 != GLEW_ARB_fragment_program;
            }

        // GLSL shaders
        case LANG_OGL_GLSL :
            if( VERTEX_SHADER == type )
            {
                return 0 != GLEW_ARB_vertex_shader;
            }
            else
            {
                GN_ASSERT( PIXEL_SHADER == type );
                return 0 != GLEW_ARB_fragment_shader;
            }

        // DX shaders are always supported
        case LANG_D3D_ASM :
        case LANG_D3D_HLSL :
            return false;

        // Check Cg shader caps
        case LANG_CG : return false;

        default:
            GN_ASSERT_EX( 0, "program should never reach here!" );
            return false;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Shader *
GN::gfx::OGLRenderer::createVtxShader( ShadingLanguage lang, const StrA & code, const StrA & )
{
    GN_GUARD;

    switch( lang )
    {
        case LANG_OGL_ARB:
            if( supportShader( VERTEX_SHADER, LANG_OGL_ARB ) )
            {
                AutoRef<OGLVtxShaderARB> p( new OGLVtxShaderARB(*this) );
                if( !p->init( code ) ) return 0;
                return p.detach();
            }
            else
            {
                GNGFX_ERROR( "Current renderer do not support ARB vertex shader." );
                return 0;
            }

        case LANG_OGL_GLSL:
            //break;

        case LANG_D3D_ASM :
        case LANG_D3D_HLSL:
        case LANG_CG:
            GNGFX_ERROR( "unsupport shading language : %s", shadingLanguage2Str(lang) );
            return 0;

        default:
            GNGFX_ERROR( "invalid shading language : %d", lang );
            return 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Shader *
GN::gfx::OGLRenderer::createPxlShader( ShadingLanguage lang, const StrA & code, const StrA & )
{
    GN_GUARD;

    switch( lang )
    {
        case LANG_OGL_ARB:
            if( supportShader( PIXEL_SHADER, LANG_OGL_ARB ) )
            {
                AutoRef<OGLPxlShaderARB> p( new OGLPxlShaderARB(*this) );
                if( !p->init( code ) ) return 0;
                return p.detach();
            }
            else
            {
                GNGFX_ERROR( "Current renderer do not support ARB pixel shader." );
                return 0;
            }

        case LANG_OGL_GLSL:
            //break;

        case LANG_D3D_ASM :
        case LANG_D3D_HLSL:
        case LANG_CG:
            GNGFX_ERROR( "unsupport shading language : %s", shadingLanguage2Str(lang) );
            return 0;

        default:
            GNGFX_ERROR( "invalid shading language : %d", lang );
            return 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::bindVtxShader( const Shader * shader )
{
    GN_GUARD_SLOW;

    mCurrentDrawState.bindVtxShader( shader );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::bindPxlShader( const Shader * shader )
{
    GN_GUARD_SLOW;

    mCurrentDrawState.bindPxlShader( shader );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::bindShaders( const Shader * vtx, const Shader * pxl )
{
    GN_GUARD_SLOW;

    mCurrentDrawState.bindVtxShader( vtx );
    mCurrentDrawState.bindPxlShader( pxl );

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::updateShaderState()
{
    GN_GUARD_SLOW;

    if( mCurrentDrawState.dirtyFlags.vtxShader )
    {
        
    }

    if( mCurrentDrawState.dirtyFlags.pxlShader )
    {
    }

    GN_UNGUARD_SLOW;
}

