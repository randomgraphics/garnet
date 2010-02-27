#ifndef __GN_GFX_UTIL_GPU_MTVTXBUF_H__
#define __GN_GFX_UTIL_GPU_MTVTXBUF_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread vtxbuf wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtgpu.h"

namespace GN { namespace gfx
{
    ///
    /// multi thread vertex buffer wrapper
    ///
    class MultiThreadVtxBuf : public VtxBuf, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadVtxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadVtxBuf( MultiThreadGpu & r ) : mGpu(r) { Clear(); }
        virtual ~MultiThreadVtxBuf() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( VtxBuf * );
        void Quit();
    private:
        void Clear() { mVtxBuf = NULL; }
        //@}

        // ********************************
        // public methods
        // ********************************
    public:

        VtxBuf * getRealVtxBuf() const { return mVtxBuf; }

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag );
        virtual void readback( DynaArray<UInt8> & data );

        // ********************************
        // private variables
        // ********************************
    private:

        MultiThreadGpu & mGpu;
        VtxBuf              * mVtxBuf;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTVTXBUF_H__
