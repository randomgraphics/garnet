#include "pch.h"
#include "d3d10Shader.h"
#include "d3d10Renderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// D3D10GpuProgramParameterDesc
// *****************************************************************************

template<class T>
static void sFreeNameString( T & t )
{
    GN::heapFree( (void*)t.name );
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
        return 0 == GN::strCmp( t.name, mName );
    }
};

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D10GpuProgramParameterDesc::D3D10GpuProgramParameterDesc()
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D10GpuProgramParameterDesc::~D3D10GpuProgramParameterDesc()
{
    // delete all name string
    std::for_each( mUniforms.begin(), mUniforms.end(), &sFreeNameString<D3D10UniformParameterDesc> );
    std::for_each( mTextures.begin(), mTextures.end(), &sFreeNameString<D3D10TextureParameterDesc> );
    std::for_each( mAttributes.begin(), mAttributes.end(), &sFreeNameString<D3D10AttributeParameterDesc> );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10GpuProgramParameterDesc::buildParameterArrays()
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
GN::gfx::D3D10UniformParameterDesc *
GN::gfx::D3D10GpuProgramParameterDesc::findUniform( const char * name )
{
    GN::gfx::D3D10UniformParameterDesc * p = std::find_if(
        mUniforms.begin(),
        mUniforms.end(),
        FindByName<D3D10UniformParameterDesc>(name) );

    return ( p == mUniforms.end() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D10TextureParameterDesc *
GN::gfx::D3D10GpuProgramParameterDesc::findTexture( const char * name )
{
    GN::gfx::D3D10TextureParameterDesc * p = std::find_if(
        mTextures.begin(),
        mTextures.end(),
        FindByName<D3D10TextureParameterDesc>(name) );

    return ( p == mTextures.end() ) ? NULL : p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D10AttributeParameterDesc *
GN::gfx::D3D10GpuProgramParameterDesc::findAttribute( const char * name )
{
    GN::gfx::D3D10AttributeParameterDesc * p = std::find_if(
        mAttributes.begin(),
        mAttributes.end(),
        FindByName<D3D10AttributeParameterDesc>(name) );

    return ( p == mAttributes.end() ) ? NULL : p;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10GpuProgram::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D10GpuProgram, () );

    // covert shader compile options
    D3D10ShaderCompileOptions options;
    options.compileFlags = 0;
    if( !desc.optimize ) options.compileFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
    if( !desc.debug ) options.compileFlags |= D3D10_SHADER_DEBUG;

    // compile all shaders
    ID3D10Device & dev = getDeviceRef();
    if( !mVs.init( dev, desc.vs, options, mParamDesc ) ) return failure();
    if( !mGs.init( dev, desc.gs, options, mParamDesc ) ) return failure();
    if( !mPs.init( dev, desc.ps, options, mParamDesc ) ) return failure();

    // build parameter array
    mParamDesc.buildParameterArrays();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10GpuProgram::quit()
{
    GN_GUARD;

    mVs.clear();
    mGs.clear();
    mPs.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10GpuProgram::applyUniforms( const SysMemUniform * const * uniforms, size_t count ) const
{
    GN_UNUSED_PARAM( uniforms );
    GN_UNUSED_PARAM( count );
    GN_UNIMPL_WARNING();
}
