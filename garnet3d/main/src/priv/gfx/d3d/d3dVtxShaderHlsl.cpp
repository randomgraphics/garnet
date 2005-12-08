#include "pch.h"
#include "d3dShader.h"
#include "d3dRenderer.h"
#include "d3dUtils.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DVtxShaderHlsl::init( const StrA & code, const StrA & entry )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3DVtxShaderHlsl, () );

    mCode = code;
    mEntry = entry;

    if( !deviceCreate() || !deviceRestore() )
    {
        quit(); return selfOK();
    }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DVtxShaderHlsl::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3DResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DVtxShaderHlsl::deviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mConstTable && !mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    // compile shader
    DWORD flag = 0;
#if GN_DEBUG
    flag |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif
    AutoComPtr<ID3DXBuffer> err, bin;
    HRESULT hr = D3DXCompileShader(
        mCode.cstr(), (UINT)mCode.size(),
        0, 0, // no defines, no includes
        mEntry.cstr(),
        D3DXGetVertexShaderProfile( dev ),
        flag,
        &bin, &err, &mConstTable );
    if( FAILED( hr ) )
    {
        printShaderCompileError( hr, mCode.cstr(), err );
        return false;
    }

    // create shader
    GN_DX_CHECK_RV(
        dev->CreateVertexShader(
            static_cast<const DWORD*>(bin->GetBufferPointer()),
            &mD3DShader ),
        false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DVtxShaderHlsl::deviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeRelease( mD3DShader );
    safeRelease( mConstTable );

    GN_UNGUARD;
}


// *****************************************************************************
// from D3DBasicShader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DVtxShaderHlsl::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    GN_DX_CHECK( dev->SetVertexShader( mD3DShader ) );

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
void GN::gfx::D3DVtxShaderHlsl::applyDirtyUniforms() const
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
bool GN::gfx::D3DVtxShaderHlsl::queryDeviceUniform( const char * name, HandleType & userData ) const
{
    GN_GUARD;

    GN_ASSERT( !strEmpty(name) );

    GN_ASSERT( mConstTable );

    D3DXHANDLE h = mConstTable->GetConstantByName( NULL, name );
    if( 0 == h )
    {
        GNGFX_ERROR( "%s is not a valid shader uniform!", name );
        return false;
    }

    // success
    userData = (HandleType)h;
    return true;

    GN_UNGUARD;
}
