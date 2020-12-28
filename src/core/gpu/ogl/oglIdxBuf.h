#ifndef __GN_GFXOGL_OGLIDXBUF_H__
#define __GN_GFXOGL_OGLIDXBUF_H__
// *****************************************************************************
/// \file
/// \brief   OGL index buffer class
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "../common/basicSurface.h"
#include "garnet/GNogl.h"

namespace GN { namespace gfx
{
    ///
    /// OGL index buffer class
    ///
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
        bool init( const IdxBufDesc & desc );
        void quit();
    private:
        void clear()
        {
            mBuffer.cleanup();
        }
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual void update( uint32 startidx, uint32 numidx, const void * data, SurfaceUpdateFlag flag );
        virtual void readback( DynaArray<uint8> & data );

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// bind the index buffer
        ///
        void bind() const { mBuffer.bind(); }

        ///
        /// Get index data pointer
        ///
        const void * data(uint32 startidx) const { return (const void*)(uintptr_t)(startidx * mBytesPerIndex); }

        // ********************************
        // private variables
        // ********************************
    private:

        ogl::BufferObject<GL_ELEMENT_ARRAY_BUFFER> mBuffer;
        uint32  mBytesPerIndex;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLIDXBUF_H__
