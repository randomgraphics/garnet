#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

static GN::Logger          * sLogger = GN::getLogger("GN.gfx.tool.meshViewer");
const char                 * filename;
Renderer                   * rndr;
ArcBall                      arcball; // arcball camera
float                        radius;  // distance from camera to object
Matrix44f                    proj, view;
AseScene                     ase;
DynaArray<Mesh*>             meshes;
DynaArray<AutoRef<Texture> > textures;
SimpleDiffuseEffect          effect;
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

    effect.setLightPos( Vector4f(0, 0, radius,1) ); // head light: same location as camera.

    // calculate move speed
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
    // load meshes
    DiskFile file;
    if( !file.open( filename, "rb" ) ) return false;
    if( !loadAseSceneFromFile(ase, file) ) return false;
    for( size_t i = 0; i < ase.meshes.size(); ++i )
    {
        AutoObjPtr<Mesh> m( new Mesh(*rndr) );
        if( !m || !m->init(ase.meshes[i]) ) return false;
        meshes.append( m );
        m.detach();
    }

    // load textures

    // initialize effect
    if( !effect.init( *rndr ) ) return false;

    // update camera stuff
    radius = ase.bbox.size()[ase.bbox.theLongestAxis()] * 1.5f;
    updateRadius();

    // initialize arcball
    arcball.setHandness( util::RIGHT_HAND );
    arcball.setViewMatrix( view );
    arcball.setTranslation( -ase.bbox.center() );
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
    ase.clear();

    for( size_t i = 0; i < meshes.size(); ++i )
    {
        delete meshes[i];
    }

    effect.quit();

    font.quit();
    safeDelete( sr );
}

void draw( const wchar_t * fps )
{
    Vector3f   position = arcball.getTranslation();
    Matrix44f  rotation = arcball.getRotationMatrix44();
    Matrix44f  world    = rotation * Matrix44f::sTranslate( position );
    effect.setTransformation( proj, view, world );

    for( size_t i = 0; i < ase.subsets.size(); ++i )
    {
        const AseMeshSubset & s = ase.subsets[i];

        Mesh * m = meshes[s.meshid];

        effect.setMesh( *m, &s );

        effect.draw();
    }

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

int main( int argc, const char * argv[] )
{
    printf( "\nGarnet mesh viewer V0.1.\n" );

    enableCRTMemoryCheck();

    // parse command line
    if( argc < 2 )
    {
        printHelp( argv[0] );
        return -1;
    }
    filename = argv[1];

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
