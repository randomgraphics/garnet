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
bool GN::gfx::D3D10ShaderHlsl::init( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D10ShaderHlsl, () );

    if( !compileShader( code, hints ) ) return failure();

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

        if( var )
        {
            D3D10_SHADER_VARIABLE_DESC vardesc;

            var->GetDesc( &vardesc );

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
    }

    // variable not found
    GN_ERROR(sLogger)( "Uniform '%s' is invalid.", name );
    return false;

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
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

    // copy data to constant buffer

    GN_ASSERT( uud.bufidx < mConstBufs.size() );
    ID3D10Buffer * cb = mConstBufs[uud.bufidx];
    UInt32 * data;
    if( FAILED( cb->Map( D3D10_MAP_WRITE, 0, (void**)&data ) ) )
    {
        GN_ERROR(sLogger)( "fail to map constant buffer." );
        return;
    }

    memcpy( data + uud.offsetdw, src, uud.sizedw * 4 );

    cb->Unmap();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10ShaderHlsl::compileShader( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mReflection && !mBinary );

    D3D10ShaderCreationHints ch;
    ch.fromStr( hints );

    // determine compile flags
    DWORD flags = D3DXSHADER_PACKMATRIX_ROWMAJOR;
    if( !ch.optimize ) flags |= D3DXSHADER_SKIPOPTIMIZATION;

    // compile shader
    AutoComPtr<ID3D10Blob> err;
    if( FAILED( D3D10CompileShader(
        code.cptr(), code.size(),
        ch.filename.cptr(),
        0, 0, // no defines, no includes
        ch.entry.cptr(),
        mProfile,
        flags,
        &mBinary,
        &err ) ) )
    {
        GN_ERROR(sLogger)(
            "\n================== Shader compile failure ===============\n"
            "%s\n"
            "\n---------------------------------------------------------\n"
            "%s\n"
            "\n=========================================================\n",
            code.cptr(),
            err ? (const char*)err->GetBufferPointer() : "Unknown error." );
        
        return false;
    }

    // get shader reflection interface
    if( FAILED( D3D10ReflectShader(
        mBinary->GetBufferPointer(),
        mBinary->GetBufferSize(),
        &mReflection ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader refelection interface" );
        return false;
    }

    // get shader description
    D3D10_SHADER_DESC desc;
    if( FAILED( mReflection->GetDesc( &desc ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader descriptor" );
        return false;
    }

    // create constant buffers
    ID3D10Device * dev = getRenderer().getDevice();
    for( size_t i = 0; i < desc.ConstantBuffers; ++i )
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
        GN_DX10_CHECK_RV( dev->CreateBuffer( &bufdesc, NULL, &buf ), false );

        mConstBufs.append( buf );
    }

    // update userdata of all uniforms
    UInt32 handle = getFirstUniform();
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
