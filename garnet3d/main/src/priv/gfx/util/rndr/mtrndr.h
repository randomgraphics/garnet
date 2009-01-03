#ifndef __GN_GFX_UTIL_RNDR_MTRNDR_H__
#define __GN_GFX_UTIL_RNDR_MTRNDR_H__
// *****************************************************************************
/// \file
/// \brief   multi thread renderer
/// \author  chenli@@REDMOND (2009.1.2)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace gfx
{
    ///
    /// Multi thread renderer wrapper
    ///
    class MultiThreadRenderer : public Renderer, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadRenderer()          { clear(); }
        virtual ~MultiThreadRenderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const RendererOptions & o );
        void quit();
    private:
        void clear()
        {
            mThread   = NULL;
            mRenderer = NULL;
        }
        //@}

        // ********************************
        // public methods
        // ********************************
    public:

        //@{

        void waitForIdle();

        void postCommand( UInt32 cmd, const void * data, size_t length );

        template<typename T>
        void postCommand1( UInt32 cmd, const T & param1 ) { postCommand( cmd, (void*)&param1, sizeof(param1) ); }

        //@}

        // ********************************
        // thread and sync objects
        // ********************************
    private:

        Thread        * mThread;
        volatile UInt32 mRendererCreationStatus; ///< 0: creation failed, 1: creation succeeded, 2: creation is not finished yet.

        // ********************************
        // front end variables
        // ********************************
    private:

        RendererOptions mRendererOptions;
        DispDesc        mDispDesc;
        void *          mD3DDevice;
        void *          mOGLRC;
        RendererCaps    mCaps;

        // ********************************
        // back-end variables
        // ********************************
    private:

        // variables used in backend thread
        Renderer * mRenderer;

        // ********************************
        // private functions
        // ********************************
    private:

        UInt32 threadProc( void * );

        // ********************************
        // rendering methods from Renderer
        // ********************************
    public:

        //@{

        virtual const RendererOptions & getOptions() const { return mRendererOptions; }
        virtual const DispDesc & getDispDesc() const { return mDispDesc; }
        virtual void * getD3DDevice() const { return mD3DDevice; }
        virtual void * getOGLRC() const { return mOGLRC; }

        virtual const RendererCaps & getCaps() const { return mCaps; }
        virtual bool checkTextureFormatSupport( ColorFormat format, TextureUsages usages ) const;
        virtual ColorFormat getDefaultTextureFormat( TextureUsages usages ) const;

        virtual CompiledGpuProgram * compileGpuProgram( const GpuProgramDesc & desc );
        virtual GpuProgram * createGpuProgram( const void * compiledGpuProgramBinary, size_t length );
        virtual Texture * createTexture( const TextureDesc & desc );
        virtual VtxBuf * createVtxBuf( const VtxBufDesc & );
        virtual IdxBuf * createIdxBuf( const IdxBufDesc & desc );

        virtual bool bindContext( const RendererContext & );
        virtual void rebindContext();
        virtual const RendererContext & getContext() const;

        virtual void present();
        virtual void clearScreen( const Vector4f & c,
                                  float            z,
                                  UInt8            s,
                                  BitFields        flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numprim,
                                  size_t        startvtx,
                                  size_t        minvtxidx,
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
#endif // __GN_GFX_UTIL_RNDR_MTRNDR_H__

