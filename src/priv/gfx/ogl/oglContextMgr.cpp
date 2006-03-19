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
    #define GNGFX_DEFINE_TS( tag, defval0, defval, d3dval, glname1, glname2 ) { glname1, glname2 },
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

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::contextClear()
{
    _GNGFX_DEVICE_TRACE();
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
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::setContextState( const ContextState & newState )
{
    GN_GUARD_SLOW;

#if GN_DEBUG
    // make sure bindContextState() does not rely on flags in tmp structure.
    ContextState tmp = newState;
    ContextState::FieldFlags flags = tmp.flags;
    tmp.flags.u32 = 0;
    bindContextState( tmp, flags, false );
#else
    bindContextState( newState, newState.flags, false );
#endif

    mContextState.mergeWith( newState );
    holdContextState( newState );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::setContextData( const ContextData & newData )
{
    GN_GUARD_SLOW;

#if GN_DEBUG
    // make sure bindContextData() does not rely on flags in tmp structure.
    ContextData tmp = newData;
    ContextData::FieldFlags flags = tmp.flags;
    tmp.flags.u32 = 0;
    bindContextData( tmp, flags, false );
#else
    bindContextData( newData, newData.flags, false );
#endif

    mContextData.mergeWith( newData );
    holdContextData( newData );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::rebindContextState( ContextState::FieldFlags flags )
{
    GN_GUARD_SLOW;
    bindContextState( mContextState, flags, true );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::rebindContextData( ContextData::FieldFlags flags )
{
    GN_GUARD_SLOW;
    bindContextData( mContextData, flags, true );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::RenderStateBlockDesc &
GN::gfx::OGLRenderer::getCurrentRenderStateBlock() const
{
    GN_GUARD_SLOW;
    GN_ASSERT( mContextState.flags.rsb );
    return mContextState.rsb;
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
GN_INLINE void GN::gfx::OGLRenderer::bindContextState(
    const ContextState & newState,
    ContextState::FieldFlags newFlags,
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

    //
    // bind shader
    //
    if( newFlags.vtxShader || newFlags.pxlShader )
    {
        newFlags.vtxShader = 0;
        newFlags.pxlShader = 0;

        const Shader * glslVs = 0;
        const Shader * glslPs = 0;

        const Shader * oldVtxShader = mContextState.shaders[VERTEX_SHADER];
        const Shader * oldPxlShader = mContextState.shaders[PIXEL_SHADER];
        const Shader * newVtxShader = newState.shaders[VERTEX_SHADER];
        const Shader * newPxlShader = newState.shaders[PIXEL_SHADER];

        if( newState.flags.vtxShader )
        {
            GN_ASSERT(
                0 == newVtxShader ||
                VERTEX_SHADER == newVtxShader->getType() );

            if( oldVtxShader )
            {
                const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(oldVtxShader);
                sh->disable();
            }
            if( newVtxShader )
            {
                if( LANG_OGL_GLSL != newVtxShader->getLang() )
                {
                    const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(newVtxShader);
                    sh->enable();
                    sh->apply();
                }
                else
                {
                    glslVs = newVtxShader;
                }
            }
        }
        else if( oldVtxShader )
        {
            if( LANG_OGL_GLSL != oldVtxShader->getLang() )
            {
                const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(oldVtxShader);
                sh->applyDirtyUniforms();
            }
            else
            {
                glslVs = oldVtxShader;
            }
        }

        if( newState.flags.pxlShader )
        {
            GN_ASSERT(
                0 == newPxlShader ||
                PIXEL_SHADER == newPxlShader->getType() );

            if( oldPxlShader )
            {
                const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(oldPxlShader);
                sh->disable();
            }
            if( newPxlShader )
            {
                if( LANG_OGL_GLSL != newPxlShader->getLang() )
                {
                    const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(newPxlShader);
                    sh->enable();
                    sh->apply();
                }
                else
                {
                    glslPs = newPxlShader;
                }
            }
        }
        else if( oldPxlShader )
        {
            if( LANG_OGL_GLSL != oldPxlShader->getLang() )
            {
                const OGLBasicShader * sh = safeCast<const OGLBasicShader *>(oldPxlShader);
                sh->applyDirtyUniforms();
            }
            else
            {
                glslPs = oldPxlShader;
            }
        }

        // handle GLSL shader and program in special way
        if( !glslVs && !glslPs ) return;
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

    //
    // bind render states
    //
    if( newFlags.rsb )
    {
        newFlags.rsb = 0;

        const RenderStateBlockDesc & from = mContextState.rsb;
        const RenderStateBlockDesc & to = newState.rsb;
        RenderStateBlockDesc diff;
        RenderStateBlockDesc::sDiff( diff, to, from );

        // apply all RSs (except blending factors) to API
        GN::gfx::RenderStateValue rsv;
        #define GNGFX_DEFINE_RS( tag, defvalue )                  \
            rsv = diff.rs[GN::gfx::RS_##tag];                     \
            GN_ASSERT( rsv < GN::gfx::NUM_RENDER_STATE_VALUES );  \
            if( GN::gfx::RSV_EMPTY != rsv )                       \
            {                                                     \
                if( GN::gfx::RS_BLEND_SRC != GN::gfx::RS_##tag && \
                     GN::gfx::RS_BLEND_DST != GN::gfx::RS_##tag ) \
                    sSet_##tag( rsv );                            \
            }
        #include "garnet/gfx/renderStateMeta.h"
        #undef GNGFX_DEFINE_RS

        // apply blending factors
        if( GN::gfx::RSV_EMPTY != diff.rs[GN::gfx::RS_BLEND_SRC] ||
            GN::gfx::RSV_EMPTY != diff.rs[GN::gfx::RS_BLEND_DST] )
        {
            GN::gfx::RenderStateValue src = GN::gfx::RSV_EMPTY == diff.rs[GN::gfx::RS_BLEND_SRC]
                ? to.rs[GN::gfx::RS_BLEND_SRC] : diff.rs[GN::gfx::RS_BLEND_SRC] ;

            GN::gfx::RenderStateValue dst = GN::gfx::RSV_EMPTY == diff.rs[GN::gfx::RS_BLEND_DST]
                ? to.rs[GN::gfx::RS_BLEND_DST] : diff.rs[GN::gfx::RS_BLEND_DST] ;

            GN_OGL_CHECK( glBlendFunc( sRsv2OGL[src], sRsv2OGL[dst] ) );
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
    }

    //
    // TODO: bind render targets
    //
    if( newFlags.renderTargets )
    {
        newFlags.renderTargets = 0;
    }

    //
    // bind viewport
    //
    if( newFlags.viewport )
    {
        newFlags.viewport = 0;
        if( newState.viewport != mContextState.viewport )
        {
            GLint x = (GLint)( newState.viewport.x * getDispDesc().width);
            GLint y = (GLint)( newState.viewport.y * getDispDesc().height );
            GLsizei w = (GLsizei)( newState.viewport.w * getDispDesc().width );
            GLsizei h = (GLsizei)( newState.viewport.h * getDispDesc().height );
            glViewport( x, y, w, h );
        }
    }

    //
    // bind FFP parameters
    //

    // When using programmable pipeline, FFP states should not change too often.
    // So here we add a check point to skip FFP states update once and for all.
    if( 0 == newFlags.u32 ) return;

    if( newFlags.world || newFlags.view )
    {
        const Matrix44f & world = newFlags.world ? newState.world : mContextState.world;
        const Matrix44f & view = newFlags.view ? newState.view : mContextState.view;
        glMatrixMode( GL_MODELVIEW );
        glLoadMatrixf( Matrix44f::sTranspose(view*world)[0] );
    }

    if( newFlags.proj )
    {
        if( newState.proj != mContextState.proj || forceRebind )
        {
            glMatrixMode( GL_PROJECTION );
            glLoadMatrixf( Matrix44f::sTranspose(newState.proj)[0] );
        }
    }

    if( newFlags.light0Pos )
    {
        if( newState.light0Pos != mContextState.light0Pos || forceRebind )
        {
            glLightfv( GL_LIGHT0, GL_DIFFUSE, newState.light0Pos );
        }
    }

    if( newFlags.light0Diffuse )
    {
        if( newState.light0Diffuse != mContextState.light0Diffuse || forceRebind )
        {
            glLightfv( GL_LIGHT0, GL_DIFFUSE, newState.light0Diffuse );
        }
    }

    if( newFlags.materialDiffuse )
    {
        if( newState.materialDiffuse != mContextState.materialDiffuse || forceRebind )
        {
            GN_OGL_CHECK( glColor4fv( newState.materialDiffuse ) );
            GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, newState.materialDiffuse ) );
        }
    }

    if( newFlags.materialSpecular )
    {
        if( newState.materialSpecular != mContextState.materialSpecular || forceRebind )
        {
            GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, newState.materialSpecular ) );
        }
    }

    if( newFlags.textureStates )
    {
        TextureStateBlockDesc desc = newState.textureStates;

        // Note: OpenGL has no way to "disable" texture stage. We simulate it
        //       by replicating previous stage.
        for( uint32_t i = 0; i < MAX_TEXTURE_STAGES; ++i )
        {
            if( TSV_DISABLE == desc.ts[i][TS_COLOROP] )
            {
                desc.ts[i][TS_ALPHAOP] = TSV_ARG0;
                desc.ts[i][TS_COLORARG0] = TSV_CURRENT_COLOR;
                desc.ts[i][TS_ALPHAARG0] = TSV_CURRENT_ALPHA;
            }
        }

        // apply all TSSs to API
        TextureStateValue tsv;
        uint32_t numstages = GN::min<uint32_t>( MAX_TEXTURE_STAGES, getCaps(CAPS_MAX_TEXTURE_STAGES) );
        for ( uint32_t i = 0; i < numstages; ++i )
        {
            chooseTextureStage( i );

            /* disabled unused texture stages (for performance reason)
            if( GN::gfx::TSV_DISABLE == desc.ts( i, GN::gfx::TS_COLOROP ) )
            {
                disableTextureStage( i );
            }*/

            if( GLEW_ARB_texture_env_combine )
            {
                for ( uint32_t j = 0; j < NUM_TEXTURE_STATES; ++j )
                {
                    tsv = desc.ts[i][j];
                    if( TSV_INVALID != tsv )
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
            else
            {
                tsv = desc.ts[i][TS_COLOROP];
                if( TSV_INVALID != tsv )
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
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::bindContextData(
    const ContextData & newData,
    ContextData::FieldFlags newFlags,
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

    //
    // bind vertex format
    //
    if( newFlags.vtxFmt )
    {
        if( newData.vtxFmt != mContextData.vtxFmt || forceRebind )
        {
            GN_ASSERT( mVtxFmts[newData.vtxFmt] );
            mVtxFmts[newData.vtxFmt]->bind();
        }
    }

    // Note: vertex and index buffers are handled in draw manager

    //
    // bind textures
    //
    if( newFlags.textures )
    {
        size_t maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);

        size_t numtex = min( maxStages, newData.numTextures );

        size_t i;
        for ( i = 0; i < numtex; ++i )
        {
            // if null handle, then disable this texture stage
            if( newData.textures[i] )
            {
                chooseTextureStage( i );
                safeCast<const OGLTexture *>(newData.textures[i])->bind();
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
