#include "pch.h"
#include "scene.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
using namespace GN::util;
using namespace GN::scene;

struct Scene
{
    Gpu               & gpu;
    GpuResourceDatabase gdb;
    World               world;

    Scene( Gpu & g )
        : gpu(g)
        , gdb(g)
    {
    }
};

Entity * root;
Entity * light;
Entity * object1;
Entity * object2;
Camera   camera;

bool init( Scene & sc )
{
    AutoRef<ModelResource> sphereModel = ModelResource::loadFromFile( sc.gdb, "media://model/sphere.ase" );
    AutoRef<ModelResource> robotModel = ModelResource::loadFromFile( sc.gdb, "media://model/R.F.R01/a01.ase" );

    Spheref bs;
    sphereModel->getMeshResource()->calculateBoundingSphere( bs );

    root = sc.world.createEntity( SPATIAL_ENTITY, "root" );

    // light is right above the origin
    light = sc.world.createEntity( LIGHT_ENTITY, "light" );
    light->getNode<SpatialNode>()->setParent( root->getNode<SpatialNode>() );
    light->getNode<SpatialNode>()->setPosition( Vector3f( 0, bs.radius * 2.0f, 0 ) );

    // object1 is attached to the camera
    object1 = sc.world.createEntity( VISUAL_ENTITY );
    object1->getNode<SpatialNode>()->setParent( light->getNode<SpatialNode>() );
    object1->getNode<VisualNode>()->addModel( sphereModel );

    // object2 stays at the origin.
    object2 = sc.world.createEntity( VISUAL_ENTITY );
    object2->getNode<SpatialNode>()->setParent( root->getNode<SpatialNode>() );
    object2->getNode<VisualNode>()->addModel( robotModel );

    // setup camera
    Matrix44f proj, view;
    sc.gpu.composePerspectiveMatrixRh( proj, 1.0f, 4.0f/3.0f, bs.radius / 100.0f, bs.radius * 10.0f );
    view.lookAtRh( Vector3f( 0, 0, bs.radius * 2.0f ), Vector3f(0,0,0), Vector3f(0,0,1) );
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
    root->getNode<VisualNode>()->draw( camera );
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
