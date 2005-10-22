#ifndef __GN_TEST_GFX_COMMON_H__
#define __GN_TEST_GFX_COMMON_H__
// *****************************************************************************
// \file    common.h
// \brief   common header of GFX test case
// \author  chenlee (2005.10.7)
// *****************************************************************************

#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include "garnet/base/ntWindow.h"

//! \cond

#if GN_WINNT || GN_CYGWIN
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
#if GN_WINNT
        GN::NTWindow win;
        GN::NTWindow::CreateParam cp;
        cp.clientWidth = 236;
        cp.clientHeight = 189;
        TS_ASSERT( win.create( cp ) );
        win.showWindow();

        if( !mCreator) return;
        GN::AutoObjPtr<GN::gfx::Renderer> r;
        GN::gfx::UserOptions uo;
        uo.software = true;
        r.reset( mCreator(uo) );
        TS_ASSERT( r );
        if( !r ) return;
        clearRed(*r);
#endif
    }

    void externalWindow()
    {
        if( !mCreator) return;

#if GN_WINNT
        GN::NTWindow win;
        GN::NTWindow::CreateParam cp;
        cp.clientWidth = 236;
        cp.clientHeight = 189;
        TS_ASSERT( win.create( cp ) );
        if( !win.getWindow() ) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r;
        GN::gfx::UserOptions uo;

        uo.useExternalWindow = true;
        uo.renderWindow = win.getWindow();
        uo.software = true;
        r.reset( mCreator(uo) );

        clearBlue(*r);

        const GN::gfx::DispDesc & dd = r->getDispDesc();

        TS_ASSERT_EQUALS( dd.windowHandle, win.getWindow() );
        //TS_ASSERT_EQUALS( dd.monitorHandle, win.getMonitor() );
        TS_ASSERT_EQUALS( dd.width, cp.clientWidth );
        TS_ASSERT_EQUALS( dd.height, cp.clientHeight );
#endif        
    }

    void changeDevice()
    {
        if( !mCreator) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GN::gfx::UserOptions uo;

        uo.windowedWidth = 320;
        uo.windowedHeight = 640;
        uo.software = true;
        r.reset( mCreator(uo) );
        TS_ASSERT( r );
        if( !r ) return;
        const GN::gfx::DispDesc & dd = r->getDispDesc();
        TS_ASSERT_EQUALS( r->getUserOptions().software, true );
        clearRed(*r);

        // recreate the device
        uo.software = false;
        TS_ASSERT( r->changeDevice(uo) );
        TS_ASSERT_EQUALS( r->getUserOptions().software, false );
        TS_ASSERT_EQUALS( dd.width, 320 );
        TS_ASSERT_EQUALS( dd.height, 640 );
        clearBlue(*r);

        // reset the device
        uo.windowedWidth = 256;
        uo.windowedHeight = 128;
        TS_ASSERT( r->changeDevice(uo) );
        TS_ASSERT_EQUALS( dd.width, 256 );
        TS_ASSERT_EQUALS( dd.height, 128 );
    }

    void fullscreen()
    {
        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GN::gfx::UserOptions uo;

        uo.fullscreen = true;

        uo.displayMode.width = 640;
        uo.displayMode.height = 480;
        r.reset( mCreator(uo) );
        TS_ASSERT(r);
        if( !r ) return;
        clearRed(*r);
        //GN::sleep( 500 );
        clearBlue(*r);
        //GN::sleep( 500 );

        uo.displayMode.width = 1024;
        uo.displayMode.height = 768;
        TS_ASSERT( r->changeDevice( uo ) );
        clearRed(*r);
        //GN::sleep( 500 );
        clearBlue(*r);
        //GN::sleep( 500 );
    }

    void defaultBackbufferSize()
    {
        if( !mCreator ) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GN::gfx::UserOptions uo;
        uo.useExternalWindow = false;
        uo.parentWindow = 0;

        r.reset( mCreator(uo) );
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

        GN::gfx::UserOptions uo;

        uo.windowedWidth = 320;
        uo.windowedHeight = 640;
        r1.reset( mCreator(uo) );
        TS_ASSERT( r1 );

        if( !r1.empty() )
        {
            const GN::gfx::DispDesc & dd = r1->getDispDesc();
            TS_ASSERT_EQUALS( dd.width, 320 );
            TS_ASSERT_EQUALS( dd.height, 640 );
            clearRed( *r1 );
        }

        uo.windowedWidth = 512;
        uo.windowedHeight = 256;
        r2.reset( mCreator(uo) );
        TS_ASSERT( r2 );

        if( !r2.empty() )
        {
            const GN::gfx::DispDesc & dd = r2->getDispDesc();
            TS_ASSERT_EQUALS( dd.width, 512 );
            TS_ASSERT_EQUALS( dd.height, 256 );
            clearBlue( *r2 );
        }
    }
};

//! \endcond

// *****************************************************************************
//                           End of common.h
// *****************************************************************************
#endif // __GN_TEST_GFX_COMMON_H__
