#include "pch.h"
#include "gpures.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
using namespace GN::util;


GpuResourceDatabase * db = NULL;
GpuResourceHandle  model = 0;

bool init( Gpu & g )
{
    db = new GpuResourceDatabase( g );

    model = ModelResource::loadFromFile( *db, "media::model\\model1.xml" );

    // success
    return true;
}

void quit( Gpu & )
{
    safeDelete( db );
}

void draw( Gpu & )
{
    ModelResource * m = GpuResource::castTo<ModelResource>( db->getResource(model) );
    if( m )
    {
        //UniformResource * u = GpuResource::castTo<UniformResource>( db->getResource( m->getUniform( "pvw" ) ) );
        //if( u ) u->getUniform()->update( Matrix44f::sIdentity() );

        m->draw();
    }
}

int run( Gpu & gpu )
{
    if( !init( gpu ) ) { quit( gpu ); return -1; }

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel( Logger::VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        gpu.processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KeyCode::ESCAPE ).down )
        {
            gogogo = false;
        }

        gpu.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw( gpu );
        gpu.present();

        fps.onFrame();
    }

    quit( gpu );

    return 0;
}

struct InputInitiator
{
    InputInitiator( Gpu & r )
    {
        initializeInputSystem( InputAPI::NATIVE );
        const DispDesc & dd = r.getDispDesc();
        gInput.attachToWindow( dd.displayHandle, dd.windowHandle );
    }

    ~InputInitiator()
    {
        shutdownInputSystem();
    }
};

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

    Gpu * r;
    if( cmdargs.useMultiThreadGpu )
        r = createMultiThreadGpu( cmdargs.rendererOptions );
    else
        r = createSingleThreadGpu( cmdargs.rendererOptions );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteGpu( r );

    return result;
}
