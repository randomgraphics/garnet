#include "pch.h"
#include "simple.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

class ShadowMap : public GN::app::SampleApp
{
    SimpleShadowMap mSimple;

public:

    ShadowMap() 
    {
    }

    bool onRendererRestore()
    {

        if( !mSimple.init( "b/objects.actor.xml" ) ) return false;

        // success
        return true;
    }

    void onRendererDispose()
    {
        mSimple.quit();
    }

    void onUpdate()
    {
        mSimple.update();
    }

    void onRender()
    {
        mSimple.draw();
    }
};

int main( int argc, const char * argv[] )
{
    ShadowMap app;
    return app.run( argc, argv );
}
