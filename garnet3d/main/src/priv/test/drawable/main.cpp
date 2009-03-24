#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
using namespace GN::util;

Drawable d1, d2;

const char * vscode =
    "varying vec2 texcoords; \n"
    "uniform mat4 transform; \n"
    "void main() { \n"
    "   gl_Position = transform * gl_Vertex; \n"
    "   texcoords.xy = gl_Vertex.xy; \n"
    "}";

const char * pscode =
    "uniform sampler2D t0; \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_FragColor = texture2D( t0, texcoords ); \n"
    "}";

bool init( Renderer & rndr )
{
    // create effect
    EffectDesc ed;
    ed.uniforms["pvw"].size = sizeof(Matrix44f);
    ed.textures["diffuse"]; // create a texture parameter named "diffuse"
    ed.shaders["glsl"].gpd.lang = GPL_GLSL;
    ed.shaders["glsl"].gpd.vs.code = vscode;
    ed.shaders["glsl"].gpd.ps.code = pscode;
    ed.shaders["glsl"].uniforms["transform"] = "pvw";
    ed.shaders["glsl"].textures["t0"] = "diffuse";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";
    Effect e( rndr );
    if( !e.init( ed ) ) return false;

    // set transformation to identity
    Matrix44f m;
    m.identity();
    e.getUniform( "pvw" )->update( m );

    // create texture
    e.setTexture( "diffuse", AutoRef<Texture>(loadTextureFromFile( rndr, "media::texture\\earth.jpg" )).get() );

    // create mesh
    float vertices[] =
    {
        0,0,0,1,
        1,0,0,1,
        1,1,0,1,
        0,1,0,1,
    };
    UInt16 indices[] = { 0, 1, 3, 2 };
    MeshDesc md;
    md.vtxfmt.numElements = 1;
    strcpy_s( md.vtxfmt.elements[0].binding, "position" );
    md.vtxfmt.elements[0].bindingIndex = 0;
    md.vtxfmt.elements[0].format = COLOR_FORMAT_FLOAT4;
    md.vtxfmt.elements[0].offset = 0;
    md.vtxfmt.elements[0].stream = 0;
    md.prim = TRIANGLE_LIST;
    md.numvtx = 4;
    md.numidx = 3;
    md.vertices[0] = vertices;
    md.indices = indices;
    Mesh mesh(rndr);
    if( !mesh.init( md ) ) return false;

    // create drawable 1
    d1.rc.clear();
    if( !e.applyToDrawable( d1, 0 ) ) return false;
    mesh.applyToDrawable( d1 );

    // make a clone of the whole drawable, with its own transformation parameter
    UInt32 ui = d1.rc.gpuProgram->getUniformIndex( "transform" );
    d2 = d1;
    d2.rc.uniforms[ui].attach( rndr.createUniform( sizeof(Matrix44f) ) );

    // modify d2's transformation (should not affect d1)
    m.translate( -1.0f, -1.0f, 0.0f );
    d2.rc.uniforms[ui]->update( m );

    // success
    return true;
}

void quit( Renderer & )
{
    d1.clear();
    d2.clear();
}

void draw( Renderer & )
{
    d1.draw();
    d2.draw();
}

int run( Renderer & rndr )
{
    if( !init( rndr ) ) { quit( rndr ); return -1; }

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel( Logger::LL_VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        rndr.processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KEY_ESCAPE ).down )
        {
            gogogo = false;
        }

        rndr.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw( rndr );
        rndr.present();

        fps.onFrame();
    }

    quit( rndr );

    return 0;
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
    enableCRTMemoryCheck();

    RendererOptions o;
    o.api = API_OGL;

    Renderer * r = createMultiThreadRenderer( o );
    //Renderer * r = createSingleThreadRenderer( o );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteRenderer( r );

    return result;
}
