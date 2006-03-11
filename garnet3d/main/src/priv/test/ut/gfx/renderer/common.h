#ifndef __GN_TEST_GFX_COMMON_H__
#define __GN_TEST_GFX_COMMON_H__
// *****************************************************************************
// \file    common.h
// \brief   common header of GFX test case
// \author  chenlee (2005.10.7)
// *****************************************************************************

#include "../../testCommon.h"
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
    uint32_t rsb1, rsb2, vtxbinding1, vtxbinding2;

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
        GN::gfx::RenderStateBlockDesc rsbd(GN::gfx::RenderStateBlockDesc::DEFAULT);
        rsbd.rs[GN::gfx::RS_BLENDING] = GN::gfx::RSV_TRUE;
        rsb1 = r.createRenderStateBlock( rsbd );
        rsbd.rs[GN::gfx::RS_ALPHA_TEST] = GN::gfx::RSV_TRUE;
        rsb2 = r.createRenderStateBlock( rsbd );
        TS_ASSERT( rsb1 );
        TS_ASSERT( rsb2 );
        if( 0 == rsb1 && 0 == rsb2 ) return false;

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
        vtxbinding1 = r.createVtxBinding( fmt1 );
        vtxbinding2 = r.createVtxBinding( fmt2 );
        TS_ASSERT( vtxbinding1 );
        TS_ASSERT( vtxbinding2 );
        if( !vtxbinding1 & !vtxbinding2 ) return false;

        // success
        return true;
    }

    void draw()
    {
        GN::gfx::Renderer & r = gRenderer;

        if( r.drawBegin() )
        {
            // draw to rt1
            r.setRenderTarget( 0, rt1 );
            r.clearScreen( GN::Vector4f(1,0,0,1) ); // clear to red

            // draw to rt2
            r.setRenderTarget( 0, rt2 );
            r.clearScreen( GN::Vector4f(0,0,1,1) ); // clear to blue

            // draw to rt3
            r.setRenderTarget( 0, rt3 );
            r.bindTexture( 0, rt1 );
            r.draw2DTexturedQuad( 0, 0, 1, 0.5 );
            r.bindTexture( 0, rt2 );
            r.draw2DTexturedQuad( 0, 0.5, 1, 1 );

            // draw to screen
            r.setRenderTarget( 0, 0 );
            r.clearScreen( GN::Vector4f(0,1,0,1) ); // clear to green
            r.bindTexture( 0, rt3 );
            r.bindRenderStateBlock( rsb1 );
            r.bindRenderStateBlock( rsb2 );

            // draw end
            r.drawEnd();
        }
    }
};


struct TestScene
{
    GN::AutoObjPtr<GfxResources> res;

    ~TestScene() { destroy(); }

    void destroy()
    {
        res.clear();
        GN::gfx::deleteRenderer();
    }

    bool create( const GN::gfx::RendererOptions & ro, GN::gfx::RendererAPI api )
    {
        destroy();

        // create renderer
        GN::gfx::Renderer * r = GN::gfx::createRenderer( ro, api );
        TS_ASSERT( r );
        if( !r ) return false;

        // create resource
        res.attach( new GfxResources );
        if( !res->create() ) return false;

        // success
        return true;
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

    void d3dInit()
    {
        mApi = GN::gfx::API_D3D;
    }

    void oglInit()
    {
        mApi = GN::gfx::API_OGL;
    }

    void externalWindow()
    {
        GN::AutoObjPtr<GN::win::Window> win;
        win.attach( GN::win::createWindow( GN::win::WCP_WINDOWED_RENDER_WINDOW ) );
        TS_ASSERT( !win.empty() );
        if( win.empty() ) return;
        win->resize( 511, 236 );
        win->show();

        TestScene scene;
        GN::gfx::RendererOptions ro;

        ro.useExternalWindow = true;
        ro.displayHandle = win->getDisplayHandle();
        ro.renderWindow = win->getWindowHandle();
        ro.software = true;
        if( !scene.create(ro,mApi) ) return;

        scene.draw();

        const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();

        TS_ASSERT_EQUALS( dd.windowHandle, win->getWindowHandle() );
        //TS_ASSERT_EQUALS( dd.monitorHandle, win->getDisplayHandle() );
        GN::Vector2<size_t> winSize = win->getClientSize();
        TS_ASSERT_EQUALS( dd.width, winSize.x );
        TS_ASSERT_EQUALS( dd.height, winSize.y );
    }

    void changeOptions()
    {
        TestScene scene;
        GN::gfx::RendererOptions ro;

        ro.windowedWidth = 320;
        ro.windowedHeight = 640;
        ro.software = true;
        if( !scene.create(ro,mApi) ) return;

        const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
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
        TestScene scene;
        GN::gfx::RendererOptions ro;

        ro.fullscreen = true;
        ro.displayMode.width = 640;
        ro.displayMode.height = 480;
        if( !scene.create(ro,mApi) ) return;

        scene.draw();

        ro.displayMode.width = 1024;
        ro.displayMode.height = 768;
        TS_ASSERT( gRenderer.changeOptions( ro ) );

        scene.draw();
    }

    void defaultBackbufferSize()
    {
        TestScene scene;

        GN::gfx::RendererOptions ro;
        ro.useExternalWindow = false;
        ro.parentWindow = 0;
        if( !scene.create(ro,mApi) ) return;

        const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
        TS_ASSERT_EQUALS( dd.width, 640 );
        TS_ASSERT_EQUALS( dd.height, 480 );

        scene.draw();
    }

    void renderStateBlock()
    {
        GN::gfx::RendererOptions ro;
        GN::gfx::Renderer * r = GN::gfx::createRenderer( ro, mApi );
        TS_ASSERT( r );
        if( 0 == r ) return;

        // renderer should be initialized with default render state
        TS_ASSERT( r->getCurrentRenderStateBlock() == GN::gfx::RenderStateBlockDesc::DEFAULT );

        GN::gfx::RenderStateBlockDesc
            rsbd1( GN::gfx::RenderStateBlockDesc::RESET_TO_DEFAULT ),
            rsbd2( GN::gfx::RenderStateBlockDesc::RESET_TO_INVALID );

        uint32_t rsb1 = r->createRenderStateBlock(rsbd1);
        uint32_t rsb2 = r->createRenderStateBlock(rsbd2);
        TS_ASSERT( rsb1 );
        TS_ASSERT( rsb2 );

        // different rsb should have different handler
        TS_ASSERT_DIFFERS( rsb1, rsb2 );

        // query for same rsb should return same handler
        TS_ASSERT_EQUALS( rsb1, r->createRenderStateBlock(rsbd1) );
        TS_ASSERT_EQUALS( rsb2, r->createRenderStateBlock(rsbd2) );

        // try setRenderState
        uint32_t rsb3 = r->setRenderState( GN::gfx::RS_BLENDING, GN::gfx::RSV_TRUE );
        uint32_t rsb4 = r->setRenderState( GN::gfx::RS_BLENDING, GN::gfx::RSV_FALSE );
        TS_ASSERT( rsb3 );
        TS_ASSERT( rsb4 );
        TS_ASSERT_DIFFERS( rsb3, rsb4 );
        TS_ASSERT_EQUALS( rsb4, rsb1 );
        TS_ASSERT( r->getCurrentRenderStateBlock() == GN::gfx::RenderStateBlockDesc::DEFAULT );

        // try setRenderState with invalid value
        TS_ASSERT_EQUALS( 0, r->setRenderState( GN::gfx::RS_INVALID, GN::gfx::RSV_TRUE ) );
        TS_ASSERT_EQUALS( 0, r->setRenderState( GN::gfx::RS_BLENDING, GN::gfx::RSV_INVALID ) );
        TS_ASSERT_EQUALS( 0, r->setRenderState( GN::gfx::RS_INVALID, GN::gfx::RSV_INVALID ) );
    }

    void vtxBuf()
    {
        GN::gfx::RendererOptions ro;
        GN::gfx::Renderer * r = GN::gfx::createRenderer( ro, mApi );
        TS_ASSERT( r );
        if( 0 == r ) return;

        GN_WARN( "TODO: vertex buffer UT!" );
    }

    void renderTarget()
    {
        GN::gfx::RendererOptions ro;
        GN::gfx::Renderer * r = GN::gfx::createRenderer( ro, mApi );
        TS_ASSERT( r );
        if( 0 == r ) return;

        GN::AutoRef<GN::gfx::Texture> rt1, rt2, rt3, rt4;

        // create render targets
        rt1.attach( r->create1DTexture( 256, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
        rt2.attach( r->create2DTexture( 256, 256, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
        rt3.attach( r->create3DTexture( 128, 128, 4, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
        rt4.attach( r->createCubeTexture( 128, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
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
