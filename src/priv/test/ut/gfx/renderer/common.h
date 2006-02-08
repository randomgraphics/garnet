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

    bool create( GN::gfx::Renderer & r )
    {
        // create textures
        tex1d.attach( r.createTexture( GN::gfx::TEXTYPE_1D, 128, 128, 128 ) );
        tex2d.attach( r.createTexture( GN::gfx::TEXTYPE_2D, 128, 128, 128 ) );
        tex3d.attach( r.createTexture( GN::gfx::TEXTYPE_3D, 128, 128, 128 ) );
        texcube.attach( r.createTexture( GN::gfx::TEXTYPE_CUBE, 128, 128, 128 ) );

        // create render targets
        rt1.attach( r.createTexture( GN::gfx::TEXTYPE_2D, 64, 64, 0, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
        rt2.attach( r.createTexture( GN::gfx::TEXTYPE_2D, 64, 64, 0, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
        rt3.attach( r.createTexture( GN::gfx::TEXTYPE_2D, 128, 64, 0, 0, GN::gfx::FMT_DEFAULT, GN::gfx::TEXUSAGE_RENDER_TARGET ) );
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

    void draw( GN::gfx::Renderer & r )
    {
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
            r.drawQuad( 0, 0, 1, 0.5 );
            r.bindTexture( 0, rt2 );
            r.drawQuad( 0, 0.5, 1, 1 );

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
    GN::AutoObjPtr<GN::gfx::Renderer> r;
    GN::AutoObjPtr<GfxResources> res;

    void destroy()
    {
        res.clear();
        r.clear();
    }

    bool create( const GN::gfx::CreateRendererFunc & fn, const GN::gfx::RendererOptions & ro )
    {
        destroy();

        // create renderer
        r.attach( fn(ro) );
        TS_ASSERT( r );
        if( r.empty() ) return false;

        // create resource
        res.attach( new GfxResources );
        if( !res->create(*r) ) return false;

        // success
        return true;
    }

    void draw()
    {
        TS_ASSERT( r && res );
        if( r && res ) res->draw(*r);
    }
};

class GfxTest
{
    GN::SharedLib mLib;
    GN::gfx::CreateRendererFunc mCreator;

protected:

    void d3dInit()
    {
#if GN_STATIC
        mCreator = &GN::gfx::createD3DRenderer;
#else
        GN::StrA libName = GN::StrA("GNgfxD3D");
        TS_ASSERT( mLib.load( (LIB_PREF+libName+LIB_SUFF).cstr() ) );
        TS_ASSERT( mLib.load( libName.cstr() ) );
        mCreator = (GN::gfx::CreateRendererFunc)mLib.getSymbol( "GNgfxCreateRenderer" );
        TS_ASSERT( mCreator );
#endif
    }

    void oglInit()
    {
#if GN_STATIC
        mCreator = &GN::gfx::createOGLRenderer;
#else
        GN::StrA libName = GN::StrA("GNgfxOGL");
        TS_ASSERT( mLib.load( (LIB_PREF+libName+LIB_SUFF).cstr() ) );
        TS_ASSERT( mLib.load( libName.cstr() ) );
        mCreator = (GN::gfx::CreateRendererFunc)mLib.getSymbol( "GNgfxCreateRenderer" );
        TS_ASSERT( mCreator );
#endif
    }

    void libFree()
    {
        mLib.free();
    }

    //
    // Make sure ntWindow and NTRenderWindow can work with each other.
    //
    void ntWindowAndNTRenderWindow()
    {
#if GN_MSWIN
        GN::win::MswWindow win;
        GN::win::MswWindow::CreateParam cp;
        cp.clientWidth = 236;
        cp.clientHeight = 189;
        TS_ASSERT( win.create(cp) );
        win.showWindow();

        TestScene scene;
        GN::gfx::RendererOptions ro;
        ro.software = true;
        if( !scene.create(mCreator,ro) ) return;

        scene.draw();
#endif
    }

    void externalWindow()
    {
        if( !mCreator) return;

#if GN_MSWIN
        GN::win::MswWindow win;
        GN::win::MswWindow::CreateParam cp;
        cp.clientWidth = 236;
        cp.clientHeight = 189;
        TS_ASSERT( win.create(cp) );
        if( !win.getWindow() ) return;

        TestScene scene;
        GN::gfx::RendererOptions ro;

        ro.useExternalWindow = true;
        ro.renderWindow = win.getWindow();
        ro.software = true;
        if( !scene.create(mCreator,ro) ) return;

        scene.draw();

        const GN::gfx::DispDesc & dd = scene.r->getDispDesc();

        TS_ASSERT_EQUALS( dd.windowHandle, win.getWindow() );
        //TS_ASSERT_EQUALS( dd.monitorHandle, win.getMonitor() );
        TS_ASSERT_EQUALS( dd.width, cp.clientWidth );
        TS_ASSERT_EQUALS( dd.height, cp.clientHeight );
#endif        
    }

    void changeOptions()
    {
        TestScene scene;
        GN::gfx::RendererOptions ro;

        ro.windowedWidth = 320;
        ro.windowedHeight = 640;
        ro.software = true;
        if( !scene.create(mCreator,ro) ) return;

        const GN::gfx::DispDesc & dd = scene.r->getDispDesc();
        TS_ASSERT_EQUALS( scene.r->getOptions().software, true );

        scene.draw();

        // recreate the device
        ro.software = false;
        TS_ASSERT( scene.r->changeOptions(ro) );
        TS_ASSERT_EQUALS( scene.r->getOptions().software, false );
        TS_ASSERT_EQUALS( dd.width, 320 );
        TS_ASSERT_EQUALS( dd.height, 640 );
        scene.draw();

        // reset the device
        ro.windowedWidth = 256;
        ro.windowedHeight = 128;
        TS_ASSERT( scene.r->changeOptions(ro) );
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
        if( !scene.create(mCreator,ro) ) return;

        scene.draw();

        ro.displayMode.width = 1024;
        ro.displayMode.height = 768;
        TS_ASSERT( scene.r->changeOptions( ro ) );

        scene.draw();
    }

    void defaultBackbufferSize()
    {
        TestScene scene;

        GN::gfx::RendererOptions ro;
        ro.useExternalWindow = false;
        ro.parentWindow = 0;
        if( !scene.create(mCreator,ro) ) return;

        const GN::gfx::DispDesc & dd = scene.r->getDispDesc();
        TS_ASSERT_EQUALS( dd.width, 640 );
        TS_ASSERT_EQUALS( dd.height, 480 );

        scene.draw();
    }

    void multiRenderer()
    {
        if( !mCreator ) return;

        TestScene s1, s2;

        GN::gfx::RendererOptions ro;

        ro.windowedWidth = 320;
        ro.windowedHeight = 640;
        if( s1.create(mCreator,ro) )
        {
            const GN::gfx::DispDesc & dd = s1.r->getDispDesc();
            TS_ASSERT_EQUALS( dd.width, 320 );
            TS_ASSERT_EQUALS( dd.height, 640 );
            s1.draw();
        }

        ro.windowedWidth = 512;
        ro.windowedHeight = 256;
        if( s2.create(mCreator,ro) )
        {
            const GN::gfx::DispDesc & dd = s2.r->getDispDesc();
            TS_ASSERT_EQUALS( dd.width, 512 );
            TS_ASSERT_EQUALS( dd.height, 256 );
            s2.draw();
        }
    }

    void renderStateBlock()
    {
        GN::AutoObjPtr<GN::gfx::Renderer> r;
        GN::gfx::RendererOptions ro;
        r.attach( mCreator(ro) );
        TS_ASSERT( r );
        if( r.empty() ) return;

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
        GN::AutoObjPtr<GN::gfx::Renderer> r;
        GN::gfx::RendererOptions ro;
        r.attach( mCreator(ro) );
        TS_ASSERT( r );
        if( r.empty() ) return;

        GN_WARN( "TODO: vertex buffer UT!" );
    }

    void renderTarget()
    {
        GN::AutoObjPtr<GN::gfx::Renderer> r;
        GN::gfx::RendererOptions ro;
        r.attach( mCreator(ro) );
        TS_ASSERT( r );
        if( r.empty() ) return;

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
