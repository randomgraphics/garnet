#ifndef __GN_GFX_UTIL_RNDR_MTVTXBUF_H__
#define __GN_GFX_UTIL_RNDR_MTVTXBUF_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread vtxbuf wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtrndr.h"

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
        MultiThreadVtxBuf( MultiThreadRenderer & r ) : mRenderer(r) { clear(); }
        virtual ~MultiThreadVtxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( VtxBuf * );
        void quit();
    private:
        void clear() { mVtxBuf = NULL; }
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

        virtual void update( size_t offset, size_t length, const void * data, UpdateFlag flag );
        virtual void readback( std::vector<UInt8> & data );

        // ********************************
        // private variables
        // ********************************
    private:

        MultiThreadRenderer & mRenderer;
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
#endif // __GN_GFX_UTIL_RNDR_MTVTXBUF_H__
