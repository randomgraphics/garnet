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

class GfxTest
{
    GN::SharedLib mLib;
    GN::gfx::CreateRendererFunc mCreator;

    struct GfxResources
    {
        GN::AutoRef<GN::gfx::Texture> tex1d, tex2d, tex3d, texcube;
    };

    void clear( GN::gfx::Renderer & r, const GN::Vector4f & color )
    {
        bool b = r.drawBegin();
        TS_ASSERT( b );
        if( b )
        {
            r.clearScreen( color );
            r.drawEnd();
        }
    }

    void createResources( GN::gfx::Renderer & r, GfxResources & res )
    {
        // create textures
        res.tex1d.attach( r.createTexture( GN::gfx::TEXTYPE_1D, 128, 128, 128 ) );
        res.tex2d.attach( r.createTexture( GN::gfx::TEXTYPE_2D, 128, 128, 128 ) );
        res.tex3d.attach( r.createTexture( GN::gfx::TEXTYPE_3D, 128, 128, 128 ) );
        res.texcube.attach( r.createTexture( GN::gfx::TEXTYPE_CUBE, 128, 128, 128 ) );
    }

    void clearRed( GN::gfx::Renderer & r )
    {
        clear( r, GN::Vector4f(1,0,0,1) );
    }

    void clearBlue( GN::gfx::Renderer & r )
    {
        clear( r, GN::Vector4f(0,0,1,1) );
    }

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
        TS_ASSERT( win.create( cp ) );
        win.showWindow();

        if( !mCreator) return;
        GN::AutoObjPtr<GN::gfx::Renderer> r;
        GN::gfx::RendererOptions ro;
        ro.software = true;
        r.reset( mCreator(ro) );
        TS_ASSERT( r );
        if( !r ) return;

        GfxResources res;
        createResources( *r, res );
        clearRed(*r);
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
        TS_ASSERT( win.create( cp ) );
        if( !win.getWindow() ) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r;
        GN::gfx::RendererOptions ro;

        ro.useExternalWindow = true;
        ro.renderWindow = win.getWindow();
        ro.software = true;
        r.reset( mCreator(ro) );
        TS_ASSERT( r );
        if( !r ) return;

        GfxResources res;
        createResources( *r, res );
        clearBlue(*r);

        const GN::gfx::DispDesc & dd = r->getDispDesc();

        TS_ASSERT_EQUALS( dd.windowHandle, win.getWindow() );
        //TS_ASSERT_EQUALS( dd.monitorHandle, win.getMonitor() );
        TS_ASSERT_EQUALS( dd.width, cp.clientWidth );
        TS_ASSERT_EQUALS( dd.height, cp.clientHeight );
#endif        
    }

    void changeOptions()
    {
        if( !mCreator) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GfxResources res;

        GN::gfx::RendererOptions ro;

        ro.windowedWidth = 320;
        ro.windowedHeight = 640;
        ro.software = true;
        r.reset( mCreator(ro) );
        TS_ASSERT( r );
        if( !r ) return;
        const GN::gfx::DispDesc & dd = r->getDispDesc();
        TS_ASSERT_EQUALS( r->getOptions().software, true );

        createResources( *r, res );

        clearRed(*r);

        // recreate the device
        ro.software = false;
        TS_ASSERT( r->changeOptions(ro) );
        TS_ASSERT_EQUALS( r->getOptions().software, false );
        TS_ASSERT_EQUALS( dd.width, 320 );
        TS_ASSERT_EQUALS( dd.height, 640 );
        clearBlue(*r);

        // reset the device
        ro.windowedWidth = 256;
        ro.windowedHeight = 128;
        TS_ASSERT( r->changeOptions(ro) );
        TS_ASSERT_EQUALS( dd.width, 256 );
        TS_ASSERT_EQUALS( dd.height, 128 );
    }

    void fullscreen()
    {
        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GN::gfx::RendererOptions ro;

        ro.fullscreen = true;

        ro.displayMode.width = 640;
        ro.displayMode.height = 480;
        r.reset( mCreator(ro) );
        TS_ASSERT(r);
        if( !r ) return;

        GfxResources res;
        createResources( *r, res );
        clearRed(*r);
        //GN::sleep( 500 );
        clearBlue(*r);
        //GN::sleep( 500 );

        ro.displayMode.width = 1024;
        ro.displayMode.height = 768;
        TS_ASSERT( r->changeOptions( ro ) );
        clearRed(*r);
        //GN::sleep( 500 );
        clearBlue(*r);
        //GN::sleep( 500 );
    }

    void defaultBackbufferSize()
    {
        if( !mCreator ) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GN::gfx::RendererOptions ro;
        ro.useExternalWindow = false;
        ro.parentWindow = 0;

        r.reset( mCreator(ro) );
        TS_ASSERT(r);
        if( !r ) return;

        const GN::gfx::DispDesc & dd = r->getDispDesc();
        TS_ASSERT_EQUALS( dd.width, 640 );
        TS_ASSERT_EQUALS( dd.height, 480 );

        clearRed(*r);
    }

    void multiRenderer()
    {
        if( !mCreator ) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r1, r2;
        GfxResources res1, res2;

        GN::gfx::RendererOptions ro;

        ro.windowedWidth = 320;
        ro.windowedHeight = 640;
        r1.reset( mCreator(ro) );
        TS_ASSERT( r1 );

        if( !r1.empty() )
        {
            const GN::gfx::DispDesc & dd = r1->getDispDesc();
            TS_ASSERT_EQUALS( dd.width, 320 );
            TS_ASSERT_EQUALS( dd.height, 640 );
            createResources( *r1, res1 );
            clearRed( *r1 );
        }

        ro.windowedWidth = 512;
        ro.windowedHeight = 256;
        r2.reset( mCreator(ro) );
        TS_ASSERT( r2 );

        if( !r2.empty() )
        {
            const GN::gfx::DispDesc & dd = r2->getDispDesc();
            TS_ASSERT_EQUALS( dd.width, 512 );
            TS_ASSERT_EQUALS( dd.height, 256 );
            createResources( *r2, res2 );
            clearBlue( *r2 );
        }
    }
};

//! \endcond

// *****************************************************************************
//                           End of common.h
// *****************************************************************************
#endif // __GN_TEST_GFX_COMMON_H__
