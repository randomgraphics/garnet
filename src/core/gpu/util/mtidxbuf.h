#ifndef __GN_GFX_UTIL_GPU_MTIDXBUF_H__
#define __GN_GFX_UTIL_GPU_MTIDXBUF_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread idxbuf wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtgpu.h"

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
        MultiThreadIdxBuf( MultiThreadGpu & r ) : mGpu(r) { clear(); }
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

        virtual void update( uint32 startidx, uint32 numidx, const void * data, SurfaceUpdateFlag flag );
        virtual void readback( DynaArray<uint8> & data );

        // ********************************
        // private variables
        // ********************************
    private:

        MultiThreadGpu & mGpu;
        IdxBuf         * mIdxBuf;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTIDXBUF_H__
