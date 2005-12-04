#include "pch.h"
#include "oglVtxBuf.h"
#include "oglRenderer.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufVBO::init( size_t bytes, bool dynamic, bool /*sysCopy*/ )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLVtxBufVBO, () );

    if( 0 == bytes )
    {
        GNGFX_ERROR( "Vertex buffer size can't be zero!" );
        quit(); return selfOK();
    }

    // store properties
    setProperties( bytes, dynamic );

    // initialize system copy
    mSysCopy = (uint8_t*)memAlloc( bytes );

    // determine buffer usage
    // TODO: try GL_STREAM_DRAW_ARB
    mOGLUsage = dynamic ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB;

    // initialize device data
    if( !deviceCreate() && !deviceRestore() ) { quit(); return selfOK(); }

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

    if( getRenderer().getOGLRC() )
    {
        getRenderer().makeCurrent();
    }

    deviceDispose();
    deviceDestroy();

    safeMemFree( mSysCopy );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from OGLResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufVBO::deviceCreate()
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

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufVBO::deviceDestroy()
{
    GN_GUARD;

    if( mLocked )
    {
        GNGFX_WARN( "call unlock() before u release the vstream!" );
        unlock();
    }

    // release opengl vertex array
    if( mOGLVertexBufferObject )
    {
        GN_ASSERT( glIsBufferARB( mOGLVertexBufferObject ) );
        GN_OGL_CHECK( glDeleteBuffersARB( 1, &mOGLVertexBufferObject ) );
        mOGLVertexBufferObject = 0;
    }

    GN_UNGUARD;
}

// *****************************************************************************
// from VtxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::OGLVtxBufVBO::lock( size_t offset, size_t bytes, uint32_t flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( mLocked )
    {
        GNGFX_ERROR( "Vertex buffer is already locked!" );
        return 0;
    }
    if( offset >= getSizeInBytes() )
    {
        GNGFX_ERROR( "offset is beyond the end of vertex buffer!" );
        return 0;
    }

    // adjust offset and bytes
    if( 0 == bytes ) bytes = getSizeInBytes();
    if( offset + bytes > getSizeInBytes() ) bytes = getSizeInBytes() - offset;

    // store locking parameters
    mLocked     = true;
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

    if( !mLocked )
    {
        GNGFX_ERROR( "Can't unlock a vertex buffer that is not locked at all!" );
        return;
    }

    mLocked = false;

    if( LOCK_RO != mLockFlag )
    {
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
