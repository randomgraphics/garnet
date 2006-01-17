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
bool GN::gfx::D3DVtxShaderAsm::init( const StrA & code )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3DVtxShaderAsm, () );

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
void GN::gfx::D3DVtxShaderAsm::quit()
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
bool GN::gfx::D3DVtxShaderAsm::deviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( mMachineCode && !mD3DShader );

    GN_DX_CHECK_RV(
        getRenderer().getDevice()->CreateVertexShader(
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
void GN::gfx::D3DVtxShaderAsm::deviceDestroy()
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
void GN::gfx::D3DVtxShaderAsm::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    GN_DX_CHECK( dev->SetVertexShader( mD3DShader ) );

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
void GN::gfx::D3DVtxShaderAsm::applyDirtyUniforms() const
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
// from Shader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DVtxShaderAsm::queryDeviceUniform( const char * name, HandleType & userData ) const
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
    desc.index = (uint16_t)index;
    userData = (HandleType)desc.u32;

    // success
    return true;

    GN_UNGUARD;
}


// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DVtxShaderAsm::compileShader()
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
bool GN::gfx::D3DVtxShaderAsm::analyzeUniforms()
{
    GN_GUARD;

    GN_ASSERT( mMachineCode );

    DWORD version = D3DXGetShaderVersion( (const DWORD*)mMachineCode->GetBufferPointer() );

    if( 0 == version )
    {
        GNGFX_ERROR( "Fail to get shader version!" );
        return false;
    }

    D3DCAPS9 caps;
    GN_DX_CHECK_RV( getRenderer().getDevice()->GetDeviceCaps( &caps ), false );

    if( HIWORD(version) >= 3 ) // vs_3_x
    {
        mMaxConstF = caps.MaxVertexShaderConst;
        mMaxConstI = 16;
        mMaxConstB = 16;
    }
    else if( HIWORD(version) >= 2 ) // vs_2_x
    {
        mMaxConstF = caps.MaxVertexShaderConst;
        mMaxConstI = 16;
        mMaxConstB = 16;
    }
    else // vs_1_x
    {
        mMaxConstF = caps.MaxVertexShaderConst;
        mMaxConstI = 0;
        mMaxConstB = 0;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// --------------------------------------------------------------------------------------
GN_INLINE void
GN::gfx::D3DVtxShaderAsm::applyUniform( LPDIRECT3DDEVICE9 dev, const Uniform & u ) const
{
    GN_GUARD_SLOW;

    D3DAsmConstDesc desc;

    desc.u32 = (uint32_t)u.userData;

    switch( desc.type )
    {
        case CONST_F :
            switch( u.type )
            {
                case UVT_FLOAT:
                    if( u.valueFloat.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX_CHECK( dev->SetVertexShaderConstantF( desc.index, &u.valueFloat[0], (UINT)( u.valueFloat.size() + 3 ) / 4 ) );
                    break;

                case UVT_FLOAT4:
                    if( u.valueVector4.empty() ) return;
                    GN_DX_CHECK( dev->SetVertexShaderConstantF( desc.index, u.valueVector4[0], (UINT)u.valueVector4.size() ) );
                    break;

                case UVT_MATRIX44:
                    if( u.valueMatrix44.empty() ) return;
                    GN_DX_CHECK( dev->SetVertexShaderConstantF( desc.index, u.valueMatrix44[0][0], (UINT)(u.valueMatrix44.size()*4) ) );
                    break;

                case UVT_BOOL:
                case UVT_INT:
                    GN_ERROR( "Setting integer or boolean value to float shader register does not work." );
                    break;
            }
            break;

        case CONST_I :
            switch( u.type )
            {
                case UVT_INT:
                    if( u.valueFloat.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX_CHECK( dev->SetVertexShaderConstantI( desc.index, (const int*)&u.valueInt[0], (UINT)( u.valueInt.size() + 3 ) / 4 ) );
                    break;

                case UVT_FLOAT:
                case UVT_FLOAT4:
                case UVT_MATRIX44:
                case UVT_BOOL:
                    GN_ERROR( "integer register accepts only integer value." );
                    break;
            }
            break;

        case CONST_B :
            switch( u.type )
            {
                case UVT_BOOL:
                    if( u.valueFloat.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX_CHECK( dev->SetVertexShaderConstantB( desc.index, (const BOOL*)&u.valueBool[0], (UINT)u.valueBool.size() ) );
                    break;

                case UVT_FLOAT:
                case UVT_FLOAT4:
                case UVT_MATRIX44:
                case UVT_INT:
                    GN_ERROR( "Bool register accepts only boolean value." );
                    break;
            }
            break;

        default:
            // Program should not reach here.
            GN_ERROR( "invalid register type!" );
            GN_UNEXPECTED();
    }

    GN_UNGUARD_SLOW;
}
