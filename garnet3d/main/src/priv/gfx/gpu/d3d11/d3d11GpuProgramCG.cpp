#include "pch.h"
#include "d3d11Shader.h"
#include "d3d11Gpu.h"
#include "d3d11Texture.h"

#ifdef HAS_CG_D3D

using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// Local utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static size_t sGetCgParameterCount( CGparameter param )
{
    int rows  = cgGetParameterRows( param );
    int cols  = cgGetParameterColumns( param );
    int total = cgGetArrayTotalSize( param );
    if( 0 == total ) total = 1;

    return (size_t)( rows * cols * total );
}

//
//
// -----------------------------------------------------------------------------
static void CGENTRY sDummpySetCgTypelessParameterValue(CGparameter param, int n, const void * vals)
{
    // do nothing
    GN_UNUSED_PARAM( param );
    GN_UNUSED_PARAM( n );
    GN_UNUSED_PARAM( vals );
}

// *****************************************************************************
// GN::gfx::D3D11GpuProgramCG - Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11GpuProgramCG::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D11GpuProgramCG, () );

    CGcontext cgc = getGpu().getCgContext();
    if( NULL == cgc )
    {
        GN_ERROR(sLogger)( "Null Cg context." );
        return failure();
    }

    // get the latest vertex profile
    CGprofile profiles[ShaderStage::COUNT] =
    {
        CG_PROFILE_VS_4_0, //cgD3D11GetLatestVertexProfile(),
        CG_PROFILE_PS_4_0, //cgD3D11GetLatestPixelProfile(),
        CG_PROFILE_GS_4_0, //cgD3D11GetLatestGeometryProfile(),
        CG_PROFILE_UNKNOWN, //cgD3D11GetLatestHullProfile(),
        CG_PROFILE_UNKNOWN, //cgD3D11GetLatestDomainProfile(),
    };
    if( CG_PROFILE_UNKNOWN == profiles[ShaderStage::VS] || CG_PROFILE_UNKNOWN == profiles[ShaderStage::PS] )
    {
        GN_ERROR(sLogger)( "Programmable vertex and pixel shader are required!" );
        return failure();
    }

    uint32 d3dCompileFlags;
    d3dCompileFlags = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR; // use row major matrix at all time.
    if( !desc.optimize ) d3dCompileFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
    if( !desc.debug ) d3dCompileFlags |= D3D10_SHADER_DEBUG;

    // Initialize vertex shader
    for( int i = 0; i < ShaderStage::COUNT; ++i )
    {
        if( NULL != desc.code[i].source )
        {
            if( CG_PROFILE_UNKNOWN == profiles[i] )
            {
                GN_ERROR(sLogger)( "Programmable shader stage %d is not supported!", i );
                return failure();
            }

            if( !mShaders[i].init( cgc, profiles[i], desc.code[i].source, desc.code[i].entry, NULL ) ) return failure();

            GN_DX_CHECK_RETURN( cgD3D11LoadProgram( mShaders[i].getProgram(), d3dCompileFlags ), failure() );

            enumCgParameters( mShaders[i].getProgram(), CG_GLOBAL );
            enumCgParameters( mShaders[i].getProgram(), CG_PROGRAM );
        }
    }

    // get the vertex shader input signature
    if( mShaders[ShaderStage::VS].getProgram() )
    {
        mInputSignature.attach( cgD3D11GetCompiledProgram( mShaders[ShaderStage::VS].getProgram() ) );
    }

    // Fix up all char * pointers in parameter array
    for( D3D11CgUniform * i = mUniforms.begin(); i != mUniforms.end(); ++i)
    {
        i->desc.name = i->name.cptr();
    }
    for( D3D11CgTexture * i = mTextures.begin(); i != mTextures.end(); ++i)
    {
        i->desc.name = i->name.cptr();
    }
    for( D3D11CgAttribute * i = mAttributes.begin(); i != mAttributes.end(); ++i)
    {
        i->desc.name = i->name.cptr();
    }

    // update the parameter descriptor
    if( !mUniforms.empty() )
    {
        mParam.setUniformArray( &mUniforms[0].desc, mUniforms.size(), sizeof(D3D11CgUniform) );
    }
    if( !mTextures.empty() )
    {
        mParam.setTextureArray( &mTextures[0].desc, mTextures.size(), sizeof(D3D11CgTexture) );
    }
    if( !mAttributes.empty() )
    {
        mParam.setAttributeArray( &mAttributes[0].desc, mAttributes.size(), sizeof(D3D11CgAttribute) );
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramCG::quit()
{
    GN_GUARD;

    mParam.setUniformArray( NULL, 0, 0 );
    mParam.setTextureArray( NULL, 0, 0 );
    mParam.setAttributeArray( NULL, 0, 0 );

    mUniforms.clear();
    mTextures.clear();
    mAttributes.clear();

    mInputSignature.clear();

    for( int i = 0; i < ShaderStage::COUNT; ++i )
    {
        CGprogram program = mShaders[i].getProgram();
        if( program ) cgD3D11UnloadProgram( program );
        mShaders[i].quit();
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// GN::gfx::D3D11GpuProgramCG - Public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const char * GN::gfx::D3D11GpuProgramCG::getAttributeSemantic(
    size_t attributeIndex, UINT * semanticIndex ) const
{
    if( attributeIndex >= mAttributes.size() )
    {
        GN_ERROR(sLogger)( "Invalid attribute index: %d", attributeIndex );
        if( semanticIndex ) *semanticIndex = 0;
        return NULL;
    }
    else
    {
        const D3D11CgAttribute & attrib = mAttributes[attributeIndex];
        if( semanticIndex ) *semanticIndex = attrib.semanticIndex;
        return attrib.semantic;
    }
}

//
//
// -----------------------------------------------------------------------------
const void * GN::gfx::D3D11GpuProgramCG::getInputSignature( size_t * pSignatureSize ) const
{
    const void * sig;
    size_t size;

    if( mInputSignature )
    {
        sig = mInputSignature->GetBufferPointer();
        size = mInputSignature->GetBufferSize();
    }
    else
    {
        sig = NULL;
        size = 0;
    }

    if( pSignatureSize ) *pSignatureSize = size;

    return sig;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramCG::apply() const
{
    ID3D11DeviceContext & cxt = getDeviceContextRef();

    for( int i = 0; i < ShaderStage::COUNT; ++i )
    {
        CGprogram program = mShaders[i].getProgram();
        if( 0 != program )
        {
            cgD3D11BindProgram( program );
        }
        else switch( i )
        {
            case ShaderStage::VS: cxt.VSSetShader( NULL, NULL, 0 );
            case ShaderStage::PS: cxt.PSSetShader( NULL, NULL, 0 );
            case ShaderStage::GS: cxt.GSSetShader( NULL, NULL, 0 );
            case ShaderStage::HS: cxt.HSSetShader( NULL, NULL, 0 );
            case ShaderStage::DS: cxt.DSSetShader( NULL, NULL, 0 );
        }
    };
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramCG::applyUniforms(
    const Uniform * const * uniforms,
    size_t                  count,
    bool                    skipDirtyCheck) const
{
    for( size_t i = 0; i < std::min( count, mUniforms.size() ); ++i )
    {
        const D3D11CgUniform & desc = mUniforms[i];

        const Uniform * u = uniforms[i];

        if( NULL == u )
        {
            GN_ERROR(sLogger)( "Null uniform pointer." );
            continue;
        }

        for( const CGparameter * param = desc.handles.begin(); param != desc.handles.end(); ++param )
        {
            GN_CG_CHECK( desc.setValueFuncPtr( *param, (int)desc.count, u->getval() ) );
        }
    }

    if( count != mUniforms.size() )
    {
        GN_ERROR(sLogger)( "%s uniforms: expected(%d), actual(%d).",
            count > mUniforms.size() ? "Too many" : "Not enough",
            mUniforms.size(),
            count );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramCG::applyTextures(
    const TextureBinding * textures,
    size_t                 count,
    bool                   skipDirtyCheck ) const
{
    for( size_t i = 0; i < mTextures.size(); ++i )
    {
        const D3D11CgTexture & desc = mTextures[i];

        ID3D11Resource * d3dtex = ( i < count ) ? ((const D3D11Texture*)(textures[i].texture.get()))->getD3D11Resource() : 0;

        for( const CGparameter * param = desc.handles.begin(); param != desc.handles.end(); ++param )
        {
            GN_CG_CHECK( cgD3D11SetTextureParameter( *param, d3dtex ) );

            // TODO: set sampler
        }
    }
}

// *****************************************************************************
// GN::gfx::D3D11GpuProgramCG - Private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11GpuProgramCG::enumCgParameters( CGprogram prog, CGenum name_space )
{
    bool vs = cgGetProgramProfile( prog ) == mShaders[ShaderStage::VS].getProfile();

    for( CGparameter param = cgGetFirstParameter( prog, name_space );
         param != 0;
         param = cgGetNextParameter( param ) )
    {
        // Ignore non-input parameters
        CGenum direction = cgGetParameterDirection( param );
        if( CG_IN != direction && CG_INOUT != direction ) return;

        const char * name = cgGetParameterName( param );

        CGenum var = cgGetParameterVariability( param );

        CGparameterclass paramclass = cgGetParameterClass( param );

        if( CG_PARAMETERCLASS_SAMPLER == paramclass )
        {
            // This is a texture parameter

            D3D11CgTexture * existingTexture = std::find_if( mTextures.begin(), mTextures.end(), FindCgParameterByName(name) );

            if( existingTexture != mTextures.end() )
            {
                GN_TODO( "verify that the texture dimension is same." );

                existingTexture->handles.append( param );
            }
            else
            {
                // This is a new texture parameter.
                // Note: texture.desc.name should be updated after all parameters are found.
                D3D11CgTexture texture;
                texture.handles.append( param );
                texture.name = name;
                mTextures.append( texture );
            }
        }
        else if( CG_UNIFORM == var )
        {
            // This is a uniform parameter.

            D3D11CgUniform * existingUniform = std::find_if( mUniforms.begin(), mUniforms.end(), FindCgParameterByName(name) );

            if( existingUniform != mUniforms.end() )
            {
                GN_TODO( "Verify that the uniform type and size is same" );

                existingUniform->handles.append( param );
            }
            else
            {
                // This is a new uniform parameter.
                // Note: uniform.desc.name should be updated after all parameters are found.
                D3D11CgUniform uniform;
                uniform.handles.append( param );
                uniform.name = name;
                uniform.count = sGetCgParameterCount( param );
                uniform.setValueFuncPtr = GetCgSetParameterFuncPtr( param );
                uniform.desc.size = uniform.count * 4;
                mUniforms.append( uniform );
            }
        }
        else if( vs && CG_VARYING == var )
        {
            // This is a vertex parameter

            // must be a new attribute.
            GN_ASSERT( mAttributes.end() == std::find_if( mAttributes.begin(), mAttributes.end(), FindCgParameterByName(name) ) );

            D3D11CgAttribute attr;
            attr.handles.append( param );
            attr.name = cgGetResourceString( cgGetParameterResource( param ) );
            attr.semantic = cgGetParameterSemantic( param );
            attr.semanticIndex = cgGetParameterResourceIndex( param );
            mAttributes.append( attr );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11GpuProgramCG::SetCgTypelessParameterValue
GN::gfx::D3D11GpuProgramCG::GetCgSetParameterFuncPtr( CGparameter param )
{
    switch( cgGetParameterBaseType( param ) )
    {
        case CG_FLOAT:
        case CG_HALF:
        case CG_FIXED:
            return (SetCgTypelessParameterValue)&cgSetParameterValuefr;

        case CG_BOOL:
        case CG_INT:
            return (SetCgTypelessParameterValue)&cgSetParameterValueir;

        default:
            GN_UNEXPECTED();
            return sDummpySetCgTypelessParameterValue;
     }
}

#endif // HAS_CG_D3D
