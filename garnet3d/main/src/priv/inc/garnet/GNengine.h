#ifndef __GN_ENGINE_GNENGINE_H__
#define __GN_ENGINE_GNENGINE_H__
// *****************************************************************************
/// \file
/// \brief   public header of engine module
/// \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

#include "GNcore.h"
#include "GNgfx.h"

namespace GN { /** namespace for engine module */ namespace engine
{
    ///
    /// graphics resource type
    ///
    enum GraphicsResourceType
    {
        //@{
        GRT_SURFACE,
        GRT_PARAMETER_SET,
        GRT_PORT_BINDING,
        GRT_KERNEL,
        NUM_GRAPHICS_RESOURCE_TYPES,
        GRT_INVALID = NUM_GRAPHICS_RESOURCE_TYPES,
        //@}
    };

    ///
    /// convert graphics resource type to string
    ///
    inline const char * graphicsResourceType2String( int type )
    {
        static const char * table[] = {
            "SURFACE",
            "PARAMETER_SET",
            "PORT_BINDING",
            "KERNEL",
        };
        GN_CASSERT( GN_ARRAY_COUNT(table) == NUM_GRAPHICS_RESOURCE_TYPES );

        if( 0 <= type && type < NUM_GRAPHICS_RESOURCE_TYPES )
            return table[type];
        else
            return "INVALID";
    }

    struct GraphicsResource;

    ///
    /// describe view to surface resource
    ///
    struct SurfaceResourceView
    {
        GraphicsResource * surf;       ///< surface pointer
        UInt32             firstLevel; ///< first mipmap level. 0 means the most detailed level.
        UInt32             numLevels;  ///< set 0 for all levels staring from firstLevel.
        UInt32             firstFace;  ///< first face index, starting from 0
        UInt32             numFaces;   ///< set to 0 for all faces starting from firstFace.

        ///
        /// ctor
        ///
        SurfaceResourceView() : surf(0) {}

        ///
        /// setup port binding
        ///
        inline void set(
            GraphicsResource * surf_,
            UInt32             firstLevel_,
            UInt32             numLevels_,
            UInt32             firstFace_,
            UInt32             numFaces_ )
        {
            surf       = surf_;
            firstLevel = firstLevel_;
            numLevels  = numLevels_;
            firstFace  = firstFace_;
            numFaces   = numFaces_;
        }
    };

    ///
    /// a bunch of surface views with unique name to each of them.
    ///
    typedef std::map<StrA,SurfaceResourceView> NamedSurfaceResourceViews;

    ///
    /// graphics resource descriptor
    ///
    struct GraphicsResourceDesc
    {
        //@{

        StrA                 name; ///< For log only. Could be any value.

        GraphicsResourceType type; ///< resource type

        /// ...
        struct SurfaceDesc
        {
            gfx::SurfaceCreationParameter creation; ///< surface creation parameters
        } surface;

        /// ...
        struct ParameterSetDesc
        {
            //@{
            StrA kernel;
            //@}
        } paramset;

        /// ...
        struct PortBindingDesc
        {
            //@{
            StrA                      kernel;
            NamedSurfaceResourceViews views;
            //@}
        } binding;

        /// ...
        struct KernelDesc
        {
            StrA kernel; ///< kernel name
        } kernel;

        //@}
    };

    class RenderEngine;

    ///
    /// Graphics resource class.
    ///
    /// \note: access data members only in resource loader.
    ///
    struct GraphicsResource : public NoCopy
    {
        RenderEngine             & engine; ///< reference to the engine that created this resource.

        const GraphicsResourceDesc desc;   ///< resource descriptor

        union
        {
            //@{
            void                    * data;
            gfx::Surface            * surface;
            gfx::KernelParameterSet * paramset;
            gfx::KernelPortBinding  * binding;
            gfx::Kernel             * kernel;
            //@}
        };

    protected:

        ///
        /// protected constructor
        ///
        GraphicsResource( RenderEngine & e, const GraphicsResourceDesc & d ) : engine(e), desc(d), data(0) {}

        ///
        /// protected destructor
        ///
        ~GraphicsResource() {}
    };

    ///
    /// Application defined resource content manipulator that are used to store/restore resource content
    ///
    /// Details about concurrency:
    ///  - load()/store()/download()/upload() won't be called concurrently with itself, but might be called concurrently with other methods
    ///  - decompress()/compress() might be called concurrently with any methods.
    ///
    /// To achieve best performance as well as code simplicity:
    ///    - Try not modify any varialbes other then outbuf and inbuf in loader methods.
    ///    - Or using sync object to prevent content racing.
    ///
    struct GraphicsResourceLoadStore : public RefCounter
    {
        ///
        /// load from external/slow storage (disk, cdrom, network)
        ///
        virtual bool load( const GraphicsResourceDesc & desc, DynaArray<UInt8> & outbuf ) = 0;

        ///
        /// decompress or do other process to prepare for copy to graphics resource.
        ///
        virtual bool decompress( const GraphicsResourceDesc & desc, DynaArray<UInt8> & outbuf, DynaArray<UInt8> & inbuf ) = 0;

        ///
        /// copy data to graphics resource
        ///
        virtual bool download( GraphicsResource & res, DynaArray<UInt8> & inbuf ) = 0;

        ///
        /// store resource data to external/persistent storage
        ///
        virtual bool store( const GraphicsResourceDesc & desc, DynaArray<UInt8> & inbuf ) = 0;

        ///
        /// compress or process data to prepare for store.
        ///
        virtual bool compress( const GraphicsResourceDesc & desc, DynaArray<UInt8> & outbuf, DynaArray<UInt8> & inbuf ) = 0;

        ///
        /// copy data from graphics resource to out buffer
        ///
        virtual bool upload( GraphicsResource & res, DynaArray<UInt8> & outbuf ) = 0;
    };

    ///
    /// resource update is a kind of special resource loader that are use for progressive/partial resource update.
    ///
    /// store(), compress() and upload() will never be called for updater.
    ///
    struct GraphicsResourceUpdater : public GraphicsResourceLoadStore
    {
        //@{
        virtual bool store( const GraphicsResourceDesc &, DynaArray<UInt8> & ) { GN_UNEXPECTED(); return false; }
        virtual bool compress( const GraphicsResourceDesc &, DynaArray<UInt8> &, DynaArray<UInt8> & ) { GN_UNEXPECTED(); return false; }
        virtual bool upload( GraphicsResource &, DynaArray<UInt8> & ) { GN_UNEXPECTED(); return false; }
        //@}
    };

    ///
    /// render engine initialization parameters
    ///
    struct RenderEngineInitParameters
    {
        ///
        /// maximum resource bytes.
        ///
        size_t resourceCapacityInBytes;

        ///
        /// maximum draw command buffer bytes.
        ///
        /// Better be large enough to hold draw requests of one frame for optimal performance.
        ///
        size_t maxDrawCommandBufferBytes;
    };

    ///
    /// major render engine interface.
    ///
    /// \note
    /// - render engine is _NOT_ thread safe.
    /// - all the methods, except explicitly stated, must be called in serialized way.
    ///
    class RenderEngine : public StdClass, public SlotBase
    {
        GN_DECLARE_STDCLASS( RenderEngine, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        RenderEngine();
        virtual ~RenderEngine();
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
        /// \name graphics system management
        // ********************************
    public:

        //@{

        ///
        /// reset/recreate graphics sytem.
        ///
        bool reset( const gfx::GraphicsSystemCreationParameter & );

        ///
        /// get graphics system creation parameters
        ///
        const gfx::GraphicsSystemCreationParameter & getGraphicsSystemCreationParameter() const;

        ///
        /// get graphics system descriptor
        ///
        const gfx::GraphicsSystemDesc & getGraphicsSystemDesc() const;

        ///
        /// get total video memory size in bytes
        ///
        size_t getTotalVideoMemorySize() const;

        ///
        /// set cache capacity
        ///
        void setResourceCacheCapacity( size_t bytes );

        //@}

        // ********************************
        /// \name resource commands
        // ********************************
    public:

        /// \note
        ///
        //@{

        ///
        /// Create new resource.
        ///
        /// \note
        /// - createResource() is a very fast operation that creates resources in "DISPOSED" state.
        /// - Disposed resource occupies very little main memory and zero device memory, which means
        ///   that you cand create extreamly large number of resources before running out of memory.
        /// - render engine class will ensure that memory footprint of all "REALIZED" graphics resources
        ///   is in user defined limit.
        ///
        GraphicsResource * createResource( const GraphicsResourceDesc & desc, GraphicsResourceLoadStore * loadstore );

        ///
        /// delete an exisiting resource.
        ///
        /// \note This function runs syncronizly. Do not call it too frequently in hot path.
        ///
        void deleteResource( GraphicsResource * );

        ///
        /// delete all resource.
        ///
        /// \note This function runs syncronizly.
        ///
        void deleteAllResources();

        ///
        /// make sure a valid resource pointer
        ///
        /// \note This function runs syncronizly. Do not call it too frequently in hot path.
        ///
        bool checkResource( const GraphicsResource * ) const;

        ///
        /// load resource content, discarding old content.
        ///
        void loadResource( GraphicsResource          * resource,
                           GraphicsResourceLoadStore * loadstore );

        ///
        /// progressive/partial update to the resource
        ///
        void updateResource( GraphicsResource        * resource,
                             GraphicsResourceUpdater * updater );

        ///
        /// Dispose resource to force it to be reloaded.
        ///
        void disposeResource( GraphicsResource * );

        ///
        /// dispose all resources
        ///
        void disposeAllResources();

        //@}

        // ********************************
        /// \name async. draw commands
        // ********************************
    public:

        //@{

        UIntPtr createRenderContext( GraphicsResource * kernel, GraphicsResource * paramset, GraphicsResource * binding );
        void    deleteRenderContext( UIntPtr );

        void render( UIntPtr context );
        void render( GraphicsResource * kernel, GraphicsResource * paramset, GraphicsResource * binding );
        void present();

        //@}

        // ********************************
        /// \name helpers
        // ********************************
    public:

        //@{

        GraphicsResource * getKernel( const StrA & kernel );

        GraphicsResource * createSurface( const StrA & resname, const gfx::SurfaceCreationParameter & );
        GraphicsResource * createParameterSet( const StrA & resname, const StrA & kernel );
        GraphicsResource * createParameterSet( const StrA & resname, const GraphicsResource & kernel );
        GraphicsResource * createPortBinding( const StrA & resname, const StrA & kernel, const NamedSurfaceResourceViews & );
        GraphicsResource * createPortBinding( const StrA & resname, const GraphicsResource & kernel, const NamedSurfaceResourceViews & );

        ///
        /// create vertex buffer
        ///
        GraphicsResource * createVtxBuf(
            const StrA                      & name,
            const gfx::SurfaceElementFormat & format,
            size_t                            count );

        ///
        /// create index buffer
        ///
        GraphicsResource * createIdxBuf( const StrA & name, size_t count );

        ///
        /// create texture from image file
        ///
        GraphicsResource * createTextureFromImageFile( const StrA & filename );

        void               pushStreamData( GraphicsResource * kernel, size_t streamIndex, size_t bytes, const void * data );
        void               pushStreamData( GraphicsResource * kernel, const StrA & streamName, size_t bytes, const void * data );

        void               setParameter( GraphicsResource * paramset, size_t index, size_t offset, size_t bytes, const void * data );
        void               setParameter( GraphicsResource * paramset, const StrA & name, size_t offset, size_t bytes, const void * data );

        template<typename T>
        void               setParameterT( GraphicsResource * paramset, size_t index, const T & value ) { return setParameter( paramset, index, 0, sizeof(value), &value ); }

        template<typename T>
        void               setParameterT( GraphicsResource * paramset, const StrA & name, const T & value ) { return setParameter( paramset, name, 0, sizeof(value), &value ); }

        //@}

        // ********************************
        /// \name sub component accessor
        // ********************************
    public:

        ///
        /// These sub-component are used internally by render engine
        ///
        //@{

        class ResourceCache;
        class ResourceLRU;
        class DrawThread;
        class ResourceThread;
        class FenceManager;

        ResourceCache  & resourceCache()  { GN_ASSERT(mResourceCache); return *mResourceCache; }
        ResourceLRU    & resourceLRU()    { GN_ASSERT(mResourceLRU); return *mResourceLRU; }
        DrawThread     & drawThread()     { GN_ASSERT(mDrawThread); return *mDrawThread; }
        ResourceThread & resourceThread() { GN_ASSERT(mResourceThread); return *mResourceThread; } 
        FenceManager   & fenceManager()   { GN_ASSERT(mFenceManager); return *mFenceManager; } 

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct RefCountedResource
        {
            GraphicsResource * resource;
            size_t             refcounter;
            RefCountedResource() : resource(0), refcounter(0) {}
        };

        struct NamedResourceManager
        {
            std::map<StrA,RefCountedResource> resources;
            void               clear() { resources.clear(); }
            void               add( const StrA & name, GraphicsResource * res );
            size_t             del( const StrA & name ); ///< return modified reference counter
            GraphicsResource * get( const StrA & name );
        };

        struct DrawContext
        {
            DynaArray<GraphicsResource*> resources;
        };

        struct FrameProfiler
        {
            ProfileTimer & timer;
            bool           start;

            FrameProfiler()
                : timer( ProfilerManager::sGetGlobalInstance().getTimer("RenderEngine_FrameTime") )
                , start( 0 )
            {
            }

            ~FrameProfiler()
            {
                if( start ) timer.stop();
            }

            void nextFrame()
            {
                if( start ) timer.stop();
                timer.start();
                start = true;
            }
        };

        FenceManager                     * mFenceManager;
        ResourceCache                    * mResourceCache;
        ResourceLRU                      * mResourceLRU;
        DrawThread                       * mDrawThread;
        ResourceThread                   * mResourceThread;

        NamedResourceManager               mKernels;

        HandleManager<DrawContext,UIntPtr> mDrawContexts;

        FrameProfiler                      mFrameProfiler;

        // to avoid render engine API re-entrance
        mutable volatile SInt32            mApiReentrantFlag;

        // ********************************
        // private functions
        // ********************************
    private:

    };

    // *************************************************************************
    // drawable class
    // *************************************************************************

    ///
    /// drawable class
    ///
    class Drawable
    {
        GraphicsResource * mKernel;
        GraphicsResource * mParamSet;
        UIntPtr            mContext;

    public:

        //@{

        ///
        /// constructor
        ///
        Drawable();

        ///
        /// dtor
        ///
        ~Drawable();

        ///
        /// clear to empty
        ///
        void clear();

        ///
        /// is empty drawable or not
        ///
        bool empty() const { return 0 == mContext; }

        ///
        /// get kernel of the drawable
        ///
        GraphicsResource * getKernel() const { return mKernel; }

        ///
        /// get parameter set of the kernel
        ///
        GraphicsResource * getParamSet() const { return mParamSet; }

        ///
        /// render the drawable
        ///
        void render()
        {
            if( empty() ) return;

            GN_ASSERT( mKernel && mParamSet );

            mKernel->engine.render( mContext );
        }

        ///
        /// setup the drawable
        ///
        bool initialize( const StrA & kernelName, GraphicsResource * binding );

        ///
        /// load drawable from XML node
        ///
        bool loadFromXmlNode(
            RenderEngine  & re,
            const XmlNode & node,
            const StrA    & basedir );

        ///
        /// load drawable from XML file
        ///
        bool loadFromXmlFile(
            RenderEngine  & re,
            const StrA    & filename );
    };

    // *************************************************************************
    // helper classes and functions
    // *************************************************************************

    ///
    /// delete graphics resource safely
    ///
    inline void safeDeleteGraphicsResource( GraphicsResource * & res )
    {
        if( res )
        {
            res->engine.deleteResource( res );
            res = 0;
        }
    }

    ///
    /// automatic graphics resource pointer (free resource when out of life scope)
    ///
    class AutoGraphicsResource : public detail::BaseAutoPtr<GraphicsResource, AutoGraphicsResource>
    {
        typedef detail::BaseAutoPtr<GraphicsResource, AutoGraphicsResource> ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr<GraphicsResource, AutoGraphicsResource>;
#else
        friend class ParentType;
#endif

        static void sDoRelease( GraphicsResource * p )
        {
            if( p ) p->engine.deleteResource( p );
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

    ///
    /// helper class to use CLEAR_SCREEN kernel
    ///    
    class ClearScreen : public StdClass
    {
        GN_DECLARE_STDCLASS( ClearScreen, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        ClearScreen()          { clear(); }
        virtual ~ClearScreen() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( RenderEngine & re, GraphicsResource * binding = 0 );
        void quit();
    private:
        void clear() { mKernel = 0; mParam = 0; mContext = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// \name setup clear properites
        //@{

        void setClearColor( bool enabled, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f );

        //@}

        ///
        /// do screen clear
        ///
        void render()
        {
            GN_ASSERT( mKernel && mParam && mContext );
            mKernel->engine.render( mContext );
        }

        // ********************************
        // private variables
        // ********************************
    private:

        GraphicsResource * mKernel;
        GraphicsResource * mParam;
        UIntPtr            mContext;

        size_t
            CLEAR_COLOR,
            CLEAR_DEPTH,
            CLEAR_STENCIL,
            COLOR,
            DEPTH,
            STENCIL;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_GNENGINE2_H__
