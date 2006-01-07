#include "pch.h"
#include "d3dRenderer.h"
#include "d3dShader.h"

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
        // OGL shaders are always unsupported
        case LANG_OGL_ARB :
        case LANG_OGL_GLSL :
            return false;

        // DX shaders are always supported
        case LANG_D3D_ASM :
        case LANG_D3D_HLSL :
            if( VERTEX_SHADER == type )
            {
                return 0 != ( getCaps( CAPS_VSCAPS ) & VSCAPS_D3D_ALL );
            }
            else
            {
                GN_ASSERT( PIXEL_SHADER == type );
                return 0 != ( getCaps( CAPS_PSCAPS ) & VSCAPS_D3D_ALL );
            }

        // Check Cg shader caps
        case LANG_CG :
            return false;

        default :
            GN_ASSERT_EX( 0, "program should never reach here!" );
            return false;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Shader *
GN::gfx::D3DRenderer::createVtxShader(
    ShadingLanguage lang, const StrA & code, const StrA & entry )
{
    GN_GUARD;

    switch( lang )
    {
        case LANG_D3D_ASM :
            {
                AutoRef<D3DVtxShaderAsm> p( new D3DVtxShaderAsm(*this) );
                if( !p->init( code ) ) return 0;
                return p.detach();
            }

        case LANG_D3D_HLSL:
            {
                AutoRef<D3DVtxShaderHlsl> p( new D3DVtxShaderHlsl(*this) );
                if( !p->init( code, entry ) ) return 0;
                return p.detach();
            }

        case LANG_CG:
        case LANG_OGL_ARB:
        case LANG_OGL_GLSL:
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
GN::gfx::D3DRenderer::createPxlShader(
    ShadingLanguage lang, const StrA & code, const StrA & entry )
{
    GN_GUARD;

    switch( lang )
    {
        case LANG_D3D_ASM :
            {
                AutoRef<D3DPxlShaderAsm> p( new D3DPxlShaderAsm(*this) );
                if( !p->init( code ) ) return 0;
                return p.detach();
            }

        case LANG_D3D_HLSL:
            {
                AutoRef<D3DPxlShaderHlsl> p( new D3DPxlShaderHlsl(*this) );
                if( !p->init( code, entry ) ) return 0;
                return p.detach();
            }

        case LANG_CG:
        case LANG_OGL_ARB:
        case LANG_OGL_GLSL:
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
void GN::gfx::D3DRenderer::bindShaders( const Shader * vtx, const Shader * pxl )
{
    GN_GUARD_SLOW;

    mDrawState.bindVtxShader( vtx );
    mDrawState.bindPxlShader( pxl );

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::applyShader(
    const Shader * vtxShader, bool vtxShaderDirty,
    const Shader * pxlShader, bool pxlShaderDirty ) const
{
    GN_GUARD_SLOW;

    if( vtxShader )
    {
        if( vtxShaderDirty )
            safeCast<const D3DBasicShader*>(vtxShader)->apply();
        else
            safeCast<const D3DBasicShader*>(vtxShader)->applyDirtyUniforms();
    }
    else
    {
        GN_DX_CHECK( mDevice->SetVertexShader( 0 ) );
    }

    if( pxlShader )
    {
        if( pxlShaderDirty )
            safeCast<const D3DBasicShader*>(pxlShader)->apply();
        else
            safeCast<const D3DBasicShader*>(pxlShader)->applyDirtyUniforms();
    }
    else
    {
        GN_DX_CHECK( mDevice->SetVertexShader( 0 ) );
    }

    GN_UNGUARD_SLOW;
}
