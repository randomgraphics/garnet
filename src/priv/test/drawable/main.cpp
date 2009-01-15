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

/// read image from file
inline bool
readImageFromFile( ImageDesc & desc, std::vector<UInt8> & data )
{
    std::auto_ptr<File> fp( openFile( "media::texture\\earth.jpg", "rb" ) );
    if( NULL == fp.get() ) return false;

    ImageReader ir;
    if( !ir.reset( *fp ) ) return false;

    if( !ir.readHeader( desc ) ) return false;

    data.resize( desc.getTotalBytes() );
    if( !ir.readImage( &data[0] ) ) return false;

    return true;
}

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
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";
    Effect e( rndr );
    if( !e.init( ed ) ) return false;

    // create drawable 1
    d1.rc.resetToDefault();
    if( !e.setupDrawable( d1, NULL, 0 ) ) return false;

    // setup vertex format
    d1.rc.vtxfmt.numElements = 1;
    strcpy_s( d1.rc.vtxfmt.elements[0].binding, "position" );
    d1.rc.vtxfmt.elements[0].bindingIndex = 0;
    d1.rc.vtxfmt.elements[0].format = COLOR_FORMAT_FLOAT4;
    d1.rc.vtxfmt.elements[0].offset = 0;
    d1.rc.vtxfmt.elements[0].stream = 0;

    // load image
    ImageDesc id;
    std::vector<UInt8> texels;
    if( !readImageFromFile( id, texels ) ) return false;

    // create texture
    TextureDesc td;
    td.fromImageDesc( id );
    d1.rc.textures[0].attach( rndr.createTexture( td ) );
    if( d1.rc.textures[0] )
    {
        const MipmapDesc & md = id.getMipmap( 0, 0 );
        d1.rc.textures[0]->updateMipmap( 0, 0, 0, md.rowPitch, md.slicePitch, &texels[0], UPDATE_DEFAULT );
    }
    strcpy_s( d1.rc.texbinds[0], "t0" );

    // create vertex buffer
    static float vertices[] =
    {
        0,0,0,1,
        1,0,0,1,
        1,1,0,1,
        0,1,0,1,
    };
    VtxBufDesc vbd = {
        sizeof(vertices),
        false,
        false
    };
    d1.rc.vtxbufs[0].attach( rndr.createVtxBuf( vbd ) );
    if( NULL == d1.rc.vtxbufs[0] ) return false;
    d1.rc.vtxbufs[0]->update( 0, 0, vertices );

    // create index buffer
    UInt16 indices[] = { 0, 1, 3, 2 };
    IdxBufDesc ibd = { 4, false, false, false };
    d1.rc.idxbuf.attach( rndr.createIdxBuf( ibd ) );
    if( !d1.rc.idxbuf ) return false;
    d1.rc.idxbuf->update( 0, 0, indices );

    size_t gppidx = d1.rc.gpuProgram->getParameterIndex( "transform" );
    if( GpuProgram::PARAMETER_NOT_FOUND == gppidx ) return false;

    // create transformation matrix, set to identity
    Matrix44f m;
    d1.gpps.resize( d1.rc.gpuProgram->getNumParameters() );
    d1.gpps[gppidx].attach( createPrivateGpuProgramParam( sizeof(m) ) );
    m.identity();
    d1.gpps[gppidx]->set( m, sizeof(m) );

    // setup draw parameters
    d1.prim = TRIANGLE_LIST;
    d1.numvtx = 4;
    d1.minvtxidx = 0;
    d1.startvtx = 0;
    d1.numidx = 3;
    d1.startidx = 0;

    // make a clone of the whole drawable
    d2 = d1;

    // modify d2's transformation (should not affect d1)
    m.translate( -1.0f, -1.0f, 0.0f );
    d2.gpps[gppidx]->set( m, sizeof(m) );

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
