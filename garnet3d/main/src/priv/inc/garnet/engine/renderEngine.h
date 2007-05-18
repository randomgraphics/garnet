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
    /// coming soon ...
    ///
    struct RenderEngineInitParameters
    {
        //@{
        UInt32 maxTexBytes;   ///< zero for default value: 3/4 of total video memory
        UInt32 maxMeshBytes;  ///< zero for default value: 1/4 of total video memory
        UInt32 maxDrawCommandBufferBytes;
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
        RenderEngine()          { clear(); }
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
        void clear()
        {
            mResourceCache = 0;
            mDrawThread = 0;
            mResourceThread = 0;
        }
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
                          UInt32 numprim,
                          UInt32 startvtx,
                          UInt32 minvtxidx,
                          UInt32 numvtx,
                          UInt32 startidx );

        void draw( SInt32 prim,
                   UInt32 numprim,
                   UInt32 startvtx );

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
        createIdxBuf( const StrA            & name,
                      const gfx::IdxBufDesc & desc );

        GraphicsResource *
        createIdxBuf( const StrA & name,
                      UInt32       numidx,
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

        GraphicsResourceCache * mResourceCache;
        ResourceLRU           * mResourceLRU;
        DrawThread            * mDrawThread;
        ResourceThread        * mResourceThread;
        FenceManager          * mFenceManager;

        DrawContext mDrawContext;

        // ********************************
        // private functions
        // ********************************
    private:
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
