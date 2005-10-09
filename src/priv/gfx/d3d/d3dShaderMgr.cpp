#include "pch.h"
#include "d3dRenderer.h"

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::supportShader( ShaderType type, ShadingLanguage lang )
{
    GN_GUARD;

    // check parameter
    if( 0 > type || type >= NUM_SHADER_TYPES )
    {
        GND3D_ERROR( "invalid shader usage!" );
        return false;
    }
    if( 0 > lang || lang >= NUM_SHADING_LANGUAGES )
    {
        GND3D_ERROR( "invalid shading language!" );
        return false;
    }

    switch( lang )
    {
        // OGL shaders are always unsupported
        case LANG_OGL_ARB :
        case LANG_OGL_GLSL :
            return false;

        // DX shaders are always supported
        // TODO: need more precise detection
        case LANG_D3D_ASM :
        case LANG_D3D_HLSL :
            return true;

        // Check Cg shader caps
        case LANG_CG :
            return false;

        default :
            GN_ASSERT_EX( 0, "program should never reach here!" );
            return false;
    }

    GN_UNGUARD;
}
