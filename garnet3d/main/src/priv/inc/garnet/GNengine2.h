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
        } parameterSet;

        /// ...
        struct PortBindingDesc
        {
            //@{
            StrA kernel;
            //@}
        } portBinding;

        /// ...
        struct KernelDesc
        {
            StrA name; ///< kernel name
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
            gfx::KernelPortBinding    binding;
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

        void draw( const GraphicsResource * kernel, const GraphicsResource * param, const GraphicsResource * binding );
        void present();

        //@}

        // ********************************
        /// \name helpers
        // ********************************
    public:

        //@{

        GraphicsResource * createSurface( const gfx::SurfaceCreationParameter & );
        GraphicsResource * getStream( const StrA & kernel, const StrA & stream );
        GraphicsResource * getKernel( const StrA & name );
        GraphicsResource * createParameterSet( const StrA & kernel );
        GraphicsResource * createPortBinding( const StrA & kernel, const gfx::KernelPortBindingDesc & );

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

        FenceManager                    * mFenceManager;
        ResourceCache                   * mResourceCache;
        ResourceLRU                     * mResourceLRU;
        DrawThread                      * mDrawThread;
        ResourceThread                  * mResourceThread;

        FrameProfiler                     mFrameProfiler;

        // to avoid render engine API re-entrance
        mutable volatile SInt32           mApiReentrantFlag;

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
    class ClearScreen
    {
        GraphicsResource * mKernel;
        GraphicsResource * mParam;

    public:

        ///
        /// ctor
        ///
        ClearScreen() : mKernel( 0 ), mParam( 0 )
        {
        }

        ///
        /// dtor
        ///
        ~ClearScreen()
        {
            safeDeleteGraphicsResource( mKernel );
            safeDeleteGraphicsResource( mParam );
        }

        ///
        /// initialize
        ///
        bool init( RenderEngine & re );

        ///
        /// do screen clear
        ///
        void draw( GraphicsResource * binding = 0 )
        {
            GN_ASSERT( mKernel );
            mKernel->engine.draw( mKernel, mParam, binding );
        }

        /// \name setup clear properites
        //@{

        void setClearColor( bool enabled, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f );

        //@}

    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_GNENGINE_H__
