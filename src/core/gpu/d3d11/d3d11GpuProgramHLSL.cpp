#include "pch.h"
#include "d3d11Shader.h"
#include "d3d11Gpu.h"
#include "d3d11Texture.h"
#include "d3d11ShaderTypeTraits.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

using namespace GN;
using namespace GN::gfx;

D3D11_TEXTURE_ADDRESS_MODE sAdressModeToD3D11( unsigned short addr )
{
    static D3D11_TEXTURE_ADDRESS_MODE mapping[SamplerDesc::NUM_ADDRESS_MODES] =
    {
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_CLAMP,
        D3D11_TEXTURE_ADDRESS_BORDER,
        D3D11_TEXTURE_ADDRESS_MIRROR
    };

    if( addr < GN_ARRAY_COUNT(mapping) )
    {
        return mapping[addr];
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid garnet texture address mode: %d", addr );
        return D3D11_TEXTURE_ADDRESS_CLAMP;
    }
}

// *****************************************************************************
// D3D11GpuProgramParameterDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static const char * sD3D11CloneString( const char * str )
{
    if( NULL == str ) return NULL;

    size_t n = GN::str::length( str ) + 1;

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
template<class T>
static void sD3D11FreeNameString( T & t )
{
    HeapMemory::dealloc( (void*)t.name );
    t.name = NULL;
}

//
//
// -----------------------------------------------------------------------------
template<class T>
class D3D11FindByName
{
    const char * mName;

public:

    D3D11FindByName( const char * name ) : mName(name)
    {
    }

    bool operator()( const T & t ) const
    {
        return 0 == str::compare( t.name, mName );
    }
};

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11GpuProgramParameterDesc::D3D11GpuProgramParameterDesc()
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11GpuProgramParameterDesc::~D3D11GpuProgramParameterDesc()
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramParameterDesc::clear()
{
    // delete all name string
    std::for_each( mUniforms.begin(), mUniforms.end(), &sD3D11FreeNameString<D3D11UniformParameterDesc> );
    std::for_each( mTextures.begin(), mTextures.end(), &sD3D11FreeNameString<D3D11TextureParameterDesc> );
    std::for_each( mAttributes.begin(), mAttributes.end(), &sD3D11FreeNameString<D3D11AttributeParameterDesc> );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramParameterDesc::buildParameterArrays()
{
    mUniformArray       = mUniforms.rawptr();
    mUniformCount       = (uint32)mUniforms.size();
    mUniformArrayStride = (uint32)sizeof(mUniforms[0]);

    mTextureArray       = mTextures.rawptr();
    mTextureCount       = (uint32)mTextures.size();
    mTextureArrayStride = (uint32)sizeof(mTextures[0]);

    mAttributeArray       = mAttributes.rawptr();
    mAttributeCount       = (uint32)mAttributes.size();
    mAttributeArrayStride = (uint32)sizeof(mAttributes[0]);
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D11UniformParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findUniform( const char * name ) const
{
    const GN::gfx::D3D11UniformParameterDesc * p = std::find_if(
        mUniforms.begin(),
        mUniforms.end(),
        D3D11FindByName<D3D11UniformParameterDesc>(name) );

    return ( p == mUniforms.end() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11UniformParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findUniform( const char * name )
{
    GN::gfx::D3D11UniformParameterDesc * p = std::find_if(
        mUniforms.begin(),
        mUniforms.end(),
        D3D11FindByName<D3D11UniformParameterDesc>(name) );

    return ( p == mUniforms.end() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D11TextureParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findTexture( const char * name ) const
{
    const GN::gfx::D3D11TextureParameterDesc * p = std::find_if(
        mTextures.begin(),
        mTextures.end(),
        D3D11FindByName<D3D11TextureParameterDesc>(name) );

    return ( p == mTextures.end() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11TextureParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findTexture( const char * name )
{
    GN::gfx::D3D11TextureParameterDesc * p = std::find_if(
        mTextures.begin(),
        mTextures.end(),
        D3D11FindByName<D3D11TextureParameterDesc>(name) );

    return ( p == mTextures.end() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D11AttributeParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findAttribute( const char * name ) const
{
    const GN::gfx::D3D11AttributeParameterDesc * p = std::find_if(
        mAttributes.begin(),
        mAttributes.end(),
        D3D11FindByName<D3D11AttributeParameterDesc>(name) );

    return ( p == mAttributes.end() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11AttributeParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findAttribute( const char * name )
{
    GN::gfx::D3D11AttributeParameterDesc * p = std::find_if(
        mAttributes.begin(),
        mAttributes.end(),
        D3D11FindByName<D3D11AttributeParameterDesc>(name) );

    return ( p == mAttributes.end() ) ? NULL : p;
}

// *****************************************************************************
// D3D11GpuProgramHLSL
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
template<int SHADER_STAGE>
static bool
sInitD3D11Uniforms(
    ID3D11ShaderReflection       & reflection,
    D3D11GpuProgramParameterDesc & paramDesc )
{
    GN_CASSERT( SHADER_STAGE < ShaderStage::COUNT );

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
                GN_ASSERT( !existingUniform->ssp[SHADER_STAGE].used );
                existingUniform->ssp[SHADER_STAGE].used   = true;
                existingUniform->ssp[SHADER_STAGE].cbidx  = cbidx;
                existingUniform->ssp[SHADER_STAGE].offset = vardesc.StartOffset;
            }
            else
            {
                // this is a new uniform
                D3D11UniformParameterDesc u;
                u.name                    = sD3D11CloneString( vardesc.Name );
                u.size                    = vardesc.Size;
                u.ssp[SHADER_STAGE].used   = true;
                u.ssp[SHADER_STAGE].cbidx  = cbidx;
                u.ssp[SHADER_STAGE].offset = vardesc.StartOffset;

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
template<int SHADER_STAGE>
static bool sInitD3D11Textures(
    ID3D11ShaderReflection       & reflection,
    D3D11GpuProgramParameterDesc & paramDesc )
{
    GN_CASSERT( SHADER_STAGE < ShaderStage::COUNT );

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
            GN_ASSERT( !existingTexture->ssp[SHADER_STAGE].used );
            existingTexture->ssp[SHADER_STAGE].used  = true;
            existingTexture->ssp[SHADER_STAGE].stage = sibdesc.BindPoint;
        }
        else
        {
            // this is a new texture
            D3D11TextureParameterDesc t;
            t.name                     = sD3D11CloneString( sibdesc.Name );
            t.ssp[SHADER_STAGE].used   = true;
            t.ssp[SHADER_STAGE].stage  = sibdesc.BindPoint;

            // append t to texture array
            paramDesc.addTexture( t );
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
static inline void
sUpdateD3D11ConstBuffer(
    ID3D11DeviceContext & cxt,
    ID3D11Buffer        & buf,
    const void          * data,
    size_t                size )
{
#if 0
        cxt.UpdateSubresource(
            &buf,   // destination buffer
            0,      // sub resource
            0,      // box
            data,   // source data
            size,   // row pitch
            size ); // slice pitch
#else
        D3D11_MAPPED_SUBRESOURCE dst;
        GN_DX_CHECK_RETURN( cxt.Map( &buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &dst ) );
        memcpy( dst.pData, data, size );
        cxt.Unmap( &buf, 0 );
#endif
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11GpuProgramHLSL::D3D11GpuProgramHLSL( D3D11Gpu & r )
    : D3D11Resource(r)
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11GpuProgramHLSL::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    // covert shader compile options
    uint32 compileFlags;
    compileFlags = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR; // use row major matrix all the time.
    if( !desc.optimize ) compileFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
    if( !desc.debug ) compileFlags |= D3D10_SHADER_DEBUG;
    if( GpuProgramLanguage::HLSL9 == desc.lang )
        compileFlags |= D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
    else
        compileFlags |= D3D10_SHADER_ENABLE_STRICTNESS;

    // compile all shaders
    if( !initShader<ShaderStage::VS>( mShaders[ShaderStage::VS], desc.code[ShaderStage::VS], desc.lang, compileFlags ) ) return failure();
    if( !initShader<ShaderStage::PS>( mShaders[ShaderStage::PS], desc.code[ShaderStage::PS], desc.lang, compileFlags ) ) return failure();
    if( !initShader<ShaderStage::GS>( mShaders[ShaderStage::GS], desc.code[ShaderStage::GS], desc.lang, compileFlags ) ) return failure();
    if( !initShader<ShaderStage::HS>( mShaders[ShaderStage::HS], desc.code[ShaderStage::HS], desc.lang, compileFlags ) ) return failure();
    if( !initShader<ShaderStage::DS>( mShaders[ShaderStage::DS], desc.code[ShaderStage::DS], desc.lang, compileFlags ) ) return failure();

    // build parameter array
    mParamDesc.buildParameterArrays();

    const size_t NUM_STAGES = getGpu().caps().maxTextures;
    if( mParamDesc.textures.count() > NUM_STAGES )
    {
        GN_ERROR(sLogger)( "The GPU program requires more textures than current hardware supports." );
        return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramHLSL::quit()
{
    GN_GUARD;

    for( int s = 0; s < GN_ARRAY_COUNT(mShaders); ++s )
    {
        mShaders[s].clear();
    }

    // clear parameter descriptor
    mParamDesc.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
const char * GN::gfx::D3D11GpuProgramHLSL::getAttributeSemantic(
    uint32 attributeIndex,
    UINT * semanticIndex ) const
{
    if( attributeIndex >= mParamDesc.attributes.count() )
    {
        GN_ERROR(sLogger)( "Invalid attribute index: %d", attributeIndex );
        if( semanticIndex ) *semanticIndex = 0;
        return NULL;
    }
    else
    {
        const D3D11AttributeParameterDesc & attribute = (const D3D11AttributeParameterDesc &)mParamDesc.attributes[attributeIndex];

        if( semanticIndex ) *semanticIndex = attribute.semanticIndex;

        return attribute.semanticName;
    }
}

//
//
// -----------------------------------------------------------------------------
const void * GN::gfx::D3D11GpuProgramHLSL::getInputSignature( size_t * pSignatureSize ) const
{
    if( mInputSignature )
    {
        if( pSignatureSize ) *pSignatureSize = mInputSignature->GetBufferSize();
        return mInputSignature->GetBufferPointer();
    }
    else
    {
        GN_ERROR(sLogger)( "The GPU program has no vertex shader." );
        if( pSignatureSize ) *pSignatureSize = 0;
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramHLSL::apply() const
{
    ID3D11DeviceContext & cxt = getDeviceContextRef();

    for( int s = 0; s < GN_ARRAY_COUNT(mShaders); ++s )
    {
        const ShaderHLSL & shader = mShaders[s];

        D3D11BindShader[s]( cxt, shader.shader );

        if( shader.constBufs.size() > 0 )
        {
            D3D11BindConstBuffers[s](
                cxt,
                0,
                (uint32)shader.constBufs.size(),
                &shader.constBufs[0] );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramHLSL::applyUniforms(
    const Uniform * const * uniforms,
    uint32                  count,
    bool                    skipDirtyCheck ) const
{
    count = math::getmin( count, mParamDesc.uniforms.count() );

    // dirty flags
    bool constDirty[GN_ARRAY_COUNT(mShaders)][D3D11ConstBufferArray::MAX_SIZE];
    ::memset( constDirty, 0, sizeof(constDirty) );

    // copy uniform data into constant data array
    for( size_t i = 0; i < count; ++i )
    {
        const D3D11UniformParameterDesc & ud = (const D3D11UniformParameterDesc &)mParamDesc.uniforms[i];
        const SysMemUniform             & u  = *(const SysMemUniform*)uniforms[i];

        for( int s = 0; s < GN_ARRAY_COUNT(mShaders); ++s )
        {
            sUpdateD3D11ConstData( ud, u, mShaders[s].constData, (ShaderStage::Enum)s, constDirty[s] );
        }
    }

    ID3D11DeviceContext & cxt = getDeviceContextRef();

    // update vertex shader constant buffers
    for( int s = 0; s < GN_ARRAY_COUNT(mShaders); ++s )
    {
        for( size_t i = 0; i < mShaders[s].constBufs.size(); ++i )
        {
            if( skipDirtyCheck || constDirty[s][i] )
            {
                ID3D11Buffer           & buf = *mShaders[s].constBufs[i];
                const DynaArray<uint8> & data = mShaders[s].constData[i];
                sUpdateD3D11ConstBuffer( cxt, buf, data.rawptr(), data.size() );
            }
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramHLSL::applyTextures(
    const TextureBinding * bindings,
    uint32                 count,
    bool                   skipDirtyCheck ) const
{
    const size_t NUM_STAGES = getGpu().caps().maxTextures;

    // allocate SRV array on stack, clear to zero.
    const size_t SRV_ARRAY_SIZE = sizeof(void*) * NUM_STAGES * ShaderStage::COUNT;
    ID3D11ShaderResourceView ** srvArray = (ID3D11ShaderResourceView **)alloca( SRV_ARRAY_SIZE );
    memset( srvArray, 0, SRV_ARRAY_SIZE );

    // determine effective texture count
    if( count > mParamDesc.textures.count() )
    {
        count = mParamDesc.textures.count();
    }
    GN_ASSERT( count <= NUM_STAGES );

    D3D11Gpu & gpu = getGpu();

    D3D11_SAMPLER_DESC sd;
    GN::d3d11::constructDefaultSamplerDesc( sd );

    // iterate textures
    for( size_t i = 0; i < count; ++i )
    {
        const TextureBinding & tb = bindings[i];

        D3D11Texture * tex = (D3D11Texture*)tb.texture.rawptr();

        if( tex )
        {
            const D3D11TextureParameterDesc & texParam = (const D3D11TextureParameterDesc &)mParamDesc.textures[i];
            for( int i = 0; i < ShaderStage::COUNT; ++i )
            {
                if( texParam.ssp[i].used )
                {
                    size_t srvidx = NUM_STAGES * i + texParam.ssp[i].stage;

                    srvArray[srvidx] = tex->getSRView();

                    // apply sampler
                    sd.AddressU = sAdressModeToD3D11( tb.sampler.addressU );
                    sd.AddressV = sAdressModeToD3D11( tb.sampler.addressV );
                    sd.AddressW = sAdressModeToD3D11( tb.sampler.addressW );
                    GN_TODO( "more sampler fields" );
                    gpu.setSampler( i, texParam.ssp[i].stage,  sd, skipDirtyCheck );
                }
            }
        }
    }

    ID3D11DeviceContext & cxt = getDeviceContextRef();

    if( mShaders[0].shader ) cxt.VSSetShaderResources( 0, (UINT)NUM_STAGES, &srvArray[NUM_STAGES * 0] );
    if( mShaders[1].shader ) cxt.PSSetShaderResources( 0, (UINT)NUM_STAGES, &srvArray[NUM_STAGES * 1] );
    if( mShaders[2].shader ) cxt.GSSetShaderResources( 0, (UINT)NUM_STAGES, &srvArray[NUM_STAGES * 2] );
    if( mShaders[3].shader ) cxt.HSSetShaderResources( 0, (UINT)NUM_STAGES, &srvArray[NUM_STAGES * 3] );
    if( mShaders[4].shader ) cxt.DSSetShaderResources( 0, (UINT)NUM_STAGES, &srvArray[NUM_STAGES * 4] );
}

// *****************************************************************************
// D3D11GpuProgramHLSL private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
template<int SHADER_STAGE>
bool GN::gfx::D3D11GpuProgramHLSL::initShader(
    ShaderHLSL                      & shader,
    const ShaderCode                & code,
    GpuProgramLanguage                targetLanguage,
    uint32                            compileFlags )
{
    // do nothing for empty shader code
    if( str::isEmpty( code.source ) ) return true;

    // define shader type traits
    typedef D3D11ShaderTypeTraits<SHADER_STAGE> Traits;

    ID3D11Device & dev = getDeviceRef();

    D3D11GpuProgramParameterDesc & paramDesc = mParamDesc;

    // compile shader
    AutoComPtr<ID3DBlob> binary;
    shader.shader.attach( Traits::create(
        dev,
        code.source,
        0,
        compileFlags,
        code.entry,
        Traits::getLatestProfile( dev, targetLanguage ),
        &binary ) );
    if( NULL == shader.shader ) return false;

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
    if( !sInitConstBuffers( dev, *reflection, shader.constBufs, shader.constData ) ) return false;

    // initialize uniform parameters
    if( !sInitD3D11Uniforms<SHADER_STAGE>( *reflection, paramDesc ) ) return false;

    // initialize texture parameters
    if( !sInitD3D11Textures<SHADER_STAGE>( *reflection, paramDesc ) ) return false;

    // initialize attribute parameters (vertex shader only)
    if( ShaderStage::VS == SHADER_STAGE )
    {
        D3D11_SHADER_DESC desc;
        reflection->GetDesc( &desc );

        for( UINT i = 0; i < desc.InputParameters; ++i )
        {
            D3D11_SIGNATURE_PARAMETER_DESC sig;
            reflection->GetInputParameterDesc( i, &sig );

            D3D11AttributeParameterDesc a;
            a.semanticName  = sig.SemanticName;
            a.semanticIndex = sig.SemanticIndex;
            a.name          = sD3D11CloneString( str::format( "%s%d", sig.SemanticName, sig.SemanticIndex ) );

            // append to attribute array
            paramDesc.addAttribute( a );

        }

        mInputSignature = binary;
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11GpuProgramHLSL::sInitConstBuffers(
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
    for( uint32 i = 0; i < desc.ConstantBuffers; ++i )
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
void GN::gfx::D3D11GpuProgramHLSL::sUpdateD3D11ConstData(
    const D3D11UniformParameterDesc & desc,
    const SysMemUniform             & uniform,
    SysMemConstBufferArray          & cbarray,
    ShaderStage::Enum                 shaderStage,
    bool                            * dirtyFlags )
{
    const D3D11UniformParameterDesc::ShaderSpecificProperties & ssp = desc.ssp[shaderStage];

    // do nothing, if the uniform is not used by the shader
    if( !ssp.used ) return;

    if( desc.size != uniform.size() )
    {
        GN_WARN(sLogger)(
            "parameter %s: value size(%d) differs from size defined in shader code(%d).",
            desc.name,
            uniform.size(),
            desc.size );
    }

    DynaArray<uint8>             & cb = cbarray[ssp.cbidx];
    SafeArrayAccessor<const uint8> src( (const uint8*)uniform.getval(), uniform.size() );
    SafeArrayAccessor<uint8>       dst( cb.rawptr(), cb.size() );

    // copy uniform data to system const buffer
    src.copyTo(
        0,          // src offset
        dst,        // dst buffer
        ssp.offset, // dst offset
        desc.size   // size in bytes
        );

    // update dirty flag
    dirtyFlags[ssp.cbidx] = true;
}
