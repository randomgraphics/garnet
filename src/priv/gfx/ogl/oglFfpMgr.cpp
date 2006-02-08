#include "pch.h"
#include "oglRenderer.h"

// *****************************************************************************
// local codes
// *****************************************************************************

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
// from Renderer
// *****************************************************************************

//
// matrix computation
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::gfx::OGLRenderer::composePerspectiveMatrix(
    Matrix44f & result,
    float fovy,
    float ratio,
    float znear,
    float zfar ) const
{
    return result.perspectiveOGL( fovy, ratio, znear, zfar );
}
//
GN::Matrix44f &
GN::gfx::OGLRenderer::composeOrthoMatrix(
    Matrix44f & result,
    float left,
    float bottom,
    float width,
    float height,
    float znear,
    float zfar ) const
{
    return result.orthoOGL(
        left,
        left+width,
        bottom,
        bottom+height,
        znear,
        zfar );
}

// *****************************************************************************
// private function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::applyFfpState()
{
    GN_GUARD_SLOW;

    // make sure at least one state is dirty
    GN_ASSERT( mFfpDirtyFlags.u32 );

    if( mFfpDirtyFlags.TransformWorld || mFfpDirtyFlags.TransformView )
    {
        const Matrix44f & world = mTransformWorld.top();
        const Matrix44f & view = mTransformView.top();
        glMatrixMode( GL_MODELVIEW );
        glLoadMatrixf( Matrix44f::sTranspose(view*world)[0] );
    }

    if( mFfpDirtyFlags.TransformProj )
    {
        glMatrixMode( GL_PROJECTION );
        glLoadMatrixf( Matrix44f::sTranspose(mTransformProj.top())[0] );
    }

    if( mFfpDirtyFlags.Viewport )
    {
        GLint x = (GLint)( mViewport.top().x * mCurrentRTSize.x );
        GLint y = (GLint)( mViewport.top().y * mCurrentRTSize.y );
        GLsizei w = (GLsizei)( mViewport.top().w * mCurrentRTSize.x );
        GLsizei h = (GLsizei)( mViewport.top().h * mCurrentRTSize.y );
        glViewport( x, y, w, h );
        GNGFX_INFO( "Update viewport to: x(%d), y(%d), w(%d), h(%d)", x, y, w, h );
    }

    if( mFfpDirtyFlags.Light0Pos )
    {
        glLightfv( GL_LIGHT0, GL_POSITION, mLight0Pos.top() );
    }

    if( mFfpDirtyFlags.Light0Diffuse )
    {
        glLightfv( GL_LIGHT0, GL_DIFFUSE, mLight0Diffuse.top() );
    }

    if( mFfpDirtyFlags.MaterialDiffuse )
    {
        GN_OGL_CHECK( glColor4fv( mMaterialDiffuse.top() ) );
        GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mMaterialDiffuse.top() ) );
    }

    if( mFfpDirtyFlags.MaterialSpecular )
    {
        GN_OGL_CHECK( glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mMaterialSpecular.top() ) );
    }

    if( mFfpDirtyFlags.TextureStates )
    {
        TextureStateBlockDesc desc = mTextureStates.top();

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

    // check opengl errors
    GN_OGL_CHECK(;);

    // clear dirty set
    mFfpDirtyFlags.u32 = 0;

    GN_UNGUARD_SLOW;
}
