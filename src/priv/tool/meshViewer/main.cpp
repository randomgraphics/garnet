#include "pch.h"
#include "loader.h"
#include "../../test/cmdargs.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;
using namespace GN::scene;

static GN::Logger          * sLogger = GN::getLogger("GN.tool.meshViewer");
const char                 * filename;
Renderer                   * rndr;
ArcBall                      arcball; // arcball camera
float                        radius;  // distance from camera to object
Matrix44f                    proj, view;
AutoObjPtr<Scene>            rootScene;
AutoObjPtr<GeometryNode>     model;
SpriteRenderer             * sr = NULL;
BitmapFont                   font;

void updateRadius()
{
    const DispDesc & dd = rndr->getDispDesc();

    view.lookAtRh( Vector3f(0,0,radius), Vector3f(0,0,0), Vector3f(0,1,0) );
    rndr->composePerspectiveMatrixRh( proj, GN_PI/4.0f, (float)dd.width/dd.height, radius / 100.0f, radius * 2.0f );

    float h = tan( 0.5f ) * radius * 2.0f;
    arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );
    arcball.setViewMatrix( view );
    arcball.setTranslationSpeed( h / dd.height );

    rootScene->setDefaultLight0Position( Vector3f(0,0,radius) ); // head light: same location as camera.

    // calculate move speed
}

void onRenderWindowResize( HandleType, UInt32 width, UInt32 height )
{
    arcball.setMouseMoveWindow( 0, 0, (int)width, (int)height );
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

bool init()
{
    rndr->sigRendererWindowSizeMove.connect( &onRenderWindowResize );

    SimpleDiffuseEffect effect;

    // create scene
    rootScene.attach( createScene( *rndr ) );
    if( !rootScene ) return false;

    // initialize effect
    if( !effect.init( *rndr ) ) return false;

    // load model
    model.attach( loadGeometryFromFile( *rootScene, effect.getEffect(), filename ) );
    if( !model ) return false;

    // update camera stuff
    const Spheref & bs = model->getBoundingSphere();
    radius = bs.radius * 1.5f;
    updateRadius();

    // initialize arcball
    arcball.setHandness( util::RIGHT_HAND );
    arcball.setViewMatrix( view );
    arcball.setTranslation( -bs.center );
    arcball.connectToInput();

    // load font
    sr = new SpriteRenderer( *rndr );
    if( !sr->init() ) return false;
    AutoRef<FontFace> ff( createSimpleAsciiFont() );
    if( !ff ) return false;
    if( !font.init( sr, ff ) ) return false;

    // connect to input device
    gInput.sigAxisMove.connect( onAxisMove );

    // success
    return true;
}

void quit()
{
    model.clear();
    rootScene.clear();
    font.quit();
    safeDelete( sr );
}

void draw( const wchar_t * fps )
{
    const Vector3f & position = arcball.getTranslation();

    rootScene->setProj( proj );
    rootScene->setView( view );
    model->setPosition( position );
    model->setRotation( arcball.getRotation() );
    rootScene->renderNodeHierarchy( model );

    font.drawText( fps, 0, 0 );
    font.drawText(
        strFormat(
            L"position : %f, %f, %f\n"
            L"radius   : %f",
            position.x, position.y, position.z,
            radius ).cptr(),
        0, 20 );
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
        draw( fps.getFpsString() );
        drawCoords();
        rndr->present();

        fps.onFrame();
    }

    quit();

    return 0;
}

void printHelp( const CommandLineArguments & cmdargs )
{
    StrA exefilename = fs::baseName( cmdargs.applicationName ) + fs::extName( cmdargs.applicationName );

    GN_INFO(sLogger)( "\nUsage: %s [options] meshfile\n", exefilename.cptr() );
    cmdargs.showStandardCommandLineOptions();
}

struct InputInitiator
{
    InputInitiator( Renderer & r )
    {
        initializeInputSystem( InputAPI::NATIVE );
        const DispDesc & dd = r.getDispDesc();
        gInput.attachToWindow( dd.displayHandle, dd.windowHandle );
    }

    ~InputInitiator()
    {
        shutdownInputSystem();
    }
};

int main( int argc, const char * argv[] )
{
    printf( "\nGarnet mesh viewer V0.1.\n" );

    enableCRTMemoryCheck();

    // parse command line
    CommandLineArguments cmdargs( argc, argv );
    switch( cmdargs.status )
    {
        case CommandLineArguments::SHOW_HELP:
            printHelp( cmdargs );
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

    // get mesh file name
    if( 0 == cmdargs.extraArgc )
    {
        GN_ERROR(sLogger)( "Mesh file name is missing." );
        return -1;
    }
    filename = cmdargs.extraArgv[0];

    // create renderer
    if( cmdargs.useMultiThreadRenderer )
        rndr = createMultiThreadRenderer( cmdargs.rendererOptions );
    else
        rndr = createSingleThreadRenderer( cmdargs.rendererOptions );
    if( NULL == rndr ) return -1;

    // initialize input device
    InputInitiator ii(*rndr);

    // enter main loop
    int result = run();

    // done
    deleteRenderer( rndr );
    return result;
}
