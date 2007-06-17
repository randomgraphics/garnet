#include "pch.h"
using namespace GN;
using namespace GN::gfx2;

static bool quit = false;

int run( GraphicsSystem & gs )
{
    while( !quit )
    {
        gs.onFrame();
    }

    return 0;
}

int main()
{
    GraphicsSystemCreationParameter gscp =
    {
        FOURCC::sMake( 'D','X','9',0 ),
        0,               // first monitor
        640, 480, 32, 0, // fullscreen properties
        800, 600,        // windowed properties
        false,           // windowed mode
        false,           // no vsync
    };

    GraphicsSystemCreator gs;

    if( !gs.create( gscp ) ) return -1;

    return run( *gs.get() );
}
