#include "pch.h"
#include "oglRenderer.h"
#include "oglRenderTargetMgr.h"
#include "oglShader.h"
#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"
#include "oglTexture.h"

using namespace GN::gfx;

// *****************************************************************************
// local function
// *****************************************************************************

static inline OGLVtxFmt *
sGetOGLVtxFmt(
    OGLRenderer                       & r,
    std::map<VertexFormat,OGLVtxFmt*> & vfmap,
    const VertexFormat                & vf )
{
    std::map<VertexFormat,OGLVtxFmt*>::iterator i = vfmap.find( vf );

    if( i != vfmap.end() ) return i->second;

    std::auto_ptr<OGLVtxFmt> oglvf( new OGLVtxFmt(r) );
    if( !oglvf->init( vf ) ) return NULL;

    vfmap[vf] = oglvf.get();

    return oglvf.release();
}

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

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::contextQuit()
{
    // reset context
    mContext.resetToDefault();

    // delete all vertex formats
    for( std::map<VertexFormat,OGLVtxFmt*>::iterator i = mVertexFormats.begin();
         i != mVertexFormats.end();
         ++i )
    {
        delete i->second;
    }
    mVertexFormats.clear();
}

// *****************************************************************************
// from BasicRenderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool
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

    if( !bindContextShaders( newContext, forceBinding ) ) return false;

    if( !bindContextRenderStates( newContext, forceBinding ) ) return false;

    if( !bindContextRenderTargets( newContext, forceBinding ) ) return false;

    if( !bindContextResources( newContext, forceBinding ) ) return false;

    return true;

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
inline bool
GN::gfx::OGLRenderer::bindContextShaders(
    const RendererContext & newContext,
    bool                    forceBinding )
{
    GN_GUARD_SLOW;

    const OGLBasicGpuProgram * oldProgram = (const OGLBasicGpuProgram*)mContext.gpuProgram.get();
    const OGLBasicGpuProgram * newProgram = (const OGLBasicGpuProgram*)newContext.gpuProgram.get();

    if( oldProgram == newProgram )
    {
        if( newProgram )
        {
            if( forceBinding )
            {
                newProgram->apply();
            }
            else
            {
                newProgram->applyDirtyParameters();
            }
        }
    }
    else
    {
        // disable old program
        if( oldProgram ) oldProgram->disable();

        // apply new program
        if( newProgram ) newProgram->apply();
    }

    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline bool
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

    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline bool
GN::gfx::OGLRenderer::bindContextRenderTargets(
    const RendererContext & newContext,
    bool                    forceBinding )
{
    GN_UNUSED_PARAM( newContext );
    GN_UNUSED_PARAM( forceBinding );

    /* bind render targets
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
    }*/

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool
GN::gfx::OGLRenderer::bindContextResources(
    const RendererContext & newContext,
    bool                    forceBinding )
{
    GN_GUARD_SLOW;

    //
    // bind vertex format
    //
    if( forceBinding || newContext.vtxfmt != mContext.vtxfmt )
    {
        mCurrentOGLVtxFmt = sGetOGLVtxFmt( *this, mVertexFormats, newContext.vtxfmt );
        if( !mCurrentOGLVtxFmt ) return false;
        mCurrentOGLVtxFmt->bindStates();
    }

    //
    // Note: vertex and index buffers are binded by draw manager
    //

    /*
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

    return true;

    GN_UNGUARD_SLOW;
}
