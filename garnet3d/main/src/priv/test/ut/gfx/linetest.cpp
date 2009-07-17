#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include "garnet/GNwin.h"

class LineRendererTest : public CxxTest::TestSuite
{
    class LineRendererInitiator
    {
    public:

        GN::AutoObjPtr<GN::win::Window> mWindow;
        GN::gfx::Renderer             * mRndr;
        GN::gfx::LineRenderer         * mLineRndr;

        LineRendererInitiator() : mRndr(NULL), mLineRndr(NULL)
        {
            GN::gfx::RendererOptions ro;
            mRndr = GN::gfx::createSingleThreadRenderer( ro );
            if( NULL == mRndr ) return;

            mLineRndr = new GN::gfx::LineRenderer( *mRndr );
            if( !mLineRndr->init() ) delete mLineRndr, mLineRndr = NULL;
        }

        ~LineRendererInitiator()
        {
            if( mLineRndr ) delete mLineRndr;
            if( mRndr ) GN::gfx::deleteRenderer( mRndr );
        }

        operator bool() const { return NULL != mLineRndr; }
    };

public:

    void testManyLines()
    {
        LineRendererInitiator l;
        if( !l ) return;

        GN::DynaArray<GN::Vector3f> positions( 513 );

        l.mLineRndr->drawLines( positions.cptr(), 0, 513, 0, GN::Matrix44f::sIdentity() );
    }
};
