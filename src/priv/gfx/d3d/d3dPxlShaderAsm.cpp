#include "pch.h"
#include "d3dShader.h"
#include "d3dRenderer.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DPxlShaderAsm::init( const StrA & code )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3DPxlShaderAsm, () );

    mCode = code;

    // compile shader
    DWORD flag = 0;

#if GN_DEBUG
    flag |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif

    AutoComPtr<ID3DXBuffer> err;

    if( FAILED( D3DXAssembleShader(
        mCode.cstr(), mCode.size(),
        0, 0, // no defines, no includes
        flag,
        &mMachineCode, &err ) ) )
    {
        GNGFX_ERROR( (const char*)err->GetBufferPointer() );
        quit(); return selfOK();
    }

    GNGFX_WARN( "TODO: analyze shader uniforms." );

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
void GN::gfx::D3DPxlShaderAsm::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    safeRelease( mMachineCode );

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
bool GN::gfx::D3DPxlShaderAsm::deviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mD3DShader && mMachineCode );

    GN_DX_CHECK_RV(
        getRenderer().getDevice()->CreatePixelShader(
            static_cast<const DWORD*>(mMachineCode->GetBufferPointer()),
            &mD3DShader ),
        false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DPxlShaderAsm::deviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeRelease( mD3DShader );

    GN_UNGUARD;
}


// *****************************************************************************
// from D3DBasicShader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DPxlShaderAsm::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    GN_DX_CHECK( dev->SetPixelShader( mD3DShader ) );

    GNGFX_WARN( "TODO: apply shader uniforms" );

    GN_UNGUARD_SLOW;
}
