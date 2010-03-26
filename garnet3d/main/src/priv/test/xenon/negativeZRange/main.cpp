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

	bool OnInit()
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

	void OnQuit()
	{
	}

	void OnUpdate()
	{
	}

	void OnKeyPress( input::KeyEvent ke )
	{
		app::SampleApp::OnKeyPress( ke );
	}

	void OnRender()
	{
	}
};

int main()
{
	MyApp app;
	return app.Run( 0, 0 );
}
