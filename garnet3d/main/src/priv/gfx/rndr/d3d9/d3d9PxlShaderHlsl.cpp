#include "pch.h"
#include "d3d9Shader.h"
#include "d3d9Renderer.h"
#include "garnet/GNd3d9.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9PxlShaderHlsl::init( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D9PxlShaderHlsl, () );

    if( !createShader( code, hints ) || !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9PxlShaderHlsl::quit()
{
    GN_GUARD;

    deviceDispose();

    safeRelease( mD3DShader );
    safeRelease( mConstTable );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3D9BasicShader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9PxlShaderHlsl::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    GN_DX9_CHECK( dev->SetPixelShader( mD3DShader ) );

    // apply ALL uniforms to D3D device
    uint32_t handle = getFirstUniform();
    while( handle )
    {
        applyUniform( dev, mConstTable, getUniform( handle ) );
        handle = getNextUniform( handle );
    }
    clearDirtySet();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9PxlShaderHlsl::applyDirtyUniforms() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    const std::set<uint32_t> dirtySet = getDirtyUniforms();
    std::set<uint32_t>::const_iterator i, e = dirtySet.end();
    for( i = dirtySet.begin(); i != e; ++i )
    {
        applyUniform( dev, mConstTable, getUniform( *i ) );
    }
    clearDirtySet();

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// from Shader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9PxlShaderHlsl::queryDeviceUniform( const char * name, HandleType & userData ) const
{
    GN_GUARD;

    GN_ASSERT( !strEmpty(name) );

    GN_ASSERT( mConstTable );

    D3DXHANDLE h = mConstTable->GetConstantByName( NULL, name );
    if( 0 == h )
    {
        GN_ERROR(sLogger)( "%s is not a valid shader uniform!", name );
        return false;
    }

    // success
    userData = (HandleType)h;
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9PxlShaderHlsl::createShader( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mConstTable && !mD3DShader );

    CreationHints ch;
    ch.fromStr( hints );

    // determine compile profile
    const char * target = ch.target.cptr();
    if( strEmpty(target) )
        target = D3DXGetPixelShaderProfile( getRenderer().getDevice() );
    if( !ch.sm3 && ( 0 == strCmpI( "ps_3_0", target ) || ( 0 == strCmpI( "ps.3.0", target ) ) ) )
        target = "ps_2_a";

    mD3DShader = d3d9::compilePS(
        getRenderer().getDevice(),
        code.cptr(),
        code.size(),
        ch.optimize ? 0 : D3DXSHADER_SKIPOPTIMIZATION, // flags
        ch.entry.cptr(),
        target,
        &mConstTable );
    if( 0 == mD3DShader ) return false;

    // update userdata of all uniforms
    uint32_t handle = getFirstUniform();
    while( handle )
    {
        Uniform & u = getUniform( handle );
        if( !queryDeviceUniform( u.name.cptr(), u.userData ) ) return false;
        handle = getNextUniform( handle );
    }

    // success
    return true;

    GN_UNGUARD;
}
