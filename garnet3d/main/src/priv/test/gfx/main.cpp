#include "pch.h"
#if 0
using namespace GN;
using namespace GN::gfx;
using namespace GN::app;
using namespace GN::util;
using namespace GN::scene;

bool gAnimation = true;

class TestScene
{
    SampleApp & app;
    
    UInt32 tex0;

    UInt32 eff0;

    Scene scene;
    Actor box;

    Matrix44f world, view, proj;

    ArcBall arcball;

public:

    TestScene( SampleApp & a ) : app(a), box(scene) {}

    ~TestScene() { quit(); }

    bool init()
    {
        scene::ResourceManager & rm = gSceneResMgr;

        // load box
        if( !loadFromXmlFile( box, "media::cube/cube.actor.xml" ) ) return false;

        // load texture
        tex0 = rm.getResourceId( "media::texture/rabit.png" );
        if( 0 == tex0 ) return false;

        // load effect
        eff0 = rm.getResourceId( "media::effect/sprite.xml" );
        if( 0 == eff0 ) return false;

        // initialize matrices
        float r = box.getBoundingSphere().radius * 1.0f;
        world.identity();
        view.lookAtRh( Vector3f(r,r,r), Vector3f(0,0,0), Vector3f(0,1,0) );
        gRenderer.composePerspectiveMatrixRh( proj, 1.0f, 4.0f/3.0f, r/100.0f, r*10.0f );

        // initialize arcball.
        arcball.setHandness( util::RIGHT_HAND );
        arcball.setViewMatrix( view );
        const DispDesc & dd = gRenderer.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );
        arcball.connectToInput();

        // initialize scene
        scene.setProj( proj );
        scene.setView( view );

        // success
        return true;
    }

    void quit()
    {
    }

    void update()
    {
        scene::ResourceManager & rm = gSceneResMgr;

        // update box rotation
        box.setRotation( arcball.getRotation() );

        // update color
        static int r = 0; static int rr = 1;
        static int g = 80; static int gg = 1;
        static int b = 160; static int bb = 1;
        r += rr; if( 0 == r || 255 == r ) rr = -rr;
        g += gg; if( 0 == g || 255 == g ) gg = -gg;
        b += bb; if( 0 == b || 255 == b ) bb = -bb;
        GN::gfx::UniformValue u;
        u.setV( GN::Vector4f( r/255.0f, g/255.0f, b/255.0f, 1.0f ) );
        rm.getResourceT<Effect>(eff0)->setUniformByName( "color", u );
    }

    void draw()
    {
        Renderer & r = gRenderer;
        scene::ResourceManager & rm = gSceneResMgr;

        // quad 1
        gQuadRenderer.drawBegin( rm.getResourceT<Texture>(tex0), QuadRenderer::OPT_DEPTH_WRITE );
        gQuadRenderer.drawTextured( 0, 0, 0, 0.5f, 0.5f );
        gQuadRenderer.drawEnd();

        // quad 2,3
        gQuadRenderer.drawSolidBegin();
        gQuadRenderer.drawSolid( GN_RGBA32(0,255,0,255), 0, 0.5, 0.0, 1.0, 0.5 );
        gQuadRenderer.drawSolid( GN_RGBA32(255,0,0,255), 0, 0.0, 0.5, 0.5, 1.0 );
        gQuadRenderer.drawEnd();

        // quad 4
        RendererContext ctx;
        ctx.clearToNull();
        Effect * eff = rm.getResourceT<Effect>( eff0 );
        for( size_t i = 0; i < eff->getNumPasses(); ++i )
        {
            eff->passBegin( ctx, i );
            eff->commitChanges();
            r.setContext( ctx );
            gQuadRenderer.drawSingleTexturedQuad( 0, QuadRenderer::OPT_USER_CONTEXT, 0, 0.5, 0.5, 1.0, 1.0 );
            eff->passEnd();
        }

        // draw box
        box.draw();

        /* a wireframe box
        {
            static const float E = 80.0f;
            static float lines[] =
            {
                -E, -E, -E,  E, -E, -E,
                 E, -E, -E,  E,  E, -E,
                 E,  E, -E, -E,  E, -E,
                -E,  E, -E, -E, -E, -E,

				-E, -E, -E, -E, -E,  E,
                 E, -E, -E,  E, -E,  E,
                 E,  E, -E,  E,  E,  E,
                -E,  E, -E, -E,  E,  E,

                -E, -E,  E,  E, -E,  E,
                 E, -E,  E,  E,  E,  E,
                 E,  E,  E, -E,  E,  E,
                -E,  E,  E, -E, -E,  E,
			};
            static size_t count = sizeof(lines)/sizeof(float)/6;
            
            r.drawLines(
                0,
                lines,
                sizeof(float)*3,
                count,
                GN_RGBA(255,255,0,255),
                world, view, proj );
        }//*/
    }
};

///
/// GFX module test application
///
class GfxTestApp : public SampleApp
{
    TestScene * mScene;
public:

    GfxTestApp() : mScene(0) {}

    bool onRendererCreate()
    {
        mScene = new TestScene(*this);
        return mScene->init();
    }

    void onRendererDestroy()
    {
        safeDelete(mScene);
    }

    void onUpdate()
    {
        if( gAnimation ) mScene->update();
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        
        r.clearScreen( Vector4f(0,0,0,1) ); // clear to pure black

        // draw scene
        GN_ASSERT( mScene );
        mScene->draw();

        // draw mouse position on screen
        StrW mousePos;
        int x = 1, y = 1;
        gInput.getMousePosition( x, y );
        mousePos.format( L"Mouse Position : %d, %d", x, y );
        getFontRenderer().drawText( mousePos.cptr(), 0, 100 );
    }
};

int main( int argc, const char * argv[] )
{
    GfxTestApp app;
    return app.run( argc, argv );
}

#else

#include "garnet/GNengine.h"
using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

struct Vertex
{
    Vector4f pos;
};

class VtxBufLoader : public GraphicsResourceLoader
{
    virtual bool load( void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    bool decompress( void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
    {
        GN_ASSERT( 0 == inbuf && 0 == inbytes );
        Vertex * data = new Vertex[3];
        data[0].pos.set( 0, 0, 0, 0 );
        data[0].pos.set( 1, 0, 0, 0 );
        data[0].pos.set( 1, 1, 0, 0 );
        outbuf = data;
        outbytes = sizeof(Vertex) * 3;
        return true;
    }

    virtual bool copy( GraphicsResource & gfxres, const void * inbuf, size_t inbytes, int )
    {
        VtxBuf * vb = gfxres.vtxbuf;
        void * data = vb->lock( 0, 0, LOCK_DISCARD );
        if( 0 == data ) return false;
        memcpy( data, inbuf, inbytes );
        vb->unlock();
        return true;
    }

    virtual void freebuf( void * inbuf, size_t )
    {
        safeDeleteArray( inbuf );
    }
};

class IdxBufLoader : public GraphicsResourceLoader
{
    virtual bool load( void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    bool decompress( void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
    {
        GN_ASSERT( 0 == inbuf && 0 == inbytes );
        UInt16 * data = new UInt16[3];
        data[0] = 0;
        data[0] = 1;
        data[0] = 2;
        outbuf = data;
        outbytes = sizeof(UInt16) * 3;
        return true;
    }

    virtual bool copy( GraphicsResource & gfxres, const void * inbuf, size_t inbytes, int )
    {
        IdxBuf * ib = gfxres.idxbuf;
        void * data = ib->lock( 0, 0, LOCK_DISCARD );
        if( 0 == data ) return false;
        memcpy( data, inbuf, inbytes );
        ib->unlock();
        return true;
    }

    virtual void freebuf( void * inbuf, size_t )
    {
        safeDeleteArray( inbuf );
    }
};

GraphicsResourceId vs, vb, ib;

const char * vscode =
    "uniform float4x4 pvw;\n"
    "float4 main( in float4 ipos: POSITION ) : POSITION \n"
    "{ \n"
    "    return mul( pvw, ipos ); \n"
    "}";

bool init( RenderEngine & engine )
{
    // create vertex shader
    GraphicsResourceDesc vsdesc;
    vsdesc.name = "vs1";
    vsdesc.type = GRT_SHADER;
    vsdesc.sd.type = SHADER_VS;
    vsdesc.sd.lang = LANG_D3D_HLSL;
    vsdesc.sd.code = vscode;
    vsdesc.sd.vtxfmt.clear();
    vsdesc.sd.vtxfmt.addAttrib( 0, 0, VTXSEM_POS0, FMT_FLOAT4 );
    vs = engine.allocResource( vsdesc );
    if( 0 == vs ) return 0;

    // create vertex buffer
    GraphicsResourceDesc vbdesc;
    vbdesc.name = "vb1";
    vbdesc.type = GRT_VTXBUF;
    vbdesc.vd.bytes = 3 * sizeof(Vertex);
    vbdesc.vd.dynamic = false;
    vbdesc.vd.readback = false;
    vb = engine.allocResource( vbdesc );
    if( 0 == vb ) return false;
    AutoRef<VtxBufLoader> vbloader( new VtxBufLoader );
    engine.updateResource( vb, 0, vbloader );

    // create index buffer
    GraphicsResourceDesc ibdesc;
    ibdesc.name = "ib1";
    ibdesc.type = GRT_IDXBUF;
    ibdesc.id.numidx = 3;
    ibdesc.id.dynamic = false;
    ibdesc.id.readback = false;
    ib = engine.allocResource( ibdesc );
    if( 0 == ib ) return false;
    AutoRef<IdxBufLoader> ibloader( new IdxBufLoader );
    engine.updateResource( ib, 0, ibloader );

    return true;
}

void quit()
{
}

void run( RenderEngine & engine )
{
    __try
    {
        if( !init( engine ) ) return;
        while( 1 )
        {
            gInput.processInputEvents();

            if( gInput.getKeyStatus( KEY_ESCAPE ).down )
            {
                break;
            }
            
            engine.frameBegin();

            engine.clearScreen( Vector4f(0,0,1,0) );

            engine.frameEnd();
        }
    }
    __finally
    {
        quit();
    }
}

struct InputInitiator
{
    InputInitiator()
    {
        createInputSystem( API_NATIVE );
    }

    ~InputInitiator()
    {
        if( gInputPtr ) delete gInputPtr;
    }
};

int main()
{
    InputInitiator input;

    RenderEngine engine;

    UInt32 MB = 1024 * 1024;
    RenderEngineInitParameters reip = { 32*MB, 32*MB, 4*MB };

    RendererOptions ro;

    if( !engine.init(reip) ) return -1;

    if( !engine.resetRenderer( API_D3D9, ro ) ) return -1;

    run( engine );

    

    // success
    return 0;
}

#endif
