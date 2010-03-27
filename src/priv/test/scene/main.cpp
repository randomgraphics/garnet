#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
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

bool Init( Scene & sc )
{
    root = sc.world.createSpatialEntity( "root" );

    // robot stays at the origin.
    robot = sc.world.createVisualEntity();
    robot->getNode<SpatialNode>()->SetParent( root->getNode<SpatialNode>() );
    robot->getNode<VisualNode>()->loadModelsFromFile( "media::/boxes/boxes.ase" );//R.F.R01/a01.ase" );

    const Spheref & bs = robot->getNode<SpatialNode>()->getBoundingSphere();

    Vector3f eye( 0, 0, bs.radius * 2.0f );

    // light is where eyes are
    light = sc.world.createLightEntity( "light" );
    light->getNode<SpatialNode>()->SetParent( root->getNode<SpatialNode>() );
    light->getNode<SpatialNode>()->setPosition( eye );

    // setup camera
    Matrix44f proj, view;
    sc.gpu.ComposePerspectiveMatrixRh( proj, 1.0f, 4.0f/3.0f, bs.radius / 100.0f, bs.radius * 10.0f );
    view.LookAtRh( eye, Vector3f(0,0,0), Vector3f(0,1,0) );
    camera.setProjectionMatrix( proj );
    camera.setViewMatrix( view );

    // success
    return true;
}

void Quit( Scene & )
{
}

void Update()
{
}

void Draw( Scene & )
{
    robot->getNode<VisualNode>()->graph().Draw( camera );
}

struct InputInitiator
{
    InputInitiator( Gpu & g )
    {
        InitializeInputSystem( InputAPI::NATIVE );
        const DispDesc & dd = g.GetDispDesc();
        gInput.AttachToWindow( dd.displayHandle, dd.windowHandle );
    }

    ~InputInitiator()
    {
        ShutdownInputSystem();
    }
};

int Run( Gpu & gpu )
{
    InputInitiator ii(gpu);

    Scene sc( gpu );

    if( !Init( sc ) ) { Quit( sc ); return -1; }

    bool gogogo = true;

    FpsCalculator fps;
    GetLogger("GN.util.fps")->SetLevel( Logger::VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        gpu.ProcessRenderWindowMessages( false );

        Input & in = gInput;

        in.ProcessInputEvents();

        if( in.GetKeyStatus( KeyCode::ESCAPE ).down )
        {
            gogogo = false;
        }
        Update();

        gpu.ClearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        Draw( sc );
        gpu.Present();

        fps.onFrame();
    }

    Quit( sc );

    return 0;
}

int main( int argc, const char * argv[] )
{
    EnableCRTMemoryCheck();

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
        r = CreateMultiThreadGpu( cmdargs.rendererOptions );
    else
        r = CreateSingleThreadGpu( cmdargs.rendererOptions );
    if( NULL == r ) return -1;

    int result = Run( *r );

    DeleteGpu( r );

    return result;
}
