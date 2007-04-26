#ifndef __GN_ENGINE_RENDERENGINE_H__
#define __GN_ENGINE_RENDERENGINE_H__
// *****************************************************************************
//! \file    engine/renderEngine.h
//! \brief   interface of render engine
//! \author  chenli@@FAREAST (2007.4.13)
// *****************************************************************************

namespace GN { namespace engine
{
    struct ShaderResourceDesc
    {
        gfx::ShaderType      type;
        gfx::ShadingLanguage lang;
        const StrA &         code;
        const StrA &         hints;
    };

    struct TextureResourceDesc
    {
        gfx::TextureDesc desc;
        gfx::ImageDesc * initialData;
    };

    struct VtxBufResourceDesc
    {
        gfx::VtxBufDesc desc;
        const void *    initialData;
    };

    struct IdxBufResourceDesc
    {
        gfx::IdxBufDesc desc;
        const void *    initialData;
    };

    struct GraphicsResourceDesc
    {
        int                 type;
        ShaderResourceDesc  sd;
        TextureResourceDesc td;
        VtxBufResourceDesc  vd;
        IdxBufResourceDesc  id;
    };

    enum GraphicsResourceState
    {
        GRS_READY,
        GRS_DEREALIZED,
    };

    struct GraphicsResource
    {
        int draw_fence; ///< last used by this draw
        int state;
        int type;
        union
        {
            gfx::Shader  * shader;
            gfx::Texture * texture;
            gfx::VtxBuf  * vtxBuf;
            gfx::IdxBuf  * idxBuf;
        };
    };

    typedef int GraphicsResourceId;

    ///
    /// Graphics Resource cache (owned by 
    ///
    class GraphicsResourceCache
    {
    public:

        ///
        /// ctor
        ///
        GraphicsResourceCache( UInt32 maxtexbytes, UInt32 maxmeshbytes );

        ///
        /// you can create as many as graphics resources as you want. But only limited
        /// number of resources can be realized at any given time. The resource is
        /// useable only between realizeResource() and derealizeResource().
        ///
        /// note that only realizeResource() and derealizeResource() touch rendering device.
        ///
        //@{
        GraphicsResourceId createResource( const GraphicsResoureDesc & );
        void               freeResource( GraphicsResourceId );
        GraphicsResource * getResourceById( GraphicsResourceId );
        bool               realizeResource( GraphicsResourceId, bool * needreloadData );
        void               derealizeResource( GraphicsResourceId );
        //@}
    };

    extern GraphicsResourceCache gGraphicsResourceCache;

    ///
    /// Graphics resource loader
    ///
    struct GraphicsResourceLoader : public NoCopy
    {
        bool load( void * & data, size_t & bytes, int lod ) = 0;  ///< load from external/slow storage (disk, cdrom, network)

        bool decompress( void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int lod ) = 0; ///< decompress or other process to prepare to copy to graphics resource.

        bool populate( GraphicsResource & gfxres, const void * data, size_t bytes, int lod ) = 0;
    };

    ///
    /// ...
    ///
    struct SingleGraphicsResourceRequest
    {
        //@{
        int                      wait_for_draw_fence; ///< resource update must be after this draw fence
        GraphicsResourceId       resourceid;
        int                      lowestlod; ///< ignored for derealize operation.
        int                      targetlod; ///< ignored for derealize operation.
        GraphicsResourceLoader * loader; ///< ignored for derealize operation.
        //@}
    };

    ///
    /// type of operations
    ///
    enum GraphicsResourceOperation
    {
        OP_LOAD,       ///< load from disk
        OP_DECOMPRESS, ///< do decompress or other process to prepare to copy to graphics resource.
        OP_POPULATE,   ///< copy data to graphics resource
        OP_LOCK,       ///< lock the graphics resource, realize those derealized as well, need touch rendering device.
        OP_UNLOCK,     ///< unlock the graphics resource, need touch rendering device.
        OP_DEREALIZE,  ///< derealize the resource.
    };

    struct BatchedGraphicsResourceRequestHeader
    {
        int                       resource_fence;
        GraphicsResourceOperation op;
        UInt32                    count;
    };

    ///
    /// ...
    ///
    void ResourceThread()
    {
        while( !queue.empty() )
        {
            const ResourceRequest & rr = queue.pophead();
            bool reload;
            if( gGraphicsResourceCache.realizeResource( rr.resourceid, &reload ) && reload )
            {
                rr.loader( rr.resourceid, rr.targetlod );
            }
        }
    }

    ///
    /// ...
    ///
    struct DrawRequest
    {
        int                draw_fence;

        //@{
        GraphicsResourceId resources[MAX_RESOURCE_PER_DRAW];
        UInt32             numres;
        //@}
 
        //@{
        gfx::PrimitiveType primtype;
        UInt32             numprim;
        UInt32             startvtx;
        UInt32             minvtxidx;
        UInt32             numvtx;
        UInt32             startidx;
        //@}
    };

    ///
    /// ...
    ///
    class RenderingThread
    {
        void render_loop()
        {
            while( !end_of_game() )
            {
                size_t count;
                const DrawRequest * requests = get_draw_request_buffer( &count ); // this function will block rendering thread, until draw request buffer is ready or game is about to exit.
                for( size_t i = 0; i < count; ++i )
                {
                    // resource request has priority
                    handle_all_resource_requests();

                    const DrawRequest & dr = requests[i];

                    wait_for_all_resoures_ready( dr );

                    do_rendering( dr );

                    update_draw_fence( dr.draw_fence );
                }
            }
        }
    };

    class GameThread
    {
        void game_loop()
        {
        	frame_begin();

            compose_and_submit_resource_request( 1 );
            compose_and_submit_draw_request( 1 );

            compose_and_submit_resource_request( 2 );
            compose_and_submit_draw_request( 2 );

            ...

            compose_and_submit_resource_request( n );
            compose_and_submit_draw_request( n );

        	frame_end();
        }

        void compose_and_submit_resource_request( int draw_fence )
        {
            std::vector<SingleGraphicsResourceRequest*> to_be_loaded;
            std::vector<SingleGraphicsResourceRequest*> to_be_realized;
            std::vector<SingleGraphicsResourceRequest*> to_be_derealized;

            const GraphicsResourceId * resources;
            UInt32 count;
            get_resources_used_by_draw( &resources, &count, draw_fence );
            const GraphicsResourceId * end = resources + count;

            for( ; resources < end; ++resources )
            {
                GraphicsResourceId newres = *resources;

                if( resource_is_derealized( newres ) )
                {
                    while( there_is_no_enough_room_to_hold_resource( newres ) )
                    {
                        GraphicsResourceId oldres = get_the_appropriate_resource_to_be_derealized();

                        mark_as_derealized( oldres );

                        add_to_list( to_be_derealized, oldres );
                    }

                    mark_as_realized( newres );
                }

                add_to_list( to_be_loaded, newres );

                update_resource_draw_fence( newres, draw_fence );
            }

            if( !to_be_derealized.empty() )
            {
                // Note: we could submit this request to rendering thread directly,
                // because derealize is a purly rendering device task. But we need
                // resource thread to serialize the requests, so that "derealize" always
                // happens before "load".
                submit_resource_request_to_resource_thread(
                    OP_DEREALIZE,
                    to_be_derealized );
            }

            if( !to_be_loaded.empty() )
            {
                submit_resource_request_to_resource_thread(
                    OP_LOAD,
                    to_be_loaded );
            }
        }

        void compose_and_submit_draw_request( int draw_fence )
        {
            DrawRequest dr;
            dr.draw_fence = draw_fence;
            ...;
            add_draw_request_to_draw_request_buffer( dr );
        }

        void frame_begin()
        {
            // currently, do nothing
        }

        void frame_end()
        {
            wait_for_rendering_of_last_frame();
            submit_draw_request_buffer_of_this_frame();
            switch_draw_request_buffer_pointer();
        }
    };

    ///
    /// Rendering backend that handles all operations touching rendering device.
    ///
    class RenderEngine
    {
    public:

        /// syncronized command
        //@{

        // reset rendering engine.
        // all resources must be deleted, before calling this function.
        void resetRenderer( const gfx::RendererOptions &, gfx::RendererAPI = gfx::API_AUTO );

        // fence
        UInt32 getCurrentFence();

        // resource management
        void           deleteShader( result, gfx::Shader * );
        void           deleteTexture( gfx::Texture * );
        void           deleteVtxBuf( gfx::VtxBuf * );
        void           deleteIdxBuf( gfx::IdxBuf * );

        // sync operations
        void waitForIdle();          ///< wait until all submitted async command are executed
        void waitForFence( UInt32 ); ///< wait until specific fence is passed.

        //@}

        /// asyncronized commands
        //@{

        // fence
        UInt32 insertFence();

        /* resource instance creation (rely one resource cache implementation)
        void createShader( gfx::Shader * result, gfx::ShaderType type, gfx::ShadingLanguage lang, const StrA & code, const StrA & hints );
        void createTexture( gfx::Shader * result, const gfx::TextureDesc &, const gfx::ImageDesc * initialData );
        void createVtxBuf( gfx::VtxBuf * result, const gfx::VtxBufDesc &, const void * initialData );
        void createIdxBuf( gfx::IdxBuf * result, const gfx::IdxBufDesc &, const void * initialData );

        // resource content update
        void updateTexture( gfx::Texture *, ... );
        void updateVtxBuf( gfx::VtxBuf *, ... );
        void updateIdxBuf( gfx::IdxBuf *, ... );
        */

        // rendering
        void updateContext( const gfx::RendererContext & ctx );
        void draw( gfx::PrimitiveType prim, UInt32 numprim, UInt32 startvtx );
        void drawIndexed( gfx::PrimitiveType prim, UInt32 numprim, UInt32 startvtx, UInt32 startidx );
        //void drawUp( gfx::PrimitiveType prim, UInt32 numprim, ... );
        //void drawIndexedUp(...);
        //void drawLines(...);

        //@}
    };
}}

// *****************************************************************************
//                           End of renderEngine.h
// *****************************************************************************
#endif // __GN_ENGINE_RENDERENGINE_H__
