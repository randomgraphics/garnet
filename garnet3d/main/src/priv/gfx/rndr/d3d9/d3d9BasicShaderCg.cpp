#include "pch.h"
#include "d3d9Shader.h"
#include "d3d9Renderer.h"

#ifdef HAS_CG_D3D9

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9BasicShaderCg::init( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9BasicShaderCg, () );

    // get the latest profile
    switch( getType() )
    {
        case SHADER_VS :
            mProfile = cgD3D9GetLatestVertexProfile();
            break;
        case SHADER_PS :
            mProfile = cgD3D9GetLatestPixelProfile();
            break;
        case SHADER_GS :
            GN_ERROR(sLogger)( "D3D9 renderer does not support geometry shader." );
            return false;
    }
    if( CG_PROFILE_UNKNOWN == mProfile )
    {
        GN_ERROR(sLogger)( "Fail to get the lastest shader profile!" );
        return failure();
    }

    // parse hints
    Registry reg( hints );
    StrA entry = reg.gets( "entry", "main" );

    // create the shader
    if( !mShader.init( getRenderer().getCgContext(), mProfile, code, entry ) ) return failure();

    // load the program
    DWORD asmFlags = 0;
#if GN_DEBUG_BUILD
    asmFlags |= D3DXSHADER_DEBUG;
#endif
    GN_DX9_CHECK_RV( cgD3D9LoadProgram( mShader.getProgram(), CG_FALSE, asmFlags ), failure() );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9BasicShaderCg::quit()
{
    GN_GUARD;

    if( cgIsProgram( mShader.getProgram() ) )
    {
        GN_DX9_CHECK( cgD3D9UnloadProgram( mShader.getProgram() ) );
    }

    mShader.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
// from D3D9BasicShader
// ****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9BasicShaderCg::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    // enable the shader
    GN_DX9_CHECK( cgD3D9BindProgram( mShader.getProgram() ) );

    // apply ALL uniforms to D3D device
    UInt32 handle = getFirstUniform();
    while( handle )
    {
        applyUniform( getUniform( handle ) );
        handle = getNextUniform( handle );
    }
    clearDirtySet();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9BasicShaderCg::applyDirtyUniforms() const
{
    GN_GUARD_SLOW;
    const std::set<UInt32> dirtySet = getDirtyUniforms();
    std::set<UInt32>::const_iterator i, e = dirtySet.end();
    for( i = dirtySet.begin(); i != e; ++i )
    {
        applyUniform( getUniform( *i ) );
    }
    clearDirtySet();
    GN_UNGUARD_SLOW;
}

// ****************************************************************************
// private functions
// ****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9BasicShaderCg::queryDeviceUniform(
    const char * name, HandleType & userData ) const
{
    GN_GUARD;
    
    GN_ASSERT( !strEmpty(name) );

    CGparameter param = mShader.getUniformHandle( name );
    if( 0 == param ) return false;

    // success
    userData = (HandleType)param;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9BasicShaderCg::applyUniform( const Uniform & u ) const
{
    GN_GUARD_SLOW;

    CGparameter param = (CGparameter)u.userData;

    GN_ASSERT( cgIsParameter( param ) );

    switch( u.value.type )
    {
        case UVT_VECTOR4 :
            if( 1 == u.value.vector4s.size() )
            {
                GN_DX9_CHECK( cgD3D9SetUniform(
                    param,
                    u.value.vector4s[0] ) );
            }
            else
            {
                GN_DX9_CHECK( cgD3D9SetUniformArray(
                    param,
                    0,
                    (DWORD)u.value.vector4s.size(),
                    u.value.vector4s[0] ) );
            }
            break;

        case UVT_MATRIX44 :
            if( 1 == u.value.matrix44s.size() )
            {
                GN_DX9_CHECK( cgD3D9SetUniformMatrix(
                    param,
                    (const D3DXMATRIX*)&u.value.matrix44s[0] ) );
            }
            else
            {
                GN_DX9_CHECK( cgD3D9SetUniformMatrixArray(
                    param,
                    0,
                    (DWORD)u.value.matrix44s.size(),
                    (const D3DXMATRIX*)&u.value.matrix44s[0] ) );
            }
            break;

        case UVT_FLOAT :
            if( 1 == u.value.floats.size() )
            {
                GN_CG_CHECK( cgD3D9SetUniform(
                    param,
                    &u.value.floats[0] ) );
            }
            else
            {
                GN_CG_CHECK( cgD3D9SetUniformArray(
                    param,
                    0,
                    (DWORD)u.value.floats.size(),
                    &u.value.floats[0] ) );
            }
            break;

        case UVT_BOOL :
        case UVT_INT :
            GN_ERROR(sLogger)( "D3D9 Cg shader does not support boolean and integer uniforms." );
            break;

        default:
            GN_ERROR(sLogger)( "invalid Cg uniform type." );
    }

    GN_UNGUARD_SLOW;
}

#endif
