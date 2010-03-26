#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::util;

class MyApp : public SampleApp
{
    Scene   mScene;
    Actor   mActor;
    ArcBall mArcBall;
    float   mRadius;

public:
	MyApp() : mScene( getEntityManager(), getRenderEngine() ), mActor(mScene)
    {
    }

	bool onInit()
	{
        // initialize actor
        LoadFromXmlFile( mActor, "media::/cube/cube.actor.xml" );

        // update camera stuff
        mRadius = mActor.getBoundingSphere().radius * 2.0f;

        // initialize mArcBall
        mArcBall.setHandness( util::RIGHT_HAND );
        //mArcBall.setViewMatrix( mView );
        mArcBall.connectToInput();

		return true;
	}

	void onQuit()
	{
	}

	void onUpdate()
	{
	}

	void onKeyPress( input::KeyEvent ke )
	{
		app::SampleApp::onKeyPress( ke );
	}

	void onRender()
	{
	}
};

int main()
{
	MyApp app;
	return app.run( 0, 0 );
}
