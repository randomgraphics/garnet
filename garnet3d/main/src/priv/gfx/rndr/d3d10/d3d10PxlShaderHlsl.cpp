#include "pch.h"
#include "d3d10Shader.h"
#include "d3d10Renderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10PxlShaderHlsl::init( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D10PxlShaderHlsl, ( code, hints ) );

    if( !createShader() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10PxlShaderHlsl::quit()
{
    GN_GUARD;

    safeRelease( mD3DShader );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3D10BasicShader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10PxlShaderHlsl::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    getDevice()->PSSetShader( mD3DShader );

    applyAllUniforms();

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10PxlShaderHlsl::createShader()
{
    GN_GUARD;

    ID3D10Device * dev = getRenderer().getDevice();
    ID3D10Blob * bin = getBinary();

    GN_DX10_CHECK_RV(
        dev->CreatePixelShader(
            bin->GetBufferPointer(),
            bin->GetBufferSize(),
            &mD3DShader ),
        false );

    // success
    return true;

    GN_UNGUARD;
}
