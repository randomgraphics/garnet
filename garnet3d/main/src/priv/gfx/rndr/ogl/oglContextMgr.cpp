#include "pch.h"
#include "oglRenderer.h"
#include "oglRenderTargetMgr.h"
#include "oglShader.h"
#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"
#include "oglTexture.h"

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// local function
// *****************************************************************************

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
    // create render target manager
    mRTMgr = OGLRTMgrFBO::usable() ? (OGLBasicRTMgr*)new OGLRTMgrFBO(*this) : new OGLRTMgrCopyFrame(*this);
    if( !mRTMgr->init() ) return false;

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
    for( std::map<VertexFormatKey,OGLVtxFmt*>::iterator i = mVertexFormats.begin();
         i != mVertexFormats.end();
         ++i )
    {
        delete i->second;
    }
    mVertexFormats.clear();

    // delete render target manager
    safeDelete( mRTMgr );
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
    bool                    skipDirtyCheck )
{
    GN_GUARD_SLOW;

    if( !bindContextShaders( newContext, skipDirtyCheck ) ) return false;

    if( !bindContextRenderStates( newContext, skipDirtyCheck ) ) return false;

    if( !bindContextRenderTargets( newContext, skipDirtyCheck ) ) return false;

    if( !bindContextResources( newContext, skipDirtyCheck ) ) return false;

    return true;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline OGLVtxFmt *
GN::gfx::OGLRenderer::findOrCreateOGLVtxFmt(
    const VertexFormat       & vf,
    const OGLBasicGpuProgram * program )
{
    // get shader ID
    UInt64 shaderID;
    if( program )
    {
        shaderID = program->uniqueID();
        GN_ASSERT( 0 != shaderID );
    }
    else
    {
        shaderID = 0;
    }

    VertexFormatKey key = { vf, shaderID };

    std::map<VertexFormatKey,OGLVtxFmt*>::iterator i = mVertexFormats.find( key );

    if( i != mVertexFormats.end() ) return i->second;

    AutoObjPtr<OGLVtxFmt> oglvf( new OGLVtxFmt(*this) );
    if( !oglvf->init( vf, program ) ) return NULL;

    mVertexFormats[key] = oglvf;

    return oglvf.detach();
}

//
//
// -----------------------------------------------------------------------------
inline bool
GN::gfx::OGLRenderer::bindContextShaders(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    GN_GUARD_SLOW;

    const OGLBasicGpuProgram  * oldProgram = (const OGLBasicGpuProgram*)mContext.gpuProgram.get();
    const OGLBasicGpuProgram  * newProgram = (const OGLBasicGpuProgram*)newContext.gpuProgram.get();

    if( oldProgram == newProgram )
    {
        if( newProgram )
        {
            if( skipDirtyCheck )
            {
                newProgram->enable();
            }

            // Make sure size of AutoRef<T> and T* are same. So we can safely convert AutoRef<T> * to T **
            GN_CASSERT( sizeof(AutoRef<Uniform>) == sizeof(Uniform*) );

            newProgram->applyUniforms( (const Uniform * const *)newContext.uniforms.cptr(), newContext.uniforms.size() );
            newProgram->applyTextures( newContext.textures.cptr(), newContext.textures.size() );
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
            newProgram->applyUniforms( (const Uniform * const *)newContext.uniforms.cptr(), newContext.uniforms.size() );
            newProgram->applyTextures( newContext.textures.cptr(), newContext.textures.size() );
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
    bool                    skipDirtyCheck )
{
    GN_GUARD_SLOW;

    if( skipDirtyCheck || newContext.renderStates != mContext.renderStates )
    {
        // misc 0
        if( skipDirtyCheck || newContext.miscFlags0 != mContext.miscFlags0 )
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
        if( skipDirtyCheck || newContext.depthFlags != mContext.depthFlags )
        {
            if( newContext.depthTest ) glEnable( GL_DEPTH_TEST ); else glDisable( GL_DEPTH_TEST );
            glDepthMask( newContext.depthWrite );
            glDepthFunc( CONVERT_CMP[newContext.depthFunc] );
        };

        // stencil
        if( skipDirtyCheck || newContext.stencilFlags != mContext.stencilFlags )
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
        if( skipDirtyCheck || newContext.blendFlags != mContext.blendFlags )
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
    bool                    skipDirtyCheck )
{
    // bind render targets
    if( !mRTMgr->bind( mContext.rendertargets, newContext.rendertargets, skipDirtyCheck ) )
        return false;

    // get render target size
    UInt32 width, height;
    mRTMgr->getColorRenderTargetSize( width, height );

    // clip viewport against render target size
    Rect<UInt32> newvp = newContext.viewport;
    if( (newvp.x+newvp.w) > width )
    {
        GN_WARN(sLogger)( "Viewport cannot be larger with current render target size." );
        if( newvp.x >= width )
        {
             newvp.x = width;
             newvp.w = 0;
        }
        else
        {
            newvp.w = width - newvp.x;
        }
    }
    if( (newvp.y+newvp.h) > height )
    {
        GN_WARN(sLogger)( "Viewport cannot be larger with current render target size." );
        if( newvp.y > height )
        {
            newvp.y = height;
            newvp.h = 0;
        }
        else
        {
            newvp.h = height - newvp.y;
        }
    }

    // bind viewport
    if( 0 == newvp.x && 0 == newvp.y && 0 == newvp.w && 0 == newvp.h )
    {
        newvp.x = 0;
        newvp.y = 0;
        newvp.w = width;
        newvp.h = height;

        GN_OGL_CHECK( glViewport( (GLint)newvp.x, (GLint)newvp.y, (GLsizei)newvp.w, (GLsizei)newvp.h ) );
    }
    else
    {
        newvp.y = height - (newvp.y + newvp.h);
        GN_OGL_CHECK( glViewport( (GLint)newvp.x, (GLint)newvp.y, (GLsizei)newvp.w, (GLsizei)newvp.h ) );
    }

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
    bool                    skipDirtyCheck )
{
    GN_GUARD_SLOW;

    //
    // bind vertex format
    //
    if( skipDirtyCheck || newContext.vtxfmt != mContext.vtxfmt || newContext.gpuProgram != mContext.gpuProgram )
    {
        mCurrentOGLVtxFmt = findOrCreateOGLVtxFmt( newContext.vtxfmt, (const OGLBasicGpuProgram*)newContext.gpuProgram.get() );
        if( !mCurrentOGLVtxFmt ) return false;
        if( !mCurrentOGLVtxFmt->bindStates() ) return false;
    }

    //
    // Note: vertex and index buffers are binded by draw manager
    //

    GN_OGL_CHECK( (void)0 );
    return true;

    GN_UNGUARD_SLOW;
}
