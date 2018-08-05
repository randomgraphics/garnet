#include "../testCommon.h"
#include "garnet/GNgfx.h"

class LineRendererTest : public CxxTest::TestSuite
{
    class LineRendererInitiator
    {
    public:

        GN::gfx::Gpu                  * mGpu;
        GN::gfx::LineRenderer         * mLine;

        LineRendererInitiator() : mGpu(NULL), mLine(NULL)
        {
            GN::gfx::GpuOptions ro;
            mGpu = GN::gfx::createGpu( ro, 0 );
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

        l.mLine->drawLines( positions.rawptr(), 0, 513, 0, GN::Matrix44f::sIdentity() );
    }
};
