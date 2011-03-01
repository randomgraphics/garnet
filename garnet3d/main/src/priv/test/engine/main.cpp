#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::engine;
using namespace GN::util;

SampleSpacialEntity * root = NULL;
SampleVisualEntity * box = NULL;
SampleVisualEntity * robot = NULL;
Matrix44f proj, view;

bool init()
{
    root = new SampleSpacialEntity();

    // robot stays at the origin.
    robot = new SampleVisualEntity();
    robot->spacial()->setParent( root->spacial() );
    if( !loadModelsFromFile( *robot->visual(), "media::/boxes/boxes.ase" ) ) return false;
    //if( !loadModelsFromFile( *robot->visual(), "media::/model/R.F.R01/a01.ase" ) ) return false;

    const Boxf & bbox = robot->spacial()->getBoundingBox();
    Spheref bs;
    calculateBoundingSphereFromBoundingBox( bs, bbox );

    // light is where eyes are
    Vector3f eye( 0, 0, bs.radius * 2.0f );
    engine::getStandardUniformManager()->setGlobalUniform( engine::StandardUniformType::LIGHT0_POSITION, Vector4f(eye,1) );

    // setup camera
    getGpu()->composePerspectiveMatrixRh( proj, 1.0f, 4.0f/3.0f, bs.radius / 100.0f, bs.radius * 10.0f );
    view.lookAtRh( eye, bbox.center(), Vector3f(0,1,0) );

    // success
    return true;
}

void quit()
{
    safeDelete( box );
    safeDelete( robot );
    safeDelete( root );
}

void draw()
{
    engine::getStandardUniformManager()->setTransform( proj, view );
    robot->visual()->draw();
}

bool run()
{
    if( !init() ) { quit(); return false; }

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel( Logger::VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        getGpu()->processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KeyCode::ESCAPE ).down )
        {
            gogogo = false;
        }

        getGpu()->clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw();
        getGpu()->present();

        fps.onFrame();
    }

    quit();

    return true;
}

int main( int argc, const char * argv[] )
{
    enableCRTMemoryCheck();

    CommandLineArguments cmdargs( argc, argv );
    switch( cmdargs.status )
    {
        case CommandLineArguments::SHOW_HELP:
            cmdargs.showDefaultHelp();
            return 0;

        case CommandLineArguments::INVALID_COMMAND_LINE:
            return -1;

        case CommandLineArguments::CONTINUE_EXECUTION:
            // do nothing
            break;

        default:
            GN_UNEXPECTED();
            return -1;
    }

    bool noerror = engine::initialize();

    if( noerror )
    {
        noerror = engine::inputInitialize( input::InputAPI::NATIVE );
    }

    if( noerror )
    {
        // setup graphics options
        engine::GfxInitOptions gio;
        gio.gpuOptions = cmdargs.rendererOptions;
        gio.useMultithreadGpu = cmdargs.useMultiThreadGpu;
        gio.defaultNonAsciiFont.fontname = "font::/simsun.ttc";
        gio.defaultNonAsciiFont.width = 16;
        gio.defaultNonAsciiFont.height = 16;
        gio.defaultNonAsciiFont.quality = gfx::FontFaceDesc::ANTIALIASED;
        gio.defaultAsciiFont.fontname = "font::/ltype.ttf";
        gio.defaultAsciiFont.width = 16;
        gio.defaultAsciiFont.height = 16;
        gio.defaultAsciiFont.quality = gfx::FontFaceDesc::ANTIALIASED;

        noerror = engine::gfxInitialize( gio );
    }

    if( noerror )
    {
        noerror = run();
    }

    engine::shutdown();

    return noerror ? 0 : -1;
}
