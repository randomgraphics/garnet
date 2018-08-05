#ifndef __GN_GFX_UTIL_GPU_MTGPU_H__
#define __GN_GFX_UTIL_GPU_MTGPU_H__
// *****************************************************************************
/// \file
/// \brief   multi thread GPU
/// \author  chenli@@REDMOND (2009.1.2)
// *****************************************************************************

#if 0 // disabled for now

#include "../../gpu/gpupch.h"
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
    /// Function type to create renderer.
    ///
    typedef GN::gfx::Gpu * (*CreateSingleThreadFunc)( const GN::gfx::GpuOptions & options, void * context );

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
        bool init( const GpuOptions & o, const MultiThreadGpuOptions & mo, CreateSingleThreadFunc func, void * context );
        void quit();
    private:
        void clear()
        {
            mThread = 0;
            mCreator = NULL;
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

        CommandBuffer      mCommandBuffer;
        volatile uint32    mGpuCreationStatus; ///< 0: creation failed, 1: creation succeeded, 2: creation is not finished yet.
        SyncEvent          mWaitForIdleFence;
        SyncEvent          mPresentFence;
        Thread::Identifier mThread;

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
        CreateSingleThreadFunc mCreator;
        Gpu *                  mGpu;
        void *                 mCreationContext;

        // ********************************
        // private functions
        // ********************************
    private:

        void threadProc( void * );

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
        virtual GpuProgram * createGpuProgram( const void * compiledGpuProgramBinary, uint32 length );
        virtual Uniform * createUniform( uint32 size );
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
                                  uint32        numprim,
                                  uint32        basevtx,
                                  uint32        startvtx,
                                  uint32        numvtx,
                                  uint32        startidx );
        virtual void draw( PrimitiveType prim,
                           uint32        numprim,
                           uint32        startvtx );
        virtual void drawIndexedUp(
                             PrimitiveType  prim,
                             uint32         numprim,
                             uint32         numvtx,
                             const void *   vertexData,
                             uint32         strideInBytes,
                             const uint16 * indexData );
        virtual void drawUp( PrimitiveType prim,
                             uint32        numprim,
                             const void *  vertexData,
                             uint32        strideInBytes );
        virtual void drawLines( uint32         options,
                                const void *      positions,
                                uint32            stride,
                                uint32            count,
                                uint32            rgba,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj );

        virtual GpuSignals & getSignals() { GN_ASSERT(mSignals); return *mSignals; }
        virtual void getBackBufferContent( BackBufferContent & );
        virtual void processRenderWindowMessages( bool blockWhileMinimized );
        virtual void setUserData( const Guid & id, const void * data, uint32 length );
        virtual const void * getUserData( const Guid & id, uint32 * length ) const;
        virtual bool hasUserData( const Guid & id ) const;
        virtual void debugEnableParameterCheck( bool enable );
        virtual void debugDumpNextFrame( uint32 startBatchIndex, uint32 numBatches );
        virtual void debugMarkBegin( const char * markerName );
        virtual void debugMarkEnd();
        virtual void debugMarkSet( const char * markerName );

        //@}
    };

    ///
    /// Create multi-threads GPU
    // -------------------------------------------------------------------------
    inline Gpu * createMultiThreadGpu( const GpuOptions & go, CreateSingleThreadFunc creator, void * context )
    {
        GN_GUARD;
        MultiThreadGpu * r = new MultiThreadGpu;
        MultiThreadGpuOptions mo; // use default multithread options
        if( !r->init( go, mo, creator, context ) ) delete r, r = NULL;
        return r;
        GN_UNGUARD;
    }
}}3

#else

namespace GN { namespace gfx
{
    ///
    /// Function type to create renderer.
    ///
    typedef GN::gfx::Gpu * (*CreateSingleThreadFunc)( const GN::gfx::GpuOptions & options, void * context );

    ///
    /// Create multi-threads GPU
    // -------------------------------------------------------------------------
    inline Gpu * createMultiThreadGpu( const GpuOptions & go, CreateSingleThreadFunc creator, void * context )
    {
        return creator(go, context);
    }
    
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTGPU_H__

