#include "pch.h"
#include "oglRenderer.h"

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
