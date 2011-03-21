#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::engine;
using namespace GN::util;

namespace GN { namespace engine
{
    class SkinnedMesh : public Entity
    {
    public:

         SkinnedMesh();
        ~SkinnedMesh();

        /// get the spacial component
        const SpacialComponent & spacial() const { return mSpacial; }

        /// get the spacial component
        SpacialComponent & spacial() { return mSpacial; }

        struct Bone
        {
            Vector4f translation;
            Quaternionf rotation;
            int id;
            int parent;
        };

        bool init()
        {

        }

    private:

        StackRefCounter<SpacialComponent> mSpacial;
        AutoRef<MeshResource> mMesh;
        AutoRef<EffectResource> mEffect;
    };
}}

//
//
// -----------------------------------------------------------------------------
GN::engine::SkinnedMesh::SkinnedMesh()
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::SkinnedMesh::~SkinnedMesh()
{
}


//
//
// -----------------------------------------------------------------------------
bool init()
{
    // success
    return true;
}

void quit()
{
}

void draw()
{
}

bool run()
{
    if( !init() ) { quit(); return false; }

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel( Logger::VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        getGpu()->processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KeyCode::ESCAPE ).down )
        {
            gogogo = false;
        }

        getGpu()->clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw();
        getGpu()->present();

        fps.onFrame();
    }

    quit();

    return true;
}

int main( int argc, const char * argv[] )
{
    enableCRTMemoryCheck();

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

    bool noerror = engine::initialize();

    if( noerror )
    {
        noerror = engine::inputInitialize( input::InputAPI::NATIVE );
    }

    if( noerror )
    {
        // setup graphics options
        engine::GfxInitOptions gio;
        gio.gpuOptions = cmdargs.rendererOptions;
        gio.useMultithreadGpu = cmdargs.useMultiThreadGpu;
        gio.defaultNonAsciiFont.fontname = "font::/simsun.ttc";
        gio.defaultNonAsciiFont.width = 16;
        gio.defaultNonAsciiFont.height = 16;
        gio.defaultNonAsciiFont.quality = gfx::FontFaceDesc::ANTIALIASED;
        gio.defaultAsciiFont.fontname = "font::/ltype.ttf";
        gio.defaultAsciiFont.width = 16;
        gio.defaultAsciiFont.height = 16;
        gio.defaultAsciiFont.quality = gfx::FontFaceDesc::ANTIALIASED;

        noerror = engine::gfxInitialize( gio );
    }

    if( noerror )
    {
        noerror = run();
    }

    engine::shutdown();

    return noerror ? 0 : -1;
}
