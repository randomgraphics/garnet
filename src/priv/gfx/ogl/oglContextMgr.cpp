#include "pch.h"
#include "oglRenderer.h"
#if !GN_ENABLE_INLINE
#include "oglContextMgr.inl"
#endif
#include "oglShader.h"
#include "oglTexture.h"
#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"

// *****************************************************************************
// local function
// *****************************************************************************

//!
//! render state value map
//!
static GLenum sRsv2OGL[GN::gfx::NUM_RENDER_STATE_VALUES] =
{
    #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) glval,
    #include "garnet/gfx/renderStateValueMeta.h"
    #undef GNGFX_DEFINE_RSV
};

// Individual RS/TSS functions
#include "oglRenderState.inl"

//!
//! opengl texture operation structure
//!
struct OGLTextureState
{
    GLenum op1; //!< OpenGL texture opcode 1
    GLenum op2; //!< OpenGL texture opcode 2
};

//!
//! opengl texture stage state value structure
struct OGLTextureStateValue
{
    GLuint val1; //!< OpenGL texture opvalue 1
    GLuint val2; //!< OpenGL texture opvalue 2
};

//!
//! opengl texture stage state operation map
//!
static OGLTextureState sTs2OGL[GN::gfx::NUM_TEXTURE_STATES] =
{
    #define GNGFX_DEFINE_TS( tag, defval0, d3dval, glname1, glname2 ) { glname1, glname2 },
    #include "garnet/gfx/textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
};

//!
//! opengl texture stage state value map
//!
static OGLTextureStateValue sTsv2OGL[GN::gfx::NUM_TEXTURE_STATE_VALUES] =
{
    #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) { glval1, glval2 },
    #include "garnet/gfx/textureStateValueMeta.h"
    #undef GNGFX_DEFINE_TSV
};

//
//
// ------------------------------------------------------------------------
static GN_INLINE void
sCopyFrameBufferTo( const GN::gfx::RendererContext::SurfaceDesc & surf )
{
    using namespace GN;
    using namespace GN::gfx;
    
    GN_ASSERT( surf.texture );

    const OGLTexture * oldtex = safeCast<const OGLTexture*>(surf.texture);

    // get (old) texture size
    uint32_t sx, sy;
    surf.texture->getMipSize<uint32_t>( surf.level, &sx, &sy );

    // copy framebuffer to current (old) render target texture
    GLint currentTexID;
    switch( oldtex->getDesc().type )
    {
        case TEXTYPE_CUBE :
            GN_ASSERT( sx == sy );
            GN_OGL_CHECK(
                glGetIntegerv( GL_TEXTURE_BINDING_CUBE_MAP_ARB, &currentTexID ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_CUBE_MAP_ARB,
                    oldtex->getOGLTexture() ) );
            GN_OGL_CHECK(
                glCopyTexImage2D(
                    OGLTexture::sCubeface2OGL( surf.face ), 0,
                    oldtex->getOGLInternalFormat(), 0, 0, sx, sx, 0 ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, currentTexID ) );
            break;

        case TEXTYPE_2D :
            GN_OGL_CHECK(
                glGetIntegerv( GL_TEXTURE_BINDING_2D, &currentTexID ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_2D,
                    oldtex->getOGLTexture() ) );
            GN_OGL_CHECK(
                glCopyTexImage2D( GL_TEXTURE_2D, 0,
                    oldtex->getOGLInternalFormat(), 0, 0, sx, sy, 0 ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_2D, currentTexID ) );
            break;

        case TEXTYPE_1D :
            GN_ASSERT( 1 == sy );
            GN_OGL_CHECK(
                glGetIntegerv( GL_TEXTURE_BINDING_1D, &currentTexID ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_1D,
                    oldtex->getOGLTexture() ) );
            GN_OGL_CHECK(
                glCopyTexImage1D( GL_TEXTURE_1D, 0,
                    oldtex->getOGLInternalFormat(), 0, 0, sx, 0 ) );
            GN_OGL_CHECK(
                glBindTexture( GL_TEXTURE_1D, currentTexID ) );
            break;

        default:
            GN_ERROR( "invalid texture type!" );
            return;
    }
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::contextClear()
{
    _GNGFX_DEVICE_TRACE();
    mContext.resetToDefault();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::contextDeviceCreate()
{
    _GNGFX_DEVICE_TRACE();
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::contextDeviceRestore()
{
    _GNGFX_DEVICE_TRACE();

    // rebind context
    bindContext( mContext, mContext.flags, true );

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::contextDeviceDispose()
{
    _GNGFX_DEVICE_TRACE();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::contextDeviceDestroy()
{
    _GNGFX_DEVICE_TRACE();
    clearContextResources(); 
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::setContext( const RendererContext & newContext )
{
    GN_GUARD_SLOW;

#if GN_DEBUG
    // make sure bindContext() does not rely on flags in tmp structure.
    RendererContext tmp = newContext;
    RendererContext::FieldFlags flags = tmp.flags;
    tmp.flags.u32 = 0;
    bindContext( tmp, flags, false );
#else
    bindContext( newContext, newContext.flags, false );
#endif

    mContext.mergeWith( newContext );
    holdContextResources( newContext );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::rebindContext( RendererContext::FieldFlags flags )
{
    GN_GUARD_SLOW;
    bindContext( mContext, flags, true );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::RenderStateBlockDesc &
GN::gfx::OGLRenderer::getCurrentRenderStateBlock() const
{
    GN_ASSERT( mContext.flags.rsb );
    return mContext.rsb;
}

// *****************************************************************************
// public functions
// *****************************************************************************

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::bindContext(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    //
    // Parameter check
    //
    if( isParameterCheckEnabled() )
    {
        // TODO: verify data in new context
        // TODO: make sure all fields in current context are valid.
    }

    if( newFlags.state )
    {
        bindContextShaders( newContext, newFlags, forceRebind );
        bindContextRenderStates( newContext, newFlags, forceRebind );
        bindContextRenderTargetsAndViewport( newContext, newFlags, forceRebind );
    }
#if !GN_XENON
    if( newFlags.ffp ) bindContextFfp( newContext, newFlags, forceRebind );
#endif
    if( newFlags.data ) bindContextData( newContext, newFlags, forceRebind );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::bindContextShaders(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    if( 0 == newFlags.shaders ) return;

	const Shader * glslVs = 0;
    const Shader * glslPs = 0;

    const Shader * oldVtxShader = mContext.shaders[VERTEX_SHADER];
    const Shader * oldPxlShader = mContext.shaders[PIXEL_SHADER];
    const Shader * newVtxShader = newContext.shaders[VERTEX_SHADER];
    const Shader * newPxlShader = newContext.shaders[PIXEL_SHADER];

    if( newFlags.shaderBit( VERTEX_SHADER ) )
    {
        GN_ASSERT(
            0 == newVtxShader ||
            VERTEX_SHADER == newVtxShader->getType() );

        if( oldVtxShader != newVtxShader || forceRebind )
        {
            if( oldVtxShader )
            {
                const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(oldVtxShader);
                sh->disable();
            }
            if( newVtxShader && LANG_OGL_GLSL != newVtxShader->getLang() )
            {
                const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(newVtxShader);
                sh->enable();
                sh->apply();
            }
            else
            {
                GN_ASSERT( 0 == newVtxShader || LANG_OGL_GLSL == newVtxShader->getLang() );
                glslVs = newVtxShader;
            }
        }
        else if( newVtxShader && LANG_OGL_GLSL != newVtxShader->getLang() )
        {
            const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(newVtxShader);
            sh->applyDirtyUniforms();
        }
        else
        {
            GN_ASSERT( 0 == newVtxShader || LANG_OGL_GLSL == newVtxShader->getLang() );
            glslVs = newVtxShader;
        }
    }
    else if( oldVtxShader && LANG_OGL_GLSL != oldVtxShader->getLang() )
    {
        const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(oldVtxShader);
        sh->applyDirtyUniforms();
    }
    else
    {
        GN_ASSERT( 0 == oldVtxShader || LANG_OGL_GLSL == oldVtxShader->getLang() );
        glslVs = oldVtxShader;
    }

    if( newFlags.shaderBit( PIXEL_SHADER ) )
    {
        GN_ASSERT(
            0 == newPxlShader ||
            PIXEL_SHADER == newPxlShader->getType() );

        if( oldPxlShader != newPxlShader || forceRebind )
        {
            if( oldPxlShader )
            {
                const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(oldPxlShader);
                sh->disable();
            }
            if( newPxlShader && LANG_OGL_GLSL != newPxlShader->getLang() )
            {
                const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(newPxlShader);
                sh->enable();
                sh->apply();
            }
            else
            {
                GN_ASSERT( 0 == newPxlShader || LANG_OGL_GLSL == newPxlShader->getLang() );
                glslPs = newPxlShader;
            }
        }
        else if( newPxlShader && LANG_OGL_GLSL != newPxlShader->getLang() )
        {
            const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(newPxlShader);
            sh->applyDirtyUniforms();
        }
        else
        {
            GN_ASSERT( 0 == newPxlShader || LANG_OGL_GLSL == newPxlShader->getLang() );
            glslPs = newPxlShader;
        }
    }
    else if( oldPxlShader && LANG_OGL_GLSL != oldPxlShader->getLang() )
    {
        const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(oldPxlShader);
        sh->applyDirtyUniforms();
    }
    else
    {
        GN_ASSERT( 0 == oldPxlShader || LANG_OGL_GLSL == oldPxlShader->getLang() );
        glslPs = oldPxlShader;
    }

    // handle GLSL shader and program in special way
    if( glslVs || glslPs )
    {
        GLSLShaders key = { glslVs, glslPs };
        GLSLProgramMap::const_iterator i = mGLSLProgramMap.find( key );
        if( mGLSLProgramMap.end() != i )
        {
            // found!
            GN_ASSERT( i->second );
            ((const OGLProgramGLSL*)i->second)->apply();
        }
        else
        {
            // not found. we have to create a new GLSL program object
            AutoObjPtr<OGLProgramGLSL> newProg( new OGLProgramGLSL );
            if( !newProg->init(
            	safeCast<const OGLBasicShaderGLSL*>(glslVs),
            	safeCast<const OGLBasicShaderGLSL*>(glslPs) ) ) return ;
            mGLSLProgramMap[key] = newProg;
            newProg.detach()->apply();
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::bindContextRenderStates(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    if( 0 == newFlags.rsb ) return;

    GN_ASSERT( newContext.rsb.valid() );

    const RenderStateBlockDesc & newRsb = newContext.rsb;
    const RenderStateBlockDesc & oldRsb = mContext.rsb;

    bool updateAlphaFunc = false;
    int alphaFunc = oldRsb.get( RS_ALPHA_FUNC );
    int alphaRef  = oldRsb.get( RS_ALPHA_REF );

    bool updateBlend = false;
    int blendSrc  = oldRsb.get( RS_BLEND_SRC );
    int blendDst  = oldRsb.get( RS_BLEND_DST );

    // apply all RSs (except blending factors) to API
    #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) \
        if( newRsb.isSet( RS_##tag ) &&                          \
            ( newRsb.get(RS_##tag) != oldRsb.get(RS_##tag) || forceRebind ) ) \
        {                                                        \
            if( RS_ALPHA_FUNC == RS_##tag )                      \
            {                                                    \
                updateAlphaFunc = true;                          \
                alphaFunc = newRsb.get( RS_ALPHA_FUNC );         \
            }                                                    \
            else if( RS_ALPHA_REF == RS_##tag )                  \
            {                                                    \
                updateAlphaFunc = true;                          \
                alphaRef = newRsb.get( RS_ALPHA_REF );           \
            }                                                    \
            else if( RS_BLEND_SRC == RS_##tag )                  \
            {                                                    \
                updateBlend = true;                              \
                blendSrc = newRsb.get( RS_BLEND_SRC );           \
            }                                                    \
            else if( RS_BLEND_DST == RS_##tag )                  \
            {                                                    \
                updateBlend = true;                              \
                blendDst = newRsb.get( RS_BLEND_DST );           \
            }                                                    \
            else                                                 \
            {                                                    \
                sSet_##tag( newRsb.get(RS_##tag) );              \
            }                                                    \
        }
    #include "garnet/gfx/renderStateMeta.h"
    #undef GNGFX_DEFINE_RS

    // apply alpha function
    if( updateAlphaFunc )
    {
        GN_OGL_CHECK( glAlphaFunc( sRsv2OGL[alphaFunc], alphaRef / 255.0f ) );
    }

    // apply blending factors
    if( updateBlend )
    {
        GN_OGL_CHECK( glBlendFunc( sRsv2OGL[blendSrc], sRsv2OGL[blendDst] ) );
    }

    // NOTE : 当启用OpenGL的ColorMaterial属性时，材质信息会随着顶点的颜色
    //        而改变，因而需要用下面的代码来恢复材质信息。如果禁用了
    //        ColorMaterial属性，则可以注释掉这段代码。
    //
    //// restore material parameters
    //const GLfloat * diff = r.fastget_ambient();
    //const GLfloat * ambi = r.fastget_ambient();
    //const GLfloat * spec = r.fastget_specular();
    //const GLfloat * emis = r.fastget_emission();
    //uint32_t            shin = r.fastget_shininess();

    //GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE  , diff ) );
    //GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT  , ambi ) );
    //GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR , spec ) );
    //GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION , emis ) );
    //GN_OGL_CHECK( glMateriali ( GL_FRONT_AND_BACK, GL_SHININESS, shin ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::bindContextRenderTargetsAndViewport(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    // bind color buffers
    bool rebindViewport = false;
    if( newFlags.colorBuffers )
    {
        uint32_t count = min( (uint32_t)newContext.numColorBuffers, getCaps( CAPS_MAX_RENDER_TARGETS ) );
        if( 0 == count ) count = 1;
        for( uint32_t i = 0; i < count; ++i )
        {
            const RendererContext::SurfaceDesc * oldSurface = i < mContext.numColorBuffers ? &mContext.colorBuffers[i] : 0;
            const RendererContext::SurfaceDesc * newSurface = i < newContext.numColorBuffers ? &newContext.colorBuffers[i] : 0;

            if( forceRebind || // if "force" rebinding, then do it.
                oldSurface != newSurface && // if oldSurface and newSurface are both NULL, then do nothing.
                // program reaches here, means at least one surface is _NOT_ null. So if one of them is NULL
                // or content of the two surfaces are different, we have to do the binding.
                ( NULL == oldSurface || NULL == newSurface || *oldSurface != *newSurface ) )
            {
                // Do copy only when oldSurface points to a texture.
                if( oldSurface && oldSurface->texture ) sCopyFrameBufferTo( *oldSurface );

                // update render target size
                if( 0 == i )
                {
                    uint32_t oldw = mColorBufferWidth;
                    uint32_t oldh = mColorBufferHeight;
                    if( newSurface && newSurface->texture )
                    {
                        newSurface->texture->getMipSize<uint32_t>( newSurface->level, &mColorBufferWidth, &mColorBufferHeight );
                    }
                    else
                    {
                        // use default back buffer size
                        mColorBufferWidth = getDispDesc().width;
                        mColorBufferHeight = getDispDesc().height;
                    }

                    rebindViewport = ( oldw != mColorBufferWidth || oldh != mColorBufferHeight );
                }
            }
        }
    }

    // bind depth buffer
    if( newFlags.depthBuffer )
    {
        const RendererContext::SurfaceDesc & oldSurface = mContext.depthBuffer;
        const RendererContext::SurfaceDesc & newSurface = newContext.depthBuffer;

        if( oldSurface.texture && ( oldSurface != newSurface || forceRebind ) )
        {
            sCopyFrameBufferTo( oldSurface );
        }
    }

    // bind viewport
    if( newFlags.viewport )
    {
        if( rebindViewport || newContext.viewport != mContext.viewport || forceRebind )
        {
            GLint x = (GLint)( newContext.viewport.x * mColorBufferWidth );
            GLint y = (GLint)( newContext.viewport.y * mColorBufferHeight );
            GLsizei w = (GLsizei)( newContext.viewport.w * mColorBufferWidth );
            GLsizei h = (GLsizei)( newContext.viewport.h * mColorBufferHeight );
            GN_OGL_CHECK( glViewport( x, y, w, h ) );
        }
    } else if( rebindViewport )
    {
        GLint x = (GLint)( mContext.viewport.x * mColorBufferWidth );
        GLint y = (GLint)( mContext.viewport.y * mColorBufferHeight );
        GLsizei w = (GLsizei)( mContext.viewport.w * mColorBufferWidth );
        GLsizei h = (GLsizei)( mContext.viewport.h * mColorBufferHeight );
        GN_OGL_CHECK( glViewport( x, y, w, h ) );
    }

    GN_OGL_CHECK( ; );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::bindContextFfp(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    GN_ASSERT( newFlags.ffp );

    // When using programmable pipeline, FFP states should not change too often.
    // So here we add a check point to skip FFP states update once and for all.
    if( 0 == newFlags.u32 ) return;

    if( newFlags.world || newFlags.view )
    {
        const Matrix44f & world = newFlags.world ? newContext.world : mContext.world;
        const Matrix44f & view = newFlags.view ? newContext.view : mContext.view;
        glMatrixMode( GL_MODELVIEW );
        glLoadMatrixf( Matrix44f::sTranspose(view*world)[0] );
    }

    if( newFlags.proj )
    {
        if( newContext.proj != mContext.proj || forceRebind )
        {
            glMatrixMode( GL_PROJECTION );
            glLoadMatrixf( Matrix44f::sTranspose(newContext.proj)[0] );
        }
    }

    if( newFlags.light0Pos )
    {
        if( newContext.light0Pos != mContext.light0Pos || forceRebind )
        {
            glLightfv( GL_LIGHT0, GL_DIFFUSE, newContext.light0Pos );
        }
    }

    if( newFlags.light0Diffuse )
    {
        if( newContext.light0Diffuse != mContext.light0Diffuse || forceRebind )
        {
            glLightfv( GL_LIGHT0, GL_DIFFUSE, newContext.light0Diffuse );
        }
    }

    if( newFlags.materialDiffuse )
    {
        if( newContext.materialDiffuse != mContext.materialDiffuse || forceRebind )
        {
            GN_OGL_CHECK( glColor4fv( newContext.materialDiffuse ) );
            GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, newContext.materialDiffuse ) );
        }
    }

    if( newFlags.materialSpecular )
    {
        if( newContext.materialSpecular != mContext.materialSpecular || forceRebind )
        {
            GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, newContext.materialSpecular ) );
        }
    }

    if( newFlags.tsb )
    {
        const TextureStateBlockDesc & newDesc = newContext.tsb;
        const TextureStateBlockDesc & oldDesc = mContext.tsb;

        TextureStateValue tsv;

        uint32_t maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);
        uint32_t numStages = GN::min<uint32_t>( (uint32_t)newDesc.getNumStages(), maxStages );

        // apply all TSSs to API
        for ( uint32_t i = 0; i < numStages; ++i )
        {
            chooseTextureStage( i );

            if( GLEW_ARB_texture_env_combine )
            {
                for ( uint32_t j = 0; j < NUM_TEXTURE_STATES; ++j )
                {
                    if( newDesc.isSet( i, (TextureState)j ) )
                    {
                        tsv = newDesc.get( i, (TextureState) j );

                        if( !oldDesc.isSet( i, (TextureState)j ) ||
                            oldDesc.get( i, (TextureState)j ) != tsv ||
                            forceRebind )
                        {
                            if( TSV_DOT3 == tsv && !GLEW_ARB_texture_env_dot3 )
                            {
                                GN_DO_ONCE( GN_WARN( "do not support GL_ARB_texture_env_dot3!" ) );
                                tsv = TSV_ARG0;
                            }
                            GN_OGL_CHECK( glTexEnvi( GL_TEXTURE_ENV, sTs2OGL[j].op1, sTsv2OGL[tsv].val1 ) );
                            GN_OGL_CHECK( glTexEnvi( GL_TEXTURE_ENV, sTs2OGL[j].op2, sTsv2OGL[tsv].val2 ) );
                        }
                    }
                }
            }
            else if( newDesc.isSet( i, TS_COLOROP ) )
            {
                tsv = newDesc.get( i, TS_COLOROP );

                if( !oldDesc.isSet( i, TS_COLOROP ) ||
                    oldDesc.get( i, TS_COLOROP ) != tsv ||
                    forceRebind )
                {
                    GLint glop = sTs2OGL[TS_COLOROP].op1;
                    switch( glop )
                    {
                        case GL_REPLACE:
                        case GL_MODULATE:
                            GN_OGL_CHECK( glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, glop ) );
                            break;
                        default:
                            GN_DO_ONCE( GN_WARN( "do not support GL_ARB_texture_env_combine" ) );
                            GN_OGL_CHECK( glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE ) );
                            break;
                    }
                }
            }
        }
        // disabled remaining stages
        for( uint32_t i = numStages; i < maxStages; ++i )
        {
            disableTextureStage( i );
        }

    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::bindContextData(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    GN_ASSERT( newFlags.data );

    //
    // bind vertex format
    //
    if( newFlags.vtxFmt )
    {
        if( newContext.vtxFmt )
        {
            if( newContext.vtxFmt != mContext.vtxFmt || forceRebind )
            {
                GN_ASSERT( mVtxFmts[newContext.vtxFmt] );
                mVtxFmts[newContext.vtxFmt]->bind();
            }
        }
    }

    // Note: vertex and index buffers are handled in draw manager

    //
    // bind textures
    //
    if( newFlags.textures )
    {
        size_t maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);

        size_t numtex = min( maxStages, newContext.numTextures );

        size_t i;
        for ( i = 0; i < numtex; ++i )
        {
            // if null handle, then disable this texture stage
            if( newContext.textures[i] )
            {
                chooseTextureStage( i );
                safeCast<const OGLTexture *>(newContext.textures[i])->bind();
            }
            else
            {
                disableTextureStage( i );
            }
        }

        // disable remaining texture stages
        for( ; i < maxStages; ++i )
        {
            disableTextureStage( i );
        }
    }

    GN_UNGUARD_SLOW;
}
