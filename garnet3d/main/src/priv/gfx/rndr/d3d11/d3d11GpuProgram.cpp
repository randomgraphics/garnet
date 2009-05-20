#include "pch.h"
#include "d3d11Shader.h"
#include "d3d11Renderer.h"
#include "d3d11Texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D11");

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// D3D11GpuProgramParameterDesc
// *****************************************************************************

template<class T>
static void sFreeNameString( T & t )
{
    heapFree( (void*)t.name );
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
        return 0 == strCmp( t.name, mName );
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
    std::for_each( mUniforms.begin(), mUniforms.end(), &sFreeNameString<D3D11UniformParameterDesc> );
    std::for_each( mTextures.begin(), mTextures.end(), &sFreeNameString<D3D11TextureParameterDesc> );
    std::for_each( mAttributes.begin(), mAttributes.end(), &sFreeNameString<D3D11AttributeParameterDesc> );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramParameterDesc::buildParameterArrays()
{
    mUniformArray       = mUniforms.cptr();
    mUniformCount       = mUniforms.size();
    mUniformArrayStride = sizeof(mUniforms[0]);

    mTextureArray       = mTextures.cptr();
    mTextureCount       = mTextures.size();
    mTextureArrayStride = sizeof(mTextures[0]);

    mAttributeArray       = mAttributes.cptr();
    mAttributeCount       = mAttributes.size();
    mAttributeArrayStride = sizeof(mAttributes[0]);
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
        FindByName<D3D11UniformParameterDesc>(name) );

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
        FindByName<D3D11UniformParameterDesc>(name) );

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
        FindByName<D3D11TextureParameterDesc>(name) );

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
        FindByName<D3D11TextureParameterDesc>(name) );

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
        FindByName<D3D11AttributeParameterDesc>(name) );

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
        FindByName<D3D11AttributeParameterDesc>(name) );

    return ( p == mAttributes.end() ) ? NULL : p;
}

// *****************************************************************************
// D3D11GpuProgram
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void
sUpdateConstBuffer(
    ID3D11Device & dev,
    ID3D11Buffer & buf,
    const void   * data,
    size_t         size )
{
#if 0
        dev.UpdateSubresource(
            &buf,   // destination buffer
            0,      // sub resource
            0,      // box
            data,   // source data
            size,   // row pitch
            size ); // slice pitch
#else
        GN_UNUSED_PARAM( dev );
        void * dst;
        GN_DX10_CHECK_R( buf.Map( D3D11_MAP_WRITE_DISCARD, 0, &dst ) );
        memcpy( dst, data, size );
        buf.Unmap();
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

    DynaArray<UInt8>             & cb = cbarray[ssp.cbidx];
    SafeArrayAccessor<const UInt8> src( (const UInt8*)uniform.getval(), uniform.size() );
    SafeArrayAccessor<UInt8>       dst( cb.cptr(), cb.size() );

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

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11GpuProgram::init( const GpuProgramDesc & desc, bool hlsl9 )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D11GpuProgram, () );

    // covert shader compile options
    D3D11ShaderCompileOptions options;
    options.compileFlags = D3D11_SHADER_PACK_MATRIX_ROW_MAJOR; // use row major matrix all the time.
    if( !desc.optimize ) options.compileFlags |= D3D11_SHADER_SKIP_OPTIMIZATION;
    if( !desc.debug ) options.compileFlags |= D3D11_SHADER_DEBUG;
    if( hlsl9 )
        options.compileFlags |= D3D11_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
    else
        options.compileFlags |= D3D11_SHADER_ENABLE_STRICTNESS;

    // compile all shaders
    ID3D11Device & dev = getDeviceRef();
    if( !mVs.init( dev, desc.vs, options, mParamDesc ) ) return failure();
    if( !mGs.init( dev, desc.gs, options, mParamDesc ) ) return failure();
    if( !mPs.init( dev, desc.ps, options, mParamDesc ) ) return failure();

    // build parameter array
    mParamDesc.buildParameterArrays();

    const size_t NUM_STAGES = getRenderer().getCaps().maxTextures;
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
void GN::gfx::D3D11GpuProgram::quit()
{
    GN_GUARD;

    mVs.clear();
    mGs.clear();
    mPs.clear();

    // standard quit procedure
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
    count = math::getmin( count, mParamDesc.uniforms.count() );

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

    ID3D11Device & dev = getDeviceRef();

    // update vertex shader constant buffers
    for( size_t i = 0; i < mVs.constBufs.size(); ++i )
    {
        if( skipDirtyCheck || vscDirty[i] )
        {
            ID3D11Buffer           & buf = *mVs.constBufs[i];
            const DynaArray<UInt8> & data = mVs.constData[i];
            sUpdateConstBuffer( dev, buf, data.cptr(), data.size() );
        }
    }

    // update geometry shader constant buffers
    for( size_t i = 0; i < mGs.constBufs.size(); ++i )
    {
        if( skipDirtyCheck || gscDirty[i] )
        {
            ID3D11Buffer           & buf = *mGs.constBufs[i];
            const DynaArray<UInt8> & data = mGs.constData[i];
            sUpdateConstBuffer( dev, buf, data.cptr(), data.size() );
        }
    }

    // update pixel shader constant buffers
    for( size_t i = 0; i < mPs.constBufs.size(); ++i )
    {
        if( skipDirtyCheck || pscDirty[i] )
        {
            ID3D11Buffer           & buf = *mPs.constBufs[i];
            const DynaArray<UInt8> & data = mPs.constData[i];
            sUpdateConstBuffer( dev, buf, data.cptr(), data.size() );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgram::applyTextures(
    const TextureBinding * bindings,
    size_t                 count,
    bool                   /*skipDirtyCheck*/ ) const
{
    const size_t NUM_STAGES = getRenderer().getCaps().maxTextures;

    // allocate SRV array on stack, clear to zero.
    const size_t SRV_ARRAY_SIZE = sizeof(void*) * NUM_STAGES * 3;
    ID3D11ShaderResourceView ** srvArray = (ID3D11ShaderResourceView **)alloca( SRV_ARRAY_SIZE );
    memset( srvArray, 0, SRV_ARRAY_SIZE );

    // determine effective texture count
    if( count > mParamDesc.textures.count() )
    {
        count = mParamDesc.textures.count();
    }
    GN_ASSERT( count <= NUM_STAGES );

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
                }
            }
        }
    }

    ID3D11Device & dev = getDeviceRef();

    dev.VSSetShaderResources( 0, NUM_STAGES, srvArray );
    dev.GSSetShaderResources( 0, NUM_STAGES, srvArray + NUM_STAGES );
    dev.PSSetShaderResources( 0, NUM_STAGES, srvArray + NUM_STAGES * 2 );
}
