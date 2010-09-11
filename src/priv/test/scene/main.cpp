#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

struct Scene
{
    Gpu               & gpu;
    GpuResourceDatabase gdb;
    World               world;

    Scene( Gpu & g )
        : gpu(g)
        , gdb(g)
        , world( gdb )
    {
    }
};

Entity * root;
Entity * light;
Entity * box;
Entity * robot;
Camera   camera;

bool init( Scene & sc )
{
    root = sc.world.createSpatialEntity( "root" );

    // robot stays at the origin.
    robot = sc.world.createVisualEntity();
    robot->getNode<SpatialNode>()->setParent( root->getNode<SpatialNode>() );
    robot->getNode<VisualNode>()->loadModelsFromFile( "media::/boxes/boxes.ase" );//R.F.R01/a01.ase" );

    const Spheref & bs = robot->getNode<SpatialNode>()->getBoundingSphere();

    Vector3f eye( 0, 0, bs.radius * 2.0f );

    // light is where eyes are
    light = sc.world.createLightEntity( "light" );
    light->getNode<SpatialNode>()->setParent( root->getNode<SpatialNode>() );
    light->getNode<SpatialNode>()->setPosition( eye );

    // setup camera
    Matrix44f proj, view;
    sc.gpu.composePerspectiveMatrixRh( proj, 1.0f, 4.0f/3.0f, bs.radius / 100.0f, bs.radius * 10.0f );
    view.lookAtRh( eye, Vector3f(0,0,0), Vector3f(0,1,0) );
    camera.setProjectionMatrix( proj );
    camera.setViewMatrix( view );

    // success
    return true;
}

void quit( Scene & )
{
}

void update()
{
}

void draw( Scene & )
{
    robot->getNode<VisualNode>()->graph().draw( camera );
}

struct InputInitiator
{
    InputInitiator( Gpu & g )
    {
        initializeInputSystem( InputAPI::NATIVE );
        const DispDesc & dd = g.getDispDesc();
        gInput.attachToWindow( dd.displayHandle, dd.windowHandle );
    }

    ~InputInitiator()
    {
        shutdownInputSystem();
    }
};

int run( Gpu & gpu )
{
    InputInitiator ii(gpu);

    Scene sc( gpu );

    if( !init( sc ) ) { quit( sc ); return -1; }

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel( Logger::VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        gpu.processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KeyCode::ESCAPE ).down )
        {
            gogogo = false;
        }
        update();

        gpu.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw( sc );
        gpu.present();

        fps.onFrame();
    }

    quit( sc );

    return 0;
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

    Gpu * r;
    if( cmdargs.useMultiThreadGpu )
        r = createMultiThreadGpu( cmdargs.rendererOptions );
    else
        r = createSingleThreadGpu( cmdargs.rendererOptions );
    if( NULL == r ) return -1;

    int result = run( *r );

    deleteGpu( r );

    return result;
}
