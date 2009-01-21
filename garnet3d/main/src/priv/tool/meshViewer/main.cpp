#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.tool.meshViewer");

const char * meshFileName;
Renderer * rndr;
ArcBall    arcball; // arcball camera
float      radius;  // distance from camera to object
Matrix44f  proj, view;

void updateRadius()
{
    const DispDesc & dd = rndr->getDispDesc();

    view.lookAtRh( Vector3f(0,0,radius), Vector3f(0,0,0), Vector3f(0,1,0) );
    rndr->composePerspectiveMatrixRh( proj, GN_PI/4.0f, (float)dd.width/dd.height, radius / 100.0f, radius * 2.0f );

    float h = tan( 0.5f ) * radius * 2.0f;
    arcball.setViewMatrix( view );
    arcball.setTranslationSpeed( h / dd.height );

    // TODO: update light position
    // mScene.light(0).position.set( 0, 0, radius ); // head light: same location as camera.

    // calculate move speed
}

bool init()
{
    // update camera stuff
    radius = 2.0f; // initial radius should be 2.0 * mesh-bounding-sphere-radius
    updateRadius();

    // initialize arcball
    arcball.setHandness( util::RIGHT_HAND );
    arcball.setViewMatrix( view );
    arcball.setTranslation( Vector3f(0,0,0) ); // TODO: initial translation should be mesh center.
    arcball.connectToInput();

    return true;
}

void quit()
{
}

void onAxisMove( Axis a, int d )
{
    if( AXIS_MOUSE_WHEEL_0 == a )
    {
        float speed = radius / 100.0f;
        radius -= speed * d;
        if( radius < 0.1f ) radius = 0.1f;
        updateRadius();
    }
}

void update( double )
{
}

void draw()
{
}

void drawCoords()
{
    static const float X[] = { 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f, 0.0f };
    static const float Y[] = { 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f };
    static const float Z[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f };

    const Matrix44f & world = arcball.getRotationMatrix44();
    rndr->drawLines( 0, X, 3*sizeof(float), 2, GN_RGBA32(255,0,0,255), world, view, proj );
    rndr->drawLines( 0, Y, 3*sizeof(float), 2, GN_RGBA32(0,255,0,255), world, view, proj );
    rndr->drawLines( 0, Z, 3*sizeof(float), 2, GN_RGBA32(0,0,255,255), world, view, proj );
}

int run()
{
    if( !init() ) { quit(); return -1; }

    bool gogogo = true;

    FpsCalculator fps;

    printf( "Press ESC to exit...\n" );
    while( gogogo )
    {
        // handle inputs
        rndr->processRenderWindowMessages( false );
        Input & in = gInput;
        in.processInputEvents();
        if( in.getKeyStatus( KEY_ESCAPE ).down )
        {
            gogogo = false;
        }

        // render
        rndr->clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        update( fps.getLastFrameElasped() );
        draw();
        drawCoords();
        rndr->present();

        fps.onFrame();
    }

    quit();

    return 0;
}

void printHelp( const char * exepath )
{
    StrA exefilename = baseName( exepath ) + extName( exepath );

    printf( "\nUsage: %s <meshfile>\n", exefilename.cptr() );
}

struct InputInitiator
{
    InputInitiator( Renderer & r )
    {
        initializeInputSystem( API_NATIVE );
        const DispDesc & dd = r.getDispDesc();
        gInput.attachToWindow( dd.displayHandle, dd.windowHandle );
    }

    ~InputInitiator()
    {
        shutdownInputSystem();
    }
};

int main( int, const char *[] )
{
    printf( "\nGarnet mesh viewer V0.1.\n" );

    enableCRTMemoryCheck();

    /* parse command line
    if( argc < 2 )
    {
        printHelp( argv[0] );
        return -1;
    }
    meshFileName = argv[1];*/

    // create renderer
    RendererOptions o;
    o.api = API_OGL;
    rndr = createMultiThreadRenderer( o );
    //rndr = createSingleThreadRenderer( o );
    if( NULL == rndr ) return -1;

    // initialize input device
    InputInitiator ii(*rndr);

    // enter main loop
    int result = run();

    // done
    deleteRenderer( rndr );
    return result;
}

