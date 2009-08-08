#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include "garnet/GNwin.h"

class LineRendererTest : public CxxTest::TestSuite
{
    class LineRendererInitiator
    {
    public:

        GN::AutoObjPtr<GN::win::Window> mWindow;
        GN::gfx::Gpu                  * mGpu;
        GN::gfx::LineRenderer         * mLine;

        LineRendererInitiator() : mGpu(NULL), mLine(NULL)
        {
            GN::gfx::GpuOptions ro;
            mGpu = GN::gfx::createSingleThreadGpu( ro );
            if( NULL == mGpu ) return;

            mLine = new GN::gfx::LineRenderer( *mGpu );
            if( !mLine->init() ) delete mLine, mLine = NULL;
        }

        ~LineRendererInitiator()
        {
            if( mLine ) delete mLine;
            if( mGpu ) GN::gfx::deleteGpu( mGpu );
        }

        operator bool() const { return NULL != mLine; }
    };

public:

    void testManyLines()
    {
        LineRendererInitiator l;
        if( !l ) return;

        GN::DynaArray<GN::Vector3f> positions( 513 );

        l.mLine->drawLines( positions.cptr(), 0, 513, 0, GN::Matrix44f::sIdentity() );
    }
};
