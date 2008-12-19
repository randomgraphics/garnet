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

        virtual void update( size_t startidx, size_t numidx, const void * data, UpdateFlag flag );
        virtual void readback( std::vector<UInt8> & data );

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Get index data pointer
        ///
        const void * getIdxData( size_t startidx ) const { return mBuffer + startidx * mBytesPerIndex; }

        // ********************************
        // private variables
        // ********************************
    private:

        UInt8 * mBuffer;
        size_t  mBytesPerIndex;

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
