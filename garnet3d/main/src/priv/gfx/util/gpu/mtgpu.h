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
        MultiThreadGpu()          { clear(); }
        virtual ~MultiThreadGpu() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GpuOptions & o, const MultiThreadGpuOptions & mo );
        void quit();
    private:
        void clear()
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

        size_t getRingBufferSize() const { return mRingBuffer.size(); }

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

        virtual const GpuOptions & getOptions() const { return mGpuOptions; }
        virtual const DispDesc & getDispDesc() const { return mDispDesc; }
        virtual void * getD3DDevice() const { return mD3DDevice; }
        virtual void * getOGLRC() const { return mOGLRC; }

        virtual const GpuCaps & getCaps() const { return mCaps; }
        virtual bool checkTextureFormatSupport( ColorFormat format, TextureUsage usages ) const;
        virtual ColorFormat getDefaultTextureFormat( TextureUsage usages ) const;

        virtual Blob * compileGpuProgram( const GpuProgramDesc & desc );
        virtual GpuProgram * createGpuProgram( const void * compiledGpuProgramBinary, size_t length );
        virtual Uniform * createUniform( size_t size );
        virtual Texture * createTexture( const TextureDesc & desc );
        virtual VtxBuf * createVtxBuf( const VtxBufDesc & );
        virtual IdxBuf * createIdxBuf( const IdxBufDesc & desc );

        virtual void bindContext( const GpuContext & );
        virtual void rebindContext();
        virtual const GpuContext & getContext() const;

        virtual void present();
        virtual void clearScreen( const Vector4f & c,
                                  float            z,
                                  UInt8            s,
                                  BitFields        flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numprim,
                                  size_t        basevtx,
                                  size_t        startvtx,
                                  size_t        numvtx,
                                  size_t        startidx );
        virtual void draw( PrimitiveType prim,
                           size_t        numprim,
                           size_t        startvtx );
        virtual void drawIndexedUp(
                             PrimitiveType  prim,
                             size_t         numprim,
                             size_t         numvtx,
                             const void *   vertexData,
                             size_t         strideInBytes,
                             const UInt16 * indexData );
        virtual void drawUp( PrimitiveType prim,
                             size_t        numprim,
                             const void *  vertexData,
                             size_t        strideInBytes );
        virtual void drawLines( BitFields         options,
                                const void *      positions,
                                size_t            stride,
                                size_t            count,
                                UInt32            rgba,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj );

        virtual GpuSignals & getSignals() { GN_ASSERT(mSignals); return *mSignals; }
        virtual void getBackBufferContent( BackBufferContent & );
        virtual void processRenderWindowMessages( bool blockWhileMinimized );
        virtual void enableParameterCheck( bool enable );
        virtual void dumpNextFrame( size_t startBatchIndex, size_t numBatches );
        virtual void setUserData( const Guid & id, const void * data, size_t length );
        virtual const void * getUserData( const Guid & id, size_t * length ) const;
        virtual bool hasUserData( const Guid & id ) const;

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTGPU_H__

