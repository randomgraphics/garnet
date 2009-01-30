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

static const GLenum CONVERT_FILL_MODES[] =
{
    GL_FILL,   // FILL_SOLID
    GL_LINE,   // FILL_WIREFRAME
    GL_POINT,  // FILL_POINT
};

static const GLenum CONVERT_CULL_MODES[] =
{
    GL_FALSE, // CULL_NONE
    GL_FRONT, // CULL_FRONT
    GL_BACK,  // CULL_BACK
};

static const GLenum CONVERT_FRONT_FACE[] =
{
    GL_CCW, // FRONT_CCW
    GL_CW,  // FRONT_CW
};

static const GLenum CONVERT_CMP[] =
{
    GL_NEVER    , // CMP_NEVER
    GL_LESS     , // CMP_LESS
    GL_LEQUAL   , // CMP_LESS_EQUAL
    GL_EQUAL    , // CMP_EQUAL
    GL_GEQUAL   , // CMP_GREATER_EQUAL
    GL_GREATER  , // CMP_GREATER
    GL_NOTEQUAL , // CMP_NOT_EQUAL
    GL_ALWAYS   , // CMP_ALWAYS
};

static const GLenum CONVERT_STENCIL_OP[] =
{
    GL_KEEP,          // STENCIL_KEEP = 0,
    GL_ZERO,          // STENCIL_ZERO,
    GL_REPLACE,       // STENCIL_REPLACE,
    GL_INCR_WRAP_EXT, // STENCIL_INC_SAT,
    GL_DECR_WRAP_EXT, // STENCIL_DEC_SAT,
    GL_INVERT,        // STENCIL_INVERT,
    GL_INCR,          // STENCIL_INC,
    GL_DECR           // STENCIL_DEC,
};

static const GLenum CONVERT_BLEND_ARG[] =
{
    GL_ZERO                , // BLEND_ZERO = 0,
    GL_ONE                 , // BLEND_ONE,
    GL_SRC_COLOR           , // BLEND_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR , // BLEND_INV_SRC_COLOR,
    GL_SRC_ALPHA           , // BLEND_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA , // BLEND_INV_SRC_ALPHA,
    GL_DST_COLOR           , // BLEND_DEST_ALPHA,
    GL_ONE_MINUS_DST_COLOR , // BLEND_INV_DEST_ALPHA,
    GL_DST_ALPHA           , // BLEND_DEST_COLOR,
    GL_ONE_MINUS_DST_ALPHA , // BLEND_INV_DEST_COLOR,
    GL_SRC_ALPHA_SATURATE  , // BLEND_BLEND_FACTOR,
};

static const GLenum CONVERT_BLEND_OP[] =
{
    GL_FUNC_ADD_EXT              , // BLEND_OP_ADD = 0,
    GL_FUNC_SUBTRACT_EXT         , // BLEND_OP_SUB,
    GL_FUNC_REVERSE_SUBTRACT_EXT , // BLEND_OP_REV_SUB,
    GL_MIN_EXT                   , // BLEND_OP_MIN,
    GL_MAX_EXT                   , // BLEND_OP_MAX,
};
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
    mContext.clear();

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
    const OGLUniform * const * uniforms   = (const OGLUniform * const *)newContext.uniforms.cptr();

    if( oldProgram == newProgram )
    {
        if( newProgram )
        {
            if( forceBinding )
            {
                newProgram->enable();
            }

            newProgram->applyUniforms( uniforms, newContext.uniforms.size() );
        }
    }
    else
    {
        // disable old program
        if( oldProgram ) oldProgram->disable();

        // apply new program
        if( newProgram )
        {
            newProgram->enable();
            newProgram->applyUniforms( uniforms, newContext.uniforms.size() );
        }
    }

    GN_OGL_CHECK( (void)0 );
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

    if( forceBinding || newContext.renderStates != mContext.renderStates )
    {
        // misc 0
        if( forceBinding || newContext.miscFlags0 != mContext.miscFlags0 )
        {
            // fill mode
            glPolygonMode( GL_FRONT_AND_BACK, CONVERT_FILL_MODES[newContext.fillMode] );

            // cull mode
            glPolygonMode( GL_FRONT_AND_BACK, CONVERT_FILL_MODES[newContext.fillMode] );

            // front face
            glFrontFace( CONVERT_FRONT_FACE[newContext.frontFace] );

            // TODO: msaa flag
            if( newContext.msaaEnabled != mContext.msaaEnabled )
            {
                GN_UNIMPL();
            }
        }

        // depth
        if( forceBinding || newContext.depthFlags != mContext.depthFlags )
        {
            if( newContext.depthTest ) glEnable( GL_DEPTH_TEST ); else glDisable( GL_DEPTH_TEST );
            glDepthMask( newContext.depthWrite );
            glDepthFunc( CONVERT_CMP[newContext.depthFunc] );
        };

        // stencil
        if( forceBinding || newContext.stencilFlags != mContext.stencilFlags )
        {
            if( newContext.stencilEnabled ) glEnable( GL_STENCIL_TEST ); else glDisable( GL_STENCIL_TEST );

            GLenum failop  = newContext.stencilFailOp;
            GLenum zfailop = newContext.stencilZFailOp;
            GLenum zpassop = newContext.stencilPassOp;
            if( !GLEW_EXT_stencil_wrap )
            {
#define CHECK_WRAP_STENCIL_OP( op ) \
                if( RendererContext::STENCIL_INC_SAT == op ) { op = RendererContext::STENCIL_INC; nonSupportedWrapOp = true; } \
                if( RendererContext::STENCIL_DEC_SAT == op ) { op = RendererContext::STENCIL_DEC; nonSupportedWrapOp = true; }

                bool nonSupportedWrapOp = false;
                CHECK_WRAP_STENCIL_OP( failop );
                CHECK_WRAP_STENCIL_OP( zfailop );
                CHECK_WRAP_STENCIL_OP( zpassop );

                if( nonSupportedWrapOp )
                {
                    GN_ERROR(sLogger)( "EXT_stencil_wrap is not supported, which is required to implement STENCIL_INC_SAT and STENCIL_DEC_SAT opertion." );
                }
            }
            glStencilOp(
                CONVERT_STENCIL_OP[failop],
                CONVERT_STENCIL_OP[zfailop],
                CONVERT_STENCIL_OP[zpassop] );
        }

        // alpha blending
        if( forceBinding || newContext.blendFlags != mContext.blendFlags )
        {
            // blending enable bit
            if( newContext.blendEnabled ) glEnable( GL_BLEND ); else glDisable( GL_BLEND );

            if( GLEW_EXT_blend_minmax &&
                GLEW_EXT_blend_subtract &&
                GLEW_EXT_blend_func_separate &&
                GLEW_EXT_blend_equation_separate )
            {
                glBlendFuncSeparateEXT(
                    CONVERT_BLEND_ARG[newContext.blendSrc],
                    CONVERT_BLEND_ARG[newContext.blendDst],
                    CONVERT_BLEND_ARG[newContext.blendAlphaSrc],
                    CONVERT_BLEND_ARG[newContext.blendAlphaDst] );

                glBlendEquationSeparateEXT(
                    CONVERT_BLEND_OP[newContext.blendOp],
                    CONVERT_BLEND_OP[newContext.blendAlphaOp] );
            }
            else
            {
                glBlendFunc(
                    CONVERT_BLEND_ARG[newContext.blendSrc],
                    CONVERT_BLEND_ARG[newContext.blendDst] );

                if( newContext.blendOp != RendererContext::BLEND_OP_ADD )
                {
                    GN_ERROR(sLogger)(
                        "EXT_blend_minmax and/or EXT_blend_subtract are missing, which are "
                        "required to implement extended alpha blending operation other than ADD." );
                }

                if( newContext.blendAlphaOp  != newContext.blendOp ||
                    newContext.blendAlphaSrc != newContext.blendSrc ||
                    newContext.blendAlphaDst != newContext.blendDst )
                {
                    GN_ERROR(sLogger)(
                        "EXT_blend_func_separate is missing that is support "
                        "different blend function for alpha channel." );
                }
            }
        }
    }

    // done
    GN_OGL_CHECK( (void)0 );
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
    }

    // bind scissor rect
    if( !forceBinding || newContext.scissorRect != mContext.scissorRect )
    {
        if( 0 == newContext.scissorRect.x ||
            0 == newContext.scissorRect.y ||
            0 == newContext.scissorRect.w ||
            0 == newContext.scissorRect.h )
        {
            glDisable( GL_SCISSOR_TEST );
        }
        else
        {
            glEnable( GL_SCISSOR_TEST );
            GLint x = newContext.scissorRect.x * renderTargetSize.x;
            GLint y = newContext.scissorRect.y * renderTargetSize.y;
            GLint w = newContext.scissorRect.w * renderTargetSize.x;
            GLint h = newContext.scissorRect.h * renderTargetSize.y;
            glScissor( x, y, w, h );
        }
    }*/

    // done
    GN_OGL_CHECK( (void)0 );
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

    //
    // bind textures and samplers
    //
    size_t maxStages = getCaps().maxTextures;
    size_t numtex = min<size_t>( maxStages, RendererContext::MAX_TEXTURES );

    size_t i;
    for ( i = 0; i < numtex; ++i )
    {
        // if null handle, then disable this texture stage
        if( newContext.textures[i] )
        {
            chooseTextureStage( i );

            const OGLTexture * t = safeCastPtr<const OGLTexture>(newContext.textures[i].get());

            t->setSampler( newContext.samplers[i] );

            t->bind();

            if( newContext.gpuProgram )
            {
                // we bind texture to specific texture stage, then bind that stage to shader.
                ((const OGLBasicGpuProgram*)newContext.gpuProgram.get())->applyTexture( newContext.texbinds[i], i );
            }
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

    GN_OGL_CHECK( (void)0 );
    return true;

    GN_UNGUARD_SLOW;
}
