#ifndef __GN_GFXOGL_OGLIDXBUF_H__
#define __GN_GFXOGL_OGLIDXBUF_H__
// *****************************************************************************
/// \file
/// \brief   OGL index buffer class
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "../common/basicSurface.h"

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
            mBuffer = 0;
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
        /// Get index data pointer
        ///
        const void * getIdxData( uint32 startidx ) const { return mBuffer + startidx * mBytesPerIndex; }

        // ********************************
        // private variables
        // ********************************
    private:

        uint8 * mBuffer;
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
