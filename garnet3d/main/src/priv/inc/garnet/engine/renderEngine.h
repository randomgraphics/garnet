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
    struct ConstBuf : public NoCopy
    {
    };
}}

namespace GN { namespace engine
{
    ///
    /// shader resourece descriptor
    ///
    struct ShaderCreationParameters
    {
        //@{
        gfx::ShaderType      type;
        gfx::ShadingLanguage lang;
        StrA                 code;
        StrA                 hints;
        //@}
    };

    ///
    /// ...
    ///
    struct TextureCreationParameters
    {
        //@{
        gfx::TextureDesc desc;
        //@}
    };

    ///
    /// ...
    ///
    struct VtxBufCreationParameters
    {
        //@{
        gfx::VtxBufDesc desc;
        //@}
    };

    ///
    /// ...
    ///
    struct IdxBufCreationParameters
    {
        //@{
        gfx::IdxBufDesc desc;
        //@}
    };

    ///
    /// ...
    ///
    struct ConstBufCreationParameters
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
    /// ...
    ///
    struct GraphicsResourceCreationParameter
    {
        //@{
        GraphicsResourceType      type;
        ShaderCreationParameters  sd;
        TextureCreationParameters td;
        VtxBufCreationParameters  vd;
        IdxBufCreationParameters  id;
        //@}
    };

    ///
    /// graphics resource ID. 0 is invalid ID.
    ///
    typedef UInt32 GraphicsResourceId;

    ///
    /// ...
    ///
    struct GraphicsResource : public NoCopy
    {
        const GraphicsResourceId   id;   ///< resource id
        const GraphicsResourceType type; ///< resource type. shader, texture, vb, ib, const.
        int                        lod;  ///< current LOD. Definition of LOD is resource/application dependent.
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
        GraphicsResource( GraphicsResourceId id_, GraphicsResourceType type_ )
            : id(id_), type(type_), lod(0)
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
    /// resource operations will happens in strict order as the enum itself.
    ///
    enum GraphicsResourceOperation
    {
        GROP_LOAD,       ///< load from external/slow/remote storage. Handled by IO tread.

        GROP_DECOMPRESS, ///< do decompress or other process to prepare to copy to graphics resource.
                         ///< Handled by decompress thread.

        GROP_COPY,       ///< copy data to graphics resource. Handled by draw thread.

        GROP_DISPOSE,    ///< dispose the resource. Handled by draw thread
    };

    ///
    /// fence ID type
    ///
    typedef int FenceId;

    ///
    /// ...
    ///
    struct GraphicsResourceCommand
    {
        //@{
        GraphicsResourceOperation             op;               ///< requested operation.
        GraphicsResourceId                    resourceId;       ///< target resource
        FenceId                               waitForDrawFence; ///< the request must be happend after this draw fence. For lock/unlock/dispose only.
        int                                   targetLod;        ///< ...
        AutoRef<GraphicsResourceLoader,Mutex> loader;           ///< ...
        volatile SInt32                     * pendingResources; ///< when this request is done. It'll decrease value pointed by this pointer.
        //@}

        //@{
        void decPendingResourceCount() { GN_ASSERT( atomGet32(pendingResources) > 0 ); atomDec32( pendingResources ); }
        //@}
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
        /// shader resource list
        ///
        struct ShaderResourceList
        {
            ShaderResource resources[64]; ///< resource list
            UInt32         count;         ///< resource count
        };

        // states
        GraphicsResourceId        shaders[gfx::NUM_SHADER_TYPES]; ///< shaders
        gfx::RenderStateBlockDesc rsb;                            ///< render state block.
        gfx::RenderTargetDesc     renderTargets;                  ///< render target descriptor
        Rectf                     viewport;                       ///< Viewport. [0,0,1,1] means whole render target.
        gfx::VtxFmtHandle         vtxfmt;                         ///< vertex format handle. 0 means no vertex data at all.

        // resources
        ShaderResourceList        resources[gfx::NUM_SHADER_TYPES]; ///< each shader has an resource list.
    };

    ///
    /// ...
    ///
    struct DrawCommand
    {
        FenceId            fence;            ///< fence ID of this draw
        volatile SInt32    pendingResources; ///< number of resources that has to be updated before this draw happens.
        int                action;           ///< 0: bindcontext, 1: clear, 2: draw, 3: drawindexed

        //@{
        union
        {
            DrawContext context; ///< draw context

            struct
            {
                //@{
                float            r, g, b, a;
                float            z;
                UInt8            s;
                BitFields        flags;
                Vector4f       & color() { return *(Vector4f*)&r; }
                const Vector4f & color() const { return *(Vector4f*)&r; }
                //@}
            } clear; ///< clear parameters

            struct
            {
                //@{
                gfx::PrimitiveType primtype;
                UInt32             numprim;
                UInt32             startvtx;
                //@}
            } draw; ///< draw parameters

            struct
            {
                //@{
                gfx::PrimitiveType primtype;
                UInt32             numprim;
                UInt32             startvtx;
                UInt32             minvtxidx;
                UInt32             numvtx;
                UInt32             startidx;
                //@}
            } drawindexed; ///< drawindexed parameters
        };
        //@}

        //@{
        void   incPendingResourceCount() { atomInc32( &pendingResources ); }
        SInt32 getPendingResourceCount() const { return atomGet32(&pendingResources); }
        //@}
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

        //@}

        // ********************************
        /// \name draw request management.
        // ********************************
    public:

        //@{

        void frameBegin();
        void setContext( gfx::RendererContext & context );
        void clearScreen( ... );
        void draw( ... );
        void drawindexed( ... );
        void frameEnd();

        /*
        /// Must called between frameBegin() and frameEnd(). And the returned reference
        /// to draw request object will be invalidated after frameEnd().
        ///
        DrawCommand & newDrawCommand();

        void submitResourceCommand(
            DrawCommand &             dr,
            GraphicsResourceId        resource,
            int                       lod,
            GraphicsResourceLoader  * loader,
            bool                      reload ); // force resource reload
        */

        //@}

        // ********************************
        /// \name reusable graphics resource cache
        // ********************************
    public:

        ///
        /// these calls are not thead safe:
        ///
        /// - alloc() and free() must be called in serialized way.
        /// - id2res() won't change cache states. So it can be called simutaneously with itself,
        ///   but still need to be synced with alloc() and free().
        ///
        //@{

        GraphicsResourceId allocres( const GraphicsResourceCreationParameter & );
        void               freeres( GraphicsResourceId );
        GraphicsResource * id2res( GraphicsResourceId );
        void               updateres( GraphicsResourceId       resource,
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
        class DrawThread;
        class ResourceThread;

        GraphicsResourceCache & resourceCache()  { GN_ASSERT(mResourceCache); return *mResourceCache; }
        DrawThread            & drawThread()     { GN_ASSERT(mDrawThread); return *mDrawThread; }
        ResourceThread        & resourceThread() { GN_ASSERT(mResourceThread); return *mResourceThread; } 

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        GraphicsResourceCache * mResourceCache;
        DrawThread            * mDrawThread;
        ResourceThread        * mResourceThread;

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
