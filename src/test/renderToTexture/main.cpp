#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;
using namespace GN::input;

static GN::Logger * sLogger = GN::getLogger("GN.test.RenderToTexture");

float RT_WIDTH  = 256.0f;
float RT_HEIGHT = 256.0f;

class RenderToTexture {
    GpuResourceDatabase & db;
    SpriteRenderer        sr;
    AutoRef<Texture>      c0, ds;
    AutoRef<Texture>      tex0, tex1;
    GpuContext            context;

    // box scene data
    struct BoxVert {
        float x, y, z, nx, ny, nz, u, v;
    };
    Matrix44f          proj, view;
    SimpleDiffuseModel model;
    Clock              timer;

public:
    RenderToTexture(GpuResourceDatabase & db_): db(db_), model(db_) {}

    ~RenderToTexture() { quit(); }

    bool init() {
        Gpu & gpu = db.getGpu();

        // create sprite renderer
        if (!sr.init(gpu)) return false;

        // create render targets
        c0.attach(gpu.create2DTexture(ColorFormat::RGBA8, (uint32) RT_WIDTH, (uint32) RT_HEIGHT, 1, TextureUsage::COLOR_RENDER_TARGET));
        if (!c0) {
            GN_ERROR(sLogger)("Current graphics hardware does not support render-to-texture at all.");
            return false;
        }

        ds.attach(gpu.create2DTexture(ColorFormat::RG_24_UNORM_8_UINT, (uint32) RT_WIDTH, (uint32) RT_HEIGHT, 1, TextureUsage::DEPTH_RENDER_TARGET));
        if (!ds) { GN_WARN(sLogger)("Current graphics hardware does not support depth-texture. All tests related depth-texture are disabled."); }

        // load textures
        tex0.attach(loadTextureFromFile(gpu, "media::texture/rabit.png"));
        tex1.attach(loadTextureFromFile(gpu, "media::texture/earth.jpg"));
        if (0 == tex0 || 0 == tex1) return false;

        // create box mesh
        float   edge = 200.0f;
        BoxVert vertices[24];
        uint16  indices[36];
        createBox(edge, edge, edge, &vertices[0].x, sizeof(BoxVert), &vertices[0].u, sizeof(BoxVert), &vertices[0].nx, sizeof(BoxVert), 0, 0, // tangent
                  0, 0,                                                                                                                       // binormal
                  indices, 0);
        MeshResourceDesc md;
        md.vtxfmt                     = MeshVertexFormat::XYZ_NORM_UV();
        md.prim                       = PrimitiveType::TRIANGLE_LIST;
        md.numvtx                     = 24;
        md.numidx                     = 36;
        md.vertices[0]                = vertices;
        md.indices                    = indices;
        AutoRef<MeshResource> boxmesh = db.createResource<MeshResource>(NULL);
        if (!boxmesh->reset(&md)) return false;

        // setup transformation matrices
        view.lookAtRh(Vector3f(200, 200, 200), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
        gpu.composePerspectiveMatrix(proj, 1.0f, 4.0f / 3.0f, 80.0f, 600.0f);

        // initialize the model
        if (!model.init()) return false;
        model.modelResource().setMeshResource(boxmesh);
        model.setLightPos(Vector4f(200, 200, 200, 1)); // light is at eye position.
        model.setAlbedoTexture(tex1);

        return true;
    }

    void quit() {
        model.quit();
        c0.clear();
        ds.clear();
        tex0.clear();
        tex1.clear();
        context.clear();
        sr.quit();
    }

    void drawBox(float speed) {
        float angle = speed * fmod((float) timer.seconds(), GN_TWO_PI);

        Matrix44f world;
        world.rotateY(angle);

        model.setTransform(proj, view, world);

        model.modelResource().draw();
    }

    void drawToColorRenderTarget(Texture * tex) {
        Gpu & gpu = db.getGpu();
        context.colortargets.resize(1);
        context.colortargets[0].texture = c0;
        gpu.bindContext(context);
        gpu.clearScreen(Vector4f(0, 0, 1, 1)); // clear to green
        sr.drawSingleTexturedSprite(tex, GN::gfx::SpriteRenderer::TRANSPARENT_2D_IMAGE, 0, 0, RT_WIDTH, RT_HEIGHT);
    }

    void drawToDepthTexture() {
        Gpu & gpu = db.getGpu();
        context.colortargets.clear();
        context.depthstencil.texture = ds;
        gpu.bindContext(context);
        gpu.clearScreen();

        drawBox(1.0f);
    }

    void drawToBothColorAndDepthTexture() {
        Gpu & gpu = db.getGpu();
        context.colortargets.resize(1);
        context.colortargets[0].texture = c0;
        context.depthstencil.texture    = ds;
        gpu.bindContext(context);
        gpu.clearScreen(Vector4f(0, 0, 1, 1)); // clear to green

        drawBox(-1.0f);
    }

    void drawToBackBuffer(Texture * tex, float x, float y) {
        Gpu & gpu = db.getGpu();
        context.colortargets.clear();
        context.depthstencil.clear();
        gpu.bindContext(context);
        sr.drawSingleTexturedSprite(tex, GN::gfx::SpriteRenderer::TRANSPARENT_2D_IMAGE, x, y, RT_WIDTH, RT_HEIGHT);
    }

    void render() {
        drawToColorRenderTarget(tex0);
        drawToBackBuffer(c0, 0, 0);

        drawToDepthTexture();
        drawToBackBuffer(ds, RT_WIDTH, 0);

        drawToBothColorAndDepthTexture();
        drawToBackBuffer(c0, 0, RT_HEIGHT);
        drawToBackBuffer(ds, RT_WIDTH, RT_HEIGHT);
    }
};

int run(Gpu & gpu) {
    GpuResourceDatabase db(gpu);

    RenderToTexture scene(db);

    if (!scene.init()) return -1;

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel(Logger::VERBOSE); // enable FPS logger

    while (gogogo && gpu.getRenderWindow().runUntilNoNewEvents(false)) {

        Input & in = gInput;

        in.processInputEvents();

        if (in.getKeyStatus(KeyCode::ESCAPE).down) { gogogo = false; }

        gpu.clearScreen(Vector4f(0, 0.5f, 0.5f, 1.0f));
        scene.render();
        gpu.present();

        fps.onFrame();
    }

    return 0;
}

struct InputInitiator {
    InputInitiator(Gpu & r) {
        initializeInputSystem(InputAPI::NATIVE);
        const DispDesc & dd = r.getDispDesc();
        gInput.attachToWindow(dd.displayHandle, dd.windowHandle);
    }

    ~InputInitiator() { shutdownInputSystem(); }
};

int main(int argc, const char * argv[]) {
    enableCRTMemoryCheck();

    CommandLineArguments cmdargs(argc, argv);
    switch (cmdargs.status) {
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

    cmdargs.rendererOptions.displayMode.width  = (uint32) RT_WIDTH * 2;
    cmdargs.rendererOptions.displayMode.height = (uint32) RT_HEIGHT * 2;

    Gpu * r = createGpu(cmdargs.rendererOptions, cmdargs.useMultiThreadGpu ? GPU_CREATION_MULTIPLE_THREADS : 0);
    if (NULL == r) return -1;

    InputInitiator ii(*r);

    int result = run(*r);

    deleteGpu(r);

    return result;
}
