#include "pch.h"
#include "oglVtxBuf.h"
#include "oglGpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufVBO::init( const VtxBufDesc & desc )
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLVtxBufVBO, () );

    if( 0 == desc.length )
    {
        GN_ERROR(sLogger)( "Vertex buffer size can't be zero!" );
        return failure();
    }

    // store properties
    setDesc( desc );

    // determine buffer usage
    // TODO: try GL_STREAM_DRAW_ARB
    mOGLUsage = desc.fastCpuWrite ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB;

    // initialize device data
    if( !createVBO() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::quit()
{
    GN_GUARD;

    // release opengl vertex array
    if( mOGLVertexBufferObject )
    {
        GN_ASSERT( glIsBufferARB( mOGLVertexBufferObject ) );
        GN_OGL_CHECK( glDeleteBuffersARB( 1, &mOGLVertexBufferObject ) );
        mOGLVertexBufferObject = 0;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from VtxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !validateUpdateParameters( offset, &length, data, flag ) ) return;

    if( 0 == length ) return;

    OGLAutoAttribStack autoAttribStack( 0, GL_CLIENT_VERTEX_ARRAY_BIT );

    // sanity check
    GN_ASSERT(
        offset < getDesc().length &&
        0 < length &&
        (offset + length) <= getDesc().length );

    // bind as active buffer
    GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, mOGLVertexBufferObject ) );

    // update VBO
    GN_OGL_CHECK( glBufferSubDataARB(
        GL_ARRAY_BUFFER_ARB,
        offset,
        length,
        data ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::readback( DynaArray<UInt8> & data )
{
    GN_UNIMPL_WARNING();
    data.clear();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufVBO::createVBO()
{
    GN_GUARD;

    struct AutoDel
    {
        PFNGLDELETEBUFFERSARBPROC func;
        GLuint vbo;

        AutoDel( PFNGLDELETEBUFFERSARBPROC f, GLuint v ) : func(f), vbo(v) {}

        ~AutoDel() { if(vbo) func( 1, &vbo ); }

        void dismiss() { vbo = 0; }
    };

    // create VBO
    GN_OGL_CHECK_RV( glGenBuffersARB( 1, &mOGLVertexBufferObject ), false );
    AutoDel ad( glDeleteBuffersARB, mOGLVertexBufferObject );

    // initialize VBO memory store
    GN_OGL_CHECK_RV(
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, mOGLVertexBufferObject ),
        false );
    GN_OGL_CHECK_RV(
        glBufferDataARB(
            GL_ARRAY_BUFFER_ARB,
            getDesc().length,
            NULL,
            mOGLUsage ),
        false );

    // success
    ad.dismiss();
    return true;

    GN_UNGUARD;
}
