#include "pch.h"
#include "d3dShader.h"
#if !GN_ENABLE_INLINE
#include "d3dPxlShaderAsm.inl"
#endif
#include "d3dRenderer.h"
#include "d3dUtils.h"

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

    if( !compileShader() || !analyzeUniforms() || !deviceCreate() || !deviceRestore() )
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

    GN_ASSERT( mMachineCode && !mD3DShader );

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

    // apply ALL uniforms to D3D device
    uint32_t handle = getFirstUniform();
    while( handle )
    {
        applyUniform( dev, getUniform( handle ) );
        handle = getNextUniform( handle );
    }
    clearDirtySet();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DPxlShaderAsm::applyDirtyUniforms() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    const std::set<uint32_t> dirtySet = getDirtyUniforms();
    std::set<uint32_t>::const_iterator i, e = dirtySet.end();
    for( i = dirtySet.begin(); i != e; ++i )
    {
        applyUniform( dev, getUniform( *i ) );
    }
    clearDirtySet();

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// from D3DBasicShader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DPxlShaderAsm::compileShader()
{
    GN_GUARD;

    // compile shader
    DWORD flag = 0;

#if GN_DEBUG
    flag |= D3DXSHADER_DEBUG;
#endif

    AutoComPtr<ID3DXBuffer> err;

    HRESULT hr = D3DXAssembleShader(
        mCode.cstr(), (UINT)mCode.size(),
        0, 0, // no defines, no includes
        flag,
        &mMachineCode, &err ); 
    if( FAILED( hr ) )
    {
        printShaderCompileError( hr, mCode.cstr(), err );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DPxlShaderAsm::analyzeUniforms()
{
    GN_GUARD;

    GN_ASSERT( mMachineCode );

    DWORD version = D3DXGetShaderVersion( (const DWORD*)mMachineCode->GetBufferPointer() );

    if( 0 == version )
    {
        GNGFX_ERROR( "Fail to get shader version!" );
        return false;
    }

    if( HIWORD(version) >= 3 ) // ps_3_x
    {
        mMaxConstF = 224;
        mMaxConstI = 16;
        mMaxConstB = 16;
    }
    else if( HIWORD(version) >= 2 ) // ps_2_x
    {
        mMaxConstF = 32;
        mMaxConstI = 16;
        mMaxConstB = 16;
    }
    else // ps_1_x
    {
        mMaxConstF = 8;
        mMaxConstI = 0;
        mMaxConstB = 0;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DPxlShaderAsm::queryDeviceUniform( const char * name, HandleType * userData ) const
{
    GN_GUARD;

    GN_ASSERT( !strEmpty(name) );

    unsigned int index;

    if( 1 != sscanf( name+1, "%u", &index ) )
    {
        GNGFX_ERROR( "invalid register name: %s", name );
        return false;
    }

    D3DAsmConstDesc desc;

    switch( name[0] )
    {
        case 'c':
        case 'C':
            if( index >= mMaxConstF )
            {
                GNGFX_ERROR( "register index(%d) is too large. (max: %d)", index, mMaxConstF );
                return false;
            }
            desc.type = CONST_F;
            break;

        case 'i':
        case 'I':
            if( index >= mMaxConstI )
            {
                GNGFX_ERROR( "register index(%d) is too large. (max: %d)", index, mMaxConstI );
                return false;
            }
            desc.type = CONST_I;
            break;

        case 'b':
        case 'B':
            if( index >= mMaxConstB )
            {
                GNGFX_ERROR( "register index(%d) is too large. (max: %d)", index, mMaxConstB );
                return false;
            }
            desc.type = CONST_B;
            break;

        default:
            GNGFX_ERROR( "invalid register name: %s", name );
            return false;
    }

    // set user data
    if( userData )
    {
        desc.index = (uint16_t)index;
        *userData = (HandleType)desc.u32;
    }

    // success
    return true;

    GN_UNGUARD;
}
