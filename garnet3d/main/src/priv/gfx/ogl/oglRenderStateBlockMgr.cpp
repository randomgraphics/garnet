#include "pch.h"
#include "oglRenderer.h"

// ****************************************************************************
// local types/variables/functions
// ****************************************************************************

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
//! render state value map
//!
static GLenum sRsv2OGL[GN::gfx::NUM_RENDER_STATE_VALUES] =
{
    #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) glval,
    #include "garnet/gfx/renderStateValueMeta.h"
    #undef GNGFX_DEFINE_RSV
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

// Individual RS/TSS functions
#include "oglRenderState.inl"

//!
//! apply render state block to OpenGL
// ------------------------------------------------------------------------
static void sApplyRenderStateBlock(
    GN::gfx::OGLRenderer                & r,
    const GN::gfx::RenderStateBlockDesc & /*from*/,
    const GN::gfx::RenderStateBlockDesc & to,
    const GN::gfx::RenderStateBlockDesc & diff )
{
    GN_GUARD;

    // apply all RSs (except blending factors) to API
    GN::gfx::RenderStateValue rsv;
    #define GNGFX_DEFINE_RS( tag, defvalue )                     \
        rsv = diff.rs[GN::gfx::RS_##tag];                        \
        if( GN::gfx::RSV_INVALID != rsv )                        \
        {                                                        \
            GN_ASSERT( rsv < GN::gfx::NUM_RENDER_STATE_VALUES ); \
            if( GN::gfx::RS_BLEND_SRC != GN::gfx::RS_##tag &&    \
                 GN::gfx::RS_BLEND_DST != GN::gfx::RS_##tag )    \
                sSet_##tag( rsv );                               \
        }
    #include "garnet/gfx/renderStateMeta.h"
    #undef GNGFX_DEFINE_RS

    // apply blending factors
    if( GN::gfx::RSV_INVALID != diff.rs[GN::gfx::RS_BLEND_SRC] ||
        GN::gfx::RSV_INVALID != diff.rs[GN::gfx::RS_BLEND_DST] )
    {
        GN_ASSERT(
            to.rs[GN::gfx::RS_BLEND_SRC] < GN::gfx::NUM_RENDER_STATE_VALUES &&
            to.rs[GN::gfx::RS_BLEND_DST] < GN::gfx::NUM_RENDER_STATE_VALUES );
        GN_OGL_CHECK( glBlendFunc(
            sRsv2OGL[to.rs[GN::gfx::RS_BLEND_SRC]],
            sRsv2OGL[to.rs[GN::gfx::RS_BLEND_DST]] ) );
    }

    // apply all TSSs to API
    GN::gfx::TextureStateValue tsv;
    uint32_t numstages = std::min<uint32_t>( GN::gfx::MAX_TEXTURE_STAGES, r.getCaps(GN::gfx::CAPS_MAX_TEXTURE_STAGES) );
    for ( uint32_t i = 0; i < numstages; ++i )
    {
        r.chooseTextureStage( i );

        if( GLEW_ARB_texture_env_combine )
        {
            for ( uint32_t j = 0; j < GN::gfx::NUM_TEXTURE_STATES; ++j )
            {
                tsv = diff.ts[i][j];
                if( GN::gfx::TSV_INVALID != tsv )
                {
                    if( GN::gfx::TSV_DOT3 == tsv && !GLEW_ARB_texture_env_dot3 )
                    {
                        GN_DO_ONCE( GN_ERROR( "do not support GLEW_ARB_texture_env_dot3!" ) );
                        tsv = GN::gfx::TSV_ARG0;
                    }
                    GN_OGL_CHECK( glTexEnvi( GL_TEXTURE_ENV, sTs2OGL[j].op1, sTsv2OGL[tsv].val1 ) );
                    GN_OGL_CHECK( glTexEnvi( GL_TEXTURE_ENV, sTs2OGL[j].op2, sTsv2OGL[tsv].val2 ) );
                }
            }
        }
        else
        {
            tsv = diff.ts[i][GN::gfx::TS_COLOROP];
            if( GN::gfx::TSV_INVALID != tsv )
            {
                GLint glop = sTs2OGL[GN::gfx::TS_COLOROP].op1;
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

    GN_UNGUARD;
}

// *****************************************************************************
// class OGLRenderStateBlock
// *****************************************************************************

//!
//! OpenGL device state block class
//!
struct OGLRenderStateBlock : public GN::gfx::DeviceRenderStateBlock
{
    GLuint                        mDisplayList;
    GN::gfx::RenderStateBlockDesc mFrom, mTo, mDiff;

    //!
    //! Dtor
    //!
    ~OGLRenderStateBlock()
    {
        if( 0 != mDisplayList )
        {
            GN_OGL_CHECK( glDeleteLists( mDisplayList, 1 ) );
        }
    }

    bool init(
        GN::gfx::OGLRenderer                & r,
        const GN::gfx::RenderStateBlockDesc & from,
        const GN::gfx::RenderStateBlockDesc & to )
    {
        GN_GUARD;

        mFrom = from;
        mTo   = to;

        // Note: OpenGL has no way to "disable" texture stage. We simulate it
        //       by replicating previous stage.
        for( uint32_t i = 0; i < GN::gfx::MAX_TEXTURE_STAGES; ++i )
        {
            if( GN::gfx::TSV_DISABLE == mFrom.ts[i][GN::gfx::TS_COLOROP] )
            {
                mFrom.ts[i][GN::gfx::TS_ALPHAOP] = GN::gfx::TSV_ARG0;
                mFrom.ts[i][GN::gfx::TS_COLORARG0] = GN::gfx::TSV_CURRENT_COLOR;
                mFrom.ts[i][GN::gfx::TS_ALPHAARG0] = GN::gfx::TSV_CURRENT_ALPHA;
            }
            if( GN::gfx::TSV_DISABLE == mTo.ts[i][GN::gfx::TS_COLOROP] )
            {
                mTo.ts[i][GN::gfx::TS_ALPHAOP] = GN::gfx::TSV_ARG0;
                mTo.ts[i][GN::gfx::TS_COLORARG0] = GN::gfx::TSV_CURRENT_COLOR;
                mTo.ts[i][GN::gfx::TS_ALPHAARG0] = GN::gfx::TSV_CURRENT_ALPHA;
            }
        }
        mDiff = mTo - mFrom;

        // generate new display list
        mDisplayList = glGenLists( 1 );
        if( 0 == mDisplayList )
        {
            GNGFX_ERROR( "fail to generate displaylist : " );
            return false;
        }

        // compile display list
        glNewList( mDisplayList, GL_COMPILE );
        sApplyRenderStateBlock( r, mFrom, mTo, mDiff );
        glEndList();

        // check opengl error
        if( GL_NO_ERROR != glGetError() )
        {
            GNGFX_ERROR( "fail to compile displaylist! " );
            return false;
        }

        // success
        return true;

        GN_UNGUARD;
    }

    void apply() const
    {
        GN_GUARD_SLOW;

        /*
        // disabled unused texture stages (for performance reason)
        uint32_t numstages = std::min<uint32_t>( GN::gfx::MAX_TEXTURE_STAGES, r.getCaps(GN::gfx::CAPS_MAX_TEXTURE_STAGES) );
        for ( uint32_t i = 0; i < numstages; ++i )
        {
            if( GN::gfx::TSV_DISABLE == mDiff.ts( i, GN::gfx::TS_COLOROP ) )
            {
                r.disableTextureStage( i );
            }
        } //*/

        // apply state block
        GN_ASSERT( glIsList(mDisplayList) );
        GN_OGL_CHECK( glCallList(mDisplayList) );
        //sApplyRenderStateBlock( mDiff );

        GN_UNGUARD_SLOW;
    }
};

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::DeviceRenderStateBlock *
GN::gfx::OGLRenderer::createDeviceRenderStateBlock(
    const RenderStateBlockDesc & from, const RenderStateBlockDesc & to )
{
    GN_GUARD;

    AutoRef<OGLRenderStateBlock> rsb( new OGLRenderStateBlock );

    if( !rsb->init( *this, from, to ) ) return 0;

    // success
    return rsb.detach();

    GN_UNGUARD;
}
