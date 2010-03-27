#ifndef __GN_GFX_UTIL_GPU_MTGPU_H__
#define __GN_GFX_UTIL_GPU_MTGPU_H__
// *****************************************************************************
/// \file
/// \brief   multi thread GPU
/// \author  chenli@@REDMOND (2009.1.2)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "ringbuffer.h"

namespace GN { namespace gfx
{
    ///
    /// options specific to multi-thread GPU
    struct MultiThreadGpuOptions
    {
        ///
        /// internal command buffer size. Default is 4MB.
        ///
        size_t commandBufferSize;

        ///
        /// Set it to true to limit number of commands cached in command buffer to at most one frame. Default is true.
        ///
        bool cacheOneFrameAtMost;

        /// ctor
        MultiThreadGpuOptions()
            : commandBufferSize( 4 * 1024 * 1024 )
            , cacheOneFrameAtMost( true )
        {
        }
    };

    ///
    /// Multi thread GPU wrapper
    ///
    class MultiThreadGpu : public Gpu, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadGpu, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadGpu()          { Clear(); }
        virtual ~MultiThreadGpu() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const GpuOptions & o, const MultiThreadGpuOptions & mo );
        void Quit();
    private:
        void Clear()
        {
            mThread   = NULL;
            mGpu = NULL;
        }
        //@}

        // ********************************
        // public methods, can only be called in front end thread
        // ********************************
    public:

        //@{

        void waitForIdle() { return waitForFence( mFrontEndFence ); }
        void waitForFence( UInt32 fence );
        UInt32 getCurrentFence() const { return mFrontEndFence; }

        size_t getRingBufferSize() const { return mRingBuffer.Size(); }

        UInt8 * beginPostCommand( UInt32 cmd, size_t length ); ///< always return non-NULL valid pointer.
        void    endPostCommand() { mRingBuffer.endProduce(); }

        void postCommand0( UInt32 cmd )
        {
            beginPostCommand( cmd, 0 );
            endPostCommand();
        }

        template<typename T1>
        void postCommand1( UInt32 cmd, const T1 & p1 )
        {
            UInt8 * buf = beginPostCommand( cmd, sizeof(T1) );
            memcpy( buf, &p1, sizeof(T1) );
            endPostCommand();
        }

        template<typename T1, typename T2>
        void postCommand2( UInt32 cmd, const T1 & p1, const T2 & p2 )
        {
            UInt8 * buf = beginPostCommand( cmd, sizeof(T1) + sizeof(T2) );
            memcpy( buf, &p1, sizeof(T1) ); buf += sizeof(T1);
            memcpy( buf, &p2, sizeof(T2) );
            endPostCommand();
        }

        template<typename T1, typename T2, typename T3>
        void postCommand3( UInt32 cmd, const T1 & p1, const T2 & p2, const T3 & p3 )
        {
            UInt8 * buf = beginPostCommand( cmd, sizeof(T1) + sizeof(T2) + sizeof(T3) );
            memcpy( buf, &p1, sizeof(T1) ); buf += sizeof(T1);
            memcpy( buf, &p2, sizeof(T2) ); buf += sizeof(T2);
            memcpy( buf, &p3, sizeof(T3) );
            endPostCommand();
        }

        template<typename T1, typename T2, typename T3, typename T4>
        void postCommand4( UInt32 cmd, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4 )
        {
            UInt8 * buf = beginPostCommand( cmd, sizeof(T1) + sizeof(T2) + sizeof(T3) + sizeof(T4) );
            memcpy( buf, &p1, sizeof(T1) ); buf += sizeof(T1);
            memcpy( buf, &p2, sizeof(T2) ); buf += sizeof(T2);
            memcpy( buf, &p3, sizeof(T3) ); buf += sizeof(T3);
            memcpy( buf, &p4, sizeof(T4) );
            endPostCommand();
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5>
        void postCommand5( UInt32 cmd, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5 )
        {
            UInt8 * buf = beginPostCommand( cmd, sizeof(T1) + sizeof(T2) + sizeof(T3) + sizeof(T4) + sizeof(T5) );
            memcpy( buf, &p1, sizeof(T1) ); buf += sizeof(T1);
            memcpy( buf, &p2, sizeof(T2) ); buf += sizeof(T2);
            memcpy( buf, &p3, sizeof(T3) ); buf += sizeof(T3);
            memcpy( buf, &p4, sizeof(T4) ); buf += sizeof(T4);
            memcpy( buf, &p5, sizeof(T5) );
            endPostCommand();
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
        void postCommand6( UInt32 cmd, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6 )
        {
            UInt8 * buf = beginPostCommand( cmd, sizeof(T1) + sizeof(T2) + sizeof(T3) + sizeof(T4) + sizeof(T5) + sizeof(T6) );
            memcpy( buf, &p1, sizeof(T1) ); buf += sizeof(T1);
            memcpy( buf, &p2, sizeof(T2) ); buf += sizeof(T2);
            memcpy( buf, &p3, sizeof(T3) ); buf += sizeof(T3);
            memcpy( buf, &p4, sizeof(T4) ); buf += sizeof(T4);
            memcpy( buf, &p5, sizeof(T5) ); buf += sizeof(T5);
            memcpy( buf, &p6, sizeof(T6) );
            endPostCommand();
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
        void postCommand7( UInt32 cmd, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6, const T7 & p7 )
        {
            UInt8 * buf = beginPostCommand( cmd, sizeof(T1) + sizeof(T2) + sizeof(T3) + sizeof(T4) + sizeof(T5) + sizeof(T6) + sizeof(T7) );
            memcpy( buf, &p1, sizeof(T1) ); buf += sizeof(T1);
            memcpy( buf, &p2, sizeof(T2) ); buf += sizeof(T2);
            memcpy( buf, &p3, sizeof(T3) ); buf += sizeof(T3);
            memcpy( buf, &p4, sizeof(T4) ); buf += sizeof(T4);
            memcpy( buf, &p5, sizeof(T5) ); buf += sizeof(T5);
            memcpy( buf, &p5, sizeof(T6) ); buf += sizeof(T6);
            memcpy( buf, &p6, sizeof(T7) );
            endPostCommand();
        }

        //@}

        // ********************************
        // thread and sync objects
        // ********************************
    private:

        RingBuffer      mRingBuffer;
        volatile UInt32 mGpuCreationStatus; ///< 0: creation failed, 1: creation succeeded, 2: creation is not finished yet.
        volatile UInt32 mFrontEndFence;
        volatile UInt32 mBackEndFence;
        Thread        * mThread;

        // ********************************
        // front end variables
        // ********************************
    private:

        MultiThreadGpuOptions mMultithreadOptions;
        GpuOptions            mGpuOptions;
        DispDesc                   mDispDesc;
        void *                     mD3DDevice;
        void *                     mOGLRC;
        GpuCaps               mCaps;
        GpuSignals          * mSignals;
        GpuContext            mGpuContext;
        UInt32                     mLastPresentFence;

        // ********************************
        // back-end variables
        // ********************************
    private:

        // variables used in backend thread
        Gpu * mGpu;

        // ********************************
        // private functions
        // ********************************
    private:

        UInt32 threadProc( void * );

        // ********************************
        // rendering methods from Gpu
        // ********************************
    public:

        //@{

        virtual const GpuOptions & GetOptions() const { return mGpuOptions; }
        virtual const DispDesc & GetDispDesc() const { return mDispDesc; }
        virtual void * GetD3DDevice() const { return mD3DDevice; }
        virtual void * GetOGLRC() const { return mOGLRC; }

        virtual const GpuCaps & GetCaps() const { return mCaps; }
        virtual bool CheckTextureFormatSupport( ColorFormat format, TextureUsage usages ) const;
        virtual ColorFormat GetDefaultTextureFormat( TextureUsage usages ) const;

        virtual Blob * CompileGpuProgram( const GpuProgramDesc & desc );
        virtual GpuProgram * CreateGpuProgram( const void * compiledGpuProgramBinary, size_t length );
        virtual Uniform * CreateUniform( size_t size );
        virtual Texture * CreateTexture( const TextureDesc & desc );
        virtual VtxBuf * CreateVtxBuf( const VtxBufDesc & );
        virtual IdxBuf * CreateIdxBuf( const IdxBufDesc & desc );

        virtual void BindContext( const GpuContext & );
        virtual void RebindContext();
        virtual const GpuContext & GetContext() const;

        virtual void Present();
        virtual void ClearScreen( const Vector4f & c,
                                  float            z,
                                  UInt8            s,
                                  BitFields        flags );
        virtual void DrawIndexed( PrimitiveType prim,
                                  size_t        numprim,
                                  size_t        basevtx,
                                  size_t        startvtx,
                                  size_t        numvtx,
                                  size_t        startidx );
        virtual void Draw( PrimitiveType prim,
                           size_t        numprim,
                           size_t        startvtx );
        virtual void DrawIndexedUp(
                             PrimitiveType  prim,
                             size_t         numprim,
                             size_t         numvtx,
                             const void *   vertexData,
                             size_t         strideInBytes,
                             const UInt16 * indexData );
        virtual void DrawUp( PrimitiveType prim,
                             size_t        numprim,
                             const void *  vertexData,
                             size_t        strideInBytes );
        virtual void DrawLines( BitFields         options,
                                const void *      positions,
                                size_t            stride,
                                size_t            count,
                                UInt32            rgba,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj );

        virtual GpuSignals & GetSignals() { GN_ASSERT(mSignals); return *mSignals; }
        virtual void GetBackBufferContent( BackBufferContent & );
        virtual void ProcessRenderWindowMessages( bool blockWhileMinimized );
        virtual void EnableParameterCheck( bool enable );
        virtual void DumpNextFrame( size_t startBatchIndex, size_t numBatches );
        virtual void SetUserData( const Guid & id, const void * data, size_t length );
        virtual const void * GetUserData( const Guid & id, size_t * length ) const;
        virtual bool HasUserData( const Guid & id ) const;

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTGPU_H__

