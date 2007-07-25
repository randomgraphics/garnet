#include "pch.h"
#include "testCase.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine2;

void run( RenderEngine & re )
{
    while( 1 )
    {
        gInput.processInputEvents();

        KeyEvent k = gInput.popLastKeyEvent();
        if( k.status.down )
        {
            if( KEY_ESCAPE == k.code ) return;
            if( KEY_SPACEBAR == k.code ) return;
        }

        //re.clearScreen( Vector4f(0,0,1,0) );
        //c.draw();
        re.present();
    }
}

struct InputInitiator
{
    InputInitiator()
    {
        createInputSystem( API_NATIVE );
    }

    ~InputInitiator()
    {
        if( gInputPtr ) delete gInputPtr;
    }
};

int main()
{
    InputInitiator input;
    RenderEngine   re;

    size_t MB = 1024 * 1024;
    RenderEngineInitParameters reip = { 32*MB, 4*MB };

    GraphicsSystemCreationParameter gscp = {
        GN_MAKE_FOURCC( 'A', 'U', 'T', 'O' ),
        0, // monitor
        640, 480, 32, 0, // fullscreen properties
        640, 480, // windowed properties
        false, // windowed mode
        false, // no vsync
    };

    if( !re.init(reip) || !re.reset( gscp ) ) return -1;

    run( re );

    // success
    return 0;
}
