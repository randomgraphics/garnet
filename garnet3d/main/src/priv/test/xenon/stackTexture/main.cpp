#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

class StackTexture : public app::SampleApp
{
public:

	StackTexture() {}

	bool OnInit()
	{
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
        RenderEngine & re = getRenderEngine();
        re.clearScreen( Vector4f(0,0,0,1) ); // clear to pure black
	}
};

int main()
{
	StackTexture app;
	return app.Run( 0, 0 );
}
