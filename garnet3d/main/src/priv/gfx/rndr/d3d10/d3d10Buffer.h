#ifndef __GN_GFXD3D10_D3D10IDXBUF_H__
#define __GN_GFXD3D10_D3D10IDXBUF_H__
// *****************************************************************************
/// \file    d3d10/d3d10IdxBuf.h
/// \brief   D3D index buffer class
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "d3d10Resource.h"
#include "../common/basicBuffer.h"

namespace GN { namespace gfx
{
    ///
    /// D3D index buffer class.
    ///
    class D3D10IdxBuf : public BasicIdxBuf, public D3D10Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D10IdxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10IdxBuf( D3D10Renderer & r ) : D3D10Resource(r) { clear(); }
        virtual ~D3D10IdxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t numIdx, bool dynamic, bool syscopy );
        void quit();
    private:
        void clear()
        {
            mD3DIdxBuf = 0;
        }
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual UInt16 * lock( size_t startIdx, size_t numIdx, LockFlag flag );
        virtual void unlock();

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Return pointer of D3D index buffer
        ///
        ID3D10Buffer * getD3DIb() const
        {
            GN_ASSERT( mD3DIdxBuf );
            return mD3DIdxBuf;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10Buffer * mD3DIdxBuf;

        DynaArray<UInt16> mLockBuffer;
        size_t            mLockStartIdx;
        size_t            mLockNumIdx;
        LockFlag          mLockFlag;
        bool              mSysCopy;

        static Logger * sLogger;

        // ********************************
        // private function
        // ********************************
    private:

        bool createBuffer();
    };

    ///
    /// convert garnet buffer lock flags to D3D9 mapping flag
    // ----------------------------------------------------------------------------
    inline D3D10_MAP lockFlags2D3D10( LockFlag lock )
    {
        static D3D10_MAP d3dFlags[] =
        {
            D3D10_MAP_READ_WRITE,
            D3D10_MAP_READ,
            D3D10_MAP_WRITE,
            D3D10_MAP_WRITE_DISCARD,
            D3D10_MAP_WRITE_NO_OVERWRITE
        };
        GN_CASSERT( sizeof(d3dFlags)/sizeof(d3dFlags[0]) == NUM_LOCK_FLAGS );

        GN_ASSERT( 0 <= lock && lock < NUM_LOCK_FLAGS );

        return d3dFlags[lock];
    }


}}

// *****************************************************************************
//                           End of d3d10IdxBuf.h
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10IDXBUF_H__
