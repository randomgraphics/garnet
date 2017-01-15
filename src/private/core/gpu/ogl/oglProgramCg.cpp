#include "pch.h"
#include "oglShader.h"
#include "oglGpu.h"
#include "oglTexture.h"

#ifdef HAS_CG_OGL

using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");

// *****************************************************************************
// Local utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static uint32 sGetCgParameterCount( CGparameter param )
{
    int rows  = cgGetParameterRows( param );
    int cols  = cgGetParameterColumns( param );
    int total = cgGetArrayTotalSize( param );
    if( 0 == total ) total = 1;

    return (uint32)( rows * cols * total );
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

//
//
// -----------------------------------------------------------------------------
static OGLVertexBindingDesc
sGetCgVertexAttributeBinding( CGparameter param )
{
    OGLVertexBindingDesc binding;
    memset( &binding, 0, sizeof(binding) );

    CGresource res = cgGetParameterBaseResource( param );
    switch( res )
    {
        case CG_POSITION0:
            binding.semantic = VERTEX_SEMANTIC_VERTEX;
            binding.index = (uint8)cgGetParameterResourceIndex( param );
            break;

        case CG_NORMAL0:
            binding.semantic = VERTEX_SEMANTIC_NORMAL;
            binding.index = (uint8)cgGetParameterResourceIndex( param );
            break;

        case CG_DIFFUSE0:
            binding.semantic = VERTEX_SEMANTIC_COLOR;
            binding.index = 0;
            break;

        case CG_SPECULAR0:
            binding.semantic = VERTEX_SEMANTIC_COLOR;
            binding.index = 1;
            break;

        case CG_COLOR0:
            binding.semantic = VERTEX_SEMANTIC_COLOR;
            binding.index = (uint8)cgGetParameterResourceIndex( param );
            break;

        case CG_TEXCOORD0:
            binding.semantic = VERTEX_SEMANTIC_TEXCOORD;
            binding.index = (uint8)cgGetParameterResourceIndex( param );
            break;

        case CG_FOGCOORD:
        case CG_FOG0:
            binding.semantic = VERTEX_SEMANTIC_FOG;
            binding.index = 0;
            break;

        case CG_ATTR0:
            binding.semantic = VERTEX_SEMANTIC_ATTRIBUTE;
            binding.index = (uint8)cgGetParameterResourceIndex( param );
            break;

        default:
            GN_UNEXPECTED();
            break;
    }

    return binding;
}

// *****************************************************************************
// GN::gfx::OGLGpuProgramCG - Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgramCG::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLGpuProgramCG, () );

    CGcontext cgc = getGpu().getCgContext();
    if( NULL == cgc )
    {
        GN_ERROR(sLogger)( "Null Cg context." );
        return failure();
    }

    // get the latest vertex profile
    CGprofile vsprof = cgGLGetLatestProfile( CG_GL_VERTEX );
    CGprofile psprof = cgGLGetLatestProfile( CG_GL_FRAGMENT );
    if( CG_PROFILE_UNKNOWN == vsprof || CG_PROFILE_UNKNOWN == psprof )
    {
        GN_ERROR(sLogger)( "Fail to get the lastest profile!" );
        return failure();
    }

    // Initialize vertex shader
    if( NULL != desc.vs.source )
    {
        if( !mVs.init( cgc, vsprof, desc.vs.source, desc.vs.entry, NULL ) ) return failure();
        GN_CG_CHECK_RV( cgGLLoadProgram( mVs.getProgram() ), failure() );
        enumCgParameters( mVs.getProgram(), CG_GLOBAL );
        enumCgParameters( mVs.getProgram(), CG_PROGRAM );
    }

    // Initialize fragment shader
    if( NULL != desc.ps.source )
    {
        if( !mPs.init( cgc, psprof, desc.ps.source, desc.ps.entry, NULL ) ) return failure();
        GN_CG_CHECK_RV( cgGLLoadProgram( mPs.getProgram() ), failure() );
        enumCgParameters( mPs.getProgram(), CG_GLOBAL );
        enumCgParameters( mPs.getProgram(), CG_PROGRAM );
    }

    // Fix up all char * pointers in parameter array
    GN_VTRACE(sLogger)("Dump GPU program parameters:");
    for( OglCgUniform * i = mUniforms.begin(); i != mUniforms.end(); ++i)
    {
        i->desc.name = i->name.rawptr();
        GN_VTRACE(sLogger)( "\tuniform  : %s", i->desc.name );
    }
    for( OglCgTexture * i = mTextures.begin(); i != mTextures.end(); ++i)
    {
        i->desc.name = i->name.rawptr();
        GN_VTRACE(sLogger)( "\ttexture  : %s", i->desc.name );
    }
    for( OglCgAttribute * i = mAttributes.begin(); i != mAttributes.end(); ++i)
    {
        i->desc.name = i->name.rawptr();
        GN_VTRACE(sLogger)( "\tattribute: %s", i->desc.name );
    }

    // update the parameter descriptor
    if( !mUniforms.empty() )
    {
        mParam.setUniformArray( &mUniforms[0].desc, mUniforms.size(), sizeof(OglCgUniform) );
    }
    if( !mTextures.empty() )
    {
        mParam.setTextureArray( &mTextures[0].desc, mTextures.size(), sizeof(OglCgTexture) );
    }
    if( !mAttributes.empty() )
    {
        mParam.setAttributeArray( &mAttributes[0].desc, mAttributes.size(), sizeof(OglCgAttribute) );
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramCG::quit()
{
    GN_GUARD;

    mParam.setUniformArray( NULL, 0, 0 );
    mParam.setTextureArray( NULL, 0, 0 );
    mParam.setAttributeArray( NULL, 0, 0 );

    mUniforms.clear();
    mTextures.clear();
    mAttributes.clear();

    // TODO: Unload the programs
    mVs.quit();
    mPs.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// GN::gfx::OGLGpuProgramCG - Public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgramCG::getBindingDesc(
    OGLVertexBindingDesc & result, uint32 attributeIndex ) const
{
    if( attributeIndex >= mAttributes.size() )
    {
        GN_ERROR(sLogger)( "Invalid attribute index." );
        result.index = 255;
        result.semantic = (OGLVertexSemantic)-1;
        return false;
    }

    const OglCgAttribute & attrib = mAttributes[attributeIndex];
    result = attrib.binding;

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramCG::applyUniforms(
    const Uniform * const * uniforms, uint32 count ) const
{
    for( uint32 i = 0; i < math::getmin<>( count, mUniforms.size() ); ++i )
    {
        const OglCgUniform & desc = mUniforms[i];

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
void GN::gfx::OGLGpuProgramCG::applyTextures( const TextureBinding * textures, uint32 count ) const
{
    for( uint32 i = 0; i < mTextures.size(); ++i )
    {
        const OglCgTexture & desc = mTextures[i];

        GLuint ogltex = ( i < count ) ? ((const OGLTexture*)(textures[i].texture.rawptr()))->getOGLTexture() : 0;

        for( const CGparameter * param = desc.handles.begin(); param != desc.handles.end(); ++param )
        {
            GN_CG_CHECK( cgGLSetTextureParameter( *param, ogltex ) );
            GN_CG_CHECK( cgGLEnableTextureParameter( *param ) );
        }
    }
}

// *****************************************************************************
// GN::gfx::OGLGpuProgramCG - Private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramCG::enumCgParameters( CGprogram prog, CGenum name_space )
{
    bool vs = cgGetProgramProfile( prog ) == mVs.getProfile();

    for( CGparameter param = cgGetFirstLeafParameter( prog, name_space );
         param != 0;
         param = cgGetNextLeafParameter( param ) )
    {
        const char * name = cgGetParameterName( param );

        // Ignore non-referenced parameters
        if( !cgIsParameterReferenced( param ) )
        {
            GN_VTRACE(sLogger)( "Ignore unreferenced parameter: %s", name );
            continue;
        }

        // Ignore non-input parameters
        CGenum direction = cgGetParameterDirection( param );
        if( CG_IN != direction && CG_INOUT != direction )
        {
            GN_VTRACE(sLogger)( "Ignore non-input parameter: %s", name );
            return;
        }

        CGenum var = cgGetParameterVariability( param );

        CGparameterclass paramclass = cgGetParameterClass( param );

        if( CG_PARAMETERCLASS_SAMPLER == paramclass )
        {
            // This is a texture parameter

            OglCgTexture * existingTexture = std::find_if( mTextures.begin(), mTextures.end(), FindCgParameterByName(name) );

            if( existingTexture != mTextures.end() )
            {
                GN_TODO( "verify that the texture dimension is same." );

                existingTexture->handles.append( param );
            }
            else
            {
                // This is a new texture parameter.
                // Note: texture.desc.name should be updated after all parameters are found.
                OglCgTexture texture;
                texture.handles.append( param );
                texture.name = name;
                mTextures.append( texture );
                GN_VTRACE(sLogger)( "new texture: %s", name );
            }
        }
        else if( CG_UNIFORM == var )
        {
            // This is a uniform parameter.

            OglCgUniform * existingUniform = std::find_if( mUniforms.begin(), mUniforms.end(), FindCgParameterByName(name) );

            if( existingUniform != mUniforms.end() )
            {
                GN_TODO( "Verify that the uniform type and size is same" );

                existingUniform->handles.append( param );
            }
            else
            {
                // This is a new uniform parameter.
                // Note: uniform.desc.name should be updated after all parameters are found.
                OglCgUniform uniform;
                uniform.handles.append( param );
                uniform.name = name;
                uniform.count = sGetCgParameterCount( param );
                uniform.setValueFuncPtr = GetCgSetParameterFuncPtr( param );
                uniform.desc.size = uniform.count * 4;
                mUniforms.append( uniform );
                GN_VTRACE(sLogger)( "new uniform: %s", name );
            }
        }
        else if( vs && CG_VARYING == var )
        {
            // This is a vertex parameter

            // must be a new attribute.
            GN_ASSERT( mAttributes.end() == std::find_if( mAttributes.begin(), mAttributes.end(), FindCgParameterByName(name) ) );

            OglCgAttribute attr;
            attr.handles.append( param );
            attr.name = name;//cgGetResourceString( cgGetParameterResource( param ) );
            attr.binding = sGetCgVertexAttributeBinding( param );
            mAttributes.append( attr );
            GN_VTRACE(sLogger)( "new attribute: name=%s", name );
        }
        else
        {
            GN_VTRACE(sLogger)( "ignore parameter with unkonw type: %s", name );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::OGLGpuProgramCG::SetCgTypelessParameterValue
GN::gfx::OGLGpuProgramCG::GetCgSetParameterFuncPtr( CGparameter param )
{
    switch( cgGetParameterBaseType( param ) )
    {
        case CG_FLOAT:
        case CG_HALF:
        case CG_FIXED:
            return (SetCgTypelessParameterValue)cgSetParameterValuefr;

        case CG_BOOL:
        case CG_INT:
            return (SetCgTypelessParameterValue)cgSetParameterValueir;

        default:
            GN_UNEXPECTED();
            return sDummpySetCgTypelessParameterValue;
     }
}

#endif // HAS_CG_OGL
