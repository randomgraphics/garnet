#ifndef __GN_GFXOGL_OGLVTXBUF_H__
#define __GN_GFXOGL_OGLVTXBUF_H__
// *****************************************************************************
/// \file    ogl/oglVtxBuf.h
/// \brief   OGL vertex buffer classes
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "oglResource.h"
#include "../common/basicBuffer.h"

namespace GN { namespace gfx
{
    ///
    /// Basic OGL vertex buffer class
    ///
    struct OGLBasicVtxBuf : public BasicVtxBuf
    {
        ///
        /// 返回指向顶点数据的指针
        ///
        virtual const UInt8 * getVtxData() const = 0;
    };

    ///
    /// Normal OGL vertex buffer class
    ///
    class OGLVtxBufNormal : public OGLBasicVtxBuf, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLVtxBufNormal, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLVtxBufNormal()          { clear(); }
        virtual ~OGLVtxBufNormal() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const VtxBufDesc & desc );
        void quit();
    private:
        void clear() { mBuffer = 0; }
        //@}

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void * lock( size_t offset, size_t bytes, LockFlag flag );
        virtual void unlock();

        // ********************************
        // public OGLBasicVtxBuf
        // ********************************
    public:

        const UInt8 * getVtxData() const
        {
            GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 ) );
            return mBuffer;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        UInt8 * mBuffer;
    };

    ///
    /// Vertex buffer that use GL_ARB_vertex_buffer_object.
    ///
    class OGLVtxBufVBO : public OGLBasicVtxBuf, public OGLResource, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLVtxBufVBO, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLVtxBufVBO( OGLRenderer & r ) : OGLResource(r) { clear(); }
        virtual ~OGLVtxBufVBO() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const VtxBufDesc & desc );
        void quit();
    private:
        void clear()
        {
            mSysCopy = 0;
            mOGLVertexBufferObject = 0;
         }
        //@}

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void * lock( size_t offset, size_t bytes, LockFlag flag );
        virtual void unlock();

        // ********************************
        // public OGLBasicVtxBuf
        // ********************************
    public:

        const UInt8 * getVtxData() const
        {
            GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, mOGLVertexBufferObject ) );
            return 0;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        UInt8 * mSysCopy;
        GLuint    mOGLVertexBufferObject;
        GLenum    mOGLUsage;
        size_t    mLockOffset; ///< bytes from buffer start to locked start.
        size_t    mLockBytes;
        LockFlag  mLockFlag;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createVBO();
    };
}}

// *****************************************************************************
//                           End of oglVtxBuf.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLVTXBUF_H__
