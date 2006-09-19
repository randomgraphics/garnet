#ifndef __GN_GFXOGL_OGLIDXBUF_H__
#define __GN_GFXOGL_OGLIDXBUF_H__
// *****************************************************************************
//! \file    ogl/oglIdxBuf.h
//! \brief   OGL index buffer class
//! \author  chenlee (2005.11.21)
// *****************************************************************************

#include "../common/basicBuffer.h"

namespace GN { namespace gfx
{
    //!
    //! OGL index buffer class
    //!
    class OGLIdxBuf : public BasicIdxBuf, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLIdxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLIdxBuf()          { clear(); }
        virtual ~OGLIdxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t numIdx, bool dynamic );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mBuffer = 0;
        }
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual uint16_t * lock( size_t startIdx, size_t numIdx, LockFlag flag );
        virtual void unlock() {}

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Get index data pointer
        //!
        const uint16_t * getIdxData( size_t startIdx ) const { return mBuffer + startIdx; }

        // ********************************
        // private variables
        // ********************************
    private:

        uint16_t * mBuffer;

        static GN::Logger * sLogger;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of oglIdxBuf.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLIDXBUF_H__
