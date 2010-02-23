#include "pch.h"
#include "d3d11Shader.h"
#include "d3d11Gpu.h"
#include "d3d11Texture.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.D3D11");

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

template<class T>
static void sFreeNameString( T & t )
{
    HeapFree( (void*)t.name );
}

template<class T>
class FindByName
{
    const char * mName;

public:

    FindByName( const char * name ) : mName(name)
    {
    }

    bool operator()( const T & t ) const
    {
        return 0 == StringCompare( t.name, mName );
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
    // delete all name string
    std::for_each( mUniforms.Begin(), mUniforms.End(), &sFreeNameString<D3D11UniformParameterDesc> );
    std::for_each( mTextures.Begin(), mTextures.End(), &sFreeNameString<D3D11TextureParameterDesc> );
    std::for_each( mAttributes.Begin(), mAttributes.End(), &sFreeNameString<D3D11AttributeParameterDesc> );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramParameterDesc::buildParameterArrays()
{
    mUniformArray       = mUniforms.GetRawPtr();
    mUniformCount       = mUniforms.Size();
    mUniformArrayStride = sizeof(mUniforms[0]);

    mTextureArray       = mTextures.GetRawPtr();
    mTextureCount       = mTextures.Size();
    mTextureArrayStride = sizeof(mTextures[0]);

    mAttributeArray       = mAttributes.GetRawPtr();
    mAttributeCount       = mAttributes.Size();
    mAttributeArrayStride = sizeof(mAttributes[0]);
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D11UniformParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findUniform( const char * name ) const
{
    const GN::gfx::D3D11UniformParameterDesc * p = std::find_if(
        mUniforms.Begin(),
        mUniforms.End(),
        FindByName<D3D11UniformParameterDesc>(name) );

    return ( p == mUniforms.End() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11UniformParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findUniform( const char * name )
{
    GN::gfx::D3D11UniformParameterDesc * p = std::find_if(
        mUniforms.Begin(),
        mUniforms.End(),
        FindByName<D3D11UniformParameterDesc>(name) );

    return ( p == mUniforms.End() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D11TextureParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findTexture( const char * name ) const
{
    const GN::gfx::D3D11TextureParameterDesc * p = std::find_if(
        mTextures.Begin(),
        mTextures.End(),
        FindByName<D3D11TextureParameterDesc>(name) );

    return ( p == mTextures.End() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11TextureParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findTexture( const char * name )
{
    GN::gfx::D3D11TextureParameterDesc * p = std::find_if(
        mTextures.Begin(),
        mTextures.End(),
        FindByName<D3D11TextureParameterDesc>(name) );

    return ( p == mTextures.End() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D11AttributeParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findAttribute( const char * name ) const
{
    const GN::gfx::D3D11AttributeParameterDesc * p = std::find_if(
        mAttributes.Begin(),
        mAttributes.End(),
        FindByName<D3D11AttributeParameterDesc>(name) );

    return ( p == mAttributes.End() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11AttributeParameterDesc *
GN::gfx::D3D11GpuProgramParameterDesc::findAttribute( const char * name )
{
    GN::gfx::D3D11AttributeParameterDesc * p = std::find_if(
        mAttributes.Begin(),
        mAttributes.End(),
        FindByName<D3D11AttributeParameterDesc>(name) );

    return ( p == mAttributes.End() ) ? NULL : p;
}

// *****************************************************************************
// D3D11GpuProgram
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void
sUpdateConstBuffer(
    ID3D11DeviceContext & cxt,
    ID3D11Buffer & buf,
    const void   * data,
    size_t         size )
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
        GN_DX_CHECK_RETURN_VOID( cxt.Map( &buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &dst ) );
        memcpy( dst.pData, data, size );
        cxt.Unmap( &buf, 0 );
#endif
}

//
//
// -----------------------------------------------------------------------------
static inline void
sUpdateConstData(
    const D3D11UniformParameterDesc & desc,
    const SysMemUniform             & uniform,
    SysMemConstBufferArray          & cbarray,
    size_t                            shaderType, // 0: VS, 1: GS, 2: PS
    bool                            * dirtyFlags )
{
    const D3D11UniformParameterDesc::ShaderSpecificProperties & ssp = desc.ssp[shaderType];

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

    DynaArray<UInt8>             & cb = cbarray[ssp.cbidx];
    SafeArrayAccessor<const UInt8> src( (const UInt8*)uniform.getval(), uniform.size() );
    SafeArrayAccessor<UInt8>       dst( cb.GetRawPtr(), cb.Size() );

    // copy uniform data to system const buffer
    src.CopyTo(
        0,          // src offset
        dst,        // dst buffer
        ssp.offset, // dst offset
        desc.size   // size in bytes
        );

    // update dirty flag
    dirtyFlags[ssp.cbidx] = true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11GpuProgram::Init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D11GpuProgram, () );

    // covert shader compile options
    D3D11ShaderCompileOptions options;
    options.compileFlags = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR; // use row major matrix all the time.
    if( !desc.optimize ) options.compileFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
    if( !desc.debug ) options.compileFlags |= D3D10_SHADER_DEBUG;
    if( GpuProgramLanguage::HLSL9 == desc.lang )
        options.compileFlags |= D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
    else
        options.compileFlags |= D3D10_SHADER_ENABLE_STRICTNESS;

    // compile all shaders
    ID3D11Device & dev = getDeviceRef();
    if( !mVs.Init( dev, desc.vs, options, mParamDesc ) ) return Failure();
    if( !mGs.Init( dev, desc.gs, options, mParamDesc ) ) return Failure();
    if( !mPs.Init( dev, desc.ps, options, mParamDesc ) ) return Failure();

    // build parameter array
    mParamDesc.buildParameterArrays();

    const size_t NUM_STAGES = getGpu().GetCaps().maxTextures;
    if( mParamDesc.textures.count() > NUM_STAGES )
    {
        GN_ERROR(sLogger)( "The GPU program requires more textures than current hardware supports." );
        return Failure();
    }

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgram::Quit()
{
    GN_GUARD;

    mVs.Clear();
    mGs.Clear();
    mPs.Clear();

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgram::applyUniforms(
    const Uniform * const * uniforms,
    size_t                  count,
    bool                    skipDirtyCheck ) const
{
    count = math::GetMin( count, mParamDesc.uniforms.count() );

    // dirty flags
    bool vscDirty[D3D11ConstBufferArray::MAX_SIZE];
    bool gscDirty[D3D11ConstBufferArray::MAX_SIZE];
    bool pscDirty[D3D11ConstBufferArray::MAX_SIZE];
    ::memset( vscDirty, 0, sizeof(vscDirty) );
    ::memset( gscDirty, 0, sizeof(gscDirty) );
    ::memset( pscDirty, 0, sizeof(pscDirty) );

    // copy uniform data into constant data array
    for( size_t i = 0; i < count; ++i )
    {
        const D3D11UniformParameterDesc & ud = (const D3D11UniformParameterDesc &)mParamDesc.uniforms[i];
        const SysMemUniform             & u  = *(const SysMemUniform*)uniforms[i];

        sUpdateConstData( ud, u, mVs.constData, 0, vscDirty );
        sUpdateConstData( ud, u, mGs.constData, 1, gscDirty );
        sUpdateConstData( ud, u, mPs.constData, 2, pscDirty );
    }

    ID3D11DeviceContext & cxt = getDeviceContextRef();

    // update vertex shader constant buffers
    for( size_t i = 0; i < mVs.constBufs.Size(); ++i )
    {
        if( skipDirtyCheck || vscDirty[i] )
        {
            ID3D11Buffer           & buf = *mVs.constBufs[i];
            const DynaArray<UInt8> & data = mVs.constData[i];
            sUpdateConstBuffer( cxt, buf, data.GetRawPtr(), data.Size() );
        }
    }

    // update geometry shader constant buffers
    for( size_t i = 0; i < mGs.constBufs.Size(); ++i )
    {
        if( skipDirtyCheck || gscDirty[i] )
        {
            ID3D11Buffer           & buf = *mGs.constBufs[i];
            const DynaArray<UInt8> & data = mGs.constData[i];
            sUpdateConstBuffer( cxt, buf, data.GetRawPtr(), data.Size() );
        }
    }

    // update pixel shader constant buffers
    for( size_t i = 0; i < mPs.constBufs.Size(); ++i )
    {
        if( skipDirtyCheck || pscDirty[i] )
        {
            ID3D11Buffer           & buf = *mPs.constBufs[i];
            const DynaArray<UInt8> & data = mPs.constData[i];
            sUpdateConstBuffer( cxt, buf, data.GetRawPtr(), data.Size() );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgram::applyTextures(
    const TextureBinding * bindings,
    size_t                 count,
    bool                   skipDirtyCheck ) const
{
    const size_t NUM_STAGES = getGpu().GetCaps().maxTextures;

    // allocate SRV array on stack, Clear to zero.
    const size_t SRV_ARRAY_SIZE = sizeof(void*) * NUM_STAGES * 3;
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

        D3D11Texture * tex = (D3D11Texture*)tb.texture.get();

        if( tex )
        {
            const D3D11TextureParameterDesc & texParam = (const D3D11TextureParameterDesc &)mParamDesc.textures[i];
            for( int i = 0; i < 3; ++i )
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

    cxt.VSSetShaderResources( 0, (UINT)NUM_STAGES, srvArray );
    cxt.GSSetShaderResources( 0, (UINT)NUM_STAGES, srvArray + NUM_STAGES );
    cxt.PSSetShaderResources( 0, (UINT)NUM_STAGES, srvArray + NUM_STAGES * 2 );
}
