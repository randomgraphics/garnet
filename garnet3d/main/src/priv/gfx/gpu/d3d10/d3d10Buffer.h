#ifndef __GN_GFXD3D10_D3D10BUFFER_H__
#define __GN_GFXD3D10_D3D10BUFFER_H__
// *****************************************************************************
/// \file
/// \brief   D3D10 buffer classes
/// \author  chenlee (2005.11.21)
// *****************************************************************************

#include "d3d10Resource.h"
#include "../common/basicSurface.h"

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
        D3D10Buffer( D3D10Gpu & r ) : D3D10Resource(r) { Clear(); }
        virtual ~D3D10Buffer() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( size_t bytes, bool fastCpuWrite, UInt32 bindFlags );
        void Quit();
    private:
        void Clear()
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
        void           update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag );
        void           readback( std::vector<UInt8> & data );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10Buffer * mD3DBuffer;
        size_t         mBytes;
        bool           mFastCpuWrite;

        // ********************************
        // private function
        // ********************************
    private:
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
        D3D10VtxBuf( D3D10Gpu & r ) : D3D10Buffer(r) { Clear(); }
        virtual ~D3D10VtxBuf() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const VtxBufDesc & desc );
        void Quit();
    private:
        void Clear() {}
        //@}

        // ********************************
        // from VtxBuf
        // ********************************
    public:

        virtual void update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag );
        virtual void readback( std::vector<UInt8> & data );

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
        D3D10IdxBuf( D3D10Gpu & r ) : D3D10Buffer(r) { Clear(); }
        virtual ~D3D10IdxBuf() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const IdxBufDesc & desc );
        void Quit();
    private:
        void Clear() {}
        //@}

        // ********************************
        // from IdxBuf
        // ********************************
    public:

        virtual void update( size_t startidx, size_t numidx, const void * data, SurfaceUpdateFlag flag );
        virtual void readback( std::vector<UInt8> & data );

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
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10BUFFER_H__
