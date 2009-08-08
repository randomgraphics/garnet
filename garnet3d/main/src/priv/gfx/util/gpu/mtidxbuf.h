#ifndef __GN_GFX_UTIL_RNDR_MTIDXBUF_H__
#define __GN_GFX_UTIL_RNDR_MTIDXBUF_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread idxbuf wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtrndr.h"

namespace GN { namespace gfx
{
    ///
    /// multi thread index buffer wrapper
    ///
    class MultiThreadIdxBuf : public IdxBuf, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadIdxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadIdxBuf( MultiThreadRenderer & r ) : mRenderer(r) { clear(); }
        virtual ~MultiThreadIdxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( IdxBuf * );
        void quit();
    private:
        void clear() { mIdxBuf = NULL; }
        //@}

        // ********************************
        // public methods
        // ********************************
    public:

        IdxBuf * getRealIdxBuf() const { return mIdxBuf; }

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual void update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag );
        virtual void readback( std::vector<UInt8> & data );

        // ********************************
        // private variables
        // ********************************
    private:

        MultiThreadRenderer & mRenderer;
        IdxBuf              * mIdxBuf;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_RNDR_MTIDXBUF_H__
