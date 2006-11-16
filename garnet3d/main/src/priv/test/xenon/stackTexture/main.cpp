#include "pch.h"

using namespace GN;
using namespace GN::gfx;

class StackTexture : public app::SampleApp
{
public:
	StackTexture() {}

	bool onRendererCreate()
	{
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
	StackTexture app;
	return app.run( 0, 0 );
}
