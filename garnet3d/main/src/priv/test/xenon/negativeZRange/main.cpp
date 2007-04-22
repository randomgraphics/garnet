#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;
using namespace GN::util;

class MyApp : public app::SampleApp
{
    Scene   mScene;
    Actor   mActor;
    ArcBall mArcBall;

public:
	MyApp() {}

	bool onRendererCreate()
	{
        // initialize actor
        loadFromXmlFile( mActor, "media::/cube/cube.actor.xml" );

        // update camera stuff
        mRadius = mActor.getBoundingSphere().radius * 2.0f;
        updateRadius();

        // initialize mArcBall
        mArcBall.setHandness( util::RIGHT_HAND );
        mArcBall.setViewMatrix( mView );
        mArcBall.connectToInput();

		return true;
	}

	void onRendererDestroy()
	{
	}

	void onUpdate()
	{
	}

	void onKeyPress( input::KeyEvent ke )
	{
		app::SampleApp::onKeyPress( ke );
	}

	void onRenderer()
	{
	}
};

int main()
{
	MyApp app;
	return app.run( 0, 0 );
}
