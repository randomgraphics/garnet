#include "pch.h"
#include "oglVtxBuf.h"
#include "oglRenderer.h"

GN::Logger * GN::gfx::OGLVtxBufVBO::sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufVBO::init( size_t bytes, bool dynamic, bool /*sysCopy*/ )
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLVtxBufVBO, () );

    if( 0 == bytes )
    {
        GN_ERROR(sLogger)( "Vertex buffer size can't be zero!" );
        quit(); return selfOK();
    }

    // store properties
    setProperties( bytes, dynamic );

    // initialize system copy
    mSysCopy = (uint8_t*)heapAlloc( bytes );

    // determine buffer usage
    // TODO: try GL_STREAM_DRAW_ARB
    mOGLUsage = dynamic ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB;

    // initialize device data
    if( !createVBO() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::quit()
{
    GN_GUARD;

    if( isLocked() )
    {
        GN_WARN(sLogger)( "call unlock() before u release the vstream!" );
        unlock();
    }

    // release opengl vertex array
    if( mOGLVertexBufferObject )
    {
        GN_ASSERT( glIsBufferARB( mOGLVertexBufferObject ) );
        GN_OGL_CHECK( glDeleteBuffersARB( 1, &mOGLVertexBufferObject ) );
        mOGLVertexBufferObject = 0;
    }

    safeHeapFree( mSysCopy );

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
void * GN::gfx::OGLVtxBufVBO::lock( size_t offset, size_t bytes, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !basicLock( offset, bytes, flag ) ) return false;

    // store locking parameters
    mLockOffset = offset;
    mLockBytes  = bytes;
    mLockFlag   = flag;
    return &mSysCopy[offset];

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !basicUnlock() ) return;

    if( LOCK_RO != mLockFlag )
    {
        OGLAutoAttribStack autoAttribStack( 0, GL_CLIENT_VERTEX_ARRAY_BIT );
        
        GN_ASSERT(
            mLockOffset < getSizeInBytes() &&
            0 < mLockBytes &&
            (mLockOffset + mLockBytes) <= getSizeInBytes() );

        // bind as active buffer
        GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, mOGLVertexBufferObject ) );

        // invalidate previous buffer for "discard" lock
        if( LOCK_DISCARD == mLockFlag )
        {
            GN_OGL_CHECK( glBufferDataARB(
                GL_ARRAY_BUFFER_ARB,
                getSizeInBytes(),
                0,
                mOGLUsage ) );
        }

        // update VBO
        GN_OGL_CHECK( glBufferSubDataARB(
            GL_ARRAY_BUFFER_ARB,
            mLockOffset,
            mLockBytes,
            &mSysCopy[mLockOffset] ) );
    }

    GN_UNGUARD_SLOW;
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
            getSizeInBytes(),
            mSysCopy,
            mOGLUsage ),
        false );

    // call user-defined content loader
    if( !getLoader().empty() )
    {
        if( !getLoader()( *this ) ) return false;
    }

    // success
    ad.dismiss();
    return true;

    GN_UNGUARD;
}
