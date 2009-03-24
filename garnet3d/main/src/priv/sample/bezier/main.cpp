#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;
using namespace GN::scene;

struct BezierVertex
{
    Vector3f p0, p1, p2;
    Vector3f n0, n1, n2;
    Vector2f t0, t1, t2;
    Vector2f bc;

    BezierVertex( float u, float v )
    {
        p0.set( -1, -1, 0 ); p1.set(  1, -1, 0 ); p2.set(    0,  1, 0 );
        n0.set( -1, -1, 1 ); n1.set(  1, -1, 1 ); n2.set(    0,  1, 1 );
        t0.set(  0,  1 );    t1.set(  1,  1 );    t2.set( 0.5f,  0 );
        bc.set(  u,  v );

        n0.normalize();
        n1.normalize();
        n2.normalize();
    }
};

static GN::Logger          * sLogger = GN::getLogger("GN.sample.Bezier");
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

Mesh * createMesh()
{
    MeshDesc md;

    md.vtxfmt.numElements = 7;
    md.vtxfmt.elements[0].stream = 0;
    md.vtxfmt.elements[0].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[0].offset = GN_FIELD_OFFSET( BezierVertex, p0 );
    md.vtxfmt.elements[0].bindTo( "pos", 0 );
    md.vtxfmt.elements[1].stream = 0;
    md.vtxfmt.elements[1].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[1].offset = GN_FIELD_OFFSET( BezierVertex, p1 );
    md.vtxfmt.elements[1].bindTo( "pos", 1 );
    md.vtxfmt.elements[2].stream = 0;
    md.vtxfmt.elements[2].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[2].offset = GN_FIELD_OFFSET( BezierVertex, p2 );
    md.vtxfmt.elements[2].bindTo( "pos", 2 );
    md.vtxfmt.elements[3].stream = 0;
    md.vtxfmt.elements[3].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[3].offset = GN_FIELD_OFFSET( BezierVertex, n0 );
    md.vtxfmt.elements[3].bindTo( "normal", 0 );
    md.vtxfmt.elements[4].stream = 0;
    md.vtxfmt.elements[4].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[4].offset = GN_FIELD_OFFSET( BezierVertex, n1 );
    md.vtxfmt.elements[4].bindTo( "nml", 1 );
    md.vtxfmt.elements[5].stream = 0;
    md.vtxfmt.elements[5].format = COLOR_FORMAT_FLOAT3;
    md.vtxfmt.elements[5].offset = GN_FIELD_OFFSET( BezierVertex, n2 );
    md.vtxfmt.elements[5].bindTo( "nml", 2 );
    md.vtxfmt.elements[6].stream = 0;
    md.vtxfmt.elements[6].format = COLOR_FORMAT_FLOAT2;
    md.vtxfmt.elements[6].offset = GN_FIELD_OFFSET( BezierVertex, bc );
    md.vtxfmt.elements[6].bindTo( "bc" );

    //                               v0 (0,0)
    //                              / \
    //                             /   \
    //                            /     \
    //                           /       \
    //                          /         \
    //                 (1/3,0) v1----------v2 (0,1/3)
    //                        / \         / \
    //                       /   \       /   \
    //                      /     \     /     \
    //                     /       \   /       \
    //                    /         \ /         \
    //           (2/3,0) v3----------v4----------v5 (0,2/3)
    //                  / \         / \(1/3,1/3)/ \
    //                 /   \       /   \       /   \
    //                /     \     /     \     /     \
    //               /       \   /       \   /       \
    //              /         \ /         \ /         \
    //             v6----------v7----------v8---------v9
    //           (1,0)     (2/3,1/3)   (1/3,2/3)     (0,1)

    float a = 1.0f / 3.0f;
    float b = 2.0f / 3.0f;
    BezierVertex vertices[] =
    {
        BezierVertex( 0, 0 ), // 0
        BezierVertex( a, 0 ), // 1
        BezierVertex( 0, a ), // 2
        BezierVertex( b, 0 ), // 3
        BezierVertex( a, a ), // 4
        BezierVertex( 0, b ), // 5
        BezierVertex( 1, 0 ), // 6
        BezierVertex( b, a ), // 7
        BezierVertex( a, b ), // 8
        BezierVertex( 0, 1 ), // 9
    };
    md.numvtx = GN_ARRAY_COUNT( vertices );
    md.vertices[0] = vertices;
    md.strides[0] = sizeof(BezierVertex);

    UInt16 indices[] =
    {
        0, 1, 2,
        1, 3, 4,
        1, 4, 2,
        2, 4, 5,
        3, 6, 7,
        3, 7, 4,
        4, 7, 8,
        4, 8, 5,
        5, 8, 9,
    };
    md.numidx = GN_ARRAY_COUNT( indices );
    md.indices = indices;
    md.prim = TRIANGLE_LIST;

    AutoObjPtr<Mesh> mesh( new Mesh(*rndr) );
    if( !mesh || !mesh->init(md) ) return false;

    return mesh.detach();
}

Effect *
createEffect()
{
    const char * glslvscode =

        // define bezier control point structure
        "struct BezierControlPoints \n"
        "{                          \n"
        "   vec3 p300;              \n"
        "   vec3 p030;              \n"
        "   vec3 p003;              \n"
        "   vec3 p210;              \n"
        "   vec3 p120;              \n"
        "   vec3 p201;              \n"
        "   vec3 p102;              \n"
        "   vec3 p012;              \n"
        "   vec3 p021;              \n"
        "   vec3 p111;              \n"
        "                           \n"
        "   vec3 n200;              \n"
        "   vec3 n020;              \n"
        "   vec3 n002;              \n"
        "};                         \n"

        // utils to calculate bezier control points
        "BezierControlPoints                                                                     \n"
        "calcBezierControlPoints( vec3 p0, vec3 p1, vec3 p2, vec3 n0, vec3 n1, vec3 n2 )         \n"
        "{                                                                                       \n"
        "	BezierControlPoints cp;                                                              \n"
        "                                                                                        \n"
        "	cp.p300 = p0;                                                                        \n"
        "	cp.p030 = p1;                                                                        \n"
        "	cp.p003 = p2;                                                                        \n"
        "	cp.n200 = n0;                                                                        \n"
        "	cp.n020 = n1;                                                                        \n"
        "	cp.n002 = n2;                                                                        \n"
        "	cp.p210 = ( 2 * cp.p300 + cp.p030 - dot(cp.n200, (cp.p030 - cp.p300)) * cp.n200 ) / 3; \n"
        "	cp.p120 = ( 2 * cp.p030 + cp.p300 - dot(cp.n020, (cp.p300 - cp.p030)) * cp.n020 ) / 3; \n"
        "	cp.p201 = ( 2 * cp.p300 + cp.p003 - dot(cp.n200, (cp.p003 - cp.p300)) * cp.n200 ) / 3; \n"
        "	cp.p102 = ( 2 * cp.p003 + cp.p300 - dot(cp.n002, (cp.p300 - cp.p003)) * cp.n002 ) / 3; \n"
        "	cp.p012 = ( 2 * cp.p003 + cp.p030 - dot(cp.n002, (cp.p030 - cp.p003)) * cp.n002 ) / 3; \n"
        "	cp.p021 = ( 2 * cp.p030 + cp.p003 - dot(cp.n020, (cp.p003 - cp.p030)) * cp.n020 ) / 3; \n"
        "	cp.p111 = ( cp.p210 + cp.p120 + cp.p102 + cp.p201 + cp.p021 + cp.p012 ) / 4 - ( cp.p300 + cp.p030 + cp.p003 ) / 6;  \n"
        "                                                                                        \n"
        "	return cp;                                                                           \n"
        "}                                                                                       \n"

        // type and function to calcuatle single bezier vertex
        "struct BezierVertex                                             \n"
        "{                                                               \n"
        "	vec3 position;                                               \n"
        "	vec3 normal;                                                 \n"
        "};                                                              \n"
        "                                                                \n"
        "BezierVertex                                                    \n"
        "calcBezierVertex(                                               \n"
        "	BezierControlPoints cp,                                      \n"
        "	float u, float v )                                           \n"
        "{                                                               \n"
        "	float u2 = u * u;                                            \n"
        "	float u3 = u2 * u;                                           \n"
        "	float v2 = v * v;                                            \n"
        "	float v3 = v2 * v;                                           \n"
        "	float w  = 1.0 - u - v;                                      \n"
        "	float w2 = w * w;                                            \n"
        "	float w3 = w2 * w;                                           \n"
        "	                                                             \n"
        "	BezierVertex vert;                                           \n"
        "                                                                \n"
        "	vert.position =                                              \n"
        "		u3 * cp.p300 + v3 * cp.p030 + w3 * cp.p003 +             \n"
        "		3.0 * ( u2 * v * cp.p210 + u2 * w * cp.p201 + u * v2 * cp.p120 + v2 * w * cp.p021 + v * w2 * cp.p012 + u * w2 * cp.p102 ) + \n"
        "		6.0 * u * v * w * cp.p111;                               \n"
        "                                                                \n"
        "	vert.normal = cp.n200 * u + cp.n020 * v + cp.n002 * w;       \n"
        "	                                                             \n"
        "	return vert;                                                 \n"
        "}                                                               \n"

        // shader parameters
        "uniform   mat4 pvw; \n"
        "uniform   mat4 world; \n"
        "uniform   mat4 wit; \n"
        "attribute vec3 pos1; \n"
        "attribute vec3 pos2; \n"
        "attribute vec3 nml1; \n"
        "attribute vec3 nml2; \n"
        "attribute vec2 bc; \n"
        "varying   vec4 pos_world; // vertex position in world space \n"
        "varying   vec3 nml_world; // vertex normal in world space \n"
        "varying   vec2 texcoords; \n"

        // main function
        "void main() { \n"

        // calculate control points
        "   vec3 pos0 = gl_Vertex.xyz; \n"
        "   vec3 nml0 = gl_Normal.xyz; \n"
        "   BezierControlPoints cp = calcBezierControlPoints( pos0, pos1, pos2, nml0, nml1, nml2 ); \n"

        // calculate output vertex
        "   float u  = bc.x; \n"
        "   float v  = bc.y; \n"
        "   BezierVertex vert = calcBezierVertex( cp, u, v ); \n"

        // test code
        //"   vert.position = pos0 * (1-u-v) + pos1 * u + pos2 * v; \n"
        //"   vert.normal   = nml0 * (1-u-v) + nml1 * u + nml2 * v; \n"

        // translate position
        "   gl_Position = pvw * vec4(vert.position,1); \n"
        "   pos_world   = world * vec4(vert.position,1); \n"

        // translate normal
        "   nml_world = (wit * vec4(vert.normal,0)).xyz; \n"

        // texcoord
        "   vec2 tc0 = vec2(0.0, 0.0); \n"
        "   vec2 tc1 = vec2(1.0, 0.0); \n"
        "   vec2 tc2 = vec2(1.0, 1.0); \n"
        "   texcoords = tc0 * (1-u-v) + tc1 * u + tc2 * v; \n"

        // end of vertex shader
        "}";

    const char * glslpscode =
        "uniform vec4 lightpos; // light positin in world space \n"
        "uniform sampler2D t0; \n"
        "varying vec4 pos_world; // position in world space \n"
        "varying vec3 nml_world; // normal in world space \n"
        "varying vec2 texcoords; \n"
        "void main() { \n"
        "   vec3  L      = normalize( (lightpos - pos_world).xyz ); \n"
        "   vec3  N      = normalize( nml_world ); \n"
        "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
        "   vec4  tex    = texture2D( t0, texcoords ); \n"
        "   gl_FragColor = diff * tex; \n"
        "}";

    EffectDesc ed;
    ed.uniforms["MATRIX_PVW"].size = sizeof(Matrix44f);
    ed.uniforms["MATRIX_WORLD"].size = sizeof(Matrix44f);
    ed.uniforms["MATRIX_WORLD_IT"].size = sizeof(Matrix44f); // used to translate normal from local space into world space
    ed.uniforms["LIGHT0_POSITION"].size = sizeof(Vector4f);
    ed.textures["DIFFUSE_TEXTURE"]; // create a texture parameter named "DIFFUSE_TEXTURE"
    ed.shaders["glsl"].gpd.lang = GPL_GLSL;
    ed.shaders["glsl"].gpd.vs.code = glslvscode;
    ed.shaders["glsl"].gpd.ps.code = glslpscode;
    ed.shaders["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["glsl"].uniforms["world"] = "MATRIX_WORLD";
    ed.shaders["glsl"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.shaders["glsl"].uniforms["lightpos"] = "LIGHT0_POSITION";
    ed.shaders["glsl"].textures["t0"] = "DIFFUSE_TEXTURE";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";

    Effect * e = new Effect( *rndr );
    if( !e->init( ed ) ) { delete e; return NULL; }

    e->textures["DIFFUSE_TEXTURE"].attach( loadTextureFromFile( *rndr, "media::texture/earth.jpg" ) );

    return e;
}

bool init()
{
    AutoObjPtr<Mesh>   mesh;
    AutoObjPtr<Effect> effect;

    // create scene
    rootScene.attach( createScene( *rndr ) );
    if( !rootScene ) return false;

    // initialize effect
    effect.attach( createEffect() );
    if( !effect ) return false;

    // load meshes
    mesh.attach( createMesh() );
    if( !mesh ) return false;

    // create model
    model.attach( new GeometryNode(*rootScene) );
    model->addGeometryBlock( effect, mesh );
    model->setPivot( Vector3f(0,0,0) );

    // update camera stuff
    radius = 3.0f;
    updateRadius();

    // initialize arcball
    arcball.setHandness( util::RIGHT_HAND );
    arcball.setViewMatrix( view );
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

void printHelp( const char * exepath )
{
    StrA exefilename = fs::baseName( exepath ) + fs::extName( exepath );

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

