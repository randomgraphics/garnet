#include "pch.h"
#include "oglShader.h"
#include "oglGpu.h"

#ifdef HAS_CG_OGL

using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");

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

//
//
// -----------------------------------------------------------------------------
static OGLVertexBindingDesc
sGetCgVertexAttributeBinding( CGparameter param )
{
    OGLVertexBindingDesc binding;

    CGresource res = cgGetParameterBaseResource( param );
    switch( res )
    {
        case CG_POSITION0:
            break;

        case CG_ATTR0:
            break;

        default:
            GN_UNEXPECTED();
            break;
    }

    // TODO: get real binding information
    binding.semantic = VERTEX_SEMANTIC_VERTEX;
    binding.index = 0;

    return binding;
}

// *****************************************************************************
// GN::gfx::OGLGpuProgramCg - Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgramCg::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLGpuProgramCg, () );

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
        enumCgParameters( mVs.getProgram() );
    }

    // Initialize fragment shader
    if( NULL != desc.ps.source )
    {
        if( !mPs.init( cgc, psprof, desc.ps.source, desc.ps.entry, NULL ) ) return failure();
        enumCgParameters( mPs.getProgram() );
    }

    // Fix up all char * pointers in parameter array
    for( OglCgUniform * i = mUniforms.begin(); i != mUniforms.end(); ++i)
    {
        i->desc.name = i->name.cptr();
    }
    for( OglCgTexture * i = mTextures.begin(); i != mTextures.end(); ++i)
    {
        i->desc.name = i->name.cptr();
    }
    for( OglCgAttribute * i = mAttributes.begin(); i != mAttributes.end(); ++i)
    {
        i->desc.name = i->name.cptr();
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
void GN::gfx::OGLGpuProgramCg::quit()
{
    GN_GUARD;

    mParam.setUniformArray( NULL, 0, 0 );
    mParam.setTexureArray( NULL, 0, 0 );
    mParam.setAttributeArray( NULL, 0, 0 );

    mUniforms.clear();
    mTextures.clear();
    mAttributes.clear();

    mVs.quit();
    mPs.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// GN::gfx::OGLGpuProgramCg - Public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgramCg::getBindingDesc(
    OGLVertexBindingDesc & result, const char * bindingName, UInt8 bindingIndex ) const
{
    for( OglCgAttribute * attr = mAttributes.begin(); attr != mAttributes.end(); ++attr )
    {
        if( attr->desc.name == bindingName && attr->binding.index == bindingIndex )
        {
            // found it.
            result = attr->binding;
            return true;
        }
    }

    // not found
    GN_ERROR( "Vertex bidning %s(%d) not found.", bindingName, bindingIndex );
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramCg::applyUniforms(
    const Uniform * const * uniforms, size_t count ) const
{
    for( size_t i = 0; i < std::min( count, mUniforms.size() ); ++i )
    {
        const OglCgUniform & desc = mUniforms[i];
        const Uniform & u = uniforms[i];

        for( CGparameter * param = desc.handles.begin(); param != desc.handles.end(); ++param )
        {
            GN_CG_CHECK( desc.setValueFuncPtr( *param, (int)desc.count, u.getval() ) );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramCg::applyTextures( const TextureBinding * textures, size_t count ) const
{
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// GN::gfx::OGLGpuProgramCg - Private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramCg::enumCgParameters( CGprogram prog )
{
    CGprofile profile = cgGetProgramProfile( prog );

    for( CGparameter param = cgGetFirstLeafParameter( prog, CG_PROGRAM );
         param != 0;
         param = cgGetNextLeafParameter( param ) )
    {
        const char * name = cgGetParameterName( param );

        CGenum var = cgGetParameterVariability( param );

        if( CG_UNIFORM == var )
        {
            CGparameterclass paramclass = cgGetParameterClass( param );
            if( CG_PARAMETERCLASS_SAMPLER == paramclass )
            {
                // A texture parameter

                OglCgTexture * existingTexture = std::find_if( mTextures.begin(), mTextures.end(), FindCgParameterByName(name) );

                if( existingTexture )
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
                }
            }
            else
            {
                // A uniform parameter

                OglCgUniform * existingUniform = std::find_if( mUniforms.begin(), mUniforms.end(), FindCgParameterByName(name) );

                if( existingUniform )
                {
                    GN_TODO( "Verify that the uniform type and size is same" );

                    existingUniform->handles.append( prog );
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
                }
            }
        }
        else if( CG_VARYING == var && CG_PROFILE_VERTEX == profile )
        {
            // Note: we care about varying variables only on vertex stage.

            // must be a new attribute.
            GN_ASSERT( mAttributes.end() == std::find_if( mAttributes.begin(), mAttributes.end(), FindCgParameterByName(name) ) );

            OglCgAttribute attr;
            attr.handles.append( param );
            attr.name = name;
            attr.binding = sGetCgVertexAttributeBinding( param );
            mAttributes.append( attr );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::OGLGpuProgramCg::SetCgTypelessParameterValue
GN::gfx::OGLGpuProgramCg::GetCgSetParameterFuncPtr( CGparameter param )
{
    switch( cgGetParameterBaseType( param ) )
    {
        case CG_FLOAT:
        case CG_HALF:
        case CG_FIXED:
            (SetCgTypelessParameterValue)cgSetParameterValuefr;
            return

        case CG_BOOL:
        case CG_INT:
            (SetCgTypelessParameterValue)cgSetParameterValueir;
             break;

        default:
            GN_UNEXPECTED();
            return sDummpySetCgTypelessParameterValue;
     }
}

#endif // HAS_CG_OGL
