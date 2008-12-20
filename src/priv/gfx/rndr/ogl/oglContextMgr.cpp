#include "pch.h"
#include "oglRenderer.h"
#include "oglRenderTargetMgr.h"
//#include "oglShader.h"
//#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"
#include "oglTexture.h"
//#include "oglSampler.h"

// *****************************************************************************
// local function
// *****************************************************************************

// *****************************************************************************
// init/quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::contextInit()
{
    _GNGFX_DEVICE_TRACE();

    // bind default context to device
    rebindContext();

    return true;
}

// *****************************************************************************
// from BasicRenderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::OGLRenderer::bindContextImpl(
    const RendererContext & newContext,
    bool                    forceBinding )
{
    GN_GUARD_SLOW;

    //
    // Parameter check
    //
    if( paramCheckEnabled() )
    {
        GN_TODO( "verify renderer context data" );
    }

    bindContextShaders( newContext, forceBinding );

    bindContextRenderStates( newContext, forceBinding );

    // bind render targets
    bool renderTargetSizeChanged = false;
    //mRTMgr->bind( mContext.renderTargets, newContext.renderTargets, forceBinding, renderTargetSizeChanged );

    // bind viewport
    if( renderTargetSizeChanged || newContext.viewport != mContext.viewport || forceBinding )
    {
        UInt32 rtw, rth;
        mRTMgr->getRTSize( rtw, rth );
        GLint x = (GLint)( newContext.viewport.x * rtw );
        GLint y = (GLint)( newContext.viewport.y * rth );
        GLsizei w = (GLsizei)( newContext.viewport.w * rtw );
        GLsizei h = (GLsizei)( newContext.viewport.h * rth );
        GN_OGL_CHECK( glViewport( x, y, w, h ) );
    }

    bindContextResources( newContext, forceBinding );

    GN_UNGUARD_SLOW;
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
inline void
GN::gfx::OGLRenderer::bindContextShaders(
    const RendererContext & newContext,
    bool                    forceBinding )
{
    GN_GUARD_SLOW;

    GN_UNUSED_PARAM( newContext );
    GN_UNUSED_PARAM( forceBinding );

    /*

    const Shader * glslVs = 0;
    const Shader * glslPs = 0;

    const Shader * oldVtxShader = mContext.shaders[SHADER_VS];
    const Shader * oldPxlShader = mContext.shaders[SHADER_PS];
    const Shader * newVtxShader = newContext.shaders[SHADER_VS];
    const Shader * newPxlShader = newContext.shaders[SHADER_PS];

    if( newFlags.shaderBit( SHADER_VS ) )
    {
        GN_ASSERT(
            0 == newVtxShader ||
            SHADER_VS == newVtxShader->getType() );

        if( oldVtxShader != newVtxShader || forceBinding )
        {
            if( oldVtxShader )
            {
                const OGLBasicShader * sh = safeCastPtr<const OGLBasicShader>(oldVtxShader);
                sh->disable();
            }
            if( newVtxShader && LANG_OGL_GLSL != newVtxShader->getLang() )
            {
                const OGLBasicShader * sh = safeCastPtr<const OGLBasicShader>(newVtxShader);
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
            const OGLBasicShader * sh = safeCastPtr<const OGLBasicShader>(newVtxShader);
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
        const OGLBasicShader * sh = safeCastPtr<const OGLBasicShader>(oldVtxShader);
        sh->applyDirtyUniforms();
    }
    else
    {
        GN_ASSERT( 0 == oldVtxShader || LANG_OGL_GLSL == oldVtxShader->getLang() );
        glslVs = oldVtxShader;
    }

    if( newFlags.shaderBit( SHADER_PS ) )
    {
        GN_ASSERT(
            0 == newPxlShader ||
            SHADER_PS == newPxlShader->getType() );

        if( oldPxlShader != newPxlShader || forceBinding )
        {
            if( oldPxlShader )
            {
                const OGLBasicShader * sh = safeCastPtr<const OGLBasicShader>(oldPxlShader);
                sh->disable();
            }
            if( newPxlShader && LANG_OGL_GLSL != newPxlShader->getLang() )
            {
                const OGLBasicShader * sh = safeCastPtr<const OGLBasicShader>(newPxlShader);
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
            const OGLBasicShader * sh = safeCastPtr<const OGLBasicShader>(newPxlShader);
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
        const OGLBasicShader * sh = safeCastPtr<const OGLBasicShader>(oldPxlShader);
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
        GLSLShaders key = { { glslVs, glslPs, 0 } };
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
            	safeCastPtr<const OGLBasicShaderGLSL>(glslVs),
            	safeCastPtr<const OGLBasicShaderGLSL>(glslPs) ) ) return ;
            mGLSLProgramMap[key] = newProg;
            newProg.detach()->apply();
        }
    }

    */

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline void
GN::gfx::OGLRenderer::bindContextRenderStates(
    const RendererContext & newContext,
    bool                    forceBinding )
{
    GN_GUARD_SLOW;

    GN_UNUSED_PARAM( newContext );
    GN_UNUSED_PARAM( forceBinding );

    /*

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
            ( newRsb.get(RS_##tag) != oldRsb.get(RS_##tag) || forceBinding ) ) \
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
    //UInt32            shin = r.fastget_shininess();

    //GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE  , diff ) );
    //GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT  , ambi ) );
    //GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR , spec ) );
    //GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION , emis ) );
    //GN_OGL_CHECK( glMateriali ( GL_FRONT_AND_BACK, GL_SHININESS, shin ) );

    */

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline void
GN::gfx::OGLRenderer::bindContextResources(
    const RendererContext & newContext,
    bool                    forceBinding )
{
    GN_GUARD_SLOW;

    GN_UNUSED_PARAM( newContext );
    GN_UNUSED_PARAM( forceBinding );

    /*

    //
    // bind vertex format
    //
    if( newContext.vtxfmt != mContext.vtxfmt || forceBinding )
    {
        GN_ASSERT( mVtxFmts[newContext.vtxfmt] );
        mVtxFmts[newContext.vtxfmt]->bind();
    }

    //
    // check if we need to bind vertex buffers
    //
    mNeedRebindVtxBufs = 0;
    for( UInt32 i = 0; i < newContext.numVtxBufs; ++i )
    {
        const RendererContext::VtxBufDesc & vb = newContext.vtxbufs[i];
        if( vb != mContext.vtxbufs[i] || forceBinding )
        {
            mNeedRebindVtxBufs |= 1 << i;
        }
    }

    //
    // Note: index buffer is binded by draw manager
    //

    //
    // bind textures and samplers
    //
    UInt32 maxStages = getCaps().maxTextures;
    UInt32 numtex = min( maxStages, newContext.numTextures );

    UInt32 i;
    for ( i = 0; i < numtex; ++i )
    {
        // if null handle, then disable this texture stage
        if( newContext.textures[i] )
        {
            chooseTextureStage( i );

            safeCastPtr<const OGLTexture>(newContext.textures[i])->bind();

            SamplerHandle samp = ( i < newContext.numSamplers ) ? newContext.samplers[i] : mDefaultSampler;
            mSamplers[samp]->bind();
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

    */

    GN_UNGUARD_SLOW;
}
