#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

GpuResourceDatabase * db    = NULL;
ModelResource *       model = 0;
AutoRef<Texture>      tex[2];

static const char * hlslvscode = "uniform float4x4 pvw; \n"
                                 "struct VSOUTPUT \n"
                                 "{ \n"
                                 "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
                                 "   float2 texcoords : TEXCOORD; \n"
                                 "}; \n"
                                 "struct VSINPUT \n"
                                 "{ \n"
                                 "   float4 position  : POSITION; \n"
                                 "   float2 texcoords : TEXCOORD; \n"
                                 "}; \n"
                                 "VSOUTPUT main( in VSINPUT i ) { \n"
                                 "   VSOUTPUT o; \n"
                                 "   o.hpos      = mul( pvw, i.position ); \n"
                                 "   o.texcoords = i.texcoords; \n"
                                 "   return o; \n"
                                 "}";

static const char * hlslpscode = "sampler s0; \n"
                                 "struct VSOUTPUT \n"
                                 "{ \n"
                                 "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
                                 "   float2 texcoords : TEXCOORD; \n"
                                 "}; \n"
                                 "float4 main( in VSOUTPUT i ) : COLOR0 { \n"
                                 "   float4  tex  = tex2D( s0, i.texcoords ); \n"
                                 "   return tex; \n"
                                 "}";

static const char * glslvscode = "in vec4 i_Pos0; \n"
                                 "in vec2 i_TexCoord0; \n"
                                 "uniform mat4 pvw; \n"
                                 "varying vec2 texcoords; \n"
                                 "void main() { \n"
                                 "   gl_Position = pvw * i_Pos0; \n"
                                 "   texcoords   = i_TexCoord0.xy; \n"
                                 "}";

static const char * glslpscode = "uniform sampler2D t0; \n"
                                 "varying vec2 texcoords; \n"
                                 "void main() { \n"
                                 "   vec4  tex    = texture2D( t0, texcoords ); \n"
                                 "   gl_FragColor = tex; \n"
                                 "}";

void initEffectDesc(EffectResourceDesc & ed) {
    ed.uniforms["MATRIX_PVW"];
    ed.textures["ALBEDO_TEXTURE"];
    ed.attributes["POSITION"];
    ed.attributes["TEXCOORD"];

    ed.gpuprograms["glsl"].gpd.lang                  = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.shaderModels          = ShaderModel::GLSL_1_10;
    ed.gpuprograms["glsl"].gpd.vs.source             = glslvscode;
    ed.gpuprograms["glsl"].gpd.ps.source             = glslpscode;
    ed.gpuprograms["glsl"].uniforms["pvw"]           = "MATRIX_PVW";
    ed.gpuprograms["glsl"].textures["t0"]            = "ALBEDO_TEXTURE";
    ed.gpuprograms["glsl"].attributes["i_Pos0"]      = "POSITION";
    ed.gpuprograms["glsl"].attributes["i_TexCoord0"] = "TEXCOORD";

    ed.gpuprograms["hlsl"].gpd.lang                = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl"].gpd.shaderModels        = ShaderModel::SM_2_0;
    ed.gpuprograms["hlsl"].gpd.vs.source           = hlslvscode;
    ed.gpuprograms["hlsl"].gpd.vs.entry            = "main";
    ed.gpuprograms["hlsl"].gpd.ps.source           = hlslpscode;
    ed.gpuprograms["hlsl"].gpd.ps.entry            = "main";
    ed.gpuprograms["hlsl"].uniforms["pvw"]         = "MATRIX_PVW";
    ed.gpuprograms["hlsl"].textures["s0"]          = "ALBEDO_TEXTURE";
    ed.gpuprograms["hlsl"].attributes["POSITION0"] = "POSITION";
    ed.gpuprograms["hlsl"].attributes["TEXCOORD0"] = "TEXCOORD";

    ed.techniques.resize(2);
    ed.techniques[0].passes.resize(1);
    ed.techniques[0].passes[0].gpuprogram = "glsl";
    ed.techniques[1].passes.resize(1);
    ed.techniques[1].passes[0].gpuprogram = "hlsl";
}

void initMeshDesc(MeshResourceDesc & md) {
    struct Vertex {
        float x, y, u, v;
    };

    static const Vertex vertices[] = {
        {-1.0f, 1.0f, 0.0f, 0.0f},
        {-1.0f, -1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 0.0f},
        {1.0f, -1.0f, 1.0f, 1.0f},
    };

    static const uint16 indices[] = {0, 1, 2, 2, 1, 3};

    md.prim        = PrimitiveType::TRIANGLE_LIST;
    md.numvtx      = GN_ARRAY_COUNT(vertices);
    md.numidx      = GN_ARRAY_COUNT(indices);
    md.vtxfmt      = MeshVertexFormat::XY_UV();
    md.vertices[0] = (void *) vertices;
    md.indices     = (void *) indices;
}

bool init(Gpu & g) {
    db = new GpuResourceDatabase(g);

    EffectResourceDesc ed;
    initEffectDesc(ed);
    AutoRef<EffectResource> e = db->createResource<EffectResource>("e0");
    if (!e || !e->reset(&ed)) return false;

    MeshResourceDesc med;
    initMeshDesc(med);
    AutoRef<MeshResource> mesh = db->createResource<MeshResource>("m0");
    if (!mesh || !mesh->reset(&med)) return false;

    ModelResourceDesc mod;
    mod.effect                                  = "e0";
    mod.mesh                                    = "m0";
    mod.textures["ALBEDO_TEXTURE"].resourceName = "media::/texture/rabit.png";
    mod.uniforms["MATRIX_PVW"].size             = sizeof(Matrix44f);

    model = db->createResource<ModelResource>("m0").detach();
    if (0 == model) return false;

    if (!model->reset(&mod)) return false;

    tex[0].attach(loadTextureFromFile(db->getGpu(), "media::/texture/rabit.png"));
    tex[1].attach(loadTextureFromFile(db->getGpu(), "media::/texture/earth.jpg"));

    // success
    return true;
}

void quit(Gpu &) {
    tex[0].clear();
    tex[1].clear();
    safeDecref(model);
    safeDelete(db);
}

void update(Input & in) {
    KeyEvent k = in.popLastKeyEvent();

    if (KeyCode::SPACEBAR == k.code() && k.status.down) {
        static int i = 0;
        i            = (i + 1) % 2;

        AutoRef<TextureResource> t(db->findResource<TextureResource>("media::/texture/rabit.png"));
        if (t) t->setTexture(tex[i]);
    }
}

void draw(Gpu &) {
    AutoRef<UniformResource> u(model->uniformResource("MATRIX_PVW"));
    if (u) u->uniform()->update(Matrix44f::sIdentity());

    model->draw();
}

int run(Gpu & gpu) {
    if (!init(gpu)) {
        quit(gpu);
        return -1;
    }

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel(Logger::VERBOSE); // enable FPS logger

    while (gogogo && gpu.getRenderWindow().runUntilNoNewEvents(false)) {

        Input & in = gInput;

        in.processInputEvents();

        if (in.getKeyStatus(KeyCode::ESCAPE).down) { gogogo = false; }

        update(in);

        gpu.clearScreen(Vector4f(0, 0.5f, 0.5f, 1.0f));
        draw(gpu);
        gpu.present();

        fps.onFrame();
    }

    quit(gpu);

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

    Gpu * r = createGpu(cmdargs.rendererOptions, cmdargs.useMultiThreadGpu ? GPU_CREATION_MULTIPLE_THREADS : 0);
    if (NULL == r) return -1;

    InputInitiator ii(*r);

    int result = run(*r);

    deleteGpu(r);

    return result;
}
