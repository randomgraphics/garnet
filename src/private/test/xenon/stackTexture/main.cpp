#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

class StackTexture : public app::SampleApp
{
public:

	StackTexture() {}

	bool onInit()
	{
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
        RenderEngine & re = getRenderEngine();
        re.clearScreen( Vector4f(0,0,0,1) ); // clear to pure black
	}
};

int main()
{
	StackTexture app;
	return app.run( 0, 0 );
}
