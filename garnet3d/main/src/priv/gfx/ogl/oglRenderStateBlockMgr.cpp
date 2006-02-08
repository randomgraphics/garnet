#include "pch.h"
#include "oglRenderer.h"

// ****************************************************************************
// local types/variables/functions
// ****************************************************************************

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
//! apply render state block to OpenGL
// ------------------------------------------------------------------------
static void sApplyRenderStateBlock(
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
    GN::gfx::OGLRenderer        & mRenderer;
    GLuint                        mDisplayList;
    GN::gfx::RenderStateBlockDesc mFrom, mTo, mDiff;

    //!
    //! Ctor
    //!
    OGLRenderStateBlock( GN::gfx::OGLRenderer & r ) : mRenderer(r) {}

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
        const GN::gfx::RenderStateBlockDesc & from,
        const GN::gfx::RenderStateBlockDesc & to )
    {
        GN_GUARD;

        mFrom = from;
        mTo   = to;
        mDiff = mTo - mFrom;

        // generate new display list
        mDisplayList = glGenLists( 1 );
        if( 0 == mDisplayList )
        {
            GN_ERROR( "fail to generate displaylist : " );
            return false;
        }

        // compile display list
        glNewList( mDisplayList, GL_COMPILE );
        sApplyRenderStateBlock( mFrom, mTo, mDiff );
        glEndList();

        // check opengl error
        if( GL_NO_ERROR != glGetError() )
        {
            GN_ERROR( "fail to compile displaylist! " );
            return false;
        }

        // success
        return true;

        GN_UNGUARD;
    }

    void apply() const
    {
        GN_GUARD_SLOW;

        mRenderer.makeCurrent();

        // apply state block
#if GN_DEBUG
        sApplyRenderStateBlock( mFrom, mTo, mDiff );
#else
        GN_ASSERT( glIsList(mDisplayList) );
        GN_OGL_CHECK( glCallList(mDisplayList) );
#endif
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

    makeCurrent();

    AutoRef<OGLRenderStateBlock> rsb( new OGLRenderStateBlock(*this) );

    if( !rsb->init( from, to ) ) return 0;

    // success
    return rsb.detach();

    GN_UNGUARD;
}
