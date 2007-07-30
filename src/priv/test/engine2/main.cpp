#include "pch.h"
#include "font.h"
#include "testCase.h"
//#include "triangle.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine2;
using namespace GN::scene;

int run( RenderEngine & re )
{
    ClearScreen cs;
    if( !cs.init( re ) ) return -1;

    test::BitmapFont font( re );
    FontFaceDesc ffd;
    ffd.fontname = "font::/simsun.ttc";
    ffd.width  = 16;
    ffd.height = 16;
    if( !font.init( ffd ) ) return -1;

    TestCase * cases[] =
    {
        //new TestCube(em,re,qr),
        0, //new TestTriangle(re),
    };

    for( size_t i = 0; i < GN_ARRAY_COUNT(cases); ++i )
    {
        TestCase * c = cases[i];

        bool next = false;

        // run the case
        while( 1 )
        {
            gInput.processInputEvents();

            KeyEvent k = gInput.popLastKeyEvent();
            if( k.status.down )
            {
                if( KEY_ESCAPE == k.code ) { next = false; break; }
                if( KEY_SPACEBAR == k.code ) { next = true; break; }
            }

            cs.render(); // clear screen

            if( c ) c->render();

            // draw some text
            font.drawText( L"ENGINE2 test application", 10, 10 );

            re.present();
        }

        // delete the case
        if( c )
        {
            c->quit();
            delete c;
        }

        if( !next ) break;
    }

    return 0;
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
