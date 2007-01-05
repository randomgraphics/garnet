#ifndef __GN_GFXD3D9_D3D9IDXBUF_H__
#define __GN_GFXD3D9_D3D9IDXBUF_H__
// *****************************************************************************
//! \file    d3d9/d3d9IdxBuf.h
//! \brief   D3D index buffer class
//! \author  chenlee (2005.11.21)
// *****************************************************************************

#include "d3d9Resource.h"
#include "../common/basicBuffer.h"

namespace GN { namespace gfx
{
    //!
    //! D3D index buffer class.
    //!
    class D3D9IdxBuf : public BasicIdxBuf, public D3D9Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D9IdxBuf, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9IdxBuf( D3D9Renderer & r ) : D3D9Resource(r) { clear(); }
        virtual ~D3D9IdxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader );
        void quit();
    private:
        void clear()
        {
            mSysCopy.clear();
            mD3DIb = 0;
        }
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual UInt16 * lock( size_t startIdx, size_t numIdx, LockFlag flag );
        virtual void unlock();

        // ********************************
        // from D3D9Resource
        // ********************************
    public:

        bool deviceRestore();
        void deviceDispose();

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Return pointer of D3D index buffer
        //!
        LPDIRECT3DINDEXBUFFER9 getD3DIb() const
        {
            GN_ASSERT( mD3DIb );
            return mD3DIb;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        std::vector<UInt16>  mSysCopy;
        LPDIRECT3DINDEXBUFFER9 mD3DIb;

        size_t   mLockStartIdx;
        size_t   mLockNumIdx;
        LockFlag mLockFlag;

        static Logger * sLogger;
    };
}}

// *****************************************************************************
//                           End of d3d9IdxBuf.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9IDXBUF_H__
