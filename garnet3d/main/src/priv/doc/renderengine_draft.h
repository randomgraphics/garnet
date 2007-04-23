#ifndef __GN_SCENE_RENDERENGINE_H__
#define __GN_SCENE_RENDERENGINE_H__
// *****************************************************************************
//! \file    scene/renderEngine.h
//! \brief   interface of render engine
//! \author  chenli@@FAREAST (2007.4.13)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// Resource cache
    ///
    class ResourceCache
    {
    public:

        void allocResource(...);
    };

    struct ResourceRequest
    {
        int resourceid;
        int lowestlod;
        int targetlod;
    };

    struct DrawRequest
    {
        ...;
    };

    void GameThread()
    {
        void setup_resource_request( ResourceRequest & );
        int submit_resourece_request( const ResourceRequest & ); // return resource fence
        void setup_draw_request( DrawRequest & );

        int submit_draw_request( const DrawRequest &, int resource_fence ) // return draw fence
        {
            // add draw request and resource fence to draw request list.
        }

        void switch_frame_buffer() // call at the end of a frame
        {
            // wait for end of rendering of last frame
            // submit draw request list of this frame.
            // switch draw request list pointer.
        }
    }

    void ResourceThread()
    {
    }

    void RenderingThread()
    {
    }

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
        void reset( const gfx::RendererOptions &, gfx::RendererAPI = gfx::API_AUTO );

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
#endif // __GN_SCENE_RENDERENGINE_H__
