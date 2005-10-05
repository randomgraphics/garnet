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

    void multiRenderer()
    {
        GN::AutoObjPtr<GN::gfx::Renderer> r1, r2;

        GN::gfx::DeviceSettings ds;

        ds.width = 320;
        ds.height = 240;
        r1.reset( mCreator(ds) );
        TS_ASSERT( r1 );

        if( !r1.empty() )
        {
            const GN::gfx::DispDesc & dd = r1->getDispDesc();
            TS_ASSERT_EQUALS( dd.width, 320 );
            TS_ASSERT_EQUALS( dd.height, 240 );
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
        }
    }
};

#if GN_WINNT

class GfxD3DTest : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { libInit("D3D"); }

    void tearDown() { libFree(); }

    void testMultiRenderer() { multiRenderer(); }
};

#endif

class GfxOGLTest : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { libInit("OGL"); }

    void tearDown() { libFree(); }

    //void testMultiRenderer() { multiRenderer(); }
};

