#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;
using namespace GN::input;

static GN::Logger * sLogger = GN::GetLogger( "GN.test.RenderToTexture" );

float RT_WIDTH  = 256.0f;
float RT_HEIGHT = 256.0f;

class RenderToTexture
{
    GpuResourceDatabase & db;
    SpriteRenderer        sr;
    AutoRef<Texture>      c0, ds;
    AutoRef<Texture>      tex0, tex1;
    GpuContext            context;

    // box scene data
    struct BoxVert
    {
        float x, y, z, nx, ny, nz, u, v;
    };
    Matrix44f           proj, view;
    SimpleDiffuseModel  model;
    Clock               timer;

public:

    RenderToTexture( GpuResourceDatabase & db_ )
        : db(db_)
        , sr(db_.GetGpu())
        , model(db_)
    {
    }

    ~RenderToTexture()
    {
        Quit();
    }

    bool Init()
    {
        // create sprite renderer
        if( !sr.Init() ) return false;

        Gpu & gpu = db.GetGpu();

        // create render targets
        c0.Attach( gpu.Create2DTexture( (UInt32)RT_WIDTH, (UInt32)RT_HEIGHT, 1, ColorFormat::RGBA32, TextureUsage::COLOR_RENDER_TARGET ) );
        if( !c0 )
        {
            GN_ERROR(sLogger)( "Current graphics hardware does not support render-to-texture at all." );
            return false;
        }

        ds.Attach( gpu.Create2DTexture( (UInt32)RT_WIDTH, (UInt32)RT_HEIGHT, 1, ColorFormat::UNKNOWN, TextureUsage::DEPTH_RENDER_TARGET ) );
        if( !ds )
        {
            GN_WARN(sLogger)( "Current graphics hardware does not support depth-texture. All tests related depth-texture are disabled." );
        }

        // load textures
        tex0.Attach( loadTextureFromFile( gpu, "media::texture/rabit.png" ) );
        tex1.Attach( loadTextureFromFile( gpu, "media::texture/earth.jpg" ) );
        if( 0 == tex0 || 0 == tex1 ) return false;

        // create box mesh
        float edge = 200.0f;
        BoxVert vertices[24];
        UInt16  indices[36];
        CreateBox(
            edge, edge, edge,
            &vertices[0].x, sizeof(BoxVert),
            &vertices[0].u, sizeof(BoxVert),
            &vertices[0].nx, sizeof(BoxVert),
            0, 0, // tangent
            0, 0, // binormal
            indices, 0 );
        MeshResourceDesc md;
        md.vtxfmt      = VertexFormat::XYZ_NORM_UV();
        md.prim        = PrimitiveType::TRIANGLE_LIST;
        md.numvtx      = 24;
        md.numidx      = 36;
        md.vertices[0] = vertices;
        md.indices     = indices;
        AutoRef<MeshResource> boxmesh = db.CreateResource<MeshResource>( NULL );
        if( !boxmesh->Reset( &md ) ) return false;

        // setup transformation matrices
        view.LookAtRh( Vector3f(200,200,200), Vector3f(0,0,0), Vector3f(0,1,0) );
        gpu.ComposePerspectiveMatrix( proj, 1.0f, 4.0f/3.0f, 80.0f, 600.0f );

        // initialize the model
        if( !model.Init() ) return false;
        model.modelResource().SetMeshResource( boxmesh );
        model.setLightPos( Vector4f(200,200,200,1) ); // light is at eye position.
        model.setAlbedoTexture( tex1 );

        return true;
    }

    void Quit()
    {
        model.Quit();
        c0.Clear();
        ds.Clear();
        tex0.Clear();
        tex1.Clear();
        context.Clear();
        sr.Quit();
    }

    void drawBox( float speed )
    {
        float angle = speed * fmod( timer.GetTimef(), GN_TWO_PI );

        Matrix44f world;
        world.RotateY( angle );

        model.setTransformation( proj, view, world );

        model.modelResource().Draw();
    }

    void drawToColorRenderTarget( Texture * tex )
    {
        Gpu & gpu = db.GetGpu();
        context.colortargets.Resize( 1 );
        context.colortargets[0].texture = c0;
        gpu.BindContext( context );
        gpu.ClearScreen( Vector4f(0, 0, 1, 1 ) ); // clear to green
        sr.drawSingleTexturedSprite( tex, GN::gfx::SpriteRenderer::DEFAULT_OPTIONS, 0, 0, RT_WIDTH, RT_HEIGHT );
    }

    void drawToDepthTexture()
    {
        Gpu & gpu = db.GetGpu();
        context.colortargets.Clear();
        context.depthstencil.texture = ds;
        gpu.BindContext( context );
        gpu.ClearScreen();

        drawBox( 1.0f );
    }

    void drawToBothColorAndDepthTexture()
    {
        Gpu & gpu = db.GetGpu();
        context.colortargets.Resize( 1 );
        context.colortargets[0].texture = c0;
        context.depthstencil.texture = ds;
        gpu.BindContext( context );
        gpu.ClearScreen( Vector4f(0, 0, 1, 1 ) ); // clear to green

        drawBox( -1.0f );
    }

    void drawToBackBuffer( Texture * tex, float x, float y )
    {
        Gpu & gpu = db.GetGpu();
        context.colortargets.Clear();
        context.depthstencil.Clear();
        gpu.BindContext( context );
        sr.drawSingleTexturedSprite( tex, GN::gfx::SpriteRenderer::DEFAULT_OPTIONS, x, y, RT_WIDTH, RT_HEIGHT );
    }

    void render()
    {
        drawToColorRenderTarget( tex0 );
        drawToBackBuffer( c0, 0, 0 );

        drawToDepthTexture();
        drawToBackBuffer( ds, RT_WIDTH, 0 );

        drawToBothColorAndDepthTexture();
        drawToBackBuffer( c0, 0, RT_HEIGHT );
        drawToBackBuffer( ds, RT_WIDTH, RT_HEIGHT );
    }
};

int Run( Gpu & gpu )
{
    GpuResourceDatabase db(gpu);

    RenderToTexture scene( db );

    if( !scene.Init() ) return -1;

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

        gpu.ClearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        scene.render();
        gpu.Present();

        fps.onFrame();
    }

    return 0;
}

struct InputInitiator
{
    InputInitiator( Gpu & r )
    {
        InitializeInputSystem( InputAPI::NATIVE );
        const DispDesc & dd = r.GetDispDesc();
        gInput.AttachToWindow( dd.displayHandle, dd.windowHandle );
    }

    ~InputInitiator()
    {
        ShutdownInputSystem();
    }
};

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

    cmdargs.rendererOptions.windowedWidth = (UInt32)RT_WIDTH * 2;
    cmdargs.rendererOptions.windowedHeight = (UInt32)RT_HEIGHT * 2;

    Gpu * r;
    if( cmdargs.useMultiThreadGpu )
        r = CreateMultiThreadGpu( cmdargs.rendererOptions );
    else
        r = CreateSingleThreadGpu( cmdargs.rendererOptions );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = Run( *r );

    DeleteGpu( r );

    return result;
}

