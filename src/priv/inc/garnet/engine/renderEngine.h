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
        gfx::ShaderType      type;  ///< shader type
        gfx::ShadingLanguage lang;  ///< shading language
        StrA                 code;  ///< shader code
        StrA                 hints; ///< shader creation hints
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
            : id(id_), desc(desc_), shader(0)
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
        virtual void freebuf( void * inbuf, size_t inbytes );
    };

    ///
    /// ...
    ///
    struct DrawContext
    {
        ///
        /// single shader resource descriptor
        ///
        struct ShaderResource
        {
            int                binding;  ///< binding slot to the shader
            GraphicsResourceId resource; ///< resource id.

            union
            {
                struct
                {
                    UInt32     offset; ///< byte offset of the first vertex in buffer.
                    UInt32     stride; ///< vertex stride in bytes
                } vb; ///< vertex buffer properties
            };
        };

        ///
        /// shader descriptor
        ///
        struct ShaderDesc
        {
            GraphicsResourceId shader;        ///< shader itself.
            ShaderResource     resources[64]; ///< resource list. Each shader can reference at most 64 resources at any time.
            UInt32             count;         ///< resource count
        };

        ///
        /// ...
        ///
        struct RenderTargetTexture
        {
            GraphicsResourceId texture;    ///< render target texture
            unsigned int       face  : 14; ///< cubemap face. Must be zero for non-cube/stack texture.
            unsigned int       slice : 14; ///< slice index. Must be zero for non 3D texture.
            unsigned int       level :  4; ///< mipmap level
        };

        ///
        /// ...
        ///
        struct RenderTargetDesc
        {
            RenderTargetTexture cbuffers[gfx::MAX_RENDER_TARGETS]; ///< color buffer descriptions. Ignored when draw to back buffer.
            RenderTargetTexture zbuffer; ///< z buffer description. Set zbuffer.texture to NULL to use auto-zbuffer.
            unsigned int        count :  5; ///< color buffer count. 0 means draw to back buffer.
            unsigned int        aa    :  3; ///< anti-alias type. One of MsaaType. Ignored when draw to back buffer.
        };

        ShaderDesc                shaders[gfx::NUM_SHADER_TYPES];   ///< shaders and their resouces
        RenderTargetDesc          renderTargets;                    ///< render target descriptor
        Rectf                     viewport;                         ///< Viewport. [0,0,1,1] means whole render target.
        gfx::RenderStateBlockDesc rsb;                              ///< render state block.
        gfx::VtxFmtDesc           vtxfmt;                           ///< vertex format.
    };

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

        // below commands must called in between of frameBegin() and frameEnd().

        void setContext( const DrawContext & context );

        void clearScreen(
            const Vector4f & c = Vector4f(0,0,0,1),
            float z = 1.0f, UInt8 s = 0,
            BitFields flags = gfx::CLEAR_ALL );

        void draw( ... );
        void drawindexed( ... );

        //@}

    private:

        inline void prepareResource( GraphicsResourceId id );

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

        ///
        /// \note
        ///     Render engine will hold a reference to the loader. So users can
        ///     safely release their own refernence to the loader.
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
