#ifndef __GN_GFX_UTIL_GPU_MTGPU_H__
#define __GN_GFX_UTIL_GPU_MTGPU_H__
// *****************************************************************************
/// \file
/// \brief   multi thread GPU
/// \author  chenli@@REDMOND (2009.1.2)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "cmdbuf.h"

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
            mThread = NULL;
            mGpu = NULL;
        }
        //@}

        // ********************************
        // public methods, can only be called in front end thread
        // ********************************
    public:

        //@{

        CommandBuffer & cmdbuf() { return mCommandBuffer; }
        void waitForIdle();

        //@}

        // ********************************
        // thread and sync objects
        // ********************************
    private:

        CommandBuffer   mCommandBuffer;
        volatile uint32 mGpuCreationStatus; ///< 0: creation failed, 1: creation succeeded, 2: creation is not finished yet.
        SyncEvent       mWaitForIdleFence;
        SyncEvent       mPresentFence;
        Thread        * mThread;

        // ********************************
        // front end variables
        // ********************************
    private:

        MultiThreadGpuOptions mMultithreadOptions;
        GpuOptions            mGpuOptions;
        DispDesc              mDispDesc;
        void *                mD3DDevice;
        void *                mOGLRC;
        GpuCaps               mCaps;
        GpuSignals          * mSignals;
        GpuContext            mGpuContext;

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

        uint32 threadProc( void * );

        // ********************************
        // rendering methods from Gpu
        // ********************************
    public:

        //@{

        virtual const GpuOptions & getOptions() const { return mGpuOptions; }
        virtual const DispDesc & getDispDesc() const { return mDispDesc; }
        virtual void * getD3DDevice() const { return mD3DDevice; }
        virtual void * getOGLRC() const { return mOGLRC; }

        virtual const GpuCaps & caps() const { return mCaps; }
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
                                  uint8            s,
                                  uint32        flags );
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
                             const uint16 * indexData );
        virtual void drawUp( PrimitiveType prim,
                             size_t        numprim,
                             const void *  vertexData,
                             size_t        strideInBytes );
        virtual void drawLines( uint32         options,
                                const void *      positions,
                                size_t            stride,
                                size_t            count,
                                uint32            rgba,
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

