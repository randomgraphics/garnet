#include "pch.h"
#include "testCase.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine2;

int run( RenderEngine & re )
{
    ClearScreen cs;
    if( !cs.init( re ) ) return -1;

    while( 1 )
    {
        gInput.processInputEvents();

        KeyEvent k = gInput.popLastKeyEvent();
        if( k.status.down )
        {
            if( KEY_ESCAPE == k.code ) return 0;
            if( KEY_SPACEBAR == k.code ) return 0;
        }

        cs.draw(); // clear screen

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

    return run( re );
}
