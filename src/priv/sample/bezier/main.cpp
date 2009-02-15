#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

struct BezierVertex
{
    Vector3f p0, p1, p2;
    Vector3f n0, n1, n2;
    Vector2f bc;

    BezierVertex( float u, float v )
    {
        p0.set( -1, -1, -1 );
        p1.set(  1, -1, -1 );
        p2.set(  1,  1, -1 );
        n0.set(  0,  0, -1 );
        n1.set(  0,  0, -1 );
        bc.set(  u,  v );
    }
};

static GN::Logger          * sLogger = GN::getLogger("GN.sample.Bezier");
Renderer                   * rndr;
ArcBall                      arcball; // arcball camera
float                        radius;  // distance from camera to object
Matrix44f                    proj, view;
AutoObjPtr<Mesh>             mesh;
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

    effect.setLightPos( Vector4f( 0, 0, radius, 1 ) ); // head light: same location as camera.

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

bool createMesh()
{
    MeshDesc md;

    md.vtxfmt.numElements = 7;
    md.vtxfmt.elements[0].stream = 0;
    md.vtxfmt.elements[0].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[0].offset = 0;
    md.vtxfmt.elements[0].setBinding( "position", 0 );
    md.vtxfmt.elements[1].stream = 0;
    md.vtxfmt.elements[1].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[1].offset = 12;
    md.vtxfmt.elements[1].setBinding( "attribute", 0 );
    md.vtxfmt.elements[2].stream = 0;
    md.vtxfmt.elements[2].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[2].offset = 24;
    md.vtxfmt.elements[2].setBinding( "attribute", 2 );
    md.vtxfmt.elements[3].stream = 0;
    md.vtxfmt.elements[3].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[3].offset = 36;
    md.vtxfmt.elements[3].setBinding( "normal", 0 );
    md.vtxfmt.elements[4].stream = 0;
    md.vtxfmt.elements[4].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[4].offset = 48;
    md.vtxfmt.elements[4].setBinding( "attribute", 2 );
    md.vtxfmt.elements[5].stream = 0;
    md.vtxfmt.elements[5].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[5].offset = 60;
    md.vtxfmt.elements[5].setBinding( "attribute", 3 );
    md.vtxfmt.elements[6].stream = 0;
    md.vtxfmt.elements[6].format = COLOR_FORMAT_FLOAT2;
    md.vtxfmt.elements[6].offset = 72;
    md.vtxfmt.elements[6].setBinding( "attribute", 4 );
    GN_CASSERT( sizeof(BezierVertex) == 80 );

    md.prim = TRIANGLE_LIST;

    BezierVertex vertices[] =
    {
        BezierVertex( 0, 0 ),
        BezierVertex( 1, 0 ),
        BezierVertex( 0, 1 ),
    };
    md.numvtx = GN_ARRAY_COUNT( vertices );
    md.vertices[0] = vertices;
    md.strides[0] = sizeof(BezierVertex);

    mesh.attach( new Mesh(*rndr) );
    if( !mesh || !mesh->init(md) ) return false;

    return true;
}

bool createEffect()
{
   if( !effect.init( *rndr ) ) return false;

   return true;
}

bool init()
{
    // create meshes
    if( !createMesh() ) return false;

    // initialize effect
    if( !createEffect() ) return false;

    // update camera stuff
    radius = 3.0f;
    updateRadius();

    // initialize arcball
    arcball.setHandness( util::RIGHT_HAND );
    arcball.setViewMatrix( view );
    arcball.setTranslation( Vector3f(0,0,0) );
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
    mesh.clear();

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

    effect.setMesh( *mesh, NULL );

    effect.draw();

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

int main()
{
    printf( "\nBezier triangle sample.\n" );

    enableCRTMemoryCheck();

    // create renderer
    RendererOptions o;
    o.api = API_OGL;
    //rndr = createMultiThreadRenderer( o );
    rndr = createSingleThreadRenderer( o );
    if( NULL == rndr ) return -1;

    // initialize input device
    InputInitiator ii(*rndr);

    // enter main loop
    int result = run();

    // done
    deleteRenderer( rndr );
    return result;
}
