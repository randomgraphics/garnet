#include "pch.h"
#include "simple.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

class ShadowMap : public SampleApp
{
    SimpleShadowMap * mSimple;

public:

    ShadowMap() : mSimple(0)
    {
    }

    bool OnInit()
    {
        mSimple = new SimpleShadowMap(*this);
        if( !mSimple->Init( "a/objects.actor.xml" ) ) return false;

        // success
        return true;
    }

    void OnQuit()
    {
        delete mSimple;
        mSimple = 0;
    }

    void OnUpdate()
    {
        mSimple->Update();
    }

    void OnRender()
    {
        mSimple->Draw();
    }
};

int main( int argc, const char * argv[] )
{
    ShadowMap app;
    return app.Run( argc, argv );
}
