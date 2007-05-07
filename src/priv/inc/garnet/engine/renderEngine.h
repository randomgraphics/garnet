#ifndef __GN_ENGINE_RENDERENGINE_H__
#define __GN_ENGINE_RENDERENGINE_H__
// *****************************************************************************
//! \file    engine/renderEngine.h
//! \brief   interface of render engine
//! \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// fake const buffer class
    ///
    struct ConstBuf : public RefCounter
    {
    };
}}

namespace GN { namespace engine
{
    ///
    /// shader resourece descriptor
    ///
    struct ShaderDesc
    {
        gfx::ShaderType      type;   ///< shader type
        gfx::ShadingLanguage lang;   ///< shading language
        StrA                 code;   ///< shader code
        StrA                 hints;  ///< shader creation hints
        gfx::VtxFmtDesc      vtxfmt; ///< vertex format descriptor
    };

    ///
    /// ...
    ///
    struct ConstBufDesc
    {
        //@{
        int type;  ///< vector4, int4, bool, matrix44
        int count; ///< number of items, not bytes.
        //@}
    };

    ///
    /// graphics resource type
    ///
    enum GraphicsResourceType
    {
        //@{
        GRT_SHADER,
        GRT_TEXTURE,
        GRT_VTXBUF,
        GRT_IDXBUF,
        GRT_CONSTBUF,
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
        StrA                 name; ///< this field is for log and debug. you may set it to any value.
        GraphicsResourceType type;
        ShaderDesc           sd;
        gfx::TextureDesc     td;
        gfx::VtxBufDesc      vd;
        gfx::IdxBufDesc      id;
        ConstBufDesc         cd;
        //@}
    };

    ///
    /// graphics resource ID. 0 is invalid ID.
    ///
    typedef UInt32 GraphicsResourceId;

    ///
    /// Graphics resource class.
    ///
    /// \note
    ///     Do not read/write data member of this class, unless in
    ///     GraphicsResourceLoader class.
    ///
    struct GraphicsResource : public NoCopy
    {
        const GraphicsResourceId   id;   ///< resource id
        const GraphicsResourceDesc desc; ///< resource descriptor
        union
        {
            //@{
            void          * data;     ///< ...
            gfx::Shader   * shader;   ///< ...
            gfx::Texture  * texture;  ///< ...
            gfx::VtxBuf   * vtxbuf;   ///< ...
            gfx::IdxBuf   * idxbuf;   ///< ...
            gfx::ConstBuf * constbuf; ///< ...
            //@}
        };

    protected:

        ///
        /// protected constructor
        ///
        GraphicsResource( GraphicsResourceId id_, const GraphicsResourceDesc & desc_ )
            : id(id_), desc(desc_), data(0)
        {}

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
    ///  - Seems that freebuf() has to be protected by sync object, to achieve thread safety.
    ///
    struct GraphicsResourceLoader : public RefCounter
    {
        ///
        /// load from external/slow storage (disk, cdrom, network)
        ///
        virtual bool load( void * & outbuf, size_t & outbytes, int lod ) = 0;

        ///
        /// decompress or do other process to prepare for copy to graphics resource.
        ///
        virtual bool decompress( void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int lod ) = 0;

        ///
        /// copy data to graphics resource
        ///
        virtual bool copy( GraphicsResource & gfxres, const void * inbuf, size_t inbytes, int lod ) = 0;

        ///
        /// free data buffer returned by load() and decompress()
        ///
        virtual void freebuf( void * inbuf, size_t inbytes ) = 0;
    };

    ///
    /// \note
    ///     - all resource pointers here must store graphics resource ID, instead of data pointer.
    ///
    typedef gfx::RendererContext DrawContext;

    ///
    /// ...
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
    /// \notes
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
        ///
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
            GraphicsResourceId        shader,
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

        GraphicsResourceId allocResource( const GraphicsResourceDesc & );
        void               freeResource( GraphicsResourceId );
        GraphicsResource * getResourceById( GraphicsResourceId );

        void disposeResource( GraphicsResourceId );
        void disposeAllResources();

        ///
        /// \note
        ///     Render engine will hold a reference to the loader. So users can
        ///     safely release their own reference to the loader.
        ///
        void updateResource( GraphicsResourceId       resource,
                             int                      lod,
                             GraphicsResourceLoader * loader );

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
}}

// *****************************************************************************
//                           End of renderEngine.h
// *****************************************************************************
#endif // __GN_ENGINE_RENDERENGINE_H__
