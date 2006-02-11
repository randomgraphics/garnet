#include "pch.h"
#include "../common/sampleApp.h"

class RenderToTexture : public GN::sample::SampleApp
{
public:

    void onRender()
    {
        gRenderer.clearScreen();
    }
};

int main( int argc, const char * argv[] )
{
    RenderToTexture app;
    if( !app.init(argc,argv) ) return -1;
    return app.run();
}
