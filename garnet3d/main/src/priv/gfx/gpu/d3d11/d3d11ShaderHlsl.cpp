#include "pch.h"
#include "d3d11Shader.h"
#include "d3d11Gpu.h"
#include <d3d11shader.h>

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

///
/// D3D11 shader type
///
enum D3D11ShaderType
{
    VERTEX_SHADER,          ///< VS
    GEOMETRY_SHADER,        ///< GS
    PIXEL_SHADER,           ///< PS
    NUM_D3D11_SHADER_TYPES, ///< there are 3 shader types.
};

template<D3D11ShaderType SHADER_TYPE>
struct D3D11ShaderTypeTemplate
{
};

template<>
struct D3D11ShaderTypeTemplate<VERTEX_SHADER>
{
    //@{
    typedef ID3D11VertexShader ShaderClass;

    typedef void (__stdcall ID3D11DeviceContext::*SetConstantBuffersFuncPtr)(
            UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer *const * ppConstantBuffers );
    ///@}

    /// member data
    //@{
    SetConstantBuffersFuncPtr     setConstantBuffers;
    const char                  * profile;
    //@}

    /// ctor
    D3D11ShaderTypeTemplate()
    {
        setConstantBuffers     = &ID3D11DeviceContext::VSSetConstantBuffers;
        profile                = "vs_4_0";
    }

    ID3D11VertexShader * create(
        ID3D11Device & dev,
        const char   * source,
        size_t         len,
        UInt32         flags,
        const char   * entry,
        const char   * profile,
        ID3D10Blob  ** signature )
    {
        AutoComPtr<ID3D10Blob> bin( GN::d3d11::compileShader( profile, source, len, flags, entry ) );
        if( !bin ) return NULL;

        ID3D11VertexShader * shader;
        GN_DX_CHECK_RETURN( dev.CreateVertexShader( bin->GetBufferPointer(), bin->GetBufferSize(), NULL, &shader ), 0 );

        // success
        if( signature ) *signature = bin.detach();
        return shader;
    }
};

template<>
struct D3D11ShaderTypeTemplate<GEOMETRY_SHADER>
{
    //@{
    typedef ID3D11GeometryShader ShaderClass;

    typedef void (__stdcall ID3D11DeviceContext::*SetConstantBuffersFuncPtr)(
            UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer *const * ppConstantBuffers );
    ///@}

    /// member data
    //@{
    SetConstantBuffersFuncPtr     setConstantBuffers;
    const char                  * profile;
    //@}

    /// ctor
    D3D11ShaderTypeTemplate()
    {
        setConstantBuffers     = &ID3D11DeviceContext::GSSetConstantBuffers;
        profile                = "gs_4_0";
    }

    ID3D11GeometryShader * create(
        ID3D11Device & dev,
        const char   * source,
        size_t         len,
        UInt32         flags,
        const char   * entry,
        const char   * profile,
        ID3D10Blob  ** signature )
    {
        AutoComPtr<ID3D10Blob> bin( GN::d3d11::compileShader( profile, source, len, flags, entry ) );
        if( !bin ) return NULL;

        ID3D11GeometryShader * shader;
        GN_DX_CHECK_RETURN( dev.CreateGeometryShader( bin->GetBufferPointer(), bin->GetBufferSize(), NULL, &shader ), 0 );

        // success
        if( signature ) *signature = bin.detach();
        return shader;
    }
};

template<>
struct D3D11ShaderTypeTemplate<PIXEL_SHADER>
{
    //@{
    typedef ID3D11PixelShader ShaderClass;

    typedef void (__stdcall ID3D11DeviceContext::*SetConstantBuffersFuncPtr)(
            UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer *const * ppConstantBuffers );
    ///@}

    /// member data
    //@{
    SetConstantBuffersFuncPtr     setConstantBuffers;
    const char                  * profile;
    //@}

    /// ctor
    D3D11ShaderTypeTemplate()
    {
        setConstantBuffers     = &ID3D11DeviceContext::PSSetConstantBuffers;
        profile                = "ps_4_0";
    }

    ID3D11PixelShader * create(
        ID3D11Device & dev,
        const char   * source,
        size_t         len,
        UInt32         flags,
        const char   * entry,
        const char   * profile,
        ID3D10Blob  ** signature )
    {
        AutoComPtr<ID3D10Blob> bin( GN::d3d11::compileShader( profile, source, len, flags, entry ) );
        if( !bin ) return NULL;

        ID3D11PixelShader * shader;
        GN_DX_CHECK_RETURN( dev.CreatePixelShader( bin->GetBufferPointer(), bin->GetBufferSize(), NULL, &shader ), 0 );

        // success
        if( signature ) *signature = bin.detach();
        return shader;
    }
};

//
//
// -----------------------------------------------------------------------------
static const char * sCloneString( const char * str )
{
    if( NULL == str ) return NULL;

    size_t n = GN::StringLength( str ) + 1;

    char * clone = (char*)GN::HeapAlloc( n );
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
    ID3D11Device           & dev,
    ID3D11ShaderReflection & reflection,
    D3D11ConstBufferArray  & constBufs,
    SysMemConstBufferArray & constData )
{
    // get shader description
    D3D11_SHADER_DESC desc;
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
            "Currently D3D11 GPU support %d constant buffers.",
            constBufs.MAX_SIZE );
        return false;
    }

    // create constant buffers
    constBufs.resize( desc.ConstantBuffers );
    constData.resize( desc.ConstantBuffers );
    for( UInt32 i = 0; i < desc.ConstantBuffers; ++i )
    {
        ID3D11ShaderReflectionConstantBuffer * cb = reflection.GetConstantBufferByIndex( i );
        GN_ASSERT( cb );

        D3D11_SHADER_BUFFER_DESC cbdesc;
        cb->GetDesc( &cbdesc );
        GN_ASSERT( D3D10_CT_CBUFFER == cbdesc.Type );

        D3D11_BUFFER_DESC bufdesc;
        bufdesc.ByteWidth      = cbdesc.Size;
        bufdesc.Usage          = D3D11_USAGE_DYNAMIC;
        bufdesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        bufdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufdesc.MiscFlags      = 0;
        GN_DX_CHECK_RETURN( dev.CreateBuffer( &bufdesc, NULL, &constBufs[i] ), false );

        constData[i].resize( cbdesc.Size );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
template<D3D11ShaderType SHADER_TYPE>
static bool
sInitUniforms(
    ID3D11ShaderReflection       & reflection,
    D3D11GpuProgramParameterDesc & paramDesc )
{
    GN_CASSERT( SHADER_TYPE < NUM_D3D11_SHADER_TYPES );

    D3D11_SHADER_DESC desc;
    if( FAILED( reflection.GetDesc( &desc ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader descriptor" );
        return false;
    }

    // iterate const buffers
    for( UINT cbidx = 0; cbidx < desc.ConstantBuffers; ++cbidx )
    {
        ID3D11ShaderReflectionConstantBuffer * cb = reflection.GetConstantBufferByIndex( cbidx );
        D3D11_SHADER_BUFFER_DESC cbdesc;
        cb->GetDesc( &cbdesc );

        // iterate shader variables
        for( UINT i = 0; i < cbdesc.Variables; ++i )
        {
            ID3D11ShaderReflectionVariable * var = cb->GetVariableByIndex( i );
            D3D11_SHADER_VARIABLE_DESC vardesc;
            var->GetDesc( &vardesc );

            // find uniform with same name
            D3D11UniformParameterDesc * existingUniform = paramDesc.findUniform( vardesc.Name );
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
                D3D11UniformParameterDesc u;
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
template<D3D11ShaderType SHADER_TYPE>
static bool
sInitTextures(
    ID3D11ShaderReflection       & reflection,
    D3D11GpuProgramParameterDesc & paramDesc )
{
    GN_CASSERT( SHADER_TYPE < NUM_D3D11_SHADER_TYPES );

    D3D11_SHADER_DESC desc;
    if( FAILED( reflection.GetDesc( &desc ) ) )
    {
        GN_ERROR(sLogger)( "fail to get shader descriptor" );
        return false;
    }

    // iterate resources
    for( UINT i = 0; i < desc.BoundResources; ++i )
    {
        D3D11_SHADER_INPUT_BIND_DESC sibdesc;
        reflection.GetResourceBindingDesc( i, &sibdesc );

        // ignore non-texture inputs
        if( D3D10_SIT_TEXTURE != sibdesc.Type ) continue;

        // find uniform with same name
        D3D11TextureParameterDesc * existingTexture = paramDesc.findTexture( sibdesc.Name );
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
            D3D11TextureParameterDesc t;
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
template<D3D11ShaderType SHADER_TYPE>
static bool
sInitShader(
    ID3D11Device                                                           & dev,
    const ShaderCode                                                       & code,
    const D3D11ShaderCompileOptions                                        & options,
    D3D11GpuProgramParameterDesc                                           & paramDesc,
    AutoComPtr<typename D3D11ShaderTypeTemplate<SHADER_TYPE>::ShaderClass> & shader,
    D3D11ConstBufferArray                                                  & constBufs,
    SysMemConstBufferArray                                                 & constData )
{
    GN_GUARD;

    // do nothing for empty shader code
    if( IsStringEmpty( code.source ) ) return true;

    // initialize shader type template
    D3D11ShaderTypeTemplate<SHADER_TYPE> templ;

    // compile shader
    AutoComPtr<ID3D10Blob> binary;
    shader.attach( templ.create(
        dev,
        code.source,
        0,
        options.compileFlags,
        code.entry,
        templ.profile,
        &binary ) );
    if( NULL == shader ) return false;

    // get shader reflection interface
    AutoComPtr<ID3D11ShaderReflection> reflection;
    if( FAILED( D3DReflect(
        binary->GetBufferPointer(),
        binary->GetBufferSize(),
        IID_ID3D11ShaderReflection,
        (void**)&reflection ) ) )
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
// D3D11VertexShaderHLSL
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11VertexShaderHLSL::init(
    ID3D11Device                    & dev,
    const ShaderCode                & code,
    const D3D11ShaderCompileOptions & options,
    D3D11GpuProgramParameterDesc    & paramDesc )
{
    return sInitShader<VERTEX_SHADER>( dev, code, options, paramDesc, shader, constBufs, constData );
}

// *****************************************************************************
// D3D11GeometryShaderHLSL
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11GeometryShaderHLSL::init(
    ID3D11Device                    & dev,
    const ShaderCode                & code,
    const D3D11ShaderCompileOptions & options,
    D3D11GpuProgramParameterDesc    & paramDesc )
{
    return sInitShader<GEOMETRY_SHADER>( dev, code, options, paramDesc, shader, constBufs, constData );
}

// *****************************************************************************
// D3D11PixelShaderHLSL
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11PixelShaderHLSL::init(
    ID3D11Device                    & dev,
    const ShaderCode                & code,
    const D3D11ShaderCompileOptions & options,
    D3D11GpuProgramParameterDesc    & paramDesc )
{
    return sInitShader<PIXEL_SHADER>( dev, code, options, paramDesc, shader, constBufs, constData );
}
