#include "pch.h"
#include "d3d9Shader.h"
#include "d3d9Renderer.h"
#include "garnet/GNd3d9.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9VtxShaderHlsl::init( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D9VtxShaderHlsl, () );

    if( !createShader( code, hints ) || !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9VtxShaderHlsl::quit()
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
void GN::gfx::D3D9VtxShaderHlsl::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    GN_DX9_CHECK( dev->SetVertexShader( mD3DShader ) );

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
void GN::gfx::D3D9VtxShaderHlsl::applyDirtyUniforms() const
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
bool GN::gfx::D3D9VtxShaderHlsl::queryDeviceUniform( const char * name, HandleType & userData ) const
{
    GN_GUARD;

    GN_ASSERT( !strEmpty(name) );

    GN_ASSERT( mConstTable );

    D3DXHANDLE h = mConstTable->GetConstantByName( NULL, name );
    if( 0 == h )
    {
        GN_ERROR( "%s is not a valid shader uniform!", name );
        return false;
    }

    // success
    userData = (HandleType)h;
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from D3D9Resource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9VtxShaderHlsl::createShader( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mConstTable && !mD3DShader );

    CreationHints ch;
    ch.fromStr( hints );

    // determine compile profile
    const char * target = ch.target.cptr();
    if( strEmpty(target) )
        target = D3DXGetVertexShaderProfile( getRenderer().getDevice() );
    if( !ch.sm3 && ( 0 == strCmpI( "vs_3_0", target ) || ( 0 == strCmpI( "vs.3.0", target ) ) ) )
        target = "vs_2_a";

    mD3DShader = d3d9::compileVS(
        getRenderer().getDevice(),
        code.cptr(),
        code.size(),
        0, // flags
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
