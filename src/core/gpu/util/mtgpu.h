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
        volatile uint32_t    mGpuCreationStatus; ///< 0: creation failed, 1: creation succeeded, 2: creation is not finished yet.
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

        virtual Blob * compileGpuProgram( const GpuProgramDesc & desc );
        virtual GpuProgram * createGpuProgram( const void * compiledGpuProgramBinary, uint32_t length );
        virtual Uniform * createUniform( uint32_t size );
        virtual Texture * createTexture( const TextureDesc & desc );
        virtual VtxBuf * createVtxBuf( const VtxBufDesc & );
        virtual IdxBuf * createIdxBuf( const IdxBufDesc & desc );

        virtual void bindContext( const GpuContext & );
        virtual void rebindContext();
        virtual const GpuContext & getContext() const;

        virtual void present();
        virtual void clearScreen( const Vector4f & c,
                                  float            z,
                                  uint8_t            s,
                                  uint32_t        flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  uint32_t        numprim,
                                  uint32_t        basevtx,
                                  uint32_t        startvtx,
                                  uint32_t        numvtx,
                                  uint32_t        startidx );
        virtual void draw( PrimitiveType prim,
                           uint32_t        numprim,
                           uint32_t        startvtx );
        virtual void drawIndexedUp(
                             PrimitiveType  prim,
                             uint32_t         numprim,
                             uint32_t         numvtx,
                             const void *   vertexData,
                             uint32_t         strideInBytes,
                             const uint16_t * indexData );
        virtual void drawUp( PrimitiveType prim,
                             uint32_t        numprim,
                             const void *  vertexData,
                             uint32_t        strideInBytes );
        virtual void drawLines( uint32_t         options,
                                const void *      positions,
                                uint32_t            stride,
                                uint32_t            count,
                                uint32_t            rgba,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj );

        virtual GpuSignals & getSignals() { GN_ASSERT(mSignals); return *mSignals; }
        virtual void getBackBufferContent( BackBufferContent & );
        virtual void processRenderWindowMessages( bool blockWhileMinimized );
        virtual void setUserData( const Guid & id, const void * data, uint32_t length );
        virtual const void * getUserData( const Guid & id, uint32_t * length ) const;
        virtual bool hasUserData( const Guid & id ) const;
        virtual void debugEnableParameterCheck( bool enable );
        virtual void debugDumpNextFrame( uint32_t startBatchIndex, uint32_t numBatches );
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

namespace GN {
namespace gfx {
///
/// Function type to create renderer.
///
typedef GN::gfx::Gpu * (*CreateSingleThreadFunc)(const GN::gfx::GpuOptions & options, void * context);

///
/// Create multi-threads GPU
// -------------------------------------------------------------------------
inline Gpu * createMultiThreadGpu(const GpuOptions & go, CreateSingleThreadFunc creator, void * context) { return creator(go, context); }

} // namespace gfx
} // namespace GN

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTGPU_H__
