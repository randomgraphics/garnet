#ifndef __GN_ENGINE_GNENGINE_H__
#define __GN_ENGINE_GNENGINE_H__
// *****************************************************************************
/// \file
/// \brief   public header of engine module
/// \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

#include "GNcore.h"
#include "GNgfx2.h"

namespace GN { /** namespace for engine2 */ namespace engine2
{
    ///
    /// graphics resource type
    ///
    enum GraphicsResourceType
    {
        //@{
        GRT_SURFACE,
        GRT_STREAM,
        GRT_PARAMETER_SET,
        GRT_PORT_BINDING,
        GRT_KERNEL,
        NUM_GRAPHICS_RESOURCE_TYPES,
        GRT_INVALID = NUM_GRAPHICS_RESOURCE_TYPES,
        //@}
    };

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
        struct StreamDesc
        {
            //@{
            StrA kernel; ///< kernel name
            StrA stream; ///< stream name
            //@}
        } stream;

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
            StrA                               kernel;
            std::map<StrA,SurfaceResourceView> views;
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
            gfx::StreamSource       * stream;
            gfx::KernelParameterSet * paramset;
            gfx::KernelPortBinding  * binding;
            gfx::Kernel             * kernel;
            //@}
        };

        StrA toString() const; ///< convert to string (for log only)

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
    /// Application defined graphics resource loader.
    ///
    /// Details about concurrency:
    ///  - load() won't be called concurrently with itself, but might be called concurrently with other methods
    ///  - copy() won't be called concurrently with itself, but might be called concurrently with other methods
    ///  - decompress() and freebuf() could be called concurrently with any methods.
    ///
    /// So, to achieve maximum performance, it is advised to avoid using sync objects as much as possible.
    /// The possible implementation could be:
    ///  - keep data used by each method separated. So they won't mess with each other, when called concurrently.
    ///  - do not modify any states in decompress(). So it can be safely called anytime anywhere, w/o using sync objects.
    ///
    struct GraphicsResourceLoader : public RefCounter
    {
        ///
        /// load from external/slow storage (disk, cdrom, network)
        ///
        virtual bool load( const GraphicsResourceDesc & desc, DynaArray<UInt8> & outbuf ) = 0;

        ///
        /// decompress or do other process to prepare for copy to graphics resource.
        ///
        virtual bool decompress( const GraphicsResourceDesc & desc, DynaArray<UInt8> & outbuf, const DynaArray<UInt8> & inbuf ) = 0;

        ///
        /// copy data to graphics resource
        ///
        virtual bool copy( GraphicsResource & res, const DynaArray<UInt8> & inbuf ) = 0;
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
        /// get total video meory size in bytes
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

        /// All these methods are executed asyncronizly, unless explicitly speficied.
        //@{

        ///
        /// create new resource
        ///
        GraphicsResource * createResource( const GraphicsResourceDesc & desc );

        ///
        /// delete an exisiting resource.
        ///
        /// \note This function runs syncronizly. Do not call it too frequently in hot path.
        ///
        void deleteResource( GraphicsResource * );

        ///
        /// make sure a valid resource pointer
        ///
        /// \note This function runs syncronizly. Do not call it too frequently in hot path.
        ///
        bool checkResource( const GraphicsResource * ) const;

        ///
        /// \note
        ///     Render engine will hold a reference to the loader. So users can
        ///     safely release their own reference to the loader.
        ///
        void updateResource( GraphicsResource       * resource,
                             GraphicsResourceLoader * loader );

        ///
        /// Dispose resource to force it to be reloaded.
        ///
        void disposeResource( GraphicsResource * );

        void disposeAllResources();

        //@}

        // ********************************
        /// \name async. draw commands
        // ********************************
    public:

        //@{

        UIntPtr createRenderContext( GraphicsResource * kernel, GraphicsResource * param, GraphicsResource * binding );
        void    deleteRenderContext( UIntPtr );

        void render( UIntPtr context );
        void present();

        //@}

        // ********************************
        /// \name helpers
        // ********************************
    public:

        //@{

        GraphicsResource * getKernel( const StrA & kernel );

        GraphicsResource * getStream( const StrA & kernel, const StrA & stream );
        GraphicsResource * getStream( GraphicsResource & kernel, const StrA & stream );

        GraphicsResource * createSurface( const StrA & resname, const gfx::SurfaceCreationParameter & );
        GraphicsResource * createParameterSet( const StrA & resname, const StrA & kernel );
        GraphicsResource * createParameterSet( const StrA & resname, GraphicsResource & kernel );
        GraphicsResource * createPortBinding( const StrA & resname, const StrA & kernel, const std::map<StrA,SurfaceResourceView> & );
        GraphicsResource * createPortBinding( const StrA & resname, GraphicsResource & kernel, const std::map<StrA,SurfaceResourceView> & );

        void               setParameter( GraphicsResource * paramset, size_t index, size_t offset, size_t bytes, const void * data );
        void               setParameter( GraphicsResource * paramset, const StrA & name, size_t offset, size_t bytes, const void * data );

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
            size_t             del( const StrA & name, GraphicsResource * res ); ///< return modified reference counter
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
        NamedResourceManager               mStreams;

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
        GraphicsResource * mBinding;
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
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_GNENGINE_H__
