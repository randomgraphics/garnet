#ifndef __GN_GFXOGL_OGLVTXBUF_H__
#define __GN_GFXOGL_OGLVTXBUF_H__
// *****************************************************************************
//! \file    ogl/oglVtxBuf.h
//! \brief   OGL vertex buffer classes
//! \author  chenlee (2005.11.21)
// *****************************************************************************

#include "oglTypes.h"

namespace GN { namespace gfx
{
    //!
    //! Basic OGL vertex buffer class
    //!
    struct OGLBasicVtxBuf : public VtxBuf
    {
        //!
        //! 返回指向顶点数据的指针
        //!
        virtual const uint8_t * getVtxData() const = 0;
    };

    //!
    //! Normal OGL vertex buffer class
    //!
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
        // standard init/quit
        // ********************************

        //@{
    public:
        bool init( size_t bytes, ResourceUsage usage );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mBuffer = 0; mLocked = false; }
        //@}

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void * lock( size_t offset, size_t bytes, uint32_t flag );
        virtual void unlock();

        // ********************************
        // public OGLBasicVtxBuf
        // ********************************
    public:

        const uint8_t * getVtxData() const { return mBuffer; }

        // ********************************
        // private variables
        // ********************************
    private:

        uint8_t * mBuffer;
        bool      mLocked;
    };

    //!
    //! Vertex buffer that use GL_ARB_vertex_buffer_object.
    //!
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
        // standard init/quit
        // ********************************

        //@{
    public:
        bool init( size_t bytes, ResourceUsage usage, bool sysCopy );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mSysCopy = 0;
            mOGLVertexBufferObject = 0;
            mLocked = false;
        }
        //@}

        // ********************************
        // From OGLResource
        // ********************************
    public:

        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void * lock( size_t offset, size_t bytes, uint32_t flag );
        virtual void unlock();

        // ********************************
        // public OGLBasicVtxBuf
        // ********************************
    public:

        const uint8_t * getVtxData() const
        {
            GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, mOGLVertexBufferObject ) );
            return 0;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        uint8_t * mSysCopy;
        GLuint    mOGLVertexBufferObject;
        GLenum    mOGLUsage;
        bool      mLocked;
        size_t    mLockOffset; //!< bytes from buffer start to locked start.
        size_t    mLockBytes;
        uint32_t  mLockFlag;
    };
}}

// *****************************************************************************
//                           End of oglVtxBuf.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLVTXBUF_H__
