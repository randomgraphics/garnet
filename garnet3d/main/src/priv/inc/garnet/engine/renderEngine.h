#ifndef __GN_ENGINE_RENDERENGINE_H__
#define __GN_ENGINE_RENDERENGINE_H__
// *****************************************************************************
//! \file    engine/renderEngine.h
//! \brief   interface of render engine
//! \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

#include "garnet/base/thread.h"

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
    /// - load() and copy() will be used in seralized way. There's no need to be thread safe for these 2 methods.
    /// - decompress() might be called simutaneously, only when the loader is attached to multiple resources that the same time.
    ///
    struct GraphicsResourceLoader : public NoCopy
    {
        virtual bool load( void * & data, size_t & bytes, int lod ) = 0;  ///< load from external/slow storage (disk, cdrom, network)

        virtual bool decompress( void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int lod ) = 0; ///< decompress or other process to prepare to copy to graphics resource.

        virtual bool copy( GraphicsResource & gfxres, const void * data, size_t bytes, int lod ) = 0;
    };

    ///
    /// ...
    ///
    enum GraphicsResourceOperation
    {
        GROP_LOAD,       ///< load from external/slow/remote storage. Handled by IO tread.

        GROP_COPY,       ///< copy data to graphics resource. Handled by copy thread.

        GROP_DECOMPRESS, ///< do decompress or other process to prepare to copy to graphics resource.
                         ///< Handled by processing thread.

        /// these operations are handled by rendering thread
        //@{
        GROP_LOCK,       ///< lock the graphics resource, realize those disposed as well.
        GROP_UNLOCK,     ///< unlock the graphics resource.
        GROP_DISPOSE,    ///< dispose the resource.
        //@}
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
        GraphicsResourceOperation op;               ///< requested operation. Any operation, except OP_DISPOSE.
        GraphicsResourceId        resourceId;       ///< target resource

        // below parameters will be ignored, for OP_DISPOSE.

        FenceId                   waitForDrawFence; ///< the request must be happend after this draw fence
        volatile SInt32         * pendingResources; ///< when this request is done. It'll decrease value pointed by this pointer.
        int                       targetLod;        ///< ...
        GraphicsResourceLoader  * loader;           ///< ...
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
    private:

        volatile SInt32    pendingResources; ///< number of resources that has to be updated before this draw happens.

    public:

        FenceId            fence;            ///< fence ID of this draw
        DrawContext        context;          ///< context ID

        //@{
        gfx::PrimitiveType primtype;
        UInt32             numprim;
        UInt32             startvtx;
        UInt32             minvtxidx;
        UInt32             numvtx;
        UInt32             startidx;
        //@}

        //@{
        void   incPendingResourceCount() { atomInc32( &pendingResources ); }
        void   decPendingResourceCount() { GN_ASSERT( getPendingResourceCount() > 0 ); atomDec32( &pendingResources ); }
        SInt32 getPendingResourceCount() const { return atomGet32(&pendingResources); }
        void   attachResourceCommand( GraphicsResourceCommand & cmd ) { cmd.pendingResources = &pendingResources; }
        //@}
    };

    ///
    /// ...
    ///
    struct RenderEngineInitParameters
    {
        //@{
        UInt32 maxTexBytes;
        UInt32 maxMeshBytes;
        UInt32 maxDrawCommandBufferBytes;
        //@}
    };

    ///
    /// major render engine interface
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
            mGfxResCache = 0;
            mDrawThread = 0;
            mResourceThread = 0;
        }
        //@}

        // ********************************
        /// \name draw request management. Normally called by game update thread.
        ///
        /// 1. There should be no more than one threads that call these functions at the same time.
        /// 2. All methods in this section must be called in the thread that calls frameBegin().
        ///
        // ********************************
    public:

        //@{

        void frameBegin();
        void frameEnd();

        ///
        /// Must called between frameBegin() and frameEnd(). And the returned reference
        /// to draw request object will be invalidated after frameEnd().
        ///
        DrawCommand & newDrawCommand();

        void composeAndSubmitResourceCommand(
            DrawCommand &             dr,
            GraphicsResourceOperation op,
            GraphicsResourceId        resource,
            int                       lod,
            GraphicsResourceLoader  * loader,
            //void                    * parameters,   // loader parameters
            //size_t                    bytes,        // loader parameter bytes
            bool                      reload ); // force resource reload
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

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        class GraphicsResourceCache;
        class DrawThread;
        class ResourceThread;

        GraphicsResourceCache * mGfxResCache;
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
