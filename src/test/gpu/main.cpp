#include "pch.h"

//*
using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

bool       blankScreen = false;
GpuContext rc;

const char * hlsl_vscode = "uniform float4x4 transform; \n"
                           "struct VSOUT { float4 pos : POSITION0; float2 uv : TEXCOORD; }; \n"
                           "VSOUT main( in float4 pos : POSITION ) { \n"
                           "   VSOUT o; \n"
                           "   o.pos = mul( transform, pos ); \n"
                           "   o.uv  = pos.xy; \n"
                           "   return o; \n"
                           "}";

const char * hlsl_pscode = "sampler t0; \n"
                           "struct VSOUT { float4 pos : POSITION0; float2 uv : TEXCOORD; }; \n"
                           "float4 main( in VSOUT i ) : COLOR0 { \n"
                           "   return tex2D( t0, i.uv ); \n"
                           "}";

const char * glsl_vscode = "in vec4 i_Position0;\n"
                           "varying vec2 texcoords; \n"
                           "uniform mat4 transform; \n"
                           "void main() { \n"
                           "   gl_Position = transform * i_Position0; \n"
                           "   texcoords.xy = i_Position0.xy; \n"
                           "}";

const char * glsl_pscode = "uniform sampler2D t0; \n"
                           "varying vec2 texcoords; \n"
                           "void main() { \n"
                           "   gl_FragColor = texture2D( t0, texcoords ); \n"
                           "}";

bool init(Gpu & gpu) {
    if (blankScreen) return true;

    rc.clear();

    // create GPU program
    GpuProgramDesc gpd("test-gpu");
    if (GpuAPI::OGL == gpu.getOptions().api) {
        gpd.lang         = GpuProgramLanguage::GLSL;
        gpd.shaderModels = ShaderModel::GLSL_1_10;
        gpd.vs.source    = glsl_vscode;
        gpd.ps.source    = glsl_pscode;
    } else {
        gpd.lang         = GpuProgramLanguage::HLSL9;
        gpd.shaderModels = ShaderModel::SM_2_0;
        gpd.vs.source    = hlsl_vscode;
        gpd.ps.source    = hlsl_pscode;
        gpd.vs.entry     = "main";
        gpd.ps.entry     = "main";
    }
    rc.gpuProgram.attach(gpu.createGpuProgram(gpd));
    if (!rc.gpuProgram) return false;

    // create uniform
    rc.uniforms.resize(1);
    rc.uniforms[0].attach(gpu.createUniform(sizeof(Matrix44f)));
    if (!rc.uniforms[0]) return false;

    // setup vertex format
    rc.vtxbind.resize(1);
    rc.vtxbind[0].attribute = 0; // bind to the first GPU program attribute
    rc.vtxbind[0].format    = PixelFormat::FLOAT4();
    rc.vtxbind[0].offset    = 0;
    rc.vtxbind[0].stream    = 0;

    // create texture
    rc.textures[0].texture.attach(loadTextureFromFile(gpu, "media::texture\\earth.jpg"));

    // create vertex buffer
    static float vertices[] = {
        0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1,
    };
    VtxBufDesc vbd = {
        sizeof(vertices),
        false,
    };
    rc.vtxbufs[0].vtxbuf.attach(gpu.createVtxBuf(vbd));
    if (NULL == rc.vtxbufs[0].vtxbuf) return false;
    rc.vtxbufs[0].vtxbuf->update(0, 0, vertices);

    // create index buffer
    uint16     indices[] = {0, 1, 3, 2};
    IdxBufDesc ibd       = {4, false, false};
    rc.idxbuf.attach(gpu.createIdxBuf(ibd));
    if (!rc.idxbuf) return false;
    rc.idxbuf->update(0, 0, indices);

    return true;
}

void quit(Gpu &) { rc.clear(); }

void draw(Gpu & r) {
    if (blankScreen) return;

    Matrix44f m;

    // DRAW_UP: triangle at left top corner
    static float vertices[] = {
        0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1,
    };
    m.translate(-1.0f, -0.0f, 0);
    rc.uniforms[0]->update(m);
    r.bindContext(rc);
    r.drawUp(PrimitiveType::TRIANGLE_LIST, 3, vertices, 4 * sizeof(float));

    // DRAW_INDEXED_UP : triangle at left bottom
    static uint16 indices[] = {0, 1, 3};
    m.translate(-1.0f, -1.0f, 0);
    rc.uniforms[0]->update(m);
    r.bindContext(rc);
    r.drawIndexedUp(PrimitiveType::TRIANGLE_STRIP, 3, 4, vertices, 4 * sizeof(float), indices);

    // DRAW: triangle at right top corner
    m.identity();
    rc.uniforms[0]->update(m);
    r.bindContext(rc);
    r.draw(PrimitiveType::TRIANGLE_LIST, 3, 0);

    // DRAW_INDEXED : quad at right bottom corner
    m.translate(0.5f, -1.5f, 0);
    rc.uniforms[0]->update(m);
    r.bindContext(rc);
    r.drawIndexed(PrimitiveType::TRIANGLE_STRIP, 4, 0, 0, 4, 0);
}

struct InputInitiator {
    InputInitiator(Gpu & r) {
        initializeInputSystem(InputAPI::NATIVE);
        const DispDesc & dd = r.getDispDesc();
        gInput.attachToWindow(dd.displayHandle, dd.windowHandle);
    }

    ~InputInitiator() { shutdownInputSystem(); }
};

int run(Gpu & gpu) {
    InputInitiator ii(gpu);

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

        if (in.getKeyStatus(KeyCode::ESCAPE).down || in.getKeyStatus(KeyCode::XB360_A).down) { gogogo = false; }

        gpu.clearScreen(Vector4f(0, 0.5f, 0.5f, 1.0f));
        draw(gpu);
        gpu.present();

        fps.onFrame();
    }

    quit(gpu);

    return 0;
}

void showHelp(CommandLineArguments & ca) {
    StrA executableName = fs::baseName(ca.applicationName) + fs::extName(ca.applicationName);
    GN_INFO(ca.logger)("Usage: %s [options]\n", executableName.rawptr());
    ca.showStandardCommandLineOptions();
    GN_INFO(ca.logger)("  -b                       Draw blank screen only. Do not create any graphics resources.\n");
}

int main(int argc, const char * argv[]) {
    enableCRTMemoryCheck();

    CommandLineArguments cmdargs(argc, argv);
    switch (cmdargs.status) {
    case CommandLineArguments::SHOW_HELP:
        showHelp(cmdargs);
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

    for (size_t i = 0; i < cmdargs.extraArgc; ++i) {
        if (0 == str::compareI("-b", cmdargs.extraArgv[i])) {
            blankScreen = true;
        } else {
            GN_ERROR(cmdargs.logger)("Invalid command line argument: %s", cmdargs.extraArgv[i]);
            showHelp(cmdargs);
            return -1;
        }
    }

    Gpu * r = createGpu(cmdargs.rendererOptions, cmdargs.useMultiThreadGpu ? GPU_CREATION_MULTIPLE_THREADS : 0);
    // createOGLGpu( cmdargs.rendererOptions, cmdargs.useMultiThreadGpu ? GPU_CREATION_MULTIPLE_THREADS : 0 );
    // createD3DGpu( cmdargs.rendererOptions, cmdargs.useMultiThreadGpu ? GPU_CREATION_MULTIPLE_THREADS : 0 );
    if (nullptr == r) { return -1; }

    int result = run(*r);

    deleteGpu(r);

    return result;
}
/*/
int main()
{
    return 0;
}//*/
