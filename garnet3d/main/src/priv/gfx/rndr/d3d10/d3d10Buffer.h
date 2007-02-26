#ifndef __GN_GFXD3D10_D3D10BUFFER_H__
#define __GN_GFXD3D10_D3D10BUFFER_H__
// *****************************************************************************
/// \file    d3d10/d3d10Buffer.h
/// \brief   D3D10 buffer classes
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "d3d10Resource.h"
#include "../common/basicBuffer.h"

namespace GN { namespace gfx
{
    ///
    /// D3D buffer class
    ///
    class D3D10Buffer : public D3D10Resource, public StdClass
    {
        GN_DECLARE_STDCLASS( D3D10Buffer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10Buffer( D3D10Renderer & r ) : D3D10Resource(r) { clear(); }
        virtual ~D3D10Buffer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( UInt32 bytes, bool dynamic, bool readback );
        void quit();
    private:
        void clear()
        {
            mD3DBuffer = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        ID3D10Buffer * getD3DBuffer() const { GN_ASSERT(mD3DBuffer); return mD3DBuffer; }

        void * d3dlock( size_t offset, size_t bytes, LockFlag flag );
        void d3dunlock();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10Buffer * mD3DBuffer;
        UInt32         mBytes;
        bool           mDynamic;
        bool           mReadback;

        DynaArray<UInt8>  mLockBuffer;
        UInt32            mLockOffset;
        UInt32            mLockBytes;
        LockFlag          mLockFlag;
        bool              mSysCopy;

        // ********************************
        // private function
        // ********************************
    private:

        bool createBuffer();
    };

    ///
    /// D3D vertex buffer class.
    ///
    class D3D10VtxBuf : public BasicVtxBuf, public D3D10Buffer
    {
         GN_DECLARE_STDCLASS( D3D10VtxBuf, D3D10Buffer );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10VtxBuf( D3D10Renderer & r ) : D3D10Buffer(r) { clear(); }
        virtual ~D3D10VtxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const VtxBufDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void * lock( size_t offset, size_t bytes, LockFlag flag );
        virtual void unlock();

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private function
        // ********************************
    private:
    };

    ///
    /// D3D index buffer class.
    ///
    class D3D10IdxBuf : public BasicIdxBuf, public D3D10Buffer
    {
        GN_DECLARE_STDCLASS( D3D10IdxBuf, D3D10Buffer );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10IdxBuf( D3D10Renderer & r ) : D3D10Buffer(r) { clear(); }
        virtual ~D3D10IdxBuf() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const IdxBufDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual UInt16 * lock( size_t startidx, size_t numidx, LockFlag flag );
        virtual void unlock();

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private function
        // ********************************
    private:
    };

    ///
    /// convert garnet buffer lock flags to D3D10 mapping flag
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
//                           End of d3d10Buffer.h
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10BUFFER_H__
