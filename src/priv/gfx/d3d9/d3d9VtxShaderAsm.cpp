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
bool GN::gfx::D3D9VtxShaderAsm::init( const StrA & code )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D9VtxShaderAsm, () );

    if( !createShader( code ) || !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9VtxShaderAsm::quit()
{
    GN_GUARD;

    deviceDispose();

    safeRelease( mD3DShader );

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
void GN::gfx::D3D9VtxShaderAsm::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( mD3DShader );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    GN_DX9_CHECK( dev->SetVertexShader( mD3DShader ) );

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
void GN::gfx::D3D9VtxShaderAsm::applyDirtyUniforms() const
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
bool GN::gfx::D3D9VtxShaderAsm::queryDeviceUniform( const char * name, HandleType & userData ) const
{
    GN_GUARD;

    GN_ASSERT( !strEmpty(name) );

    unsigned int index;

    if( 1 != sscanf( name+1, "%u", &index ) )
    {
        GN_ERROR( "invalid register name: %s", name );
        return false;
    }

    D3DAsmConstDesc desc;

    switch( name[0] )
    {
        case 'c':
        case 'C':
            if( index >= mMaxConstF )
            {
                GN_ERROR( "register index(%d) is too large. (max: %d)", index, mMaxConstF );
                return false;
            }
            desc.type = CONST_F;
            break;

        case 'i':
        case 'I':
            if( index >= mMaxConstI )
            {
                GN_ERROR( "register index(%d) is too large. (max: %d)", index, mMaxConstI );
                return false;
            }
            desc.type = CONST_I;
            break;

        case 'b':
        case 'B':
            if( index >= mMaxConstB )
            {
                GN_ERROR( "register index(%d) is too large. (max: %d)", index, mMaxConstB );
                return false;
            }
            desc.type = CONST_B;
            break;

        default:
            GN_ERROR( "invalid register name: %s", name );
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
bool GN::gfx::D3D9VtxShaderAsm::createShader( const StrA & code )
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mD3DShader );

    // create shader
    mD3DShader = d3d9::assembleVS( getRenderer().getDevice(), code.cptr(), code.size() );
    if( 0 == mD3DShader ) return false;

    // get shader function
    UINT sz;
    GN_DX9_CHECK_RV( mD3DShader->GetFunction( 0, &sz ), false );
    AutoObjArray<uint8_t> func( new uint8_t[sz] );
    GN_DX9_CHECK_RV( mD3DShader->GetFunction( func, &sz ), false );

    // analyze uniforms
    if( !analyzeUniforms( (const DWORD*)func.get() ) ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9VtxShaderAsm::analyzeUniforms( const DWORD * shaderFunction )
{
    GN_GUARD;

    GN_ASSERT( shaderFunction );
    DWORD version = D3DXGetShaderVersion( shaderFunction );

    if( 0 == version )
    {
        GN_ERROR( "Fail to get shader version!" );
        return false;
    }

    D3DCAPS9 caps;
    GN_DX9_CHECK_RV( getRenderer().getDevice()->GetDeviceCaps( &caps ), false );

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
GN::gfx::D3D9VtxShaderAsm::applyUniform( LPDIRECT3DDEVICE9 dev, const Uniform & u ) const
{
    GN_GUARD_SLOW;

    D3DAsmConstDesc desc;

    desc.u32 = (uint32_t)u.userData;

    switch( desc.type )
    {
        case CONST_F :
            switch( u.value.type )
            {
                case UVT_FLOAT:
                    if( u.value.floats.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX9_CHECK( dev->SetVertexShaderConstantF( desc.index, &u.value.floats[0], (UINT)( u.value.floats.size() + 3 ) / 4 ) );
                    break;

                case UVT_VECTOR4:
                    if( u.value.vector4s.empty() ) return;
                    GN_DX9_CHECK( dev->SetVertexShaderConstantF( desc.index, u.value.vector4s[0], (UINT)u.value.vector4s.size() ) );
                    break;

                case UVT_MATRIX44:
                    if( u.value.matrix44s.empty() ) return;
                    GN_DX9_CHECK( dev->SetVertexShaderConstantF( desc.index, u.value.matrix44s[0][0], (UINT)(u.value.matrix44s.size()*4) ) );
                    break;

                case UVT_BOOL:
                case UVT_INT:
                    GN_ERROR( "Setting integer or boolean value to float shader register does not work." );
                    break;
            }
            break;

        case CONST_I :
            switch( u.value.type )
            {
                case UVT_INT:
                    if( u.value.floats.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX9_CHECK( dev->SetVertexShaderConstantI( desc.index, (const int*)&u.value.ints[0], (UINT)( u.value.ints.size() + 3 ) / 4 ) );
                    break;

                case UVT_FLOAT:
                case UVT_VECTOR4:
                case UVT_MATRIX44:
                case UVT_BOOL:
                    GN_ERROR( "integer register accepts only integer value." );
                    break;
            }
            break;

        case CONST_B :
            switch( u.value.type )
            {
                case UVT_BOOL:
                    if( u.value.floats.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX9_CHECK( dev->SetVertexShaderConstantB( desc.index, (const BOOL*)&u.value.bools[0], (UINT)u.value.bools.size() ) );
                    break;

                case UVT_FLOAT:
                case UVT_VECTOR4:
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
