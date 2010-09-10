#include "pch.h"
#include "d3d10Shader.h"
#include "d3d10Gpu.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D10");

///
/// D3D10 shader type
///
enum D3D10ShaderType
{
    VERTEX_SHADER,          ///< VS
    GEOMETRY_SHADER,        ///< GS
    PIXEL_SHADER,           ///< PS
    NUM_D3D10_SHADER_TYPES, ///< there are 3 shader types.
};

template<D3D10ShaderType SHADER_TYPE>
struct D3D10ShaderTypeTemplate
{
};

template<>
struct D3D10ShaderTypeTemplate<VERTEX_SHADER>
{
    //@{
    typedef ID3D10VertexShader ShaderClass;

    typedef ShaderClass * (*CompileAndCreateShaderFuncPtr)(
            ID3D10Device & dev,
            const char   * source,
            size_t         len,
            uint32         flags,
            const char   * entry,
            const char   * profile,
            ID3D10Blob  ** binary );

    typedef void (__stdcall ID3D10Device::*SetConstantBuffersFuncPtr)(
            UINT StartSlot,
            UINT NumBuffers,
            ID3D10Buffer *const * ppConstantBuffers );
    ///@}

    /// member data
    //@{
    CompileAndCreateShaderFuncPtr compileAndCreateShader;
    SetConstantBuffersFuncPtr     setConstantBuffers;
    const char                  * profile;
    //@}

    /// ctor
    D3D10ShaderTypeTemplate()
    {
        compileAndCreateShader = &GN::d3d10::compileAndCreateVS;
        setConstantBuffers     = &ID3D10Device::VSSetConstantBuffers;
        profile                = "vs_4_0";
    }
};

template<>
struct D3D10ShaderTypeTemplate<GEOMETRY_SHADER>
{
    //@{
    typedef ID3D10GeometryShader ShaderClass;

    typedef ShaderClass * (*CompileAndCreateShaderFuncPtr)(
            ID3D10Device & dev,
            const char   * source,
            size_t         len,
            uint32         flags,
            const char   * entry,
            const char   * profile,
            ID3D10Blob  ** binary );

    typedef void (__stdcall ID3D10Device::*SetConstantBuffersFuncPtr)(
            UINT StartSlot,
            UINT NumBuffers,
            ID3D10Buffer *const * ppConstantBuffers );
    ///@}

    /// member data
    //@{
    CompileAndCreateShaderFuncPtr compileAndCreateShader;
    SetConstantBuffersFuncPtr     setConstantBuffers;
    const char                  * profile;
    //@}

    /// ctor
    D3D10ShaderTypeTemplate()
    {
        compileAndCreateShader = &GN::d3d10::compileAndCreateGS;
        setConstantBuffers     = &ID3D10Device::GSSetConstantBuffers;
        profile                = "gs_4_0";
    }
};

template<>
struct D3D10ShaderTypeTemplate<PIXEL_SHADER>
{
    //@{
    typedef ID3D10PixelShader ShaderClass;

    typedef ShaderClass * (*CompileAndCreateShaderFuncPtr)(
            ID3D10Device & dev,
            const char   * source,
            size_t         len,
            uint32         flags,
            const char   * entry,
            const char   * profile,
            ID3D10Blob  ** binary );

    typedef void (__stdcall ID3D10Device::*SetConstantBuffersFuncPtr)(
            UINT StartSlot,
            UINT NumBuffers,
            ID3D10Buffer *const * ppConstantBuffers );
    ///@}

    /// member data
    //@{
    CompileAndCreateShaderFuncPtr compileAndCreateShader;
    SetConstantBuffersFuncPtr     setConstantBuffers;
    const char                  * profile;
    //@}

    /// ctor
    D3D10ShaderTypeTemplate()
    {
        compileAndCreateShader = &GN::d3d10::compileAndCreatePS;
        setConstantBuffers     = &ID3D10Device::PSSetConstantBuffers;
        profile                = "ps_4_0";
    }
};

//
//
// -----------------------------------------------------------------------------
static const char * sCloneString( const char * str )
{
    if( NULL == str ) return NULL;

    size_t n = GN::stringLength( str ) + 1;

    char * clone = (char*)GN::HeapMemory::alloc( n );
    if( NULL == clone )
    {
        GN_GPU_RIP( "Out of memory!" );
        return NULL;
    }

    memcpy( clone, str, n );

    return clone;
}

//
//
// -----------------------------------------------------------------------------
static bool
sInitConstBuffers(
    ID3D10Device           & dev,
    ID3D10ShaderReflection & reflection,
    D3D10ConstBufferArray  & constBufs,
    SysMemConstBufferArray & constData )
{
    // get shader description
    D3D10_SHADER_DESC desc;
    if( FAILED( reflection.GetDesc( &desc ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader descriptor" );
        return false;
    }

    // check for number of constant buffers
    if( desc.ConstantBuffers > constBufs.MAX_SIZE )
    {
        GN_ERROR(sLogger)(
            "Too many constant buffers. "
            "Currently D3D10 GPU support %d constant buffers.",
            constBufs.MAX_SIZE );
        return false;
    }

    // create constant buffers
    constBufs.resize( desc.ConstantBuffers );
    constData.resize( desc.ConstantBuffers );
    for( uint32 i = 0; i < desc.ConstantBuffers; ++i )
    {
        ID3D10ShaderReflectionConstantBuffer * cb = reflection.GetConstantBufferByIndex( i );
        GN_ASSERT( cb );

        D3D10_SHADER_BUFFER_DESC cbdesc;
        cb->GetDesc( &cbdesc );
        GN_ASSERT( D3D10_CT_CBUFFER == cbdesc.Type );

        D3D10_BUFFER_DESC bufdesc;
        bufdesc.ByteWidth      = cbdesc.Size;
        bufdesc.Usage          = D3D10_USAGE_DYNAMIC;
        bufdesc.BindFlags      = D3D10_BIND_CONSTANT_BUFFER;
        bufdesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
        bufdesc.MiscFlags      = 0;
        GN_DX_CHECK_RETURN( dev.CreateBuffer( &bufdesc, NULL, &constBufs[i] ), false );

        constData[i].resize( cbdesc.Size );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
template<D3D10ShaderType SHADER_TYPE>
static bool
sInitUniforms(
    ID3D10ShaderReflection       & reflection,
    D3D10GpuProgramParameterDesc & paramDesc )
{
    GN_CASSERT( SHADER_TYPE < NUM_D3D10_SHADER_TYPES );

    D3D10_SHADER_DESC desc;
    if( FAILED( reflection.GetDesc( &desc ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader descriptor" );
        return false;
    }

    // iterate const buffers
    for( UINT cbidx = 0; cbidx < desc.ConstantBuffers; ++cbidx )
    {
        ID3D10ShaderReflectionConstantBuffer * cb = reflection.GetConstantBufferByIndex( cbidx );
        D3D10_SHADER_BUFFER_DESC cbdesc;
        cb->GetDesc( &cbdesc );

        // iterate shader variables
        for( UINT i = 0; i < cbdesc.Variables; ++i )
        {
            ID3D10ShaderReflectionVariable * var = cb->GetVariableByIndex( i );
            D3D10_SHADER_VARIABLE_DESC vardesc;
            var->GetDesc( &vardesc );

            // find uniform with same name
            D3D10UniformParameterDesc * existingUniform = paramDesc.findUniform( vardesc.Name );
            if( existingUniform )
            {
                if( existingUniform->size != vardesc.Size )
                {
                    GN_ERROR(sLogger)( "If an uniform name is used by more than one shaders, its type and size must be same in all shaders." );
                    return false;
                }

                // update shader specific properties
                GN_ASSERT( !existingUniform->ssp[SHADER_TYPE].used );
                existingUniform->ssp[SHADER_TYPE].used   = true;
                existingUniform->ssp[SHADER_TYPE].cbidx  = cbidx;
                existingUniform->ssp[SHADER_TYPE].offset = vardesc.StartOffset;
            }
            else
            {
                // this is a new uniform
                D3D10UniformParameterDesc u;
                u.name                    = sCloneString( vardesc.Name );
                u.size                    = vardesc.Size;
                u.ssp[SHADER_TYPE].used   = true;
                u.ssp[SHADER_TYPE].cbidx  = cbidx;
                u.ssp[SHADER_TYPE].offset = vardesc.StartOffset;

                // append u to uniform array
                paramDesc.addUniform( u );
            }
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
template<D3D10ShaderType SHADER_TYPE>
static bool
sInitTextures(
    ID3D10ShaderReflection       & reflection,
    D3D10GpuProgramParameterDesc & paramDesc )
{
    GN_CASSERT( SHADER_TYPE < NUM_D3D10_SHADER_TYPES );

    D3D10_SHADER_DESC desc;
    if( FAILED( reflection.GetDesc( &desc ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader descriptor" );
        return false;
    }

    // iterate resources
    for( UINT i = 0; i < desc.BoundResources; ++i )
    {
        D3D10_SHADER_INPUT_BIND_DESC sibdesc;
        reflection.GetResourceBindingDesc( i, &sibdesc );

        // ignore non-texture inputs
        if( D3D10_SIT_TEXTURE != sibdesc.Type ) continue;

        // find uniform with same name
        D3D10TextureParameterDesc * existingTexture = paramDesc.findTexture( sibdesc.Name );
        if( existingTexture )
        {
            // update shader specific properties
            GN_ASSERT( !existingTexture->ssp[SHADER_TYPE].used );
            existingTexture->ssp[SHADER_TYPE].used  = true;
            existingTexture->ssp[SHADER_TYPE].stage = sibdesc.BindPoint;
        }
        else
        {
            // this is a new texture
            D3D10TextureParameterDesc t;
            t.name                    = sCloneString( sibdesc.Name );
            t.ssp[SHADER_TYPE].used   = true;
            t.ssp[SHADER_TYPE].stage  = sibdesc.BindPoint;

            // append t to texture array
            paramDesc.addTexture( t );
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
template<D3D10ShaderType SHADER_TYPE>
static bool
sInitShader(
    ID3D10Device                                                           & dev,
    const ShaderCode                                                       & code,
    const D3D10ShaderCompileOptions                                        & options,
    D3D10GpuProgramParameterDesc                                           & paramDesc,
    AutoComPtr<typename D3D10ShaderTypeTemplate<SHADER_TYPE>::ShaderClass> & shader,
    D3D10ConstBufferArray                                                  & constBufs,
    SysMemConstBufferArray                                                 & constData )
{
    GN_GUARD;

    // do nothing for empty shader code
    if( stringEmpty( code.source ) ) return true;

    // initialize shader type template
    D3D10ShaderTypeTemplate<SHADER_TYPE> templ;

    // compile shader
    AutoComPtr<ID3D10Blob> binary;
    shader.attach( templ.compileAndCreateShader(
        dev,
        code.source,
        0,
        options.compileFlags,
        code.entry,
        templ.profile,
        &binary ) );
    if( NULL == shader ) return false;

    // get shader reflection interface
    AutoComPtr<ID3D10ShaderReflection> reflection;
    if( FAILED( D3D10ReflectShader(
        binary->GetBufferPointer(),
        binary->GetBufferSize(),
        &reflection ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader refelection interface" );
        return false;
    }

    // initialize constant buffers
    if( !sInitConstBuffers( dev, *reflection, constBufs, constData ) ) return false;

    // initialize uniform parameters
    if( !sInitUniforms<SHADER_TYPE>( *reflection, paramDesc ) ) return false;

    // initialize texture parameters
    if( !sInitTextures<SHADER_TYPE>( *reflection, paramDesc ) ) return false;

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// D3D10VertexShaderHLSL
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10VertexShaderHLSL::init(
    ID3D10Device                    & dev,
    const ShaderCode                & code,
    const D3D10ShaderCompileOptions & options,
    D3D10GpuProgramParameterDesc    & paramDesc )
{
    return sInitShader<VERTEX_SHADER>( dev, code, options, paramDesc, shader, constBufs, constData );
}

// *****************************************************************************
// D3D10GeometryShaderHLSL
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10GeometryShaderHLSL::init(
    ID3D10Device                    & dev,
    const ShaderCode                & code,
    const D3D10ShaderCompileOptions & options,
    D3D10GpuProgramParameterDesc    & paramDesc )
{
    return sInitShader<GEOMETRY_SHADER>( dev, code, options, paramDesc, shader, constBufs, constData );
}

// *****************************************************************************
// D3D10PixelShaderHLSL
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10PixelShaderHLSL::init(
    ID3D10Device                    & dev,
    const ShaderCode                & code,
    const D3D10ShaderCompileOptions & options,
    D3D10GpuProgramParameterDesc    & paramDesc )
{
    return sInitShader<PIXEL_SHADER>( dev, code, options, paramDesc, shader, constBufs, constData );
}

/*
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10ShaderHLSL::applyUniform( const Uniform & u ) const
{
    GN_GUARD_SLOW;

    UniformUserData uud;

    uud.u32 = (uint32)u.userData;

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
            GN_ERROR(GN::getLogger("GN.gfx.gpu.D3D9"))( "unitialized/invalid uniform!" );
            break;

    }

    if( 0 == src ) return;

    GN_ASSERT( uud.bufidx < constBufs.size() );
    ID3D10Buffer * cb = constBufs[uud.bufidx];
    DynaArray<uint8> & syscopy = constData[uud.bufidx];

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
    uint32 * data;
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
