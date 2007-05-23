#ifndef __GN_ENGINE_RENDERENGINE_H__
#define __GN_ENGINE_RENDERENGINE_H__
// *****************************************************************************
//! \file    engine/renderEngine.h
//! \brief   interface of render engine
//! \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// Mini-application ID
    ///
    typedef UIntPtr MiniAppId;

    ///
    /// Mini-application that runs in draw thread. For fast prototype only.
    ///
    /// \note Can not use GraphicsResource and Entity explicitly or implicitly in this class.
    ///
    struct MiniApp
    {
        //@{
        virtual ~MiniApp() {}
        virtual bool onInit() = 0;
        virtual bool onRendererCreate() = 0;
        virtual bool onRendererRestore() = 0;
        virtual void onRendererDispose() = 0;
        virtual void onRendererDestroy() = 0;
        virtual void onQuit() = 0;
        virtual void onFrame() = 0;
        //@}

        bool noerr; ///< this value is managed by render engine. Do not modify it in your own code.
    };

    ///
    /// render engine initialization parameters
    ///
    struct RenderEngineInitParameters
    {
        //@{
        UInt32 maxTexBytes;               ///< zero for default value: 3/4 of total video memory
        UInt32 maxMeshBytes;              ///< zero for default value: 1/4 of total video memory
        UInt32 maxDrawCommandBufferBytes; ///< maximum draw command buffer bytes. Must be large enough to hold draw requests of one frame.
        //@}
    };

    ///
    /// major render engine interface.
    ///
    /// \note
    /// - render engine is _NOT_ thread safe.
    /// - all the methods, except explicitly stated, must be called in serialized way.
    ///
    class RenderEngine : public StdClass
    {
        GN_DECLARE_STDCLASS( RenderEngine, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        RenderEngine() : mApiReentrantFlag(0) { clear(); }
        virtual ~RenderEngine() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const RenderEngineInitParameters & );
        void quit();
    private:
        void clear();
        //@}

        // ********************************
        /// \name rendering device management
        // ********************************
    public:

        //@{

        ///
        /// reset rendering device.
        ///
        bool resetRenderer( gfx::RendererAPI, const gfx::RendererOptions & );

        ///
        /// get renderer options
        ///
        const gfx::RendererOptions & getRendererOptions() const;

        ///
        /// get display properties
        ///
        const gfx::DispDesc & getDispDesc() const;

        ///
        /// get total video meory size in bytes
        ///
        size_t getTotalVideoMemorySize() const;

        ///
        /// set cache capacity
        ///
        void setResourceCacheCapacity( size_t maxTexBytes, size_t maxMeshBytes );

    private:

        ///
        /// do reset w/o checking API reentrance
        ///
        bool internalResetRenderer( gfx::RendererAPI, const gfx::RendererOptions & );

        //@}

        // ********************************
        /// \name draw request management.
        // ********************************
    public:

        //@{

        void frameBegin();
        void frameEnd();

        // below commands must called between frameBegin() and frameEnd().

        void setContext( const DrawContext & context );

        void setShaderUniform(
            GraphicsResource        * shader,
            const StrA              & uniformName,
            const gfx::UniformValue & value );

        void clearScreen(
            const Vector4f & c = Vector4f(0,0,0,1),
            float z = 1.0f, UInt8 s = 0,
            BitFields flags = gfx::CLEAR_ALL );

        void drawIndexed( SInt32 prim,
                          size_t numprim,
                          size_t startvtx,
                          size_t minvtxidx,
                          size_t numvtx,
                          size_t startidx );

        void draw( SInt32 prim,
                   size_t numprim,
                   size_t startvtx );

        void drawIndexedUp( SInt32         prim,
                            size_t         numprim,
                            size_t         numvtx,
                            const void *   vertexData,
                            size_t         strideInBytes,
                            const UInt16 * indexData );

        void drawUp( SInt32        prim,
                     size_t        numprim,
                     const void *  vertexData,
                     size_t        strideInBytes );

        void drawLines(
            BitFields         options,
            const void *      positions,
            size_t            stride,
            size_t            count,
            UInt32            rgba,
            const Matrix44f & model,
            const Matrix44f & view,
            const Matrix44f & proj );

        //@}

        // ********************************
        /// \name resource commands
        // ********************************
    public:

        /// alloc() and free() will stall rendering process. So do not
        /// call them frequently in hot path.
        ///
        //@{

        GraphicsResource * allocResource( const GraphicsResourceDesc & );
        void               freeResource( GraphicsResource * );
        bool               checkResource( const GraphicsResource * ) const;

        void disposeResource( GraphicsResource * );
        void disposeAllResources();

        ///
        /// \note
        ///     Render engine will hold a reference to the loader. So users can
        ///     safely release their own reference to the loader.
        ///
        void updateResource( GraphicsResource       * resource,
                             int                      lod,
                             GraphicsResourceLoader * loader );

        //@}

        // ********************************
        /// \name mini application management
        // ********************************
    public:

        //@{

        MiniAppId registerMiniApp( MiniApp * );
        MiniApp * unregisterMiniApp( MiniAppId );
        void      runMiniApp( MiniAppId );

        //@}

        // ********************************
        /// \name helpers
        // ********************************
    public:

        //@{

        GraphicsResource *
        createShader( const StrA       & name,
                      const ShaderDesc & desc );

        GraphicsResource *
        createShader( const StrA         & name,
                      gfx::ShaderType      type,
                      gfx::ShadingLanguage lang,
                      const StrA         & code,
                      const StrA         & hints = StrA::EMPTYSTR );

        ///
        /// Create new texture
        /// See TextureDesc for detail explaination of each fields in descriptor.
        ///
        GraphicsResource *
        createTexture( const StrA & name, const gfx::TextureDesc & desc );

        ///
        /// Create new texture, with individual creation parameters.
        ///
        GraphicsResource *
        createTexture( const StrA & name,
                       gfx::TexDim  dim,
                       size_t       sx,
                       size_t       sy,
                       size_t       sz,
                       size_t       faces = 0,
                       size_t       levels = 0,
                       gfx::ClrFmt  format = gfx::FMT_DEFAULT,
                       BitFields    usage = 0 )
        {
            gfx::TextureDesc desc =
            {
                dim,
                (UInt32)sx, (UInt32)sy, (UInt32)sz,
                (UInt32)faces, (UInt32)levels,
                format,
                { usage }
            };
            return createTexture( name, desc );
        }

        ///
        /// Create 1D texture
        ///
        GraphicsResource *
        create1DTexture( const StrA & name,
                         size_t       sx,
                         size_t       levels = 0,
                         gfx::ClrFmt  format = gfx::FMT_DEFAULT,
                         BitFields    usage = 0 )
        {
            return createTexture( name, gfx::TEXDIM_1D, sx, 1, 1, 1, levels, format, usage );
        }

        ///
        /// Create 2D texture
        ///
        GraphicsResource *
        create2DTexture( const StrA & name,
                         size_t       sx,
                         size_t       sy,
                         size_t       levels = 0,
                         gfx::ClrFmt  format = gfx::FMT_DEFAULT,
                         BitFields    usage = 0 )
        {
            return createTexture( name, gfx::TEXDIM_2D, sx, sy, 1, 1, levels, format, usage );
        }

        ///
        /// Create 3D texture
        ///
        GraphicsResource *
        create3DTexture( const StrA & name,
                         size_t       sx,
                         size_t       sy,
                         size_t       sz,
                         size_t       levels = 0,
                         gfx::ClrFmt  format = gfx::FMT_DEFAULT,
                         BitFields    usage = 0 )
        {
            return createTexture( name, gfx::TEXDIM_3D, sx, sy, sz, 1, levels, format, usage );
        }

        ///
        /// Create CUBE texture
        ///
        GraphicsResource *
        createCubeTexture( const StrA & name,
                           size_t       sx,
                           size_t       levels = 0,
                           gfx::ClrFmt  format = gfx::FMT_DEFAULT,
                           BitFields    usage = 0 )
        {
            return createTexture( name, gfx::TEXDIM_CUBE, sx, sx, 1, 6, levels, format, usage );
        }

        GraphicsResource *
        createVtxFmt( const StrA            & name,
                      const gfx::VtxFmtDesc & desc );

        GraphicsResource *
        createVtxBuf( const StrA            & name,
                      const gfx::VtxBufDesc & desc );

        GraphicsResource *
        createVtxBuf( const StrA & name,
                      size_t       bytes,
                      bool         dynamic = false,
                      bool         readback = false );

        GraphicsResource *
        createIdxBuf( const StrA            & name,
                      const gfx::IdxBufDesc & desc );

        GraphicsResource *
        createIdxBuf( const StrA & name,
                      size_t       numidx,
                      bool         dynamic = false,
                      bool         readback = false );

        ///
        /// reset draw context to default value (not referencing any resources)
        ///
        void clearDrawContext();

        //@}

        // ********************************
        /// \name sub component accessor
        // ********************************
    public:

        ///
        /// These sub-component are used internally by render engine
        ///
        //@{

        class GraphicsResourceCache;
        class ResourceLRU;
        class DrawThread;
        class ResourceThread;
        class FenceManager;

        GraphicsResourceCache & resourceCache()  { GN_ASSERT(mResourceCache); return *mResourceCache; }
        ResourceLRU           & resourceLRU()    { GN_ASSERT(mResourceLRU); return *mResourceLRU; }
        DrawThread            & drawThread()     { GN_ASSERT(mDrawThread); return *mDrawThread; }
        ResourceThread        & resourceThread() { GN_ASSERT(mResourceThread); return *mResourceThread; } 
        FenceManager          & fenceManager()   { GN_ASSERT(mFenceManager); return *mFenceManager; } 

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        enum RendererSignalType
        {
            RENDERER_CREATE,
            RENDERER_RESTORE,
            RENDERER_DISPOSE,
            RENDERER_DESTROY,
            RENDERER_SIZEMOVE,
        };
        struct RendererSignal
        {
            RendererSignalType type;
            union
            {
                struct
                {
                    HandleType monitor;
                    UInt32     width;
                    UInt32     height;
                } sizemove;
            };

            RendererSignal() {}
            RendererSignal( RendererSignalType t ) : type(t) {}
        };

        GraphicsResourceCache           * mResourceCache;
        ResourceLRU                     * mResourceLRU;
        DrawThread                      * mDrawThread;
        ResourceThread                  * mResourceThread;
        FenceManager                    * mFenceManager;

        DrawContext                       mDrawContext;

        bool                              mFrameBegun;

        // render engine API is not reentrant-safe
        mutable volatile SInt32           mApiReentrantFlag;

        HandleManager<MiniApp*,MiniAppId> mMiniApps;

        DynaArray<RendererSignal>         mRendererSignals;
        mutable SpinLoop                  mRendererSignalMutex;

        // ********************************
        // private functions
        // ********************************
    private:

        // these 4 methods run in draw thread (called by renderer signals)
        bool onRendererCreate();
        bool onRendererRestore();
        void onRendererDispose();
        void onRendererDestroy();
        void onRenderWindowSizeMove( HandleType m, UInt32 w, UInt32 h );
    };

    ///
    /// free graphics resource safely
    ///
    inline void safeFreeGraphicsResource( GraphicsResource * & res )
    {
        if( res )
        {
            res->engine.freeResource( res );
            res = 0;
        }
    }

    ///
    /// automatic graphics resource pointer (free resource when out of life scope)
    ///
    struct AutoGraphicsResource : public detail::BaseAutoPtr<GraphicsResource, AutoGraphicsResource>
    {
        typedef detail::BaseAutoPtr<GraphicsResource, AutoGraphicsResource> ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr<GraphicsResource, AutoGraphicsResource>;
#else
        friend class ParentType;
#endif

        static void sDoRelease( GraphicsResource * p )
        {
            if( p ) p->engine.freeResource( p );
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoGraphicsResource( GraphicsResource * p = 0 ) throw() : ParentType(p) {}

        ///
        /// Convert to T *
        ///
        operator GraphicsResource * () const { return get(); }

        ///
        /// arrow operator
        ///
        GraphicsResource * operator->() const { return get(); }
    };
}}

// *****************************************************************************
//                           End of renderEngine.h
// *****************************************************************************
#endif // __GN_ENGINE_RENDERENGINE_H__
