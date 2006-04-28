#ifndef __GN_TEST_GFX_COMMON_H__
#define __GN_TEST_GFX_COMMON_H__
// *****************************************************************************
// \file    common.h
// \brief   common header of GFX test case
// \author  chenlee (2005.10.7)
// *****************************************************************************

#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include "garnet/GNwin.h"

//! \cond

#if GN_MSWIN || GN_CYGWIN
#define LIB_PREF ""
#define LIB_SUFF ".dll"
#else
#define LIB_PREF "lib"
#define LIB_SUFF ".so"
#endif

struct GfxResources
{
    GN::AutoRef<GN::gfx::Texture> tex1d, tex2d, tex3d, texcube, rt1, rt2, rt3;
    GN::AutoRef<GN::gfx::VtxBuf> vb1, vb2;
    GN::AutoRef<GN::gfx::IdxBuf> ib1, ib2;
    GN::gfx::RenderStateBlockDesc rsb1, rsb2;
    uint32_t vtxFmt1, vtxFmt2;

    bool create()
    {
        GN::gfx::Renderer & r = gRenderer;

        // create textures
        tex1d.attach( r.createTexture( GN::gfx::TEXTYPE_1D, 128, 128, 128, 128 ) );
        tex2d.attach( r.createTexture( GN::gfx::TEXTYPE_2D, 128, 128, 128, 128 ) );
        tex3d.attach( r.createTexture( GN::gfx::TEXTYPE_3D, 128, 128, 128, 128 ) );
        texcube.attach( r.createTexture( GN::gfx::TEXTYPE_CUBE, 128, 128, 128, 128 ) );

        // create render targets
        rt1.attach( r.create2DTexture(  64, 64, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
        rt2.attach( r.create2DTexture(  64, 64, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
        rt3.attach( r.create2DTexture( 128, 64, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
        TS_ASSERT( rt1 );
        TS_ASSERT( rt2 );
        TS_ASSERT( rt3 );

        // create render state blocks
        rsb1.resetToDefault();
        rsb1.set( GN::gfx::RS_BLENDING, GN::gfx::RSV_TRUE );
        rsb2.resetToDefault();
        rsb2.set( GN::gfx::RS_ALPHA_TEST, GN::gfx::RSV_TRUE );

        // create vertex buffers
        vb1.attach( r.createVtxBuf( 100 ) );
        vb2.attach( r.createVtxBuf( 100 ) );
        TS_ASSERT( vb1 );
        TS_ASSERT( vb2 );
        if( !vb1 & !vb2 ) return false;

        // create index buffers
        ib1.attach( r.createIdxBuf( 100 ) );
        ib2.attach( r.createIdxBuf( 100 ) );
        TS_ASSERT( ib1 );
        TS_ASSERT( ib2 );
        if( !ib1 & !ib2 ) return false;

        // create vertex bindings
        GN::gfx::VtxFmtDesc fmt1, fmt2;
        fmt1.addAttrib( 0, 0, GN::gfx::VTXSEM_COORD, GN::gfx::FMT_FLOAT4 );
        fmt2.addAttrib( 0, 0, GN::gfx::VTXSEM_COORD, GN::gfx::FMT_FLOAT2 );
        vtxFmt1 = r.createVtxFmt( fmt1 );
        vtxFmt2 = r.createVtxFmt( fmt2 );
        TS_ASSERT( vtxFmt1 );
        TS_ASSERT( vtxFmt2 );
        if( !vtxFmt1 & !vtxFmt2 ) return false;

        // success
        return true;
    }

    void draw()
    {
        GN::gfx::Renderer & r = gRenderer;

        if( r.drawBegin() )
        {
            // draw to rt1
            r.setColorBuffer( 0, rt1 );
            r.clearScreen( GN::Vector4f(1,0,0,1) ); // clear to red

            // draw to rt2
            r.setColorBuffer( 0, rt2 );
            r.clearScreen( GN::Vector4f(0,0,1,1) ); // clear to blue

            // draw to rt3
            r.setColorBuffer( 0, rt3 );
            r.setTexture( 0, rt1 );
            r.draw2DTexturedQuad( 0, 0, 1, 0.5 );
            r.setTexture( 0, rt2 );
            r.draw2DTexturedQuad( 0, 0.5, 1, 1 );

            // draw to screen
            r.setColorBuffer( 0, 0 );
            r.clearScreen( GN::Vector4f(0,1,0,1) ); // clear to green
            r.setTexture( 0, rt3 );
            r.setRenderStateBlock( rsb1 );
            r.setRenderStateBlock( rsb2 );

            // draw end
            r.drawEnd();
        }
    }
};


struct TestScene : public GN::SlotBase
{
    GN::AutoObjPtr<GfxResources> res;

    TestScene()
    {
        using namespace GN;
        using namespace GN::gfx;
        Renderer::sSigCreate.connect( this, &TestScene::create );
        Renderer::sSigDestroy.connect( this, &TestScene::destroy );
    }

    ~TestScene() { destroy(); }

    bool create()
    {
        destroy();

        // create resource
        res.attach( new GfxResources );
        if( !res->create() ) return false;

        // success
        return true;
    }

    void destroy()
    {
        res.clear();
    }

    void draw()
    {
        TS_ASSERT( gRendererPtr && res );
        if( gRendererPtr && res ) res->draw();
    }
};

class GfxTest
{
    GN::gfx::RendererAPI mApi;

protected:

#define CREATE_RENDERER( r, ro ) \
        r = createRenderer( mApi ); \
        TS_ASSERT( r ); if( 0 == r ) return; \
        if( !r->changeOptions( ro ) ) { TS_ASSERT(0); return; }

    void d3d9Init()
    {
        mApi = GN::gfx::API_D3D9;
    }

    void oglInit()
    {
        mApi = GN::gfx::API_OGL;
    }

    void externalWindow()
    {
        using namespace GN;
        using namespace GN::gfx;

        GN::AutoObjPtr<win::Window> win;
        win.attach( win::createWindow( win::WCP_WINDOWED_RENDER_WINDOW ) );
        TS_ASSERT( !win.empty() );
        if( win.empty() ) return;
        win->resize( 511, 236 );
        win->show();

        TestScene scene;
        Renderer * r;
        RendererOptions ro;

        ro.useExternalWindow = true;
        ro.displayHandle = win->getDisplayHandle();
        ro.renderWindow = win->getWindowHandle();
        ro.software = true;
        CREATE_RENDERER( r, ro );

        scene.draw();

        const DispDesc & dd = gRenderer.getDispDesc();

        TS_ASSERT_EQUALS( dd.windowHandle, win->getWindowHandle() );
        //TS_ASSERT_EQUALS( dd.monitorHandle, win->getDisplayHandle() );
        Vector2<size_t> winSize = win->getClientSize();
        TS_ASSERT_EQUALS( dd.width, winSize.x );
        TS_ASSERT_EQUALS( dd.height, winSize.y );

		// delete renderer, to make sure renderer is deleted before window.
		deleteRenderer();
    }

    void changeOptions()
    {
        using namespace GN;
        using namespace GN::gfx;

        TestScene scene;
        Renderer * r;
        RendererOptions ro;

        ro.windowedWidth = 320;
        ro.windowedHeight = 640;
        ro.software = true;
        CREATE_RENDERER( r, ro );

        const DispDesc & dd = gRenderer.getDispDesc();
        TS_ASSERT_EQUALS( gRenderer.getOptions().software, true );

        scene.draw();

        // recreate the device
        ro.software = false;
        TS_ASSERT( gRenderer.changeOptions(ro) );
        TS_ASSERT_EQUALS( gRenderer.getOptions().software, false );
        TS_ASSERT_EQUALS( dd.width, 320 );
        TS_ASSERT_EQUALS( dd.height, 640 );
        scene.draw();

        // reset the device
        ro.windowedWidth = 256;
        ro.windowedHeight = 128;
        TS_ASSERT( gRenderer.changeOptions(ro) );
        TS_ASSERT_EQUALS( dd.width, 256 );
        TS_ASSERT_EQUALS( dd.height, 128 );
        scene.draw();
    }

    void fullscreen()
    {
        using namespace GN;
        using namespace GN::gfx;

        TestScene scene;

        Renderer * r;
        RendererOptions ro;
        ro.fullscreen = true;
        ro.displayMode.width = 640;
        ro.displayMode.height = 480;
        CREATE_RENDERER( r, ro );

        scene.draw();

        ro.displayMode.width = 1024;
        ro.displayMode.height = 768;
        TS_ASSERT( gRenderer.changeOptions( ro ) );

        scene.draw();
    }

    void defaultBackbufferSize()
    {
        using namespace GN;
        using namespace GN::gfx;

        TestScene scene;

        Renderer * r;
        RendererOptions ro;
        ro.useExternalWindow = false;
        ro.parentWindow = 0;
        CREATE_RENDERER( r, ro );

        const DispDesc & dd = gRenderer.getDispDesc();
        TS_ASSERT_EQUALS( dd.width, 640 );
        TS_ASSERT_EQUALS( dd.height, 480 );

        scene.draw();
    }

    void renderStateBlock()
    {
        using namespace GN;
        using namespace GN::gfx;

        Renderer * r;
        RendererOptions ro;
        CREATE_RENDERER( r, ro );

        // renderer should be initialized with default render state
        TS_ASSERT_EQUALS( r->getCurrentRenderStateBlock(), RenderStateBlockDesc::DEFAULT );

        // try setRenderState
        r->setRenderState( RS_BLENDING, RSV_TRUE );
        TS_ASSERT_EQUALS( r->getCurrentRenderStateBlock().get(RS_BLENDING), RSV_TRUE );

        r->setRenderState( RS_BLENDING, RSV_FALSE );
        TS_ASSERT_EQUALS( r->getCurrentRenderStateBlock().get(RS_BLENDING), RSV_FALSE );

        TS_ASSERT_EQUALS( r->getCurrentRenderStateBlock(), RenderStateBlockDesc::DEFAULT );

        // try setRenderState with invalid value
        //TS_ASSERT_EQUALS( 0, r->setRenderState( RS_INVALID, RSV_TRUE ) );
        //TS_ASSERT_EQUALS( 0, r->setRenderState( RS_BLENDING, RSV_INVALID ) );
        //TS_ASSERT_EQUALS( 0, r->setRenderState( RS_INVALID, RSV_INVALID ) );
    }

    void vtxBuf()
    {
        using namespace GN;
        using namespace GN::gfx;

        Renderer * r;
        RendererOptions ro;
        CREATE_RENDERER( r, ro );

        GN_WARN( "TODO: vertex buffer UT!" );
    }

    void renderTarget()
    {
        using namespace GN;
        using namespace GN::gfx;

        Renderer * r;
        RendererOptions ro;
        CREATE_RENDERER( r, ro );

        GN::AutoRef<Texture> rt1, rt2, rt3, rt4;

        // create render targets
        rt1.attach( r->create1DTexture( 256, 0, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        rt2.attach( r->create2DTexture( 256, 256, 0, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        rt3.attach( r->create3DTexture( 128, 128, 4, 0, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        rt4.attach( r->createCubeTexture( 128, 0, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        TS_ASSERT( rt1 );
        TS_ASSERT( rt2 );
        TS_ASSERT( !rt3 );
        TS_ASSERT( rt4 );
    }
};

//! \endcond

// *****************************************************************************
//                           End of common.h
// *****************************************************************************
#endif // __GN_TEST_GFX_COMMON_H__
