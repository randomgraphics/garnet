#ifndef __GN_TEST_GFX_COMMON_H__
#define __GN_TEST_GFX_COMMON_H__
// *****************************************************************************
// \file    common.h
// \brief   common header of GFX test case
// \author  chenlee (2005.10.7)
// *****************************************************************************

#include "../testCommon.h"
#include "garnet/GNgfx.h"

#if GN_WINNT
#define EXTNAME ".dll"
#else
#define EXTNAME ".so"
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

    void libInit( const char * api )
    {
        GN::StrA libName = GN::StrA("GNgfx") + api;
        TS_ASSERT( mLib.load( (libName+EXTNAME).cstr() ) );
        TS_ASSERT( mLib.load( libName.cstr() ) );
        mCreator = (GN::gfx::CreateRendererFunc)mLib.getSymbol( "GNgfxCreateRenderer" );
        TS_ASSERT( mCreator );
    }

    void libFree()
    {
        mLib.free();
    }

    void changeDevice()
    {
        if( !mCreator) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GN::gfx::DeviceSettings ds;

        ds.width = 320;
        ds.height = 640;
        ds.software = true;
        r.reset( mCreator(ds) );
        TS_ASSERT( r );
        if( !r ) return;
        const GN::gfx::DispDesc & dd = r->getDispDesc();
        TS_ASSERT_EQUALS( dd.software, true );
        clearRed(*r);

        // recreate the device
        ds.software = false;
        TS_ASSERT( r->changeDevice(ds) );
        TS_ASSERT_EQUALS( dd.software, false );
        TS_ASSERT_EQUALS( dd.width, 320 );
        TS_ASSERT_EQUALS( dd.height, 640 );
        clearBlue(*r);

        // reset the device
        ds.width = 256;
        ds.height = 128;
        TS_ASSERT( r->changeDevice(ds) );
        TS_ASSERT_EQUALS( dd.width, 256 );
        TS_ASSERT_EQUALS( dd.height, 128 );
    }

    void fullscreen()
    {
        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GN::gfx::DeviceSettings ds;

        ds.fullscreen = true;

        ds.width = 640;
        ds.height = 480;
        r.reset( mCreator(ds) );
        TS_ASSERT(r);
        if( !r ) return;
        clearRed(*r);
        GN::sleep( 500 );
        clearBlue(*r);
        GN::sleep( 500 );

        ds.width = 1024;
        ds.height = 768;
        TS_ASSERT( r->changeDevice( ds ) );
        clearRed(*r);
        GN::sleep( 500 );
        clearBlue(*r);
        GN::sleep( 500 );
    }

    void defaultBackbufferSize()
    {
        if( !mCreator ) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r;

        GN::gfx::DeviceSettings ds;
        ds.useExternalWindow = false;
        ds.parentWindow = 0;
        ds.width = ds.height = 0;

        r.reset( mCreator(ds) );
        TS_ASSERT(r);
        if( !r ) return;

        const GN::gfx::DispDesc & dd = r->getDispDesc();
        TS_ASSERT_EQUALS( dd.width, 640 );
        TS_ASSERT_EQUALS( dd.height, 480 );

        clearRed(*r);

        GN::sleep(1000);
    }

    void multiRenderer()
    {
        if( !mCreator ) return;

        GN::AutoObjPtr<GN::gfx::Renderer> r1, r2;

        GN::gfx::DeviceSettings ds;

        ds.width = 320;
        ds.height = 640;
        r1.reset( mCreator(ds) );
        TS_ASSERT( r1 );

        if( !r1.empty() )
        {
            const GN::gfx::DispDesc & dd = r1->getDispDesc();
            TS_ASSERT_EQUALS( dd.width, 320 );
            TS_ASSERT_EQUALS( dd.height, 640 );
            clearRed( *r1 );
        }

        ds.width = 512;
        ds.height = 256;
        r2.reset( mCreator(ds) );
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

// *****************************************************************************
//                           End of common.h
// *****************************************************************************
#endif // __GN_TEST_GFX_COMMON_H__
