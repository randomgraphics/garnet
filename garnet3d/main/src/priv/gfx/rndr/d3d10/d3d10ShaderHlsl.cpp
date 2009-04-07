#include "pch.h"
#include "d3d10Shader.h"
#include "d3d10Renderer.h"
#include "garnet/GNd3d10.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D10ShaderHlsl::D3D10ShaderHlsl( D3D10Renderer & r, const char * profile )
    : D3D10Resource( r )
    , mProfile( profile )
{
    clear();

    if( 0 == strCmp( mProfile, "vs_4_0" ) )
    {
        mSetConstantBuffers = &ID3D10Device::VSSetConstantBuffers;
    }
    else if( 0 == strCmp( mProfile, "ps_4_0" ) )
    {
        mSetConstantBuffers = &ID3D10Device::PSSetConstantBuffers;
    }
    else if( 0 == strCmp( mProfile, "gs_4_0" ) )
    {
        mSetConstantBuffers = &ID3D10Device::GSSetConstantBuffers;
    }
    else
    {
        // you should never reach here
        GN_UNEXPECTED();
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10ShaderHlsl::init( const ShaderCode & code, const D3D10ShaderCompileOptions & options )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D10ShaderHlsl, () );

    if( code.source )
    {
        // determine compile flags
        DWORD flags =
            D3D10_SHADER_PACK_MATRIX_ROW_MAJOR |
            D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY |
            options.compileFlags;

        // compile shader
        mBinary = d3d10::compileShader( "vs_4_0", code.source, 0, flags, code.entry );
        if( NULL == mBinary ) return failure();

        // get shader reflection interface
        if( FAILED( D3D10ReflectShader(
            mBinary->GetBufferPointer(),
            mBinary->GetBufferSize(),
            &mReflection ) ) )
        {
            GN_ERROR(sLogger)( "fail to get shader refelection interface" );
            return failure();
        }

        // get shader description
        D3D10_SHADER_DESC desc;
        if( FAILED( mReflection->GetDesc( &desc ) ) )
        {
            GN_ERROR(sLogger)( "fail to get shader descriptor" );
            return failure();
        }

        ID3D10Device & dev = getDeviceRef();

        // create constant buffers
        GN_ASSERT( desc.ConstantBuffers <= 16 );
        for( UInt32 i = 0; i < desc.ConstantBuffers; ++i )
        {
            ID3D10ShaderReflectionConstantBuffer * cb = mReflection->GetConstantBufferByIndex( i );
            GN_ASSERT( cb );

            D3D10_SHADER_BUFFER_DESC cbdesc;
            cb->GetDesc( &cbdesc );
            GN_ASSERT( D3D10_CT_CBUFFER == cbdesc.Type );

            ID3D10Buffer * buf;
            D3D10_BUFFER_DESC bufdesc;
            bufdesc.ByteWidth = cbdesc.Size;
            bufdesc.Usage = D3D10_USAGE_DYNAMIC;
            bufdesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
            bufdesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
            bufdesc.MiscFlags = 0;
            GN_DX10_CHECK_RV( dev.CreateBuffer( &bufdesc, NULL, &buf ), failure() );
            mConstBufs.append( buf );

            mConstCopies.resize( mConstCopies.size() + 1 );
            mConstCopies.back().resize( cbdesc.Size );
        }
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10ShaderHlsl::quit()
{
    GN_GUARD;

    safeRelease( mBinary );
    safeRelease( mReflection );

    for( size_t i = 0; i < mConstBufs.size(); ++i )
    {
        GN_ASSERT( mConstBufs[i] );
        mConstBufs[i]->Release();
    }
    mConstBufs.clear();
    mConstCopies.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3D10BasicShader
// *****************************************************************************

/*
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10ShaderHlsl::applyDirtyUniforms() const
{
    GN_GUARD_SLOW;

    const std::set<UInt32> dirtySet = getDirtyUniforms();

    std::set<UInt32>::const_iterator i, e = dirtySet.end();
    for( i = dirtySet.begin(); i != e; ++i )
    {
        applyUniform( getUniform(*i) );
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
bool GN::gfx::D3D10ShaderHlsl::queryDeviceUniform( const char * name, HandleType & userData ) const
{
    GN_GUARD;

    GN_ASSERT( !strEmpty(name) );
    GN_ASSERT( mReflection );

    // get user descriptor
    D3D10_SHADER_DESC desc;
    if( FAILED( mReflection->GetDesc( &desc ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader descriptor" );
        return false;
    }

    // search the uniform name in every constant buffers.
    for( UINT i = 0; i < desc.ConstantBuffers; ++i )
    {
        ID3D10ShaderReflectionConstantBuffer * cb = mReflection->GetConstantBufferByIndex( i );
        GN_ASSERT( cb );

        ID3D10ShaderReflectionVariable * var = cb->GetVariableByName( name );
        D3D10_SHADER_VARIABLE_DESC vardesc;

        if( !var || FAILED( var->GetDesc( &vardesc ) ) ) continue;

        // variable found!
        GN_ASSERT( 0 == (vardesc.StartOffset % 4) );
        GN_ASSERT( 0 == (vardesc.Size % 4) );
        UniformUserData uud;
        uud.bufidx   = i;
        uud.offsetdw = vardesc.StartOffset / 4;
        uud.sizedw   = vardesc.Size / 4;
        userData = (HandleType)uud.u32;

        // success
        return true;
    }

    // variable not found
    GN_ERROR(sLogger)( "Uniform '%s' is invalid.", name );
    return false;

    GN_UNGUARD;
}*/

// *****************************************************************************
// private functions
// *****************************************************************************

/*
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10ShaderHlsl::applyUniform( const Uniform & u ) const
{
    GN_GUARD_SLOW;

    UniformUserData uud;

    uud.u32 = (UInt32)u.userData;

    // get source pointer
    const void * src = 0;
    switch( u.value.type )
    {
        case UVT_BOOL:
            if( !u.value.bools.empty() )
            {
                GN_ASSERT( uud.sizedw == u.value.bools.size() );
                src = &u.value.bools[0];
            }
            break;

        case UVT_INT:
            if( !u.value.ints.empty() )
            {
                GN_ASSERT( uud.sizedw == u.value.ints.size() );
                src = &u.value.ints[0];
            }
            break;

        case UVT_FLOAT:
            if( !u.value.floats.empty() )
            {
                GN_ASSERT( uud.sizedw == u.value.floats.size() );
                src = &u.value.floats[0];
            }
            break;

        case UVT_VECTOR4:
            if( !u.value.vector4s.empty() )
            {
                GN_ASSERT( uud.sizedw == u.value.vector4s.size() * 4 );
                src = &u.value.vector4s[0];
            }
            break;

        case UVT_MATRIX44:
            if( !u.value.matrix44s.empty() )
            {
                GN_ASSERT( uud.sizedw == u.value.matrix44s.size() * 16 );
                src = &u.value.matrix44s[0];
            }
            break;

        default:
            GN_ERROR(GN::getLogger("GN.gfx.rndr.D3D9"))( "unitialized/invalid uniform!" );
            break;

    }

    if( 0 == src ) return;

    GN_ASSERT( uud.bufidx < mConstBufs.size() );
    ID3D10Buffer * cb = mConstBufs[uud.bufidx];
    DynaArray<UInt8> & syscopy = mConstCopies[uud.bufidx];

    // copy data to system copy
    memcpy( &syscopy[uud.offsetdw*4], src, uud.sizedw * 4 );

    // then copy to D3D constant buffer
#if 0
    getDevice()->UpdateSubresource(
        cb,
        0, // sub resource
        0, // box
        syscopy.cptr(),
        0,   // row pitch
        0 ); // slice pitch

#else
    UInt32 * data;
    if( FAILED( cb->Map( D3D10_MAP_WRITE_DISCARD, 0, (void**)&data ) ) )
    {
        GN_ERROR(sLogger)( "fail to map constant buffer." );
        return;
    }
    memcpy( data, syscopy.cptr(), syscopy.size() );
    cb->Unmap();
#endif

    GN_UNGUARD_SLOW;
}*/
